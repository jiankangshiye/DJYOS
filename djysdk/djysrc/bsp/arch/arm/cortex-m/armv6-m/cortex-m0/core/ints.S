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
//����ģ��:  �ж�ģ��
//���ߣ�     lst
//�汾��      V1.00
//��ʼ�汾������ڣ�2009-08-10
//�ļ�����:  �ж�ģ���еĻ�ಿ��
//����˵��:  ��
//�޶���ʷ:
//2. ...
//1. ����:
//   ����:
//   �°汾�ţ�
//   �޸�˵��:
//------------------------------------------------------

.syntax		unified
.cpu		cortex-m0
.fpu		softvfp
.thumb
.file	"ints.S"

.extern  __Int_EngineAsynSignal

@ΪʲôҪ����r4-r11�أ�������жϵ����߳�a�����жϷ�����ʹ�����ȼ����߳�b������
@�͵÷��ص��߳�b�У������߳�a��r4-r11���浽�߳�a���������У���������������Ҫʹ��
@r4-r11��Ϊɶ��������ջ����?����Ϊ__asm_switch_context_int������֪��ջ�������
@����si�汾����û����ģ���������dlsp�汾�г�����
.text
.align 2
.global __start_int
.thumb
.thumb_func
__start_int:
    push    {r0,lr}				@r0ֻ��Ϊ������ջָ��8�ֽڶ���������
    mrs     r1,psp
    subs    r1,r1,#8*4          @����8���Ĵ���,���ڱ���r4~r11
    msr     psp,r1
    stmia   r1!,{r4-r7}         @�����Ĵ���,����tick�쳣���������߳��л���
    mov     r2,r8
    mov     r3,r9
    mov     r4,r10
    mov     r5,r11
    stmia   r1!,{r2-r5}         @�����Ĵ���,�жϿ��������߳��л���
    bl      __Int_EngineAsynSignal
    pop     {r0,r1}
    mov     lr,r1
    mrs     r0,psp
    ldmfd   r0!,{r4-r7}
    ldmfd   r0!,{r2,r3}
    mov     r8,r2
    mov     r9,r3
    ldmfd   r0!,{r2,r3}
    mov     r10,r2
    mov     r11,r3
    msr     psp,r0
    bx      lr

.text
.align 2
.global __start_int_0
.thumb
.thumb_func
__start_int_0:
    push    {r0,lr}			    @r0ֻ��Ϊ������ջָ��8�ֽڶ���������
    movs    r0,#0              @ȡ�쳣��
    
    ldr     r1,=(vector_table+0*4) 
    ldr     r1,[r1]
    blx     r1
    pop     {r0,pc}

.text
.align 2
.global __start_int_1
.thumb
.thumb_func
__start_int_1:
    push    {r0,lr}			    @r0ֻ��Ϊ������ջָ��8�ֽڶ���������
    movs    r0,#1              @ȡ�쳣��
    
    ldr     r1,=(vector_table+1*4) 
    ldr     r1,[r1]
    blx     r1
    pop     {r0,pc}

.text
.align 2
.global __start_int_2
.thumb
.thumb_func
__start_int_2:
    push    {r0,lr}			    @r0ֻ��Ϊ������ջָ��8�ֽڶ���������
    movs    r0,#2              @ȡ�쳣��
    
    ldr     r1,=(vector_table+2*4) 
    ldr     r1,[r1]
    blx     r1
    pop     {r0,pc}

.text
.align 2
.global __start_int_3
.thumb
.thumb_func
__start_int_3:
    push    {r0,lr}			    @r0ֻ��Ϊ������ջָ��8�ֽڶ���������
    movs    r0,#3              @ȡ�쳣��
    
    ldr     r1,=(vector_table+3*4) 
    ldr     r1,[r1]
    blx     r1
    pop     {r0,pc}

.text
.align 2
.global __start_int_4
.thumb
.thumb_func
__start_int_4:
    push    {r0,lr}			    @r0ֻ��Ϊ������ջָ��8�ֽڶ���������
    movs    r0,#4              @ȡ�쳣��
    
    ldr     r1,=(vector_table+4*4) 
    ldr     r1,[r1]
    blx     r1
    pop     {r0,pc}

.text
.align 2
.global __start_int_5
.thumb
.thumb_func
__start_int_5:
    push    {r0,lr}			    @r0ֻ��Ϊ������ջָ��8�ֽڶ���������
    movs    r0,#5              @ȡ�쳣��
    
    ldr     r1,=(vector_table+5*4) 
    ldr     r1,[r1]
    blx     r1
    pop     {r0,pc}

.text
.align 2
.global __start_int_6
.thumb
.thumb_func
__start_int_6:
    push    {r0,lr}			    @r0ֻ��Ϊ������ջָ��8�ֽڶ���������
    movs    r0,#6              @ȡ�쳣��
    
    ldr     r1,=(vector_table+6*4) 
    ldr     r1,[r1]
    blx     r1
    pop     {r0,pc}

.text
.align 2
.global __start_int_7
.thumb
.thumb_func
__start_int_7:
    push    {r0,lr}			    @r0ֻ��Ϊ������ջָ��8�ֽڶ���������
    movs    r0,#7              @ȡ�쳣��
    
    ldr     r1,=(vector_table+7*4) 
    ldr     r1,[r1]
    blx     r1
    pop     {r0,pc}

.text
.align 2
.global __start_int_8
.thumb
.thumb_func
__start_int_8:
    push    {r0,lr}			    @r0ֻ��Ϊ������ջָ��8�ֽڶ���������
    movs    r0,#8              @ȡ�쳣��
    
    ldr     r1,=(vector_table+8*4) 
    ldr     r1,[r1]
    blx     r1
    pop     {r0,pc}

.text
.align 2
.global __start_int_9
.thumb
.thumb_func
__start_int_9:
    push    {r0,lr}			    @r0ֻ��Ϊ������ջָ��8�ֽڶ���������
    movs    r0,#9              @ȡ�쳣��
    
    ldr     r1,=(vector_table+9*4) 
    ldr     r1,[r1]
    blx     r1
    pop     {r0,pc}

.text
.align 2
.global __start_int_10
.thumb
.thumb_func
__start_int_10:
    push    {r0,lr}			    @r0ֻ��Ϊ������ջָ��8�ֽڶ���������
    movs    r0,#10              @ȡ�쳣��
    
    ldr     r1,=(vector_table+10*4) 
    ldr     r1,[r1]
    blx     r1
    pop     {r0,pc}

.text
.align 2
.global __start_int_11
.thumb
.thumb_func
__start_int_11:
    push    {r0,lr}			    @r0ֻ��Ϊ������ջָ��8�ֽڶ���������
    movs    r0,#11              @ȡ�쳣��
    
    ldr     r1,=(vector_table+11*4) 
    ldr     r1,[r1]
    blx     r1
    pop     {r0,pc}

.text
.align 2
.global __start_int_12
.thumb
.thumb_func
__start_int_12:
    push    {r0,lr}			    @r0ֻ��Ϊ������ջָ��8�ֽڶ���������
    movs    r0,#12              @ȡ�쳣��
    
    ldr     r1,=(vector_table+12*4) 
    ldr     r1,[r1]
    blx     r1
    pop     {r0,pc}

.text
.align 2
.global __start_int_13
.thumb
.thumb_func
__start_int_13:
    push    {r0,lr}			    @r0ֻ��Ϊ������ջָ��8�ֽڶ���������
    movs    r0,#13              @ȡ�쳣��
    
    ldr     r1,=(vector_table+13*4) 
    ldr     r1,[r1]
    blx     r1
    pop     {r0,pc}

.text
.align 2
.global __start_int_14
.thumb
.thumb_func
__start_int_14:
    push    {r0,lr}			    @r0ֻ��Ϊ������ջָ��8�ֽڶ���������
    movs    r0,#14              @ȡ�쳣��
    
    ldr     r1,=(vector_table+14*4) 
    ldr     r1,[r1]
    blx     r1
    pop     {r0,pc}

.text
.align 2
.global __start_int_15
.thumb
.thumb_func
__start_int_15:
    push    {r0,lr}			    @r0ֻ��Ϊ������ջָ��8�ֽڶ���������
    movs    r0,#15              @ȡ�쳣��
    
    ldr     r1,=(vector_table+15*4) 
    ldr     r1,[r1]
    blx     r1
    pop     {r0,pc}

.text
.align 2
.global __start_int_16
.thumb
.thumb_func
__start_int_16:
    push    {r0,lr}			    @r0ֻ��Ϊ������ջָ��8�ֽڶ���������
    movs    r0,#16              @ȡ�쳣��
    
    ldr     r1,=(vector_table+16*4) 
    ldr     r1,[r1]
    blx     r1
    pop     {r0,pc}

.text
.align 2
.global __start_int_17
.thumb
.thumb_func
__start_int_17:
    push    {r0,lr}			    @r0ֻ��Ϊ������ջָ��8�ֽڶ���������
    movs    r0,#17              @ȡ�쳣��
    
    ldr     r1,=(vector_table+17*4) 
    ldr     r1,[r1]
    blx     r1
    pop     {r0,pc}

.text
.align 2
.global __start_int_18
.thumb
.thumb_func
__start_int_18:
    push    {r0,lr}			    @r0ֻ��Ϊ������ջָ��8�ֽڶ���������
    movs    r0,#18              @ȡ�쳣��
    
    ldr     r1,=(vector_table+18*4) 
    ldr     r1,[r1]
    blx     r1
    pop     {r0,pc}

.text
.align 2
.global __start_int_19
.thumb
.thumb_func
__start_int_19:
    push    {r0,lr}			    @r0ֻ��Ϊ������ջָ��8�ֽڶ���������
    movs    r0,#19              @ȡ�쳣��
    
    ldr     r1,=(vector_table+19*4) 
    ldr     r1,[r1]
    blx     r1
    pop     {r0,pc}

.text
.align 2
.global __start_int_20
.thumb
.thumb_func
__start_int_20:
    push    {r0,lr}			    @r0ֻ��Ϊ������ջָ��8�ֽڶ���������
    movs    r0,#20              @ȡ�쳣��
    
    ldr     r1,=(vector_table+20*4) 
    ldr     r1,[r1]
    blx     r1
    pop     {r0,pc}

.text
.align 2
.global __start_int_21
.thumb
.thumb_func
__start_int_21:
    push    {r0,lr}			    @r0ֻ��Ϊ������ջָ��8�ֽڶ���������
    movs    r0,#21              @ȡ�쳣��
    
    ldr     r1,=(vector_table+21*4) 
    ldr     r1,[r1]
    blx     r1
    pop     {r0,pc}

.text
.align 2
.global __start_int_22
.thumb
.thumb_func
__start_int_22:
    push    {r0,lr}			    @r0ֻ��Ϊ������ջָ��8�ֽڶ���������
    movs    r0,#22              @ȡ�쳣��
    
    ldr     r1,=(vector_table+22*4) 
    ldr     r1,[r1]
    blx     r1
    pop     {r0,pc}

.text
.align 2
.global __start_int_23
.thumb
.thumb_func
__start_int_23:
    push    {r0,lr}			    @r0ֻ��Ϊ������ջָ��8�ֽڶ���������
    movs    r0,#23              @ȡ�쳣��
    
    ldr     r1,=(vector_table+23*4) 
    ldr     r1,[r1]
    blx     r1
    pop     {r0,pc}

.text
.align 2
.global __start_int_24
.thumb
.thumb_func
__start_int_24:
    push    {r0,lr}			    @r0ֻ��Ϊ������ջָ��8�ֽڶ���������
    movs    r0,#24              @ȡ�쳣��
    
    ldr     r1,=(vector_table+24*4) 
    ldr     r1,[r1]
    blx     r1
    pop     {r0,pc}

.text
.align 2
.global __start_int_25
.thumb
.thumb_func
__start_int_25:
    push    {r0,lr}			    @r0ֻ��Ϊ������ջָ��8�ֽڶ���������
    movs    r0,#25              @ȡ�쳣��
    
    ldr     r1,=(vector_table+25*4) 
    ldr     r1,[r1]
    blx     r1
    pop     {r0,pc}

.text
.align 2
.global __start_int_26
.thumb
.thumb_func
__start_int_26:
    push    {r0,lr}			    @r0ֻ��Ϊ������ջָ��8�ֽڶ���������
    movs    r0,#26              @ȡ�쳣��
    
    ldr     r1,=(vector_table+26*4) 
    ldr     r1,[r1]
    blx     r1
    pop     {r0,pc}

.text
.align 2
.global __start_int_27
.thumb
.thumb_func
__start_int_27:
    push    {r0,lr}			    @r0ֻ��Ϊ������ջָ��8�ֽڶ���������
    movs    r0,#27              @ȡ�쳣��
    
    ldr     r1,=(vector_table+27*4) 
    ldr     r1,[r1]
    blx     r1
    pop     {r0,pc}

.text
.align 2
.global __start_int_28
.thumb
.thumb_func
__start_int_28:
    push    {r0,lr}			    @r0ֻ��Ϊ������ջָ��8�ֽڶ���������
    movs    r0,#28              @ȡ�쳣��
    
    ldr     r1,=(vector_table+28*4) 
    ldr     r1,[r1]
    blx     r1
    pop     {r0,pc}

.text
.align 2
.global __start_int_29
.thumb
.thumb_func
__start_int_29:
    push    {r0,lr}			    @r0ֻ��Ϊ������ջָ��8�ֽڶ���������
    movs    r0,#29              @ȡ�쳣��
    
    ldr     r1,=(vector_table+29*4) 
    ldr     r1,[r1]
    blx     r1
    pop     {r0,pc}

.text
.align 2
.global __start_int_30
.thumb
.thumb_func
__start_int_30:
    push    {r0,lr}			    @r0ֻ��Ϊ������ջָ��8�ֽڶ���������
    movs    r0,#30              @ȡ�쳣��
    
    ldr     r1,=(vector_table+30*4) 
    ldr     r1,[r1]
    blx     r1
    pop     {r0,pc}

.text
.align 2
.global __start_int_31
.thumb
.thumb_func
__start_int_31:
    push    {r0,lr}			    @r0ֻ��Ϊ������ջָ��8�ֽڶ���������
    movs    r0,#31              @ȡ�쳣��
    
    ldr     r1,=(vector_table+31*4) 
    ldr     r1,[r1]
    blx     r1
    pop     {r0,pc}

.end