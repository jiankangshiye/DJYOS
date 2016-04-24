//----------------------------------------------------
// Copyright (c) 2014, SHENZHEN PENGRUI SOFT CO LTD. All rights reserved.

// Redistribution and use in source and binary forms, with or without 
// modification, are permitted provided that the following conditions are met:

// 1. Redistributions of source code must retain the above copyright notice, 
//    this list of conditions and the following disclaimer.
// 2. Redistributions in binary form must reproduce the above copyright notice, 
//    this list of conditions and the following disclaimer in the documentation 
//    and/or other materials provided with the distribution.

// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" 
// AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE 
// IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE 
// ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE 
// LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
// CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
// SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
// INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN 
// CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
// ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
// POSSIBILITY OF SUCH DAMAGE.
//-----------------------------------------------------------------------------
// Copyright (c) 2014 ����Ȩ�����������������޹�˾���С�����Ȩ�˱���һ��Ȩ����
// 
// �����Ȩ�����ʹ���߷������������������£�����ʹ����ʹ�ü���ɢ����
// ������װԭʼ�뼰����λ��ִ����ʽ��Ȩ�������۴˰�װ�Ƿ񾭸�����Ȼ��
// 
// 1. ���ڱ�����Դ�������ɢ�������뱣�������İ�Ȩ���桢�������б�����
//    ������������������
// 2. ���ڱ��׼�����λ��ִ����ʽ����ɢ���������������ļ��Լ�������������
//    ��ɢ����װ�е�ý�鷽ʽ����������֮��Ȩ���桢�������б����Լ�����
//    ������������

// �����������������Ǳ�������Ȩ�������Լ�����������״��"as is"���ṩ��
// ��������װ�����κ���ʾ��Ĭʾ֮�������Σ������������ھ��������Լ��ض�Ŀ
// �ĵ�������ΪĬʾ�Ե�������Ȩ�����˼�������֮�����ߣ������κ�������
// ���۳�����κ��������塢���۴�����Ϊ���Լ��ϵ���޹�ʧ������������Υ
// Լ֮��Ȩ��������ʧ������ԭ��ȣ����𣬶����κ���ʹ�ñ�������װ��������
// �κ�ֱ���ԡ�����ԡ�ż���ԡ������ԡ��ͷ��Ի��κν�����𺦣�����������
// �������Ʒ������֮���á�ʹ����ʧ��������ʧ��������ʧ��ҵ���жϵȵȣ���
// �����κ����Σ����ڸ���ʹ���ѻ���ǰ��֪���ܻ���ɴ����𺦵���������Ȼ��
//-----------------------------------------------------------------------------
// ģ������:�쳣������
// ģ��汾: V1.00(��ʼ���汾)
// ������Ա:Administrator
// ����ʱ��:2013-7-26--����10:15:54
//         ����ģ��ʵ�ֵ��ǻ������еĵ�һ��ָ��
// =============================================================================
// �����޸ļ�¼(���µķ�����ǰ��):
// <�汾��> <�޸�����>, <�޸���Ա>: <�޸Ĺ��ܸ���>
// =============================================================================

.file   "Exp_Vec.S"
#include  "E500v2Reg.h"
#include  "E500v2RegBits.h"
#include  "VMStack.h"
 /*
@-------------------------------------------------------------------------------
@����: �Ӵ˿�ʼ������DJY�������ʼ��CPU������Ҫ��׼��
@����: ��
@����: ��
@-------------------------------------------------------------------------------
*/

.macro  vector_align
    .align  8
    .endm

/* �쳣������ */
.align  8
.global   exception_tab
exception_tab:
.long 0x12345678
.align  8

.global IVOR0_OFFSET
IVOR0_OFFSET:
    b     .
    mtspr  spr4, sp
    xor    sp, sp, sp
    li     sp, cn_majorexp_critical_input@l
    mtspr  spr5, sp
    lis    sp, cint_exp_top@h
    ori    sp, sp, cint_exp_top@l
    subi   sp, sp, _IRQ_TOP_OFFSET
    b   Default_cint_exception
    /*CRITICAL INPUT*/

    vector_align
.global IVOR1_OFFSET
IVOR1_OFFSET:
    b  .
    mtspr  spr4, sp
    xor    sp, sp, sp
    li     sp, cn_majorexp_machine_check@l
    mtspr  spr5, sp
    lis    sp, sys_exp_top@h
    ori    sp, sp, sys_exp_top@l
    subi   sp, sp, _IRQ_TOP_OFFSET
    b   Default_machinecheck_exception
    /*MACHINE CHECK */

    vector_align
.global IVOR2_OFFSET
IVOR2_OFFSET:
    b  .
    mtspr  spr4, sp
    xor    sp, sp, sp
    li     sp, cn_majorexp_data_storage@l
    mtspr  spr5, sp
    lis    sp, sys_exp_top@h
    ori    sp, sp, sys_exp_top@l
    subi   sp, sp, _IRQ_TOP_OFFSET
    b   Default_sys_general_exception
     /*DATA STORAGE */

    vector_align
.global IVOR3_OFFSET
IVOR3_OFFSET:
    b  .
    mtspr  spr4, sp
    xor    sp, sp, sp
    li     sp, cn_majorexp_instruction_storage@l
    mtspr  spr5, sp
    lis    sp, sys_exp_top@h
    ori    sp, sp, sys_exp_top@l
    subi   sp, sp, _IRQ_TOP_OFFSET
    b   Default_sys_general_exception
    /*INSTRUCTION STORAGE */

    vector_align
.global IVOR4_OFFSET
IVOR4_OFFSET:
    b   Default_externalint_exception
    /*EXTERNAL INT EXCEPTION */

    vector_align
.global IVOR5_OFFSET
IVOR5_OFFSET:
    b  .
    mtspr  spr4, sp
    xor    sp, sp, sp
    li     sp, cn_majorexp_alignment@l
    mtspr  spr5, sp
    lis    sp, sys_exp_top@h
    ori    sp, sp, sys_exp_top@l
    subi   sp, sp, _IRQ_TOP_OFFSET
    b   Default_sys_general_exception
    /*ALIGNMENT EXCEPTION */

    vector_align
.global IVOR6_OFFSET
IVOR6_OFFSET:
    b      .
    mtspr  spr4, sp
    xor    sp, sp, sp
    li     sp, cn_majorexp_program@l
    mtspr  spr5, sp
    lis    sp, sys_exp_top@h
    ori    sp, sp, sys_exp_top@l
    subi   sp, sp, _IRQ_TOP_OFFSET
    b   Default_sys_general_exception
    /*PROGRAM EXCEPTION*/

    vector_align
.global IVOR7_OFFSET
IVOR7_OFFSET:

    mtspr  spr4, sp
    xor    sp, sp, sp
    li     sp, cn_majorexp_reserved_7@l
    mtspr  spr5, sp
    lis    sp, sys_exp_top@h
    ori    sp, sp, sys_exp_top@l
    subi   sp, sp, _IRQ_TOP_OFFSET
    b   Default_sys_general_exception
    /*RESERVED  EXCEPTION*/

    vector_align
.global IVOR8_OFFSET
IVOR8_OFFSET:
    b  .
    mtspr  spr4, sp
    xor    sp, sp, sp
    li     sp, cn_majorexp_syscall@l
    mtspr  spr5, sp
    lis    sp, sys_exp_top@h
    ori    sp, sp, sys_exp_top@l
    subi   sp, sp, _IRQ_TOP_OFFSET
    b   Default_sys_general_exception
    /*SYSTEM CALL EXCEPTION*/

    vector_align
.global IVOR9_OFFSET
IVOR9_OFFSET:
    b  .
    mtspr  spr4, sp
    xor    sp, sp, sp
    li     sp, cn_majorexp_reserved_9@l
    mtspr  spr5, sp
    lis    sp, sys_exp_top@h
    ori    sp, sp, sys_exp_top@l
    subi   sp, sp, _IRQ_TOP_OFFSET
    b   Default_sys_general_exception
     /* RESERVED EXCEPTION */

    vector_align
.global IVOR10_OFFSET
IVOR10_OFFSET:
    b   Default_tick_exception
    /*DECREMENT TIMER EXCEPTION*/

    vector_align
.global IVOR11_OFFSET
IVOR11_OFFSET:

    mtspr  spr4, sp
    xor    sp, sp, sp
    li     sp, cn_majorexp_fixed_timer@l
    mtspr  spr5, sp
    lis    sp, sys_exp_top@h
    ori    sp, sp, sys_exp_top@l
    subi   sp, sp, _IRQ_TOP_OFFSET
    b   .
    /*FIXED TIMER EXCEPTION*/

    vector_align
.global IVOR12_OFFSET
IVOR12_OFFSET:
    mtspr  spr4, sp
    xor    sp, sp, sp
    li     sp, cn_majorexp_watch_dog_timer@l
    mtspr  spr5, sp
    lis    sp, cint_exp_top@h
    ori    sp, sp, cint_exp_top@l
    subi   sp, sp, _IRQ_TOP_OFFSET
    b      .  //not implement yet--TODO
    /*WATCH TIMER EXCEPTION*/

    vector_align
.global IVOR13_OFFSET
IVOR13_OFFSET:
    b  .
    mtspr  spr4, sp
    xor    sp, sp, sp
    li     sp, cn_majorexp_data_tlb_error@l
    mtspr  spr5, sp
    lis    sp, sys_exp_top@h
    ori    sp, sp, sys_exp_top@l
    subi   sp, sp, _IRQ_TOP_OFFSET
    b   Default_sys_general_exception
    /*DATA TLB ERROR EXCEPTION*/

    vector_align
.global IVOR14_OFFSET
IVOR14_OFFSET:
    b  .
    mtspr  spr4, sp
    xor    sp, sp, sp
    li     sp, cn_majorexp_instruction_tlb_error@l
    mtspr  spr5, sp
    lis    sp, sys_exp_top@h
    ori    sp, sp, sys_exp_top@l
    subi   sp, sp, _IRQ_TOP_OFFSET
    b   Default_sys_general_exception
    /*INSTRUCTION TLB ERROR EXCEPTION*/

    vector_align
.global IVOR15_OFFSET
IVOR15_OFFSET:
   /*
    mtspr  spr4, sp
    xor    sp, sp, sp
    li     sp, cn_majorexp_debug@l
    mtspr  spr5, sp
    lis    sp, dbg_exp_top@h
    ori    sp, sp, dbg_exp_top@l
    subi   sp, sp, _IRQ_TOP_OFFSET
    b   Default_DEBUG_exception
    */
    /*INSTRUCTION TLB ERROR EXCEPTION*/
    b   Default_debug_exception
    /*DEBUG EXCEPTION*/

    vector_align
.global IVOR32_OFFSET
IVOR32_OFFSET:
    mtspr  spr4, sp
    xor    sp, sp, sp
    li     sp, cn_majorexp_speapu_unavailable@l
    mtspr  spr5, sp
    lis    sp, sys_exp_top@h
    ori    sp, sp, sys_exp_top@l
    subi   sp, sp, _IRQ_TOP_OFFSET
    b   Default_sys_general_exception
    /*SPE APU UNAVAILABLE EXCEPTION*/

    vector_align
.global IVOR33_OFFSET
IVOR33_OFFSET:
    mtspr  spr4, sp
    xor    sp, sp, sp
    li     sp, cn_majorexp_embeded_fp_data@l
    mtspr  spr5, sp
    lis    sp, sys_exp_top@h
    ori    sp, sp, sys_exp_top@l
    subi   sp, sp, _IRQ_TOP_OFFSET
    b   Default_sys_general_exception
    /*EMBEDED FLOAT POINT DATA EXCEPTION*/

    vector_align
.global IVOR34_OFFSET
IVOR34_OFFSET:
    mtspr  spr4, sp
    xor    sp, sp, sp
    li     sp, cn_majorexp_embeded_fp_round@l
    mtspr  spr5, sp
    lis    sp, sys_exp_top@h
    ori    sp, sp, sys_exp_top@l
    subi   sp, sp, _IRQ_TOP_OFFSET
    b   Default_sys_general_exception
    /*EMBEDED FLOAT POINT ROUND EXCEPTION*/

    vector_align
.global IVOR35_OFFSET
IVOR35_OFFSET:
    b   .
    /*PERFORMANCE MONITOR EXCEPTION*/
    
    
/*
@-------------------------------------------------------------------------------
@����:�����쳣������
@����:��
@����:��
@����ԭ��: void __AsmSetExpVecTab(void)
@-------------------------------------------------------------------------------
*/
.global __AsmSetExpVecTab
__AsmSetExpVecTab:
    //set the exception tab
    //so we should better disable all the interrupt anyway
    stwu sp, -32(sp)
    stw  r3, 8(sp)

    xor   r3, r3, r3
    li r3, IVOR0_OFFSET@l
    mtspr  IVOR0, r3

    li r3, IVOR1_OFFSET@l
    mtspr  IVOR1, r3

    li r3, IVOR2_OFFSET@l
    mtspr  IVOR2, r3

    li r3, IVOR3_OFFSET@l
    mtspr  IVOR3, r3

    li r3, IVOR4_OFFSET@l
    mtspr  IVOR4, r3

    li r3, IVOR5_OFFSET@l
    mtspr  IVOR5, r3

    li r3, IVOR6_OFFSET@l
    mtspr  IVOR6, r3

    li r3, IVOR7_OFFSET@l
    mtspr  IVOR7, r3

    li r3, IVOR8_OFFSET@l
    mtspr  IVOR8, r3

    li r3, IVOR9_OFFSET@l
    mtspr  IVOR9, r3

    li r3, IVOR10_OFFSET@l
    mtspr  IVOR10, r3

    li r3, IVOR11_OFFSET@l
    mtspr  IVOR11, r3

    li r3, IVOR12_OFFSET@l
    mtspr  IVOR12, r3

    li r3, IVOR13_OFFSET@l
    mtspr  IVOR13, r3

    li r3, IVOR14_OFFSET@l
    mtspr  IVOR14, r3

    li r3, IVOR15_OFFSET@l
    mtspr  IVOR15, r3

    li r3, IVOR32_OFFSET@l
    mtspr  IVOR32, r3

    li r3, IVOR33_OFFSET@l
    mtspr  IVOR33, r3

    li r3, IVOR34_OFFSET@l
    mtspr  IVOR34, r3

    li r3, IVOR35_OFFSET@l
    mtspr  IVOR35, r3

    isync
    msync

    //������ƫ����
    lis   r3, exception_tab@h
    ori   r3, r3, exception_tab@l
    mtspr   IVPR,r3
    isync
    msync

    lwz sp, 0(sp)
    lwz r3, 8(sp)
    blr    

.align 16
.section    .stack.usr, "aw", %nobits
.align  16
    .space  0x10000
.section    .stack.irq, "aw", %nobits
.align  16
    .space  0x10000

.section    .stack.prg_exp, "aw", %nobits
.align  16
    .space  0x10000

.section    .stack.dbg_exp, "aw", %nobits
.align  16
    .space  0x10000

.section    .stack.cint_exp, "aw", %nobits
.align  16
    .space  0x10000

.section    .stack.mc_exp, "aw", %nobits
.align  16
    .space  0x10000

.section    .stack.sys_exp, "aw", %nobits
.align  16
    .space  0x10000
    
.section    .stack.core1_stack, "aw", %nobits
.align  16
    .space  0x10000

.end
