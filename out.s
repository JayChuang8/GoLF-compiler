	Ltrue = 1
	Lfalse = 0
.data
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
GA_0:
	.word 0
GA_1:
	.word 0
GA_2:
	.word 0
GA_3:
	.word 0
GA_4:
	.word 0
GA_5:
	.word 0
GA_6:
	.word 0
GA_7:
	.word 0
GA_8:
	.word 0
GA_9:
	.word 0
GA_10:
	.word 0
GA_11:
	.word 0
GA_12:
	.word 0
GA_13:
	.word 0
GA_14:
	.word 0
GA_15:
	.word 0
GA_16:
	.word 0
GA_17:
	.word 0
GA_18:
	.word 0
GA_19:
	.word 0
GA_20:
	.word 0
GA_21:
	.word 0
GA_22:
	.word 0
GA_23:
	.word 0
GA_24:
	.word 0
GA_25:
	.word 0
GA_26:
	.word 0
GA_27:
	.word 0
GA_28:
	.word 0
GA_29:
	.word 0
GA_30:
	.word 0
GA_31:
	.word 0
GA_32:
	.word 0
GA_33:
	.word 0
GA_34:
	.word 0
GA_35:
	.word 0
GA_36:
	.word 0
GA_37:
	.word 0
GA_38:
	.word 0
GA_39:
	.word 0
GA_40:
	.word 0
GA_41:
	.word 0
GA_42:
	.word 0
GA_43:
	.word 0
GA_44:
	.word 0
GA_45:
	.word 0
GA_46:
	.word 0
GA_47:
	.word 0
GA_48:
	.word 0
GA_49:
	.word 0
GA_50:
	.word 0
GA_51:
	.word 0
GA_52:
	.word 0
GA_53:
	.word 0
GA_54:
	.word 0
GA_55:
	.word 0
GA_56:
	.word 0
GA_57:
	.word 0
GA_58:
	.word 0
GA_59:
	.word 0
GA_60:
	.word 0
GA_61:
	.word 0
GA_62:
	.word 0
GA_63:
	.word 0
GA_64:
	.word 0
GA_65:
	.word 0
GA_66:
	.word 0
GA_67:
	.word 0
GA_68:
	.word 0
GA_69:
	.word 0
GA_70:
	.word 0
GA_71:
	.word 0
GA_72:
	.word 0
GA_73:
	.word 0
GA_74:
	.word 0
GA_75:
	.word 0
GA_76:
	.word 0
GA_77:
	.word 0
GA_78:
	.word 0
GA_79:
	.word 0
GA_80:
	.word 0
GA_81:
	.word 0
GA_82:
	.word 0
GA_83:
	.word 0
GA_84:
	.word 0
GA_85:
	.word 0
GA_86:
	.word 0
GA_87:
	.word 0
GA_88:
	.word 0
GA_89:
	.word 0
GA_90:
	.word 0
GA_91:
	.word 0
GA_92:
	.word 0
GA_93:
	.word 0
GA_94:
	.word 0
GA_95:
	.word 0
GA_96:
	.word 0
GA_97:
	.word 0
GA_98:
	.word 0
GA_99:
	.word 0
GA_100:
	.word 0
Gnot_there:
	.word 0
main:
	jal Lmain
	j Lhalt
GINTERNALX:
	.word 0
Lmain:
	subu $sp, $sp, 16
	sw $ra, 0($sp)
	sw $0, 4($sp)
	sw $0, 8($sp)
	sw $0, 12($sp)
	li $t0, 1
	sw $t0, 4($sp)
L0for:
	lw $t0, 4($sp)
	li $t1, 100
	sle $t2, $t0, $t1
	beqz $t2, L1for
	lw $t0, 4($sp)
	li $t1, 1
	seteq $t2, $t0, $t1
	beqz $t2, L2else
	li $t0, 0
	sw $t0, GA_1
	j L3ifelse
L2else:
