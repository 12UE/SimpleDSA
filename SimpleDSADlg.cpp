
// SimpleDSADlg.cpp: 实现文件
//

#include "pch.h"
#include "framework.h"
#include "SimpleDSA.h"
#include "SimpleDSADlg.h"
#include "afxdialogex.h"
#include<iostream>
#include<vector>
#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CSimpleDSADlg 对话框



CSimpleDSADlg::CSimpleDSADlg(CWnd* pParent /*=nullptr*/)
	: CDialog(IDD_SIMPLEDSA_DIALOG, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CSimpleDSADlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CSimpleDSADlg, CDialog)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BUTTON1, &CSimpleDSADlg::OnBnClickedButton1)
END_MESSAGE_MAP()


// CSimpleDSADlg 消息处理程序

BOOL CSimpleDSADlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// 设置此对话框的图标。  当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	// TODO: 在此添加额外的初始化代码

	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。  对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CSimpleDSADlg::OnPaint()
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
		CDialog::OnPaint();
	}
}

//当用户拖动最小化窗口时系统调用此函数取得光标
//显示。
HCURSOR CSimpleDSADlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}




void CSimpleDSADlg::OnBnClickedButton1()
{
	auto print = [](int number) {
		std::cout << number << " ";
	};
	AllocConsole();
	freopen("CONOUT$", "w+t", stdout);// Application Writing
	freopen("CONIN$", "r+t", stdin); // Apply for reading
	dsa::Vector<int> v(3);
	//随机放入20个数
	for (int i = 0; i < 10; i++)
	{
		v.insert(rand() % 20);
	}
	//插入10个有序数
	for (int i = 0; i < 10; i++)
	{
		v.insert(i);
	}
	std::cout<<"data"<<std::endl;	
	v.traverse(print);
	//排序
	v.sort();
	std::cout << std::endl << "sort" << std::endl;
	v.traverse(print);
	//去除重复数据
	v.deduplicate();
	std::cout << std::endl << "duplicate" << std::endl;
	v.traverse(print);
	std::cout << std::endl << "remove" << std::endl;
	v.remove(5);
	v.traverse(print);
}
