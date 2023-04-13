	Ltrue = 1
	Lfalse = 0
.data
PDCTrue:
	.byte 116
	.byte 114
	.byte 117
	.byte 101
	.byte 10
	.byte 0
PDCFalse:
	.byte 102
	.byte 97
	.byte 108
	.byte 115
	.byte 101
	.byte 10
	.byte 0
divlabel:
	.byte 101
	.byte 114
	.byte 114
	.byte 111
	.byte 114
	.byte 58
	.byte 32
	.byte 100
	.byte 105
	.byte 118
	.byte 105
	.byte 115
	.byte 105
	.byte 111
	.byte 110
	.byte 32
	.byte 98
	.byte 121
	.byte 32
	.byte 122
	.byte 101
	.byte 114
	.byte 111
	.byte 10
	.byte 0
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
G1:
	.word 0
G2:
	.word 0
G3:
	.word 0
G4:
	.word 0
G5:
	.word 0
G6:
	.word 0
G7:
	.word 0
G8:
	.word 0
G9:
	.word 0
G10:
	.word 0
G11:
	.word 0
G12:
	.word 0
G13:
	.word 0
G14:
	.word 0
G15:
	.word 0
G16:
	.word 0
G17:
	.word 0
G18:
	.word 0
G19:
	.word 0
G20:
	.word 0
G21:
	.word 0
G22:
	.word 0
G23:
	.word 0
G24:
	.word 0
G25:
	.word 0
G26:
	.word 0
G27:
	.word 0
G28:
	.word 0
G29:
	.word 0
G30:
	.word 0
G31:
	.word 0
G32:
	.word 0
G33:
	.word 0
G34:
	.word 0
G35:
	.word 0
G36:
	.word 0
G37:
	.word 0
G38:
	.word 0
G39:
	.word 0
G40:
	.word 0
G41:
	.word 0
G42:
	.word 0
G43:
	.word 0
G44:
	.word 0
G45:
	.word 0
G46:
	.word 0
G47:
	.word 0
G48:
	.word 0
G49:
	.word 0
G50:
	.word 0
G51:
	.word 0
G52:
	.word 0
G53:
	.word 0
G54:
	.word 0
G55:
	.word 0
G56:
	.word 0
G57:
	.word 0
G58:
	.word 0
G59:
	.word 0
G60:
	.word 0
G61:
	.word 0
G62:
	.word 0
G63:
	.word 0
G64:
	.word 0
G65:
	.word 0
G66:
	.word 0
G67:
	.word 0
G68:
	.word 0
G69:
	.word 0
G70:
	.word 0
G71:
	.word 0
G72:
	.word 0
G73:
	.word 0
G74:
	.word 0
G75:
	.word 0
G76:
	.word 0
G77:
	.word 0
G78:
	.word 0
G79:
	.word 0
G80:
	.word 0
G81:
	.word 0
G82:
	.word 0
G83:
	.word 0
G84:
	.word 0
G85:
	.word 0
G86:
	.word 0
G87:
	.word 0
G88:
	.word 0
G89:
	.word 0
G90:
	.word 0
G91:
	.word 0
G92:
	.word 0
G93:
	.word 0
G94:
	.word 0
G95:
	.word 0
G96:
	.word 0
G97:
	.word 0
G98:
	.word 0
G99:
	.word 0
G100:
	.word 0
G101:
	.word 0
main:
	jal Lmain
	j Lhalt
G102:
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
	sw $t0,G1
	j L3ifelse
L2else:
