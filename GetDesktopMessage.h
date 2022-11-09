#pragma once
#include "STLVITEM64A.h"		//LPLVITEM64A��LVITEM64A�ṹ��ͷ�ļ�

class GetDesktopMessage
{
public:
	GetDesktopMessage();
	~GetDesktopMessage();
	int GetIcon();//��ȡ����ͼ����Ϣ
	int BackupIcon();//����ͼ����Ϣ���ļ�
	int RestoreIcon();//��ԭͼ����Ϣ����
	int MoveIcon();//�ƶ�ͼ��λ�õ�ԭ����λ��

public:
	DWORD dwProcessId;
	HWND hDeskTop;//������
	HANDLE hProcess;
	LPVOID lpvPt;
	LPLVITEM64A _lv;		//STLVITEM64A.hͷ�ļ���
	LVITEM64A  lvi;			//STLVITEM64A.hͷ�ļ���
	char* _item;
	char itemFile[150][512] = { 0 };//����¼�����ͼ������
	char itemDesk[150][512] = { 0 };//�������ȡ��ͼ������
	POINT ptFile[150];//����¼�����ͼ��λ����Ϣ
	POINT ptDesk[150];//�������ȡ��ͼ��λ����Ϣ
	unsigned int mD_iconCount;//����ͼ������
	unsigned int mF_iconCount;//�ļ��ж�ȡ��ͼ������
};
