.text
main:
addi $sp $sp -4
sw $ra 0($sp)
addi $sp $sp -4
sw $fp 0($sp)
move $fp $sp
addi $sp $sp -4

addi $s2 $fp -4
li.s $f0 1.2
l.s $f0 0($s2)
addi $sp $sp -4
s.s $f0 0($sp)
li $s0 1
addi $sp $sp -4
sw $s0 0($sp)
lw $s1 0($sp)
l.s $f0 4($sp)
addi $sp $sp 4
mtc1 $s1 $f1
cvt.s.w $f1 $f1
add.s $f0 $f0 $f1
s.s $f0 0($sp)
l.s $f1 0($sp)
addi $sp $sp 4
addi $s2 $fp -4
s.s $f1 0($s2)

L0:
addi $sp $sp 4
lw $fp 0($sp)
addi $sp $sp 4
lw $ra 0($sp)
addi $sp $sp 4
jr $ra

nop
