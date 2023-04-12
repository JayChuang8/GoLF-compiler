	Ltrue = 1
	Lfalse = 0
.data
PDCTrue:
	.byte 116
	.byte 114
	.byte 117
	.byte 101
PDCFalse:
	.byte 102
	.byte 97
	.byte 108
	.byte 115
	.byte 101
S0:
	.byte 0
S1:
	.byte 50
	.byte 10
	.byte 0
S2:
	.byte 10
	.byte 0
S3:
	.byte 32
	.byte 112
	.byte 114
	.byte 105
	.byte 109
	.byte 101
	.byte 32
	.byte 110
	.byte 117
	.byte 109
	.byte 98
	.byte 101
	.byte 114
	.byte 115
	.byte 32
	.byte 60
	.byte 61
	.byte 32
	.byte 0
	
.text
G0:
	.word 0
G0:
	.word 0
G0:
	.word 0
G0:
	.word 0
G0:
	.word 0
G0:
	.word 0
G0:
	.word 0
G0:
	.word 0
G0:
	.word 0
G0:
	.word 0
G0:
	.word 0
G0:
	.word 0
G0:
	.word 0
G0:
	.word 0
G0:
	.word 0
G0:
	.word 0
G0:
	.word 0
G0:
	.word 0
G0:
	.word 0
G0:
	.word 0
G0:
	.word 0
G0:
	.word 0
G0:
	.word 0
G0:
	.word 0
G0:
	.word 0
G0:
	.word 0
G0:
	.word 0
G0:
	.word 0
G0:
	.word 0
G0:
	.word 0
G0:
	.word 0
G0:
	.word 0
G0:
	.word 0
G0:
	.word 0
G0:
	.word 0
G0:
	.word 0
G0:
	.word 0
G0:
	.word 0
G0:
	.word 0
G0:
	.word 0
G0:
	.word 0
G0:
	.word 0
G0:
	.word 0
G0:
	.word 0
G0:
	.word 0
G0:
	.word 0
G0:
	.word 0
G0:
	.word 0
G0:
	.word 0
G0:
	.word 0
G0:
	.word 0
G0:
	.word 0
G0:
	.word 0
G0:
	.word 0
G0:
	.word 0
G0:
	.word 0
G0:
	.word 0
G0:
	.word 0
G0:
	.word 0
G0:
	.word 0
G0:
	.word 0
G0:
	.word 0
G0:
	.word 0
G0:
	.word 0
G0:
	.word 0
G0:
	.word 0
G0:
	.word 0
G0:
	.word 0
G0:
	.word 0
G0:
	.word 0
G0:
	.word 0
G0:
	.word 0
G0:
	.word 0
G0:
	.word 0
G0:
	.word 0
G0:
	.word 0
G0:
	.word 0
G0:
	.word 0
G0:
	.word 0
G0:
	.word 0
G0:
	.word 0
G0:
	.word 0
G0:
	.word 0
G0:
	.word 0
G0:
	.word 0
G0:
	.word 0
G0:
	.word 0
G0:
	.word 0
G0:
	.word 0
G0:
	.word 0
G0:
	.word 0
G0:
	.word 0
G0:
	.word 0
G0:
	.word 0
G0:
	.word 0
G0:
	.word 0
G0:
	.word 0
G0:
	.word 0
G0:
	.word 0
G0:
	.word 0
G0:
	.word 0
G0:
	.word 0
main:
	jal Lmain
	j Lhalt
G0:
	.word 0
Lmain:
	subu $sp,$sp,16
	sw $ra,0($sp)
	sw $0,4($sp)
	sw $0,8($sp)
	sw $0,12($sp)
	li $t0,1
	sw $t0,4($sp)
L0for:
	lw $t0,4($sp)
	li $t1,100
	sle $t2,$t0,$t1
	beqz $t2,L1for
	lw $t0,4($sp)
	li $t1,1
	seq $t2,$t0,$t1
	beqz $t2,L2else
	li $t0,0
	sw $t0,G0
	j L3ifelse
L2else:
