ENTRY(Init_Cpu)

MEMORY
{
    ROM1(RX)       : org = 0x00000000, len = 512k
    RAM1(RXW)      : org = 0x10000000, len = 32k
    RAM2(RXW)      : org = 0x2007C000, len = 32k
}

/*不同的make程序，文件路径表示方式，是不同的，输入段中文件名写法要相应修改*/
/*正确的写法，可参看console中输出的命令行*/
SECTIONS
{   
    text_startup_rROM1_lROM1 :
    {
        . = ALIGN(0x08);
        KEEP(* (.isr_vector .isr_vector.*))
        KEEP(*liblibOS-release.a:exceptions.o (.text .text.* .rodata .rodata.*))
        KEEP(*liblibOS-release.a:initcpuc.o (.text .text.* .rodata .rodata.*))
        KEEP(*liblibOS-release.a:pre_loader.o (.text .text.* .rodata .rodata.*))
    }>ROM1 
    
    text_preload_rROM1_lROM1 :
    {
        . = ALIGN(0x08);
        *liblibOS-release.a:loader.o (.text .text.* .rodata .rodata.*)
        *liblibOS-release.a:int_hard.o (.text .text.* .rodata .rodata.*)
        *liblibOS-release.a:int.o (.text .text.* .rodata .rodata.*)
        *liblibOS-release.a:ints.o (.text .text.* .rodata .rodata.*)
        *src/user/critical/*.o (.text .text.* .rodata .rodata.*)
        *liblibOS-release.a:heap-static.o (.text .text.* .rodata .rodata.*)
    }>ROM1 
    
    rw_preload_rRAM1_lROM1 :
    {
        . = ALIGN(0x08);
        KEEP(* (.table.exceptions .table.exceptions.*))
        KEEP(* (.table.vectors .table.vectors.*))
        *liblibOS-release.a:loader.o (.data .data.*)
        *liblibOS-release.a:int_hard.o (.data .data.*)
        *liblibOS-release.a:int.o (.data .data.*)
        *liblibOS-release.a:ints.o (.data .data.*)
        *src/user/critical/*.o (.data .data.*)
        *liblibOS-release.a:heap-static.o (.data .data.*)
        }>RAM1 AT>ROM1
    
    zi_preload_rRAM1_lRAM1 (NOLOAD) :
    {
        . = ALIGN(0x08);
        *liblibOS-release.a:loader.o (.bss .bss.* COMMON)
        *liblibOS-release.a:int_hard.o (.bss .bss.* COMMON)
        *liblibOS-release.a:int.o (.bss .bss.* COMMON)
        *liblibOS-release.a:ints.o (.bss .bss.* COMMON)
        *src/user/critical/*.o (.bss .bss.* COMMON)
        *liblibOS-release.a:heap-static.o (.bss .bss.* COMMON)
    }>RAM1 
    
    text_sysload_rROM1_lROM1 :
    {
        . = ALIGN(0x08);
        * (.text .text.* .rodata .rodata.*)
    }>ROM1 
    
    rw_sysload_rRAM1_lROM1 :
    {
        . = ALIGN(0x08);
        * (.data .data.*)
    }>RAM1 AT>ROM1
    
    zi_sysload_rRAM1_lRAM1 (NOLOAD) :
    {
        . = ALIGN(0x08);
        * (.bss .bss.* COMMON)
    }>RAM1 
    
    preload_cp_table :
    {
        . = ALIGN(0x08);
        preload_copy_table = .;
        LONG(16)
        LONG(3)
        
        LONG(LOADADDR(text_preload_rROM1_lROM1))
        LONG(ADDR(text_preload_rROM1_lROM1))
        LONG(SIZEOF(text_preload_rROM1_lROM1))
        LONG(1)
        
        LONG(LOADADDR(rw_preload_rRAM1_lROM1))
        LONG(ADDR(rw_preload_rRAM1_lROM1))
        LONG(SIZEOF(rw_preload_rRAM1_lROM1))
        LONG(1)
        
        LONG(LOADADDR(zi_preload_rRAM1_lRAM1))
        LONG(ADDR(zi_preload_rRAM1_lRAM1))
        LONG(SIZEOF(zi_preload_rRAM1_lRAM1))
        LONG(0)
    }>ROM1 
    
    sysload_cp_table :
    {
        . = ALIGN(0x08);
        sysload_copy_table = .;
        LONG(16)
        LONG(3)
        
        LONG(LOADADDR(text_sysload_rROM1_lROM1))
        LONG(ADDR(text_sysload_rROM1_lROM1))
        LONG(SIZEOF(text_sysload_rROM1_lROM1))
        LONG(1)
        
        LONG(LOADADDR(rw_sysload_rRAM1_lROM1))
        LONG(ADDR(rw_sysload_rRAM1_lROM1))
        LONG(SIZEOF(rw_sysload_rRAM1_lROM1))
        LONG(1)
        
        LONG(LOADADDR(zi_sysload_rRAM1_lRAM1))
        LONG(ADDR(zi_sysload_rRAM1_lRAM1))
        LONG(SIZEOF(zi_sysload_rRAM1_lRAM1))
        LONG(0)
    }>ROM1 
    
    stack_rRAM1_lRAM1 (NOLOAD) :
    {
        . = ALIGN(0x08);
        msp_bottom = .;
        KEEP(* (.stack.msp .stack.msp.*))
        msp_top = ALIGN(0x08);     
    }>RAM1 
    
    heap_rRAM1_lRAM1 (NOLOAD) :
    {
        . = ALIGN(0x08);
        sys_heap_bottom = .;
    }>RAM1 
    sys_heap_top = 0x10000000+32k;
    
    heap_rRAM2_lRAM2 (NOLOAD) :
    {
        . = ALIGN(0x08);
        sys_heap_bottom_ext = .;
    }>RAM2 
    sys_heap_top_ext = 0x2007C000+32k;
    
    heap_cp_table :
    {
        . = ALIGN(0x08);
        pHeapList = .;
        
/* 默认堆，必须排首位，名字必须是“sys” */
        LONG(3)                     /*表示该堆由两段内存组成*/
        LONG(0)                     /*该堆上分配的内存的对齐尺寸，0表示使用系统对齐*/
        LONG(sys_heap_bottom)       /*第一段基址，须符合对齐要求*/
        LONG(sys_heap_top)          /*第一段地址上限（不含），须符合对齐要求*/
        LONG(32)                    /*第一段页尺寸=32bytes*/
        LONG(msp_bottom)            /*第二段基址，回收初始化栈，须符合对齐要求*/
        LONG(msp_top - 384)         /*第二段地址上限（不含），384是留给中断使用的，须符合对齐要求*/
        LONG(24)                    /*第二段页尺寸=24bytes*/
        LONG(sys_heap_bottom_ext)   /*第三段基址，须符合对齐要求*/
        LONG(sys_heap_top_ext)      /*第三段地址上限（不含），须符合对齐要求*/
        LONG(24)                    /*第三段页尺寸=32bytes*/
        BYTE(0x73)                  /*'s'*/
        BYTE(0x79)                  /*'y'*/
        BYTE(0x73)                  /*'s'*/
        BYTE(0)                     /*串结束，"sys"是堆的名字*/
        . = ALIGN(0x04);
        LONG(0)     /*结束标志*/
    }>ROM1 
}
/**** The End ****/
