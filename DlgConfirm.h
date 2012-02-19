#if !defined(AFX_DLGCONFIRM_H__8A9B6141_BC75_11D2_A7FE_004005388001__INCLUDED_)
#define AFX_DLGCONFIRM_H__8A9B6141_BC75_11D2_A7FE_004005388001__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DlgConfirm.h : ヘッダー ファイル
//

/////////////////////////////////////////////////////////////////////////////
// CDlgConfirm ダイアログ

class CDlgConfirm : public CDialog
{
// コンストラクション
public:
	CDlgConfirm(CWnd* pParent = NULL);   // 標準のコンストラクタ

// ダイアログ データ
	//{{AFX_DATA(CDlgConfirm)
	enum { IDD = IDD_DLG_CONFF };
	CString	m_mes;
	//}}AFX_DATA


// オーバーライド
	// ClassWizard は仮想関数のオーバーライドを生成します。
	//{{AFX_VIRTUAL(CDlgConfirm)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV サポート
	//}}AFX_VIRTUAL

// インプリメンテーション
protected:

	// 生成されたメッセージ マップ関数
	//{{AFX_MSG(CDlgConfirm)
	virtual void OnCancel();
	virtual void OnOK();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////
// CDlgInstall ダイアログ

class CDlgInstall : public CDialog
{
// コンストラクション
public:
	CDlgInstall(CWnd* pParent = NULL);   // 標準のコンストラクタ

// ダイアログ データ
	//{{AFX_DATA(CDlgInstall)
	enum { IDD = IDD_DLG_INSTALL };
	BOOL	m_CONTEXT;
	BOOL	m_DESKTOP;
	BOOL	m_MENU;
	int		m_UNIN;
	//}}AFX_DATA


// オーバーライド
	// ClassWizard は仮想関数のオーバーライドを生成します。
	//{{AFX_VIRTUAL(CDlgInstall)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV サポート
	//}}AFX_VIRTUAL

// インプリメンテーション
protected:

	// 生成されたメッセージ マップ関数
	//{{AFX_MSG(CDlgInstall)
	afx_msg void OnInsthlp();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};
//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ は前行の直前に追加の宣言を挿入します。

#endif // !defined(AFX_DLGCONFIRM_H__8A9B6141_BC75_11D2_A7FE_004005388001__INCLUDED_)
