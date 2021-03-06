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
//所属模块:功能函数库
//作者：mr.logic
//版本：V1.0.0
//文件描述:CRC校验部分
//其他说明:
//修订历史:
//2. ...
//1. 日期: 2009-01-04
//   作者: lst
//   新版本号: V1.0.0
//   修改说明: 原始版本
//------------------------------------------------------
#include "stdint.h"
#include "arch_feature.h"
static const u16 crc16_tab[] = {
    0x0000, 0x1021, 0x2042, 0x3063, 0x4084, 0x50a5, 0x60c6, 0x70e7,
    0x8108, 0x9129, 0xa14a, 0xb16b, 0xc18c, 0xd1ad, 0xe1ce, 0xf1ef,
    0x1231, 0x0210, 0x3273, 0x2252, 0x52b5, 0x4294, 0x72f7, 0x62d6,
    0x9339, 0x8318, 0xb37b, 0xa35a, 0xd3bd, 0xc39c, 0xf3ff, 0xe3de,
    0x2462, 0x3443, 0x0420, 0x1401, 0x64e6, 0x74c7, 0x44a4, 0x5485,
    0xa56a, 0xb54b, 0x8528, 0x9509, 0xe5ee, 0xf5cf, 0xc5ac, 0xd58d,
    0x3653, 0x2672, 0x1611, 0x0630, 0x76d7, 0x66f6, 0x5695, 0x46b4,
    0xb75b, 0xa77a, 0x9719, 0x8738, 0xf7df, 0xe7fe, 0xd79d, 0xc7bc,
    0x48c4, 0x58e5, 0x6886, 0x78a7, 0x0840, 0x1861, 0x2802, 0x3823,
    0xc9cc, 0xd9ed, 0xe98e, 0xf9af, 0x8948, 0x9969, 0xa90a, 0xb92b,
    0x5af5, 0x4ad4, 0x7ab7, 0x6a96, 0x1a71, 0x0a50, 0x3a33, 0x2a12,
    0xdbfd, 0xcbdc, 0xfbbf, 0xeb9e, 0x9b79, 0x8b58, 0xbb3b, 0xab1a,
    0x6ca6, 0x7c87, 0x4ce4, 0x5cc5, 0x2c22, 0x3c03, 0x0c60, 0x1c41,
    0xedae, 0xfd8f, 0xcdec, 0xddcd, 0xad2a, 0xbd0b, 0x8d68, 0x9d49,
    0x7e97, 0x6eb6, 0x5ed5, 0x4ef4, 0x3e13, 0x2e32, 0x1e51, 0x0e70,
    0xff9f, 0xefbe, 0xdfdd, 0xcffc, 0xbf1b, 0xaf3a, 0x9f59, 0x8f78,
    0x9188, 0x81a9, 0xb1ca, 0xa1eb, 0xd10c, 0xc12d, 0xf14e, 0xe16f,
    0x1080, 0x00a1, 0x30c2, 0x20e3, 0x5004, 0x4025, 0x7046, 0x6067,
    0x83b9, 0x9398, 0xa3fb, 0xb3da, 0xc33d, 0xd31c, 0xe37f, 0xf35e,
    0x02b1, 0x1290, 0x22f3, 0x32d2, 0x4235, 0x5214, 0x6277, 0x7256,
    0xb5ea, 0xa5cb, 0x95a8, 0x8589, 0xf56e, 0xe54f, 0xd52c, 0xc50d,
    0x34e2, 0x24c3, 0x14a0, 0x0481, 0x7466, 0x6447, 0x5424, 0x4405,
    0xa7db, 0xb7fa, 0x8799, 0x97b8, 0xe75f, 0xf77e, 0xc71d, 0xd73c,
    0x26d3, 0x36f2, 0x0691, 0x16b0, 0x6657, 0x7676, 0x4615, 0x5634,
    0xd94c, 0xc96d, 0xf90e, 0xe92f, 0x99c8, 0x89e9, 0xb98a, 0xa9ab,
    0x5844, 0x4865, 0x7806, 0x6827, 0x18c0, 0x08e1, 0x3882, 0x28a3,
    0xcb7d, 0xdb5c, 0xeb3f, 0xfb1e, 0x8bf9, 0x9bd8, 0xabbb, 0xbb9a,
    0x4a75, 0x5a54, 0x6a37, 0x7a16, 0x0af1, 0x1ad0, 0x2ab3, 0x3a92,
    0xfd2e, 0xed0f, 0xdd6c, 0xcd4d, 0xbdaa, 0xad8b, 0x9de8, 0x8dc9,
    0x7c26, 0x6c07, 0x5c64, 0x4c45, 0x3ca2, 0x2c83, 0x1ce0, 0x0cc1,
    0xef1f, 0xff3e, 0xcf5d, 0xdf7c, 0xaf9b, 0xbfba, 0x8fd9, 0x9ff8,
    0x6e17, 0x7e36, 0x4e55, 0x5e74, 0x2e93, 0x3eb2, 0x0ed1, 0x1ef0,
};

const u8 ecc_table[] = {
0x00,0x55,0x59,0x0c,0x65,0x30,0x3c,0x69,0x69,0x3c,0x30,0x65,0x0c,0x59,0x55,0x00,
0x95,0xc0,0xcc,0x99,0xf0,0xa5,0xa9,0xfc,0xfc,0xa9,0xa5,0xf0,0x99,0xcc,0xc0,0x95,
0x99,0xcc,0xc0,0x95,0xfc,0xa9,0xa5,0xf0,0xf0,0xa5,0xa9,0xfc,0x95,0xc0,0xcc,0x99,
0x0c,0x59,0x55,0x00,0x69,0x3c,0x30,0x65,0x65,0x30,0x3c,0x69,0x00,0x55,0x59,0x0c,
0xa5,0xf0,0xfc,0xa9,0xc0,0x95,0x99,0xcc,0xcc,0x99,0x95,0xc0,0xa9,0xfc,0xf0,0xa5,
0x30,0x65,0x69,0x3c,0x55,0x00,0x0c,0x59,0x59,0x0c,0x00,0x55,0x3c,0x69,0x65,0x30,
0x3c,0x69,0x65,0x30,0x59,0x0c,0x00,0x55,0x55,0x00,0x0c,0x59,0x30,0x65,0x69,0x3c,
0xa9,0xfc,0xf0,0xa5,0xcc,0x99,0x95,0xc0,0xc0,0x95,0x99,0xcc,0xa5,0xf0,0xfc,0xa9,
0xa9,0xfc,0xf0,0xa5,0xcc,0x99,0x95,0xc0,0xc0,0x95,0x99,0xcc,0xa5,0xf0,0xfc,0xa9,
0x3c,0x69,0x65,0x30,0x59,0x0c,0x00,0x55,0x55,0x00,0x0c,0x59,0x30,0x65,0x69,0x3c,
0x30,0x65,0x69,0x3c,0x55,0x00,0x0c,0x59,0x59,0x0c,0x00,0x55,0x3c,0x69,0x65,0x30,
0xa5,0xf0,0xfc,0xa9,0xc0,0x95,0x99,0xcc,0xcc,0x99,0x95,0xc0,0xa9,0xfc,0xf0,0xa5,
0x0c,0x59,0x55,0x00,0x69,0x3c,0x30,0x65,0x65,0x30,0x3c,0x69,0x00,0x55,0x59,0x0c,
0x99,0xcc,0xc0,0x95,0xfc,0xa9,0xa5,0xf0,0xf0,0xa5,0xa9,0xfc,0x95,0xc0,0xcc,0x99,
0x95,0xc0,0xcc,0x99,0xf0,0xa5,0xa9,0xfc,0xfc,0xa9,0xa5,0xf0,0x99,0xcc,0xc0,0x95,
0x00,0x55,0x59,0x0c,0x65,0x30,0x3c,0x69,0x69,0x3c,0x30,0x65,0x0c,0x59,0x55,0x00,
};

u32 ecc_count_bits(u8 x)
{
    u32 r = 0;
    
#if (CN_BYTE_BITS == 32)
    x &= 0xFF;
#endif//#if (CN_BYTE_BITS == 32)

    while(x)
    {
        if(x & 1) r++;
        x >>= 1;
    }
    return r;
}

//----计算ecc值----------------------------------------------------------------
//功能: 产生ecc效验码
//参数: data : 要效验的数据首地址
//      ecc : 存放ecc效验的首地址
//返回: 无
//----------------------------------------------------------------------------
void Ecc_Make256(const u8 *data,u8 *ecc)
{
    u32 i;
    u8 col_parity = 0;
    u8 line_parity = 0;
    u8 line_parity_prime = 0;
    u8 t;
    u8 b;

    for(i = 0; i < 256; i++)
    {
        b = ecc_table[*data++];
        col_parity ^= b;

        if(b & 0x01)
        {
            line_parity ^= i;
            line_parity_prime ^= ~i;
        }

    }

    ecc[2] = (~col_parity) | 0x03;

    t = 0;
    if(line_parity       & 0x80) t |= 0x80;
    if(line_parity_prime & 0x80) t |= 0x40;
    if(line_parity       & 0x40) t |= 0x20;
    if(line_parity_prime & 0x40) t |= 0x10;
    if(line_parity       & 0x20) t |= 0x08;
    if(line_parity_prime & 0x20) t |= 0x04;
    if(line_parity       & 0x10) t |= 0x02;
    if(line_parity_prime & 0x10) t |= 0x01;
    ecc[1] = ~t;

    t = 0;
    if(line_parity       & 0x08) t |= 0x80;
    if(line_parity_prime & 0x08) t |= 0x40;
    if(line_parity       & 0x04) t |= 0x20;
    if(line_parity_prime & 0x04) t |= 0x10;
    if(line_parity       & 0x02) t |= 0x08;
    if(line_parity_prime & 0x02) t |= 0x04;
    if(line_parity       & 0x01) t |= 0x02;
    if(line_parity_prime & 0x01) t |= 0x01;
    ecc[0] = ~t;

    t = ecc[0];
    ecc[0] = ecc[1];
    ecc[1] = t;
}


//----ecc检测并修正数据-------------------------------------------------------
//功能: ecc检测并修正数据
//参数: data : 检测的数据
//      old_ecc : 之前效验的ecc
//返回: 0 : 正确
//      1 : 数据被修正
//      2 : 之前效验的ecc是错误的
//      -1: 数据无法恢复
//----------------------------------------------------------------------------
s32 Ecc_Corect256(u8 *data, const u8 *old_ecc)
{
    u8 d0, d1, d2;
    u8 new_ecc[3];
    //把data进行ecc运算，生成新的ecc
    Ecc_Make256(data,new_ecc);

    // 新旧ecc进行异或运算，如果等于1，说明有问题
    d0 = old_ecc[0] ^ new_ecc[0];
    d1 = old_ecc[1] ^ new_ecc[1];
    d2 = old_ecc[2] ^ new_ecc[2];
    if((d0 | d1 | d2) == 0)
    {
        return 0;
    }

    if( ((d0 ^ (d0 >> 1)) & 0x55) == 0x55 )
    if( ((d1 ^ (d1 >> 1)) & 0x55) == 0x55 )
    if( ((d2 ^ (d2 >> 1)) & 0x54) == 0x54 )
    {
        unsigned byte;
        unsigned bit;
        u8 t;

        t = d0;
        d0 = d1;
        d1 = t;
        bit = byte = 0;

        if(d1 & 0x80) byte |= 0x80;
        if(d1 & 0x20) byte |= 0x40;
        if(d1 & 0x08) byte |= 0x20;
        if(d1 & 0x02) byte |= 0x10;
        if(d0 & 0x80) byte |= 0x08;
        if(d0 & 0x20) byte |= 0x04;
        if(d0 & 0x08) byte |= 0x02;
        if(d0 & 0x02) byte |= 0x01;

        if(d2 & 0x80) bit |= 0x04;
        if(d2 & 0x20) bit |= 0x02;
        if(d2 & 0x08) bit |= 0x01;

        data[byte] ^= (1 << bit);

        return 1;
    }

    if((ecc_count_bits(d0)+ecc_count_bits(d1)+ecc_count_bits(d2)) == 1)
    {
        return 2;
    }
    else
    {
        return -1;
    }
}

u16 Crc_16(u8 *buf, u32 len)
{
    u32 i;
    u16 cksum;

    cksum = 0;
    for (i = 0;  i < len;  i++) {
    cksum = crc16_tab[((cksum>>8) ^ *buf++) & 0xFF] ^ (cksum << 8);
    }
    
#if (CN_BYTE_BITS == 32)
    cksum &= 0x0000ffff;
#endif	//#if (CN_BYTE_BITS == 32)

    return cksum;
}
