//----------------------------------------------------
//Copyright (C), 2011,  mjh.
//版权所有 (C), 2011,   mjh.
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
#include "os_inc.h"

#if (cfg_gui_kernel == 1)

#include "loc_string.h"
#include "encoding.h"

//----计算字符长度-------------------------------------------------------------
//功能: 计算一个当前字符集的多字节字符的长度(字节数)。
//参数: mbs，保存多字节字符的缓冲区指针
//      n，最大检测长度，
//返回: 0: mbs是NULL指针或者指向空串。
//      -1:mbs指向的不是合法多字节字符，或者长度n内未能检测到完整多字节字符
//      其他:mbs缓冲区内第一个完整多字节字符的长度。
//-----------------------------------------------------------------------------
s32 mblen(const char* mbs, u32 n)
{
    u32 wc;
    struct char_encoding_rsc* cur_enc;

    if(mbs == NULL)
        return 0;

    cur_enc = nls_get_cur_encoding_rsc();

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
s32 mblen_l(const char* mbs, u32 n, struct char_encoding_rsc* locale)
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
    struct char_encoding_rsc* cur_enc;

    if((pwc == NULL) || (mbs == NULL))
        return 0;

    cur_enc = nls_get_cur_encoding_rsc();
    return cur_enc->mb_to_ucs4(pwc,mbs,n);
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
s32 mbtowc_l(u32* pwc, const char* mbs, u32 n, struct char_encoding_rsc* locale)
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
    struct char_encoding_rsc* cur_enc;

    if(pwcs == NULL)
        return 0;

    cur_enc = nls_get_cur_encoding_rsc();
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
                    struct char_encoding_rsc* locale)
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
    struct char_encoding_rsc* cur_enc;
    
    if(mb == NULL)
        return 0;

    cur_enc = nls_get_cur_encoding_rsc();
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
s32 wctomb_l(char* mb, u32 wc,struct char_encoding_rsc* locale)
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
    struct char_encoding_rsc* cur_enc;

    if(pwcs == NULL)
        return 0;

    cur_enc = nls_get_cur_encoding_rsc();
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
                struct char_encoding_rsc* locale)
{
    if((pwcs == NULL) || (locale == NULL))
        return 0;

    return locale->ucs4s_to_mbs(mbs,pwcs,n);
}

#endif      //for #if (cfg_gui_kernel == 1)

