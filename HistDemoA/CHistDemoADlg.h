
// HistDemoADlg.h: 头文件
//

#pragma once
#include "CImageWnd.h"

// CHistDemoADlg 对话框
class CHistDemoADlg : public CDialogEx
{
// 构造
public:
	CHistDemoADlg(CWnd* pParent = nullptr);	// 标准构造函数

	int nWidth;
	int nHeight;
	int nLen;
	int nByteWidth;
	BYTE* lpBackup;
	BYTE* lpBitmap;
	BYTE* lpBits;
	CString FileName;
	CImageWnd source, dest;

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_HISTDEMOA_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持


// 实现
protected:
	HICON m_hIcon;

	// 生成的消息映射函数
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedButton1();
	void LoadBitmap();
	afx_msg void OnHist();
	afx_msg void OnOpen();
	void HistogramEq();
	void HistogramEq1(int nWidth, int nHeight, BYTE* lpInput, BYTE* lpOutput);
	void NoColor();
	// 均值滤波按钮点击消息
	afx_msg void OnBnClickedMean();
	// 调用均值滤波函数对图像进行处理并输出到右侧窗格
	void MeanFilter();
	// 进行中值滤波的函数
	void MeanFilter1(int nWidth, int nHeight, BYTE* lpInput, BYTE* lpOutput);
	void OnBnClickedMedian();
	// 调用均值滤波并把图像输出到右窗格
	void MedianFilter();
	// 进行均值滤波的函数
	void MedianFilter1(int nWidth, int nHeight, BYTE* lpInput, BYTE* lpOutput);
	// 求序列的中值
	int Median(int* points);
	// 对数组进行快速排序
	void quick_sort(int* q, int l, int r);
};
