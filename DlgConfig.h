#if !defined(AFX_DLGCONFIG_H__7BD9E5C7_BD40_11D2_A7FE_004005388001__INCLUDED_)
#define AFX_DLGCONFIG_H__7BD9E5C7_BD40_11D2_A7FE_004005388001__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DlgConfig.h : ヘッダー ファイル
//

/////////////////////////////////////////////////////////////////////////////
// CDlgPDel ダイアログ

class CDlgPDel : public CPropertyPage
{
	DECLARE_DYNCREATE(CDlgPDel)

// コンストラクション
public:
	CDlgPDel();
	~CDlgPDel();

// ダイアログ データ
	//{{AFX_DATA(CDlgPDel)
	enum { IDD = IDD_DLG_PROPDEL };
	BOOL	m_Date;
	int		m_nOvwr;
	int		m_rNull;
	int		m_rZLen;
	int		m_rRen;
	//}}AFX_DATA


// オーバーライド
	// ClassWizard は仮想関数のオーバーライドを生成します。

	//{{AFX_VIRTUAL(CDlgPDel)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV サポート
	//}}AFX_VIRTUAL

// インプリメンテーション
protected:
	// 生成されたメッセージ マップ関数
	//{{AFX_MSG(CDlgPDel)
		// メモ: ClassWizard はこの位置にメンバ関数を追加します。
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

};

/////////////////////////////////////////////////////////////////////////////
// CDlgPDisp ダイアログ

class CDlgPDisp : public CPropertyPage
{
	DECLARE_DYNCREATE(CDlgPDisp)

// コンストラクション
public:
	CDlgPDisp();
	~CDlgPDisp();

// ダイアログ データ
	//{{AFX_DATA(CDlgPDisp)
	enum { IDD = IDD_DLG_PROPDISP };
	BOOL	m_Confirm;
	BOOL	m_DispF;
	BOOL	m_NDispE;
	BOOL	m_NDispN;
	BOOL	m_Log;
	//}}AFX_DATA


// オーバーライド
	// ClassWizard は仮想関数のオーバーライドを生成します。

	//{{AFX_VIRTUAL(CDlgPDisp)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV サポート
	//}}AFX_VIRTUAL

// インプリメンテーション
protected:
	// 生成されたメッセージ マップ関数
	//{{AFX_MSG(CDlgPDisp)
		// メモ: ClassWizard はこの位置にメンバ関数を追加します。
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

};
/////////////////////////////////////////////////////////////////////////////
// CDlgPOther ダイアログ

/////////////////////////////////////////////////////////////////////////////
// CDlgPOther ダイアログ

class CDlgPOther : public CPropertyPage
{
	DECLARE_DYNCREATE(CDlgPOther)

// コンストラクション
public:
	HRESULT CreateShellLink(LPCSTR pszShortcutFile, LPSTR pszLink, LPSTR pszDesc);
	CDlgPOther();
	~CDlgPOther();

// ダイアログ データ
	//{{AFX_DATA(CDlgPOther)
	enum { IDD = IDD_DLG_PROPOTHER };
	CEdit	m_ctrl_nFiles;
	CButton	m_ctrl_dmyskip;
	BOOL	m_Dummy;
	BOOL	m_DummySkip;
	int		m_nFiles;
	//}}AFX_DATA


// オーバーライド
	// ClassWizard は仮想関数のオーバーライドを生成します。

	//{{AFX_VIRTUAL(CDlgPOther)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV サポート
	//}}AFX_VIRTUAL

// インプリメンテーション
protected:
	// 生成されたメッセージ マップ関数
	//{{AFX_MSG(CDlgPOther)
	afx_msg void OnUninstall();
	afx_msg void OnVerinfo();
	afx_msg void OnCDmy();
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

};
/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ は前行の直前に追加の宣言を挿入します。
#endif //(AFX_DLGCONFIG_H__7BD9E5C7_BD40_11D2_A7FE_004005388001__INCLUDED_)
