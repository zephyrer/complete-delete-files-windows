// CompDel.h : COMPDEL アプリケーションのメイン ヘッダー ファイルです。
//

#if !defined(AFX_COMPDEL_H__13E1F064_ADF6_11D2_8D63_004005388001__INCLUDED_)
#define AFX_COMPDEL_H__13E1F064_ADF6_11D2_8D63_004005388001__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// メイン シンボル
#include "Fileman.h"

/////////////////////////////////////////////////////////////////////////////
// CCompDelApp:
// このクラスの動作の定義に関しては CompDel.cpp ファイルを参照してください。
//

class CCompDelApp : public CWinApp
{
public:
	BOOL QuickDeleteExec(CString sFileName);
	CString sLogStr;
	CCompDelApp();

// オーバーライド
	// ClassWizard は仮想関数のオーバーライドを生成します。
	//{{AFX_VIRTUAL(CCompDelApp)
	public:
	virtual BOOL InitInstance();
//	virtual void WinHelp(DWORD dwData, UINT nCmd = HELP_CONTEXT);
//}}AFX_VIRTUAL

// インプリメンテーション

	//{{AFX_MSG(CCompDelApp)
		// メモ - ClassWizard はこの位置にメンバ関数を追加または削除します。
		//        この位置に生成されるコードを編集しないでください。
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
protected:
	BOOL ProcessCommandline();
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ は前行の直前に追加の宣言を挿入します。

#endif // !defined(AFX_COMPDEL_H__13E1F064_ADF6_11D2_8D63_004005388001__INCLUDED_)
