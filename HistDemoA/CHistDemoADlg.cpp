// HistDemoADlg.cpp: 实现文件
//

#include "pch.h"
#include "framework.h"
#include "CHistDemoA.h"
#include "CHistDemoADlg.h"
#include "afxdialogex.h"
#include <algorithm>

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// 用于应用程序“关于”菜单项的 CAboutDlg 对话框

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ABOUTBOX };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

// 实现
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(IDD_ABOUTBOX)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// CHistDemoADlg 对话框



CHistDemoADlg::CHistDemoADlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_HISTDEMOA_DIALOG, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	// 对存放原图像和处理结果图像的指针进行初始化
	lpBitmap = 0;
	lpBackup = 0;
}

void CHistDemoADlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CHistDemoADlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_HIST, &CHistDemoADlg::OnHist)
	ON_BN_CLICKED(IDC_OPEN, &CHistDemoADlg::OnOpen)
	ON_BN_CLICKED(IDC_MEAN, &CHistDemoADlg::OnBnClickedMean)
	ON_BN_CLICKED(IDC_MEDIAN, &CHistDemoADlg::OnBnClickedMedian)
END_MESSAGE_MAP()


// CHistDemoADlg 消息处理程序

BOOL CHistDemoADlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 将“关于...”菜单项添加到系统菜单中。

	// IDM_ABOUTBOX 必须在系统命令范围内。
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != nullptr)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// 设置此对话框的图标。  当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	// TODO: 在此添加额外的初始化代码
	// 在对话框窗口的CRect所指定区域绘制两个窗口，分别显示原图像和处理完的图像
	source.Create(0, "Source", WS_CHILD | WS_VISIBLE, CRect(40, 40, 360, 280), this, 10000);
	dest.Create(0, "Destination", WS_CHILD | WS_VISIBLE, CRect(400, 40, 720, 280), this, 10001);

	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

void CHistDemoADlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。  对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CHistDemoADlg::OnPaint()
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
		CDialogEx::OnPaint();
	}
}

//当用户拖动最小化窗口时系统调用此函数取得光标
//显示。
HCURSOR CHistDemoADlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


void CHistDemoADlg::LoadBitmap()
{
	// TODO: 在此处添加实现代码.
	// BITMAPINFOHEADER结构包含有关
	// 与设备无关的位图 （DIB） 的尺寸和颜色格式的信息。
	BITMAPINFOHEADER* pInfo;
	// 得到lpBitmap指针执行的图像文件所在的文件头的起始位置
	pInfo = (BITMAPINFOHEADER *)(lpBitmap + sizeof(BITMAPFILEHEADER));
	// 获得图像的宽度,单位:像素
	nWidth = pInfo->biWidth;
	// 获得图像的位宽度，字节为单位，RGB格式的
	// 是三倍的像素图像宽度
	nByteWidth = nWidth * 3;
	if (nByteWidth % 4) nByteWidth += 4 - (nByteWidth % 4);
	// 获得图像高度
	nHeight = pInfo->biHeight;
	if (pInfo->biBitCount != 24) {
		if (pInfo->biBitCount != 8) {
			// 吐过图像不是位图格式，直接退出程序
			AfxMessageBox("无效位图");
			delete lpBitmap;
			lpBitmap = 0;
			return;
		}
		// 调色板相关操作
		unsigned int PaletteSize = 1 << pInfo->biBitCount;
		if (pInfo->biClrUsed != 0 && pInfo->biClrUsed < PaletteSize)
			PaletteSize = pInfo->biClrUsed;

		// 得到lpBitmap指针所指向的图像文件的真正存储
		// 图像调色板和像素值的位置，即文件头后的第一个字节
		lpBits = lpBitmap + sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER);
		// 得到调色板
		RGBQUAD* pPalette = (RGBQUAD*)lpBits;
		// 得到图像第一个像素所在位置
		lpBits += sizeof(RGBQUAD) * PaletteSize;
		// 去掉调色板之后的图像文件长度，以字节为单位
		nLen = sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER) + nByteWidth * nHeight;
		// 暂存lpBitmap中的图像
		BYTE* lpTemp = lpBitmap;
		// 开辟内存空间
		lpBitmap = new BYTE[nLen];
		// 设置文件头
		BITMAPFILEHEADER bmh;
		BITMAPINFOHEADER bmi;
		bmh.bfType = 'B' + 'M' * 256;
		bmh.bfSize = nLen;
		bmh.bfReserved1 = 0;
		bmh.bfReserved2 = 0;
		bmh.bfOffBits = 54;
		bmi.biSize = sizeof(BITMAPINFOHEADER);
		bmi.biWidth = nWidth;
		bmi.biHeight = nHeight;
		bmi.biPlanes = 1;
		bmi.biBitCount = 24;
		bmi.biCompression = BI_RGB;
		bmi.biSizeImage = 0;
		bmi.biXPelsPerMeter = 0;
		bmi.biYPelsPerMeter = 0;
		bmi.biClrUsed = 0;
		bmi.biClrImportant = 0;
		int nBWidth = pInfo->biWidth;
		if (nBWidth % 4) nBWidth += 4 - (nBWidth % 4);
		memset(lpBitmap, 0, nLen);
		memcpy(lpBitmap, &bmh, sizeof(BITMAPFILEHEADER));
		memcpy(lpBitmap + sizeof(BITMAPFILEHEADER), &bmi, sizeof(BITMAPINFOHEADER));
		// lpBits2指向新建的lpBitmap所指图像的第一个像素的位置
		BYTE* lpBits2 = lpBitmap + sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER);
		// 下面这段代码实在利用调色板生成像素的具体RGB值，
		// 并保存在lpBits2指针所指的内存区域 
		int x, y, p1, p2, Palette;
		for (y = 0; y < nHeight; y++)
		{
			for (x = 0; x < nWidth; x++)
			{
				p1 = y * nBWidth + x;
				p2 = y * nByteWidth + x * 3;
				if (lpBits[p1] < PaletteSize) Palette = lpBits[p1];
				else Palette = 0;
				lpBits2[p2] = pPalette[Palette].rgbBlue;
				lpBits2[p2 + 1] = pPalette[Palette].rgbGreen;
				lpBits2[p2 + 2] = pPalette[Palette].rgbRed;
			}
		}
		delete lpTemp;
	}
lpBits = lpBitmap + sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER);
if (lpBackup) delete lpBackup;
lpBackup = new BYTE[nLen];
// 将刚生成的去掉调色板的lpBitmap所指图像进行备份
memcpy(lpBackup, lpBitmap, nLen);
}

// 点击打开按钮消息响应
void CHistDemoADlg::OnOpen()
{
	// TODO: 在此添加控件通知处理程序代码
	CFile File;
	// 创建一个打开文件的对话框，让用户选择文件打开
	CFileDialog dlg(TRUE, 0, 0, OFN_HIDEREADONLY, "位图文件|*.bmp|所有文件|*.*||", this);
	if (dlg.DoModal() == IDOK)
	{
		FileName = dlg.GetPathName();
		if (!File.Open(FileName, CFile::modeRead)) return;
		// TODO: add loading code here
		// 加载所选的图像文件
		// 清空上次图片信息
		if (lpBitmap) delete lpBitmap;
		// 获取文件的长度(以字节为单位)
		nLen = File.GetLength();
		// 开辟内存空间存放图像文件,然后读入文件存储
		lpBitmap = new BYTE[nLen];
		File.Read(lpBitmap, nLen);
		// 利用刚才定义的LoadBitmap()函数，去掉调色板,以备后面进行图像的处理
		LoadBitmap();
		// 在左边窗格中显示原图像
		if (lpBitmap) source.SetImage(nWidth, nHeight, lpBits);
	}
}

// 直方图均衡化按钮点击消息响应函数
void CHistDemoADlg::OnHist()
{
	// TODO: 在此添加控件通知处理程序代码
	HistogramEq();

}

// 定义宏，使得能够像访问二维数组一样访问一维数组存储的图像信息
#define Point(x,y) lpPoints[(x)+(y)*nWidth]
#define Point1(x,y) lpPoints1[(x)+(y)*nWidth]
void GetPoints(int nWidth, int nHeight, BYTE* lpBits, BYTE* lpPoints)
{
	int x, y, p;
	int nByteWidth = nWidth * 3;
	if (nByteWidth % 4) nByteWidth += 4 - (nByteWidth % 4);
	for (y = 0; y < nHeight; y++)
	{
		for (x = 0; x < nWidth; x++)
		{
			p = x * 3 + y * nByteWidth;
			lpPoints[x + y * nWidth] = (BYTE)(0.299 * (float)lpBits[p + 2] + 0.587 * (float)lpBits[p + 1] + 0.114 * (float)lpBits[p] + 0.1);
		}
	}
}
void PutPoints(int nWidth, int nHeight, BYTE* lpBits, BYTE* lpPoints)
{
	int nByteWidth = nWidth * 3;
	if (nByteWidth % 4) nByteWidth += 4 - (nByteWidth % 4);
	int x, y, p, p1;
	for (y = 0; y < nHeight; y++)
	{
		for (x = 0; x < nWidth; x++)
		{
			p = x * 3 + y * nByteWidth;
			p1 = x + y * nWidth;
			lpBits[p] = lpPoints[p1];
			lpBits[p + 1] = lpPoints[p1];
			lpBits[p + 2] = lpPoints[p1];
		}
	}
}

void CHistDemoADlg::HistogramEq()
{
	// TODO: 在此处添加实现代码.
	if (lpBitmap == 0) return;
	BYTE* lpOutput = new BYTE[nByteWidth * nHeight];
	// 调用下面的函数，对lpBits所指区域内的图像矩阵进行直方图均衡化，
	// 存到lpOutput指针所指内存区域进行显示
	HistogramEq1(nWidth, nHeight, lpBits, lpOutput);
	SetDlgItemText(IDC_STATIC1, "经过直方图均衡化处理后的图像");
	// 在右侧目标窗格显示处理后的图像
	dest.SetImage(nWidth, nHeight, lpOutput);
	delete lpOutput;
	// 为方便与处理后的结果进行对比，将原图像的彩色去掉
	NoColor();
}



void CHistDemoADlg::HistogramEq1(int nWidth, int nHeight, BYTE* lpInput, BYTE* lpOutput)
{
	// TODO: 在此处添加实现代码.
	int x, y;
	BYTE* lpPoints = new BYTE[nWidth * nHeight];
	GetPoints(nWidth, nHeight, lpInput, lpPoints);
	int r[256], s[256];
	ZeroMemory(r, 1024);
	ZeroMemory(s, 1024);
	// 获得原图像的统计直方图
	for (y = 0; y < nHeight; y++) {
		for (x = 0; x < nWidth; x++) {
			r[Point(x, y)]++;
		}
	}
	// 计算累积直方图
	s[0] = r[0];
	for (y = 1; y < 256; y++)
	{
		s[y] = s[y - 1];
		s[y] += r[y];
	}

	// 进行直方图均衡化
	for (y = 0; y < nHeight; y++) {
		for (x = 0; x < nWidth; x++) {
			Point(x, y) = s[Point(x, y)] * 255 / nWidth / nHeight;
		}
	}
	PutPoints(nWidth, nHeight, lpOutput, lpPoints);
	delete lpPoints;
}


void CHistDemoADlg::NoColor()
{
	// TODO: 在此处添加实现代码.
	if (lpBitmap == 0) return;
	int x, y, p;
	BYTE Point;
	for (y = 0; y < nHeight; y++)
	{
		for (x = 0; x < nWidth; x++)
		{
			p = x * 3 + y * nByteWidth;
			Point = (BYTE)(0.299 * (float)lpBits[p + 2] + 0.587 * (float)lpBits[p + 1] + 0.114 * (float)lpBits[p] + 0.1);
			lpBits[p + 2] = Point;
			lpBits[p + 1] = Point;
			lpBits[p] = Point;
		}
	}
	source.SetImage(nWidth, nHeight, lpBits);
}


// 均值滤波按钮点击事件消息
void CHistDemoADlg::OnBnClickedMean()
{
	// TODO: 在此添加控件通知处理程序代码
	MeanFilter();
	
}


void CHistDemoADlg::MeanFilter()
{
	// TODO: 在此处添加实现代码.
	if (lpBitmap == 0) return;
	BYTE* lpOutput = new BYTE[nByteWidth * nHeight];
	// 调用下面的函数，对lpBits所指区域内的图像矩阵进行均值滤波，
	// 存到lpOutput指针所指内存区域进行显示
	MeanFilter1(nWidth, nHeight, lpBits, lpOutput);
	SetDlgItemText(IDC_STATIC1, "经过均值滤波处理后的图像");
	// 在右侧目标窗格显示处理后的图像
	dest.SetImage(nWidth, nHeight, lpOutput);
	delete lpOutput;
	// 为方便与处理后的结果进行对比，将原图像的彩色去掉
	NoColor();

}


void CHistDemoADlg::MeanFilter1(int nWidth, int nHeight,BYTE* lpInput, BYTE* lpOutput)
{
	// TODO: 在此处添加实现代码.
	int x, y;
	BYTE* lpPoints = new BYTE[nWidth * nHeight];
	GetPoints(nWidth, nHeight, lpInput, lpPoints);
	int r[256], s[256];
	ZeroMemory(r, 1024);
	ZeroMemory(s, 1024);
	// 均值滤波过程，和周围点取均值之后赋给中心点
	for (y = 1; y < nHeight - 1; y++) {
		for (x = 1; x < nWidth - 1; x++) {
			Point(x, y) = (Point(x - 1, y - 1) + Point(x - 1, y) + Point(x - 1, y + 1) + Point(x, y - 1) 
				+ Point(x, y) + Point(x, y + 1) + Point(x + 1, y - 1) + Point(x + 1, y) + Point(x + 1, y + 1)) / 9;
		}
	}
	PutPoints(nWidth, nHeight, lpOutput, lpPoints);
	delete lpPoints;
}


// 中值滤波按钮被点击消息
void CHistDemoADlg::OnBnClickedMedian()
{
	// TODO: 在此添加控件通知处理程序代码
	MedianFilter();
}


void CHistDemoADlg::MedianFilter()
{
	// TODO: 在此处添加实现代码.
	if (lpBitmap == 0) return;
	BYTE* lpOutput = new BYTE[nByteWidth * nHeight];
	// 调用下面的函数，对lpBits所指区域内的图像矩阵进行中值滤波，
	// 存到lpOutput指针所指内存区域进行显示
	MedianFilter1(nWidth, nHeight, lpBits, lpOutput);
	SetDlgItemText(IDC_STATIC1, "经过中值滤波处理后的图像");
	// 在右侧目标窗格显示处理后的图像
	dest.SetImage(nWidth, nHeight, lpOutput);
	delete lpOutput;
	// 为方便与处理后的结果进行对比，将原图像的彩色去掉
	NoColor();
}


void CHistDemoADlg::MedianFilter1(int nWidth, int nHeight, BYTE* lpInput, BYTE* lpOutput)
{
	// TODO: 在此处添加实现代码.
	BYTE* lpPoints = new BYTE[nWidth * nHeight];
	GetPoints(nWidth, nHeight, lpInput, lpPoints);
	int r[256], s[256];
	ZeroMemory(r, 1024);
	ZeroMemory(s, 1024);

	// 中值滤波过程
	for (int x = 1; x < nWidth-1; ++x) {
		for (int y = 1; y < nHeight-1; ++y) {
			int flag[3] = { -1, 0, 1 };
			int points[9], t = 0;
			// 在图像上取周围的点给数组赋值
			for (int i = 0; i < 3; ++i) {
				for (int j = 0; j < 3; ++j) {
					points[t++] = Point(x + flag[i], y + flag[j]);
				}
			}
			Point(x,y) = Median(points);
		}
	}

	PutPoints(nWidth, nHeight, lpOutput, lpPoints);
	delete lpPoints;

}


// 求序列的中值
int CHistDemoADlg::Median(int* points)
{
	// TODO: 在此处添加实现代码.
	// 快速排序
	quick_sort(points, 0, 8);
	// 返回中值
	return points[4];
}


// 对数组进行快速排序
void CHistDemoADlg::quick_sort(int* q, int l, int r)
{
	// TODO: 在此处添加实现代码.
	if (l >= r) return;

	int i = l - 1, j = r + 1, x = q[l + r >> 1];
	while (i < j) {
		do i++; while (q[i] < x);
		do j--; while (q[j] > x);
		if (i < j)std::swap(q[i], q[j]);
	}
	quick_sort(q, l, j), quick_sort(q, j + 1, r);
}


