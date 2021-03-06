//////////////////////////////////////////////////
//类名：CTransparentWnd
//功能：透明窗体实现
//作者：徐景周(Johnny Xu, xujingzhou2016@gmail.com)
//组织：未来工作室(Future Studio)
//日期：2001.12.1
//////////////////////////////////////////////////
#if !defined(AFX_TRANSPARENTWND_H__INCLUDED_)
#define AFX_TRANSPARENTWND_H__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif

#include "XInfoTip.h"						// 加入提示窗类
#include "NTray.h"							// 加入系统托盘支持头文件
#include "BCMenu.h"							// 添加位图菜单
class CAdvertise;							// 前置声明广告特效窗体
class CMirrorInWater;						// 前置声明水中倒影窗体

class CTransparentWnd : public CWnd
{

public:
	CTransparentWnd();
	virtual ~CTransparentWnd();

	// 创建窗体
	void CreateTransparent(LPCTSTR pTitle, RECT &rect);

	// 公共鼠标右键弹出菜单
	void RButtonMenu(CPoint& point);

private:
	// --位图水平方向映像功能函数集--
	void GetWidthAndHeight(CBitmap* pBitmap, int* pw, int* ph) const ;
	void SwapYInvertBlt (CDC* pDC1, int x1, int y1,
                          int nWidth, int nHeight,
                          CDC* pDC2, int x2, int y2
                          );

	void MirrorLeftAndRightHalves (CBitmap* pBitmap);

	HBITMAP GetInvertedBitmap(HBITMAP hBitmap, BOOL bLateral = TRUE);
	// -------------------------
	
	// 开机自启动(bAddDel默认为写入)
	BOOL StartupAutorun(BOOL bExist, BOOL bAddDel=TRUE);

	// 将总位图中相应单帧映射到m_bmpDraw中
	void Map2Frame(CDC *dc,const CBitmap& bmpMap, int nColFrame,int nRowFrame);
	
	void SetupRegion(CDC *pDC);

	void DoSet(void);
	void DoWalk(void);
	void DoRun(void);
	void DoIdle(void);
	void DoDrag(void);                      // 拖动窗体时表现

	void WinAnimation(bool bShowFlag);		// 窗体显示/隐藏的动画效果    
	CRect GetTopWndTitleRect();             // 获取当前桌面最顶层窗口标题区大小
	int  GetCurAction();                    // 获取当前行为表现方式
	void SoundPlay(void);					// 播放内镶WAV文件
	BOOL GetSourceHtml(CString theUrl) ;	// 下载提示文件

	CAdvertise*		m_pADdlg ;				// 广告窗体
	CMirrorInWater* m_pMirrorInWaterDlg;	// 水中倒影窗体
	BCMenu			m_popmenu;				// 定义位图菜单
	
	BOOL			m_bAutorun;             // 是否设置了开机自启动标志
	BOOL			m_bDownloadFailed;		// 是否下载成功标志
	BOOL			m_bSuccess;				// 流文件打开是否成功
	CStdioFile		m_sdFile;				// 流文件

	CRect			m_Rect;					// 最顶层窗口标题区大小
	int				m_iAction;				// 当前行为表现方式
	CString			strPath;				// 主执行程序所在的路径
	CBitmap			m_bmpDraw;				// 单帧位图
	CBitmap			m_bmpMap;				// 整个位图(向左)
	CBitmap			m_bmpMirrorMap;			// 水平映射整个位图(向右)
	int				m_iAniSeq;				// 位图变化量
	CPoint			m_ptDest;				// 运动点
	int				m_iLastDirection;		// 方向
	bool			m_bFullScreenWalk;		// 全屏运动标志
	bool			m_bDragDrop;			// 拖动标志位
	CPoint			m_ptCurPoint;			// 记录当前鼠标位置

	HICON			m_hIcon[1];				// 系统托盘支持设置变量
	CTrayNotifyIcon m_TrayIcon;
	CXInfoTip		m_Tip;					// 加入提示窗类

public:
// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CTransparentWnd)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	//}}AFX_VIRTUAL

// Implementation
public:
	
protected:

	// Generated message map functions
protected:
	//{{AFX_MSG(CTransparentWnd)
	afx_msg LRESULT OnTrayNotification(WPARAM wParam, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg void OnRButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnExit();
	afx_msg void OnFullscreenWalk();
	afx_msg void OnShowhide();
	afx_msg void OnInitMenuPopup(CMenu* pPopupMenu, UINT nIndex, BOOL bSysMenu);
	afx_msg void OnDownload();
	afx_msg void OnActionRun();
	afx_msg void OnActionWalk();
	afx_msg void OnActionIdle();
	afx_msg void OnMeasureItem(int nIDCtl, LPMEASUREITEMSTRUCT lpMeasureItemStruct);
	afx_msg LRESULT OnMenuChar(UINT nChar, UINT nFlags, CMenu* pMenu);
	afx_msg void OnAbout();
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnAutorun();
	afx_msg void OnEffectWater();
	afx_msg void OnEffectFire();
	afx_msg void OnEffectPlasma();
	afx_msg void OnEffectFont();
	afx_msg void OnEffectElectricity();
	afx_msg void OnEffectParticle();
	afx_msg void OnEffectSnow();
	afx_msg void OnEffectScreensaver();
	afx_msg void OnEffectLogo();
	afx_msg void OnEffectFireworks();
	afx_msg void OnMirrorInWater();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif
