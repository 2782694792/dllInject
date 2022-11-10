
// Inject_x64Dlg.cpp : implementation file
//


/**
 * Company : OPT
 * FileName : Inject_x64Dlg.cpp
 * Author :  ZhengShuoBin
 * Version :
 * Date : 2022-9-16
 * Description : //
 * //	1、判断进程是 32 位的还是 64 位的
 * //	2、AddString 使进程列表将所有进程名添加到列表控件当中
 * //	3、SetItemData 设置对应的添加 item 数据为进程 ID ，它的第一个参数为 AddString 的返回值
 * //	4、给注入按钮添加响应代码
 * //		先是取出选择的 DLL 路径：GetWindowText ，
 *	//		调用函数 RemoteLoadLibrary 来注入程序
 * //	5、实现远程注入函数：
 *	//		RemoteLoadLibrary(HANDLE hProcess, LPCSTR lpLibFileName)
 * Functino List :
 * // 1.
 *
 * Other : 快照、创建、打开、注入、句柄等，调用完需进行释放！
 *			  return 返回之前需进行释放处理！
 *			  分配空间需进行释放！（分配还需确定分配成功）
 *
 */

#include "stdafx.h"
#include "Inject_x64.h"
#include "Inject_x64Dlg.h"
#include "afxdialogex.h"
#include <TlHelp32.h>	// CreateToolhelp32Snapshot

#include "Logger.h"
using namespace bin::demo;


#ifdef _DEBUG
#define new DEBUG_NEW
#endif


void RemoteLoadLibrary( HANDLE hProcess, LPCTSTR lpLibFileName );


// CAboutDlg dialog used for App About

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg( );

	// Dialog Data
	enum
	{
		IDD = IDD_ABOUTBOX
	};

protected:
	virtual void DoDataExchange( CDataExchange * pDX );    // DDX/DDV support

	// Implementation
protected:
	DECLARE_MESSAGE_MAP( )
};

CAboutDlg::CAboutDlg( ) : CDialogEx( CAboutDlg::IDD )
{
}

void CAboutDlg::DoDataExchange( CDataExchange * pDX )
{
	CDialogEx::DoDataExchange( pDX );
}

BEGIN_MESSAGE_MAP( CAboutDlg, CDialogEx )
END_MESSAGE_MAP( )


// CInject_x64Dlg dialog



CInject_x64Dlg::CInject_x64Dlg( CWnd * pParent /*=NULL*/ )
	: CDialogEx( CInject_x64Dlg::IDD, pParent )
{
	m_hIcon = AfxGetApp( )->LoadIcon( IDR_MAINFRAME );
}

void CInject_x64Dlg::DoDataExchange( CDataExchange * pDX )
{
	CDialogEx::DoDataExchange( pDX );
	DDX_Control( pDX, IDC_MFCEDITBROWSE_filebrowser, m_DllEditBrowse );
	DDX_Control( pDX, IDC_LIST_display, m_ProcList );
}

BEGIN_MESSAGE_MAP( CInject_x64Dlg, CDialogEx )
	ON_WM_SYSCOMMAND( )
	ON_WM_PAINT( )
	ON_WM_QUERYDRAGICON( )
	ON_BN_CLICKED( ID_btn_refresh, &CInject_x64Dlg::OnBnClickedbtnrefresh )
	ON_BN_CLICKED( ID_btn_inject, &CInject_x64Dlg::OnBnClickedbtninject )
	ON_BN_CLICKED( IDC_btn_backinject, &CInject_x64Dlg::OnBnClickedbtnbackinject )
END_MESSAGE_MAP( )


// CInject_x64Dlg message handlers

BOOL CInject_x64Dlg::OnInitDialog( )
{
	CDialogEx::OnInitDialog( );

	// Add "About..." menu item to system menu.

	// IDM_ABOUTBOX must be in the system command range.
	ASSERT( ( IDM_ABOUTBOX & 0xFFF0 ) == IDM_ABOUTBOX );
	ASSERT( IDM_ABOUTBOX < 0xF000 );

	CMenu * pSysMenu = GetSystemMenu( FALSE );
	if ( pSysMenu != NULL )
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString( IDS_ABOUTBOX );
		ASSERT( bNameValid );
		if ( !strAboutMenu.IsEmpty( ) )
		{
			pSysMenu->AppendMenu( MF_SEPARATOR );
			pSysMenu->AppendMenu( MF_STRING, IDM_ABOUTBOX, strAboutMenu );
		}
	}

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon( m_hIcon, TRUE );			// Set big icon
	SetIcon( m_hIcon, FALSE );		// Set small icon

	// TODO: Add extra initialization here
	
	//__debug( "1233312123123123123123123" );

	// 指定可选择文件类型后缀 *.dll、*.*、All Files(*.*)
	m_DllEditBrowse.EnableFileBrowseButton( NULL
											, _T( "Dll Files (*.dll)|*.dll|All Files (*.*)|*.*||" ) );
	// 启动便刷新
	//CInject_x64Dlg::OnBnClickedbtnrefresh( );


	/*
	// 文件对话框，获取 dll 路径
	CFileDialog fileDialog( TRUE, 0, 0, 6UL, _T( "Dll Files|*.dll|" ) );
	// 弹出对话框且选择了 dll ，进行控件显示
	if ( fileDialog.DoModal( ) == IDOK )
	{
		CString dllPath;
		dllPath = fileDialog.GetPathName( );
		setDlgItemText( IDC_btn_dllpath, dllPath );
	}
	*/

	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CInject_x64Dlg::OnSysCommand( UINT nID, LPARAM lParam )
{
	if ( ( nID & 0xFFF0 ) == IDM_ABOUTBOX )
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal( );
	}
	else
	{
		CDialogEx::OnSysCommand( nID, lParam );
	}
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CInject_x64Dlg::OnPaint( )
{
	if ( IsIconic( ) )
	{
		CPaintDC dc( this ); // device context for painting

		SendMessage( WM_ICONERASEBKGND
					 , reinterpret_cast<WPARAM>( dc.GetSafeHdc( ) ), 0 );

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics( SM_CXICON );
		int cyIcon = GetSystemMetrics( SM_CYICON );
		CRect rect;
		GetClientRect( &rect );
		int x = ( rect.Width( ) - cxIcon + 1 ) / 2;
		int y = ( rect.Height( ) - cyIcon + 1 ) / 2;

		// Draw the icon
		dc.DrawIcon( x, y, m_hIcon );
	}
	else
	{
		CDialogEx::OnPaint( );
	}
}

// The system calls this function to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CInject_x64Dlg::OnQueryDragIcon( )
{
	return static_cast<HCURSOR>( m_hIcon );
}


/*
 * Description：		刷新可注入的应用程序，刷新显示的进程列表
 *
 * Other：进程入口结构体 PROCESSENTRY32W
 *	//		  获取进程名称 Process32First（Next）.szExeFile
 *	//       判断 cpu ：
 *					GetNativeSystemInfo：PROCESSOR_ARCHITECTURE_AMD64  表示 x64
 *	//		  判断进程位数 ：
 *					IsWow64Process : PROCESSOR_ARCHITECTURE_INTEL  表示 x86
 *
 */
void CInject_x64Dlg::OnBnClickedbtnrefresh( )
{
	// TODO: Add your control notification handler code here
	m_ProcList.ResetContent( );


	// 创建进程快照：CreateToolhelp32Snapshot   #include<TlHelp32.h>
	// 返回进程快照句柄，否则返回 -1
	HANDLE hProcessSnap =
		CreateToolhelp32Snapshot( TH32CS_SNAPPROCESS, 0 );
	// 宏定义：-1 转换为无效地址
	if ( INVALID_HANDLE_VALUE == hProcessSnap )
	{
		//AfxMessageBox( TEXT( "进程快照句柄获取失败！" ) );
		OutputDebugString( TEXT( "进程快照句柄获取失败！\n" ) );
		return;
	}
	// 获取进程信息（名称）
	PROCESSENTRY32W procEnt = { 0 }; // 进程入口结构体，用于存放进程信息
	procEnt.dwSize = sizeof( procEnt ); // 设置结构体大小才可获取
	if ( !Process32First( hProcessSnap, &procEnt ) )
	{
		//AfxMessageBox( TEXT( "进程入口结构体初始化失败！" ) );
		OutputDebugString( TEXT( "进程入口结构体初始化失败！\n" ) );
	}
	else
	{
		/// /// 成功获取首个进程记录
		//AfxMessageBox( procEnt.szExeFile );

		// 缓冲区记录进程信息
		TCHAR szProcessStr[ MAX_PATH ];
		TCHAR * szProcBit;

		// 结构体指定存储 cpu 信息
		SYSTEM_INFO sys_info;
		GetNativeSystemInfo( &sys_info );

		// 获取当前进程快照下的其他进程记录信息
		while ( Process32Next( hProcessSnap, &procEnt ) )
		{
			if ( sys_info.wProcessorArchitecture == PROCESSOR_ARCHITECTURE_AMD64 )
			{ // 32 位或 64 位
				// 根据进程 ID 获取进程句柄
				HANDLE hProcess = OpenProcess( PROCESS_QUERY_INFORMATION, false
											   , procEnt.th32ProcessID );
				int ierror = WSAGetLastError( );  // 断点运行监视窗口进行数据监视
				if ( hProcess == NULL )
				{
					//AfxMessageBox( _T( "进程句柄获取失败！" ) );
					OutputDebugString( _T( "该进程权限异常，句柄获取失败！\n" ) );
					continue;
				}
				BOOL isWow64;
				// 判定进程 32 或 64 位
				if ( !IsWow64Process( hProcess, &isWow64 ) )
				{
					//AfxMessageBox( TEXT( "进程 bit 位数获取失败！" ) );
					OutputDebugString( TEXT( "进程 bit 位数获取失败！\n" ) );
				}

				// 释放
				CloseHandle( hProcess );

				if ( isWow64 )
				{ // Window 是 64 位的系统，用来存放 32 位的文件
					szProcBit = TEXT( "32位" );

				}
				else
				{ // 64
					szProcBit = TEXT( "64位" );
				}
			}
			else if
				( sys_info.wProcessorArchitecture == PROCESSOR_ARCHITECTURE_INTEL )
			{ // 32 位进程
				szProcBit = TEXT( "32位" );
			}
			else
			{
				szProcBit = TEXT( "UN" );
			}

			// 格式化进程信息
			wsprintf( szProcessStr, TEXT( "%4d (%s) %s" ), procEnt.th32ProcessID, szProcBit, procEnt.szExeFile );
			// 把所有（格式化）进程信息添加到列表控件
			int iList = m_ProcList.AddString( szProcessStr );
			// 为相应的进程关联进程 ID
			m_ProcList.SetItemData( iList, procEnt.th32ProcessID );

		}
	}
	// 释放处理
	CloseHandle( hProcessSnap );

}


/**
 * Function :		OnBnClickedbtninject
 * Description :	注入清单列表选定应用程序
 *	//	1、根据列表选择进行指定注入
 * //	2、列表绑定进程 PID
 * //	3、根据文件选择 DLL 路径进行 remoteLoadLibrary 注入
 * Calls :			remoteLoadLibrary(HANDLE, LPCTSTR)
 * Called By :		按钮单击
 * Output :			程序注入
 * Others :
 */
void CInject_x64Dlg::OnBnClickedbtninject( )
{
	// TODO: Add your control notification handler code here

	// 1、获取控件选择窗口的 dll 文件路径
	CString strDllPathName = _T( " - 1" );
	m_DllEditBrowse.GetWindowText( strDllPathName );

	// 2、获取列表清单中选择的进程 id ，获取具有权限的进程的句柄
	/// 读、写进进程所在存储空间，同步，存储空间中请求添加线程，请求执行函数
	DWORD curItemData_pid = m_ProcList.GetItemData( m_ProcList.GetCurSel( ) );
	HANDLE cur_pid_process = OpenProcess( PROCESS_CREATE_THREAD
										  | PROCESS_VM_OPERATION | PROCESS_VM_READ | PROCESS_VM_WRITE
										  | SYNCHRONIZE, false, curItemData_pid );
	// 2.1 判断进程是否初始化
	if ( cur_pid_process == NULL )
	{
		OutputDebugStringA( "进程句柄获取失败！\n" );
	}

	// 3、进行程序注入
	//RemoteLoadLibrary( cur_pid_process, (LPCSTR) strDllPathName.GetBuffer( strDllPathName.GetLength( ) ) );
	RemoteLoadLibrary( cur_pid_process, (LPCTSTR) strDllPathName );
}


/*
 * Description:		反注入，注销已注入的应用程序动态库
 */
void CInject_x64Dlg::OnBnClickedbtnbackinject( )
{
	// TODO: Add your control notification handler code here
}


/**
 * Function :			RemoteLoadLibrary
 * Description :		将 dll 注入指定的程序中
 * Calls :				LoadLibrary 进行远程注入
 * Called By :			inject 按钮响应
 * param1 :				hProcess 进程句柄
 * param2 :				lpLibFileName 指定的 dll 文件路径
 * Output :
 * Others :				LPCTSTR 多字节字符指针
 */
void RemoteLoadLibrary( HANDLE hProcess, LPCTSTR lpLibFileName )
{
	/// 4 在远程地址空间中申请空间（避免不可写入）
	PVOID pMen = VirtualAllocEx( hProcess, NULL, 0x1000, MEM_COMMIT
								 , PAGE_READWRITE );
	if ( NULL == pMen )
	{
		OutputDebugStringA( "远程地址空间申请失败！" );
	}
	else
	{
		/*
		SIZE_T pathSize = (_tcslen( dllPath ) + 1) * sizeof(TCHAR);
		 */
		// 注入中文字符串时，以 lstrlenW 计算，需 * 2 ，包括路径末尾存在 /0 ，需
		/// 3 将 dll 地址（字符串）写进远程地址空间中 + 2
		WriteProcessMemory( hProcess, pMen, lpLibFileName, lstrlenW( lpLibFileName ) * 2 + 2
							, NULL );

		/// 2 LoadLibrary 递归获取进程地址 lpThreadStart，加载 	Kernel32.dll 库
		HMODULE hmodule = LoadLibrary( TEXT( "Kemel32.dll" ) );
		int hmodule_error = GetLastError( );
		// loadLibraryW  Unicode 字符集
		LPTHREAD_START_ROUTINE lpThreadStart
			= (LPTHREAD_START_ROUTINE) GetProcAddress( hmodule, "LoadLibraryW" );

		/// 1 创建远程线程
		DWORD dwThreadID = { 0 }; // 存储远程线程 ID
		// lpThreadStart（LoadLibraryW） : 进程地址共用相同，共用系统的库，dll 加载属于公共加载，在公共的存储空间里
		HANDLE createThread = CreateRemoteThread( hProcess, NULL, 0, lpThreadStart, pMen
												  , 0, &dwThreadID );
		if ( createThread == NULL )
		{
			OutputDebugStringA( LPCSTR( "创建线程失败！\n" ) );
		}
		else
		{
			// 判断线程状态，执行完毕进行释放
			WaitForSingleObject( createThread, INFINITE );
			AfxMessageBox( TEXT( "线程注入成功！" ) );
		}

	}
	// 释放处理
	VirtualFreeEx( hProcess, pMen, 0x1000, MEM_RELEASE );

}