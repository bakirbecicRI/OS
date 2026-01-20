/* See COPYRIGHT for copyright information. */

#include <inc/mmu.h>
#include <inc/memlayout.h>
#include <inc/trap.h>

#include <kern/picirq.h>

###################################################################
# exceptions/interrupts
###################################################################

/* TRAPHANDLER defines a globally-visible function for handling a trap.
 * It pushes a trap number onto the stack, then jumps to _alltraps.
 * Use TRAPHANDLER for traps where the CPU automatically pushes an error code.
 *
 * You shouldn't call a TRAPHANDLER function from C, but you may
 * need to _declare_ one in C (for instance, to get a function pointer
 * during IDT setup).  You can declare the function with
 *   void NAME();
 * where NAME is the argument passed to TRAPHANDLER.
 */
#define TRAPHANDLER(name, num)						\
	.data;                                                          \
	.long name;                                                     \
	.text;                                                          \
	.globl name;		/* define global symbol for 'name' */	\
	.type name, @function;	/* symbol type is function */		\
	.align 2;		/* align function definition */		\
	name:			/* function starts here */		\
	pushl $(num);							\
	jmp _alltraps

/*macro TRAPHANDLER for exercise 4. before challenge is same but without
.data; \
.long name; \
// .text; \
*/

/* Use TRAPHANDLER_NOEC for traps where the CPU doesn't push an error code.
 * It pushes a 0 in place of the error code, so the trap frame has the same
 * format in either case.
 */
#define TRAPHANDLER_NOEC(name, num)					\
	.data;                                                          \
	.long name;                                                     \
	.text;                                                          \
	.globl name;							\
	.type name, @function;						\
	.align 2;							\
	name:								\
	pushl $0;							\
	pushl $(num);							\
	jmp _alltraps
//exercise 4. before challenge same as TRAPHANDLER


//exercise 4. challenge macro skip
#define skip  \
  .data;      \
    .long 0x0 \

//exercise 7. tosyscall skip
#define tosys \
  .rept 28;       \
    .long 0x0;    \
  .endr

/*
 * Lab 3: Your code here for generating entry points for the different traps.
 */

//exercise 4. challenge - trap_handlers array
.data
.global trap_handlers
trap_handlers:
.text
TRAPHANDLER_NOEC(t_divide, T_DIVIDE)
TRAPHANDLER_NOEC(t_debug, T_DEBUG)
TRAPHANDLER_NOEC(t_nmi, T_NMI)
TRAPHANDLER_NOEC(t_brkpt, T_BRKPT)
TRAPHANDLER_NOEC(t_oflow, T_OFLOW)
TRAPHANDLER_NOEC(t_bound, T_BOUND)
TRAPHANDLER_NOEC(t_illop, T_ILLOP)
TRAPHANDLER_NOEC(t_device, T_DEVICE)
TRAPHANDLER(t_dblflt, T_DBLFLT)
skip
TRAPHANDLER(t_tss, T_TSS)
TRAPHANDLER(t_segnp, T_SEGNP)
TRAPHANDLER(t_stack, T_STACK)
TRAPHANDLER(t_gpflt, T_GPFLT)
TRAPHANDLER(t_pgflt, T_PGFLT)
skip
TRAPHANDLER_NOEC(t_fperr, T_FPERR)
TRAPHANDLER(t_align, T_ALIGN)
TRAPHANDLER_NOEC(t_mchk, T_MCHK)
TRAPHANDLER_NOEC(t_simderr, T_SIMDERR)
.data
.rept (IRQ_OFFSET - (T_SIMDERR + 1)) 
  .long 0
.endr
.text
TRAPHANDLER_NOEC(irq0, IRQ_OFFSET + 0)
TRAPHANDLER_NOEC(irq1, IRQ_OFFSET + 1)
TRAPHANDLER_NOEC(irq2, IRQ_OFFSET + 2)
TRAPHANDLER_NOEC(irq3, IRQ_OFFSET + 3)
TRAPHANDLER_NOEC(irq4, IRQ_OFFSET + 4)
TRAPHANDLER_NOEC(irq5, IRQ_OFFSET + 5)
TRAPHANDLER_NOEC(irq6, IRQ_OFFSET + 6)
TRAPHANDLER_NOEC(irq7, IRQ_OFFSET + 7)
TRAPHANDLER_NOEC(irq8, IRQ_OFFSET + 8)
TRAPHANDLER_NOEC(irq9, IRQ_OFFSET + 9)
TRAPHANDLER_NOEC(irq10, IRQ_OFFSET + 10)
TRAPHANDLER_NOEC(irq11, IRQ_OFFSET + 11)
TRAPHANDLER_NOEC(irq12, IRQ_OFFSET + 12)
TRAPHANDLER_NOEC(irq13, IRQ_OFFSET + 13)
TRAPHANDLER_NOEC(irq14, IRQ_OFFSET + 14)
TRAPHANDLER_NOEC(irq15, IRQ_OFFSET + 15)
TRAPHANDLER_NOEC(t_syscall, T_SYSCALL)

/*
exercise 4. before challenge - generating entry points
TRAPHANDLER_NOEC(t_divide, T_DIVIDE)
TRAPHANDLER_NOEC(t_debug, T_DEBUG)
TRAPHANDLER_NOEC(t_nmi, T_NMI)
TRAPHANDLER_NOEC(t_brkpt, T_BRKPT)
TRAPHANDLER_NOEC(t_oflow, T_OFLOW)
TRAPHANDLER_NOEC(t_bound, T_BOUND)
TRAPHANDLER_NOEC(t_illop, T_ILLOP)
TRAPHANDLER_NOEC(t_device, T_DEVICE)
TRAPHANDLER(t_dblflt, T_DBLFLT)
TRAPHANDLER(t_tss, T_TSS)
TRAPHANDLER(t_segnp, T_SEGNP)
TRAPHANDLER(t_stack, T_STACK)
TRAPHANDLER(t_gpflt, T_GPFLT)
TRAPHANDLER(t_pgflt, T_PGFLT)
TRAPHANDLER_NOEC(t_fperr, T_FPERR)
TRAPHANDLER(t_align, T_ALIGN)
TRAPHANDLER_NOEC(t_mchk, T_MCHK)
TRAPHANDLER_NOEC(t_simderr, T_SIMDERR)
*/

/*
alltraps same before challenge vs after challenge
 * Lab 3: Your code here for _alltraps
 */
.text

_alltraps:
pushl %ds;
pushl %es;
pushal;
movw $GD_KD, %ax;
movw %ax, %ds
movw %ax, %es
pushl %esp
call trap


