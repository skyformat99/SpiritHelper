/////////////////////////////////////////////////////////////////////////
//类名：CFireRoutine
//功能：火焰效果
//附注：经测试，火焰效果最耗费CPU资源(其它还可以)，请慎用
//修改：徐景周(Johnny Xu, xujingzhou2016@gmail.com)
//组织：未来工作室(Future Studio)
//日期：2002.1.8
////////////////////////////////////////////////////////////////////////
#include "stdafx.h"				// 加入预编译头文件,jingzhou xu
#include "FireRoutine.h"
//#include <time.h>

// 取指定范围内的随机数
#define getrandom( min, max ) (( rand() % (int)((( max ) + 1 ) - ( min ))) + ( min ))

// ---------------------------------------------------------
//	名称: CFireRoutine
//	功能: 构造涵数，初始化工作
//	参数: 无
//	返回: 无
//	修改: 徐景周(Johnny Xu, xujingzhou2016@gmail.com)，2002.4.8
// ---------------------------------------------------------
CFireRoutine::CFireRoutine()
{
	m_iWidth = 0;
	m_iHeight = 0;
	m_pFireBits = NULL;
	m_iFireSource = 2;
	m_iFireChance = 10;
	m_iFlameHeight = 50;
	m_iAlpha = 255;

	m_FireColors[0]  = RGB(0,0,0);		// 黑
	m_FireColors[1]  = RGB(255,0,0);	// 红
	m_FireColors[2]  = RGB(255,255,0);	// 黄
	m_FireColors[3]  = RGB(255,255,255);// 白
	m_iAvgFlameWidth = 35;
}

// ---------------------------------------------------------
//	名称: ~CFireRoutine
//	功能: 析构涵数，清扫工作
//	参数: 无
//	返回: 无
//	修改: 徐景周，2002.4.8
// ---------------------------------------------------------
CFireRoutine::~CFireRoutine()
{
	if(m_pFireBits != NULL)
		delete [] m_pFireBits;
	
	m_pFireBits = NULL;

}

// ---------------------------------------------------------
//	名称: InitFire
//	功能: 初始化火焰效果所需数据，需预先调用
//	参数: 无
//	返回: 无
//	修改: 徐景周(Johnny Xu, xujingzhou2016@gmail.com)，2002.4.8
// ---------------------------------------------------------
void CFireRoutine::InitFire()
{
	// 如果已存在，则清空它们
	if(m_pFireBits != NULL)
		delete [] m_pFireBits;

	if(m_pYIndexes.empty())
		m_pYIndexes.clear();

	// 源高度加3(为加入初始m_iFireSource种子数)
	m_iHeight+=3;

	m_pFireBits = new BYTE[m_iWidth*m_iHeight];

	// 初始化火源字节数组
	memset(m_pFireBits,0,(m_iWidth*m_iHeight));

	// 为防止原m_pYIndexes指针引起的内存泄漏，现改为vector动态数组,jingzhou xu.
	// 初始化载入位图每行对应的字节位
	for (int y = m_iHeight; y >=0; y--)  
		m_pYIndexes.push_back(y * m_iWidth);

	// 初始化调色板
	InitPallette();
	ClrHotSpots();

}

// ---------------------------------------------------------
//	名称: ClrHotSpots
//	功能: 复位火焰热点既火源所在行为0
//	参数: 无
//	返回: 无
//	修改: 徐景周，2002.4.8
// ---------------------------------------------------------
void CFireRoutine::ClrHotSpots()
{
	// 清除火源种子所在行字节
	memset(&m_pFireBits[m_pYIndexes[m_iFireSource]],0,m_iWidth);
}

// ---------------------------------------------------------
//	名称: InitPallette
//	功能: 初始化调色板数据，用于颜色处理
//	参数: 无
//	返回: 无
//	修改: 徐景周，2002.4.8
// ---------------------------------------------------------
void CFireRoutine::InitPallette()
{
	// 创建火源渐变色
	long iCount = 0;
	COLORREF clrStart;
	COLORREF clrEnd;

	for(int iColor = 1;iColor<4;iColor++)
	{

		
		clrStart = m_FireColors[iColor-1];
		clrEnd = m_FireColors[iColor];

			
		int r, g, b;					// 颜色像素值
		float rStep, gStep, bStep;		// 每种颜色步长

		// 获取红、绿、蓝色
		r = (GetRValue(clrEnd) - GetRValue(clrStart));
		g = (GetGValue(clrEnd) - GetGValue(clrStart));
		b =  (GetBValue(clrEnd) - GetBValue(clrStart));

		int nSteps = max(abs(r), max(abs(g), abs(b)));
		float fStep = (float)(255/3)/ (float)nSteps;
		// 计算各颜色步长
		rStep = r/(float)nSteps;
		gStep = g/(float)nSteps;
		bStep = b/(float)nSteps;

		// 恢复初始位置颜色
		r = GetRValue(clrStart);
		g = GetGValue(clrStart);
		b = GetBValue(clrStart);

		for (int iOnBand = 0; iOnBand < nSteps; iOnBand++) 
		{
			//COLORREF color = RGB(r+rStep*iOnBand, g + gStep*iOnBand, b + bStep *iOnBand);
			COLORREF color = RGB(b + bStep *iOnBand, g + gStep*iOnBand,r+rStep*iOnBand);

			long lIndex = (int)(iOnBand * fStep);

			if(lIndex+((iColor-1)*85) < 255)
				m_pPalletteBuffer[lIndex+((iColor-1)*85)] = color;				
		}
	}
	// 两种基本色范围
	clrStart = m_FireColors[0];
	clrEnd = m_FireColors[1];

	for(int kj=0;kj<m_iFlameHeight;kj++)
		m_pPalletteBuffer[kj] = 0;

	int r, g, b;					// 颜色像素值
	float rStep, gStep, bStep;		// 每种颜色步长r

	// 获取红、绿、蓝色
	r = (GetRValue(clrEnd) - GetRValue(clrStart));
	g = (GetGValue(clrEnd) - GetGValue(clrStart));
	b =  (GetBValue(clrEnd) - GetBValue(clrStart));

	int nSteps = max(abs(r), max(abs(g), abs(b)));
	float fStep = (float)(85-m_iFlameHeight)/ (float)nSteps;
	// 计算各颜色步长
	rStep = r/(float)nSteps;
	gStep = g/(float)nSteps;
	bStep = b/(float)nSteps;

	// 恢复初始位置颜色
	r = GetRValue(clrStart);
	g = GetGValue(clrStart);
	b = GetBValue(clrStart);

	for (int iOnBand = 0; iOnBand < nSteps; iOnBand++) 
	{
		//COLORREF color = RGB(r+rStep*iOnBand, g + gStep*iOnBand, b + bStep *iOnBand);
		COLORREF color = RGB(b + bStep *iOnBand, g + gStep*iOnBand,r+rStep*iOnBand);

		long lIndex = (int)(iOnBand * fStep);

		m_pPalletteBuffer[lIndex+(85-m_iFlameHeight)] = color;				
	}

}

// ---------------------------------------------------------
//	名称: SetHotSpots
//	功能: 随机设置火源点行中部分点数据为白色
//	参数: 无
//	返回: 无
//	修改: 徐景周，2002.4.8
// ---------------------------------------------------------
void CFireRoutine::SetHotSpots()
{  
	ClrHotSpots();

	long lPosition = 0;
	while(lPosition < m_iWidth)
	{
		// 创建火焰点
		if (getrandom(0,100) < m_iFireChance)
		{
			// 获取火焰宽度
			long lFlameWidth = getrandom(1,m_iAvgFlameWidth);
			for(int i=0;i<lFlameWidth;i++)
			{
				if(lPosition < m_iWidth)
				{
					m_pFireBits[m_pYIndexes[m_iFireSource]+lPosition] =254;// 设置一个火焰点 
					lPosition++;
				}
			}
		}
		lPosition++;
	}
//	for (x = 0; x < m_iWidth; x++)  
//	{  
//		if (getrandom(0,100) < m_iFireChance)
//		{
			
//		}
//	}  
} 

// ---------------------------------------------------------
//	名称: MakeLines
//	功能: 设置从火源所在行向上所有点数据颜色，颜色为它自身像素
//		  所在上、下、左、右的周围像素颜色的平均值
//	参数: 无
//	返回: 无
//	修改: 徐景周，2002.4.8
// ---------------------------------------------------------
void CFireRoutine::MakeLines()  
{  
	int x, y;  

	for (x = 0; x < m_iWidth; x++)  
	{
		 for (y = m_iFireSource; y<m_iHeight-1;y++) 
 //       for (y = m_iHeight; y > m_iFireSource; y--)  
		{
			//m_pFireBits[m_pYIndexes[y-1]+x] =Average(x,y);
			 m_pFireBits[m_pYIndexes[y+1]+x] =Average(x,y);
		}
	}
} 

// ---------------------------------------------------------
//	名称: Average
//	功能: 取上、下、左、右各颜色的平均值
//	参数:  x，y -- 像素所在列号、行号
//	返回: 取得的颜色平均值
//	修改: 徐景周，2002.4.8
// ---------------------------------------------------------
unsigned char CFireRoutine::Average(int x, int y)  
{  

	unsigned char ave_color;  
	unsigned char ave1, ave2, ave3, ave4, ave5, ave6, ave7;  

	// 是否最后一行
	if(y == m_iHeight)
		ave1 = m_pFireBits[m_pYIndexes[y-1] + x];  
	else
		ave1 = m_pFireBits[m_pYIndexes[y + 1] + x];  

	ave2 = m_pFireBits[m_pYIndexes[y - 1] + x];  
	ave3 = m_pFireBits[m_pYIndexes[y] + x + 1];  
	ave4 = m_pFireBits[m_pYIndexes[y] + x - 1];  
	ave5 = m_pFireBits[m_pYIndexes[y] + x + 2];  
	ave6 = m_pFireBits[m_pYIndexes[y] + x - 2];  
	ave7 = m_pFireBits[m_pYIndexes[y] + x];

	ave_color = (ave1 + ave2 + ave3 + ave4 + ave5 + ave6 + ave7) / 7;  

	return(ave_color);  
} 

// ---------------------------------------------------------
//	名称: Render
//	功能: 火焰效果主处理涵数，将处理后的像素点放入目标位图中
//	参数:  x，y -- 像素所在列号、行号
//	返回: 无
//	修改: 徐景周，2002.4.8
// ---------------------------------------------------------
void CFireRoutine::Render(DWORD* pVideoMemory,
		                 int iwidth, 
						 int iheight 						 
						 )
{
	SetHotSpots();   // 产生随机火焰点  
	MakeLines();	 // 进行内部行运算

	// 将作好的火焰放到参数pVideoMemory中
	unsigned char* pSrcBitlin;// = m_pFireBits+(m_iWidth*3);// get rid of our fire source
	BYTE *dst;//=(BYTE*)Dib->pVideoMemory;

	BYTE r;
	BYTE g;
	BYTE b;

	for(int i=0;i<m_iHeight-3;i++)
	{
		dst = (BYTE*)&pVideoMemory[(iwidth*i)];
		pSrcBitlin =&m_pFireBits[m_pYIndexes[i+3]];

		for(int x=0;x<m_iWidth;x++)
		{
			r = GetRValue(m_pPalletteBuffer[pSrcBitlin[x]]);
			g = GetGValue(m_pPalletteBuffer[pSrcBitlin[x]]);
			b = GetBValue(m_pPalletteBuffer[pSrcBitlin[x]]);

			dst[0]=(BYTE)(((r-dst[0])*m_iAlpha+(dst[0]<<8))>>8);
			dst[1]=(BYTE)(((g-dst[1])*m_iAlpha+(dst[1]<<8))>>8);
			dst[2]=(BYTE)(((b-dst[2])*m_iAlpha+(dst[2]<<8))>>8);	
			dst+=4;
		}
	}

}


