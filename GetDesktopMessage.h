#pragma once
#include "STLVITEM64A.h"		//LPLVITEM64A，LVITEM64A结构体头文件

class GetDesktopMessage
{
public:
	GetDesktopMessage();
	~GetDesktopMessage();
	int GetIcon();//获取桌面图标信息
	int BackupIcon();//备份图标信息到文件
	int RestoreIcon();//还原图标信息到类
	int MoveIcon();//移动图标位置到原来的位置

public:
	DWORD dwProcessId;
	HWND hDeskTop;//桌面句柄
	HANDLE hProcess;
	LPVOID lpvPt;
	LPLVITEM64A _lv;		//STLVITEM64A.h头文件中
	LVITEM64A  lvi;			//STLVITEM64A.h头文件中
	char* _item;
	char itemFile[150][512] = { 0 };//备份录里面的图标名称
	char itemDesk[150][512] = { 0 };//从桌面获取的图标名称
	POINT ptFile[150];//备份录里面的图标位置信息
	POINT ptDesk[150];//从桌面获取的图标位置信息
	unsigned int mD_iconCount;//桌面图标数量
	unsigned int mF_iconCount;//文件中读取的图标数量
};
