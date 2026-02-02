Operativni sistemi 2025/2026

# MIT 6.828 (2018) — JOS Kernel Labs (Lab 1–4)

This repository contains my implementation of the first four labs from **MIT 6.828: Operating System Engineering (2018)**:
https://pdos.csail.mit.edu/6.828/2018/overview.html

The goal of these labs is to build core parts of the **JOS teaching operating system**, step by step, starting from a bootloader and gradually adding memory management, process support, and basic kernel mechanisms.

---

## ✅ Completed Labs

### Lab 1 — Booting a PC
- Boot process overview (BIOS → bootloader → kernel)
- Entering protected mode
- Basic kernel initialization and early console output
- Using QEMU + GDB for debugging low-level execution

### Lab 2 — Memory Management
- Physical page allocator
- Page tables and virtual memory setup
- Kernel memory layout and permissions
- Mapping regions and handling page structures (`PageInfo`, free list, refcounting)

### Lab 3 — User Environments and Exception Handling
- Creating and managing user environments (`Env`)
- Context switching between environments
- Trap handling and IDT setup
- System call mechanism and user/kernel transitions

### Lab 4 — Multiprocessor, Copy-on-Write Fork, Preemption, IPC
- Multiprocessor support (per-CPU state, stacks, TSS setup)
- Big kernel lock and safe trap handling on multiple CPUs
- User-level page fault handling (exception stack + upcalls)
- Copy-on-write `fork()` (efficient process creation)
- Timer interrupts and preemptive scheduling
- IPC primitives (send/receive + optional page mapping)

---

## 🟡 Why Lab 5 is not included (yet)

I did not implement **Lab 5** in this repository because it focuses heavily on the **user-level file system server and file system interface**.  
At my university, file systems are covered later in the curriculum (the *System Programming* course in the 8th semester), so I plan to complete that part after finishing the file system material formally.

The lab set used in our laboratory sessions is therefore focused on **Lab 1–4**, i.e. the core JOS kernel work: boot, memory, traps/syscalls, environments, multiprocessing, COW fork, preemption, and IPC.

---

## 🧪 How to Build & Run

Typical workflow (inside the lab environment):
```bash
make
make qemu
```

To debug with GDB:
```bash
make qemu-gdb
# in another terminal
gdb
```

(Exact commands may depend on your setup. The official MIT 6.828 lab tools and environment are recommended.)

---

## Notes

This project was done as part of my operating systems laboratory work, with the intention of understanding low-level OS fundamentals:
- x86 boot + protected mode
- paging and memory management
- traps, syscalls, and user/kernel boundary
- process-like environments and scheduling
- concurrency/multiprocessor basics

---

## License / Credits

All credits for JOS and the lab handouts belong to MIT PDOS / MIT 6.828 staff.  
This repository contains my solutions and notes based on the 2018 course materials.
