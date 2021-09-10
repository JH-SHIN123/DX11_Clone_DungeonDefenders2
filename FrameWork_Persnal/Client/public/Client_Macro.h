#pragma once
#ifndef __CLIENT_MACRO_H__

#define GET_CURSOR_MANAGER CCursor_Manager::GetInstance()
#define GET_CURSOR_XY CCursor_Manager::GetInstance()->Get_Mouse()
#define GET_DATA_MANAGER CData_Manager::GetInstance()

#define __CLIENT_MACRO_H__
#endif // !__CLIENT_MACRO_H__
