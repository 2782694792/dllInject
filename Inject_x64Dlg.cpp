
// Inject_x64Dlg.cpp : implementation file
//


/**
 * Company : OPT
 * FileName : Inject_x64Dlg.cpp
 * Author :  ZhengShuoBin
 * Version :
 * Date : 2022-9-16
 * Description : //
 * //	1���жϽ����� 32 λ�Ļ��� 64 λ��
 * //	2��AddString ʹ�����б����н�������ӵ��б�ؼ�����
 * //	3��SetItemData ���ö�Ӧ����� item ����Ϊ���� ID �����ĵ�һ������Ϊ AddString �ķ���ֵ
 * //	4����ע�밴ť�����Ӧ����
 * //		����ȡ��ѡ��� DLL ·����GetWindowText ��
 *	//		���ú��� RemoteLoadLibrary ��ע�����
 * //	5��ʵ��Զ��ע�뺯����
 *	//		RemoteLoadLibrary(HANDLE hProcess, LPCSTR lpLibFileName)
 * Functino List :
 * // 1.
 *
 * Other : ���ա��������򿪡�ע�롢����ȣ�������������ͷţ�
 *			  return ����֮ǰ������ͷŴ���
 *			  ����ռ�������ͷţ������仹��ȷ������ɹ���
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

	// ָ����ѡ���ļ����ͺ�׺ *.dll��*.*��All Files(*.*)
	m_DllEditBrowse.EnableFileBrowseButton( NULL
											, _T( "Dll Files (*.dll)|*.dll|All Files (*.*)|*.*||" ) );
	// ������ˢ��
	//CInject_x64Dlg::OnBnClickedbtnrefresh( );


	/*
	// �ļ��Ի��򣬻�ȡ dll ·��
	CFileDialog fileDialog( TRUE, 0, 0, 6UL, _T( "Dll Files|*.dll|" ) );
	// �����Ի�����ѡ���� dll �����пؼ���ʾ
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
 * Description��		ˢ�¿�ע���Ӧ�ó���ˢ����ʾ�Ľ����б�
 *
 * Other��������ڽṹ�� PROCESSENTRY32W
 *	//		  ��ȡ�������� Process32First��Next��.szExeFile
 *	//       �ж� cpu ��
 *					GetNativeSystemInfo��PROCESSOR_ARCHITECTURE_AMD64  ��ʾ x64
 *	//		  �жϽ���λ�� ��
 *					IsWow64Process : PROCESSOR_ARCHITECTURE_INTEL  ��ʾ x86
 *
 */
void CInject_x64Dlg::OnBnClickedbtnrefresh( )
{
	// TODO: Add your control notification handler code here
	m_ProcList.ResetContent( );


	// �������̿��գ�CreateToolhelp32Snapshot   #include<TlHelp32.h>
	// ���ؽ��̿��վ�������򷵻� -1
	HANDLE hProcessSnap =
		CreateToolhelp32Snapshot( TH32CS_SNAPPROCESS, 0 );
	// �궨�壺-1 ת��Ϊ��Ч��ַ
	if ( INVALID_HANDLE_VALUE == hProcessSnap )
	{
		//AfxMessageBox( TEXT( "���̿��վ����ȡʧ�ܣ�" ) );
		OutputDebugString( TEXT( "���̿��վ����ȡʧ�ܣ�\n" ) );
		return;
	}
	// ��ȡ������Ϣ�����ƣ�
	PROCESSENTRY32W procEnt = { 0 }; // ������ڽṹ�壬���ڴ�Ž�����Ϣ
	procEnt.dwSize = sizeof( procEnt ); // ���ýṹ���С�ſɻ�ȡ
	if ( !Process32First( hProcessSnap, &procEnt ) )
	{
		//AfxMessageBox( TEXT( "������ڽṹ���ʼ��ʧ�ܣ�" ) );
		OutputDebugString( TEXT( "������ڽṹ���ʼ��ʧ�ܣ�\n" ) );
	}
	else
	{
		/// /// �ɹ���ȡ�׸����̼�¼
		//AfxMessageBox( procEnt.szExeFile );

		// ��������¼������Ϣ
		TCHAR szProcessStr[ MAX_PATH ];
		TCHAR * szProcBit;

		// �ṹ��ָ���洢 cpu ��Ϣ
		SYSTEM_INFO sys_info;
		GetNativeSystemInfo( &sys_info );

		// ��ȡ��ǰ���̿����µ��������̼�¼��Ϣ
		while ( Process32Next( hProcessSnap, &procEnt ) )
		{
			if ( sys_info.wProcessorArchitecture == PROCESSOR_ARCHITECTURE_AMD64 )
			{ // 32 λ�� 64 λ
				// ���ݽ��� ID ��ȡ���̾��
				HANDLE hProcess = OpenProcess( PROCESS_QUERY_INFORMATION, false
											   , procEnt.th32ProcessID );
				int ierror = WSAGetLastError( );  // �ϵ����м��Ӵ��ڽ������ݼ���
				if ( hProcess == NULL )
				{
					//AfxMessageBox( _T( "���̾����ȡʧ�ܣ�" ) );
					OutputDebugString( _T( "�ý���Ȩ���쳣�������ȡʧ�ܣ�\n" ) );
					continue;
				}
				BOOL isWow64;
				// �ж����� 32 �� 64 λ
				if ( !IsWow64Process( hProcess, &isWow64 ) )
				{
					//AfxMessageBox( TEXT( "���� bit λ����ȡʧ�ܣ�" ) );
					OutputDebugString( TEXT( "���� bit λ����ȡʧ�ܣ�\n" ) );
				}

				// �ͷ�
				CloseHandle( hProcess );

				if ( isWow64 )
				{ // Window �� 64 λ��ϵͳ��������� 32 λ���ļ�
					szProcBit = TEXT( "32λ" );

				}
				else
				{ // 64
					szProcBit = TEXT( "64λ" );
				}
			}
			else if
				( sys_info.wProcessorArchitecture == PROCESSOR_ARCHITECTURE_INTEL )
			{ // 32 λ����
				szProcBit = TEXT( "32λ" );
			}
			else
			{
				szProcBit = TEXT( "UN" );
			}

			// ��ʽ��������Ϣ
			wsprintf( szProcessStr, TEXT( "%4d (%s) %s" ), procEnt.th32ProcessID, szProcBit, procEnt.szExeFile );
			// �����У���ʽ����������Ϣ��ӵ��б�ؼ�
			int iList = m_ProcList.AddString( szProcessStr );
			// Ϊ��Ӧ�Ľ��̹������� ID
			m_ProcList.SetItemData( iList, procEnt.th32ProcessID );

		}
	}
	// �ͷŴ���
	CloseHandle( hProcessSnap );

}


/**
 * Function :		OnBnClickedbtninject
 * Description :	ע���嵥�б�ѡ��Ӧ�ó���
 *	//	1�������б�ѡ�����ָ��ע��
 * //	2���б�󶨽��� PID
 * //	3�������ļ�ѡ�� DLL ·������ remoteLoadLibrary ע��
 * Calls :			remoteLoadLibrary(HANDLE, LPCTSTR)
 * Called By :		��ť����
 * Output :			����ע��
 * Others :
 */
void CInject_x64Dlg::OnBnClickedbtninject( )
{
	// TODO: Add your control notification handler code here

	// 1����ȡ�ؼ�ѡ�񴰿ڵ� dll �ļ�·��
	CString strDllPathName = _T( " - 1" );
	m_DllEditBrowse.GetWindowText( strDllPathName );

	// 2����ȡ�б��嵥��ѡ��Ľ��� id ����ȡ����Ȩ�޵Ľ��̵ľ��
	/// ����д���������ڴ洢�ռ䣬ͬ�����洢�ռ�����������̣߳�����ִ�к���
	DWORD curItemData_pid = m_ProcList.GetItemData( m_ProcList.GetCurSel( ) );
	HANDLE cur_pid_process = OpenProcess( PROCESS_CREATE_THREAD
										  | PROCESS_VM_OPERATION | PROCESS_VM_READ | PROCESS_VM_WRITE
										  | SYNCHRONIZE, false, curItemData_pid );
	// 2.1 �жϽ����Ƿ��ʼ��
	if ( cur_pid_process == NULL )
	{
		OutputDebugStringA( "���̾����ȡʧ�ܣ�\n" );
	}

	// 3�����г���ע��
	//RemoteLoadLibrary( cur_pid_process, (LPCSTR) strDllPathName.GetBuffer( strDllPathName.GetLength( ) ) );
	RemoteLoadLibrary( cur_pid_process, (LPCTSTR) strDllPathName );
}


/*
 * Description:		��ע�룬ע����ע���Ӧ�ó���̬��
 */
void CInject_x64Dlg::OnBnClickedbtnbackinject( )
{
	// TODO: Add your control notification handler code here
}


/**
 * Function :			RemoteLoadLibrary
 * Description :		�� dll ע��ָ���ĳ�����
 * Calls :				LoadLibrary ����Զ��ע��
 * Called By :			inject ��ť��Ӧ
 * param1 :				hProcess ���̾��
 * param2 :				lpLibFileName ָ���� dll �ļ�·��
 * Output :
 * Others :				LPCTSTR ���ֽ��ַ�ָ��
 */
void RemoteLoadLibrary( HANDLE hProcess, LPCTSTR lpLibFileName )
{
	/// 4 ��Զ�̵�ַ�ռ�������ռ䣨���ⲻ��д�룩
	PVOID pMen = VirtualAllocEx( hProcess, NULL, 0x1000, MEM_COMMIT
								 , PAGE_READWRITE );
	if ( NULL == pMen )
	{
		OutputDebugStringA( "Զ�̵�ַ�ռ�����ʧ�ܣ�" );
	}
	else
	{
		/*
		SIZE_T pathSize = (_tcslen( dllPath ) + 1) * sizeof(TCHAR);
		 */
		// ע�������ַ���ʱ���� lstrlenW ���㣬�� * 2 ������·��ĩβ���� /0 ����
		/// 3 �� dll ��ַ���ַ�����д��Զ�̵�ַ�ռ��� + 2
		WriteProcessMemory( hProcess, pMen, lpLibFileName, lstrlenW( lpLibFileName ) * 2 + 2
							, NULL );

		/// 2 LoadLibrary �ݹ��ȡ���̵�ַ lpThreadStart������ 	Kernel32.dll ��
		HMODULE hmodule = LoadLibrary( TEXT( "Kemel32.dll" ) );
		int hmodule_error = GetLastError( );
		// loadLibraryW  Unicode �ַ���
		LPTHREAD_START_ROUTINE lpThreadStart
			= (LPTHREAD_START_ROUTINE) GetProcAddress( hmodule, "LoadLibraryW" );

		/// 1 ����Զ���߳�
		DWORD dwThreadID = { 0 }; // �洢Զ���߳� ID
		// lpThreadStart��LoadLibraryW�� : ���̵�ַ������ͬ������ϵͳ�Ŀ⣬dll �������ڹ������أ��ڹ����Ĵ洢�ռ���
		HANDLE createThread = CreateRemoteThread( hProcess, NULL, 0, lpThreadStart, pMen
												  , 0, &dwThreadID );
		if ( createThread == NULL )
		{
			OutputDebugStringA( LPCSTR( "�����߳�ʧ�ܣ�\n" ) );
		}
		else
		{
			// �ж��߳�״̬��ִ����Ͻ����ͷ�
			WaitForSingleObject( createThread, INFINITE );
			AfxMessageBox( TEXT( "�߳�ע��ɹ���" ) );
		}

	}
	// �ͷŴ���
	VirtualFreeEx( hProcess, pMen, 0x1000, MEM_RELEASE );

}