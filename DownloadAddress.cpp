//////////////////////////////////////////////////
//类名：CDownloadAddress
//功能：下载网页设置
//作者：徐景周(Johnny Xu, xujingzhou2016@gmail.com)
//组织：未来工作室(Future Studio)
//日期：2001.12.1
//////////////////////////////////////////////////
#include "stdafx.h"
#include "helptip.h"
#include "DownloadAddress.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDownloadAddress dialog
CDownloadAddress::CDownloadAddress(CWnd* pParent /*=NULL*/)
	: CDialog(CDownloadAddress::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDownloadAddress)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CDownloadAddress::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDownloadAddress)
	DDX_Control(pDX, IDC_DOWNLOAD, m_Address);
	DDX_Control(pDX, IDCANCEL, m_Cancel);
	DDX_Control(pDX, IDOK, m_Download);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDownloadAddress, CDialog)
	//{{AFX_MSG_MAP(CDownloadAddress)
	ON_BN_CLICKED(IDOK, OnOk)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDownloadAddress message handlers
// ---------------------------------------------------------
//	名称: OnInitDialog
//	功能: 初始化下载对话框
//	参数: 无
//	返回: TRUE --成功，FALSE -- 失败
//	编写: 徐景周(Johnny Xu, xujingzhou2016@gmail.com)
//  组织: 未来工作室(Future Studio)
//	日期: 2002.1.8
// ---------------------------------------------------------
BOOL CDownloadAddress::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// 网页下载设置对话框中，初始化阴影位图按钮
    m_Download.SetTextColor(RGB(0,0,0));
	m_Download.SetToolTipText("下载");  
	m_Download.SetShade(SHS_HARDBUMP,10,20,5,RGB(55,55,255));

	m_Cancel.SetTextColor(RGB(0,0,0));
	m_Cancel.SetToolTipText("取消");  
	m_Cancel.SetShade(SHS_HARDBUMP,10,20,5,RGB(55,55,255));  	

	m_strAddress="";

	m_Address.SetWindowText("http://");

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

// ---------------------------------------------------------
//	名称: OnOk
//	功能: 开始下载网页
//	参数: 无
//	返回: 无
//	编写: 徐景周(Johnny Xu, xujingzhou2016@gmail.com)
//  组织: 未来工作室(Future Studio)
//	日期: 2002.1.8
// ---------------------------------------------------------
void CDownloadAddress::OnOk() 
{
	m_Address.GetWindowText(m_strAddress);

	if ("" == m_strAddress)
	{
		AfxMessageBox("\n 抱歉,下载网址不能为空! ");
		return ;
	}

	CString szHTTP	= m_strAddress;
	szHTTP	= szHTTP.Left(5);
	szHTTP.MakeUpper();

	// 增加一个合法的网址头
	if(_T("HTTP:") != szHTTP) 
		m_strAddress = _T("HTTP://") + m_strAddress;

	CDialog::OnOK();	// 关闭此对话框，必须加入，否则不能返回IDOK
}

// ---------------------------------------------------------
//	名称: GetDownloadAddress
//	功能: 获取当前网页下载地址
//	参数: 无
//	返回: 成功 -- 要下载的网址,　失败 -- 空子串
//	编写: 徐景周(Johnny Xu, xujingzhou2016@gmail.com)
//  组织: 未来工作室(Future Studio)
//	日期: 2002.1.8
// ---------------------------------------------------------
CString CDownloadAddress::GetDownloadAddress()  
{
	return m_strAddress;
}

// ---------------------------------------------------------
//	名称: PreTranslateMessage
//	功能: 截获ESC和回车键，避免按下此键时关闭对话框
//	参数: pMsg -- 消息
//	返回: 成功返回TRUE，否则返回FALSE
//	编写: 徐景周(Johnny Xu, xujingzhou2016@gmail.com)，2002.4.8
// ---------------------------------------------------------
BOOL CDownloadAddress::PreTranslateMessage(MSG* pMsg) 
{
	//截获ESC和回车键，避免按下此键时关闭对话框
	if (pMsg->message == WM_KEYDOWN)
    { 
      if(pMsg->wParam==VK_ESCAPE)
		  return true; 
	  if(pMsg->wParam==VK_RETURN) 
	  {
		  return true; 
	  }
    } 
	
	return CDialog::PreTranslateMessage(pMsg);
}
