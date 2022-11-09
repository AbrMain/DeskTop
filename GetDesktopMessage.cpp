#include "pch.h"
#include "GetDesktopMessage.h"

GetDesktopMessage::GetDesktopMessage()
{
	
	//获取桌面图标信息到类中
	// 下面的三行代码为获取WIN7以上版本的获取"progman"窗口单个桌面句柄程序  获取失败率比较高
	hDeskTop = ::FindWindow("progman", NULL);
	hDeskTop = ::FindWindowEx(hDeskTop, 0, "shelldll_defview", NULL);
	hDeskTop = ::FindWindowEx(hDeskTop, 0, "syslistview32", NULL); //获取桌面窗口句柄;
	
	//如果这部分代码获取桌面图标句柄失败  就找“WorkerW”的窗口句柄
	if (hDeskTop==NULL)
	{
		//循环获取多个桌面句柄  从中找出含有SysListView32类的图标窗口句柄
		HWND dwndparent;
		HWND dwndviem = NULL;
		dwndparent = ::FindWindowEx(0, 0, "WorkerW", NULL);//获得第一个WorkerW类的窗口，
		while ((!dwndviem) && dwndparent)//因为可能会有多个窗口类名为“WorkerW”的窗口存在，所以只能依次遍历
		{
			dwndviem = ::FindWindowEx(dwndparent, 0, "SHELLDLL_DefView", 0);
			dwndparent = ::FindWindowEx(0, dwndparent, "WorkerW", NULL);
		}
		hDeskTop =::FindWindowEx(dwndviem, 0, "SysListView32", "FolderView");
	}

	//判断两种方式是否获取桌面图标窗口句柄
	if (hDeskTop == NULL)
	{
		if (IDOK == MessageBox(NULL, "erro", "窗口句柄获取失败！请重启系统重试 按确定退出", MB_ICONSTOP | MB_OK))
		{
			exit(-1);//退出软件
		}
		return;
	}
	GetWindowThreadProcessId(hDeskTop, &dwProcessId);//通过桌面窗口句柄获取此窗口所在进程的PID,其实就是explorer进程
	hProcess = OpenProcess(PROCESS_ALL_ACCESS, FALSE, dwProcessId);//打开zhi指定PID进程,取得进程句柄
	
	GetIcon();//获取桌面信息到类中

	//读取备份录,读取图标信息到类中
	int z = 0;
	FILE* fpr = fopen("icon pos and name.txt", "r");
	if (fpr == NULL)//如果文件不存在或打不开直接创建一个 并且把桌面图标信息写入到文件中
	{
		FILE* fpw = fopen("icon pos and name.txt", "w");
		if (fpw == NULL)
			return;//文件创建失败
		else
		{
			for (size_t i = 0; i < mD_iconCount; i++)
			{
				fprintf(fpw, "[%.2d] (x,y)=(%.4ld,%.4ld)\t\tname:[%s]\n", i, 
					ptDesk[i].x, ptDesk[i].y, itemDesk[i]); //备份图标信息到文件中
				//从桌面获取的信息拷贝到类中文件缓存中
				ptFile[i].x = ptDesk[i].x;
				ptFile[i].y = ptDesk[i].y;
				strcpy_s(itemFile[i], 512, itemDesk[i]);
				mF_iconCount = mD_iconCount;
			}
			fclose(fpw);//关闭文件
		}
	}
	else
	{
		while (feof(fpr) == 0)//文件内容读取到类中
		{
			fscanf(fpr, "[%d] (x,y)=(%ld,%ld)\t\tname:[%[^\]]]\n", &z,
				&ptFile[z].x, &ptFile[z].y, itemFile[z]);//文件内容读取到类中
			z++;
		}
		fclose(fpr);//关闭文件
	}	
}

//析构函数
GetDesktopMessage::~GetDesktopMessage()
{	
	//释放申请的空间
	VirtualFreeEx(hProcess, lpvPt, 0, MEM_RELEASE);
	VirtualFreeEx(hProcess, _lv, 0, MEM_RELEASE);
	VirtualFreeEx(hProcess, _item, 0, MEM_RELEASE);
	CloseHandle(hProcess);//关闭句柄
}

//获取桌面图标信息
int GetDesktopMessage::GetIcon()
{
	//获取桌面图标相关消息
	//下面七行代码可以放在成员函数GetIcon()中
	lpvPt = VirtualAllocEx(hProcess, NULL, sizeof(POINT), MEM_COMMIT, PAGE_READWRITE);//在指定进程里面申请一个POINI结构大小的空间.
	_lv = (tagLVITEM64A*)VirtualAllocEx(hProcess, NULL, sizeof(tagLVITEM64A), MEM_COMMIT, PAGE_READWRITE);//在指定进程里面申请一个文本结构大小的空间.
	mD_iconCount = ListView_GetItemCount(hDeskTop);
	_item = (char*)VirtualAllocEx(hProcess, NULL, 512, MEM_COMMIT, PAGE_READWRITE);
	ZeroMemory(&lvi, sizeof(LVITEM));
	lvi.mask = LVIF_TEXT;
	lvi.cchTextMax = 512;
	for (size_t i = 0; i < mD_iconCount; i++)
	{
		//坐标
		ListView_GetItemPosition(hDeskTop, i, lpvPt);//获取第i个图标的坐标,存入lpvPt
		ReadProcessMemory(hProcess, lpvPt, &ptDesk[i], sizeof(POINT), NULL);
		//名称
		lvi.iSubItem = 0;
		lvi.pszText = (INT64)_item;
		WriteProcessMemory(hProcess, _lv, &lvi, sizeof(LVITEM), NULL);
		SendMessage(hDeskTop, LVM_GETITEMTEXT, (WPARAM)i, (LPARAM)_lv);
		ReadProcessMemory(hProcess, _item, itemDesk[i], 512, NULL);
	}
	return 0;
}

//备份图标信息到文件
int GetDesktopMessage::BackupIcon()
{
	//读取备份录//读取图标信息到类中
	GetIcon();//获取桌面图标信息
	FILE* fpw = fopen("icon pos and name.txt", "w");
	if (fpw == NULL)
		return -1;//文件创建失败
	else
	{
		for (size_t i = 0; i < mD_iconCount; i++)
		{
			fprintf(fpw, "[%.2d] (x,y)=(%.4ld,%.4ld)\t\tname:[%s]\n", i,
				ptDesk[i].x, ptDesk[i].y, itemDesk[i]); //备份图标信息到文件中
		}
		fclose(fpw);//关闭文件
	}
	return 0;
}

//还原图标信息到类
int GetDesktopMessage::RestoreIcon()
{
	int i = 0;
	FILE* fpr = fopen("icon pos and name.txt", "r");
	if (fpr == NULL)//如果文件不存在或打不开直接创建一个 并且把桌面图标信息写入到文件中
	{
		printf("没有还原点！请先备份\n");
		return -1;
	}
	else
	{
		while (feof(fpr) == 0)//文件内容读取到类中
		{
			fscanf(fpr, "[%d] (x,y)=(%ld,%ld)\t\tname:[%[^\]]]\n", &i,
				&ptFile[i].x, &ptFile[i].y, itemFile[i]);//文件内容读取到类中
			i++;
		}
		fclose(fpr);//关闭文件
	}
	return 0;
}

//移动图标位置到原来的位置
int GetDesktopMessage::MoveIcon()
{
	RestoreIcon();//还原图标信息到类
	
	for (size_t i = 0; i < mD_iconCount; i++)
	{
		for (size_t z = 0; z < mD_iconCount; z++)
		{
			if (0 == strcmp(itemDesk[i], itemFile[z]))//判断两个数组中的文件名是否相等- 就是判断是不是同一个图标
			{
				::SendMessage(hDeskTop, LVM_SETITEMPOSITION, i, MAKELPARAM(ptFile[z].x, ptFile[z].y));
				break;
			}
		}
	}
	ListView_RedrawItems(hDeskTop, 0, ListView_GetItemCount(hDeskTop) - 1);//重绘所有图标
	::UpdateWindow(hDeskTop);//刷新
	return 0;	
}