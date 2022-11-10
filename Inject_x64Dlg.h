
// Inject_x64Dlg.h : header file
//

#pragma once
#include "afxeditbrowsectrl.h"
#include "afxwin.h"


// CInject_x64Dlg dialog
class CInject_x64Dlg : public CDialogEx
{
// Construction
public:
	CInject_x64Dlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	enum { IDD = IDD_INJECT_X64_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support


// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedbtnrefresh();
	afx_msg void OnBnClickedbtninject();
	afx_msg void OnBnClickedbtnbackinject();
	// 控件变量：选定注入程序动态库
	CMFCEditBrowseCtrl m_DllEditBrowse; 
	// 控件变量：可选择程序列表清单
	CListBox m_ProcList;
};
