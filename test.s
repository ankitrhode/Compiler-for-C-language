	.file	1 "test.c"
	.section .mdebug.abi32
	.previous
	.gnu_attribute 4, 1
	.abicalls

 # -G value = 0, Arch = mips1, ISA = 1
 # GNU C (Debian 4.3.5-4) version 4.3.5 (mips-linux-gnu)
 #	compiled by GNU C version 4.4.5, GMP version 4.3.2, MPFR version 3.0.0-p3.
 # warning: MPFR header version 3.0.0-p3 differs from library version 3.1.2-p3.
 # GGC heuristics: --param ggc-min-expand=100 --param ggc-min-heapsize=131072
 # options passed:  test.c -meb -mllsc -mno-shared -mabi=32 -fverbose-asm
 # -fno-asynchronous-unwind-tables
 # options enabled:  -falign-loops -fargument-alias -fauto-inc-dec
 # -fbranch-count-reg -fcommon -fearly-inlining
 # -feliminate-unused-debug-types -ffunction-cse -fgcse-lm -fident -fivopts
 # -fkeep-static-consts -fleading-underscore -fmath-errno
 # -fmerge-debug-strings -fmove-loop-invariants -fpcc-struct-return
 # -fpeephole -fpic -fsched-interblock -fsched-spec
 # -fsched-stalled-insns-dep -fsigned-zeros -fsplit-ivs-in-unroller
 # -ftoplevel-reorder -ftrapping-math -ftree-cselim -ftree-loop-im
 # -ftree-loop-ivcanon -ftree-loop-optimize -ftree-parallelize-loops=
 # -ftree-reassoc -ftree-scev-cprop -ftree-vect-loop-version -fverbose-asm
 # -fzero-initialized-in-bss -mabicalls -mcheck-zero-division
 # -mdivide-traps -mdouble-float -meb -mexplicit-relocs -mextern-sdata
 # -mfp-exceptions -mfp32 -mfused-madd -mglibc -mgp32 -mgpopt -mhard-float
 # -mllsc -mlocal-sdata -mlong32 -mno-mips16 -mno-mips3d -msplit-addresses

 # Compiler executable checksum: 85b6753ea3cb37034719bf12e6ba259f

	.text
	.align	2
	.globl	lol
	.ent	lol
	.type	lol, @function
lol:
	.set	nomips16
	.frame	$fp,8,$31		# vars= 0, regs= 1/0, args= 0, gp= 0
	.mask	0x40000000,-4
	.fmask	0x00000000,0
	.set	noreorder
	.set	nomacro
	
	addiu	$sp,$sp,-8	 #,,
	sw	$fp,4($sp)	 #,
	move	$fp,$sp	 #,
	li	$2,1			# 0x1	 # D.1220,
	move	$sp,$fp	 #,
	lw	$fp,4($sp)	 #,
	addiu	$sp,$sp,8	 #,,
	j	$31
	nop
	 #
	.set	macro
	.set	reorder
	.end	lol
	.align	2
	.globl	main
	.ent	main
	.type	main, @function
main:
	.set	nomips16
	.frame	$fp,40,$31		# vars= 8, regs= 2/0, args= 16, gp= 8
	.mask	0xc0000000,-4
	.fmask	0x00000000,0
	.set	noreorder
	.set	nomacro
	
	addiu	$sp,$sp,-40	 #,,
	sw	$31,36($sp)	 #,
	sw	$fp,32($sp)	 #,
	move	$fp,$sp	 #,
	.cprestore	16	 #
	.option	pic0
	jal	lol
	nop
	 #
	.option	pic2
	lw	$28,16($fp)	 #,
	move	$sp,$fp	 #,
	lw	$31,36($sp)	 #,
	lw	$fp,32($sp)	 #,
	addiu	$sp,$sp,40	 #,,
	j	$31
	nop
	 #
	.set	macro
	.set	reorder
	.end	main
	.ident	"GCC: (Debian 4.3.5-4) 4.3.5"
