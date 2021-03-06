/////////////////////////////////////////////////////////////////////////
//类名：CAsciiAnimation
//功能：利用ASCII码动画，实现烟花等效果
//修改：徐景周(Johnny Xu, xujingzhou2016@gmail.com)
//组织：未来工作室(Future Studio)
//日期：2002.1.8
////////////////////////////////////////////////////////////////////////
#if !defined(AFX_ASCIIANIMATION_H__9F4DE293_CC79_4525_A551_55C88A57AD6D__INCLUDED_)
#define AFX_ASCIIANIMATION_H__9F4DE293_CC79_4525_A551_55C88A57AD6D__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <string>
#include <math.h>

typedef struct tagMathStruct
{
	float SIN[360];
	float COS[360];

	// 预先初始化所用正弦、余弦值(速度优化)
	void InitSINCOS()
	{
		for (int ang=0; ang<360; ++ang)
		{
			float theta = (float) (ang * ((float)3.14159) / 180);
			SIN[ang] = (float) sin(theta);
			COS[ang] = (float) cos(theta);
		}
	};
}MathStruct;

typedef struct tagFIREWORKS
{
	int x, y, init_x, init_y;
	int radius;
	int max_height, max_radius;
	short nAsciiCode, initAsciiCode;

	// 初始化烟花效果数据
	void Init(int newx, int newy, short newAscii, int nMaxH, int nMaxR)
	{
		x = init_x = newx;
		y = init_y = newy;
		radius = 1;
		max_height = nMaxH;
		max_radius = nMaxR;
		nAsciiCode = initAsciiCode = newAscii;
	};

	// 重新计算烟花效果数据
	void Calculate()
	{
		if (y == max_height)
		{
			if (radius < max_radius)
			{
				radius++;
				nAsciiCode = '*';
			}
			else
			{
				radius = 1;
				y = init_y;
				nAsciiCode = initAsciiCode;
			}
		}
		else
			y--;
	};
}FIREWORKS;

class CAsciiAnimation  
{
public:
	CAsciiAnimation();
	virtual ~CAsciiAnimation();

	// 初始创建，传入显示范围(注：要预先调用)
	void Create(int nWidth, int nHeight);

	// 设置背景、前景色
	void SetBackColor(int R, int G, int B);
	void SetForeColor(int R, int G, int B);

	// 清空显示范围
	void ClearScreen(CDC* pDC);
	// 指定位置绘制字符
	void DrawCharacter(CDC* pDC,short nAsciiCode, int x, int y);
	// 指定位置绘制字符串
	void CharBlit(CDC* pDC, const std::string& strChar, int Width, int Height, int DestX, int DestY, short nAsciiMask);
	
	// 环形、弧形、矩形效果显示字符
	void Circle(CDC* pDC,int x, int y, int radius, int step, short nAsciiCode, BOOL bFilled);
	void Arc(CDC* pDC,int x, int y, int radius, int degfrom, int degto, int step, short nAsciiCode);
	void Rectangle(CDC* pDC,int left, int top, int right, int bottom, short nAsciiCode, BOOL bFilled);

private:
	COLORREF	crBackColor;		// 背景色
	COLORREF	crForeColor;		// 前景色
	HFONT		hfont;				// 显示字体

	MathStruct	math;				// 角度值

	int			m_nWidth;			// 范围宽度
	int			m_nHeight;			// 范围高度
};

#endif // !defined(AFX_ASCIIANIMATION_H__9F4DE293_CC79_4525_A551_55C88A57AD6D__INCLUDED_)
