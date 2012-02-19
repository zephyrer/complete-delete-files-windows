// ************************************************************
// インストール・アンインストール関連
//
// ************************************************************

#include "StdAfx.h"
#include "InstFunc.h"
#include <io.h>			// findfirst

// リソース文字の番号解決に必要
#include "resource.h"		// メイン シンボル


// レジストリに設定する各種値のグローバル値をセットする。（重要）


// 自動実行のレジストリ位置
#define STR_REGKEY_RUN		"Software\\Microsoft\\Windows\\CurrentVersion\\Run"

// コントロールパネルのアンインストール情報の格納位置
#define STR_REGKEY_UNINST	"Software\\Microsoft\\Windows\\CurrentVersion\\Uninstall"
// コントロールパネルに表示する 名前（アンインストール用）
// リソースのAFX_STR_UNINST_CPLNAME が優先される
#define STR_UNINST_CPLNAME	"完全削除"

// スタートアップ・メニューのフォルダ名等が格納されているレジストリ
#define STR_REGKEY_EXPLR_FOLDER	"Software\\Microsoft\\Windows\\CurrentVersion\\Explorer\\Shell Folders"

// このプログラムのインストールフラグ用に使うレジストリ位置
#define STR_REGKEY_INST		"Software\\hi_soft"

// プログラムのユーザデータ（HKCU) の位置（ユーザデータの削除に利用）
// 最下段 (この下の "Settings" キーを削除する)
#define STR_REGKEY_USERDAT_1	"Software\\hi soft\\compdel"
// 上段 (この下の AFX_IDS_APP_TITLE キーを削除する)
#define STR_REGKEY_USERDAT_2	"Software\\hi soft"


// このプログラムの設定用に使うレジストリの位置（プログラム名）
//  リソースの AFX_IDS_APP_TITLE にセットすること
//  この場合、sTmp.
#define _ALTER_APP_TITLE "compdel"		// AFX_IDS_APP_TITLE が無いときはこちらを使う

// アンインストール・ショートカット名
// リソースのAFX_STR_LINK_UNINST が優先される
#define STR_LINK_UNINST		"\\完全削除アンインストール.LNK"

// ショートカット名 (スタートメニューのスタートに登録する場合の名前）
// リソースのAFX_STR_LINK_STARTUP が優先される
#define STR_LINK_STARTUP	"\\完全削除StartUp.LNK"

// ショートカット名 （デスクトップ、コンテキストメニュー、スタートメニューのその他の時の名前）
// リソースのAFX_STR_LINK_PROG が優先される
#define STR_LINK_PROG		"\\完全削除.LNK"


// ************************************************************
// レジストリの起動時実行の設定・削除
// 「ProgramName /start」というコマンドラインとなる
// (HKLM / HKCU)
// ************************************************************

// HKLM に自動実行リンクを作る
BOOL MkHklmLnk(void)
{
	char szProg[MAX_PATH], tmpBuf[MAX_PATH];
	BOOL ret = FALSE;
	CString sProgName;	// AFX_IDS_APP_TITLE の プログラム名を読み込む

	// ****************** 多言語対応 リソーステーブルの読み込み ******************
	if(!sProgName.LoadString(AFX_IDS_APP_TITLE))
		sProgName = _ALTER_APP_TITLE;		// リソースがセットされていない時（ありえない）	
	// ****************** 多言語対応 リソーステーブルの読み込み ******************

	// 実体のフルパス名
	if(!::GetModuleFileName(NULL, tmpBuf, MAX_PATH)) return ret;
	sprintf(szProg, "\"%s\" /start", tmpBuf);
	HKEY hCU;

	if (RegOpenKeyEx(HKEY_LOCAL_MACHINE, STR_REGKEY_RUN, 0, KEY_WRITE, &hCU) == ERROR_SUCCESS)
	{
		if(RegSetValueEx( hCU, sProgName, 0, REG_SZ, (const unsigned char *)szProg, strlen(szProg)) == ERROR_SUCCESS)
			ret = TRUE;
		RegCloseKey(hCU);
	}
	return ret;
}

// HKCU に自動実行リンクを作る
BOOL MkHkcuLnk(void)
{
	char szProg[MAX_PATH], tmpBuf[MAX_PATH];
	BOOL ret=FALSE;
	CString sProgName;	// AFX_IDS_APP_TITLE の プログラム名を読み込む

	// ****************** 多言語対応 リソーステーブルの読み込み ******************
	if(!sProgName.LoadString(AFX_IDS_APP_TITLE))
		sProgName = _ALTER_APP_TITLE;		// リソースがセットされていない時（ありえない）	
	// ****************** 多言語対応 リソーステーブルの読み込み ******************

	// 実体のフルパス名
	if(!::GetModuleFileName(NULL, tmpBuf, MAX_PATH)) return ret;
	sprintf(szProg, "\"%s\" /start", tmpBuf);
	HKEY hCU;

	if (RegOpenKeyEx(HKEY_CURRENT_USER, STR_REGKEY_RUN, 0, KEY_WRITE, &hCU) == ERROR_SUCCESS)
	{
		if(RegSetValueEx( hCU, sProgName, 0, REG_SZ, (const unsigned char *)szProg, strlen(szProg)) == ERROR_SUCCESS)
			ret = TRUE;
		RegCloseKey(hCU);
	}
	return ret;
}

// HKLM の自動リンクを削除する
BOOL RmHklmLnk(void) 
{
	// TODO: この位置にコントロール通知ハンドラ用のコードを追加してください
	HKEY hCU;
	BOOL ret=FALSE;
	CString sProgName;	// AFX_IDS_APP_TITLE の プログラム名を読み込む

	// ****************** 多言語対応 リソーステーブルの読み込み ******************
	if(!sProgName.LoadString(AFX_IDS_APP_TITLE))
		sProgName = _ALTER_APP_TITLE;		// リソースがセットされていない時（ありえない）	
	// ****************** 多言語対応 リソーステーブルの読み込み ******************

	if (RegOpenKeyEx(HKEY_LOCAL_MACHINE, STR_REGKEY_RUN, 0, KEY_WRITE, &hCU) == ERROR_SUCCESS)
	{
		if(RegDeleteValue( hCU, sProgName) == ERROR_SUCCESS)
			ret = TRUE;
		RegCloseKey(hCU);
	}
	return ret;	
}

// HKCU の自動リンクを削除する
BOOL RmHkcuLnk(void) 
{
	// TODO: この位置にコントロール通知ハンドラ用のコードを追加してください
	HKEY hCU;
	BOOL ret=FALSE;
	CString sProgName;	// AFX_IDS_APP_TITLE の プログラム名を読み込む

	// ****************** 多言語対応 リソーステーブルの読み込み ******************
	if(!sProgName.LoadString(AFX_IDS_APP_TITLE))
		sProgName = _ALTER_APP_TITLE;		// リソースがセットされていない時（ありえない）	
	// ****************** 多言語対応 リソーステーブルの読み込み ******************

	if (RegOpenKeyEx(HKEY_CURRENT_USER, STR_REGKEY_RUN, 0, KEY_WRITE, &hCU) == ERROR_SUCCESS)
	{
		if(RegDeleteValue( hCU, sProgName) == ERROR_SUCCESS)
			ret = TRUE;
		RegCloseKey(hCU);
	}
	return ret;	
}

// ************************************************************
// スタートメニューの起動時実行の設定・削除
// 「ProgramName /start」というコマンドラインとなる
// (カレントユーザ)
// ************************************************************
// スタートメニューに自動実行アイコンを作る
void MkStartMnu(void) 
{
	// TODO: この位置にコントロール通知ハンドラ用のコードを追加してください
	char szLink[MAX_PATH];
	char szProg[MAX_PATH];
	CString sProgName;	// AFX_IDS_APP_TITLE の プログラム名を読み込む
	CString sLinkStartup;	// AFX_STR_LINK_STARTUP を読み込む

	// ****************** 多言語対応 リソーステーブルの読み込み ******************
	if(!sProgName.LoadString(AFX_IDS_APP_TITLE))
		sProgName = _ALTER_APP_TITLE;		// リソースがセットされていない時（ありえない）	
	if(!sLinkStartup.LoadString(AFX_STR_LINK_STARTUP))
		sLinkStartup = STR_LINK_STARTUP;		// リソースがセットされていない時（ありえない）	
	// ****************** 多言語対応 リソーステーブルの読み込み ******************

	// 実体のフルパス名
	if(!::GetModuleFileName(NULL, szProg, MAX_PATH)) return;
	HKEY hCU;
	DWORD lpType;
	ULONG ulSize = MAX_PATH;

	if (RegOpenKeyEx(HKEY_CURRENT_USER, STR_REGKEY_EXPLR_FOLDER, 0, KEY_QUERY_VALUE, &hCU) == ERROR_SUCCESS)
	{
		RegQueryValueEx( hCU, "Startup", NULL, &lpType, (unsigned char *)&szLink, &ulSize);
		RegCloseKey(hCU);
	}
	strcat(szLink, (LPCSTR)sLinkStartup);
	::CreateShellLink(szProg, szLink, (LPSTR)(LPCSTR)sProgName, "/start");
		
}

// スタートメニューの自動実行アイコンを削除する
void RmStartMnu(void) 
{
	// TODO: この位置にコントロール通知ハンドラ用のコードを追加してください
	char szLink[MAX_PATH];

	HKEY hCU;
	DWORD lpType;
	ULONG ulSize = MAX_PATH;
	CString sLinkStartup;	// AFX_STR_LINK_STARTUP を読み込む

	// ****************** 多言語対応 リソーステーブルの読み込み ******************
	if(!sLinkStartup.LoadString(AFX_STR_LINK_STARTUP))
		sLinkStartup = STR_LINK_STARTUP;		// リソースがセットされていない時（ありえない）	
	// ****************** 多言語対応 リソーステーブルの読み込み ******************

	if (RegOpenKeyEx(HKEY_CURRENT_USER, STR_REGKEY_EXPLR_FOLDER, 0, KEY_QUERY_VALUE, &hCU) == ERROR_SUCCESS)
	{
		RegQueryValueEx( hCU, "Startup", NULL, &lpType, (unsigned char *)&szLink, &ulSize);
		RegCloseKey(hCU);
	}
	strcat(szLink, (LPCSTR)sLinkStartup);
	::remove(szLink);
	
}

// ************************************************************
// コンテキストメニュー（SendTo)の設定・削除
// 「ProgramName」というコマンドラインとなる (スイッチなし）
// (カレントユーザ)
// ************************************************************
// コンテキストメニュー（SendTo)に自動実行アイコンを作る
void MkSendtoMnu(void) 
{
	// TODO: この位置にコントロール通知ハンドラ用のコードを追加してください
	char szLink[MAX_PATH];
	char szProg[MAX_PATH];
	CString sProgName;	// AFX_IDS_APP_TITLE の プログラム名を読み込む
	CString sLinkProg;	// AFX_STR_LINK_PROG を読み込む

	// ****************** 多言語対応 リソーステーブルの読み込み ******************
	if(!sProgName.LoadString(AFX_IDS_APP_TITLE))
		sProgName = _ALTER_APP_TITLE;		// リソースがセットされていない時（ありえない）	
	if(!sLinkProg.LoadString(AFX_STR_LINK_PROG))
		sLinkProg = STR_LINK_PROG;		// リソースがセットされていない時（ありえない）	
	// ****************** 多言語対応 リソーステーブルの読み込み ******************


	// 実体のフルパス名
	if(!::GetModuleFileName(NULL, szProg, MAX_PATH)) return;
	HKEY hCU;
	DWORD lpType;
	ULONG ulSize = MAX_PATH;

	if (RegOpenKeyEx(HKEY_CURRENT_USER, STR_REGKEY_EXPLR_FOLDER, 0, KEY_QUERY_VALUE, &hCU) == ERROR_SUCCESS)
	{
		RegQueryValueEx( hCU, "SendTo", NULL, &lpType, (unsigned char *)&szLink, &ulSize);
		RegCloseKey(hCU);
	}
	strcat(szLink, (LPCSTR)sLinkProg);
	::CreateShellLink(szProg, szLink, (LPSTR)(LPCSTR)sProgName, "");
		
}

// コンテキストメニュー（SendTo)の自動実行アイコンを削除する
void RmSendtoMnu(void) 
{
	// TODO: この位置にコントロール通知ハンドラ用のコードを追加してください
	char szLink[MAX_PATH];

	HKEY hCU;
	DWORD lpType;
	ULONG ulSize = MAX_PATH;
	CString sLinkProg;	// AFX_STR_LINK_PROG を読み込む

	// ****************** 多言語対応 リソーステーブルの読み込み ******************
	if(!sLinkProg.LoadString(AFX_STR_LINK_PROG))
		sLinkProg = STR_LINK_PROG;		// リソースがセットされていない時（ありえない）	
	// ****************** 多言語対応 リソーステーブルの読み込み ******************

	if (RegOpenKeyEx(HKEY_CURRENT_USER, STR_REGKEY_EXPLR_FOLDER, 0, KEY_QUERY_VALUE, &hCU) == ERROR_SUCCESS)
	{
		RegQueryValueEx( hCU, "SendTo", NULL, &lpType, (unsigned char *)&szLink, &ulSize);
		RegCloseKey(hCU);
	}
	strcat(szLink, (LPCSTR)sLinkProg);
	::remove(szLink);
	
}

// ************************************************************
// デスクトップ・アイコンの設定・削除
// 「ProgramName」というコマンドラインとなる (スイッチなし）
// (カレントユーザ)
// ************************************************************
// デスクトップにアイコンを作る
void MkDesktopMnu(void) 
{
	// TODO: この位置にコントロール通知ハンドラ用のコードを追加してください
	char szLink[MAX_PATH];
	char szProg[MAX_PATH];
	CString sProgName;	// AFX_IDS_APP_TITLE の プログラム名を読み込む
	CString sLinkProg;	// AFX_STR_LINK_PROG を読み込む

	// ****************** 多言語対応 リソーステーブルの読み込み ******************
	if(!sProgName.LoadString(AFX_IDS_APP_TITLE))
		sProgName = _ALTER_APP_TITLE;		// リソースがセットされていない時（ありえない）	
	if(!sLinkProg.LoadString(AFX_STR_LINK_PROG))
		sLinkProg = STR_LINK_PROG;		// リソースがセットされていない時（ありえない）	
	// ****************** 多言語対応 リソーステーブルの読み込み ******************

	// 実体のフルパス名
	if(!::GetModuleFileName(NULL, szProg, MAX_PATH)) return;
	HKEY hCU;
	DWORD lpType;
	ULONG ulSize = MAX_PATH;

	if (RegOpenKeyEx(HKEY_CURRENT_USER, STR_REGKEY_EXPLR_FOLDER, 0, KEY_QUERY_VALUE, &hCU) == ERROR_SUCCESS)
	{
		RegQueryValueEx( hCU, "Desktop", NULL, &lpType, (unsigned char *)&szLink, &ulSize);
		RegCloseKey(hCU);
	}
	strcat(szLink, (LPCSTR)sLinkProg);
	::CreateShellLink(szProg, szLink, (LPSTR)(LPCSTR)sProgName, "");
		
}

// デスクトップのアイコンを削除する
void RmDesktopMnu(void) 
{
	// TODO: この位置にコントロール通知ハンドラ用のコードを追加してください
	char szLink[MAX_PATH];

	HKEY hCU;
	DWORD lpType;
	ULONG ulSize = MAX_PATH;
	CString sLinkProg;	// AFX_STR_LINK_PROG を読み込む

	// ****************** 多言語対応 リソーステーブルの読み込み ******************
	if(!sLinkProg.LoadString(AFX_STR_LINK_PROG))
		sLinkProg = STR_LINK_PROG;		// リソースがセットされていない時（ありえない）	
	// ****************** 多言語対応 リソーステーブルの読み込み ******************

	if (RegOpenKeyEx(HKEY_CURRENT_USER, STR_REGKEY_EXPLR_FOLDER, 0, KEY_QUERY_VALUE, &hCU) == ERROR_SUCCESS)
	{
		RegQueryValueEx( hCU, "Desktop", NULL, &lpType, (unsigned char *)&szLink, &ulSize);
		RegCloseKey(hCU);
	}
	strcat(szLink, (LPCSTR)sLinkProg);
	::remove(szLink);
	
}


// ************************************************************
// スタートメニュー（その他）・アイコンの設定・削除
// 「ProgramName」というコマンドラインとなる (スイッチなし）
// (カレントユーザ)
// ************************************************************
// スタートメニューにアイコンを作る
void MkProgramsMnu(void) 
{
	// TODO: この位置にコントロール通知ハンドラ用のコードを追加してください
	char szLink[MAX_PATH];
	char szProg[MAX_PATH];
	CString sProgName;	// AFX_IDS_APP_TITLE の プログラム名を読み込む
	CString sLinkProg;	// AFX_STR_LINK_PROG を読み込む

	// ****************** 多言語対応 リソーステーブルの読み込み ******************
	if(!sProgName.LoadString(AFX_IDS_APP_TITLE))
		sProgName = _ALTER_APP_TITLE;		// リソースがセットされていない時（ありえない）	
	if(!sLinkProg.LoadString(AFX_STR_LINK_PROG))
		sLinkProg = STR_LINK_PROG;		// リソースがセットされていない時（ありえない）	
	// ****************** 多言語対応 リソーステーブルの読み込み ******************

	// 実体のフルパス名
	if(!::GetModuleFileName(NULL, szProg, MAX_PATH)) return;
	HKEY hCU;
	DWORD lpType;
	ULONG ulSize = MAX_PATH;

	if (RegOpenKeyEx(HKEY_CURRENT_USER, STR_REGKEY_EXPLR_FOLDER, 0, KEY_QUERY_VALUE, &hCU) == ERROR_SUCCESS)
	{
		RegQueryValueEx( hCU, "Programs", NULL, &lpType, (unsigned char *)&szLink, &ulSize);
		RegCloseKey(hCU);
	}
	strcat(szLink, (LPCSTR)sLinkProg);
	::CreateShellLink(szProg, szLink, (LPSTR)(LPCSTR)sProgName, "");
		
}

// スタートメニューのアイコンを削除する
void RmProgramsMnu(void) 
{
	// TODO: この位置にコントロール通知ハンドラ用のコードを追加してください
	char szLink[MAX_PATH];

	HKEY hCU;
	DWORD lpType;
	ULONG ulSize = MAX_PATH;
	CString sLinkProg;	// AFX_STR_LINK_PROG を読み込む

	// ****************** 多言語対応 リソーステーブルの読み込み ******************
	if(!sLinkProg.LoadString(AFX_STR_LINK_PROG))
		sLinkProg = STR_LINK_PROG;		// リソースがセットされていない時（ありえない）	
	// ****************** 多言語対応 リソーステーブルの読み込み ******************

	if (RegOpenKeyEx(HKEY_CURRENT_USER, STR_REGKEY_EXPLR_FOLDER, 0, KEY_QUERY_VALUE, &hCU) == ERROR_SUCCESS)
	{
		RegQueryValueEx( hCU, "Programs", NULL, &lpType, (unsigned char *)&szLink, &ulSize);
		RegCloseKey(hCU);
	}
	strcat(szLink, (LPCSTR)sLinkProg);
	::remove(szLink);
	
}

// ************************************************************
// アンインストール・ショートカットをスタートメニューに作成・削除
// 「ProgramName /delete」というコマンドラインとなる
// (カレントユーザ)
// ************************************************************
// アンインストール アイコンを作る
void MkUninstMnu(void) 
{
	// TODO: この位置にコントロール通知ハンドラ用のコードを追加してください
	char szLink[MAX_PATH];
	char szProg[MAX_PATH];
	CString sProgName;	// AFX_IDS_APP_TITLE の プログラム名を読み込む
	CString sLinkUninst;	// AFX_STR_LINK_UNINST を読み込む

	// ****************** 多言語対応 リソーステーブルの読み込み ******************
	if(!sProgName.LoadString(AFX_IDS_APP_TITLE))
		sProgName = _ALTER_APP_TITLE;		// リソースがセットされていない時（ありえない）	
	if(!sLinkUninst.LoadString(AFX_STR_LINK_UNINST))
		sLinkUninst = STR_LINK_UNINST;		// リソースがセットされていない時（ありえない）	
	// ****************** 多言語対応 リソーステーブルの読み込み ******************

	// 実体のフルパス名
	if(!::GetModuleFileName(NULL, szProg, MAX_PATH)) return;
	HKEY hCU;
	DWORD lpType;
	ULONG ulSize = MAX_PATH;

	if (RegOpenKeyEx(HKEY_CURRENT_USER, STR_REGKEY_EXPLR_FOLDER, 0, KEY_QUERY_VALUE, &hCU) == ERROR_SUCCESS)
	{
		RegQueryValueEx( hCU, "Programs", NULL, &lpType, (unsigned char *)&szLink, &ulSize);
		RegCloseKey(hCU);
	}
	strcat(szLink, (LPCSTR)sLinkUninst);
	::CreateShellLink(szProg, szLink, (LPSTR)(LPCSTR)sProgName, "/delete");
		
}

// アンインストール アイコンを削除する
void RmUninstMnu(void) 
{
	// TODO: この位置にコントロール通知ハンドラ用のコードを追加してください
	char szLink[MAX_PATH];

	HKEY hCU;
	DWORD lpType;
	ULONG ulSize = MAX_PATH;
	CString sLinkUninst;	// AFX_STR_LINK_UNINST を読み込む

	// ****************** 多言語対応 リソーステーブルの読み込み ******************
	if(!sLinkUninst.LoadString(AFX_STR_LINK_UNINST))
		sLinkUninst = STR_LINK_UNINST;		// リソースがセットされていない時（ありえない）	
	// ****************** 多言語対応 リソーステーブルの読み込み ******************

	if (RegOpenKeyEx(HKEY_CURRENT_USER, STR_REGKEY_EXPLR_FOLDER, 0, KEY_QUERY_VALUE, &hCU) == ERROR_SUCCESS)
	{
		RegQueryValueEx( hCU, "Programs", NULL, &lpType, (unsigned char *)&szLink, &ulSize);
		RegCloseKey(hCU);
	}
	strcat(szLink, (LPCSTR)sLinkUninst);
	::remove(szLink);
	
}

// ************************************************************
// アンインストール・ショートカットをレジストリ（コンパネ）に設定･削除
// 「ProgramName /delete」というコマンドラインとなる
// (HKLM)
// ************************************************************
// アンインストール アイコンを作る
void MkHklmUninstMnu(void) 
{
	// TODO: この位置にコントロール通知ハンドラ用のコードを追加してください
	char szProg[MAX_PATH];
	CString sRegKey;
	HKEY hCU;
	DWORD dw;
	CString sProgName;	// AFX_IDS_APP_TITLE の プログラム名を読み込む
	CString sCplName;	// AFX_STR_UNINST_CPLNAME を読み込む

	// ****************** 多言語対応 リソーステーブルの読み込み ******************
	if(!sProgName.LoadString(AFX_IDS_APP_TITLE))
		sProgName = _ALTER_APP_TITLE;		// リソースがセットされていない時（ありえない）	
	if(!sCplName.LoadString(AFX_STR_UNINST_CPLNAME))
		sCplName = STR_UNINST_CPLNAME;		// リソースがセットされていない時（ありえない）	
	// ****************** 多言語対応 リソーステーブルの読み込み ******************


	// アンインストール用のレジストリ位置の決定
	sRegKey.Format("%s\\%s", STR_REGKEY_UNINST, sProgName);

	// 実体のフルパス名
	if(!::GetModuleFileName(NULL, szProg, MAX_PATH)) return;
	// パス名に空白が入っている場合は、 " ... " とする
	sProgName = szProg;
	if(sProgName.Find(' ') != -1) sProgName.Format("\"%s\"", szProg);

	if (RegCreateKeyEx(HKEY_LOCAL_MACHINE, sRegKey,
		0, REG_NONE, REG_OPTION_NON_VOLATILE, KEY_WRITE|KEY_READ, NULL,
		&hCU, &dw) == ERROR_SUCCESS)
	{
		// コマンドラインの記述
		sProgName += " /delete";	// スイッチをつける
		RegSetValueEx( hCU, "UninstallString", 0, REG_SZ, (const unsigned char *)(LPCSTR)sProgName, sProgName.GetLength());
		// 表示名の記述
		strcpy(szProg, (LPCSTR)sCplName);
		RegSetValueEx( hCU, "DisplayName", 0, REG_SZ, (const unsigned char *)szProg, strlen(szProg));
		RegCloseKey(hCU);
	}
}

// アンインストール アイコンを削除する
void RmHklmUninstMnu(void) 
{
	// TODO: この位置にコントロール通知ハンドラ用のコードを追加してください
	HKEY hCU;
	CString sRegKey;
	CString sProgName;	// AFX_IDS_APP_TITLE の プログラム名を読み込む

	// ****************** 多言語対応 リソーステーブルの読み込み ******************
	if(!sProgName.LoadString(AFX_IDS_APP_TITLE))
		sProgName = _ALTER_APP_TITLE;		// リソースがセットされていない時（ありえない）	
	// ****************** 多言語対応 リソーステーブルの読み込み ******************

	// アンインストール用のレジストリ位置の決定
	sRegKey.Format("%s", STR_REGKEY_UNINST);

	if (RegOpenKeyEx(HKEY_LOCAL_MACHINE, sRegKey, 0, KEY_QUERY_VALUE, &hCU) == ERROR_SUCCESS)
	{
		RegDeleteKey( hCU, sProgName);
		RegCloseKey(hCU);
	}
}

// ************************************************************
// アンインストール・ショートカットをレジストリ（コンパネ）に設定･削除
// 「ProgramName /delete」というコマンドラインとなる
// (HKCU)
// ************************************************************
// アンインストール アイコンを作る
void MkHkcuUninstMnu(void) 
{
	// TODO: この位置にコントロール通知ハンドラ用のコードを追加してください
	char szProg[MAX_PATH];
	CString sRegKey;
	HKEY hCU;
	DWORD dw;
	CString sProgName;	// AFX_IDS_APP_TITLE の プログラム名を読み込む
	CString sCplName;	// AFX_STR_UNINST_CPLNAME を読み込む

	// ****************** 多言語対応 リソーステーブルの読み込み ******************
	if(!sProgName.LoadString(AFX_IDS_APP_TITLE))
		sProgName = _ALTER_APP_TITLE;		// リソースがセットされていない時（ありえない）	
	if(!sCplName.LoadString(AFX_STR_UNINST_CPLNAME))
		sCplName = STR_UNINST_CPLNAME;		// リソースがセットされていない時（ありえない）	
	// ****************** 多言語対応 リソーステーブルの読み込み ******************

	// アンインストール用のレジストリ位置の決定
	sRegKey.Format("%s\\%s", STR_REGKEY_UNINST, sProgName);

	// 実体のフルパス名
	if(!::GetModuleFileName(NULL, szProg, MAX_PATH)) return;
	// パス名に空白が入っている場合は、 " ... " とする
	sProgName = szProg;
	if(sProgName.Find(' ') != -1) sProgName.Format("\"%s\"", szProg);

	if (RegCreateKeyEx(HKEY_CURRENT_USER, sRegKey,
		0, REG_NONE, REG_OPTION_NON_VOLATILE, KEY_WRITE|KEY_READ, NULL,
		&hCU, &dw) == ERROR_SUCCESS)
	{
		// コマンドラインの記述
		sProgName += " /delete";	// スイッチをつける
		RegSetValueEx( hCU, "UninstallString", 0, REG_SZ, (const unsigned char *)(LPCSTR)sProgName, sProgName.GetLength());
		// 表示名の記述
		strcpy(szProg, (LPCSTR)sCplName);
		RegSetValueEx( hCU, "DisplayName", 0, REG_SZ, (const unsigned char *)szProg, strlen(szProg));
		RegCloseKey(hCU);
	}
}

// アンインストール アイコンを削除する
void RmHkcuUninstMnu(void) 
{
	// TODO: この位置にコントロール通知ハンドラ用のコードを追加してください
	HKEY hCU;
	CString sRegKey;
	CString sProgName;	// AFX_IDS_APP_TITLE の プログラム名を読み込む

	// ****************** 多言語対応 リソーステーブルの読み込み ******************
	if(!sProgName.LoadString(AFX_IDS_APP_TITLE))
		sProgName = _ALTER_APP_TITLE;		// リソースがセットされていない時（ありえない）	
	// ****************** 多言語対応 リソーステーブルの読み込み ******************

	// アンインストール用のレジストリ位置の決定
	sRegKey.Format("%s", STR_REGKEY_UNINST);

	if (RegOpenKeyEx(HKEY_CURRENT_USER, sRegKey, 0, KEY_QUERY_VALUE, &hCU) == ERROR_SUCCESS)
	{
		RegDeleteKey( hCU, sProgName);
		RegCloseKey(hCU);
	}
}


// ************************************************************
// インストール情報の書き込み･削除･確認
// (HKLM)
// ************************************************************
BOOL ChkHklmInstReg(void)
{
	HKEY hCU;
	DWORD data;
	DWORD lpType;
	ULONG ulSize = sizeof(DWORD);
	BOOL ret=FALSE;
	CString sProgName;	// AFX_IDS_APP_TITLE の プログラム名を読み込む

	// ****************** 多言語対応 リソーステーブルの読み込み ******************
	if(!sProgName.LoadString(AFX_IDS_APP_TITLE))
		sProgName = _ALTER_APP_TITLE;		// リソースがセットされていない時（ありえない）	
	// ****************** 多言語対応 リソーステーブルの読み込み ******************

	// HKLM\...\Run の検出
	if (RegOpenKeyEx(HKEY_LOCAL_MACHINE, STR_REGKEY_INST, 0, KEY_READ, &hCU) == ERROR_SUCCESS)
	{
		lpType = REG_DWORD;
		if(RegQueryValueEx(hCU, (LPCSTR)sProgName, NULL, &lpType, (unsigned char *)&data, &ulSize) == ERROR_SUCCESS)
			ret = TRUE;
		RegCloseKey(hCU);
	}
	return ret;
}

BOOL MkHklmInstReg(void)
{
	HKEY hCU;
	DWORD data = 1;
	DWORD lpType;
	ULONG ulSize = sizeof(DWORD);
	BOOL ret=FALSE;
	CString sProgName;	// AFX_IDS_APP_TITLE の プログラム名を読み込む

	// ****************** 多言語対応 リソーステーブルの読み込み ******************
	if(!sProgName.LoadString(AFX_IDS_APP_TITLE))
		sProgName = _ALTER_APP_TITLE;		// リソースがセットされていない時（ありえない）	
	// ****************** 多言語対応 リソーステーブルの読み込み ******************


	// HKLM\...\Run の検出
	if (RegOpenKeyEx(HKEY_LOCAL_MACHINE, STR_REGKEY_INST, 0, KEY_WRITE, &hCU) == ERROR_SUCCESS)
	{
		lpType = REG_DWORD;
		if(RegSetValueEx(hCU, (LPCSTR)sProgName, NULL, REG_DWORD, (CONST BYTE *)&data, sizeof(DWORD)) == ERROR_SUCCESS)
			ret = TRUE;
		RegCloseKey(hCU);
	}
	else
	{
		if(RegCreateKeyEx(HKEY_LOCAL_MACHINE, STR_REGKEY_INST, 0, NULL, REG_OPTION_NON_VOLATILE, KEY_WRITE, NULL, &hCU, &lpType) == ERROR_SUCCESS)
		{		
			if(RegSetValueEx(hCU, (LPCSTR)sProgName, NULL, REG_DWORD, (CONST BYTE *)&data, sizeof(DWORD)) == ERROR_SUCCESS)
				ret = TRUE;
			RegCloseKey(hCU);
		}
	}
	return ret;
}

BOOL RmHklmInstReg(void)
{
	HKEY hCU;
	BOOL ret=FALSE;
	CString sProgName;	// AFX_IDS_APP_TITLE の プログラム名を読み込む

	// ****************** 多言語対応 リソーステーブルの読み込み ******************
	if(!sProgName.LoadString(AFX_IDS_APP_TITLE))
		sProgName = _ALTER_APP_TITLE;		// リソースがセットされていない時（ありえない）	
	// ****************** 多言語対応 リソーステーブルの読み込み ******************

	if (RegOpenKeyEx(HKEY_LOCAL_MACHINE, STR_REGKEY_INST, 0, KEY_WRITE, &hCU) == ERROR_SUCCESS)
	{
		if(RegDeleteValue( hCU, sProgName) == ERROR_SUCCESS)
			ret = TRUE;
		RegCloseKey(hCU);
	}
	return ret;	
}

// ************************************************************
// インストール情報の書き込み･削除･確認
// (HKCU)
// ************************************************************
BOOL ChkHkcuInstReg(void)
{
	HKEY hCU;
	DWORD data;
	DWORD lpType;
	ULONG ulSize = sizeof(DWORD);
	BOOL ret=FALSE;
	CString sProgName;	// AFX_IDS_APP_TITLE の プログラム名を読み込む

	// ****************** 多言語対応 リソーステーブルの読み込み ******************
	if(!sProgName.LoadString(AFX_IDS_APP_TITLE))
		sProgName = _ALTER_APP_TITLE;		// リソースがセットされていない時（ありえない）	
	// ****************** 多言語対応 リソーステーブルの読み込み ******************

	// HKCU\...\Run の検出
	if (RegOpenKeyEx(HKEY_CURRENT_USER, STR_REGKEY_INST, 0, KEY_READ, &hCU) == ERROR_SUCCESS)
	{
		lpType = REG_DWORD;
		if(RegQueryValueEx(hCU, (LPCSTR)sProgName, NULL, &lpType, (unsigned char *)&data, &ulSize) == ERROR_SUCCESS)
			ret = TRUE;
		RegCloseKey(hCU);
	}
	return ret;
}

BOOL MkHkcuInstReg(void)
{
	HKEY hCU;
	DWORD data = 1;
	DWORD lpType;
	ULONG ulSize = sizeof(DWORD);
	BOOL ret=FALSE;
	CString sProgName;	// AFX_IDS_APP_TITLE の プログラム名を読み込む

	// ****************** 多言語対応 リソーステーブルの読み込み ******************
	if(!sProgName.LoadString(AFX_IDS_APP_TITLE))
		sProgName = _ALTER_APP_TITLE;		// リソースがセットされていない時（ありえない）	
	// ****************** 多言語対応 リソーステーブルの読み込み ******************


	// HKCU\...\Run の検出
	if (RegOpenKeyEx(HKEY_CURRENT_USER, STR_REGKEY_INST, 0, KEY_WRITE, &hCU) == ERROR_SUCCESS)
	{
		lpType = REG_DWORD;
		if(RegSetValueEx(hCU, (LPCSTR)sProgName, NULL, REG_DWORD, (CONST BYTE *)&data, sizeof(DWORD)) == ERROR_SUCCESS)
			ret = TRUE;
		RegCloseKey(hCU);
	}
	else
	{
		if(RegCreateKeyEx(HKEY_CURRENT_USER, STR_REGKEY_INST, 0, NULL, REG_OPTION_NON_VOLATILE, KEY_WRITE, NULL, &hCU, &lpType) == ERROR_SUCCESS)
		{		
			if(RegSetValueEx(hCU, (LPCSTR)sProgName, NULL, REG_DWORD, (CONST BYTE *)&data, sizeof(DWORD)) == ERROR_SUCCESS)
				ret = TRUE;
			RegCloseKey(hCU);
		}
	}
	return ret;
}

BOOL RmHkcuInstReg(void)
{
	HKEY hCU;
	BOOL ret=FALSE;
	CString sProgName;	// AFX_IDS_APP_TITLE の プログラム名を読み込む

	// ****************** 多言語対応 リソーステーブルの読み込み ******************
	if(!sProgName.LoadString(AFX_IDS_APP_TITLE))
		sProgName = _ALTER_APP_TITLE;		// リソースがセットされていない時（ありえない）	
	// ****************** 多言語対応 リソーステーブルの読み込み ******************

	if (RegOpenKeyEx(HKEY_CURRENT_USER, STR_REGKEY_INST, 0, KEY_WRITE, &hCU) == ERROR_SUCCESS)
	{
		if(RegDeleteValue( hCU, sProgName) == ERROR_SUCCESS)
			ret = TRUE;
		RegCloseKey(hCU);
	}
	return ret;	
}


// ************************************************************
// 各種設定レジストリを削除（アンインストール時に実行）
// (HKCU)
// ************************************************************
// ユーザ領域のレジストリデータを抹消する
void RmUserReg(void)
{
	HKEY hCU;
	CString sProgName;	// AFX_IDS_APP_TITLE の プログラム名を読み込む

	// ****************** 多言語対応 リソーステーブルの読み込み ******************
	if(!sProgName.LoadString(AFX_IDS_APP_TITLE))
		sProgName = _ALTER_APP_TITLE;		// リソースがセットされていない時（ありえない）	
	// ****************** 多言語対応 リソーステーブルの読み込み ******************

	// Windows NT では、サブキーを持ったキーは削除できないので、
	// 2段階で削除する
	if (RegOpenKeyEx(HKEY_CURRENT_USER, STR_REGKEY_USERDAT_1, 0, KEY_WRITE, &hCU) == ERROR_SUCCESS)
	{
		RegDeleteKey( hCU, "Settings");
		RegCloseKey(hCU);
	}
	if (RegOpenKeyEx(HKEY_CURRENT_USER, STR_REGKEY_USERDAT_2, 0, KEY_WRITE, &hCU) == ERROR_SUCCESS)
	{
		RegDeleteKey( hCU, sProgName);
		RegCloseKey(hCU);
	}
}

// ************************************************************
// ショートカットの作成
// ************************************************************
// ファイルへのショートカット・アイコンを作成する。
// SDK では Shell Links と言う。
// pszShortcutFile : 実体ファイルのフルパス名（C:\WINDOWS\NOTEPAD.EXE）
// pszLink : ショートカットファイルのフルパス名（ C:\WINDOWS\….lnk）
// pszDesc : 表示名
// pszParam : スイッチ（指定しないときは "" をセット)
// この項目に関しては、 MSDNライブラリから"Using Shell Links"のタイトルを検索する
// 関数の説明などは "The IShellLink Interface"にある
HRESULT CreateShellLink(LPCSTR pszShortcutFile,
							LPSTR pszLink, LPSTR pszDesc, LPSTR pszParam)
{
    HRESULT hres;
    IShellLink *psl;

	CoInitialize(NULL);	// MSDNサンプルファイルではここが抜けているので注意する

	// IShellLink オブジェクトを(pslに)得る
    hres = CoCreateInstance(CLSID_ShellLink, NULL, CLSCTX_INPROC_SERVER,
                            IID_IShellLink, (void **)&psl);
    if (SUCCEEDED(hres))
    {
       IPersistFile *ppf;
       
	   // Query IShellLink for the IPersistFile interface for
	   // saving the shortcut in persistent storage.
       hres = psl->QueryInterface(IID_IPersistFile, (void **)&ppf);
	   if (SUCCEEDED(hres))
	   {   
	        WORD wsz[MAX_PATH];   // buffer for Unicode string

	   		// 実体ファイル（ターゲット）のパスを設定
          	hres = psl->SetPath(pszShortcutFile);
/*
			if (!SUCCEEDED(hres))
				if(!n_NDispE)
					AfxMessageBox("ショートカット作成中にエラーを起こしました",
						MB_OK|MB_ICONINFORMATION|MB_TOPMOST);
*/
	   		// パラメータの設定を設定
			if(strlen(pszParam))
          		hres = psl->SetArguments(pszParam);

            // ショートカットの説明文(Description)をセット
			// 何のために使われる説明文か、不明？
          	hres = psl->SetDescription(pszDesc);
/*
			if (!SUCCEEDED(hres))
				if(!n_NDispE)
					AfxMessageBox("ショートカット作成中にエラーを起こしました",
						MB_OK|MB_ICONINFORMATION|MB_TOPMOST);
*/
	     	// ショートカットファイル名のMBCS を ワイドキャラクタ文字列へ
	     	MultiByteToWideChar(CP_ACP, 0, pszLink, -1, wsz, MAX_PATH);

          	// ショートカットファイルをディスクに書き込む
			// Save the shortcut via the IPersistFile::Save member function.
          	hres = ppf->Save(wsz, TRUE);
/*
			if (!SUCCEEDED(hres))
				if(!n_NDispE)
					AfxMessageBox("ショートカット作成中にエラーを起こしました",
						MB_OK|MB_ICONINFORMATION|MB_TOPMOST);
*/
            // Release the pointer to IPersistFile.
          	ppf->Release();
       }
       // Release the pointer to IShellLink.
       psl->Release();
    }
	CoUninitialize();
    return hres;

}

// ************************************************************
// EOF
// ************************************************************
