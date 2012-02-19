// CompDel.cpp : アプリケーション用クラスの定義を行います。
//


#include "stdafx.h"
#include "CompDel.h"
#include "Fileman.h"
#include "DlgConfig.h"
#include "DlgConfirm.h"
#include "DlgHelpDoc.h"

#include "InstFunc.h"

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
				if(CheckWinNT()) ::MkHkcuUninstMnu();	// WinNT 系列のとき HKCU
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
	// 設定ダイアログ表示。
	// ************************************************************
	if(!strlen(m_lpCmdLine))
	{	// コマンドライン引数なしのときは、設定ダイアログ
		// メインダイアログの表示
		sAfxMsg.LoadString(AFX_STR_MAINDLG);
		CPropertySheet dlg((LPCSTR)sAfxMsg);	// ダイアログのベース
		// プロパティーシート
		CDlgPDel dlgPDel;
		CDlgPDisp dlgPDisp;
		CDlgPOther dlgPOther;

		m_pMainWnd = &dlg;	// このダイアログをメインフレームとする
		dlgPDisp.m_NDispN = GetProfileInt("Settings","NDispN",0);
		dlgPDisp.m_NDispE = GetProfileInt("Settings","NDispE",0);
		dlgPOther.m_Dummy = GetProfileInt("Settings","Dummy",0);
		dlgPOther.m_DummySkip = GetProfileInt("Settings","DummySkip",1);
		dlgPOther.m_nFiles = GetProfileInt("Settings","nFiles",40);
		dlgPDel.m_Date = GetProfileInt("Settings","Date",1);
		dlgPDel.m_rNull = GetProfileInt("Settings","rNull",1);
		dlgPDel.m_rZLen = GetProfileInt("Settings","rZLen",1);
		dlgPDel.m_rRen = GetProfileInt("Settings","rRen",1);
		dlgPDel.m_nOvwr = GetProfileInt("Settings","nOvwr",2);
		dlgPDisp.m_Confirm = GetProfileInt("Settings","Confirm",0);
		dlgPDisp.m_DispF = GetProfileInt("Settings","DispF",1);
		dlgPDisp.m_Log = GetProfileInt("Settings","Log",1);

		// プロパティーページの連結
		dlg.AddPage(&dlgPDel);
		dlg.AddPage(&dlgPDisp);
		dlg.AddPage(&dlgPOther);
		// プロパティーシートの属性変更
		dlg.m_psh.dwFlags=(dlg.m_psh.dwFlags|PSH_NOAPPLYNOW);// &(~PSH_HASHELP);
		if(dlg.DoModal() == IDOK)
		{
			if(dlgPDisp.m_NDispN != (int)GetProfileInt("Settings","NDispN",0))
				WriteProfileInt("Settings","NDispN",dlgPDisp.m_NDispN);	
			if(dlgPDisp.m_NDispE != (int)GetProfileInt("Settings","NDispE",0))
				WriteProfileInt("Settings","NDispE",dlgPDisp.m_NDispE);	
			if(dlgPOther.m_Dummy != (int)GetProfileInt("Settings","Dummy",0))
				WriteProfileInt("Settings","Dummy",dlgPOther.m_Dummy);	
			if(dlgPOther.m_DummySkip != (int)GetProfileInt("Settings","DummySkip",1))
				WriteProfileInt("Settings","DummySkip",dlgPOther.m_DummySkip);	
			if(dlgPOther.m_nFiles != (int)GetProfileInt("Settings","nFiles",40))
				WriteProfileInt("Settings","nFiles",dlgPOther.m_nFiles);	
			if(dlgPDel.m_Date != (int)GetProfileInt("Settings","Date",1))
				WriteProfileInt("Settings","Date",dlgPDel.m_Date);	
			if(dlgPDel.m_rNull != (int)GetProfileInt("Settings","rNull",1))
				WriteProfileInt("Settings","rNull",dlgPDel.m_rNull);
			if(dlgPDel.m_rZLen != (int)GetProfileInt("Settings","rZLen",1))
				WriteProfileInt("Settings","rZLen",dlgPDel.m_rZLen);
			if(dlgPDel.m_rRen != (int)GetProfileInt("Settings","rRen",1))
				WriteProfileInt("Settings","rRen",dlgPDel.m_rRen);
			if(dlgPDel.m_nOvwr != (int)GetProfileInt("Settings","nOvwr",2))
				WriteProfileInt("Settings","nOvwr",dlgPDel.m_nOvwr);
			if(dlgPDisp.m_Confirm != (int)GetProfileInt("Settings","Confirm",0))
				WriteProfileInt("Settings","Confirm",dlgPDisp.m_Confirm);
			if(dlgPDisp.m_DispF != (int)GetProfileInt("Settings","DispF",1))
				WriteProfileInt("Settings","DispF",dlgPDisp.m_DispF);
			if(dlgPDisp.m_Log != (int)GetProfileInt("Settings","Log",1))
				WriteProfileInt("Settings","Log",dlgPDisp.m_Log);

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
		// 動作モードをレジストリより読み込む
		MyFile.n_NDispN = GetProfileInt("Settings","NDispN",0);
		MyFile.n_NDispE = GetProfileInt("Settings","NDispE",0);
		MyFile.n_Dummy = GetProfileInt("Settings","Dummy",0);
		MyFile.n_DummySkip = GetProfileInt("Settings","DummySkip",1);
		MyFile.n_Date = GetProfileInt("Settings","Date",1);
		MyFile.n_rNull = GetProfileInt("Settings","rNull",1);
		MyFile.n_rZLen = GetProfileInt("Settings","rZLen",1);
		MyFile.n_rRen = GetProfileInt("Settings","rRen",1);
		MyFile.n_nOvwr = GetProfileInt("Settings","nOvwr",2);
		MyFile.n_nFiles = GetProfileInt("Settings","nFiles",40);
		MyFile.n_Confirm = GetProfileInt("Settings","Confirm",0);
		MyFile.n_DispF = GetProfileInt("Settings","DispF",1);
		MyFile.n_Log = GetProfileInt("Settings","Log",1);

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

// ************************************************************
// WindowsNT 系列と Windows 95 系列の判定
// WinNT 系列のとき TRUE
// ************************************************************
BOOL CCompDelApp::CheckWinNT()
{
	OSVERSIONINFO ver;

	// OSバージョン
	ver.dwOSVersionInfoSize = sizeof(OSVERSIONINFO);
	::GetVersionEx((LPOSVERSIONINFO)&ver);


	if(ver.dwPlatformId == VER_PLATFORM_WIN32_NT)
		return TRUE;

	return FALSE;
}
