/////////////////////////////////////////////////////////////////////////
//类名：CAsciiAnimation
//功能：利用ASCII码动画，实现烟花等效果
//修改：徐景周(Johnny Xu, xujingzhou2016@gmail.com)
//组织：未来工作室(Future Studio)
//日期：2002.1.8
////////////////////////////////////////////////////////////////////////
#include "stdafx.h"
#include "AsciiAnimation.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
// ---------------------------------------------------------
//	名称: CAsciiAnimation
//	功能: 初始化工作
//	参数: 无
//	返回: 无
//	编写: 徐景周(Johnny Xu, xujingzhou2016@gmail.com)，2002.4.8
// ---------------------------------------------------------
CAsciiAnimation::CAsciiAnimation()
{

	crBackColor = RGB(0,0,0);
	crForeColor = RGB(255,255,255);

	m_nWidth = 0;
	m_nHeight = 0;

	// 初始化正弦、余弦角度值数组
	math.InitSINCOS();

}

// ---------------------------------------------------------
//	名称: ~CAsciiAnimation
//	功能: 退出时清除工作
//	参数: 无
//	返回: 无
//	编写: 徐景周(Johnny Xu, xujingzhou2016@gmail.com)，2002.4.8
// ---------------------------------------------------------
CAsciiAnimation::~CAsciiAnimation()
{

}

// ---------------------------------------------------------
//	名称: SetBackColor
//	功能: 设置背景色
//	参数: R、G、B -- 分别为整数红、绿和蓝三色
//	返回: 无
//	编写: 徐景周(Johnny Xu, xujingzhou2016@gmail.com)，2002.4.8
// ---------------------------------------------------------
void CAsciiAnimation::SetBackColor(int R, int G, int B)
{
	// 背景色
	crBackColor = RGB(R,G,B);

}

// ---------------------------------------------------------
//	名称: SetForeColor
//	功能: 设置前景色
//	参数: R、G、B -- 分别为整数红、绿和蓝三色
//	返回: 无
//	编写: 徐景周(Johnny Xu, xujingzhou2016@gmail.com)，2002.4.8
// ---------------------------------------------------------
void CAsciiAnimation::SetForeColor(int R, int G, int B)
{
	// 前景色
	crForeColor = RGB(R,G,B);

}

// ---------------------------------------------------------
//	名称: Create
//	功能: 初始化创建
//	参数: nWidth -- 显示范围宽度，nHeight -- 显示范围高度
//	返回: 无
//	编写: 徐景周(Johnny Xu, xujingzhou2016@gmail.com)，2002.4.8
// ---------------------------------------------------------
void CAsciiAnimation::Create(int nWidth, int nHeight)
{
	m_nWidth = nWidth;
	m_nHeight = nHeight;
}

// ---------------------------------------------------------
//	名称: ClearScreen
//	功能: 清除显示范围内字符为空
//	参数: pDC -- 相关显示DC
//	返回: 无
//	编写: 徐景周(Johnny Xu, xujingzhou2016@gmail.com)，2002.4.8
// ---------------------------------------------------------
void CAsciiAnimation::ClearScreen(CDC* pDC)
{
	int oldMode;
	COLORREF oldBackColor;
	COLORREF oldForeColor;

	TEXTMETRIC tm;
	pDC->GetTextMetrics(&tm);

	int width, height;
	// 一行显示字符最大数
	width	= m_nWidth / tm.tmMaxCharWidth;
	height	= m_nHeight / tm.tmHeight;

	oldMode = pDC->SetBkMode(TRANSPARENT);
	oldBackColor = pDC->SetBkColor(crBackColor);
	oldForeColor = pDC->SetTextColor(crBackColor);

	// 清扫整个范围字符
	for (int y=0; y<height; ++y)
	{
		for (int x=0; x<width; ++x)
			DrawCharacter(pDC,' ', x, y);
	}

	// 恢复原设置
	pDC->SetTextColor(oldForeColor);
	pDC->SetBkColor(oldBackColor);
	pDC->SetBkMode(oldMode);
}

// ---------------------------------------------------------
//	名称: DrawCharacter
//	功能: 指定位置绘制单个字符
//	参数: pDC -- 相关显示DC, nAsciiCode -- 字符码, 
//		  x、y -- 左上角坐标
//	返回: 无
//	编写: 徐景周(Johnny Xu, xujingzhou2016@gmail.com)，2002.4.8
// ---------------------------------------------------------
void CAsciiAnimation::DrawCharacter(CDC* pDC,short nAsciiCode, int x, int y)
{
	int oldMode;
	COLORREF oldBackColor;
	COLORREF oldForeColor;
	HFONT oldfont;

	LOGFONT lf;
	memset(&lf, 0, sizeof(LOGFONT));

	lf.lfHeight = -MulDiv(10, pDC->GetDeviceCaps(LOGPIXELSY), 72);
	strcpy(lf.lfFaceName, "Courier");
	lf.lfWeight = FW_NORMAL;
	if(hfont)
	{
		hfont = CreateFontIndirect(&lf);
		oldfont = (HFONT)pDC->SelectObject(hfont);
	}

	TEXTMETRIC tm;
	pDC->GetTextMetrics(&tm);

	char chr[2];
	int width, height;

	// 一行显示字符最大数
	width	= m_nWidth / tm.tmMaxCharWidth;
	height	= m_nHeight / tm.tmHeight;

	// 边界处理
	if (x >= m_nWidth || x < 0 || y >= m_nHeight || y < 0)
		return;


	x += tm.tmMaxCharWidth;
	y -= tm.tmHeight;

	chr[0] = (char)nAsciiCode;
	chr[1] = '\0';

	oldMode = pDC->SetBkMode(TRANSPARENT);
	oldBackColor = pDC->SetBkColor(crBackColor);
	oldForeColor = pDC->SetTextColor(crForeColor);

	// 显示字符
	pDC->TextOut(x, y, chr, 1);

	// 恢复原设置
	pDC->SetTextColor(oldForeColor);
	pDC->SetBkColor(oldBackColor);
	pDC->SetBkMode(oldMode);

	pDC->SelectObject(oldfont);
	DeleteObject(hfont);
	hfont = NULL;
}

// ---------------------------------------------------------
//	名称: CharBlit
//	功能: 指定位置绘制字符串(指定屏蔽码nAsciiMask不绘制)
//	参数: pDC -- 相关显示DC, strChar -- 字符串, 
//		  Width、Height -- 绘制范围宽度、高度，
//		  DestX,DestY -- 绘制左上角坐标，
//		  nAsciiMask -- 屏蔽码
//	返回: 无
//	编写: 徐景周(Johnny Xu, xujingzhou2016@gmail.com)，2002.4.8
// ---------------------------------------------------------
void CAsciiAnimation::CharBlit(CDC* pDC, const std::string& strChar, int Width, int Height, int DestX, int DestY, short nAsciiMask)
{
	// 在指定坐标(DestX, DestY) 上绘制字符串(屏蔽码nAsciiMask不绘制)
	const char* chrArray = strChar.c_str();
	int length = strlen(chrArray);

	// 高度
	for (int h=0; h<Height; h++)
	{
		// 宽度
		for (int w=0; w<Width; w++)
		{
			char chrToDraw;
			if ((w + (h*Width)) > length)
				continue;								// 越界处理
			else
				chrToDraw = chrArray[w + (h*Width)];	// 要绘制的字符

			// 跳过屏蔽码，绘制字符
			if (chrToDraw != nAsciiMask)
				DrawCharacter(pDC, chrToDraw, DestX + w, DestY + h);
		}
	}
}

// ---------------------------------------------------------
//	名称: Circle
//	功能: 环形效果显示字符，利用矩形风格指定环形坐标
//	参数: pDC -- 相关显示DC, x、y -- 左上角坐标, step -- 步长
//		  nAsciiCode -- 字符码, bFilled -- 是否填充
//	返回: 无
//	编写: 徐景周(Johnny Xu, xujingzhou2016@gmail.com)，2002.4.8
// ---------------------------------------------------------
void CAsciiAnimation::Circle(CDC* pDC,int x, int y, int radius, int step, short nAsciiCode, BOOL bFilled)
{
	int diameter = radius*2+1;
	int maxgrid = diameter*diameter;
	int* grid = new int[maxgrid];
	int i, j;
	
	// 为绘制环形设置网格
	memset(grid, 0, sizeof(int)*maxgrid);

	// 网格数组值为1，表示该坐标绘制
	for (int theta = 0; theta < 360; theta=theta+step)
	{
		int xx, yy, gridpos;
		xx = (int)(radius*math.COS[theta]);
		yy = (int)(radius*math.SIN[theta]);
		
		gridpos = (radius + xx) + ((radius + yy) * diameter);

		if (theta != 0 && theta != 90 && theta != 180 && theta != 270)
			grid[gridpos] = 1;
	}

	// 填充环
	if (TRUE == bFilled)
	{
		for (i=0; i<diameter; ++i)
		{
			int start = -1, end = -1;
			for (j=0; j<diameter; ++j)
			{
				if (-1 == start && 1 == grid[j+(i*diameter)])
					start = j+(i*diameter);			// 起始字符
				else if (start > -1 && 1 == grid[j+(i*diameter)])
					end = j+(i*diameter);			// 终止字符
			}

			if (start > -1 && end > -1)
			{
				// 添充起始到终止线段
				for (int l=start; l<end; ++l)
					grid[l] = 1;
			}			
		}
	}

	// 在网格中值为1处绘制字符
	for (i=0; i<diameter; ++i)
	{
		for (j=0; j<diameter; ++j)
		{
			if (1 == grid[j+(i*diameter)])
				DrawCharacter(pDC,nAsciiCode, x - radius + j, y - radius + i);
		}
	}

	delete [] grid;
}

// ---------------------------------------------------------
//	名称: Arc
//	功能: 弧形效果显示字符，利用矩形风格指定环形坐标
//	参数: pDC -- 相关显示DC, x、y -- 左上角坐标, step -- 步长
//		  nAsciiCode -- 字符码, bFilled -- 是否填充
//	返回: 无
//	编写: 徐景周(Johnny Xu, xujingzhou2016@gmail.com)，2002.4.8
// ---------------------------------------------------------
void CAsciiAnimation::Arc(CDC* pDC,int x, int y, int radius, int degfrom, int degto, int step, short nAsciiCode)
{
	// 绘制弧形效果字符
	int diameter = radius*2+1;
	int maxgrid = diameter*diameter;
	int* grid = new int[maxgrid];
	int i, j;
	
	memset(grid, 0, sizeof(int)*maxgrid);

	// 将要绘制部分所在风格数组内容填充为1
	for (int theta = degfrom; theta < degto; theta=theta+step)
	{
		int xx, yy, gridpos;
		xx = (int)(radius*math.COS[theta]);
		yy = (int)(radius*math.SIN[theta]);
		
		gridpos = (radius + xx) + ((radius + yy) * diameter);

		if (0 != theta && 90 != theta && 180 != theta && 270 != theta)
			grid[gridpos] = 1;
	}

	for (i=0; i<diameter; ++i)
	{
		for (j=0; j<diameter; ++j)
		{
			if (1 == grid[j+(i*diameter)])
				DrawCharacter(pDC,nAsciiCode, x - radius + j, y - radius + i);
		}
	}

	delete [] grid;

}

// ---------------------------------------------------------
//	名称: Rectangle
//	功能: 弧形效果显示字符，利用矩形风格指定环形坐标
//	参数: pDC -- 相关显示DC, left、top -- 左上角坐标, right、bottom -- 右下角坐标
//		  nAsciiCode -- 字符码, bFilled -- 是否填充
//	返回: 无
//	编写: 徐景周(Johnny Xu, xujingzhou2016@gmail.com)，2002.4.8
// ---------------------------------------------------------
void CAsciiAnimation::Rectangle(CDC* pDC,int left, int top, int right, int bottom, short nAsciiCode, BOOL bFilled)
{
	// 绘制矩形效果字符
	for (int i=top; i<bottom; ++i)
	{
		for (int j=left; j<right; ++j)
		{
			// 填充
			if (TRUE == bFilled)
				DrawCharacter(pDC,nAsciiCode, j, i);
			else
			{
				if (j == left || j == right-1 || i == top || i == bottom-1)
					DrawCharacter(pDC,nAsciiCode, j, i);
			}
		}
	}

}