//////////////////////////////////////////////////
//类名：CDownloadAddress
//功能：下载网页设置
//作者：徐景周(Johnny Xu, xujingzhou2016@gmail.com)
//组织：未来工作室(Future Studio)
//日期：2001.12.1
//////////////////////////////////////////////////
#if !defined(AFX_DOWNLOADADDRESS_H__144A5480_5518_11D6_AA35_BE8B1831D90B__INCLUDED_)
#define AFX_DOWNLOADADDRESS_H__144A5480_5518_11D6_AA35_BE8B1831D90B__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DownloadAddress.h : header file
//
#include "HoverEdit.h"				// 平面编辑框类
#include "xShadeButton.h"			// 加入自绘式阴影位图按钮类
/////////////////////////////////////////////////////////////////////////////
// CDownloadAddress dialog

class CDownloadAddress : public CDialog
{
// Construction
public:
	CDownloadAddress(CWnd* pParent = NULL);   // standard constructor

	CString GetDownloadAddress();   // 获取当前网页下载地址
	CString m_strAddress;           // 存放当前网页下载地址

// Dialog Data
	//{{AFX_DATA(CDownloadAddress)
	enum { IDD = IDD_DOWNLOAD_ADDRESS };
	CHoverEdit	m_Address;			 // 显示为平面编辑框
	CxShadeButton	m_Cancel;
	CxShadeButton	m_Download;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDownloadAddress)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDownloadAddress)
	virtual BOOL OnInitDialog();
	afx_msg void OnOk();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DOWNLOADADDRESS_H__144A5480_5518_11D6_AA35_BE8B1831D90B__INCLUDED_)
