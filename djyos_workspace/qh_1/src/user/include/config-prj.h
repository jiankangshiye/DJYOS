//--------------------------------------------------------
//DJYOS components configuration file.
//This file is generated by "com.djyos.utilities" plugin
//Thu Jul 12 15:48:15 CST 2012
//--------------------------------------------------------

#ifndef __CONFIG_PRJ_H_
#define __CONFIG_PRJ_H_

#ifdef __cplusplus
extern "C" {
#endif


/*____��Ӳ����ص�����____*/
#define M 1000000
#define cn_mclk (72*M)  //��Ƶ
#define cn_fclk cn_mclk  //cm3������������ʱ��
#define cn_ahbclk cn_mclk  //����ʱ��
#define cn_pclk2 cn_mclk  //��������ʱ��
#define cn_pclk1 (cn_mclk/2)  //��������ʱ��
#define cn_extclk 8000000  //�ⲿʱ��=8M

/*____���¶���tick����____*/
#define cn_tick_us 1000  //����ϵͳ�ں�ʱ�����峤�ȣ���usΪ��λ��
#define cn_tick_hz 1000  //�ں�ʱ��Ƶ�ʣ���λΪhz��
#define cn_fine_us 0x0000038e  //1/72M,����ʱ�����峤�ȣ���uSΪ��λ��32λ������������С����ռ16λ����Ҳ������ticks�������65535uS
#define cn_fine_hz cn_mclk  //�ں˾���ʱ��Ƶ�ʣ���cn_fine_ns�ĵ�����

/*____ʱ���������____*/
#define cfg_time_module 1  //�Ƿ����ʱ��ģ��
#define cfg_time_zone (+8)  //����ʱ��
#define cn_time_src_tick 0  //�ں������������ʱ��׼
#define cn_time_src_rtc 1  //Ӳ��rtc����ʱ��׼
#define cfg_time_src cn_time_src_rtc  //ѡ����ʱ��׼

/*____cpu����ģʽ����____*/
#define cn_mode_tiny 0  //tinyģʽ
#define cn_mode_si 1  //��ӳ��ģʽ
#define cn_mode_dlsp 2  //��̬���ص�����ģʽ
#define cn_mode_mp 3  //�����ģʽ��ֻ��mmu=true�ſ�ѡ���ģʽ
#define cn_run_mode cn_mode_si

/*____�ַ�����֧��gui kernel���������ò�������____*/
#define cfg_charset_gb2312 1  //gb2312����
#define cfg_charset_ascii 0  //gb2312�Ѿ�������ascii���룬������֧��
#define cfg_charset_utf8 0  //unicode�����е�һ�ֱ��뷽ʽ
#define cfg_charset_default "gb2312"  //����������C�ļ��ж�����ַ���������ͬ

/*____���壬֧��gui kernel���������ò�������____*/
#define cfg_gb2312_8x16_1616 1  //����8*16�����ascii�ַ���16*16��������庺��
#define cfg_ascii_8x8 1  //����8*8�����ascii�ַ�
#define cfg_ascii_8x16 0  //����8*16�����ascii�ַ�gb2312_816_1616���Ѿ����������뵥������
#define cfg_font_default "gb2312_8_16_1616"  //����������C�ļ���ʹ�õ�������Դ������ͬ,

/*____gui kernel����____*/
#define cfg_gk_down_chunnel_deep 1024  //�����������ȣ��ֽ���
#define cfg_gk_up_chunnel_deep 256  //gui kernel��Ϣ���������ȣ��ֽ���

/*____��������____*/
#define cn_device_limit 10  //�����豸������
#define cn_locks_limit 20  //��������п��õ���������������������ָ�û�����semp_create��mutex_create�����������������ں��õ�������
#define cn_events_limit 20  //�¼�����
#define cn_evtts_limit 20  //���¼�������
#define cn_paras_limit (cn_events_limit+cn_evtts_limit)  //�¼�����������
#define cn_wdts_limit 1  //������������
#define cn_pipes_limit 1  //�ܵ�����

/*____���ڴ������صĳ���____*/
#define cn_virtual_memory 0  //m3Ӳ����֧�������ڴ棬�����Ӳ��֧�������ڴ�Ļ����ϣ�����������֧�֣�Ҳ��Ϊ0
#define cn_phy_memoey 0  //�����ڴ�������֧�������ڴ�������塣
#define cn_page_size 0x400  //��ҳ�ߴ�Ϊ1Kbytes
#define cn_page_size_suffix_zero 10  //ҳ�ߴ��׺0�ĸ���
#define cn_mem_recycle true  //֧���ڴ����----���ù���
#define cn_mem_pools 5  //��������5���ڴ��
#define cn_syssvc_stack 1024  //ϵͳ�����¼�ջ����
#define cfg_dynamic_mem 1  //�Ƿ�֧�ָ߼��ڴ���䣬��ʹ��֧�֣�Ҳ����ʹ��malloc-free�����ڴ棬�������ϸ�ɶԵ��ã�������ڴ�й©
#define cfg_stack_fill 'd'  //ջ���ֵ����Ϊ0�����
#define cn_main_stack 1024  //main��������ջ����

/*____�ļ�ϵͳ�������____*/
#define cfg_djyfs 1  //�Ƿ�����ļ�ϵͳ
#define cfg_djyfs_flash 1  //�Ƿ����flash�ļ�ϵͳ������
#define cfg_fat_filesys 0  //�Ƿ����flash�ļ�ϵͳ������
#define cfg_djyfs_ram 0  //�Ƿ����ram�ļ�ϵͳ
#define cn_opened_fsnode_limit 16  //��ͬʱ�򿪵��ļ�������ע��༶·����ÿһ������һ���ļ�������ͬ��·�����ظ���

/*____�������____*/
#define cfg_pan_driver 1  //�Ƿ�������豸����ģ��
#define cfg_debug_info 1  //������Ϣ�ռ������ģ��
#define cfg_shell 1  //�Ƿ����shellģ��
#define cfg_pipe 1  //�Ƿ�����ܵ�ͨ��ģ��
#define cfg_wdt 0  //�Ƿ�������Ź�����
#define cfg_nude_terminal 0  //nude terminal��һ��������ʱ�ṩ��������˻��Ի����ܵ�ģ��
#define cfg_gui_kernel 1  //gui kernelģ��

/*____��׼����ģ������____*/
#define cfg_keyboard 1  //�Ƿ������������
#define cfg_single_touch 1  //�Ƿ�֧�ֵ��㴥����
#define cfg_muti_touch 0  //�Ƿ�֧�ֶ�㴥����
#define cfg_muti_keyboard 0  //0=��֧�ֶ���̣���ʹϵͳӵ�ж���̣����м���Ҳ�������ֵ�ʹ��
#define cfg_muti_mouse_2d 0  //0=��֧�ֶ���꣬��ʹϵͳӵ�ж����꣬�������Ҳ����ͬһ�����ָ�롣
#define cfg_muti_mouse_3d 0  //0=��֧�ֶ�3d��꣬��ʹϵͳӵ�ж��3d��꣬�������Ҳ����ͬһ�����ָ�롣
#define cfg_stdin_device "uart1"  //��׼�����豸
#define cfg_stdin_device_limit 3  //��֧�ֵ������豸����
#define cfg_gui_pattern_mode 0  //ģʽλͼ�ߴ磬0=�̶�8*8λͼ��1=����bitmap��V1.0.0��֧�֣�

/*____��������____*/
#define cfg_uart1_used 1
#define cfg_uart2_used 0
#define cfg_uart3_used 0
#define cfg_uart4_used 0
#define cfg_uart5_used 0

/*____����·������____*/
#define cfg_work_path "sys:\\"  //����·�������û������õĻ���set_work_path���ǿ��ַ�

#ifdef __cplusplus
}
#endif
#endif
