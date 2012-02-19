#if !defined(AFX_DLGNORM_H__32F96C42_B3C2_11D2_A7FE_004005388001__INCLUDED_)
#define AFX_DLGNORM_H__32F96C42_B3C2_11D2_A7FE_004005388001__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DlgNorm.h : ヘッダー ファイル
//

#define USER_TIMER_ID 0x315
/////////////////////////////////////////////////////////////////////////////
// CDlgNorm ダイアログ

class CDlgNorm : public CDialog
{
// コンストラクション
public:
	CDlgNorm(CWnd* pParent = NULL);   // 標準のコンストラクタ

// ダイアログ データ
	//{{AFX_DATA(CDlgNorm)
	enum { IDD = IDD_DLG_NORMAL };
	CProgressCtrl	m_Progress;
	//}}AFX_DATA


// オーバーライド
	// ClassWizard は仮想関数のオーバーライドを生成します。
	//{{AFX_VIRTUAL(CDlgNorm)
	public:
	virtual BOOL Create();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV サポート
	//}}AFX_VIRTUAL

// インプリメンテーション
protected:

	// 生成されたメッセージ マップ関数
	//{{AFX_MSG(CDlgNorm)
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ は前行の直前に追加の宣言を挿入します。

#endif // !defined(AFX_DLGNORM_H__32F96C42_B3C2_11D2_A7FE_004005388001__INCLUDED_)
