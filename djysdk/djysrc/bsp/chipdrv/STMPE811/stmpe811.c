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
// 这份授权条款，在使用者符合下列条件的情形下，授予使用者使用及再散播本
// 软件包装原始码及二进位可执行形式的权利，无论此包装是否经改作皆然：
//
// 1. 对于本软件源代码的再散播，必须保留上述的版权宣告、本条件列表，以
//    及下述的免责声明。
// 2. 对于本套件二进位可执行形式的再散播，必须连带以文件以及／或者其他附
//    于散播包装中的媒介方式，重制上述之版权宣告、本条件列表，以及下述
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
//所属模块:标准输入--触摸屏模块
//作者:  罗侍田.
//版本：V1.1.0
//文件描述: 触摸屏硬件驱动程序
//其他说明:
//修订历史:
//1. 日期: 2009-01-04
//   作者:  罗侍田.
//   新版本号: V1.0.0
//   修改说明: 原始版本
//------------------------------------------------------

#include "stdint.h"
#include "stdio.h"
#include "stddef.h"
#include "stm32f10x.h"
#include "Touch.h"
#include "string.h"
#include "stmpe811.h"
#include "I2C2.h"
#include "TS_I2C.h"
#include "gkernel.h"

struct tagST_TouchAdjust tg_touch_adjust;

void touch_ratio_adjust(char *display_name);
ufast_t read_touch_stmpe811(struct tagSingleTouchMsg *touch_data);
bool_t touch_hard_init(void);

//----初始化触摸屏模块---------------------------------------------------------
//功能:
//参数: display_name,本触摸屏对应的显示器名(资源名)
//返回: 无
//-----------------------------------------------------------------------------
ptu32_t ModuleInstall_Touch_Stmpe811(ptu32_t para)
{
    static struct tagSingleTouchPrivate stmpe811;
    if(!touch_hard_init( ))
        return -1;
    touch_ratio_adjust((char*)para);
    stmpe811.read_touch = read_touch_stmpe811;
    Touch_InstallDevice("stmpe811",&stmpe811);
    return 1;
}

//----触摸屏硬件初始化---------------------------------------------------------
//功能: 触摸屏硬件初始化
//参数: 无
//返回: 键盘设备号，-1表示按照键盘设备失败
//-----------------------------------------------------------------------------
bool_t touch_hard_init(void)
{
    u16 chipid;

    I2C2_Init();
    chipid = TS_Read (CHIP_ID, 2);                      //Read CHIP_ID

    if(chipid ==0x811)
    {
        TS_Write(SYS_CTRL2, 1, 0x0C);                   //打开TSC及ADC的时钟
        TS_Write(INT_EN, 1, 0x07);                      //使能中断
        //ADC分辨率（12bit），参考电压（内部）及采样时间（124）
        TS_Write(ADC_CTRL1 , 1, 0x68);
        TS_Write(ADC_CTRL2 , 1, 0x01);                  //ADC采样频率（3.25Mhz）
        TS_Write(GPIO_AF , 1, 0x00);                    //端口选择为触摸屏使用
        TS_Write(TSC_CFG, 1, 0x92);                     //TSC_CFG
        TS_Write (FIFO_TH, 1, 16);          //触摸屏的触发门限1（测量的是单击）
        //TS_Write (FIFO_TH, 1, 0x05);        //触摸屏的触发门限5（测量的是轨迹）
        TS_Write(FIFO_STA, 1, 0x01);                    //FIFO复位
        TS_Write(FIFO_STA, 1, 0x00);
        TS_Write(TSC_FRACT_XYZ, 1, 0x07);
        TS_Write(TSC_I_DRIVE, 1, 0x01);

        TS_Write(TSC_CTRL, 1, 0x01 | stmpe811_opmode<<1); //使能TSC
        TS_Write(INT_STA, 1, 0xFF);                       //清除所有中断
        TS_Write(INT_CTRL, 1, 0x01);                      //不使能TSC
        return true;
    }
    else
    {
        return false;
    }

}

//----读取触摸点坐标-----------------------------------------------------------
//功能: 读取stmpe811采集到的触摸点坐标，如果有多点，则平均之
//参数: touch_data，采集到的坐标
//返回: 1=触摸笔按下，0=触摸笔提起，
//-----------------------------------------------------------------------------
ufast_t read_touch_stmpe811(struct tagSingleTouchMsg *touch_data)
{
    s32 i, n, tch_data;
    s32 tch_x,tch_y,tch_z;
    u8 tmp,tch_int;

    tch_int = TS_Read (INT_STA, 1);     //Read Touch-screen interrupt status

    if ((tch_int & 0x02))                 //过了门限,或FIFO满，或FIFO溢出
    {
        tmp = TS_Read(INT_EN, 1);
        TS_Write(INT_EN, 1, tmp &( ~(tch_int & 0x0E)));  //读取数据时需要禁止中断

        TS_Write(INT_STA, 1, 0xff);  //读取数据时需要禁止中断

        i = TS_Read (FIFO_SIZE, 1);        //FIFO中的采样次数
        n=i;
        tch_x=0;
        tch_y=0;
        tch_z=0;

#if (stmpe811_opmode == 0)

        i <<= 2;
        while (i--)                       //需要将这些字节读出来，直到小于门限
        {
            tch_data = TS_Read(TSC_DATA, 1);
            touch_data->x = tch_data;
            tch_data |= TS_Read(TSC_DATA, 1);
            touch_data->x <<= 4;
            touch_data->x |= tch_data>>4;

            touch_data->y = tch_data & 0x0F;
            tch_data = TS_Read(TSC_DATA, 1);
            touch_data->y <<= 4;
            touch_data->y |= tch_data;
            touch_data->z = TS_Read(TSC_DATA, 1);

            tch_x +=touch_data->x;
            tch_y +=touch_data->y;
            tch_z +=touch_data->z;
        }

#elif (stmpe811_opmode == 1)
        while (i--)
        {
            tch_data = TS_Read(TSC_DATA, 1);
            touch_data->x = tch_data<<4;
            tch_data = TS_Read(TSC_DATA, 1);
            touch_data->x |= tch_data>>4;
            touch_data->y = (tch_data & 0x0F)<<8;
            tch_data = TS_Read(TSC_DATA, 1);
            touch_data->y |= tch_data;

            tch_x +=touch_data->x;
            tch_y +=touch_data->y;
            tch_z++;
        }
#endif

        //还有采样点未读完或FIFO满或FIFO溢出
        if (TS_Read(FIFO_SIZE, 1) || (tch_int & 0x0C))
        {
            TS_Write(FIFO_STA, 1, 0x01);                //FIFO复位
            TS_Write(FIFO_STA, 1, 0x00);
        }

        TS_Write(INT_EN, 1, tmp | (tch_int & 0x0E));    //重新使能中断
        TS_Write(INT_STA, 1, 0x02);

        tch_x /=n;       //平均多次座标值
        tch_y /=n;
        tch_z /=n;

        if(tg_touch_adjust.ratio_x != 0)
        {
            touch_data->x = ((tch_x << 16)- tg_touch_adjust.offset_x)
                                / tg_touch_adjust.ratio_x;
        }

        if(tg_touch_adjust.ratio_y != 0)
        {
            touch_data->y = ((tch_y << 16)- tg_touch_adjust.offset_y)
                                / tg_touch_adjust.ratio_y;
        }
        touch_data->z = tch_z;
        return 1;
    }
    else
    {
        return 0;
    }
}

//----触摸屏校准---------------------------------------------------------------
//功能: 触摸屏的尺寸可能与液晶可显示区域不完全一致，安装也可能有偏差，需要计算
//      校准系数和偏移量。为获得更高精度，使用定点小数。
//参数: display_name,本触摸屏对应的显示器名(资源名)
//返回: 无
//-----------------------------------------------------------------------------
void touch_ratio_adjust(char *display_name)
{
    struct tagGkWinRsc *desktop;
    struct tagSingleTouchMsg touch_xyz0,touch_xyz1;
    FILE *touch_init;
    s32 limit_left,limit_top,limit_right,limit_bottom;

    if((touch_init = fopen("sys:\\touch_init.dat","r")) != NULL)
    {

        fread(&tg_touch_adjust,sizeof(struct tagST_TouchAdjust),1,touch_init);
    }
    else
    {
        desktop = GK_ApiGetDesktop(display_name);

        limit_left = desktop->limit_left;
        limit_top = desktop->limit_top;
        limit_right = desktop->limit_right;
        limit_bottom = desktop->limit_bottom;
    //    GK_ApiCreateGkwin(desktop,desktop,limit_left,limit_top,limit_right,limit_bottom,
    //                      CN_COLOR_WHITE,CN_WINBUF_BUF,"&tg_touch_adjust",CN_R3_SRCCOPY,0);
    //    GK_ApiSetPrio(desktop,-1,CN_GK_SYNC);
        GK_ApiFillWin(desktop,CN_COLOR_WHITE,0);
        GK_ApiDrawText(desktop,NULL,NULL,limit_left+10,limit_top+50,
                            "触摸屏矫正",21,CN_COLOR_WHITE,CN_R2_COPYPEN,0);
        GK_ApiDrawText(desktop,NULL,NULL,limit_left+10,limit_top+70,
                            "请准确点击十字交叉点",21,CN_COLOR_WHITE,CN_R2_COPYPEN,0);
        GK_ApiLineto(desktop,0,20,40,20,CN_COLOR_RED,CN_R2_COPYPEN,0);
        GK_ApiLineto(desktop,20,0,20,40,CN_COLOR_RED,CN_R2_COPYPEN,CN_TIMEOUT_FOREVER);

        while(!read_touch_stmpe811(&touch_xyz0));           //记录触摸屏第一点校正值

        GK_ApiFillWin(desktop,CN_COLOR_WHITE,0);
        GK_ApiDrawText(desktop,NULL,NULL,limit_left+10,limit_top+50,
                            "触摸屏矫正",21,CN_COLOR_WHITE,CN_R2_COPYPEN,0);
        GK_ApiDrawText(desktop,NULL,NULL,limit_left+10,limit_top+70,
                            "再次准确点击十字交叉点",21,CN_COLOR_WHITE,CN_R2_COPYPEN,0);
        GK_ApiLineto(desktop,limit_right-40,limit_bottom-20,
                      limit_right,limit_bottom-20,
                      CN_COLOR_RED,CN_R2_COPYPEN,0);
        GK_ApiLineto(desktop,limit_right-20,limit_bottom-40,
                      limit_right-20,limit_bottom,
                      CN_COLOR_RED,CN_R2_COPYPEN,0);
        GK_ApiSyncShow(CN_TIMEOUT_FOREVER);

        if (TS_Read(FIFO_SIZE, 1))
        {
            TS_Write(FIFO_STA, 1, 0x01);                //FIFO复位
            TS_Write(FIFO_STA, 1, 0x00);
        }

        while(!read_touch_stmpe811(&touch_xyz1));           //记录触摸屏第二点校正值

        tg_touch_adjust.ratio_x = ((touch_xyz1.x - touch_xyz0.x)<<16)
                        /(limit_right - limit_left -40);
        tg_touch_adjust.offset_x = (touch_xyz0.x<<16) - 20*tg_touch_adjust.ratio_x;
        tg_touch_adjust.ratio_y = ((touch_xyz1.y - touch_xyz0.y)<<16)
                        /(limit_bottom- limit_top-40);
        tg_touch_adjust.offset_y= (touch_xyz0.y<<16) - 20*tg_touch_adjust.ratio_y;
        GK_ApiFillWin(desktop,CN_COLOR_WHITE,0);
    //    GK_DestroyWin(desktop);
        touch_init = fopen("sys:\\touch_init.dat","w+");
        fwrite(&tg_touch_adjust,sizeof(struct tagST_TouchAdjust),1,touch_init);
    }
    fclose(touch_init);

}

