
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
// Copyright (c) 2014 著作权由深圳鹏瑞软件有限公司所有。著作权人保留一切权利。
//
// 这份授权条款，在使用者符合以下二条件的情形下，授予使用者使用及再散播本
// 软件包装原始码及二进位可执行形式的权利，无论此包装是否经改作皆然：
//
// 1. 对于本软件源代码的再散播，必须保留上述的版权宣告、此三条件表列，以
//    及下述的免责声明。
// 2. 对于本套件二进位可执行形式的再散播，必须连带以文件以及／或者其他附
//    于散播包装中的媒介方式，重制上述之版权宣告、此三条件表列，以及下述
//    的免责声明。

// 免责声明：本软件是本软件版权持有人以及贡献者以现状（"as is"）提供，
// 本软件包装不负任何明示或默示之担保责任，包括但不限于就适售性以及特定目
// 的的适用性为默示性担保。版权持有人及本软件之贡献者，无论任何条件、
// 无论成因或任何责任主义、无论此责任为因合约关系、无过失责任主义或因非违
// 约之侵权（包括过失或其他原因等）而起，对于任何因使用本软件包装所产生的
// 任何直接性、间接性、偶发性、特殊性、惩罚性或任何结果的损害（包括但不限
// 于替代商品或劳务之购用、使用损失、资料损失、利益损失、业务中断等等），
// 不负任何责任，即在该种使用已获事前告知可能会造成此类损害的情形下亦然。
//-----------------------------------------------------------------------------
#include "config-prj.h"
#include "stdint.h"
#include "driver.h"
#include "cpu_peri_uart.h"
#include "cpu_peri_lcd.h"
#include "uart.h"
#include "gkernel.h"
#include "djyos.h"

//=======全局变量定义============================================================
const u32 gc_u32CfgTimeZone = 8;     //本地时区为东8区(北京)

const char *gc_pCfgDefaultCharsetName = "gb2312";  //默认字符集
const char *gc_pCfgDefaultFontName = "gb2312_8_16_1616";  //默认字体

const u32 gc_u32CfgGuiCmdDeep = 1024;   //gkernel命令缓冲区深度(字节数,非命令条数)
const u32 gc_u32CfgGuiStsDeep = 256;    //gkernel状态缓冲区深度(字节数,非命令条数)

const u32 gc_u32CfgDeviceLimit = 10;     //定义设备数量。
const u32 gc_u32CfgLockLimit = 50;      //定义锁数量。用户调用semp_create和mutex_create创建的锁，不包括内核用到的锁。
const u32 gc_u32CfgEventLimit = 50;     //事件数量
const u32 gc_u32CfgEvttLimit = 50;      //总事件类型数
const u32 gc_u32CfgWdtLimit = 5;       //允许养狗数量
const u32 gc_u32CfgPipeLimit = 5;       //管道数量
const u32 gc_u32CfgMemPoolLimit = 20;    //允许建立10个内存池
const u32 gc_u32CfgStdinDeviceLimit = 3;
const u32 gc_u32CfgMainStackLen = 4096;

const u32 gc_u32CfgOpenedfileLimit = 16;
const u32 gc_u32CfgTimerLimit = 5;

const char *gc_pCfgStddevName = "UART2";  //标准输入输出设备
const char *gc_pCfgWorkPath = "sys:\\";   //工作路径设置

//协议栈配置
const u32 gNetPkgMemSize = 8*4096;

//==============================================================================

#define	LCD_DEV_NAME	"LCD_TWR_K70"
#define	TOUCH_DEV_NAME	"TOUCH_TWR_K70"

static	const char *gdd_input_dev[]={

	TOUCH_DEV_NAME,
	NULL, //必须要以NULL作为结束标记

};

//==============================================================================
bool_t ModuleInstall_TOUCH_TWR_K70(struct tagGkWinRsc *desktop,const char *touch_dev_name);

//==============================================================================

ptu32_t djy_main(void);

ptu32_t __djy_main(void)
{
    djy_main();
    return 0;
}
//----操作系统运行参数配置-----------------------------------------------------
//功能: 配置操作系统运行参数,例如锁的数量/事件数量等.使用文本文件配置时使用。
//参数: 无
//返回: 无
//-----------------------------------------------------------------------------
void Sys_ConfigRunParam(char *CfgfileName)
{
}

void Stddev_KnlInOutInit(u32 para)  //本函数暂未实现
{

}

//----组件初始化2-----------------------------------------------------------
//功能：可选组件初始化函数共有3个:
//      1、sys_module_init，在sys_init.c中，参见该函数注释
//      2、prj_module_init，在这里初始化的模块，需要调用跟具体工程相关的代码，例
//          如键盘模块，除了调用module_init_keyboard函数外，还要调用跟硬件相关的
//          module_init_keyboard_hard函数。
//      前两步初始化时，块相联分配方式还没有初始化，驱动中如果用到动态分配，使用
//      的是准静态分配，关于准静态分配的说明，参见mems.c文件。
//      3、run_module_init，参见该函数注释。
//参数：无
//返回：无
//-----------------------------------------------------------------------------
void Sys_ModuleInit(void)
{
    tagDevHandle char_term_hdl;
    u32 stdinout;
    uint16_t evtt_main;
    static struct tagGkWinRsc desktop;
    struct tagDisplayRsc *lcd_k70;
    //struct DisplayPara K70LcdPara = {LCD_XSIZE,LCD_YSIZE,"LCD_TWRK70","extram"};

    //初始化PutChark,Putsk,GetChark,GetsK四个内核级输入输出函数,此后,printk
    //可以正常使用.printf和scanf将调用前述4个函数执行输入输出
    Stddev_KnlInOutInit( 0 );
    //shell模块,依赖:无
    ModuleInstall_Sh(0);
    //文件系统模块,依赖:shell
    ModuleInstall_Djyfs(0);
    //设备驱动模块
    ModuleInstall_Driver(0);
    //多路复用模块,提供类似Linux的epoll、select的功能
    ModuleInstall_Multiplex(0);
  //消息队列模块
    ModuleInstall_MsgQ(0);
    //提供在shell上输出内核信息的功能,依赖:shell模块
    ModuleInstall_DebugInfo(0);
    //异常监视模块,依赖:shell模块
    ModuleInstall_Exp(0);
    //安装异常信息解析器,解析异常模块记录的二进制数据
    Exp_InfoDecoderInit( );

    //flash文件系统初始化,依赖:文件系统模块,shell模块
    ModuleInstall_DFFSD(0);
    //nand flash驱动,依赖:flash文件系统模块
    ModuleInstall_FsNandFlash("extram");
    //设置工作路径,依赖:文件系统,且相关路径存在.
    Djyfs_SetWorkPath(gc_pCfgWorkPath);

//    ModuleInstall_UART(CN_UART0);
//    ModuleInstall_UART(CN_UART1);
    ModuleInstall_UART(CN_UART2);
//    ModuleInstall_UART(CN_UART3);
//    ModuleInstall_UART(CN_UART4);
//    ModuleInstall_UART(CN_UART5);

    //标准IO初始化,不影响printk函数,此后,printf和scanf将使用stdin/out输出和输
    //入,如果裁掉,将一直用PutChark,Putsk,GetChark,GetsK执行IO
    //依赖: 若stdin/out/err是文件,则依赖文件系统
    //      若是设备,则依赖设备驱动
    //      同时,还依赖用来输出信息的设施,例如串口,LCD等
    ModuleInstall_Stddev( 0 );

    //打开IO设备,不同的板件,这部分可能区别比较大.
    stdinout = Driver_FindDevice(gc_pCfgStddevName);
    char_term_hdl = Driver_OpenDeviceAlias(stdinout,O_RDWR,0);
    if(char_term_hdl != NULL)
    {
        Driver_CtrlDevice(char_term_hdl,CN_UART_START,0,0);
        Driver_CtrlDevice(char_term_hdl,CN_UART_DMA_USED,0,0);
        //设置串口波特率为115200，
        Driver_CtrlDevice(char_term_hdl,CN_UART_SET_BAUD,115200,0);
        ModuleInstall_CharTerm((ptu32_t)stdinout);
        Driver_CloseDevice(char_term_hdl);
    }

    //djybus模块
    ModuleInstall_DjyBus(0);
    //IIC总线模块,依赖:djybus
    ModuleInstall_IICBus(0);
    //SPI总线模块,依赖:djybus
    ModuleInstall_SPIBus(0);
    IIC0_Init();

    //日历时钟模块
    ModuleInstall_TM(0);
    //使用硬件RTC,注释掉则使用tick做RTC计时,依赖:日历时钟模块
    ModuleInstall_RTC(0);

    //定时器组件
    //ModuleInstall_TimerSoft(CN_TIMER_SOURCE_TICK);

    //键盘输入模块
//    ModuleInstall_KeyBoard(0);
    //键盘输入驱动,依赖:键盘输入模块
//    ModuleInstall_KeyBoardHard(0);

    //字符集模块
    ModuleInstall_Charset(0);
    //gb2312字符编码,依赖:字符集模块
    ModuleInstall_CharsetGb2312(0);
    //ascii字符集,注意,gb2312包含了ascii,初始化了gb2312后,无须本模块
    //依赖:字符集模块
//    ModuleInstall_CharsetAscii(0);
    //初始化utf8字符集
//    ModuleInstall_CharsetUtf8(0);
    //国际化字符集支持,依赖所有字符集模块以及具体字符集初始化
    ModuleInstall_CharsetNls("C");


    ModuleInstall_Font(0);                 //字体模块
    //8*8点阵的ascii字体依赖:字体模块
//    ModuleInstall_FontAscii8x8Font(0);
    //6*12点阵的ascii字体依赖:字体模块
//    ModuleInstall_FontAscii6x12Font(0);
    //从数组安装GB2312点阵字体,包含了8*16的ascii字体.依赖:字体模块
    ModuleInstall_FontGb2312_816_1616_Array(0);
    //从文件安装GB2312点阵字体,包含了8*16的ascii字体.依赖:字体模块,文件系统
//    ModuleInstall_FontGb2312_816_1616_File("sys:\\gb2312_1616");
    //8*16 ascii字体初始化,包含高128字节,依赖:字体模块
    //注:如果安装了GB2312,无须再安装
//    ModuleInstall_FonAscii8x16Font(0);

//    extern ptu32_t ModuleInstall_DjyIp(ptu32_t para);
//    ModuleInstall_DjyIp(0);
//    bool_t Enet1_ModuleInit(ptu32_t para);
//    Enet1_ModuleInit(0);

    ModuleInstall_GK(0);

    //依赖: gkernel模块
    lcd_k70 = ModuleInstall_LCD(480,272,LCD_DEV_NAME,"extram");

    //创建桌面,依赖:显示器驱动
    GK_ApiCreateDesktop(lcd_k70,&desktop,0,0,
                        CN_COLOR_RED+CN_COLOR_GREEN,CN_WINBUF_BUF,0,0);

    //触摸屏模块,依赖:gkernel模块和显示器驱动
    ModuleInstall_Touch(0);
     //触摸屏驱动,
     //依赖:触摸屏模块,宿主显示器驱动,以及所依赖的硬件,例如qh_1的IIC驱动.
     //     如果矫正数据存在文件中,还依赖文件系统.
     ModuleInstall_TOUCH_TWR_K70(&desktop,TOUCH_DEV_NAME);

    //看门狗模块,如果启动了加载时喂硬件狗,看门狗软件模块从此开始接管硬件狗.
//    ModuleInstall_Wdt(0);

    ModuleInstall_GDD(&desktop,gdd_input_dev);
    //事件的两个参数暂设为0,如果用shell启动,可用来采集shell命令行参数
    evtt_main = Djy_EvttRegist(EN_CORRELATIVE,CN_PRIO_RRS,0,0,__djy_main,
                                    NULL,gc_u32CfgMainStackLen,"main function");
    //事件的两个参数暂设为0，如果用shell启动，可用来采集shell命令行参数
    Djy_EventPop(evtt_main,NULL,0,NULL,0,200);


    //用户自己程序的初始化模块,建议放在这里.
    Heap_DynamicModuleInit(0);//自此malloc函数执行块相联算法
    //至此,初始化时使用的栈,已经被系统回收,本函数就此结束,否则会死得很难看
    return ;
}


