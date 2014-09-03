/*******************************************************************************************
 * 文件: gui_svc.h
 *
 * 说明: gui服务器
 *
 * 作者: Jun
 *
 * 时间: 2010-11-11
*******************************************************************************************/
#ifndef FAMES_GUI_SVC_H
#define FAMES_GUI_SVC_H

/*-----------------------------------------------------------------------------------------
 * 
 * 函数声明
 * 
**---------------------------------------------------------------------------------------*/
HANDLE   gui_service_get_handle(void);
void     gui_service_timer(void * data, INT16S nr);
BOOL     gui_service_msg_handler(HANDLE task, MSGCB * msg);
BOOL     gui_service_initialize(void);
void     gui_service_wait_refresh(void);
void     gui_service_wait_refresh_ex(int times);


#endif /* #ifndef FAMES_GUI_SVC_H */

/*=========================================================================================
 * 
 * 本文件结束: gui_svc.h
 * 
**=======================================================================================*/


