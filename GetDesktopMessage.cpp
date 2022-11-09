#include "pch.h"
#include "GetDesktopMessage.h"

GetDesktopMessage::GetDesktopMessage()
{
	
	//��ȡ����ͼ����Ϣ������
	// ��������д���Ϊ��ȡWIN7���ϰ汾�Ļ�ȡ"progman"���ڵ�������������  ��ȡʧ���ʱȽϸ�
	hDeskTop = ::FindWindow("progman", NULL);
	hDeskTop = ::FindWindowEx(hDeskTop, 0, "shelldll_defview", NULL);
	hDeskTop = ::FindWindowEx(hDeskTop, 0, "syslistview32", NULL); //��ȡ���洰�ھ��;
	
	//����ⲿ�ִ����ȡ����ͼ����ʧ��  ���ҡ�WorkerW���Ĵ��ھ��
	if (hDeskTop==NULL)
	{
		//ѭ����ȡ���������  �����ҳ�����SysListView32���ͼ�괰�ھ��
		HWND dwndparent;
		HWND dwndviem = NULL;
		dwndparent = ::FindWindowEx(0, 0, "WorkerW", NULL);//��õ�һ��WorkerW��Ĵ��ڣ�
		while ((!dwndviem) && dwndparent)//��Ϊ���ܻ��ж����������Ϊ��WorkerW���Ĵ��ڴ��ڣ�����ֻ�����α���
		{
			dwndviem = ::FindWindowEx(dwndparent, 0, "SHELLDLL_DefView", 0);
			dwndparent = ::FindWindowEx(0, dwndparent, "WorkerW", NULL);
		}
		hDeskTop =::FindWindowEx(dwndviem, 0, "SysListView32", "FolderView");
	}

	//�ж����ַ�ʽ�Ƿ��ȡ����ͼ�괰�ھ��
	if (hDeskTop == NULL)
	{
		if (IDOK == MessageBox(NULL, "erro", "���ھ����ȡʧ�ܣ�������ϵͳ���� ��ȷ���˳�", MB_ICONSTOP | MB_OK))
		{
			exit(-1);//�˳����
		}
		return;
	}
	GetWindowThreadProcessId(hDeskTop, &dwProcessId);//ͨ�����洰�ھ����ȡ�˴������ڽ��̵�PID,��ʵ����explorer����
	hProcess = OpenProcess(PROCESS_ALL_ACCESS, FALSE, dwProcessId);//��zhiָ��PID����,ȡ�ý��̾��
	
	GetIcon();//��ȡ������Ϣ������

	//��ȡ����¼,��ȡͼ����Ϣ������
	int z = 0;
	FILE* fpr = fopen("icon pos and name.txt", "r");
	if (fpr == NULL)//����ļ������ڻ�򲻿�ֱ�Ӵ���һ�� ���Ұ�����ͼ����Ϣд�뵽�ļ���
	{
		FILE* fpw = fopen("icon pos and name.txt", "w");
		if (fpw == NULL)
			return;//�ļ�����ʧ��
		else
		{
			for (size_t i = 0; i < mD_iconCount; i++)
			{
				fprintf(fpw, "[%.2d] (x,y)=(%.4ld,%.4ld)\t\tname:[%s]\n", i, 
					ptDesk[i].x, ptDesk[i].y, itemDesk[i]); //����ͼ����Ϣ���ļ���
				//�������ȡ����Ϣ�����������ļ�������
				ptFile[i].x = ptDesk[i].x;
				ptFile[i].y = ptDesk[i].y;
				strcpy_s(itemFile[i], 512, itemDesk[i]);
				mF_iconCount = mD_iconCount;
			}
			fclose(fpw);//�ر��ļ�
		}
	}
	else
	{
		while (feof(fpr) == 0)//�ļ����ݶ�ȡ������
		{
			fscanf(fpr, "[%d] (x,y)=(%ld,%ld)\t\tname:[%[^\]]]\n", &z,
				&ptFile[z].x, &ptFile[z].y, itemFile[z]);//�ļ����ݶ�ȡ������
			z++;
		}
		fclose(fpr);//�ر��ļ�
	}	
}

//��������
GetDesktopMessage::~GetDesktopMessage()
{	
	//�ͷ�����Ŀռ�
	VirtualFreeEx(hProcess, lpvPt, 0, MEM_RELEASE);
	VirtualFreeEx(hProcess, _lv, 0, MEM_RELEASE);
	VirtualFreeEx(hProcess, _item, 0, MEM_RELEASE);
	CloseHandle(hProcess);//�رվ��
}

//��ȡ����ͼ����Ϣ
int GetDesktopMessage::GetIcon()
{
	//��ȡ����ͼ�������Ϣ
	//�������д�����Է��ڳ�Ա����GetIcon()��
	lpvPt = VirtualAllocEx(hProcess, NULL, sizeof(POINT), MEM_COMMIT, PAGE_READWRITE);//��ָ��������������һ��POINI�ṹ��С�Ŀռ�.
	_lv = (tagLVITEM64A*)VirtualAllocEx(hProcess, NULL, sizeof(tagLVITEM64A), MEM_COMMIT, PAGE_READWRITE);//��ָ��������������һ���ı��ṹ��С�Ŀռ�.
	mD_iconCount = ListView_GetItemCount(hDeskTop);
	_item = (char*)VirtualAllocEx(hProcess, NULL, 512, MEM_COMMIT, PAGE_READWRITE);
	ZeroMemory(&lvi, sizeof(LVITEM));
	lvi.mask = LVIF_TEXT;
	lvi.cchTextMax = 512;
	for (size_t i = 0; i < mD_iconCount; i++)
	{
		//����
		ListView_GetItemPosition(hDeskTop, i, lpvPt);//��ȡ��i��ͼ�������,����lpvPt
		ReadProcessMemory(hProcess, lpvPt, &ptDesk[i], sizeof(POINT), NULL);
		//����
		lvi.iSubItem = 0;
		lvi.pszText = (INT64)_item;
		WriteProcessMemory(hProcess, _lv, &lvi, sizeof(LVITEM), NULL);
		SendMessage(hDeskTop, LVM_GETITEMTEXT, (WPARAM)i, (LPARAM)_lv);
		ReadProcessMemory(hProcess, _item, itemDesk[i], 512, NULL);
	}
	return 0;
}

//����ͼ����Ϣ���ļ�
int GetDesktopMessage::BackupIcon()
{
	//��ȡ����¼//��ȡͼ����Ϣ������
	GetIcon();//��ȡ����ͼ����Ϣ
	FILE* fpw = fopen("icon pos and name.txt", "w");
	if (fpw == NULL)
		return -1;//�ļ�����ʧ��
	else
	{
		for (size_t i = 0; i < mD_iconCount; i++)
		{
			fprintf(fpw, "[%.2d] (x,y)=(%.4ld,%.4ld)\t\tname:[%s]\n", i,
				ptDesk[i].x, ptDesk[i].y, itemDesk[i]); //����ͼ����Ϣ���ļ���
		}
		fclose(fpw);//�ر��ļ�
	}
	return 0;
}

//��ԭͼ����Ϣ����
int GetDesktopMessage::RestoreIcon()
{
	int i = 0;
	FILE* fpr = fopen("icon pos and name.txt", "r");
	if (fpr == NULL)//����ļ������ڻ�򲻿�ֱ�Ӵ���һ�� ���Ұ�����ͼ����Ϣд�뵽�ļ���
	{
		printf("û�л�ԭ�㣡���ȱ���\n");
		return -1;
	}
	else
	{
		while (feof(fpr) == 0)//�ļ����ݶ�ȡ������
		{
			fscanf(fpr, "[%d] (x,y)=(%ld,%ld)\t\tname:[%[^\]]]\n", &i,
				&ptFile[i].x, &ptFile[i].y, itemFile[i]);//�ļ����ݶ�ȡ������
			i++;
		}
		fclose(fpr);//�ر��ļ�
	}
	return 0;
}

//�ƶ�ͼ��λ�õ�ԭ����λ��
int GetDesktopMessage::MoveIcon()
{
	RestoreIcon();//��ԭͼ����Ϣ����
	
	for (size_t i = 0; i < mD_iconCount; i++)
	{
		for (size_t z = 0; z < mD_iconCount; z++)
		{
			if (0 == strcmp(itemDesk[i], itemFile[z]))//�ж����������е��ļ����Ƿ����- �����ж��ǲ���ͬһ��ͼ��
			{
				::SendMessage(hDeskTop, LVM_SETITEMPOSITION, i, MAKELPARAM(ptFile[z].x, ptFile[z].y));
				break;
			}
		}
	}
	ListView_RedrawItems(hDeskTop, 0, ListView_GetItemCount(hDeskTop) - 1);//�ػ�����ͼ��
	::UpdateWindow(hDeskTop);//ˢ��
	return 0;	
}