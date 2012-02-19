#if !defined(AFX_DLGHELPDOC_H__CA24037A_1942_4BC7_B454_6F438AAEC9BF__INCLUDED_)
#define AFX_DLGHELPDOC_H__CA24037A_1942_4BC7_B454_6F438AAEC9BF__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DlgHelpDoc.h : ヘッダー ファイル
//

/////////////////////////////////////////////////////////////////////////////
// CDlgHelpDoc ダイアログ

class CDlgHelpDoc : public CDialog
{
// コンストラクション
public:
	CDlgHelpDoc(CWnd* pParent = NULL);   // 標準のコンストラクタ

// ダイアログ データ
	//{{AFX_DATA(CDlgHelpDoc)
	enum { IDD = IDD_DLG_HELPDOC };
	CString	m_edit_main;
	//}}AFX_DATA


// オーバーライド
	// ClassWizard は仮想関数のオーバーライドを生成します。
	//{{AFX_VIRTUAL(CDlgHelpDoc)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV サポート
	//}}AFX_VIRTUAL

// インプリメンテーション
protected:

	// 生成されたメッセージ マップ関数
	//{{AFX_MSG(CDlgHelpDoc)
		// メモ: ClassWizard はこの位置にメンバ関数を追加します。
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ は前行の直前に追加の宣言を挿入します。

#endif // !defined(AFX_DLGHELPDOC_H__CA24037A_1942_4BC7_B454_6F438AAEC9BF__INCLUDED_)
