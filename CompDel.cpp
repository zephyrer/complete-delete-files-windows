// CompDel.cpp : アプリケーション用クラスの定義を行います。
//


#include "stdafx.h"
#include "CompDel.h"
#include "Fileman.h"
#include "DlgConfig.h"
#include "DlgConfirm.h"
#include "DlgHelpDoc.h"
#include "CheckWinVer.h"

#include "InstFunc.h"

#include <HtmlHelp.h>
#include <stdlib.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CCompDelApp

BEGIN_MESSAGE_MAP(CCompDelApp, CWinApp)
	//{{AFX_MSG_MAP(CCompDelApp)
		// メモ - ClassWizard はこの位置にマッピング用のマクロを追加または削除します。
		//        この位置に生成されるコードを編集しないでください。
	//}}AFX_MSG
	ON_COMMAND(ID_HELP, CWinApp::OnHelp)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CCompDelApp クラスの構築

CCompDelApp::CCompDelApp()
{
	// TODO: この位置に構築用のコードを追加してください。
	// ここに InitInstance 中の重要な初期化処理をすべて記述してください。
}

/////////////////////////////////////////////////////////////////////////////
// 唯一の CCompDelApp オブジェクト

CCompDelApp theApp;

/////////////////////////////////////////////////////////////////////////////
// 唯一の ファイル名･ファイル操作 オブジェクト

CFileman MyFile;

/////////////////////////////////////////////////////////////////////////////
// CCompDelApp クラスの初期化

BOOL CCompDelApp::InitInstance()
{
	// 標準的な初期化処理
	// もしこれらの機能を使用せず、実行ファイルのサイズを小さくしたけ
	//  れば以下の特定の初期化ルーチンの中から不必要なものを削除して
	//  ください。

#ifdef _AFXDLL
	Enable3dControls();			// 共有 DLL 内で MFC を使う場合はここをコールしてください。
#else
	Enable3dControlsStatic();	// MFC と静的にリンクする場合はここをコールしてください。
#endif

	// レジストリの指定
	// リソースでAFX_IDS_APP_TITLEにアプリケーション名を設定すること 
	// m_pszAppName = "compdel" としてはいけない！ ASSERTION エラーとなる
	SetRegistryKey((LPCTSTR)"hi soft");


	// コマンドライン引数の解析と処理実行
	ProcessCommandline();
	// ダイアログが閉じられてからアプリケーションのメッセージ ポンプを開始するよりは、
	// アプリケーションを終了するために FALSE を返してください。
	return FALSE;
}

// ************************************************************
// コマンドライン引数による振り分け
// (インストール/アンインストール/設定ダイアログ/削除実行)
// ************************************************************
BOOL CCompDelApp::ProcessCommandline()
{
	CString sAfxMsg;
	// ************************************************************
	// 初回の実行のみ、インストールダイアログを出現
	// ************************************************************
	if(!ChkHkcuInstReg())
	{	// インストールフラグが見つからない
		CDlgInstall dlgInst;
		// インストールダイアログの表示
		dlgInst.m_CONTEXT = FALSE;
		dlgInst.m_DESKTOP = TRUE;
		dlgInst.m_MENU = FALSE;
		dlgInst.m_UNIN = 1;

		if(dlgInst.DoModal() == IDOK)
		{
			::MkHkcuInstReg();		// インストールフラグの書き込み
			if(dlgInst.m_UNIN == 0)
			{	// レジストリのアンインストールメニューを作成
				if(IsWinNT()) ::MkHkcuUninstMnu();	// WinNT 系列のとき HKCU
				else  ::MkHklmUninstMnu();	// Win95 系列のとき HKLM
			}
			else
				::MkUninstMnu();		// スタートメニューにアンインストール･ショートカットを作成
			if(dlgInst.m_CONTEXT)
				::MkSendtoMnu();		// 送るメニューにショートカット作成
			if(dlgInst.m_DESKTOP)
				::MkDesktopMnu();		// デスクトップにショートカット作成
			if(dlgInst.m_MENU)
				::MkProgramsMnu();		// スタートメニューにショートカット作成
		}
		else return FALSE;	// インストール却下
	}

	// ************************************************************
	// 動作モードをレジストリより読み込む
	// ************************************************************
	MyFile.n_NDispN = GetProfileInt("Settings","NDispN",0);
	MyFile.n_NDispE = GetProfileInt("Settings","NDispE",0);
	MyFile.n_Dummy = GetProfileInt("Settings","Dummy",0);
	MyFile.n_DummySkip = GetProfileInt("Settings","DummySkip",1);
	MyFile.n_Date = GetProfileInt("Settings","Date",1);
	MyFile.n_rNull = GetProfileInt("Settings","rNull",1);
	MyFile.n_rZLen = GetProfileInt("Settings","rZLen",1);
	MyFile.n_rRen = GetProfileInt("Settings","rRen",1);
	MyFile.n_nOvwr = GetProfileInt("Settings","nOvwr",5);
	MyFile.n_nFiles = GetProfileInt("Settings","nFiles",40);
	MyFile.n_Confirm = GetProfileInt("Settings","Confirm",0);
	MyFile.n_DispF = GetProfileInt("Settings","DispF",1);
	MyFile.n_Log = GetProfileInt("Settings","Log",1);

	MyFile.b_AntiCache = GetProfileInt("Settings","AddWipe",0);
	MyFile.n_UntiFolder = GetProfileInt("Settings","rAntiFolder",0);
	MyFile.n_UnticacheSize = GetProfileInt("Settings","nAnticacheSize",20);
	MyFile.n_bAntiOneShot = GetProfileInt("Settings","bAntiOneShot",1);
	MyFile.n_BufferSize = GetProfileInt("Settings","m_nBufferSize",2048);
	MyFile.n_Overrun = GetProfileInt("Settings","m_nOverrun",0);
	MyFile.n_DodChar = GetProfileInt("Settings","m_nDodChar",(int)'A');

	MyFile.b_NotRemove = GetProfileInt("Settings","bNotDel",0);


	// ************************************************************
	// 設定ダイアログ表示。
	// ************************************************************
	if(!strlen(m_lpCmdLine))
	{	// コマンドライン引数なしのときは、設定ダイアログ
		// メインダイアログの表示
		sAfxMsg.LoadString(AFX_STR_MAINDLG);
		CPropertySheet dlg((LPCSTR)sAfxMsg);	// ダイアログのベース
		// プロパティーシート
		CDlgPDel dlgPDel;
		CDlgPDel2 dlgPDel2;
		CDlgPDel3 dlgPDel3;
		CDlgPDisp dlgPDisp;
		CDlgPOther dlgPOther;

		m_pMainWnd = &dlg;	// このダイアログをメインフレームとする
		dlgPDisp.m_NDispN = MyFile.n_NDispN;
		dlgPDisp.m_NDispE = MyFile.n_NDispE;
		dlgPDel3.m_bDummy = MyFile.n_Dummy;
		dlgPDel3.m_bDummySkip = MyFile.n_DummySkip;
		dlgPDel3.m_nFiles = MyFile.n_nFiles;
		dlgPDel.m_Date = MyFile.n_Date;
		dlgPDel.m_rNull = MyFile.n_rNull;
		dlgPDel.m_rZLen = MyFile.n_rZLen;
		dlgPDel.m_rRen = MyFile.n_rRen;
		dlgPDel.m_nOvwr = MyFile.n_nOvwr;
		dlgPDel3.m_AntiCache = MyFile.b_AntiCache;
		dlgPDel3.m_rAntiFolder = MyFile.n_UntiFolder;
		dlgPDel3.m_nAnticacheSize = MyFile.n_UnticacheSize;
		dlgPDel3.m_bAntiOneShot = MyFile.n_bAntiOneShot;
		dlgPDel2.m_nBufferSize = MyFile.n_BufferSize;
		dlgPDel2.m_nOverrun = MyFile.n_Overrun;
		dlgPDel2.m_nDodChar = MyFile.n_DodChar;
		dlgPDisp.m_Confirm = MyFile.n_Confirm;
		dlgPDisp.m_DispF = MyFile.n_DispF;
		dlgPDisp.m_Log = MyFile.n_Log;
		dlgPDel2.m_bNotRemove = MyFile.b_NotRemove;

		// プロパティーページの連結
		dlg.AddPage(&dlgPDel);
		dlg.AddPage(&dlgPDisp);
		dlg.AddPage(&dlgPDel2);
		dlg.AddPage(&dlgPDel3);
		dlg.AddPage(&dlgPOther);
		// プロパティーシートの属性変更
		dlg.m_psh.dwFlags=(dlg.m_psh.dwFlags|PSH_NOAPPLYNOW);// &(~PSH_HASHELP);
		if(dlg.DoModal() == IDOK)
		{
			if(dlgPDisp.m_NDispN != (int)GetProfileInt("Settings","NDispN",0))
				WriteProfileInt("Settings","NDispN",dlgPDisp.m_NDispN);	
			if(dlgPDisp.m_NDispE != (int)GetProfileInt("Settings","NDispE",0))
				WriteProfileInt("Settings","NDispE",dlgPDisp.m_NDispE);	
			if(dlgPDel3.m_bDummy != (int)GetProfileInt("Settings","Dummy",0))
				WriteProfileInt("Settings","Dummy",dlgPDel3.m_bDummy);	
			if(dlgPDel3.m_bDummySkip != (int)GetProfileInt("Settings","DummySkip",1))
				WriteProfileInt("Settings","DummySkip",dlgPDel3.m_bDummySkip);	
			if(dlgPDel3.m_nFiles != (int)GetProfileInt("Settings","nFiles",40))
				WriteProfileInt("Settings","nFiles",dlgPDel3.m_nFiles);	
			if(dlgPDel.m_Date != (int)GetProfileInt("Settings","Date",1))
				WriteProfileInt("Settings","Date",dlgPDel.m_Date);	
			if(dlgPDel.m_rNull != (int)GetProfileInt("Settings","rNull",1))
				WriteProfileInt("Settings","rNull",dlgPDel.m_rNull);
			if(dlgPDel.m_rZLen != (int)GetProfileInt("Settings","rZLen",1))
				WriteProfileInt("Settings","rZLen",dlgPDel.m_rZLen);
			if(dlgPDel.m_rRen != (int)GetProfileInt("Settings","rRen",1))
				WriteProfileInt("Settings","rRen",dlgPDel.m_rRen);
			if(dlgPDel.m_nOvwr != (int)GetProfileInt("Settings","nOvwr",5))
				WriteProfileInt("Settings","nOvwr",dlgPDel.m_nOvwr);
			if(dlgPDel3.m_AntiCache != (int)GetProfileInt("Settings","AddWipe",0))
				WriteProfileInt("Settings","AddWipe",dlgPDel3.m_AntiCache);
			if(dlgPDel3.m_rAntiFolder != (int)GetProfileInt("Settings","rAntiFolder",0))
				WriteProfileInt("Settings","rAntiFolder",dlgPDel3.m_rAntiFolder);
			if(dlgPDel3.m_nAnticacheSize != (int)GetProfileInt("Settings","nAnticacheSize",20))
				WriteProfileInt("Settings","nAnticacheSize",dlgPDel3.m_nAnticacheSize);
			if(dlgPDel3.m_bAntiOneShot != (int)GetProfileInt("Settings","bAntiOneShot",1))
				WriteProfileInt("Settings","bAntiOneShot",dlgPDel3.m_bAntiOneShot);
			if(dlgPDel2.m_nOverrun != (int)GetProfileInt("Settings","m_nOverrun",1024))
				WriteProfileInt("Settings","m_nOverrun",dlgPDel2.m_nOverrun);
			if(dlgPDel2.m_nDodChar != GetProfileInt("Settings","m_nDodChar",(int)'A'))
				WriteProfileInt("Settings","m_nDodChar",dlgPDel2.m_nDodChar);
			if(dlgPDel2.m_nBufferSize != (int)GetProfileInt("Settings","m_nBufferSize",2048))
				WriteProfileInt("Settings","m_nBufferSize",dlgPDel2.m_nBufferSize);
			if(dlgPDisp.m_Confirm != (int)GetProfileInt("Settings","Confirm",0))
				WriteProfileInt("Settings","Confirm",dlgPDisp.m_Confirm);
			if(dlgPDisp.m_DispF != (int)GetProfileInt("Settings","DispF",1))
				WriteProfileInt("Settings","DispF",dlgPDisp.m_DispF);
			if(dlgPDisp.m_Log != (int)GetProfileInt("Settings","Log",1))
				WriteProfileInt("Settings","Log",dlgPDisp.m_Log);
			if(dlgPDel2.m_bNotRemove != (int)GetProfileInt("Settings","bNotDel",0))
				WriteProfileInt("Settings","bNotDel",dlgPDel2.m_bNotRemove);

		}
	}
	// ************************************************************
	// アンインストール
	// ************************************************************
	else if(!strcmp(m_lpCmdLine, "/delete"))
	{	// アンインストール
		sAfxMsg.LoadString(AFX_STR_UNIN_CONFERM);
		if(::MessageBox(NULL, (LPCSTR)sAfxMsg, "CompleteDelete Uninstaller", MB_YESNO|MB_ICONQUESTION) != IDYES)
			return FALSE;
		::RmUserReg();			// ユーザ領域のレジストリデータの削除
		::RmHkcuInstReg();		// HKCU の インストールフラグの削除
		::RmUninstMnu();		// スタートメニューのアンインストール･ショートカットを削除
		::RmHkcuUninstMnu();	// レジストリのアンインストールメニューを削除 HKCU
		::RmHklmUninstMnu();	// レジストリのアンインストールメニューを削除 HKLM
		::RmDesktopMnu();		// デスクトップを削除
		::RmProgramsMnu();		// スタートメニューを削除
		::RmSendtoMnu();		// 送るメニューを削除
	}
	// ************************************************************
	// ファイルの削除 メインプログラムの実行
	// ************************************************************
	else
	{

		// ログ用CStringポインタの引渡し
		sLogStr = "";
		MyFile.sLogStr = &sLogStr;
		// 削除メインルーチン
		MyFile.DeleteMain(m_lpCmdLine);

		// ログの表示
		if(MyFile.n_Log)
		{
			CDlgHelpDoc dlgHelp;
			dlgHelp.m_edit_main = sLogStr;
			dlgHelp.DoModal();
		}
	}
	return TRUE;
}


BOOL CCompDelApp::QuickDeleteExec(CString sFileName)
{
	char f_path_buffer[_MAX_PATH];

	// " ... " で囲まれたファイル名形式にする
	sprintf(f_path_buffer, "\"%s\"", sFileName);

	// 動作モードをレジストリより読み込む
	MyFile.n_NDispN = GetProfileInt("Settings","NDispN",0);
	MyFile.n_NDispE = GetProfileInt("Settings","NDispE",0);
	MyFile.n_Dummy = GetProfileInt("Settings","Dummy",0);
	MyFile.n_DummySkip = GetProfileInt("Settings","DummySkip",1);
	MyFile.n_Date = GetProfileInt("Settings","Date",1);
	MyFile.n_rNull = GetProfileInt("Settings","rNull",1);
	MyFile.n_rZLen = GetProfileInt("Settings","rZLen",1);
	MyFile.n_rRen = GetProfileInt("Settings","rRen",1);
	MyFile.n_nOvwr = GetProfileInt("Settings","nOvwr",5);
	MyFile.n_nFiles = GetProfileInt("Settings","nFiles",40);
	MyFile.n_Confirm = GetProfileInt("Settings","Confirm",0);
	MyFile.n_DispF = GetProfileInt("Settings","DispF",1);
	MyFile.n_Log = GetProfileInt("Settings","Log",1);

	MyFile.b_AntiCache = GetProfileInt("Settings","AddWipe",0);
	MyFile.n_UntiFolder = GetProfileInt("Settings","rAntiFolder",0);
	MyFile.n_UnticacheSize = GetProfileInt("Settings","nAnticacheSize",20);
	MyFile.n_bAntiOneShot = GetProfileInt("Settings","bAntiOneShot",1);
	MyFile.n_BufferSize = GetProfileInt("Settings","m_nBufferSize",2048);
	MyFile.n_Overrun = GetProfileInt("Settings","m_nOverrun",1024);
	MyFile.n_DodChar = GetProfileInt("Settings","m_nDodChar",(int)'A');

	// ログ用CStringポインタの引渡し
	sLogStr = "";
	MyFile.sLogStr = &sLogStr;
	// 削除メインルーチン
	MyFile.DeleteMain(f_path_buffer);

	// ログの表示
	if(MyFile.n_Log)
	{
		CDlgHelpDoc dlgHelp;
		dlgHelp.m_edit_main = sLogStr;
		dlgHelp.DoModal();
	}
	return TRUE;
}

// ************************************************************
// ヘルプ表示関数（仮想関数をオーバーライド）
// HTMLヘルプに対応させるために、アプリケーションの最上位クラスでオーバーライド
// 
// 引数 dwData : 下位8ビットに、resource.hで定義されたダイアログのIDが入る
//      nCmd   : HELP_CONTEXT=1
// ************************************************************
void CCompDelApp::WinHelp(DWORD dwData, UINT nCmd) 
{
	// TODO: この位置に固有の処理を追加するか、または基本クラスを呼び出してください
	
//	既存の WinHelp 関数を無効にする
//	CWinApp::WinHelp(dwData, nCmd);


	// HELP_CONTEXT 以外は何もしない
	if(nCmd != HELP_CONTEXT) return;

	// HTMLヘルプのhWndハンドラ （失敗時はNULL）
	HWND hWnd_Help;
	// ヘルプファイルへの絶対パスを作るための、パス分解用一時文字列
	char szChmPath[MAX_PATH], szAppPath[MAX_PATH];
	char szDrive[_MAX_DRIVE];
	char szDir[_MAX_DIR];
	char szFname[_MAX_FNAME];
	char szExt[_MAX_EXT];

	CString sTmp, sAfxMsg;

	// アプリケーション自身のパスを取得し、拡張子を chm に書き換える
	// (HtmlHelp関数はカレントフォルダのヘルプファイルを取得しようとするため)
	if(!::GetModuleFileName(NULL, szAppPath, MAX_PATH)) return;
	::_splitpath(szAppPath, szDrive, szDir, szFname, szExt);
	::_makepath(szChmPath, szDrive, szDir,szFname, ".chm");

	// ヘルプの表示
	if(this->m_pMainWnd == NULL)
	{	// メインウインドウのハンドラが定義されていないとき
		hWnd_Help = ::HtmlHelp(NULL, szChmPath, HH_HELP_CONTEXT, LOWORD(dwData));
	}
	else
	{
//		hWnd_Help = ::HtmlHelp(this->m_pMainWnd->m_hWnd, szChmPath, HH_DISPLAY_TOPIC, NULL);
		hWnd_Help = ::HtmlHelp(this->m_pMainWnd->m_hWnd, szChmPath, HH_HELP_CONTEXT, LOWORD(dwData));
	}

	if(hWnd_Help == NULL)
	{	// ヘルプファイルの起動に失敗した場合
		sAfxMsg.LoadString(AFX_STR_ERR_HELP);	// 「ヘルプファイルの表示ができません\r\n ファイル: %s\r\n コンテキストID: %04X」

		sTmp.Format(sAfxMsg, szChmPath, LOWORD(dwData));
		this->m_pMainWnd->MessageBox(sTmp, "Help File Error", MB_ICONWARNING);
	}
}
