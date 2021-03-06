/////////////////////////////////////////////////////////////////////////
//类名：CFireRoutine
//功能：火焰效果
//附注：经测试，火焰效果最耗费CPU资源(其它还可以)，请慎用
//修改：徐景周(Johnny Xu, xujingzhou2016@gmail.com)
//组织：未来工作室(Future Studio)
//日期：2002.1.8
////////////////////////////////////////////////////////////////////////
#if !defined(AFX_CFIREROUTINE_H__413FF819_B711_4B5A_8EC1_580A70862BE4__INCLUDED_)
#define AFX_CFIREROUTINE_H__413FF819_B711_4B5A_8EC1_580A70862BE4__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#include <Windows.h>
#include <vector>							// 加入STL中vector,jingzhou xu

class CFireRoutine  
{
public:
	CFireRoutine();
	virtual ~CFireRoutine();

	void		InitFire();					// 初始化火焰数据
	void		ClrHotSpots();				// 清空火焰点数据
	void		InitPallette();				// 初始化调色板颜色数据
	void		SetHotSpots();				// 设置火焰点数据
	void		MakeLines();				// 设置每行火焰数据
	void		Render(DWORD* pVideoMemory,	// 处理火焰数据并放入目标位图中
		                 int iwidth, 
						 int iheight);


	unsigned char	Average(int x, int y);	// 求颜色平均值

	int			m_iAlpha;					// 透明度
	int			m_iWidth;					// 目标位图宽度
	int			m_iHeight;					// 目标位图高度

private:
	int			m_iFlameHeight;				// 火焰高度
	int			m_iFireSource;				// 火焰点所在行位置
	int			m_iFireChance;				// 火焰点发生机会
	int			m_iAvgFlameWidth;			// 火焰宽度

	COLORREF	m_FireColors[4];			// 火焰基本色

	BYTE*		m_pFireBits;				// 火焰数据
	DWORD		m_pPalletteBuffer[256];		// 颜色数据
	// 为防止原m_pYIndexes指针引起的内存泄漏，现改为vector动态数组,jingzhou xu.
	std::vector<LONG>	m_pYIndexes;

};

#endif // !defined(AFX_CFIREROUTINE_H__413FF819_B711_4B5A_8EC1_580A70862BE4__INCLUDED_)
