#ifndef _LEON3_BHV_MACROS_H
#define _LEON3_BHV_MACROS_H

typedef ac_memport<leon3_parms::ac_word, leon3_parms::ac_Hword> ac_memory;

#define ac_behavior(instr) AC_BEHAVIOR_##instr ()

#define AC_BEHAVIOR_instruction() leon3_parms::leon3_isa::_behavior_instruction(unsigned int op)

#define AC_BEHAVIOR_begin() leon3_parms::leon3_isa::_behavior_begin()
#define AC_BEHAVIOR_end() leon3_parms::leon3_isa::_behavior_end()

#define AC_BEHAVIOR_Type_F1() leon3_parms::leon3_isa::_behavior_leon3_Type_F1(unsigned int op, unsigned int disp30)
#define AC_BEHAVIOR_Type_F2A() leon3_parms::leon3_isa::_behavior_leon3_Type_F2A(unsigned int op, unsigned int rd, unsigned int op2, unsigned int imm22)
#define AC_BEHAVIOR_Type_F2B() leon3_parms::leon3_isa::_behavior_leon3_Type_F2B(unsigned int op, unsigned int an, unsigned int cond, unsigned int op2, int disp22)
#define AC_BEHAVIOR_Type_F3A() leon3_parms::leon3_isa::_behavior_leon3_Type_F3A(unsigned int op, unsigned int rd, unsigned int op3, unsigned int rs1, unsigned int is, unsigned int asi, unsigned int rs2)
#define AC_BEHAVIOR_Type_F3B() leon3_parms::leon3_isa::_behavior_leon3_Type_F3B(unsigned int op, unsigned int rd, unsigned int op3, unsigned int rs1, unsigned int is, int simm13)
#define AC_BEHAVIOR_Type_FTA() leon3_parms::leon3_isa::_behavior_leon3_Type_FTA(unsigned int op, unsigned int r1, unsigned int cond, unsigned int op2a, unsigned int rs1, unsigned int is, unsigned int r2a, unsigned int rs2)
#define AC_BEHAVIOR_Type_FTB() leon3_parms::leon3_isa::_behavior_leon3_Type_FTB(unsigned int op, unsigned int r1, unsigned int cond, unsigned int op2a, unsigned int rs1, unsigned int is, unsigned int r2a, unsigned int imm7)

#define AC_BEHAVIOR_call() leon3_parms::leon3_isa::behavior_call(unsigned int op, unsigned int disp30)
#define AC_BEHAVIOR_nop() leon3_parms::leon3_isa::behavior_nop(unsigned int op, unsigned int rd, unsigned int op2, unsigned int imm22)
#define AC_BEHAVIOR_sethi() leon3_parms::leon3_isa::behavior_sethi(unsigned int op, unsigned int rd, unsigned int op2, unsigned int imm22)
#define AC_BEHAVIOR_ba() leon3_parms::leon3_isa::behavior_ba(unsigned int op, unsigned int an, unsigned int cond, unsigned int op2, int disp22)
#define AC_BEHAVIOR_bn() leon3_parms::leon3_isa::behavior_bn(unsigned int op, unsigned int an, unsigned int cond, unsigned int op2, int disp22)
#define AC_BEHAVIOR_bne() leon3_parms::leon3_isa::behavior_bne(unsigned int op, unsigned int an, unsigned int cond, unsigned int op2, int disp22)
#define AC_BEHAVIOR_be() leon3_parms::leon3_isa::behavior_be(unsigned int op, unsigned int an, unsigned int cond, unsigned int op2, int disp22)
#define AC_BEHAVIOR_bg() leon3_parms::leon3_isa::behavior_bg(unsigned int op, unsigned int an, unsigned int cond, unsigned int op2, int disp22)
#define AC_BEHAVIOR_ble() leon3_parms::leon3_isa::behavior_ble(unsigned int op, unsigned int an, unsigned int cond, unsigned int op2, int disp22)
#define AC_BEHAVIOR_bge() leon3_parms::leon3_isa::behavior_bge(unsigned int op, unsigned int an, unsigned int cond, unsigned int op2, int disp22)
#define AC_BEHAVIOR_bl() leon3_parms::leon3_isa::behavior_bl(unsigned int op, unsigned int an, unsigned int cond, unsigned int op2, int disp22)
#define AC_BEHAVIOR_bgu() leon3_parms::leon3_isa::behavior_bgu(unsigned int op, unsigned int an, unsigned int cond, unsigned int op2, int disp22)
#define AC_BEHAVIOR_bleu() leon3_parms::leon3_isa::behavior_bleu(unsigned int op, unsigned int an, unsigned int cond, unsigned int op2, int disp22)
#define AC_BEHAVIOR_bcc() leon3_parms::leon3_isa::behavior_bcc(unsigned int op, unsigned int an, unsigned int cond, unsigned int op2, int disp22)
#define AC_BEHAVIOR_bcs() leon3_parms::leon3_isa::behavior_bcs(unsigned int op, unsigned int an, unsigned int cond, unsigned int op2, int disp22)
#define AC_BEHAVIOR_bpos() leon3_parms::leon3_isa::behavior_bpos(unsigned int op, unsigned int an, unsigned int cond, unsigned int op2, int disp22)
#define AC_BEHAVIOR_bneg() leon3_parms::leon3_isa::behavior_bneg(unsigned int op, unsigned int an, unsigned int cond, unsigned int op2, int disp22)
#define AC_BEHAVIOR_bvc() leon3_parms::leon3_isa::behavior_bvc(unsigned int op, unsigned int an, unsigned int cond, unsigned int op2, int disp22)
#define AC_BEHAVIOR_bvs() leon3_parms::leon3_isa::behavior_bvs(unsigned int op, unsigned int an, unsigned int cond, unsigned int op2, int disp22)
#define AC_BEHAVIOR_ldsb_reg() leon3_parms::leon3_isa::behavior_ldsb_reg(unsigned int op, unsigned int rd, unsigned int op3, unsigned int rs1, unsigned int is, unsigned int asi, unsigned int rs2)
#define AC_BEHAVIOR_ldsh_reg() leon3_parms::leon3_isa::behavior_ldsh_reg(unsigned int op, unsigned int rd, unsigned int op3, unsigned int rs1, unsigned int is, unsigned int asi, unsigned int rs2)
#define AC_BEHAVIOR_ldub_reg() leon3_parms::leon3_isa::behavior_ldub_reg(unsigned int op, unsigned int rd, unsigned int op3, unsigned int rs1, unsigned int is, unsigned int asi, unsigned int rs2)
#define AC_BEHAVIOR_lduh_reg() leon3_parms::leon3_isa::behavior_lduh_reg(unsigned int op, unsigned int rd, unsigned int op3, unsigned int rs1, unsigned int is, unsigned int asi, unsigned int rs2)
#define AC_BEHAVIOR_ld_reg() leon3_parms::leon3_isa::behavior_ld_reg(unsigned int op, unsigned int rd, unsigned int op3, unsigned int rs1, unsigned int is, unsigned int asi, unsigned int rs2)
#define AC_BEHAVIOR_ldd_reg() leon3_parms::leon3_isa::behavior_ldd_reg(unsigned int op, unsigned int rd, unsigned int op3, unsigned int rs1, unsigned int is, unsigned int asi, unsigned int rs2)
#define AC_BEHAVIOR_lduba() leon3_parms::leon3_isa::behavior_lduba(unsigned int op, unsigned int rd, unsigned int op3, unsigned int rs1, unsigned int is, unsigned int asi, unsigned int rs2)
#define AC_BEHAVIOR_lduha() leon3_parms::leon3_isa::behavior_lduha(unsigned int op, unsigned int rd, unsigned int op3, unsigned int rs1, unsigned int is, unsigned int asi, unsigned int rs2)
#define AC_BEHAVIOR_lda() leon3_parms::leon3_isa::behavior_lda(unsigned int op, unsigned int rd, unsigned int op3, unsigned int rs1, unsigned int is, unsigned int asi, unsigned int rs2)
#define AC_BEHAVIOR_ldda() leon3_parms::leon3_isa::behavior_ldda(unsigned int op, unsigned int rd, unsigned int op3, unsigned int rs1, unsigned int is, unsigned int asi, unsigned int rs2)
#define AC_BEHAVIOR_stba() leon3_parms::leon3_isa::behavior_stba(unsigned int op, unsigned int rd, unsigned int op3, unsigned int rs1, unsigned int is, unsigned int asi, unsigned int rs2)
#define AC_BEHAVIOR_stha() leon3_parms::leon3_isa::behavior_stha(unsigned int op, unsigned int rd, unsigned int op3, unsigned int rs1, unsigned int is, unsigned int asi, unsigned int rs2)
#define AC_BEHAVIOR_sta() leon3_parms::leon3_isa::behavior_sta(unsigned int op, unsigned int rd, unsigned int op3, unsigned int rs1, unsigned int is, unsigned int asi, unsigned int rs2)
#define AC_BEHAVIOR_stda() leon3_parms::leon3_isa::behavior_stda(unsigned int op, unsigned int rd, unsigned int op3, unsigned int rs1, unsigned int is, unsigned int asi, unsigned int rs2)
#define AC_BEHAVIOR_ldstuba() leon3_parms::leon3_isa::behavior_ldstuba(unsigned int op, unsigned int rd, unsigned int op3, unsigned int rs1, unsigned int is, unsigned int asi, unsigned int rs2)
#define AC_BEHAVIOR_ldsha() leon3_parms::leon3_isa::behavior_ldsha(unsigned int op, unsigned int rd, unsigned int op3, unsigned int rs1, unsigned int is, unsigned int asi, unsigned int rs2)
#define AC_BEHAVIOR_ldsba() leon3_parms::leon3_isa::behavior_ldsba(unsigned int op, unsigned int rd, unsigned int op3, unsigned int rs1, unsigned int is, unsigned int asi, unsigned int rs2)
#define AC_BEHAVIOR_swapa() leon3_parms::leon3_isa::behavior_swapa(unsigned int op, unsigned int rd, unsigned int op3, unsigned int rs1, unsigned int is, unsigned int asi, unsigned int rs2)
#define AC_BEHAVIOR_stb_reg() leon3_parms::leon3_isa::behavior_stb_reg(unsigned int op, unsigned int rd, unsigned int op3, unsigned int rs1, unsigned int is, unsigned int asi, unsigned int rs2)
#define AC_BEHAVIOR_sth_reg() leon3_parms::leon3_isa::behavior_sth_reg(unsigned int op, unsigned int rd, unsigned int op3, unsigned int rs1, unsigned int is, unsigned int asi, unsigned int rs2)
#define AC_BEHAVIOR_st_reg() leon3_parms::leon3_isa::behavior_st_reg(unsigned int op, unsigned int rd, unsigned int op3, unsigned int rs1, unsigned int is, unsigned int asi, unsigned int rs2)
#define AC_BEHAVIOR_std_reg() leon3_parms::leon3_isa::behavior_std_reg(unsigned int op, unsigned int rd, unsigned int op3, unsigned int rs1, unsigned int is, unsigned int asi, unsigned int rs2)
#define AC_BEHAVIOR_ldstub_reg() leon3_parms::leon3_isa::behavior_ldstub_reg(unsigned int op, unsigned int rd, unsigned int op3, unsigned int rs1, unsigned int is, unsigned int asi, unsigned int rs2)
#define AC_BEHAVIOR_swap_reg() leon3_parms::leon3_isa::behavior_swap_reg(unsigned int op, unsigned int rd, unsigned int op3, unsigned int rs1, unsigned int is, unsigned int asi, unsigned int rs2)
#define AC_BEHAVIOR_sll_reg() leon3_parms::leon3_isa::behavior_sll_reg(unsigned int op, unsigned int rd, unsigned int op3, unsigned int rs1, unsigned int is, unsigned int asi, unsigned int rs2)
#define AC_BEHAVIOR_srl_reg() leon3_parms::leon3_isa::behavior_srl_reg(unsigned int op, unsigned int rd, unsigned int op3, unsigned int rs1, unsigned int is, unsigned int asi, unsigned int rs2)
#define AC_BEHAVIOR_sra_reg() leon3_parms::leon3_isa::behavior_sra_reg(unsigned int op, unsigned int rd, unsigned int op3, unsigned int rs1, unsigned int is, unsigned int asi, unsigned int rs2)
#define AC_BEHAVIOR_add_reg() leon3_parms::leon3_isa::behavior_add_reg(unsigned int op, unsigned int rd, unsigned int op3, unsigned int rs1, unsigned int is, unsigned int asi, unsigned int rs2)
#define AC_BEHAVIOR_addcc_reg() leon3_parms::leon3_isa::behavior_addcc_reg(unsigned int op, unsigned int rd, unsigned int op3, unsigned int rs1, unsigned int is, unsigned int asi, unsigned int rs2)
#define AC_BEHAVIOR_addx_reg() leon3_parms::leon3_isa::behavior_addx_reg(unsigned int op, unsigned int rd, unsigned int op3, unsigned int rs1, unsigned int is, unsigned int asi, unsigned int rs2)
#define AC_BEHAVIOR_addxcc_reg() leon3_parms::leon3_isa::behavior_addxcc_reg(unsigned int op, unsigned int rd, unsigned int op3, unsigned int rs1, unsigned int is, unsigned int asi, unsigned int rs2)
#define AC_BEHAVIOR_taddcc_reg() leon3_parms::leon3_isa::behavior_taddcc_reg(unsigned int op, unsigned int rd, unsigned int op3, unsigned int rs1, unsigned int is, unsigned int asi, unsigned int rs2)
#define AC_BEHAVIOR_taddcctv_reg() leon3_parms::leon3_isa::behavior_taddcctv_reg(unsigned int op, unsigned int rd, unsigned int op3, unsigned int rs1, unsigned int is, unsigned int asi, unsigned int rs2)
#define AC_BEHAVIOR_sub_reg() leon3_parms::leon3_isa::behavior_sub_reg(unsigned int op, unsigned int rd, unsigned int op3, unsigned int rs1, unsigned int is, unsigned int asi, unsigned int rs2)
#define AC_BEHAVIOR_subcc_reg() leon3_parms::leon3_isa::behavior_subcc_reg(unsigned int op, unsigned int rd, unsigned int op3, unsigned int rs1, unsigned int is, unsigned int asi, unsigned int rs2)
#define AC_BEHAVIOR_subx_reg() leon3_parms::leon3_isa::behavior_subx_reg(unsigned int op, unsigned int rd, unsigned int op3, unsigned int rs1, unsigned int is, unsigned int asi, unsigned int rs2)
#define AC_BEHAVIOR_subxcc_reg() leon3_parms::leon3_isa::behavior_subxcc_reg(unsigned int op, unsigned int rd, unsigned int op3, unsigned int rs1, unsigned int is, unsigned int asi, unsigned int rs2)
#define AC_BEHAVIOR_tsubcc_reg() leon3_parms::leon3_isa::behavior_tsubcc_reg(unsigned int op, unsigned int rd, unsigned int op3, unsigned int rs1, unsigned int is, unsigned int asi, unsigned int rs2)
#define AC_BEHAVIOR_tsubcctv_reg() leon3_parms::leon3_isa::behavior_tsubcctv_reg(unsigned int op, unsigned int rd, unsigned int op3, unsigned int rs1, unsigned int is, unsigned int asi, unsigned int rs2)
#define AC_BEHAVIOR_and_reg() leon3_parms::leon3_isa::behavior_and_reg(unsigned int op, unsigned int rd, unsigned int op3, unsigned int rs1, unsigned int is, unsigned int asi, unsigned int rs2)
#define AC_BEHAVIOR_andcc_reg() leon3_parms::leon3_isa::behavior_andcc_reg(unsigned int op, unsigned int rd, unsigned int op3, unsigned int rs1, unsigned int is, unsigned int asi, unsigned int rs2)
#define AC_BEHAVIOR_andn_reg() leon3_parms::leon3_isa::behavior_andn_reg(unsigned int op, unsigned int rd, unsigned int op3, unsigned int rs1, unsigned int is, unsigned int asi, unsigned int rs2)
#define AC_BEHAVIOR_andncc_reg() leon3_parms::leon3_isa::behavior_andncc_reg(unsigned int op, unsigned int rd, unsigned int op3, unsigned int rs1, unsigned int is, unsigned int asi, unsigned int rs2)
#define AC_BEHAVIOR_or_reg() leon3_parms::leon3_isa::behavior_or_reg(unsigned int op, unsigned int rd, unsigned int op3, unsigned int rs1, unsigned int is, unsigned int asi, unsigned int rs2)
#define AC_BEHAVIOR_orcc_reg() leon3_parms::leon3_isa::behavior_orcc_reg(unsigned int op, unsigned int rd, unsigned int op3, unsigned int rs1, unsigned int is, unsigned int asi, unsigned int rs2)
#define AC_BEHAVIOR_orn_reg() leon3_parms::leon3_isa::behavior_orn_reg(unsigned int op, unsigned int rd, unsigned int op3, unsigned int rs1, unsigned int is, unsigned int asi, unsigned int rs2)
#define AC_BEHAVIOR_orncc_reg() leon3_parms::leon3_isa::behavior_orncc_reg(unsigned int op, unsigned int rd, unsigned int op3, unsigned int rs1, unsigned int is, unsigned int asi, unsigned int rs2)
#define AC_BEHAVIOR_xor_reg() leon3_parms::leon3_isa::behavior_xor_reg(unsigned int op, unsigned int rd, unsigned int op3, unsigned int rs1, unsigned int is, unsigned int asi, unsigned int rs2)
#define AC_BEHAVIOR_xorcc_reg() leon3_parms::leon3_isa::behavior_xorcc_reg(unsigned int op, unsigned int rd, unsigned int op3, unsigned int rs1, unsigned int is, unsigned int asi, unsigned int rs2)
#define AC_BEHAVIOR_xnor_reg() leon3_parms::leon3_isa::behavior_xnor_reg(unsigned int op, unsigned int rd, unsigned int op3, unsigned int rs1, unsigned int is, unsigned int asi, unsigned int rs2)
#define AC_BEHAVIOR_xnorcc_reg() leon3_parms::leon3_isa::behavior_xnorcc_reg(unsigned int op, unsigned int rd, unsigned int op3, unsigned int rs1, unsigned int is, unsigned int asi, unsigned int rs2)
#define AC_BEHAVIOR_save_reg() leon3_parms::leon3_isa::behavior_save_reg(unsigned int op, unsigned int rd, unsigned int op3, unsigned int rs1, unsigned int is, unsigned int asi, unsigned int rs2)
#define AC_BEHAVIOR_restore_reg() leon3_parms::leon3_isa::behavior_restore_reg(unsigned int op, unsigned int rd, unsigned int op3, unsigned int rs1, unsigned int is, unsigned int asi, unsigned int rs2)
#define AC_BEHAVIOR_umul_reg() leon3_parms::leon3_isa::behavior_umul_reg(unsigned int op, unsigned int rd, unsigned int op3, unsigned int rs1, unsigned int is, unsigned int asi, unsigned int rs2)
#define AC_BEHAVIOR_smul_reg() leon3_parms::leon3_isa::behavior_smul_reg(unsigned int op, unsigned int rd, unsigned int op3, unsigned int rs1, unsigned int is, unsigned int asi, unsigned int rs2)
#define AC_BEHAVIOR_umulcc_reg() leon3_parms::leon3_isa::behavior_umulcc_reg(unsigned int op, unsigned int rd, unsigned int op3, unsigned int rs1, unsigned int is, unsigned int asi, unsigned int rs2)
#define AC_BEHAVIOR_smulcc_reg() leon3_parms::leon3_isa::behavior_smulcc_reg(unsigned int op, unsigned int rd, unsigned int op3, unsigned int rs1, unsigned int is, unsigned int asi, unsigned int rs2)
#define AC_BEHAVIOR_mulscc_reg() leon3_parms::leon3_isa::behavior_mulscc_reg(unsigned int op, unsigned int rd, unsigned int op3, unsigned int rs1, unsigned int is, unsigned int asi, unsigned int rs2)
#define AC_BEHAVIOR_udiv_reg() leon3_parms::leon3_isa::behavior_udiv_reg(unsigned int op, unsigned int rd, unsigned int op3, unsigned int rs1, unsigned int is, unsigned int asi, unsigned int rs2)
#define AC_BEHAVIOR_udivcc_reg() leon3_parms::leon3_isa::behavior_udivcc_reg(unsigned int op, unsigned int rd, unsigned int op3, unsigned int rs1, unsigned int is, unsigned int asi, unsigned int rs2)
#define AC_BEHAVIOR_sdiv_reg() leon3_parms::leon3_isa::behavior_sdiv_reg(unsigned int op, unsigned int rd, unsigned int op3, unsigned int rs1, unsigned int is, unsigned int asi, unsigned int rs2)
#define AC_BEHAVIOR_sdivcc_reg() leon3_parms::leon3_isa::behavior_sdivcc_reg(unsigned int op, unsigned int rd, unsigned int op3, unsigned int rs1, unsigned int is, unsigned int asi, unsigned int rs2)
#define AC_BEHAVIOR_jmpl_reg() leon3_parms::leon3_isa::behavior_jmpl_reg(unsigned int op, unsigned int rd, unsigned int op3, unsigned int rs1, unsigned int is, unsigned int asi, unsigned int rs2)
#define AC_BEHAVIOR_wry_reg() leon3_parms::leon3_isa::behavior_wry_reg(unsigned int op, unsigned int rd, unsigned int op3, unsigned int rs1, unsigned int is, unsigned int asi, unsigned int rs2)
#define AC_BEHAVIOR_wrtbr_reg() leon3_parms::leon3_isa::behavior_wrtbr_reg(unsigned int op, unsigned int rd, unsigned int op3, unsigned int rs1, unsigned int is, unsigned int asi, unsigned int rs2)
#define AC_BEHAVIOR_wrpsr_reg() leon3_parms::leon3_isa::behavior_wrpsr_reg(unsigned int op, unsigned int rd, unsigned int op3, unsigned int rs1, unsigned int is, unsigned int asi, unsigned int rs2)
#define AC_BEHAVIOR_wrwim_reg() leon3_parms::leon3_isa::behavior_wrwim_reg(unsigned int op, unsigned int rd, unsigned int op3, unsigned int rs1, unsigned int is, unsigned int asi, unsigned int rs2)
#define AC_BEHAVIOR_rett_reg() leon3_parms::leon3_isa::behavior_rett_reg(unsigned int op, unsigned int rd, unsigned int op3, unsigned int rs1, unsigned int is, unsigned int asi, unsigned int rs2)
#define AC_BEHAVIOR_flush_reg() leon3_parms::leon3_isa::behavior_flush_reg(unsigned int op, unsigned int rd, unsigned int op3, unsigned int rs1, unsigned int is, unsigned int asi, unsigned int rs2)
#define AC_BEHAVIOR_ldfsr_reg() leon3_parms::leon3_isa::behavior_ldfsr_reg(unsigned int op, unsigned int rd, unsigned int op3, unsigned int rs1, unsigned int is, unsigned int asi, unsigned int rs2)
#define AC_BEHAVIOR_ldsb_imm() leon3_parms::leon3_isa::behavior_ldsb_imm(unsigned int op, unsigned int rd, unsigned int op3, unsigned int rs1, unsigned int is, int simm13)
#define AC_BEHAVIOR_ldsh_imm() leon3_parms::leon3_isa::behavior_ldsh_imm(unsigned int op, unsigned int rd, unsigned int op3, unsigned int rs1, unsigned int is, int simm13)
#define AC_BEHAVIOR_ldub_imm() leon3_parms::leon3_isa::behavior_ldub_imm(unsigned int op, unsigned int rd, unsigned int op3, unsigned int rs1, unsigned int is, int simm13)
#define AC_BEHAVIOR_lduh_imm() leon3_parms::leon3_isa::behavior_lduh_imm(unsigned int op, unsigned int rd, unsigned int op3, unsigned int rs1, unsigned int is, int simm13)
#define AC_BEHAVIOR_ld_imm() leon3_parms::leon3_isa::behavior_ld_imm(unsigned int op, unsigned int rd, unsigned int op3, unsigned int rs1, unsigned int is, int simm13)
#define AC_BEHAVIOR_ldd_imm() leon3_parms::leon3_isa::behavior_ldd_imm(unsigned int op, unsigned int rd, unsigned int op3, unsigned int rs1, unsigned int is, int simm13)
#define AC_BEHAVIOR_and_imm() leon3_parms::leon3_isa::behavior_and_imm(unsigned int op, unsigned int rd, unsigned int op3, unsigned int rs1, unsigned int is, int simm13)
#define AC_BEHAVIOR_andcc_imm() leon3_parms::leon3_isa::behavior_andcc_imm(unsigned int op, unsigned int rd, unsigned int op3, unsigned int rs1, unsigned int is, int simm13)
#define AC_BEHAVIOR_andn_imm() leon3_parms::leon3_isa::behavior_andn_imm(unsigned int op, unsigned int rd, unsigned int op3, unsigned int rs1, unsigned int is, int simm13)
#define AC_BEHAVIOR_andncc_imm() leon3_parms::leon3_isa::behavior_andncc_imm(unsigned int op, unsigned int rd, unsigned int op3, unsigned int rs1, unsigned int is, int simm13)
#define AC_BEHAVIOR_or_imm() leon3_parms::leon3_isa::behavior_or_imm(unsigned int op, unsigned int rd, unsigned int op3, unsigned int rs1, unsigned int is, int simm13)
#define AC_BEHAVIOR_orcc_imm() leon3_parms::leon3_isa::behavior_orcc_imm(unsigned int op, unsigned int rd, unsigned int op3, unsigned int rs1, unsigned int is, int simm13)
#define AC_BEHAVIOR_orn_imm() leon3_parms::leon3_isa::behavior_orn_imm(unsigned int op, unsigned int rd, unsigned int op3, unsigned int rs1, unsigned int is, int simm13)
#define AC_BEHAVIOR_orncc_imm() leon3_parms::leon3_isa::behavior_orncc_imm(unsigned int op, unsigned int rd, unsigned int op3, unsigned int rs1, unsigned int is, int simm13)
#define AC_BEHAVIOR_xor_imm() leon3_parms::leon3_isa::behavior_xor_imm(unsigned int op, unsigned int rd, unsigned int op3, unsigned int rs1, unsigned int is, int simm13)
#define AC_BEHAVIOR_xorcc_imm() leon3_parms::leon3_isa::behavior_xorcc_imm(unsigned int op, unsigned int rd, unsigned int op3, unsigned int rs1, unsigned int is, int simm13)
#define AC_BEHAVIOR_xnor_imm() leon3_parms::leon3_isa::behavior_xnor_imm(unsigned int op, unsigned int rd, unsigned int op3, unsigned int rs1, unsigned int is, int simm13)
#define AC_BEHAVIOR_flush_imm() leon3_parms::leon3_isa::behavior_flush_imm(unsigned int op, unsigned int rd, unsigned int op3, unsigned int rs1, unsigned int is, int simm13)
#define AC_BEHAVIOR_xnorcc_imm() leon3_parms::leon3_isa::behavior_xnorcc_imm(unsigned int op, unsigned int rd, unsigned int op3, unsigned int rs1, unsigned int is, int simm13)
#define AC_BEHAVIOR_umul_imm() leon3_parms::leon3_isa::behavior_umul_imm(unsigned int op, unsigned int rd, unsigned int op3, unsigned int rs1, unsigned int is, int simm13)
#define AC_BEHAVIOR_smul_imm() leon3_parms::leon3_isa::behavior_smul_imm(unsigned int op, unsigned int rd, unsigned int op3, unsigned int rs1, unsigned int is, int simm13)
#define AC_BEHAVIOR_umulcc_imm() leon3_parms::leon3_isa::behavior_umulcc_imm(unsigned int op, unsigned int rd, unsigned int op3, unsigned int rs1, unsigned int is, int simm13)
#define AC_BEHAVIOR_smulcc_imm() leon3_parms::leon3_isa::behavior_smulcc_imm(unsigned int op, unsigned int rd, unsigned int op3, unsigned int rs1, unsigned int is, int simm13)
#define AC_BEHAVIOR_mulscc_imm() leon3_parms::leon3_isa::behavior_mulscc_imm(unsigned int op, unsigned int rd, unsigned int op3, unsigned int rs1, unsigned int is, int simm13)
#define AC_BEHAVIOR_udiv_imm() leon3_parms::leon3_isa::behavior_udiv_imm(unsigned int op, unsigned int rd, unsigned int op3, unsigned int rs1, unsigned int is, int simm13)
#define AC_BEHAVIOR_udivcc_imm() leon3_parms::leon3_isa::behavior_udivcc_imm(unsigned int op, unsigned int rd, unsigned int op3, unsigned int rs1, unsigned int is, int simm13)
#define AC_BEHAVIOR_sdiv_imm() leon3_parms::leon3_isa::behavior_sdiv_imm(unsigned int op, unsigned int rd, unsigned int op3, unsigned int rs1, unsigned int is, int simm13)
#define AC_BEHAVIOR_sdivcc_imm() leon3_parms::leon3_isa::behavior_sdivcc_imm(unsigned int op, unsigned int rd, unsigned int op3, unsigned int rs1, unsigned int is, int simm13)
#define AC_BEHAVIOR_rdtbr() leon3_parms::leon3_isa::behavior_rdtbr(unsigned int op, unsigned int rd, unsigned int op3, unsigned int rs1, unsigned int is, int simm13)
#define AC_BEHAVIOR_wrtbr_imm() leon3_parms::leon3_isa::behavior_wrtbr_imm(unsigned int op, unsigned int rd, unsigned int op3, unsigned int rs1, unsigned int is, int simm13)
#define AC_BEHAVIOR_rdpsr() leon3_parms::leon3_isa::behavior_rdpsr(unsigned int op, unsigned int rd, unsigned int op3, unsigned int rs1, unsigned int is, int simm13)
#define AC_BEHAVIOR_wrpsr_imm() leon3_parms::leon3_isa::behavior_wrpsr_imm(unsigned int op, unsigned int rd, unsigned int op3, unsigned int rs1, unsigned int is, int simm13)
#define AC_BEHAVIOR_rdwim() leon3_parms::leon3_isa::behavior_rdwim(unsigned int op, unsigned int rd, unsigned int op3, unsigned int rs1, unsigned int is, int simm13)
#define AC_BEHAVIOR_wrwim_imm() leon3_parms::leon3_isa::behavior_wrwim_imm(unsigned int op, unsigned int rd, unsigned int op3, unsigned int rs1, unsigned int is, int simm13)
#define AC_BEHAVIOR_rett_imm() leon3_parms::leon3_isa::behavior_rett_imm(unsigned int op, unsigned int rd, unsigned int op3, unsigned int rs1, unsigned int is, int simm13)
#define AC_BEHAVIOR_ldfsr_imm() leon3_parms::leon3_isa::behavior_ldfsr_imm(unsigned int op, unsigned int rd, unsigned int op3, unsigned int rs1, unsigned int is, int simm13)
#define AC_BEHAVIOR_stb_imm() leon3_parms::leon3_isa::behavior_stb_imm(unsigned int op, unsigned int rd, unsigned int op3, unsigned int rs1, unsigned int is, int simm13)
#define AC_BEHAVIOR_sth_imm() leon3_parms::leon3_isa::behavior_sth_imm(unsigned int op, unsigned int rd, unsigned int op3, unsigned int rs1, unsigned int is, int simm13)
#define AC_BEHAVIOR_st_imm() leon3_parms::leon3_isa::behavior_st_imm(unsigned int op, unsigned int rd, unsigned int op3, unsigned int rs1, unsigned int is, int simm13)
#define AC_BEHAVIOR_std_imm() leon3_parms::leon3_isa::behavior_std_imm(unsigned int op, unsigned int rd, unsigned int op3, unsigned int rs1, unsigned int is, int simm13)
#define AC_BEHAVIOR_ldstub_imm() leon3_parms::leon3_isa::behavior_ldstub_imm(unsigned int op, unsigned int rd, unsigned int op3, unsigned int rs1, unsigned int is, int simm13)
#define AC_BEHAVIOR_swap_imm() leon3_parms::leon3_isa::behavior_swap_imm(unsigned int op, unsigned int rd, unsigned int op3, unsigned int rs1, unsigned int is, int simm13)
#define AC_BEHAVIOR_sll_imm() leon3_parms::leon3_isa::behavior_sll_imm(unsigned int op, unsigned int rd, unsigned int op3, unsigned int rs1, unsigned int is, int simm13)
#define AC_BEHAVIOR_srl_imm() leon3_parms::leon3_isa::behavior_srl_imm(unsigned int op, unsigned int rd, unsigned int op3, unsigned int rs1, unsigned int is, int simm13)
#define AC_BEHAVIOR_sra_imm() leon3_parms::leon3_isa::behavior_sra_imm(unsigned int op, unsigned int rd, unsigned int op3, unsigned int rs1, unsigned int is, int simm13)
#define AC_BEHAVIOR_add_imm() leon3_parms::leon3_isa::behavior_add_imm(unsigned int op, unsigned int rd, unsigned int op3, unsigned int rs1, unsigned int is, int simm13)
#define AC_BEHAVIOR_addcc_imm() leon3_parms::leon3_isa::behavior_addcc_imm(unsigned int op, unsigned int rd, unsigned int op3, unsigned int rs1, unsigned int is, int simm13)
#define AC_BEHAVIOR_addx_imm() leon3_parms::leon3_isa::behavior_addx_imm(unsigned int op, unsigned int rd, unsigned int op3, unsigned int rs1, unsigned int is, int simm13)
#define AC_BEHAVIOR_addxcc_imm() leon3_parms::leon3_isa::behavior_addxcc_imm(unsigned int op, unsigned int rd, unsigned int op3, unsigned int rs1, unsigned int is, int simm13)
#define AC_BEHAVIOR_taddcc_imm() leon3_parms::leon3_isa::behavior_taddcc_imm(unsigned int op, unsigned int rd, unsigned int op3, unsigned int rs1, unsigned int is, int simm13)
#define AC_BEHAVIOR_taddcctv_imm() leon3_parms::leon3_isa::behavior_taddcctv_imm(unsigned int op, unsigned int rd, unsigned int op3, unsigned int rs1, unsigned int is, int simm13)
#define AC_BEHAVIOR_sub_imm() leon3_parms::leon3_isa::behavior_sub_imm(unsigned int op, unsigned int rd, unsigned int op3, unsigned int rs1, unsigned int is, int simm13)
#define AC_BEHAVIOR_subcc_imm() leon3_parms::leon3_isa::behavior_subcc_imm(unsigned int op, unsigned int rd, unsigned int op3, unsigned int rs1, unsigned int is, int simm13)
#define AC_BEHAVIOR_subx_imm() leon3_parms::leon3_isa::behavior_subx_imm(unsigned int op, unsigned int rd, unsigned int op3, unsigned int rs1, unsigned int is, int simm13)
#define AC_BEHAVIOR_subxcc_imm() leon3_parms::leon3_isa::behavior_subxcc_imm(unsigned int op, unsigned int rd, unsigned int op3, unsigned int rs1, unsigned int is, int simm13)
#define AC_BEHAVIOR_tsubcc_imm() leon3_parms::leon3_isa::behavior_tsubcc_imm(unsigned int op, unsigned int rd, unsigned int op3, unsigned int rs1, unsigned int is, int simm13)
#define AC_BEHAVIOR_tsubcctv_imm() leon3_parms::leon3_isa::behavior_tsubcctv_imm(unsigned int op, unsigned int rd, unsigned int op3, unsigned int rs1, unsigned int is, int simm13)
#define AC_BEHAVIOR_jmpl_imm() leon3_parms::leon3_isa::behavior_jmpl_imm(unsigned int op, unsigned int rd, unsigned int op3, unsigned int rs1, unsigned int is, int simm13)
#define AC_BEHAVIOR_save_imm() leon3_parms::leon3_isa::behavior_save_imm(unsigned int op, unsigned int rd, unsigned int op3, unsigned int rs1, unsigned int is, int simm13)
#define AC_BEHAVIOR_restore_imm() leon3_parms::leon3_isa::behavior_restore_imm(unsigned int op, unsigned int rd, unsigned int op3, unsigned int rs1, unsigned int is, int simm13)
#define AC_BEHAVIOR_rdy() leon3_parms::leon3_isa::behavior_rdy(unsigned int op, unsigned int rd, unsigned int op3, unsigned int rs1, unsigned int is, int simm13)
#define AC_BEHAVIOR_wry_imm() leon3_parms::leon3_isa::behavior_wry_imm(unsigned int op, unsigned int rd, unsigned int op3, unsigned int rs1, unsigned int is, int simm13)
#define AC_BEHAVIOR_unimplemented() leon3_parms::leon3_isa::behavior_unimplemented(unsigned int op, unsigned int rd, unsigned int op2, unsigned int imm22)
#define AC_BEHAVIOR_ta() leon3_parms::leon3_isa::behavior_ta(unsigned int op, unsigned int r1, unsigned int cond, unsigned int op2a, unsigned int rs1, unsigned int is, unsigned int r2a, unsigned int rs2)
#define AC_BEHAVIOR_tn() leon3_parms::leon3_isa::behavior_tn(unsigned int op, unsigned int r1, unsigned int cond, unsigned int op2a, unsigned int rs1, unsigned int is, unsigned int r2a, unsigned int rs2)
#define AC_BEHAVIOR_tne() leon3_parms::leon3_isa::behavior_tne(unsigned int op, unsigned int r1, unsigned int cond, unsigned int op2a, unsigned int rs1, unsigned int is, unsigned int r2a, unsigned int rs2)
#define AC_BEHAVIOR_te() leon3_parms::leon3_isa::behavior_te(unsigned int op, unsigned int r1, unsigned int cond, unsigned int op2a, unsigned int rs1, unsigned int is, unsigned int r2a, unsigned int rs2)
#define AC_BEHAVIOR_tg() leon3_parms::leon3_isa::behavior_tg(unsigned int op, unsigned int r1, unsigned int cond, unsigned int op2a, unsigned int rs1, unsigned int is, unsigned int r2a, unsigned int rs2)
#define AC_BEHAVIOR_tle() leon3_parms::leon3_isa::behavior_tle(unsigned int op, unsigned int r1, unsigned int cond, unsigned int op2a, unsigned int rs1, unsigned int is, unsigned int r2a, unsigned int rs2)
#define AC_BEHAVIOR_tge() leon3_parms::leon3_isa::behavior_tge(unsigned int op, unsigned int r1, unsigned int cond, unsigned int op2a, unsigned int rs1, unsigned int is, unsigned int r2a, unsigned int rs2)
#define AC_BEHAVIOR_tl() leon3_parms::leon3_isa::behavior_tl(unsigned int op, unsigned int r1, unsigned int cond, unsigned int op2a, unsigned int rs1, unsigned int is, unsigned int r2a, unsigned int rs2)
#define AC_BEHAVIOR_tgu() leon3_parms::leon3_isa::behavior_tgu(unsigned int op, unsigned int r1, unsigned int cond, unsigned int op2a, unsigned int rs1, unsigned int is, unsigned int r2a, unsigned int rs2)
#define AC_BEHAVIOR_tleu() leon3_parms::leon3_isa::behavior_tleu(unsigned int op, unsigned int r1, unsigned int cond, unsigned int op2a, unsigned int rs1, unsigned int is, unsigned int r2a, unsigned int rs2)
#define AC_BEHAVIOR_tcc() leon3_parms::leon3_isa::behavior_tcc(unsigned int op, unsigned int r1, unsigned int cond, unsigned int op2a, unsigned int rs1, unsigned int is, unsigned int r2a, unsigned int rs2)
#define AC_BEHAVIOR_tcs() leon3_parms::leon3_isa::behavior_tcs(unsigned int op, unsigned int r1, unsigned int cond, unsigned int op2a, unsigned int rs1, unsigned int is, unsigned int r2a, unsigned int rs2)
#define AC_BEHAVIOR_tpos() leon3_parms::leon3_isa::behavior_tpos(unsigned int op, unsigned int r1, unsigned int cond, unsigned int op2a, unsigned int rs1, unsigned int is, unsigned int r2a, unsigned int rs2)
#define AC_BEHAVIOR_tneg() leon3_parms::leon3_isa::behavior_tneg(unsigned int op, unsigned int r1, unsigned int cond, unsigned int op2a, unsigned int rs1, unsigned int is, unsigned int r2a, unsigned int rs2)
#define AC_BEHAVIOR_tvc() leon3_parms::leon3_isa::behavior_tvc(unsigned int op, unsigned int r1, unsigned int cond, unsigned int op2a, unsigned int rs1, unsigned int is, unsigned int r2a, unsigned int rs2)
#define AC_BEHAVIOR_tvs() leon3_parms::leon3_isa::behavior_tvs(unsigned int op, unsigned int r1, unsigned int cond, unsigned int op2a, unsigned int rs1, unsigned int is, unsigned int r2a, unsigned int rs2)
#define AC_BEHAVIOR_ta_imm() leon3_parms::leon3_isa::behavior_ta_imm(unsigned int op, unsigned int r1, unsigned int cond, unsigned int op2a, unsigned int rs1, unsigned int is, unsigned int r2a, unsigned int imm7)
#define AC_BEHAVIOR_tn_imm() leon3_parms::leon3_isa::behavior_tn_imm(unsigned int op, unsigned int r1, unsigned int cond, unsigned int op2a, unsigned int rs1, unsigned int is, unsigned int r2a, unsigned int imm7)
#define AC_BEHAVIOR_tne_imm() leon3_parms::leon3_isa::behavior_tne_imm(unsigned int op, unsigned int r1, unsigned int cond, unsigned int op2a, unsigned int rs1, unsigned int is, unsigned int r2a, unsigned int imm7)
#define AC_BEHAVIOR_te_imm() leon3_parms::leon3_isa::behavior_te_imm(unsigned int op, unsigned int r1, unsigned int cond, unsigned int op2a, unsigned int rs1, unsigned int is, unsigned int r2a, unsigned int imm7)
#define AC_BEHAVIOR_tg_imm() leon3_parms::leon3_isa::behavior_tg_imm(unsigned int op, unsigned int r1, unsigned int cond, unsigned int op2a, unsigned int rs1, unsigned int is, unsigned int r2a, unsigned int imm7)
#define AC_BEHAVIOR_tle_imm() leon3_parms::leon3_isa::behavior_tle_imm(unsigned int op, unsigned int r1, unsigned int cond, unsigned int op2a, unsigned int rs1, unsigned int is, unsigned int r2a, unsigned int imm7)
#define AC_BEHAVIOR_tge_imm() leon3_parms::leon3_isa::behavior_tge_imm(unsigned int op, unsigned int r1, unsigned int cond, unsigned int op2a, unsigned int rs1, unsigned int is, unsigned int r2a, unsigned int imm7)
#define AC_BEHAVIOR_tl_imm() leon3_parms::leon3_isa::behavior_tl_imm(unsigned int op, unsigned int r1, unsigned int cond, unsigned int op2a, unsigned int rs1, unsigned int is, unsigned int r2a, unsigned int imm7)
#define AC_BEHAVIOR_tgu_imm() leon3_parms::leon3_isa::behavior_tgu_imm(unsigned int op, unsigned int r1, unsigned int cond, unsigned int op2a, unsigned int rs1, unsigned int is, unsigned int r2a, unsigned int imm7)
#define AC_BEHAVIOR_tleu_imm() leon3_parms::leon3_isa::behavior_tleu_imm(unsigned int op, unsigned int r1, unsigned int cond, unsigned int op2a, unsigned int rs1, unsigned int is, unsigned int r2a, unsigned int imm7)
#define AC_BEHAVIOR_tcc_imm() leon3_parms::leon3_isa::behavior_tcc_imm(unsigned int op, unsigned int r1, unsigned int cond, unsigned int op2a, unsigned int rs1, unsigned int is, unsigned int r2a, unsigned int imm7)
#define AC_BEHAVIOR_tcs_imm() leon3_parms::leon3_isa::behavior_tcs_imm(unsigned int op, unsigned int r1, unsigned int cond, unsigned int op2a, unsigned int rs1, unsigned int is, unsigned int r2a, unsigned int imm7)
#define AC_BEHAVIOR_tpos_imm() leon3_parms::leon3_isa::behavior_tpos_imm(unsigned int op, unsigned int r1, unsigned int cond, unsigned int op2a, unsigned int rs1, unsigned int is, unsigned int r2a, unsigned int imm7)
#define AC_BEHAVIOR_tneg_imm() leon3_parms::leon3_isa::behavior_tneg_imm(unsigned int op, unsigned int r1, unsigned int cond, unsigned int op2a, unsigned int rs1, unsigned int is, unsigned int r2a, unsigned int imm7)
#define AC_BEHAVIOR_tvc_imm() leon3_parms::leon3_isa::behavior_tvc_imm(unsigned int op, unsigned int r1, unsigned int cond, unsigned int op2a, unsigned int rs1, unsigned int is, unsigned int r2a, unsigned int imm7)
#define AC_BEHAVIOR_tvs_imm() leon3_parms::leon3_isa::behavior_tvs_imm(unsigned int op, unsigned int r1, unsigned int cond, unsigned int op2a, unsigned int rs1, unsigned int is, unsigned int r2a, unsigned int imm7)


#endif //_LEON3_BHV_MACROS_H

