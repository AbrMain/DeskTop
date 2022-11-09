// DesktopAppDlg.cpp: 实现文件
//
#include "Resource.h"
#include "pch.h"
#include "framework.h"
#include "DesktopApp.h"
#include "DesktopAppDlg.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

GetDesktopMessage* GDMessage = new GetDesktopMessage;//获取桌面信息

// CDesktopAppDlg 对话框

CDesktopAppDlg::CDesktopAppDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_DESKTOPAPP_DIALOG, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CDesktopAppDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CDesktopAppDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BUTTON1, &CDesktopAppDlg::OnBnClickedButton1)
	ON_BN_CLICKED(IDC_BUTTON2, &CDesktopAppDlg::OnBnClickedButton2)
END_MESSAGE_MAP()


// CDesktopAppDlg 消息处理程序

BOOL CDesktopAppDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 设置此对话框的图标。  当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);				// 设置大图标
	SetIcon(m_hIcon, FALSE);			// 设置小图标
	// TODO: 在此添加额外的初始化代码
	SetWindowText("DesktopApp");		//设置窗口标题
	//GDMessage = new GetDesktopMessage;//获取桌面信息

	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。  对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CDesktopAppDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 用于绘制的设备上下文

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 使图标在工作区矩形中居中
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 绘制图标
	    dc.DrawIcon(x, y, m_hIcon);
	
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

//当用户拖动最小化窗口时系统调用此函数取得光标
//显示。
HCURSOR CDesktopAppDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



void CDesktopAppDlg::OnBnClickedButton1()
{
	CEdit* edit_control = (CEdit*)GetDlgItem(IDC_EDIT_TEXT);	//获取文本框变量
	CString str_set = "正在备份桌面图标...";
	edit_control->SetWindowText(str_set);						//设置文本框显示内容
	edit_control->UpdateData();									//刷新文本框内容
	CButton* button = (CButton*)GetDlgItem(IDC_BUTTON1);		//获取开关变量
	button->EnableWindow(FALSE);								//开关设为不可用
	
	//返回	0：运行正常
	//	   -1：没有备份记录（没有还原点） 无法还原

	switch (GDMessage->BackupIcon())							//备份桌面图标
	{
	case 0:
		str_set = "备份桌面图标成功";
		break;
	case -1:
		str_set = "无法创建备份文件";
		break;
	}

	edit_control->SetWindowText(str_set);						//向文本框输出内容
	button->EnableWindow(TRUE);									//开关设为可用
}

//还原桌面图标
void CDesktopAppDlg::OnBnClickedButton2()						
{
	
	CEdit* edit_control = (CEdit*)GetDlgItem(IDC_EDIT_TEXT);	//获取文本框变量
	CString str_set = "正在还原桌面图标...";						
	edit_control->SetWindowText(str_set);						//设置文本框显示内容
	edit_control->UpdateData();									//刷新文本框内容
	CButton* button = (CButton*)GetDlgItem(IDC_BUTTON2);		//获取开关变量
	button->EnableWindow(FALSE);								//开关设为不可用

	//返回	0：运行正常
	//	   -1：没有备份记录（没有还原点） 无法还原
	
	switch (GDMessage->MoveIcon())								//还原桌面图标 并且判断返回值
	{
	case 0:
		str_set = "还原桌面图标成功";
		break;
	case -1:
		str_set = "没有备份记录（没有还原点） 无法还原";
		break;
	}

	edit_control->SetWindowText(str_set);						//向文本框输出内容
	button->EnableWindow(TRUE);									//开关设为可用
}