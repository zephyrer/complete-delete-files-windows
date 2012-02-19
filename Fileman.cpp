// Fileman.cpp: CFileman クラスのインプリメンテーション
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "CompDel.h"
#include "Fileman.h"
#include "DlgNorm.h"
#include "DlgConfirm.h"

#include "PharseCmdline.h"
#include <io.h>		// _mktemp

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// 構築/消滅
//////////////////////////////////////////////////////////////////////

CFileman::CFileman()
{
}

CFileman::~CFileman()
{

}

// ************************************************************
// 各種内部変数を取り出す
// ************************************************************
CString CFileman::GetFname()
{
	return sFname;
}

CString CFileman::GetFullPath()
{
	return sFullPath;
}

CString CFileman::GetPath()
{
	return sPath;
}

// ************************************************************
// フルパス名を内部変数にセット、およびパス名の分解
// 引数 buf : ファイルのフルパス（1個のファイル）
// 戻り値 ： フォルダなどの場合 FALSE, 削除可能なとき TRUE
// ************************************************************
BOOL CFileman::SetFullPath(LPCSTR buf)
{	
	sFullPath = buf;	// フルパス名のセット

	return PharsePath();
}

// ************************************************************
// フルパス名の分解
//  フルパス → パス + ファイル名
// sPath : パス名 （例 c:\Program Files\)
// sFname : ファイル名 (例 myfile~1.txt)
// sLFName : LFNファイル名 (例 myfile text.txt)
// ※ Windows2000では、sFname と sLFName の違いは無い
//
// 戻り値 ： フォルダなどの場合 FALSE, 削除可能なとき TRUE
// SetFullPathから呼ばれる関数
// ************************************************************
BOOL CFileman::PharsePath()
{
	// 最後のルート・コロン記号の位置を探すための変数
	int last_r=-1;
	// LFN ファイル名検索用の変数
	WIN32_FIND_DATA pFind;
	HANDLE hFind;

	sPath = "";		// ファイル名だけ渡されたときのための処理
	// フォルダ名の最後を "\" と ":"の記号を目安に探す
	// 最後の "\"の位置を先頭 0とした位置で返す。したがって、1足して文字数とする。
	last_r = sFullPath.ReverseFind('\\') + 1;
	// MBCS文字との競合は、2バイト目に0x5cを含む漢字で試したところ、
	//  特別な処理をしなくても、最後の1バイトのルート記号の位置を返す。
	if(last_r <= 0) last_r = sFullPath.ReverseFind(':') + 1;

	// フォルダ名の設定。最後のルート記号までコピーする
	sPath = sFullPath.Left(last_r);
	// ファイル本体の名前の設定。
	sFname = sFullPath.Right(sFullPath.GetLength() - last_r);

	// LFN(長いファイル名)を見つけるための処理
	hFind = ::FindFirstFile(sFullPath, &pFind);
	if(hFind != INVALID_HANDLE_VALUE)
	{	// ファイルが存在する
		sLFName = pFind.cFileName;
		isExist = TRUE;
		dwFileAttr = pFind.dwFileAttributes;
	}
	else
	{	// ファイルが存在しない
		isExist = FALSE;
		dwFileAttr = 0;
		return FALSE;
	}
	::FindClose(hFind);		// 検索用ハンドルをクローズ

	// 消去対象外のファイル・フォルダ
	if((dwFileAttr & FILE_ATTRIBUTE_TEMPORARY)||
			(dwFileAttr & FILE_ATTRIBUTE_OFFLINE) ||
			(dwFileAttr & FILE_ATTRIBUTE_DIRECTORY) ||
			(dwFileAttr & FILE_ATTRIBUTE_SPARSE_FILE))
		return FALSE;

	return TRUE;
}

CString CFileman::GetLFName()
{
	return sLFName;
}

// ************************************************************
// 削除メイン関数
// 複数のファイル名を受け取ることが出来る
// 
// 引数 buf : ファイル名の羅列文字列（スペース区切り）
//       例 ： "f:\temp\aaa.txt f:\temp\bbb.txt f:\temp\ccc.txt"
// 戻り値 : TRUE:成功, FALSE:失敗(bufがNULLストリング）
// ************************************************************
BOOL CFileman::DeleteMain(LPCSTR buf)
{
	CPharseCmdline PharseCmdline;	// コマンドライン切り分けクラス
	CDlgNorm *dlg;	// モードレスダイアログ用

	PharseCmdline.SetOrgStr(buf);	// コマンドライン文字列を解析対象にセット
	nFiles = PharseCmdline.CountParam();	// 引数として与えられたパス名の数

	if(nFiles <= 0) return FALSE;
	CString sTmp, sOrgFnameLog, fbuf, dlgMes, sAfxMsg;
	
	// 最初の確認
	if(n_DispF)
	{
		CDlgConfirm dlgcnf;
		CString tmpbuf;
		::MessageBeep(MB_ICONQUESTION);
		// ****************** 多言語対応 デバックメッセージのロード ******************
		sAfxMsg.LoadString(AFX_STR_DLG_CONFIRM);	// 「%d個のファイルを削除します」
		tmpbuf.Format(sAfxMsg, nFiles);
		dlgcnf.m_mes = tmpbuf;
		if(!dlgcnf.DoModal())
		{	// キャンセルの場合
			sAfxMsg.LoadString(AFX_STR_MES_CANCEL);	// 「削除処理がキャンセルされました」
			*sLogStr = (LPCSTR)sAfxMsg;
			return TRUE;
		}
	}

	// 「削除中 ...」ダイアログの作成
	if(!n_NDispN)
	{
		dlg = new CDlgNorm();
		if(dlg->Create() != TRUE)
		{
			::MessageBox(NULL, "Your Operating System is UnStable\r\nDialog Creation Error", "Error", MB_OK|MB_ICONEXCLAMATION|MB_TOPMOST);
			n_NDispN = 1;
		}
	}

	if(n_Log)
	{	// ログの記録
		// ****************** 多言語対応 デバックメッセージのロード ******************
		sAfxMsg.LoadString(AFX_STR_LOG_INIT1);	// 「削除モード :\r\n 日付変更:%s, 」
		sTmp.Format((LPCSTR)sAfxMsg, n_Date ? "ON":"OFF");
		*sLogStr += sTmp;
		// ****************** 多言語対応 デバックメッセージのロード ******************
		sAfxMsg.LoadString(AFX_STR_LOG_INIT2);	// 「上書:%s, 上書回数:%d, サイズ変更:%s, 」
		sTmp.Format((LPCSTR)sAfxMsg, n_rNull ? "RND":"NULL", n_nOvwr, n_rZLen ? "ON":"OFF");
		*sLogStr += sTmp;
		// ****************** 多言語対応 デバックメッセージのロード ******************
		sAfxMsg.LoadString(AFX_STR_LOG_INIT3);	// 「リネーム」
		if(n_rRen == 0) *sLogStr = *sLogStr + sAfxMsg + "OFF\r\n";
		else if(n_rRen == 1) *sLogStr = *sLogStr + sAfxMsg + "ON\r\n";
		else if(n_rRen == 2)  *sLogStr = *sLogStr + sAfxMsg + "ON(Win)\r\n";

		// ****************** 多言語対応 デバックメッセージのロード ******************
		sAfxMsg.LoadString(AFX_STR_LOG_INIT4);	// 「%d 個のファイルの消去を開始\r\n-----------------------------\r\n\r\n」
		sTmp.Format((LPCSTR)sAfxMsg, nFiles);
		*sLogStr += sTmp;
	}


	OldPath = "";	// ダミーファイル作成用

	// 引数を切り分けて、ファイル名に分割し、消去関数に送る 
	for(int errN=0, i=1;;i++)
	{
		// コマンドラインを分離処理して、次のひとつのパス名を得る
		fbuf = PharseCmdline.GetNextParam();
		// 最後のパスを越えた場合は、NULL文字列が返る
		if(fbuf == "") break;

		// パス名の解析（パスとファイル名の分離と、対象外パス名の除外）
		if(!SetFullPath(fbuf))
		{	// SetFullPath - PharsePath が FALSE を返した場合
			if(!n_NDispE)
			{
				CString tmpbuf;
				// ****************** 多言語対応 デバックメッセージのロード ******************
				sAfxMsg.LoadString(AFX_STR_DLG_EXCLUDE);	// 「ファイル '%s'\nこのファイルは完全削除対象外です」
				tmpbuf.Format((LPCSTR)sAfxMsg, fbuf);
				::MessageBox(NULL, tmpbuf, "Information",
				MB_OK|MB_ICONINFORMATION|MB_TOPMOST);
				errN++;	// ダイアログ表示用のカウンタ
			}
			if(n_Log)
			{	// ログの記録
				// ****************** 多言語対応 デバックメッセージのロード ******************
				sAfxMsg.LoadString(AFX_STR_LOG_EXCLUDE);	// 「対象外 : %s\r\n」
				sTmp.Format((LPCSTR)sAfxMsg, (LPCSTR)GetFullPath());
				*sLogStr += sTmp;
			}
			continue;
		}
		// ダイアログの表示内容処理
		if(!n_NDispN)
		{
			// ダイアログにファイル名をセット
			dlg->SetDlgItemText(IDC_LFNAME, sLFName);
			// ダイアログに削除番号を表示
			// ****************** 多言語対応 デバックメッセージのロード ******************
			sAfxMsg.LoadString(AFX_STR_DLG_PRGRS);	// 「%d/%d 処理中 (エラー %d 件)」
			dlgMes.Format((LPCSTR)sAfxMsg, i, nFiles, errN);
			dlg->SetDlgItemText(IDC_MES, dlgMes);
			// ダイアログの処理済バイト数表示
			dlg->SetDlgItemText(IDC_SIZE_TICK, "");
			// ダイアログのプログレスコントロール
			dlg->m_Progress.SetPos((i*100)/nFiles);
			// ダイアログをアップデート
			dlg->RedrawWindow();
			dlg->UpdateWindow();
		}

		if(n_Log)
		{	// ログの記録
			// ファイル名変更モードでオリジナルファイル名が失われるのを防ぐ
			sOrgFnameLog = GetFullPath();
		}
		// ファイル削除
		int result_code = DeleteOneFile(dlg);
		if(result_code != -1)
		{
			if(!n_NDispE)
			{
				switch(result_code)
				{
					// ****************** 多言語対応 デバックメッセージのロード ******************
					case 0 : sAfxMsg.LoadString(AFX_STR_ERR_0);break;	// ファイル属性が得られない
					case 1 : sAfxMsg.LoadString(AFX_STR_ERR_1);break;	// ファイル属性を変更できない
					case 2 : sAfxMsg.LoadString(AFX_STR_ERR_2);break;	// ファイルを開けない
					case 3 : sAfxMsg.LoadString(AFX_STR_ERR_3);break;	// ファイルに書き込めない
					case 4 : sAfxMsg.LoadString(AFX_STR_ERR_4);break;	// ファイルの長さを切り捨てられない
					case 5 : sAfxMsg.LoadString(AFX_STR_ERR_5);break;	// ファイル名が変えられない
					case 6 : sAfxMsg.LoadString(AFX_STR_ERR_6);break;	// ファイルの日付が変更できない
					case 7 : sAfxMsg.LoadString(AFX_STR_ERR_7);break;	// ファイルを削除できない
					case 8 : sAfxMsg.LoadString(AFX_STR_ERR_8);break;	// ダミーファイルを作成できない
					case 9 : sAfxMsg.LoadString(AFX_STR_ERR_9);break;	// ダミーファイルを削除できない
					case 10 : sAfxMsg.LoadString(AFX_STR_ERR_10);break;	// ファイルサイズ2GB制限
					default : sAfxMsg.LoadString(AFX_STR_ERR_N);break;	// 未確認のエラー
				}
				// ****************** 多言語対応 デバックメッセージのロード ******************
				sTmp.LoadString(AFX_STR_DLG_ERRCAUSE);		// 「ファイルの完全削除中にエラー発生\r\n対象ファイル %s\r\n原因 : %s\r\n\r\nファイルの削除は完了しませんでした」
				dlgMes.Format((LPCSTR)sTmp, GetLFName(), (LPCSTR)sAfxMsg);
				::MessageBox(NULL, dlgMes, "Error",	MB_OK|MB_ICONSTOP|MB_TOPMOST);
			}
			errN++;	// ダイアログ表示用のカウンタ
			if(n_Log)
			{	// ログの記録
				// ****************** 多言語対応 デバックメッセージのロード ******************
				sAfxMsg.LoadString(AFX_STR_LOG_FAIL);	// 「失敗 (code=%d) : %s\r\n」
				sTmp.Format((LPCSTR)sAfxMsg, result_code, sOrgFnameLog);
				*sLogStr += sTmp;
			}
		}
		else
		{	
			if(n_Log)
			{	// ログの記録 （成功時）
				// ****************** 多言語対応 デバックメッセージのロード ******************
				sAfxMsg.LoadString(AFX_STR_LOG_SUCCESS);	// 「成功 : %s\r\n」
				sTmp.Format((LPCSTR)sAfxMsg, sOrgFnameLog);
				*sLogStr += sTmp;
			}
		}
		// ダミーファイルを書き込む（ファイル毎モード）
		if(n_Dummy && !n_DummySkip) DummyfileProcess();
		// ダイアログ表示のときの、見やすいための待ち時間
		if(!n_NDispN) ::Sleep(1000/nFiles);
	}
	// ダミーファイルを書き込む（最後に1回）
	if(n_Dummy) DummyfileProcess();
	// ダイアログの消去
	if(!n_NDispN)
	{
		dlg->DestroyWindow();
		delete dlg;
	}
	if(n_Log)
	{	// ログの記録 （成功時）
		// ****************** 多言語対応 デバックメッセージのロード ******************
		sAfxMsg.LoadString(AFX_STR_LOG_COMPLETE);	// 「\r\n終了 (成功 %d 個, 失敗+対象外 %d 個)\r\n」
		sTmp.Format((LPCSTR)sAfxMsg, i-1-errN, errN);
		*sLogStr += sTmp;
	}
	return TRUE;

}

// ファイル名の変更で使用する、偽ファイル名
char *dmyFnameCore[] = {"AWSRVR32","CMCT232","D3DRG16","INST32","JAVAPRXY",
		"MSCREATE","EXCEL8","XL8049","CP_1252","ICRSCRPT"};
char *dmyFnameExt[] = {"DLL","DAT","OCX","DRV","CAB","VBX","EXE","CAB","TMP","X3"};

// ************************************************************
// ファイルの削除（完全削除）メイン関数
// 引数 : SetFullPath() で設定されたファイルを削除する（1ファイル）
//
// 戻り値 ： -1:成功, 0～n:失敗（結果コード）
// ************************************************************
int CFileman::DeleteOneFile(CDlgNorm *dlg)
{
	CFile fp;				// ファイル
	CFileStatus fStatus;	// ファイル情報取得、日付変更に使用
	char buf[2050];			// 書き込みデータのバッファ
	int nErrTrace = 0;
	CString sAfxMsg;		// 多言語対応用 メッセージをリソースより読み込む

	srand( (unsigned)time( NULL ));

	if(n_BufferSize > 2048 || n_BufferSize < 1) n_BufferSize = 2048;	// 書き込みバッファサイズ

	try
	{
		if(!CFile::GetStatus(GetFullPath(), fStatus))
			throw 0;	// 強制的に例外処理に移る（エラーの戻り値で関数を抜ける）
		//****************************
		// ファイルサイズチェック（VC++ 2003制限、2GBytes以下）
		//****************************
		if(fStatus.m_size > (LONG)2147000000 || fStatus.m_size < 0)
		{
			nErrTrace = 10;
			throw 0;	// 強制的に例外処理に移る（エラーの戻り値で関数を抜ける）
		}
		//****************************
		// 属性変更（全属性を解除)
		//****************************
		nErrTrace = 1;
		// リードオンリー、システム、隠し属性を消去
		fStatus.m_attribute = fStatus.m_attribute & (~(0x01 | 0x02 | 0x04));
		CFile::SetStatus(GetFullPath(), fStatus); // 失敗は例外がスローされる
		// *例：attr = ::GetFileAttributes(GetFullPath()); を使用してもよい
		// ****************** 多言語対応 デバックメッセージのロードと表示 ******************
		sAfxMsg.LoadString(AFX_STR_DBG_ATTR);	// 「ファイル属性解除」
		ConfirmMsgbox((LPCSTR)sAfxMsg);
		nErrTrace = 2;
		// 属性変更（全属性を解除)ここまで
// データ上書き
		if(!CFile::GetStatus(GetFullPath(), fStatus))
			throw 0;	// 強制的に例外処理に移る（エラーの戻り値で関数を抜ける）
		// 毎回オープンして、クローズするのは、強制書き込みさせるため

		//****************************
		// 上書き
		//****************************
		srand((unsigned)time(NULL));
		for(int i=0; i<n_nOvwr; i++)
		{	// i回繰り返して上書きする
			if(!fp.Open(GetFullPath(), CFile::modeReadWrite|CFile::shareExclusive|CFile::typeBinary, NULL))
				throw 0;	// 強制的に例外処理に移る（エラーの戻り値で関数を抜ける）
			nErrTrace = 3;
			// 50MBytesを超えるファイルは、処理バイト数をダイアログに表示
			if(fStatus.m_size > (long)50*1024*1024)
			{
				sAfxMsg.Format("%ld/%ldMB", 0, fStatus.m_size/1024/1024);
				dlg->SetDlgItemText(IDC_SIZE_TICK, sAfxMsg);
				// ダイアログをアップデート
				dlg->RedrawWindow();
				dlg->UpdateWindow();
			}
			if(!n_rNull)
			{	// 書き込みバッファをゼロクリア
				for(int j=0; j<(int)n_BufferSize; j++) buf[j] = 0x0;
			}
			for(long li=0, n_Tick=50*1024*1024; li<fStatus.m_size; li+=n_BufferSize)
			{
				if(n_rNull)
				{	// 書き込みバッファの内容を乱数文字列に
					for(int j=0; j<(int)n_BufferSize; j++)
						buf[j] = (unsigned char)rand();
				}
				if(fStatus.m_size - li >= (long)n_BufferSize)
					fp.Write(buf, n_BufferSize);	// バッファサイズまで書き込み
				else
					fp.Write(buf, (UINT)(fStatus.m_size - li));	// 残りのサイズを書き込み

				// 50MBytesごとに処理バイト数をダイアログに表示
				if(li>n_Tick)
				{
					sAfxMsg.Format("%ld/%ldMB", n_Tick/1024/1024, fStatus.m_size/1024/1024);
					dlg->SetDlgItemText(IDC_SIZE_TICK, sAfxMsg);
					// ダイアログをアップデート
					dlg->RedrawWindow();
					dlg->UpdateWindow();
					n_Tick += (long)(50*1024*1024);
				}

			}

			fp.Flush();
			fp.Close();

			// ****************** デバックメッセージのロードと表示 ******************
			sAfxMsg.Format("Overwrite Done ! (Pass %d/%d)", i+1, n_nOvwr);	//
			ConfirmMsgbox((LPCSTR)sAfxMsg);

			// 空きデータ領域撹乱のための、ダミー書き込み
			// （ダミー書き込みするかどうかの判断は、関数中で行っている）
			if(n_bAntiOneShot)
			{
				// 最後の１回のみ、実行
				if(i == n_nOvwr-1) AntiCacheTempWrite((LPCSTR)GetFullPath());
			}
			else
			{
				// 毎回実行
				AntiCacheTempWrite((LPCSTR)GetFullPath());
			}
		}

		// バッファを強制的にフラッシュさせるため、ダミーデータを書き込む
		if(!fp.Open(GetFullPath(), CFile::modeReadWrite|CFile::shareExclusive|CFile::typeBinary, NULL))
			throw 0;	// 強制的に例外処理に移る（エラーの戻り値で関数を抜ける）
		fp.Write("X", 1);		// 先頭一文字 「X」を書き込む
		fp.Flush();
		fp.Close();
		// ****************** 多言語対応 デバックメッセージのロードと表示 ******************
		sAfxMsg.LoadString(AFX_STR_DBG_OVER);	// 「データの上書き消去完了」
		ConfirmMsgbox((LPCSTR)sAfxMsg);
		nErrTrace = 4;
		// データ上書き ここまで
		//****************************
		// データ書き込みのオーバーラン
		//****************************
		if(n_Overrun < 0 || n_Overrun >65536) n_Overrun = 0;
		if(n_Overrun)
		{
			if(!fp.Open(GetFullPath(), CFile::modeReadWrite|CFile::shareExclusive|CFile::typeBinary, NULL))
				throw 0;	// 強制的に例外処理に移る（エラーの戻り値で関数を抜ける）
			fp.Seek(0, CFile::end);		// ファイルの末尾へ
			if(!n_rNull)
			{	// 書き込みバッファをゼロクリア
				for(int j=0; j<2048; j++) buf[j] = 0x0;
			}
			else
			{	// 書き込みバッファの内容を乱数文字列に
				for(int j=0; j<2048; j++)
					buf[j] = (unsigned char)rand();
			}
			// 書き込み
			for(long li=0; li<n_Overrun; li+=2048)
			{
				if(n_Overrun - li >= 2048L)
					fp.Write(buf, 2048);	// バッファサイズまで書き込み
				else
					fp.Write(buf, n_Overrun - li);	// 残りのサイズを書き込み
			}

			fp.Flush();
			fp.Close();
			// ****************** 多言語対応 デバックメッセージのロードと表示 ******************
			sAfxMsg.LoadString(AFX_STR_DBG_OVERRUN);	// 「オーバーラン書き込み完了」
			ConfirmMsgbox((LPCSTR)sAfxMsg);
		}

		//****************************
		// ファイル長さをゼロに切り捨てる
		//****************************
		if(n_rZLen)
		{
			if(!fp.Open(GetFullPath(), CFile::modeCreate|CFile::modeReadWrite|CFile::shareExclusive|CFile::typeBinary, NULL))
				throw 0;	// 強制的に例外処理に移る（エラーの戻り値で関数を抜ける）
			fp.Flush();
			fp.Close();
			// ****************** 多言語対応 デバックメッセージのロードと表示 ******************
			sAfxMsg.LoadString(AFX_STR_DBG_ZEROLEN);	// 「ファイル長ゼロ」
			ConfirmMsgbox((LPCSTR)sAfxMsg);
		}
		nErrTrace = 5;
		// ファイル長さをゼロに切り捨てる ここまで
// ファイル名を変更する
		if(n_rRen)
		{
			CString sNewFullPath;
			char sNewLFName[MAX_PATH];
			srand((unsigned)time(NULL));
			// ①ＬＦＮ対応、現在のファイル名を完全消去のためのファイル名変更
			// 最低15文字のファイル名とする
			for(int i=0;i<10;i++)
			{	// すでにあるファイル名に重ならないための重複検査
				WIN32_FIND_DATA pFind;
				HANDLE hFind;
				strcpy(sNewLFName, GetLFName());
				int si;
				for(si=0; (unsigned char)sNewLFName[si]; si++)
					if(sNewLFName[si] == '.') break;
				if(si<7)
				{
					for(int i=0; i<8; i++)
						sNewLFName[i] = (unsigned char)GenerateRandom(0x41,0x5a);
					sNewLFName[8] = (char)0;
					strcat(sNewLFName, ".TMP");
				}
				else
				{
					for(int i=0; sNewLFName[i]; i++)
					{
						if(sNewLFName[i]!='.')
							sNewLFName[i] = (unsigned char)GenerateRandom(0x41,0x5a);
					}
				}

				sNewFullPath = GetPath() + sNewLFName;
				hFind = ::FindFirstFile(sNewFullPath, &pFind);
				if(hFind == INVALID_HANDLE_VALUE)
				{	// ファイルが見付からなければ、このファイル名に決定
					::FindClose(hFind);		// 検索用ハンドルをクローズ
					break;
				}
				else ::FindClose(hFind);	// 検索用ハンドルをクローズ
			}
			if(i<9)
			{	// ファイル名がすべて使用済みのとき名前変更をあきらめる
				CFile::Rename(GetFullPath(), sNewFullPath);
				SetFullPath(sNewFullPath);	// ファイル名データを書き換える
				// ****************** 多言語対応 デバックメッセージのロードと表示 ******************
				sAfxMsg.LoadString(AFX_STR_DBG_FNAME);	// 「ファイル名を撹乱（ＬＦＮ上書き）完了」
				ConfirmMsgbox((LPCSTR)sAfxMsg);
			}
			// ②Windowsに似せた名前に変更
			if(n_rRen == 2)
			{
				for(int i=0;i<10;i++)
				{	// すでにあるファイル名に重ならないための重複検査
					WIN32_FIND_DATA pFind;
					HANDLE hFind;
					CString dmyFname;
					sprintf(sNewLFName, "%s.%s", dmyFnameCore[GenerateRandom(0,9)],
						dmyFnameExt[GenerateRandom(0,9)]);
					dmyFname = GetPath() + sNewLFName;
					hFind = ::FindFirstFile(dmyFname, &pFind);
					if(hFind == INVALID_HANDLE_VALUE)
					{	// ファイルが見付からなければ、このファイル名に決定
						::FindClose(hFind);		// 検索用ハンドルをクローズ
						break;
					}
					else ::FindClose(hFind);	// 検索用ハンドルをクローズ
				}
				if(i<9)
				{	// ファイル名がすべて使用済みのとき名前変更をあきらめる
					sNewFullPath = GetPath() + sNewLFName;
					CFile::Rename(GetFullPath(), sNewFullPath);
					SetFullPath(sNewFullPath);	// ファイル名データを書き換える
				}
				// ****************** 多言語対応 デバックメッセージのロードと表示 ******************
				sAfxMsg.LoadString(AFX_STR_DBG_WINNAME);	// 「ファイル名を撹乱（Windowsファイル似）完了」
				ConfirmMsgbox((LPCSTR)sAfxMsg);
			}
		}
		nErrTrace = 6;
		// ファイル名を変更する ここまで
// ファイルの日付を変更する
		if(n_Date)
		{
			if(!CFile::GetStatus(GetFullPath(), fStatus))
				throw 0;	// 強制的に例外処理に移る（エラーの戻り値で関数を抜ける）
			CTime dummyTime(1998,1,1,0,0,2);
			fStatus.m_ctime = fStatus.m_atime = fStatus.m_mtime = dummyTime;
			CFile::SetStatus(GetFullPath(), fStatus); // 失敗は例外がスローされる
			// ****************** 多言語対応 デバックメッセージのロードと表示 ******************
			sAfxMsg.LoadString(AFX_STR_DBG_DATE);	// 「ファイルの日付変更完了」
			ConfirmMsgbox((LPCSTR)sAfxMsg);
		}
		nErrTrace = 7;
		// ファイルの日付を変更する ここまで

// ファイルを削除する
		if(!b_NotRemove)
		{
			CFile::Remove(GetFullPath());
			// ****************** 多言語対応 デバックメッセージのロードと表示 ******************
			sAfxMsg.LoadString(AFX_STR_DBG_DELETE);	// 「ファイル削除完了」
			ConfirmMsgbox((LPCSTR)sAfxMsg);
			nErrTrace = 8;
		}
		// ファイルを削除する ここまで
	}
	catch(CFileException *e)
	{	// ファイル処理中のエラー
		
		if(e->m_cause==CFileException::hardIO)
		{
			::MessageBox(NULL, "File I/O Error (Hardware Error)", "Error", MB_OK|MB_ICONSTOP|MB_TOPMOST);
		}
		if (fp.m_hFile != CFile::hFileNull){ fp.Close(); }		// ファイルが開いていれば、閉じる
		return(nErrTrace);
	}
	catch(...)
	{	// その他のエラーすべてを受け付ける
		if (fp.m_hFile != CFile::hFileNull){ fp.Close(); }		// ファイルが開いていれば、閉じる
		return(nErrTrace);
	}
	return(-1);
}

// ************************************************************
// 乱数の作成（一定範囲内）
// 引数 : min, max (乱数の下限値、上限値）
//
// 戻り値 ： min ～ max の乱数を返す (上下限は含む）
// ************************************************************
unsigned int CFileman::GenerateRandom(unsigned int min, unsigned int max)
{
	unsigned int i=0;
	do
	{
		i = ((rand() & 0xff) * (max-min+1))/0xff + min;
	}while(i<min || i>max);

	return i;
}

void CFileman::ConfirmMsgbox(CString mes)
{
	if(!n_Confirm) return;
	CString buf, sAfxMsg, sAfxMsg2;
	// ****************** 多言語対応 デバックメッセージのロードと表示 ******************
	sAfxMsg.LoadString(AFX_STR_DLG_PRGRS_2);	// 「ﾌｧｲﾙ「%s」の削除処理中\n%s」
	buf.Format((LPCSTR)sAfxMsg, GetLFName(), mes);
	sAfxMsg2.LoadString(AFX_STR_DLG_PRGRS_3);	// 「完全削除処理状況」
	::MessageBox(NULL, buf, (LPCSTR)sAfxMsg2, MB_OK|MB_ICONINFORMATION);
}

// ************************************************************
// フォルダ内に複数のダミーファイルを作成し、消去する
// フォルダの削除痕跡の会席を逃れるための、手段
//
// ************************************************************
BOOL CFileman::DummyfileProcess()
{
	CFile fp;				// ファイル
	char sNewLFName[MAX_PATH];
	char buf[MAX_PATH];
	CString sTmp, sAfxMsg, dlgMes;
	CString sDummyString = "*************** THIS IS COMPDEL DUMMY FILE ***************\r\n";

// 撹乱ファイルを多数作成する
	try
	{
		WIN32_FIND_DATA pFind;
		HANDLE hFind;
		CString dmyFname;
		// ダミーファイルの先頭5文字を決定する
		for(int i=0; i<4; i++)
			sNewLFName[i] = (unsigned char)GenerateRandom(0x61,0x7a);
		sNewLFName[i] = 0x0;
		// 検索用ファイル名を作成
		sprintf(buf, "%s*.*", sNewLFName);
		dmyFname = GetPath() + buf;
		hFind = ::FindFirstFile(dmyFname, &pFind);
		if(hFind != INVALID_HANDLE_VALUE)
		{	// ダミーファイル名と既存ファイル名が一致するとき
			::FindClose(hFind);		// 検索用ハンドルをクローズ
			return FALSE;
		}
		::FindClose(hFind);		// 検索用ハンドルをクローズ
		// ダミーファイル作成
		for(i=0; i<n_nFiles; i++)
		{
			sprintf(buf, "%s%02d.tmp", sNewLFName, i);
			dmyFname = GetPath() + buf;
			if(!fp.Open(dmyFname, CFile::modeCreate|CFile::modeReadWrite|CFile::shareExclusive|CFile::typeBinary, NULL))
			{
				::MessageBox(NULL, "Dummy File I/O Error (Hardware Error)", "Error", MB_OK|MB_ICONSTOP|MB_TOPMOST);
				return FALSE;
			}
			try{
				// Ver 1.36 で追加。（NTFSのMFT 撹乱用)
				fp.Write(sDummyString,sDummyString.GetLength());
			}
			catch(CFileException *e)
			{	// ファイル処理中のエラー
				if(e->m_cause==CFileException::hardIO)
				{
					::MessageBox(NULL, "Dummy File I/O Error (Hardware Error)", "Error", MB_OK|MB_ICONSTOP|MB_TOPMOST);
				}
				fp.Close();
				return FALSE;
			}
			fp.Close();
		}
		// ****************** 多言語対応 デバックメッセージのロードと表示 ******************
		sAfxMsg.LoadString(AFX_STR_DBG_MKDUMMY);	// 「ダミーファイルを作成」
		ConfirmMsgbox((LPCSTR)sAfxMsg);

		// ダミーファイル消去
		for(i=0; i<n_nFiles; i++)
		{
			sprintf(buf, "%s%02d.tmp", sNewLFName, i);
			dmyFname = GetPath() + buf;
			CFile::Remove(dmyFname);
		}
		// ****************** 多言語対応 デバックメッセージのロードと表示 ******************
		sAfxMsg.LoadString(AFX_STR_DBG_DLDUMMY);	// 「ダミーファイルを消去」
		ConfirmMsgbox((LPCSTR)sAfxMsg);

		// 撹乱ファイルを多数作成する ここまで
		if(n_Log)
		{	// ログの記録 （成功時）
			// ****************** 多言語対応 デバックメッセージのロード ******************
			sAfxMsg.LoadString(AFX_STR_LOG_DUMMY);	// 「 ダミーファイルの作成 (%d 個)\r\n」
			sTmp.Format((LPCSTR)sAfxMsg, n_nFiles);
			*sLogStr += sTmp;
		}
	}
	catch(CFileException *e)
	{	// ファイル処理中のエラー
		
		if(e->m_cause==CFileException::hardIO)
		{
			::MessageBox(NULL, "File I/O Error (Hardware Error)", "Error", MB_OK|MB_ICONSTOP|MB_TOPMOST);
		}
		else
		{
			// ****************** 多言語対応 デバックメッセージのロード ******************
			sAfxMsg.LoadString(AFX_STR_ERR_8);	// ダミーファイルを作成できない
			sTmp.LoadString(AFX_STR_DLG_ERRCAUSE);		// 「ファイルの完全削除中にエラー発生\r\n対象ファイル %s\r\n原因 : %s\r\n\r\nファイルの削除は完了しませんでした」
			dlgMes.Format((LPCSTR)sTmp, GetLFName(), (LPCSTR)sAfxMsg);
			::MessageBox(NULL, dlgMes, "Error",	MB_OK|MB_ICONSTOP|MB_TOPMOST);
		}
		return FALSE;
	}
	return TRUE;
}

// ************************************************************
// キャッシュ機構を無効にするための、テンポラリファイルアクセス
// 
//
// ************************************************************
BOOL CFileman::AntiCacheTempWrite(const char *fname)
{
	CFile fp;				// ファイル
	CFileStatus fStatus;	// ファイルの存在の確認用
	char f_path_buffer[_MAX_PATH];	// 出来上がったテンポラリファイル名
	char f_drive[_MAX_DRIVE];		// ドライブ
	char f_dir[_MAX_DIR];			// フォルダ
	char f_body[10];				// ファイル名（ボディ）
	char buf[2050];			// 書き込みデータのバッファ
	long li;				// 書き込みループ カウンタ
	long tempsize = (long)n_UnticacheSize*1024L*1024L;	// テンポラリファイルのサイズ
	CString sAfxMsg;		// 多言語対応用 メッセージをリソースより読み込む

	if(!b_AntiCache) return TRUE;

	//****************************
	// テンポラリファイル名を作成
	//****************************
	while(1)
	{
		srand((unsigned)time(NULL));
		strcpy(f_body, "_c");	// 先頭2文字 
		for(int j=0; j<6; j++)
			f_body[j+2] = (unsigned char)GenerateRandom(0x30,0x39);		// 0 ～ 9 の文字
		f_body[8] = (char)NULL;	// 文字列最後

		if(n_UntiFolder == 0)
		{	// 削除対象ファイルのフォルダ
			_splitpath(fname, f_drive, f_dir, NULL, NULL);
			_makepath(f_path_buffer, f_drive, f_dir, f_body,".tmp");
		}
		else
		{	// システムのテンポラリフォルダ
			::GetTempPath(_MAX_DIR, f_dir);
			if(f_dir[strlen(f_dir)-1] == '\\')
				sprintf(f_path_buffer, "%s%s.tmp",f_dir, f_body);
			else
				sprintf(f_path_buffer, "%s\\%s.tmp",f_dir, f_body);
		}
		if(!CFile::GetStatus(f_path_buffer, fStatus))
			break;		// ファイルが存在していなければ、ファイル名確定。ループ抜け

	}

	//****************************
	// テンポラリファイルに書き込み
	//****************************
	srand((unsigned)time(NULL));
	try
	{

		for(int i=0; i<2; i++)
		{	// 2回繰り返して上書きする
			if(!fp.Open(f_path_buffer, CFile::modeCreate|CFile::modeWrite|CFile::shareExclusive|CFile::typeBinary, NULL))
				return FALSE;
			for(li=0; li<tempsize; li+=2048)
			{
				// 書き込みバッファの内容を乱数文字列に
				for(int j=0; j<2048; j++)
					buf[j] = (unsigned char)rand();
				if(tempsize - li >= (long)2048)
					fp.Write(buf, 2048);	// バッファサイズまで書き込み
				else
					fp.Write(buf, tempsize - li);	// 残りのサイズを書き込み
			}

			fp.Flush();
			fp.Close();
		}
	}
	catch(CFileException *e)
	{	// ファイル処理中のエラー
		
		if(e->m_cause==CFileException::hardIO)
		{
			// ****************** 多言語対応 デバックメッセージのロードと表示 ******************
			sAfxMsg.LoadString(AFX_STR_ERR_ANTICACHE);	// 「アンチ・キャッシュ用テンポラリファイルを作成できません」
			ConfirmMsgbox((LPCSTR)sAfxMsg);
		}
		return FALSE;
	}
	// ****************** 多言語対応 デバックメッセージのロードと表示 ******************
	sAfxMsg.LoadString(AFX_STR_DBG_ANTICACHE);	// 「アンチ・キャッシュ用テンポラリファイルを作成」
	ConfirmMsgbox((LPCSTR)sAfxMsg);

	// テンポラリファイルの消去
	CFile::Remove(f_path_buffer);

	return TRUE;
}
