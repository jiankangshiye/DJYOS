//----------------------------------------------------
// Copyright (c) 2014, SHENZHEN PENGRUI SOFT CO LTD. All rights reserved.

// Redistribution and use in source and binary forms, with or without
// modification, are permitted provided that the following conditions are met:

// 1. Redistributions of source code must retain the above copyright notice,
//    this list of conditions and the following disclaimer.
// 2. Redistributions in binary form must reproduce the above copyright notice,
//    this list of conditions and the following disclaimer in the documentation
//    and/or other materials provided with the distribution.
// 3. As a constituent part of djyos,do not transplant it to other software
//    without specific prior written permission.

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
// 这份授权条款，在使用者符合以下三条件的情形下，授予使用者使用及再散播本
// 软件包装原始码及二进位可执行形式的权利，无论此包装是否经改作皆然：
//
// 1. 对于本软件源代码的再散播，必须保留上述的版权宣告、本条件列表，以
//    及下述的免责声明。
// 2. 对于本套件二进位可执行形式的再散播，必须连带以文件以及／或者其他附
//    于散播包装中的媒介方式，重制上述之版权宣告、本条件列表，以及下述
//    的免责声明。
// 3. 本软件作为都江堰操作系统的组成部分，未获事前取得的书面许可，不允许移植到非
//    都江堰操作系统环境下运行。

// 免责声明：本软件是本软件版权持有人以及贡献者以现状（"as is"）提供，
// 本软件包装不负任何明示或默示之担保责任，包括但不限于就适售性以及特定目
// 的的适用性为默示性担保。版权持有人及本软件之贡献者，无论任何条件、
// 无论成因或任何责任主义、无论此责任为因合约关系、无过失责任主义或因非违
// 约之侵权（包括过失或其他原因等）而起，对于任何因使用本软件包装所产生的
// 任何直接性、间接性、偶发性、特殊性、惩罚性或任何结果的损害（包括但不限
// 于替代商品或劳务之购用、使用损失、资料损失、利益损失、业务中断等等），
// 不负任何责任，即在该种使用已获事前告知可能会造成此类损害的情形下亦然。
//-----------------------------------------------------------------------------

//所属模块: 字符编码驱动
//作者：mjh
//版本：V1.0.0
//文件描述: 字符编码相关库函数
//其他说明: 本文件内的函数将作为库函数使用，函数命名参照业界习惯，不加模块前缀
//修订历史:
//    2. ...
//    1. 日期:2011-05-17
//       作者:mjh
//       新版本号：
//       修改说明:初始版本
//------------------------------------------------------
#include "stdint.h"
#include "stddef.h"
#include "charset.h"
#include "loc_string.h"

//----计算字符长度-------------------------------------------------------------
//功能: 计算一个当前字符集的多字节字符的长度(字节数)。
//参数: mbs，保存多字节字符的缓冲区指针
//      n，最大检测长度，
//返回: 0: mbs是NULL指针或者指向空串。
//      -1:mbs指向的不是合法多字节字符，或者长度n内未能检测到完整多字节字符
//      其他:mbs缓冲区内第一个完整多字节字符的长度。
//-----------------------------------------------------------------------------
s32 mblen  (const char* mbs, u32 n)
{
    u32 wc;
    struct tagCharset* cur_enc;

    if(mbs == NULL)
        return 0;

    cur_enc = Charset_NlsGetCurCharset();
    if(cur_enc == NULL)
        return 0;

    return cur_enc->mb_to_ucs4(&wc,mbs,n);
}

//----计算字符长度-------------------------------------------------------------
//功能: 计算一个指定字符集多字节字符的长度(字节数)。
//参数: mbs，保存多字节字符的缓冲区指针
//      n，最大检测长度，
//返回: 0: mbs是NULL指针或者指向空串。
//      -1:mbs指向的不是合法多字节字符，或者长度n内未能检测到完整多字节字符
//      其他:mbs缓冲区内第一个完整多字节字符的长度。
//-----------------------------------------------------------------------------
s32 mblen_l(const char* mbs, u32 n, struct tagCharset* locale)
{
    u32 wc;

    if((mbs == NULL) || (locale == NULL))
        return 0;

    return locale->mb_to_ucs4(&wc,mbs,n);
}

//----多字节字符转为ucs4字符---------------------------------------------------
//功能: 把一个当前字符集的多字节字符转换为ucs4字符。
//参数: pwc，保存转换结果的指针，缓冲区由调用方提供
//      mbs，保存多字节字符的缓冲区指针
//      n，最大检测长度，
//返回: 0: pwc,mbs是NULL指针或者mbs指向空串。
//      -1:mbs指向的不是合法多字节字符，或者长度n内未能检测到完整多字节字符
//      其他:mbs缓冲区内第一个完整多字节字符的长度。
//-----------------------------------------------------------------------------
s32 mbtowc(u32* pwc, const char* mbs, u32 n)
{
    struct tagCharset* cur_enc;

    if((pwc == NULL) || (mbs == NULL))
        return 0;

    cur_enc = Charset_NlsGetCurCharset();
    if(cur_enc == NULL)
        return 0;
    return cur_enc->mb_to_ucs4(pwc,mbs,n);
}

//-----------------------------------------------------------------------------
//功能: 如果c不包含一个合法的当前字符集字符，返回WEOF，否则返回一个ucs4字符。
//参数: c，待转换的字符
//输出: 无
//返回: WEOF，或者ucs4字符
//------------
//更新记录:
// 1.日期: 2014/12/10
//   说明: 创建
//   作者: 罗侍田
//-----------------------------------------------------------------------------
s32 btowc(char c)
{
    #warning  待实现
    return 0;
}
//----多字节字符转为ucs4字符---------------------------------------------------
//功能: 把一个指定字符集的多字节字符转换为ucs4字符。
//参数: pwc，保存转换结果的指针，缓冲区由调用方提供
//      mbs，保存多字节字符的缓冲区指针
//      n，最大检测长度，
//返回: 0: pwc,mbs是NULL指针或者mbs指向空串。
//      -1:mbs指向的不是合法多字节字符，或者长度n内未能检测到完整多字节字符
//      其他:mbs缓冲区内第一个完整多字节字符的长度。
//-----------------------------------------------------------------------------
s32 mbtowc_l(u32* pwc, const char* mbs, u32 n, struct tagCharset* locale)
{
    if((pwc == NULL) || (mbs == NULL) || (locale == NULL))
        return 0;

    return locale->mb_to_ucs4(pwc,mbs,n);
}

//----多字节字符串转为ucs4串---------------------------------------------------
//功能: 把一个当前字符集的多字节字符串转换为ucs4字符串
//参数: pwcs，保存转换结果的指针，缓冲区由调用方提供，若空，则本函数转变为只计算
//          保存转换结果所需的字节数
//      mbs，保存多字节字符的缓冲区指针
//      n，最大检测长度，
//返回: 0: mbs是NULL指针.
//      -1:结束条件到达前，有不能转换的字符
//      其他:得到的字符数，=n表示源串是不包含串结束符'\0'。
//-----------------------------------------------------------------------------
s32 mbstowcs(u32* pwcs, const char* mbs, u32 n)
{
    struct tagCharset* cur_enc;

    if(pwcs == NULL)
        return 0;

    cur_enc = Charset_NlsGetCurCharset();
    if(cur_enc == NULL)
        return 0;
    return cur_enc->mbs_to_ucs4s(pwcs,mbs,n);
}

//----多字节字符串转为ucs4串---------------------------------------------------
//功能: 把一个指定字符集的多字节字符串转换为ucs4字符串
//参数: pwcs，保存转换结果的指针，缓冲区由调用方提供，若空，则本函数转变为只计算
//          保存转换结果所需的字节数
//      mbs，保存多字节字符的缓冲区指针
//      n，最大检测长度，
//返回: 0: mbs是NULL指针.
//      -1:结束条件到达前，有不能转换的字符
//      其他:得到的字符数，=n表示源串是不包含串结束符'\0'。
//-----------------------------------------------------------------------------
s32 mbstowcs_l(u32* pwcs, const char* mbs, u32 n,
                    struct tagCharset* locale)
{
    if((pwcs == NULL) || (locale == NULL))
        return 0;

    return locale->mbs_to_ucs4s(pwcs,mbs,n);
}

//----ucs4字符转为多字节字符---------------------------------------------------
//功能: 把一个ucs4字符转换为当前字符集的多字节字符。
//参数: mb，保存转换结果的指针，缓冲区由调用方提供
//      wc，待转换的字符
//返回: 0: mb是NULL指针
//      -1:wc不是待转换的字符集内有效的字符
//      其他:转换结果的字符长度(字节数)
//-----------------------------------------------------------------------------
s32 wctomb(char* mb, u32 wc)
{
    struct tagCharset* cur_enc;

    if(mb == NULL)
        return 0;

    cur_enc = Charset_NlsGetCurCharset();
    if(cur_enc == NULL)
        return 0;
    return cur_enc->ucs4_to_mb(mb, wc);
}

//----ucs4字符转为多字节字符---------------------------------------------------
//功能: 把一个ucs4字符转换为指定字符集的多字节字符。
//参数: mb，保存转换结果的指针，缓冲区由调用方提供
//      wc，待转换的字符
//返回: 0: mb是NULL指针
//      -1:wc不是待转换的字符集内有效的字符
//      其他:转换结果的字符长度(字节数)
//-----------------------------------------------------------------------------
s32 wctomb_l(char* mb, u32 wc,struct tagCharset* locale)
{

    if((mb == NULL) || (locale == NULL))
        return 0;

    return locale->ucs4_to_mb(mb, wc);
}

//----ucs4字符串转为多字节字符串-----------------------------------------------
//功能: 把一个ucs4字符串转换为当前字符集的多字节字符串。
//参数: mbs，保存转换结果的指针，缓冲区由调用方提供，若空，则本函数转变为只计算
//          保存转换结果所需的字节数
//      pwcs，待转换的字符
//      n，最大检测长度，遇串结束符或长度达到n结束转换，注意ucs4的结束符是连续
//          4个0x00.
//返回: 0: pwcs是NULL指针
//      -1:结束条件到达前，有不能转换的字符
//      其他:写入mbs缓冲区的字节数，含串结束符'\0'
//-----------------------------------------------------------------------------
s32 wcstombs(char* mbs, const u32* pwcs, u32 n)
{
    struct tagCharset* cur_enc;

    if(pwcs == NULL)
        return 0;

    cur_enc = Charset_NlsGetCurCharset();
    if(cur_enc == NULL)
        return 0;
    return cur_enc->ucs4s_to_mbs(mbs,pwcs,n);
}

//----ucs4字符串转为多字节字符串-----------------------------------------------
//功能: 把一个ucs4字符串转换为指定字符集的多字节字符串。
//参数: mbs，保存转换结果的指针，缓冲区由调用方提供，若空，则本函数转变为只计算
//          保存转换结果所需的字节数
//      pwcs，待转换的字符
//      n，最大检测长度，遇串结束符或长度达到n结束转换，注意ucs4的结束符是连续
//          4个0x00.
//返回: 0: pwcs是NULL指针
//      -1:结束条件到达前，有不能转换的字符
//      其他:写入mbs缓冲区的字节数，含串结束符'\0'
//-----------------------------------------------------------------------------
s32 wcstombs_l(char* mbs, const u32* pwcs, u32 n,
                struct tagCharset* locale)
{
    if((pwcs == NULL) || (locale == NULL))
        return 0;

    return locale->ucs4s_to_mbs(mbs,pwcs,n);
}

