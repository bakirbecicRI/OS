// implement fork from user space

#include <inc/string.h>
#include <inc/lib.h>

// PTE_COW marks copy-on-write page table entries.
// It is one of the bits explicitly allocated to user processes (PTE_AVAIL).
#define PTE_COW		0x800

//
// Custom page fault handler - if faulting page is copy-on-write,
// map in our own private writable copy.
//
static void
pgfault(struct UTrapframe *utf)
{
  pte_t pte;
  envid_t envid;

	void *addr = (void *) utf->utf_fault_va;
	uint32_t err = utf->utf_err;
	int ret;
  envid = sys_getenvid();
  pte = uvpt[PGNUM(addr)];

  if (!(pte & PTE_COW || !(err & FEC_WR))) {
      panic("pgfault: fault");
  }
	if ((ret = sys_page_alloc(envid, PFTEMP,
	                        PTE_U | PTE_W | PTE_P)) < 0) {
		panic("pgfault: sys_page_alloc failed: %e", ret);
	}

	memcpy(PFTEMP, ROUNDDOWN(addr, PGSIZE), PGSIZE);

	if ((ret = sys_page_map(envid, PFTEMP,
	                      envid, ROUNDDOWN(addr, PGSIZE),
	                      PTE_U | PTE_W | PTE_P)) < 0) {
		panic("pgfault: sys_page_map failed: %e", ret);
	}

	if ((ret = sys_page_unmap(envid, PFTEMP)) < 0) {
		panic("pgfault: sys_page_unmap failed: %e", ret);
	}

	// Check that the faulting access was (1) a write, and (2) to a
	// copy-on-write page.  If not, panic.
	// Hint:
	//   Use the read-only page table mappings at uvpt
	//   (see <inc/memlayout.h>).

	// LAB 4: Your code here.

	// Allocate a new page, map it at a temporary location (PFTEMP),
	// copy the data from the old page to the new page, then move the new
	// page to the old page's address.
	// Hint:
	//   You should make three system calls.

	// LAB 4: Your code here.

}

//
// Map our virtual page pn (address pn*PGSIZE) into the target envid
// at the same virtual address.  If the page is writable or copy-on-write,
// the new mapping must be created copy-on-write, and then our mapping must be
// marked copy-on-write as well.  (Exercise: Why do we need to mark ours
// copy-on-write again if it was already copy-on-write at the beginning of
// this function?)
//
// Returns: 0 on success, < 0 on error.
// It is also OK to panic on error.
//
static int
duppage(envid_t envid, unsigned pn)
{
	envid_t parent = sys_getenvid();
	uintptr_t va = pn * PGSIZE;
	int ret;

	if ((uvpt[pn] & PTE_W) == PTE_W || (uvpt[pn] & PTE_COW) == PTE_COW) {

		if ((ret = sys_page_map(parent, (void*)va, envid, (void*)va, PTE_COW | PTE_U | PTE_P)) < 0) {
			panic("duppage: %e", ret);
		}

		if ((ret = sys_page_map(parent, (void*)va, parent, (void*)va, PTE_COW | PTE_U | PTE_P)) < 0) {
			panic("duppage: %e", ret);
		}

	}
	else {
		if ((ret = sys_page_map(parent, (void*)va, envid, (void*)va, PTE_U | PTE_P)) < 0) {
			panic("duppage: %e", ret);
		}
	}

	return 0;
}


//
// User-level fork with copy-on-write.
// Set up our page fault handler appropriately.
// Create a child.
// Copy our address space and page fault handler setup to the child.
// Then mark the child as runnable and return.
//
// Returns: child's envid to the parent, 0 to the child, < 0 on error.
// It is also OK to panic on error.
//
// Hint:
//   Use uvpd, uvpt, and duppage.
//   Remember to fix "thisenv" in the child process.
//   Neither user exception stack should ever be marked copy-on-write,
//   so you must allocate a new page for the child's user exception stack.
//
envid_t
fork(void)
{
	envid_t id;
	int ret;

	set_pgfault_handler(pgfault);

	id = sys_exofork();
	if (id < 0) {
		panic("fork: sys_exofork failed: %e", id);
	}

	if (id == 0) {
		thisenv = &envs[ENVX(sys_getenvid())];
		return 0;
	}

  uintptr_t addr;
	for (addr = 0; addr < USTACKTOP; addr += PGSIZE) {

		if ((uvpd[PDX(addr)] & PTE_P) != PTE_P) {
			continue;
		}

		if ((uvpt[PGNUM(addr)] & PTE_P) != PTE_P) {
			continue;
		}

		duppage(id, PGNUM(addr));
	}

	ret = sys_page_alloc(id,
	                   (void *)(UXSTACKTOP - PGSIZE),
	                   PTE_U | PTE_W | PTE_P);
	if (ret < 0) {
		panic("fork: sys_page_alloc failed: %e", ret);
	}

	void _pgfault_upcall(void);
	ret = sys_env_set_pgfault_upcall(id, _pgfault_upcall);
	if (ret < 0) {
		panic("fork: sys_env_set_pgfault_upcall failed: %e", ret);
	}

	ret = sys_env_set_status(id, ENV_RUNNABLE);
	if (ret < 0) {
		panic("fork: sys_env_set_status failed: %e", ret);
	}

	return id;
}


// Challenge!
int
sfork(void)
{
	panic("sfork not implemented");
	return -E_INVAL;
}
