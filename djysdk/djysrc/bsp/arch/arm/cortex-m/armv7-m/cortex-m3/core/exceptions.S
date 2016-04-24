/*
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
@����ģ��:  �쳣ģ��
@���ߣ�     lst
@�汾��      V1.00
@��ʼ�汾������ڣ�2009-08-30
@�ļ�����:  ϵͳ�쳣ģ���еĻ�ಿ��
@����˵��:  ��
@�޶���ʷ:
@2. ...
@1. ����:
@   ����:
@   �°汾�ţ�
@   �޸�˵��:
@------------------------------------------------------
*/

.syntax         unified
.cpu            cortex-m3
.fpu            softvfp
.thumb
.file   "exceptions.S"

//���첽�ź�ʹ�ܲ��������ȣ����������л��лָ��첽�źżĴ�����ͬ��ʹ���첽�ź�
.macro restore_asyn_signal_reg
    bl Int_ContactAsynSignal
.endm
/**** ջ��С���� ****/
.section        .stack.msp, "aw", %nobits
.align          3
        .space  0x1000


/**** �쳣������ ****/
.section    .isr_vector, "ax", %progbits
.align      3
.global     isr_vector
isr_vector:
    .word   msp_top
    .word   Init_Cpu
    .word   nmi_handler
    .word   hardfault_handler
    .word   exp_memfault_handler      @ MemManage Fault Handler
    .word   exp_busfault_handler      @ Bus Fault Handler
    .word   exp_usagefault_handler    @ Usage Fault Handler
    .word   0                         @ Reserved
    .word   0                         @ Reserved
    .word   0                         @ Reserved
    .word   0                         @ Reserved
    .word   exp_svc_handler           @ SVCall Handler
    .word   exp_debug_handler         @ Reserved
    .word   0                         @ Reserved
    .word   exp_pendsv_handler        @ PendSV Handler
    .word   exp_systick_handler       @ SysTick Handler

/**** ������������ ****/
.text
.align 2
.global default_handler
.thumb
.thumb_func
default_handler:
    b       default_handler

.global nmi_handler
.thumb_set nmi_handler, default_handler

/*
@ ------------------------------------------------------------------------------
@ ����:Hard�쳣�����������쳣ʱ���ں˻��Զ�����R0-R3,R12,lr,pc,xpsr,�ڻ���� Ϊ�˱�
@      ���ֳ����Ὣʣ�µ�R4-R11ѹջ��Ӳ���쳣�����������쳣����
@ ����:��
@ ------------------------------------------------------------------------------
*/
.text
.align 2
.global hardfault_handler
.thumb
.thumb_func
hardfault_handler:
        //first of all,save the exp state
    TST     LR,#0x4
    ITE     EQ
    MRSEQ   R1,MSP              /* ��ʹ�õ�������ջ���ʰ�MSP��ֵȡ�� */
    MRSNE   R1,PSP

    STMFD   R1!,{R4-R11}        /*  ����R4-R11�����жϵ�ջ*/
    ADD     R2,R1,#64           /*  �������쳣��SPѹջ*/
    STMFD   R1!,{R2}

    TST     LR,#0x4
    ITE     EQ
    MSREQ   MSP,R1              /* ��ʹ�õ�������ջ���ʰ�MSP��ֵȡ�� */
    MSRNE   PSP,R1

    PUSH    {R0,LR}             /* ��������ֵ��������Ҫ�õ� */

    MOV     R0,R1
    BL       Exp_HardFaultHandler

    POP     {R0,LR}
    TST     LR,#0x4
    ITE     EQ
    MRSEQ   R0,MSP              /* ��ʹ�õ�������ջ���ʰ�MSP��ֵȡ�� */
    MRSNE   R0,PSP
    LDMFD   R0!,{R4-R11}
    LDMFD   R0!,{R2}

    TST     LR,#0x4
    ITE     EQ
    MSREQ   MSP,R0              /* ��ʹ�õ�������ջ���ʰ�MSP��ֵȡ�� */
    MSRNE   PSP,R0

    BX      LR

/*
@ ------------------------------------------------------------------------------
@ ����:MEM �쳣�����������쳣ʱ���ں˻��Զ�����R0-R3,R12,lr,pc,xpsr,�ڻ���� Ϊ�˱�
@      ���ֳ����Ὣʣ�µ�R4-R11ѹջ��һ��������MPU�������µķ���Υ��������쳣
@ ����:��
@ ------------------------------------------------------------------------------
*/
.text
.align 2
.global exp_memfault_handler
.thumb
.thumb_func
exp_memfault_handler:
    //first of all,save the exp state
    TST     LR,#0x4
    ITE     EQ
    MRSEQ   R1,MSP              /* ��ʹ�õ�������ջ���ʰ�MSP��ֵȡ�� */
    MRSNE   R1,PSP

    STMFD   R1!,{R4-R11}        /*  ����R4-R11�����жϵ�ջ*/
    ADD     R2,R1,#64           /*  �������쳣��SPѹջ*/
    STMFD   R1!,{R2}

    TST     LR,#0x4
    ITE     EQ
    MSREQ   MSP,R1              /* ��ʹ�õ�������ջ���ʰ�MSP��ֵȡ�� */
    MSRNE   PSP,R1

    PUSH    {R0,LR}             /* ��������ֵ��������Ҫ�õ� */

    MOV     R0,R1
    BL       Exp_MemManageFaultHandler

    POP     {R0,LR}
    TST     LR,#0x4
    ITE     EQ
    MRSEQ   R0,MSP              /* ��ʹ�õ�������ջ���ʰ�MSP��ֵȡ�� */
    MRSNE   R0,PSP
    LDMFD   R0!,{R4-R11}
    LDMFD   R0!,{R2}

    TST     LR,#0x4
    ITE     EQ
    MSREQ   MSP,R0              /* ��ʹ�õ�������ջ���ʰ�MSP��ֵȡ�� */
    MSRNE   PSP,R0

    BX      LR

/*
@ ------------------------------------------------------------------------------
@ ����:BusFault�����������쳣ʱ���ں˻��Զ�����R0-R3,R12,lr,pc,xpsr,�ڻ���� Ϊ�˱�
@      ���ֳ����Ὣʣ�µ�R4-R11ѹջ
@ ����:��
@ ------------------------------------------------------------------------------
*/
.text
.align 2
.global exp_busfault_handler
.thumb
.thumb_func
exp_busfault_handler:
    //first of all,save the exp state
    TST     LR,#0x4
    ITE     EQ
    MRSEQ   R1,MSP              /* ��ʹ�õ�������ջ���ʰ�MSP��ֵȡ�� */
    MRSNE   R1,PSP

    STMFD   R1!,{R4-R11}        /*  ����R4-R11�����жϵ�ջ*/
    ADD     R2,R1,#64           /*  �������쳣��SPѹջ*/
    STMFD   R1!,{R2}

    TST     LR,#0x4
    ITE     EQ
    MSREQ   MSP,R1              /* ��ʹ�õ�������ջ���ʰ�MSP��ֵȡ�� */
    MSRNE   PSP,R1

    PUSH    {R0,LR}             /* ��������ֵ��������Ҫ�õ� */

    MOV     R0,R1
    BL       Exp_BusFaultHandler

    POP     {R0,LR}
    TST     LR,#0x4
    ITE     EQ
    MRSEQ   R0,MSP              /* ��ʹ�õ�������ջ���ʰ�MSP��ֵȡ�� */
    MRSNE   R0,PSP
    LDMFD   R0!,{R4-R11}
    LDMFD   R0!,{R2}

    TST     LR,#0x4
    ITE     EQ
    MSREQ   MSP,R0              /* ��ʹ�õ�������ջ���ʰ�MSP��ֵȡ�� */
    MSRNE   PSP,R0

    BX      LR

/*
@ ------------------------------------------------------------------------------
@ ����:Debug�쳣�����������쳣ʱ���ں˻��Զ�����R0-R3,R12,lr,pc,xpsr,�ڻ���� Ϊ�˱�
@      ���ֳ����Ὣʣ�µ�R4-R11ѹջ
@ ����:��
@ ------------------------------------------------------------------------------
*/
.text
.align 2
.global exp_debug_handler
.thumb
.thumb_func
exp_debug_handler:
    b       exp_debug_handler

.text
.align 2
.global exp_pendsv_handler
.thumb
.thumb_func
exp_pendsv_handler:
    b       exp_pendsv_handler

/*
@ ------------------------------------------------------------------------------
@ ����:USAGE FAULT �쳣�����������쳣ʱ���ں˻��Զ�����R0-R3,R12,lr,pc,xpsr,�ڻ��
@      �� Ϊ�˱����ֳ����Ὣʣ�µ�R4-R11ѹջ�����ջ�����ε�������
@ ����:R0�б����˷����쳣ʱ��ջ�������ֳ�
@ ------------------------------------------------------------------------------
*/
.text
.align 2
.global exp_usagefault_handler
.thumb
.thumb_func
exp_usagefault_handler:
    //first of all,save the exp state
    TST     LR,#0x4
    ITE     EQ
    MRSEQ   R1,MSP              /* ��ʹ�õ�������ջ���ʰ�MSP��ֵȡ�� */
    MRSNE   R1,PSP

    STMFD   R1!,{R4-R11}        /*  ����R4-R11�����жϵ�ջ*/
    ADD     R2,R1,#64           /*  �������쳣��SPѹջ*/
    STMFD   R1!,{R2}

    TST     LR,#0x4
    ITE     EQ
    MSREQ   MSP,R1              /* ��ʹ�õ�������ջ���ʰ�MSP��ֵȡ�� */
    MSRNE   PSP,R1

    PUSH    {R0,LR}             /* ��������ֵ��������Ҫ�õ� */

    MOV     R0,R1
    BL       Exp_UsageFaultHandler

    POP     {R0,LR}
    TST     LR,#0x4
    ITE     EQ
    MRSEQ   R0,MSP              /* ��ʹ�õ�������ջ���ʰ�MSP��ֵȡ�� */
    MRSNE   R0,PSP
    LDMFD   R0!,{R4-R11}
    LDMFD   R0!,{R2}

    TST     LR,#0x4
    ITE     EQ
    MSREQ   MSP,R0              /* ��ʹ�õ�������ջ���ʰ�MSP��ֵȡ�� */
    MSRNE   PSP,R0

    BX      LR

/*
@ ------------------------------------------------------------------------------
@ ����:SYSTICK �쳣�����������쳣ʱ���ں˻��Զ�����R0-R3,R12,lr,pc,xpsr,�ڻ��
@      �� Ϊ�˱����ֳ����Ὣʣ�µ�R4-R11ѹջ�����������غ󣬻ָ��ֳ���������
@ ����:��
@ ------------------------------------------------------------------------------
*/
.text
.align 2
.global exp_systick_handler
.thumb
.thumb_func
exp_systick_handler:
    push    {r0,lr}             /* r0ֻ��Ϊ������ջָ��8�ֽڶ��������� */
    ldr     r0,=0xe000e010
    ldr     r0,[r0]
    mrs     r0,psp
    stmfd   r0!,{r4-r11}        /* �����Ĵ���,�Լ����ص�ַ */
    msr     psp,r0
    bl      Exp_SystickTickHandler
    pop     {r0,lr}
    mrs     r0,psp
    ldmfd   r0!,{r4-r11}
    msr     psp,r0
    bx      lr

/*
@ ------------------------------------------------------------------------------
@ ����:SVC���� �쳣������SVC�쳣��Ҫ�������߳��л�������Ҫ�����߳��л�ʱ��ʹ��SVCָ��
@      ������Ӧ�Ĳ������ݵ��쳣���쳣����SVC���ж�SVC����
@ ����:SVC��
@ -------------------------------------------------------------------------------
*/
.text
.align 2
.global exp_svc_handler
.thumb
.thumb_func
exp_svc_handler:
    push    {lr}
    restore_asyn_signal_reg
    pop     {lr}
    TST     LR,#0x4             /* ����EXC_RETURN�ı���2 */
    ITE     EQ                  /* ���Ϊ0, */
    MRSEQ   R0,MSP              /* ��ʹ�õ�������ջ���ʰ�MSP��ֵȡ�� */
    MRSNE   R0,PSP              /* ����, ʹ�õ��ǽ��̶�ջ���ʰ�MSP��ֵȡ�� */
    LDR     R1,[R0,#24]         /* ��ջ�ж�ȡPC��ֵ */
    LDRB    R1,[R1,#-2]         /* ��SVCָ���ж�ȡ�������ŵ�R0 */
    cbnz    r1,svc_error        /* Ŀǰֻ֧��0��svc���� */
    add     r0,#32              /* 0�ŵ��������������л�������pspָ�� */
    ldmfd   r0!,{r4-r11}        /* �ֹ�����r4-r11 */
    msr     psp,r0              /* pspָ�������������� */
svc_error:                      /* ֱ�ӷ��� */
    bx      lr                  /* ���أ�ʵ�ʵ����Ľ��Ǵ����������� */

.end