//////////////////////////////////////////////////
//类名：CMirrorInWater
//功能：水中倒影特效对话框实现
//作者：徐景周(Johnny Xu, xujingzhou2016@gmail.com)
//组织：未来工作室(Future Studio)
//日期：2004.12.1
//////////////////////////////////////////////////
#include "stdafx.h"
#include "helptip.h"
#include "MirrorInWater.h"

#include "WaterRoutine.h"				// 水纹特效头文件
#include "DIBSectionLite.h"				// DIB位图处理头文件
#include <complex>						// 复数结构头文件
//#include <math.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

// 匿名名字空间，可存放全局成员(变量或涵数)
namespace
{
	const int TIMER_MIRRORINWATER = 21;	// 水中倒影定时器ID
	
	using std::complex;					// using声明complex直接可用
}

// ---------------------------------------------------------
//	类名: CMirrorInWaterImpl
//	功能: CMirrorInWater中镶套类，实现编译时间缩减的防火墙类
//	附注: 可以将CMirrorInWater类中所有私有成员(包括变量和涵数),
//		  全部放入此类中，虚涵数和保护成员不要放入。
//	编写: 徐景周(Johnny Xu, xujingzhou2016@gmail.com)
//  组织: 未来工作室(Future Studio)
//	日期: 2002.1.8
// ---------------------------------------------------------
class CMirrorInWater::CMirrorInWaterImpl
{
public:
	// 获取指定范围内的随机数
	inline int GetRandomRange(int nMin,int nMax)
	{
		srand((unsigned)time(NULL));
		return (rand() % ((nMax+1)-nMin))+nMin;
	}

	// 自平方分形算法，模拟云朵移动效果
	void SelfSquare(CDC* pDC, complex<double> lambda, complex<double> z, int count);
	// 自平方分形算法中点计算
	void CalculatePoint(complex<double> lambda, complex<double>& z);

	// 水平或垂直方向镜像位图(默认水平镜像)
	HBITMAP GetInvertedBitmap(HBITMAP hBitmap, BOOL bLateral = TRUE);

	CDIBSectionLite		m_bmpRenderSource;		// 源参照图
	CDIBSectionLite		m_bmpRenderTarget;		// 目标效果图
	CWaterRoutine		m_Water;				// 水纹效果对象

	int					m_bmpWidth;				// 载入位图宽度
	int					m_bmpHeight;			// 载入位图高度

	CBitmap				m_bmp;					// 正常位图
	CBitmap				m_bmpMirror;			// 垂直镜像后的位图

	double				m_dbMove;				// 云朵移动速度
};

// ---------------------------------------------------------
//	名称: CMirrorInWater
//	功能: 构造涵数，初始化工作
//	参数: 无
//	返回: 无
//	编写: 徐景周
//	日期: 2002.1.8
// ---------------------------------------------------------
CMirrorInWater::CMirrorInWater(CWnd* pParent /*=NULL*/)
	: CDialog(CMirrorInWater::IDD, pParent)
	, pImpl( new CMirrorInWaterImpl)
{
	//{{AFX_DATA_INIT(CMirrorInWater)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}

// ---------------------------------------------------------
//	名称: ~CMirrorInWater
//	功能: 退出时清除工作
//	参数: 无
//	返回: 无
//	编写: 徐景周
//	日期: 2002.1.8
// ---------------------------------------------------------
CMirrorInWater::~CMirrorInWater() throw()
{
	// 由于pImpl为智能指针，不用在此自己清除
//	delete pImpl;
//	pImpl = NULL;
}

// ---------------------------------------------------------
//	名称: CMirrorInWater
//	功能: 拷贝构造涵数，拷贝赋值工作
//	参数: other	-- 同类型要拷贝参数
//	返回: 无
//	编写: 徐景周
//	日期: 2002.1.8
// ---------------------------------------------------------
CMirrorInWater::CMirrorInWater(CMirrorInWater& other)
	: pImpl(other.pImpl)
{
}

// ---------------------------------------------------------
//	名称: operator=
//	功能: 拷贝赋值运算符，进行赋值工作，具有强烈异常安全性
//	参数: other	-- 同类型要赋值参数
//	返回: 赋值后的值
//	编写: 徐景周(Johnny Xu, xujingzhou2016@gmail.com)
//	日期: 2002.1.8
// ---------------------------------------------------------
CMirrorInWater& CMirrorInWater::operator=(CMirrorInWater& other)
{
	CMirrorInWater temp(other);
	Swap(temp);

	return *this;
}

// ---------------------------------------------------------
//	名称: Swap
//	功能: 异常安全的进行值交换工作，不抛出异常
//	参数: other	-- 同类型要交换值参数
//	返回: 无
//	编写: 徐景周(Johnny Xu, xujingzhou2016@gmail.com)
//	日期: 2002.1.8
// ---------------------------------------------------------
void CMirrorInWater::Swap(CMirrorInWater& other) throw()
{
	std::auto_ptr<CMirrorInWaterImpl> temp(pImpl);
	pImpl		= other.pImpl;
	other.pImpl	= temp;
}

void CMirrorInWater::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CMirrorInWater)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CMirrorInWater, CDialog)
	//{{AFX_MSG_MAP(CMirrorInWater)
	ON_WM_PAINT()
	ON_WM_TIMER()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMirrorInWater message handlers
// ---------------------------------------------------------
//	名称: OnInitDialog
//	功能: 初始化水中倒影对话框
//	参数: 无
//	返回: TRUE --成功，FALSE -- 失败
//	编写: 徐景周(Johnny Xu, xujingzhou2016@gmail.com)
//  组织: 未来工作室(Future Studio)
//	日期: 2002.1.8
// ---------------------------------------------------------
BOOL CMirrorInWater::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// 获取载入位图的宽度、高度
	BITMAP bm;
	pImpl->m_bmp.LoadBitmap(IDB_ADVERTISE);
	pImpl->m_bmp.GetObject(sizeof(BITMAP), &bm);
	pImpl->m_bmpWidth  = bm.bmWidth;
	pImpl->m_bmpHeight = bm.bmHeight;

	// 获取边框及标题栏大小
	int nBorderHeight  = GetSystemMetrics(SM_CYBORDER);
	int nBorderWidth   = GetSystemMetrics(SM_CXBORDER);
	int nCaptionHeight = GetSystemMetrics(SM_CYCAPTION);
	
	// 初始置窗体大小为背景位图大小(高度为位图高度2倍)
	CRect rcWindowRect,rcDeskTopWnd;
	rcWindowRect.SetRect(0,
		0,
		pImpl->m_bmpWidth + 2*nBorderWidth,
		(2*pImpl->m_bmpHeight + nCaptionHeight + 2*nBorderHeight));
	
	::GetWindowRect(::GetDesktopWindow(),&rcDeskTopWnd);

	// 居中放置
	int nHeight			= rcWindowRect.Height();
	int nWidth			= rcWindowRect.Width();
	rcWindowRect.top	= rcDeskTopWnd.Height()/2 - nHeight/2;
	rcWindowRect.bottom = rcWindowRect.top + nHeight;
	rcWindowRect.left	= rcDeskTopWnd.Width()/2 - nWidth/2;
	rcWindowRect.right	= rcWindowRect.left + nWidth;

	// 置窗体为显示及其大小位置
	MoveWindow(rcWindowRect,FALSE);

	// 将位图垂直镜像后放到m_bmpMirror中
	pImpl->m_bmpMirror.Attach(pImpl->GetInvertedBitmap(pImpl->m_bmp,FALSE));

	// 初始化特效位图
	CPictureHolder TmpPicture;
	TmpPicture.CreateFromBitmap(pImpl->m_bmpMirror);
	pImpl->m_bmpRenderSource.Create32BitFromPicture(&TmpPicture,pImpl->m_bmpWidth,pImpl->m_bmpHeight);
	pImpl->m_bmpRenderTarget.Create32BitFromPicture(&TmpPicture,pImpl->m_bmpWidth,pImpl->m_bmpHeight);
	
	// 创建水纹对象
	pImpl->m_Water.Create(pImpl->m_bmpWidth,pImpl->m_bmpHeight);

	pImpl->m_dbMove = 0.01;

	// 启动定时器，显示默认特效
	SetTimer(TIMER_MIRRORINWATER,100,NULL);
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

// ---------------------------------------------------------
//	名称: OnPaint
//	功能: 刷新窗体背景位图
//	参数: 无
//	返回: 无
//	编写: 徐景周(Johnny Xu, xujingzhou2016@gmail.com)
//  组织: 未来工作室(Future Studio)
//	日期: 2002.1.8
// ---------------------------------------------------------
void CMirrorInWater::OnPaint() 
{
	CPaintDC dc(this); // device context for painting
	
	// 绘制窗体上半部分正常显示位图
	CDC memDC;
	memDC.CreateCompatibleDC(&dc);
	CBitmap* pOldBitmap = memDC.SelectObject(&pImpl->m_bmp);
	
	dc.BitBlt(0, 0, pImpl->m_bmpWidth, pImpl->m_bmpHeight, &memDC, 0, 0, SRCCOPY);

	// 恢复及清扫工作
	memDC.SelectObject(pOldBitmap);         
	memDC.DeleteDC();

	// 绘制窗体下半部分垂直镜像后的位图
	CPoint ptOrigin(0,pImpl->m_bmpHeight);
	pImpl->m_bmpRenderTarget.Stretch(&dc,ptOrigin,CSize(pImpl->m_bmpWidth,pImpl->m_bmpHeight));
}

// ---------------------------------------------------------
//	名称: OnTimer
//	功能: 定时更新显示
//	参数: nIDEvent	--	定时器ID
//	返回: 无
//	编写: 徐景周(Johnny Xu, xujingzhou2016@gmail.com)
//  组织: 未来工作室(Future Studio)
//	日期: 2002.1.8
// ---------------------------------------------------------
void CMirrorInWater::OnTimer(UINT nIDEvent) 
{
	if(TIMER_MIRRORINWATER == nIDEvent)
	{	
		int nTemp	= pImpl->GetRandomRange(0,2);
		int nX		= pImpl->GetRandomRange(5,pImpl->m_bmpWidth-5);
		int nY		= pImpl->GetRandomRange(5,pImpl->m_bmpHeight-5);
		if(0 == nTemp)					// 仅随机显示各种水纹效果
			pImpl->m_Water.WarpBlob(nX,nY,20,300,pImpl->m_Water.m_iHpage);
		else if(1 == nTemp)
			pImpl->m_Water.SineBlob(nX,nY,10,100,pImpl->m_Water.m_iHpage);
		else if(2 == nTemp)
			pImpl->m_Water.HeightBlob(nX,nY,10,100,pImpl->m_Water.m_iHpage);

		pImpl->m_Water.Render((DWORD*)pImpl->m_bmpRenderSource.GetDIBits(),(DWORD*)pImpl->m_bmpRenderTarget.GetDIBits());
		
		// 绘制窗体下半部分垂直镜像后的位图内部数据处理后的效果
		CClientDC dc(this);
		CPoint ptOrigin(0,pImpl->m_bmpHeight);
		pImpl->m_bmpRenderTarget.Stretch(&dc,ptOrigin,CSize(pImpl->m_bmpWidth,pImpl->m_bmpHeight));

		// 绘制窗体上半部分正常显示位图
		CDC memDC;
		memDC.CreateCompatibleDC(&dc);
		CBitmap* pOldBitmap = memDC.SelectObject(&pImpl->m_bmp);
		
		dc.BitBlt(0, 0, pImpl->m_bmpWidth, pImpl->m_bmpHeight, &memDC, 0, 0, SRCCOPY);
		
		// 恢复及清扫工作
		memDC.SelectObject(pOldBitmap);         
		memDC.DeleteDC();

		// 利用自平方分形算法，模拟云朵移动效果
		complex<double> a(1.42,1), b(2,1);
		pImpl->SelfSquare(&dc, a, b, 3000);
	}
	
	CDialog::OnTimer(nIDEvent);
}

// ---------------------------------------------------------
//	名称: PreTranslateMessage
//	功能: 截获ESC和回车键，避免按下此键时关闭对话框
//	参数: pMsg -- 消息
//	返回: 成功 -- TRUE，失败 -- FALSE
//	编写: 徐景周(Johnny Xu, xujingzhou2016@gmail.com)
//  组织: 未来工作室(Future Studio)
//	日期: 2002.1.8
// ---------------------------------------------------------
BOOL CMirrorInWater::PreTranslateMessage(MSG* pMsg) 
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

// ---------------------------------------------------------
//	名称: GetInvertedBitmap
//	功能: 进行水平或垂直方向镜像处理
//	参数: hBitmap		: 传入要镜像的位图句柄
//		  bLateral	
//						= TRUE	:	水平方向镜像(默认值)
//						= FALSE	:	垂直方向镜像
//	返回: 传回镜像处理后的HBITMAP
//	编写: 徐景周(Johnny Xu, xujingzhou2016@gmail.com)
//  组织: 未来工作室(Future Studio)
//	日期: 2002.1.8
// ---------------------------------------------------------
HBITMAP CMirrorInWater::CMirrorInWaterImpl::GetInvertedBitmap(HBITMAP hBitmap, BOOL bLateral)
{
	// 创建兼容DC
	CDC sourceDC, destDC;
	sourceDC.CreateCompatibleDC(NULL);
	destDC.CreateCompatibleDC(NULL);

	// 获取位图宽、高
	BITMAP bm;
	::GetObject(hBitmap, sizeof( bm ), &bm);

	// 创建返回结果位图
	HBITMAP hbmResult = ::CreateCompatibleBitmap(CClientDC(NULL), 
						bm.bmWidth, bm.bmHeight);

	// 选入相应位图到DC中
	HBITMAP hbmOldSource = (HBITMAP)::SelectObject(sourceDC.m_hDC, hBitmap);
	HBITMAP hbmOldDest = (HBITMAP)::SelectObject(destDC.m_hDC, hbmResult);
	
	if( bLateral )				// 水平镜像
		destDC.StretchBlt(0, 0, bm.bmWidth, bm.bmHeight, &sourceDC, 
				bm.bmWidth-1, 0, -bm.bmWidth, bm.bmHeight, SRCCOPY);
	else						// 垂直镜像
		destDC.StretchBlt(0, 0, bm.bmWidth, bm.bmHeight, &sourceDC, 
				0, bm.bmHeight-1, bm.bmWidth, -bm.bmHeight, SRCCOPY);

	// 恢复源设置
	::SelectObject(sourceDC.m_hDC, hbmOldSource);
	::SelectObject(destDC.m_hDC, hbmOldDest);

	return hbmResult;
}

// ---------------------------------------------------------
//	名称: CalculatePoint
//	功能: 自平方分形算法点计算
//	参数: lambda -- 参数值, z -- 吸附点
//	返回: 无
//	编写: 徐景周(Johnny Xu, xujingzhou2016@gmail.com)
//  组织: 未来工作室(Future Studio)
//	日期: 2002.1.8
// ---------------------------------------------------------
void CMirrorInWater::CMirrorInWaterImpl::CalculatePoint(complex<double> lambda, complex<double>& z)
{ 
	double lambdaMagSq, discrMag;
	static complex<double> fourOverLambda;
	static BOOL firstPoint	= TRUE;

	if(firstPoint)
	{
		lambdaMagSq			= std::norm(lambda);
		fourOverLambda		= 4.0 * lambda/lambdaMagSq;
		firstPoint			= FALSE;
	}

	complex<double> discr;
	discr.real(1.0 - (z.real() * fourOverLambda.real() - z.imag() * fourOverLambda.imag()));
	discr.imag(z.real() * fourOverLambda.imag() + z.imag() * fourOverLambda.real());
	discrMag				= std::abs(discr);

	// 更新Z值，进行负值处理
	double tmp				= sqrt((discrMag + discr.real())/2.0);
	if(discrMag + discr.real() < 0)
		z.real(0);
	else
		z.real(tmp);

	tmp = sqrt((discrMag - discr.real())/2.0);
	if(discrMag - discr.real() < 0)
		z.imag(0);
	else
		z.imag(0.5 * tmp);

	// 随机选取一半点为负值
	if(rand() < RAND_MAX/2)
	{
		z.real(-z.real());
		z.imag(-z.imag());
	}
	if(discr.imag() < 0)
		z.real(-z.real());

	// 最终Z实部值
	z.real(0.5 * (1 - z.real()));
}

// ---------------------------------------------------------
//	名称: SelfSquare
//	功能: 自平方分形算法，模拟云朵移动效果
//	参数: pDC -- 相关DC, lambda -- 参数值, z -- 吸附点, count -- 点数
//	返回: 无
//	编写: 徐景周(Johnny Xu, xujingzhou2016@gmail.com)
//  组织: 未来工作室(Future Studio)
//	日期: 2002.1.8
// ---------------------------------------------------------
void CMirrorInWater::CMirrorInWaterImpl::SelfSquare(CDC* pDC, complex<double> lambda, complex<double> z, int count)
{
	ASSERT(FALSE == IsBadReadPtr(pDC, sizeof(CDC*)));

	int k;

	// 初始计算几个点
	for(k = 0; k < 10; ++k)
		CalculatePoint(lambda, z);
	// 自平方分形点计算，模似云朵
	for(k = 0; k < count; ++k)
	{
		CalculatePoint(lambda, z);
		// 绘制处理后的新点
		pDC->SetPixelV(static_cast<int>(m_dbMove * m_bmpWidth + z.real() * m_bmpWidth/2), static_cast<int>(m_bmpHeight + z.imag() * m_bmpHeight), RGB(255,0,255));
	}

	// 云朵移动速度处理
	m_dbMove += 0.01;
	if(m_dbMove > 1.0)
		m_dbMove = 0.01;
}
