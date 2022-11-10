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
	DDX_Control(pDX, IDC_EDIT_TEXT, m_Edit);
	DDX_Control(pDX, IDC_BUTTON1, m_Button1);
	DDX_Control(pDX, IDC_BUTTON2, m_Button2);
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
	SetWindowText("DesktopApp");		//设置窗口标题
	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

//绘图消息处理函数
void CDesktopAppDlg::OnPaint()
{
	CDialogEx::OnPaint();//直接调用父类的绘图消息处理函数
}

//当用户拖动最小化窗口时系统调用此函数取得光标显示
HCURSOR CDesktopAppDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

//按钮1被点击处理函数(备份图标)
void CDesktopAppDlg::OnBnClickedButton1()
{
	CString str_set = "正在备份桌面图标...";
	m_Edit.SetWindowText(str_set);								//设置文本框显示内容
	m_Edit.UpdateData();										//刷新文本框内容
	m_Button1.EnableWindow(FALSE);								//开关设为不可用
	
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

	m_Edit.SetWindowText(str_set);								//向文本框输出内容
	m_Button1.EnableWindow(TRUE);								//开关设为可用
}

//按钮2被点击处理函数（还原图标）
void CDesktopAppDlg::OnBnClickedButton2()						
{
	CString str_set = "正在还原桌面图标...";						
	m_Edit.SetWindowText(str_set);								//设置文本框显示内容
	m_Edit.UpdateData();										//刷新文本框内容
	m_Button2.EnableWindow(FALSE);								//开关设为不可用

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

	m_Edit.SetWindowText(str_set);								//向文本框输出内容
	m_Button2.EnableWindow(TRUE);								//开关设为可用
}