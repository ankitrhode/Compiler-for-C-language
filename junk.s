
.data
s0:	.asciiz "Here is the array\n"
s1:	.asciiz "\n"
s2:	.asciiz "Here is the sorted array\n"
s3:	.asciiz "\n"
.text
mtc1 $0 $f2
mergesort:
addi $sp $sp -4
sw $ra 0($sp)
addi $sp $sp -4
sw $fp 0($sp)
move $fp $sp
addi $sp $sp -16

addi $s2 $fp 20
lw $s0 0($s2)
addi $sp $sp -4
sw $s0 0($sp)
addi $s2 $fp 16
lw $s0 0($s2)
addi $sp $sp -4
sw $s0 0($sp)
lw $s1 0($sp)
lw $s0 4($sp)
addi $sp $sp 4
mtc1 $s0 $f0
cvt.s.w $f0 $f0
mtc1 $s1 $f1
cvt.s.w $f1 $f1
c.lt.s $f0 $f1
bc1t L1
sw $0 0($sp)
j L2
L1:
addi $s0 $0 1
sw $s0 0($sp)
L2:
mtc1 $0 $f2
lw $s0 0($sp)
mtc1 $s0 $f0
cvt.s.w $f0 $f0
addi $sp $sp 4
c.eq.s $f0 $f2
bc1t L3
addi $s2 $fp 20
lw $s0 0($s2)
addi $sp $sp -4
sw $s0 0($sp)
addi $s2 $fp 16
lw $s0 0($s2)
addi $sp $sp -4
sw $s0 0($sp)
lw $s1 0($sp)
lw $s0 4($sp)
addi $sp $sp 4
add $s0 $s0 $s1
sw $s0 0($sp)
li $s0 2
addi $sp $sp -4
sw $s0 0($sp)
lw $s1 0($sp)
lw $s0 4($sp)
addi $sp $sp 4
div $s0 $s0 $s1
sw $s0 0($sp)
lw $s5 0($sp)
addi $sp $sp 4
addi $s2 $fp -16
sw $s5 0($s2)

L4:
addi $sp $sp 0
addi $sp $sp -28
sw $s0 24($sp)
sw $s1 20($sp)
sw $s2 16($sp)
sw $s3 12($sp)
sw $s5 8($sp)
s.s $f0 4($sp)
s.s $f1 0($sp)

# Parameter 1
addi $s2 $fp 20
lw $s1 0($s2)
addi $sp $sp -4
sw $s1 0($sp)

# Parameter 2
addi $s2 $fp -16
lw $s1 0($s2)
addi $sp $sp -4
sw $s1 0($sp)

# Parameter 3
addi $s2 $fp 12
lw $s1 0($s2)
addi $sp $sp -4
sw $s1 0($sp)

# Parameter 4
addi $s2 $fp 8
lw $s1 0($s2)
addi $sp $sp -4
sw $s1 0($sp)

jal mergesort

addi $sp $sp 16
lw $s0 24($sp)
lw $s1 20($sp)
lw $s2 16($sp)
lw $s3 12($sp)
lw $s5 8($sp)
l.s $f0 4($sp)
l.s $f1 0($sp)
addi $sp $sp 28
move $s2 $sp
addi $sp $sp 0

L5:
addi $sp $sp 0
addi $sp $sp -28
sw $s0 24($sp)
sw $s1 20($sp)
sw $s2 16($sp)
sw $s3 12($sp)
sw $s5 8($sp)
s.s $f0 4($sp)
s.s $f1 0($sp)

# Parameter 1
addi $s2 $fp -16
lw $s0 0($s2)
addi $sp $sp -4
sw $s0 0($sp)
li $s0 1
addi $sp $sp -4
sw $s0 0($sp)
lw $s1 0($sp)
lw $s0 4($sp)
addi $sp $sp 4
add $s0 $s0 $s1
sw $s0 0($sp)
lw $s1 0($sp)
addi $sp $sp 4
addi $sp $sp -4
sw $s1 0($sp)

# Parameter 2
addi $s2 $fp 16
lw $s1 0($s2)
addi $sp $sp -4
sw $s1 0($sp)

# Parameter 3
addi $s2 $fp 12
lw $s1 0($s2)
addi $sp $sp -4
sw $s1 0($sp)

# Parameter 4
addi $s2 $fp 8
lw $s1 0($s2)
addi $sp $sp -4
sw $s1 0($sp)

jal mergesort

addi $sp $sp 16
lw $s0 24($sp)
lw $s1 20($sp)
lw $s2 16($sp)
lw $s3 12($sp)
lw $s5 8($sp)
l.s $f0 4($sp)
l.s $f1 0($sp)
addi $sp $sp 28
move $s2 $sp
addi $sp $sp 0

L6:
addi $s2 $fp 20
lw $s5 0($s2)
addi $s2 $fp -12
sw $s5 0($s2)
L8:
addi $s2 $fp -12
lw $s0 0($s2)
addi $sp $sp -4
sw $s0 0($sp)
addi $s2 $fp 16
lw $s0 0($s2)
addi $sp $sp -4
sw $s0 0($sp)
lw $s1 0($sp)
lw $s0 4($sp)
addi $sp $sp 4
mtc1 $s0 $f0
cvt.s.w $f0 $f0
mtc1 $s1 $f1
cvt.s.w $f1 $f1
c.le.s $f0 $f1
bc1f L10
addi $s0 $0 1
sw $s0 0($sp)
j L11
L10:
sw $0 0($sp)
L11:
lw $s0 0($sp)
mtc1 $s0 $f0
cvt.s.w $f0 $f0
addi $sp $sp 4
c.eq.s $f0 $f2
bc1t L7
addi $s2 $fp -12
lw $s0 0($s2)
addi $sp $sp -4
sw $s0 0($sp)
addi $s2 $fp 12
lw $s2 0($s2)
lw $s0 0($sp)
addi $sp $sp 4
addi $s4 $0 4
mul $s0 $s0 $s4
add $s2 $s2 $s0
l.s $f1 0($s2)
addi $s2 $fp -12
lw $s0 0($s2)
addi $sp $sp -4
sw $s0 0($sp)
addi $s2 $fp 8
lw $s2 0($s2)
lw $s0 0($sp)
addi $sp $sp 4
addi $s4 $0 4
mul $s0 $s0 $s4
add $s2 $s2 $s0
s.s $f1 0($s2)
L9:
addi $s2 $fp -12
lw $s0 0($s2)
addi $sp $sp -4
sw $s0 0($sp)
lw $s0 0($sp)
sw $s0 0($sp)
addi $s0 $s0 1
sw $s0 0($s2)
addi $sp $sp 4
j L8

L7:
addi $s2 $fp 20
lw $s5 0($s2)
addi $s2 $fp -12
sw $s5 0($s2)

L12:
addi $s2 $fp -16
lw $s0 0($s2)
addi $sp $sp -4
sw $s0 0($sp)
li $s0 1
addi $sp $sp -4
sw $s0 0($sp)
lw $s1 0($sp)
lw $s0 4($sp)
addi $sp $sp 4
add $s0 $s0 $s1
sw $s0 0($sp)
lw $s5 0($sp)
addi $sp $sp 4
addi $s2 $fp -8
sw $s5 0($s2)

L13:
addi $s2 $fp 20
lw $s5 0($s2)
addi $s2 $fp -4
sw $s5 0($s2)
L14:
addi $s2 $fp -4
lw $s0 0($s2)
addi $sp $sp -4
sw $s0 0($sp)
addi $s2 $fp 16
lw $s0 0($s2)
addi $sp $sp -4
sw $s0 0($sp)
lw $s1 0($sp)
lw $s0 4($sp)
addi $sp $sp 4
mtc1 $s0 $f0
cvt.s.w $f0 $f0
mtc1 $s1 $f1
cvt.s.w $f1 $f1
c.le.s $f0 $f1
bc1f L16
addi $s0 $0 1
sw $s0 0($sp)
j L17
L16:
sw $0 0($sp)
L17:
lw $s0 0($sp)
mtc1 $s0 $f0
cvt.s.w $f0 $f0
addi $sp $sp 4
c.eq.s $f0 $f2
bc1t L0
addi $s2 $fp -12
lw $s0 0($s2)
addi $sp $sp -4
sw $s0 0($sp)
addi $s2 $fp -16
lw $s0 0($s2)
addi $sp $sp -4
sw $s0 0($sp)
li $s0 1
addi $sp $sp -4
sw $s0 0($sp)
lw $s1 0($sp)
lw $s0 4($sp)
addi $sp $sp 4
add $s0 $s0 $s1
sw $s0 0($sp)
lw $s1 0($sp)
lw $s0 4($sp)
addi $sp $sp 4
mtc1 $s0 $f0
cvt.s.w $f0 $f0
mtc1 $s1 $f1
cvt.s.w $f1 $f1
c.eq.s $f0 $f1
bc1f L18
addi $s0 $0 1
sw $s0 0($sp)
j L19
L18:
sw $0 0($sp)
L19:
mtc1 $0 $f2
lw $s0 0($sp)
mtc1 $s0 $f0
cvt.s.w $f0 $f0
addi $sp $sp 4
c.eq.s $f0 $f2
bc1t L20
addi $s2 $fp -8
lw $s0 0($s2)
addi $sp $sp -4
sw $s0 0($sp)
lw $s0 0($sp)
sw $s0 0($sp)
addi $s0 $s0 1
sw $s0 0($s2)
addi $s2 $fp 8
lw $s2 0($s2)
lw $s0 0($sp)
addi $sp $sp 4
addi $s4 $0 4
mul $s0 $s0 $s4
add $s2 $s2 $s0
l.s $f1 0($s2)
addi $s2 $fp -4
lw $s0 0($s2)
addi $sp $sp -4
sw $s0 0($sp)
addi $s2 $fp 12
lw $s2 0($s2)
lw $s0 0($sp)
addi $sp $sp 4
addi $s4 $0 4
mul $s0 $s0 $s4
add $s2 $s2 $s0
s.s $f1 0($s2)
j L15
L20:
addi $s2 $fp -8
lw $s0 0($s2)
addi $sp $sp -4
sw $s0 0($sp)
addi $s2 $fp 16
lw $s0 0($s2)
addi $sp $sp -4
sw $s0 0($sp)
li $s0 1
addi $sp $sp -4
sw $s0 0($sp)
lw $s1 0($sp)
lw $s0 4($sp)
addi $sp $sp 4
add $s0 $s0 $s1
sw $s0 0($sp)
lw $s1 0($sp)
lw $s0 4($sp)
addi $sp $sp 4
mtc1 $s0 $f0
cvt.s.w $f0 $f0
mtc1 $s1 $f1
cvt.s.w $f1 $f1
c.eq.s $f0 $f1
bc1f L21
addi $s0 $0 1
sw $s0 0($sp)
j L22
L21:
sw $0 0($sp)
L22:
mtc1 $0 $f2
lw $s0 0($sp)
mtc1 $s0 $f0
cvt.s.w $f0 $f0
addi $sp $sp 4
c.eq.s $f0 $f2
bc1t L23
addi $s2 $fp -12
lw $s0 0($s2)
addi $sp $sp -4
sw $s0 0($sp)
lw $s0 0($sp)
sw $s0 0($sp)
addi $s0 $s0 1
sw $s0 0($s2)
addi $s2 $fp 8
lw $s2 0($s2)
lw $s0 0($sp)
addi $sp $sp 4
addi $s4 $0 4
mul $s0 $s0 $s4
add $s2 $s2 $s0
l.s $f1 0($s2)
addi $s2 $fp -4
lw $s0 0($s2)
addi $sp $sp -4
sw $s0 0($sp)
addi $s2 $fp 12
lw $s2 0($s2)
lw $s0 0($sp)
addi $sp $sp 4
addi $s4 $0 4
mul $s0 $s0 $s4
add $s2 $s2 $s0
s.s $f1 0($s2)
j L15
L23:
addi $s2 $fp -12
lw $s0 0($s2)
addi $sp $sp -4
sw $s0 0($sp)
addi $s2 $fp 8
lw $s2 0($s2)
lw $s0 0($sp)
addi $sp $sp 4
addi $s4 $0 4
mul $s0 $s0 $s4
add $s2 $s2 $s0
l.s $f0 0($s2)
addi $sp $sp -4
s.s $f0 0($sp)
addi $s2 $fp -8
lw $s0 0($s2)
addi $sp $sp -4
sw $s0 0($sp)
addi $s2 $fp 8
lw $s2 0($s2)
lw $s0 0($sp)
addi $sp $sp 4
addi $s4 $0 4
mul $s0 $s0 $s4
add $s2 $s2 $s0
l.s $f0 0($s2)
addi $sp $sp -4
s.s $f0 0($sp)
l.s $f1 0($sp)
l.s $f0 4($sp)
addi $sp $sp 4
c.lt.s $f0 $f1
bc1t L24
sw $0 0($sp)
j L25
L24:
addi $s0 $0 1
sw $s0 0($sp)
L25:
mtc1 $0 $f2
lw $s0 0($sp)
mtc1 $s0 $f0
cvt.s.w $f0 $f0
addi $sp $sp 4
c.eq.s $f0 $f2
bc1t L26
addi $s2 $fp -12
lw $s0 0($s2)
addi $sp $sp -4
sw $s0 0($sp)
lw $s0 0($sp)
sw $s0 0($sp)
addi $s0 $s0 1
sw $s0 0($s2)
addi $s2 $fp 8
lw $s2 0($s2)
lw $s0 0($sp)
addi $sp $sp 4
addi $s4 $0 4
mul $s0 $s0 $s4
add $s2 $s2 $s0
l.s $f1 0($s2)
addi $s2 $fp -4
lw $s0 0($s2)
addi $sp $sp -4
sw $s0 0($sp)
addi $s2 $fp 12
lw $s2 0($s2)
lw $s0 0($sp)
addi $sp $sp 4
addi $s4 $0 4
mul $s0 $s0 $s4
add $s2 $s2 $s0
s.s $f1 0($s2)
j L15
L26:
addi $s2 $fp -8
lw $s0 0($s2)
addi $sp $sp -4
sw $s0 0($sp)
lw $s0 0($sp)
sw $s0 0($sp)
addi $s0 $s0 1
sw $s0 0($s2)
addi $s2 $fp 8
lw $s2 0($s2)
lw $s0 0($sp)
addi $sp $sp 4
addi $s4 $0 4
mul $s0 $s0 $s4
add $s2 $s2 $s0
l.s $f1 0($s2)
addi $s2 $fp -4
lw $s0 0($s2)
addi $sp $sp -4
sw $s0 0($sp)
addi $s2 $fp 12
lw $s2 0($s2)
lw $s0 0($sp)
addi $sp $sp 4
addi $s4 $0 4
mul $s0 $s0 $s4
add $s2 $s2 $s0
s.s $f1 0($s2)
j L15
j L15
j L15

L15:
addi $s2 $fp -4
lw $s0 0($s2)
addi $sp $sp -4
sw $s0 0($sp)
lw $s0 0($sp)
sw $s0 0($sp)
addi $s0 $s0 1
sw $s0 0($s2)
addi $sp $sp 4
j L14

j L0
L3:

j L0

L0:
addi $sp $sp 16
lw $fp 0($sp)
addi $sp $sp 4
lw $ra 0($sp)
addi $sp $sp 4
jr $ra

main:
addi $sp $sp -4
sw $ra 0($sp)
addi $sp $sp -4
sw $fp 0($sp)
move $fp $sp
addi $sp $sp -88

li $s0 0
addi $sp $sp -4
sw $s0 0($sp)
lw $s5 0($sp)
addi $sp $sp 4
addi $s2 $fp -8
sw $s5 0($s2)
L28:
addi $s2 $fp -8
lw $s0 0($s2)
addi $sp $sp -4
sw $s0 0($sp)
li $s0 10
addi $sp $sp -4
sw $s0 0($sp)
lw $s1 0($sp)
lw $s0 4($sp)
addi $sp $sp 4
mtc1 $s0 $f0
cvt.s.w $f0 $f0
mtc1 $s1 $f1
cvt.s.w $f1 $f1
c.lt.s $f0 $f1
bc1t L30
sw $0 0($sp)
j L31
L30:
addi $s0 $0 1
sw $s0 0($sp)
L31:
lw $s0 0($sp)
mtc1 $s0 $f0
cvt.s.w $f0 $f0
addi $sp $sp 4
c.eq.s $f0 $f2
bc1t L27
li.s $f0 10.934000
addi $sp $sp -4
s.s $f0 0($sp)
addi $s2 $fp -8
lw $s0 0($s2)
addi $sp $sp -4
sw $s0 0($sp)
li.s $f0 0.990000
addi $sp $sp -4
s.s $f0 0($sp)
l.s $f1 0($sp)
lw $s0 4($sp)
addi $sp $sp 4
mtc1 $s0 $f0
cvt.s.w $f0 $f0
mul.s $f0 $f0 $f1
s.s $f0 0($sp)
l.s $f1 0($sp)
l.s $f0 4($sp)
addi $sp $sp 4
sub.s $f0 $f0 $f1
s.s $f0 0($sp)
l.s $f1 0($sp)
addi $sp $sp 4
addi $s2 $fp -8
lw $s0 0($s2)
addi $sp $sp -4
sw $s0 0($sp)
addi $s2 $fp -88
lw $s0 0($sp)
addi $sp $sp 4
addi $s4 $0 4
mul $s0 $s0 $s4
add $s2 $s2 $s0
s.s $f1 0($s2)
L29:
addi $s2 $fp -8
lw $s0 0($s2)
addi $sp $sp -4
sw $s0 0($sp)
lw $s0 0($sp)
sw $s0 0($sp)
addi $s0 $s0 1
sw $s0 0($s2)
addi $sp $sp 4
j L28

L27:
li $v0 4
la $a0 s0
syscall

L32:
li $s0 0
addi $sp $sp -4
sw $s0 0($sp)
lw $s5 0($sp)
addi $sp $sp 4
addi $s2 $fp -8
sw $s5 0($s2)
L34:
addi $s2 $fp -8
lw $s0 0($s2)
addi $sp $sp -4
sw $s0 0($sp)
li $s0 10
addi $sp $sp -4
sw $s0 0($sp)
lw $s1 0($sp)
lw $s0 4($sp)
addi $sp $sp 4
mtc1 $s0 $f0
cvt.s.w $f0 $f0
mtc1 $s1 $f1
cvt.s.w $f1 $f1
c.lt.s $f0 $f1
bc1t L36
sw $0 0($sp)
j L37
L36:
addi $s0 $0 1
sw $s0 0($sp)
L37:
lw $s0 0($sp)
mtc1 $s0 $f0
cvt.s.w $f0 $f0
addi $sp $sp 4
c.eq.s $f0 $f2
bc1t L33
addi $s2 $fp -8
lw $s0 0($s2)
addi $sp $sp -4
sw $s0 0($sp)
addi $s2 $fp -88
lw $s0 0($sp)
addi $sp $sp 4
addi $s4 $0 4
mul $s0 $s0 $s4
add $s2 $s2 $s0
l.s $f12 0($s2)
li $v0 2
syscall
li $v0 4
la $a0 s1
syscall

L35:
addi $s2 $fp -8
lw $s0 0($s2)
addi $sp $sp -4
sw $s0 0($sp)
lw $s0 0($sp)
sw $s0 0($sp)
addi $s0 $s0 1
sw $s0 0($s2)
addi $sp $sp 4
j L34

L33:
addi $sp $sp 0
addi $sp $sp -28
sw $s0 24($sp)
sw $s1 20($sp)
sw $s2 16($sp)
sw $s3 12($sp)
sw $s5 8($sp)
s.s $f0 4($sp)
s.s $f1 0($sp)

# Parameter 1
li $s0 0
addi $sp $sp -4
sw $s0 0($sp)
lw $s1 0($sp)
addi $sp $sp 4
addi $sp $sp -4
sw $s1 0($sp)

# Parameter 2
li $s0 10
addi $sp $sp -4
sw $s0 0($sp)
li $s0 1
addi $sp $sp -4
sw $s0 0($sp)
lw $s1 0($sp)
lw $s0 4($sp)
addi $sp $sp 4
sub $s0 $s0 $s1
sw $s0 0($sp)
lw $s1 0($sp)
addi $sp $sp 4
addi $sp $sp -4
sw $s1 0($sp)

# Parameter 3
addi $s2 $fp -88
addi $s1 $s2 0
addi $sp $sp -4
sw $s1 0($sp)

# Parameter 4
addi $s2 $fp -48
addi $s1 $s2 0
addi $sp $sp -4
sw $s1 0($sp)

jal mergesort

addi $sp $sp 16
lw $s0 24($sp)
lw $s1 20($sp)
lw $s2 16($sp)
lw $s3 12($sp)
lw $s5 8($sp)
l.s $f0 4($sp)
l.s $f1 0($sp)
addi $sp $sp 28
move $s2 $sp
addi $sp $sp 0

L38:
li $v0 4
la $a0 s2
syscall

L39:
li $s0 0
addi $sp $sp -4
sw $s0 0($sp)
lw $s5 0($sp)
addi $sp $sp 4
addi $s2 $fp -8
sw $s5 0($s2)
L41:
addi $s2 $fp -8
lw $s0 0($s2)
addi $sp $sp -4
sw $s0 0($sp)
li $s0 10
addi $sp $sp -4
sw $s0 0($sp)
lw $s1 0($sp)
lw $s0 4($sp)
addi $sp $sp 4
mtc1 $s0 $f0
cvt.s.w $f0 $f0
mtc1 $s1 $f1
cvt.s.w $f1 $f1
c.lt.s $f0 $f1
bc1t L43
sw $0 0($sp)
j L44
L43:
addi $s0 $0 1
sw $s0 0($sp)
L44:
lw $s0 0($sp)
mtc1 $s0 $f0
cvt.s.w $f0 $f0
addi $sp $sp 4
c.eq.s $f0 $f2
bc1t L40
addi $s2 $fp -8
lw $s0 0($s2)
addi $sp $sp -4
sw $s0 0($sp)
addi $s2 $fp -88
lw $s0 0($sp)
addi $sp $sp 4
addi $s4 $0 4
mul $s0 $s0 $s4
add $s2 $s2 $s0
l.s $f12 0($s2)
li $v0 2
syscall
li $v0 4
la $a0 s3
syscall

L42:
addi $s2 $fp -8
lw $s0 0($s2)
addi $sp $sp -4
sw $s0 0($sp)
lw $s0 0($sp)
sw $s0 0($sp)
addi $s0 $s0 1
sw $s0 0($s2)
addi $sp $sp 4
j L41

L40:
addi $sp $sp 88
lw $fp 0($sp)
addi $sp $sp 4
lw $ra 0($sp)
addi $sp $sp 4
jr $ra

