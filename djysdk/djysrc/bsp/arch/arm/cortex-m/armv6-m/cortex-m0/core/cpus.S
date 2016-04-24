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
//����ģ��:������
//���ߣ�lst
//�汾��V1.0.0
//�ļ�����:����������CPUֱ����صĻ����룬��Ҫ���߳��л�����Ĵ��롣
//����˵��:
//�޶���ʷ:
//2. ...
//1. ����: 2009-01-04
//   ����: lst
//   �°汾��: V1.0.0
//   �޸�˵��: ԭʼ�汾
//------------------------------------------------------
.extern      __Djy_VmEngine
.extern      Int_HalfEnableAsynSignal

@ע: psp����������ջ�У�������vm->stack��!!

.syntax		unified
.cpu		cortex-m0
.fpu		softvfp
.thumb
.file	"cpus.S"

@----�����߳�-----------------------------------------------------------------
@����:  ��ʼ���̵߳�ջ���������߳�ִ�к������´����߳�ʱ������ñ�����
@����:  thread_routine�������õ��̺߳���
@       vm���߳������ָ��
@����:  ��ʼ��������ĵ�ǰջָ��
@����ԭ��:void * __asm_reset_thread(void (*thread_routine)(struct event_script *),
@                                        struct  tagThreadVm  *vm)@
@ע:����atpcs��ֻ��ʹ��r0~r3,�������ѹջ
@-----------------------------------------------------------------------------
.text
.align 2
.global __asm_reset_thread
.thumb
.thumb_func
__asm_reset_thread:
    ldr     r2,[r1,#4]          @ȡ�����ջ��ָ��
    subs    r2,r2,#8
    ldr     r3,=0x01000000      @xpsr�ĳ�ʼֵ
    str     r3,[r2,#4]
    ldr     r3,=__Djy_VmEngine @ȡ���������ָ��
    str     r3,[r2]
    subs    r2,r2,#14*4        @����14���Ĵ���,��ʼ״̬r0-r12�У���r0��������壬
                                @__vm_engine���������أ�lrҲ������
    @����r0�е�thread_routine��__vm_engine�Ĳ������л�������ʱ��thread_routine��
    @�ָ���r0�У����ݵ���Լ����r0��ֵ����__vm_engine�����Ĳ�����
    str     r0,[r2,#8*4]        @���� thread_routineָ�뵽r0��λ��.
    str     r2,[r1]             @����vm�ĵ�ǰջָ�뵽vm->stack��
    bx      lr
@ end of __asm_reset_thread

@----��λ���̣߳��л������߳�-------------------------------------------------
@����:  ��old_vm�����������³�ʼ�����´�����״̬��Ȼ���л������̵߳�
@       �������м���ִ�С�
@       ��һ���ڳ�פ�ڴ��������д������¼���ɺ󣬲���ɾ��������������븴λ
@       ���������Ȼ���л������������̣߳���ʱ�����Ҫ���ñ���������˱�������
@       ����λ���߳�����������ִ�С�
@����:  thread_routine�������õ��̺߳���
@       new_vm��Ŀ���̵߳������
@       old_vm������λ�������
@����:  ��
@����ԭ��:void __asm_reset_switch(void (*thread_routine)(void),
@                           struct  tagThreadVm *new_vm,struct  tagThreadVm *old_vm)@
@ע:���߳̽������ã��Ĵ���ʹ�������ƣ����뱣��
@-----------------------------------------------------------------------------
.text
.align 2
.global __asm_reset_switch
.thumb
.thumb_func
__asm_reset_switch:
    ldr     r7,[r2,#4]          @ȡ�������ջ��ָ��
    mov     sp,r7
    ldr     r7,=0x01000000      @xpsr�ĳ�ʼֵ
    ldr     r6,=__Djy_VmEngine    @ȡ���������ָ��
    push    {r6,r7}             @��pc,xpsr
    sub     sp,sp,#14*4         @����14���Ĵ���,��ʼ״̬r0-r12�У���r0��������壬
                                @__vm_engine���������أ�lrҲ������
                                @���ˣ�������������λ
    str     r0,[sp,#8*4]        @���� thread_routineָ��.���ˣ�������߳�����
    mov     r5,sp
    str     r5,[r2]             @���浱ǰջָ�뵽old_vm->stack��

    ldr     r5,[r1]             @ȡ����������ָ��
    mov     sp,r5
@    bl      Int_HalfEnableAsynSignal @��Ӧdone������ͷ�� __Int_ResetAsynSignal ����
    svc     0                   @������������
@ end of __asm_reset_switch

@----�������¼�����-----------------------------------------------------------
@����: ϵͳ��ʼ����ɺ��������¼����Ȳ������һ���¼�.
@����: new_vm���״���������������ƿ�ָ��
@����: ��
@����ԭ��: void __asm_start_thread(struct  tagThreadVm  *new_vm)@
@-----------------------------------------------------------------------------
.text
.align 2
.global __asm_start_thread
.thumb
.thumb_func
__asm_start_thread:
    ldr     r1,=msp_top
    msr     msp,r1			    @������ջָ�룬��ջ�������쳣���ж�
    movs    r1,#2
    msr     control,r1          @�л�����Ȩ�߳�ģʽ��ջָ����psp
    ldr     r1,[r0]             @ȡ����������ָ��
    mov     sp,r1
@    bl      Int_HalfEnableAsynSignal @�߳������󣬵���Ӧ����������
    svc     0                   @������������

@----����������---------------------------------------------------------------
@����:  ������ԭ�����ģ�ֱ�������µ�������ִ��
@����:  new_vm�����������������ƿ�ָ��
@����:  ��
@����ԭ��: void __asm_turnto_context(struct  tagThreadVm  *new_vm)@
@˵��:  ���¼����,��û�б�Ҫ������¼���������,ֱ���л������¼�����.
@-----------------------------------------------------------------------------
.text
.align 2
.global __asm_turnto_context
.thumb
.thumb_func
__asm_turnto_context:
    ldr     r1,[r0]             @ȡ����������ָ��
    mov     sp,r1
@    bl      Int_HalfEnableAsynSignal @��Ӧdone������ͷ�� Int_SaveAsynSignal ����
    svc     0                   @������������
@ end of __asm_turnto_context

@----�������л�---------------------------------------------------------------
@����:  ���浱ǰ�̵߳������ģ��л������̵߳��������м���ִ�С�
@����:  new_sp���������ĵ�ջָ��
@����:  old_sp���������ĵ�ջָ���ָ�룬��&vm->stack�������ṩ��������ջָ�룬
@               sp�Ĵ����ĵ�ǰֵ����
@����:  ��
@����ԭ��: void __asm_switch_context(struct  tagThreadVm *new_vm,struct  tagThreadVm *old_vm)@
@-----------------------------------------------------------------------------
.text
.align 2
.global __asm_switch_context
.thumb
.thumb_func
__asm_switch_context:
    mrs     r3,xpsr
    ldr     r2,=0x01000000
	orrs    r3,r3,r2            @xpsr��T��־�������������ֹ���λ��
    push    {r3}                @����xpsr
    push    {lr}                @����PC,�������߳��л�����ʱ���൱��ԭ�̵߳���
                                @���������أ�����lr���pc
    mov     r3,r12
    push    {r3,lr}             @����r12,lr
    push    {r0-r3}             @����r0-r3
    mov     r2,r10
    mov     r3,r11
    push    {r2,r3}             @����atpcs��r4~r7��ջǰ������ʹ�ã���ֻ����r2��
                                @r3����ʱ�����������α���r8~r11
    mov     r2,r8
    mov     r3,r9
    push    {r2,r3}
    push    {r4-r7}
    mov     r2,sp
    str     r2,[r1]             @�����������ջָ�뵽old_vm->stack

    ldr     r2,[r0]             @ȡ����������ָ��
    mov     sp,r2
@    bl      Int_HalfEnableAsynSignal
    svc     0                   @������������
@ end of __asm_switch_context

@----�ж��е��������л�-------------------------------------------------------
@����:  ���汻�ж��̵߳������ģ��л������̵߳��������м���ִ�С���������Ȼ����
@       �Ϸ�����(���û���int_isr_connect�������ӵ��жϷ�����)�У�����ARM
@       �У�ȴ������svc̬
@����:  new_sp���л�Ŀ�������
@����:  old_sp�����ж��߳������
@����:  ��
@����ԭ��: void __asm_switch_context_int(struct tagThreadVm *new_vm,struct tagThreadVm *old_vm)@
@-----------------------------------------------------------------------------
.text
.align 2
.global __asm_switch_context_int
.thumb
.thumb_func
__asm_switch_context_int:
    mrs     r2,psp      @ȡ���ж��̵߳�psp
    str     r2,[r1]     @��pspд����������ݽṹ��
    ldr     r2,[r0]     @ȡ�������̵߳�psp
    msr     psp,r2      @�������̵߳ĵ�ǰջָ��д��psp���жϷ��ػָ������Ľ��Դ�Ϊ׼
    bx      lr

.text
.align 2
.global __asm_reset_cpu
.global __asm_reset_cpu_hot
.thumb
.thumb_func
__asm_reset_cpu:
__asm_reset_cpu_hot:
    movs     r0,#0
    mov     pc,r0


.end