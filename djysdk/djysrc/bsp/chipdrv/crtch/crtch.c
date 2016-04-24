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
// =============================================================================
// Copyright (C) 2012-2020 长园继保自动化有限公司 All Rights Reserved
// 文件名     ：crtouch.c
// 模块描述: 触摸芯片CRTOUCH驱动
// 模块版本: V1.10
// 创建人员: hm
// 创建时间: 15/10.2014
// =============================================================================

#include <stddef.h>
#include <string.h>
#include <stdio.h>
#include "stdint.h"

#include "int_hard.h"
#include "int.h"
#include "cpu_peri.h"
#include "Touch.h"
#include "iicbus.h"
#include "gkernel.h"
#include "systime.h"

#define CRT_XY_AD    0x03
#define CRT_STATUS   0X01

//定义IICBUS架构下的IIC设备结构
static struct tagIIC_Device *ps_CRT_Dev = NULL;
static u32 s_CRT_Timeout = CN_TIMEOUT_FOREVER;

#define CRT_ADDRESS			0x49			//设备地址
#define CRT_CLK_FRE			(100*1000)		//总线速度，单位Hz

#define CRT_SR_RTST_MASK       (1<<7)

// =============================================================================
// 功能：读取触摸屏AD采样值，分别为X值和Y值，未做校正的数据
// 参数：X,Y，输出参数，存储读取到的数据地址
// 返回：true,读取成功;false,读取失败
// =============================================================================
static bool_t CRT_ReadXY(s32 *X, s32 *Y)
{
    u8 buf[4],RegAddr,wBytesNum;
    bool_t result = false;

    RegAddr = CRT_XY_AD;
    wBytesNum = 4;
    wBytesNum = IIC_Read(ps_CRT_Dev,RegAddr,buf,wBytesNum,s_CRT_Timeout);
    if(4 == wBytesNum)
    {
        *X = (buf[0]<<8) + buf[1];
        *Y = (buf[2]<<8) + buf[3];
        result = true;
    }

    return result;
}

// =============================================================================
// 功能：读取此时是否触摸屏上有触摸
// 参数：无
// 返回：1,触摸;0,未触摸
// =============================================================================
static bool_t CRT_Touched(void)
{
    u8 buf,RegAddr,wBytesNum;
    bool_t result = false;

    RegAddr = CRT_STATUS;
    wBytesNum = 1;
    wBytesNum = IIC_Read(ps_CRT_Dev,RegAddr,&buf,wBytesNum,s_CRT_Timeout);
    if(1 == wBytesNum)
    {
        if(buf & CRT_SR_RTST_MASK)
            result = true;
    }
    return result;
}

// =============================================================================
// 功能：CRTCH芯片初始化，包括WP写保护功能引脚初始化和加载设备到对应的IIC总线
// 参数：无
// 返回：true,成功;false,失败
// =============================================================================
bool_t CRT_Init(void)
{

	static struct tagIIC_Device s_CRT_Dev;

	//初始化IIC设备结构体
	s_CRT_Dev.DevAddr 					= CRT_ADDRESS;
	s_CRT_Dev.BitOfMemAddr 				= 8;
	s_CRT_Dev.BitOfMemAddrInDevAddr 	= 0;

	//添加CRTCH到IIC0总线
	if(NULL != IIC_DevAdd_s("IIC0","IIC_Dev_CRTCH",&s_CRT_Dev))
	{
		ps_CRT_Dev = &s_CRT_Dev;
		IIC_BusCtrl(ps_CRT_Dev,CN_IIC_SET_CLK,CRT_CLK_FRE,0);
		IIC_BusCtrl(ps_CRT_Dev,CN_IIC_SET_POLL,0,0);
		return true;
	}

	return false;
}

/*============================================================================*/

#define	TS_ORG_LD

#define TS_CAL_XOFFSET      (40)
#define TS_CAL_YOFFSET      (40)

#define TS_CAL_LU_XPOS      (TS_CAL_XOFFSET)
#define TS_CAL_LU_YPOS      (TS_CAL_YOFFSET)

#define TS_CAL_RU_XPOS      (LCD_XSIZE-TS_CAL_XOFFSET)
#define TS_CAL_RU_YPOS      (TS_CAL_YOFFSET)

#define TS_CAL_LD_XPOS      (TS_CAL_XOFFSET)
#define TS_CAL_LD_YPOS      (LCD_YSIZE-TS_CAL_YOFFSET)

#define TS_CAL_RD_XPOS      (LCD_XSIZE-TS_CAL_XOFFSET)
#define TS_CAL_RD_YPOS      (LCD_YSIZE-TS_CAL_YOFFSET)

#define TS_CAL_XSIZE        (TS_CAL_RU_XPOS-TS_CAL_LU_XPOS)
#define TS_CAL_YSIZE        (TS_CAL_RD_YPOS-TS_CAL_RU_YPOS)

typedef struct  tagTS_CFG_DATA
{
    u16 LUAdx,LUAdy;
    u16 RUAdx,RUAdy;
    u16 RDAdx,RDAdy;
    u16 LDAdx,LDAdy;
}TS_CFG_DATA;

///////////////////

static	int ts_xsize,ts_ysize;
static	int	ts_cal_ref_pos[4][2];
static  TS_CFG_DATA ts_cfg_data;   //配置数据

#define	LCD_XSIZE	ts_xsize
#define	LCD_YSIZE	ts_ysize
/*============================================================================*/
/*============================================================================*/

static bool_t ts_translate_data(s32 *x,s32 *y)
{
    int x_coor,y_coor,adx,ady;
    int ts_adx_min,ts_adx_max,ts_ady_min,ts_ady_max;

    if((x!=NULL)&&(y!=NULL))
    {
        adx =*x;
        ady =*y;

#ifdef	TS_ORG_LU
        ts_adx_min  =ts_cfg_data.LUAdx;
        ts_ady_min  =ts_cfg_data.LUAdy;

        ts_adx_max  =ts_cfg_data.RDAdx;
        ts_ady_max  =ts_cfg_data.RDAdy;

        if((ts_adx_min==ts_adx_max) || (ts_ady_min==ts_ady_max))
        {
            *x  =-1;
            *y  =-1;
            return  false;
        }

#ifdef	TS_XY_SWAP
        x_coor = TS_CAL_XOFFSET+(LCD_XSIZE-(TS_CAL_XOFFSET<<1))*(ady-ts_ady_min)/ (ts_ady_max-ts_ady_min);
        y_coor = TS_CAL_YOFFSET+(LCD_YSIZE-(TS_CAL_YOFFSET<<1))*(adx-ts_adx_min)/ (ts_adx_max-ts_adx_min);
#else
        x_coor = TS_CAL_XOFFSET+(LCD_XSIZE-(TS_CAL_XOFFSET<<1))*(adx-ts_adx_min)/ (ts_adx_max-ts_adx_min);
        y_coor = TS_CAL_YOFFSET+(LCD_YSIZE-(TS_CAL_YOFFSET<<1))*(ady-ts_ady_min)/ (ts_ady_max-ts_ady_min);
#endif
        *x =x_coor;
        *y =y_coor;
#endif

#ifdef	TS_ORG_RU
        ts_adx_min  =ts_cfg_data.RUAdx;
        ts_ady_min  =ts_cfg_data.RUAdy;

        ts_adx_max  =ts_cfg_data.LDAdx;
        ts_ady_max  =ts_cfg_data.LDAdy;

        if((ts_adx_min==ts_adx_max) || (ts_ady_min==ts_ady_max))
        {
            *x  =-1;
            *y  =-1;
            return  false;
        }

#ifdef	TS_XY_SWAP
        x_coor = TS_CAL_XOFFSET+(LCD_XSIZE-(TS_CAL_XOFFSET<<1))*(ady-ts_ady_min)/ (ts_ady_max-ts_ady_min);
        y_coor = TS_CAL_YOFFSET+(LCD_YSIZE-(TS_CAL_YOFFSET<<1))*(adx-ts_adx_min)/ (ts_adx_max-ts_adx_min);
#else
        x_coor = TS_CAL_XOFFSET+(LCD_XSIZE-(TS_CAL_XOFFSET<<1))*(adx-ts_adx_min)/ (ts_adx_max-ts_adx_min);
        y_coor = TS_CAL_YOFFSET+(LCD_YSIZE-(TS_CAL_YOFFSET<<1))*(ady-ts_ady_min)/ (ts_ady_max-ts_ady_min);
#endif
        *x  =LCD_XSIZE-x_coor;
        *y  =y_coor;
#endif

#ifdef	TS_ORG_RD
        ts_adx_min  =ts_cfg_data.RDAdx;
        ts_ady_min  =ts_cfg_data.RDAdy;

        ts_adx_max  =ts_cfg_data.LUAdx;
        ts_ady_max  =ts_cfg_data.LUAdy;

        if((ts_adx_min==ts_adx_max) || (ts_ady_min==ts_ady_max))
        {
            *x  =-1;
            *y  =-1;
            return  false;
        }

#ifdef	TS_XY_SWAP
        x_coor = TS_CAL_XOFFSET+(LCD_XSIZE-(TS_CAL_XOFFSET<<1))*(ady-ts_ady_min)/ (ts_ady_max-ts_ady_min);
        y_coor = TS_CAL_YOFFSET+(LCD_YSIZE-(TS_CAL_YOFFSET<<1))*(adx-ts_adx_min)/ (ts_adx_max-ts_adx_min);
#else
        x_coor = TS_CAL_XOFFSET+(LCD_XSIZE-(TS_CAL_XOFFSET<<1))*(adx-ts_adx_min)/ (ts_adx_max-ts_adx_min);
        y_coor = TS_CAL_YOFFSET+(LCD_YSIZE-(TS_CAL_YOFFSET<<1))*(ady-ts_ady_min)/ (ts_ady_max-ts_ady_min);
#endif
        *x  =LCD_XSIZE-x_coor;
        *y  =LCD_YSIZE-y_coor;
#endif

#ifdef	TS_ORG_LD
        ts_adx_min  =ts_cfg_data.LDAdx;
        ts_ady_min  =ts_cfg_data.LDAdy;

        ts_adx_max  =ts_cfg_data.RUAdx;
        ts_ady_max  =ts_cfg_data.RUAdy;

        if((ts_adx_min==ts_adx_max) || (ts_ady_min==ts_ady_max))
        {
            *x  =-1;
            *y  =-1;
            return  false;
        }

#ifdef	TS_XY_SWAP
        x_coor = TS_CAL_XOFFSET+(LCD_XSIZE-(TS_CAL_XOFFSET<<1))*(ady-ts_ady_min)/ (ts_ady_max-ts_ady_min);
        y_coor = TS_CAL_YOFFSET+(LCD_YSIZE-(TS_CAL_YOFFSET<<1))*(adx-ts_adx_min)/ (ts_adx_max-ts_adx_min);
#else
        x_coor = TS_CAL_XOFFSET+(LCD_XSIZE-(TS_CAL_XOFFSET<<1))*(adx-ts_adx_min)/ (ts_adx_max-ts_adx_min);
        y_coor = TS_CAL_YOFFSET+(LCD_YSIZE-(TS_CAL_YOFFSET<<1))*(ady-ts_ady_min)/ (ts_ady_max-ts_ady_min);
#endif
        *x  =x_coor;
        *y  =LCD_YSIZE-y_coor;

#endif
        return true;
    }
    return  false;
}

/*============================================================================*/


void touch_ratio_adjust(struct tagGkWinRsc *desktop);
static ufast_t read_touch_data(struct tagSingleTouchMsg *touch_data);
static bool_t touch_hard_init(void);

//----初始化触摸屏模块---------------------------------------------------------
//功能:
//参数: display_dev_name:本触摸屏对应的显示器名(资源名)
//		touch_dev_name:触摸屏设备名.
//返回: 无
//-----------------------------------------------------------------------------
bool_t ModuleInstall_TOUCH_TWR_K70(struct tagGkWinRsc *desktop,const char *touch_dev_name)
{
    static struct tagSingleTouchPrivate touch_dev;

    ts_xsize =desktop->right-desktop->left;
    ts_ysize =desktop->bottom-desktop->top;

    ts_cal_ref_pos[0][0] =TS_CAL_LU_XPOS;
    ts_cal_ref_pos[0][1] =TS_CAL_LU_YPOS;
    ts_cal_ref_pos[1][0] =TS_CAL_RU_XPOS;
    ts_cal_ref_pos[1][1] =TS_CAL_RU_YPOS;
    ts_cal_ref_pos[2][0] =TS_CAL_RD_XPOS;
    ts_cal_ref_pos[2][1] =TS_CAL_RD_YPOS;
    ts_cal_ref_pos[3][0] =TS_CAL_LD_XPOS;
    ts_cal_ref_pos[3][1] =TS_CAL_LD_YPOS;

    if(!touch_hard_init())
    {
        return false;
    }
    touch_ratio_adjust(desktop);
    touch_dev.read_touch = read_touch_data;
    Touch_InstallDevice(touch_dev_name,&touch_dev);
    return true;
}

//----触摸屏硬件初始化---------------------------------------------------------
//功能: 触摸屏硬件初始化
//参数: 无
//返回: 键盘设备号，-1表示按照键盘设备失败
//-----------------------------------------------------------------------------
static bool_t touch_hard_init(void)
{
	return CRT_Init();
}

static  bool_t ts_is_down(void)
{
	return CRT_Touched();
}

static  void InsertSort(int *A, int p, int r)
{
    int i,j;
    int key;
    for(i=p+1; i<=r; i++)
    {
        key = A[i];
        j = i-1;
        while (j >= 0 && A[j] > key)
        {
            A[j+1] = A[j];
            j--;
        }
        A[j+1] = key;
    }
}

static bool_t	ts_get_data_raw(s32 *x,s32 *y)
{
	if(ts_is_down())
	{
		CRT_ReadXY(x,y);
		return true;
	}
	return false;
}

//----读取触摸点坐标-----------------------------------------------------------
//功能: 读取stmpe811采集到的触摸点坐标，如果有多点，则平均之
//参数: touch_data，采集到的坐标
//返回: 1=触摸笔按下，0=触摸笔提起，
//-----------------------------------------------------------------------------
static ufast_t read_touch_data(struct tagSingleTouchMsg *touch_data)
{
	 static s32 x=0,y=0,z=0;

	if(ts_is_down())
	{
		ts_get_data_raw(&x,&y);
		z =1;
		ts_translate_data(&x,&y);
		touch_data->x =x;
		touch_data->y =y;
        touch_data->z = z;

        return 1;

	}
	MouseInput(x,y,0);
	return 0;

}


static	void draw_cursor(struct tagGkWinRsc *desktop,int x,int y)
{
	GK_ApiLineto(desktop,x,y,x,y-20+1,CN_COLOR_RED,CN_R2_COPYPEN,0); //上
	GK_ApiLineto(desktop,x,y,x,y+20-1,CN_COLOR_RED,CN_R2_COPYPEN,0); //下
	GK_ApiLineto(desktop,x,y,x+20-1,y,CN_COLOR_RED,CN_R2_COPYPEN,0);//右
	GK_ApiLineto(desktop,x,y,x-20+1,y,CN_COLOR_RED,CN_R2_COPYPEN,0); //左
	GK_ApiSyncShow(1000*mS);
}

static	void clr_cursor(struct tagGkWinRsc *desktop,int x,int y)
{
	GK_ApiLineto(desktop,x,y,x,y-20+1,CN_COLOR_WHITE,CN_R2_COPYPEN,0); //上
	GK_ApiLineto(desktop,x,y,x,y+20-1,CN_COLOR_WHITE,CN_R2_COPYPEN,0); //下
	GK_ApiLineto(desktop,x,y,x+20-1,y,CN_COLOR_WHITE,CN_R2_COPYPEN,0);//右
	GK_ApiLineto(desktop,x,y,x-20+1,y,CN_COLOR_WHITE,CN_R2_COPYPEN,0); //左
	GK_ApiSyncShow(1000*mS);

}

//----触摸屏校准---------------------------------------------------------------
//功能: 触摸屏的尺寸可能与液晶可显示区域不完全一致，安装也可能有偏差，需要计算
//      校准系数和偏移量。为获得更高精度，使用定点小数。
//参数: display_name,本触摸屏对应的显示器名(资源名)
//返回: 无
//-----------------------------------------------------------------------------
void touch_ratio_adjust(struct tagGkWinRsc *desktop)
{

    struct tagSingleTouchMsg touch_xyz0,touch_xyz1;
    FILE *touch_init;
    s32 limit_left,limit_top,limit_right,limit_bottom,pen_down_time,step;

    if((touch_init = fopen("sys:\\touch_init.dat","r")) != NULL)
    {

        fread(&ts_cfg_data,sizeof(TS_CFG_DATA),1,touch_init);
    }
    else
    {
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

        step=0;
        while(step<4)
        {
        	s32 adx,ady;

        	if(step>0)
        	{
        		clr_cursor(desktop,ts_cal_ref_pos[step-1][0],ts_cal_ref_pos[step-1][1]);
        	}
        	draw_cursor(desktop,ts_cal_ref_pos[step][0],ts_cal_ref_pos[step][1]);

        	while(ts_is_down())
			{
        		Djy_DelayUs(100*mS);
			}
			pen_down_time=0;
			while(1)
			{
				if(ts_is_down())
				{
					ts_get_data_raw(&adx,&ady);
					if(pen_down_time++>5)
					{
						break;
					}
				}
				else
				{
					pen_down_time=0;
				}
				Djy_DelayUs(100*mS);
			}

			switch(step)
			{
			case 0:
				ts_cfg_data.LUAdx =adx;
				ts_cfg_data.LUAdy =ady;
				break;

			case 1:
				ts_cfg_data.RUAdx =adx;
				ts_cfg_data.RUAdy =ady;
				break;

			case 2:
				ts_cfg_data.RDAdx =adx;
				ts_cfg_data.RDAdy =ady;
				break;

			case 3:
				ts_cfg_data.LDAdx =adx;
				ts_cfg_data.LDAdy =ady;
				break;

			}
			printf("ts_cal[%d]: %04X,%04X\r\n",step,adx,ady);
			step++;
        }

        GK_ApiFillWin(desktop,CN_COLOR_WHITE,0);
        GK_ApiSyncShow(1000*mS);
    	while(ts_is_down())
		{
    		Djy_DelayUs(100*mS);
		}
    	Djy_DelayUs(500*mS);

    //    GK_DestroyWin(desktop);
        touch_init = fopen("sys:\\touch_init.dat","w+");
        fwrite(&ts_cfg_data,sizeof(TS_CFG_DATA),1,touch_init);
    }
    fclose(touch_init);

}
