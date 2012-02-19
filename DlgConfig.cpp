// DlgConfig.cpp : インプリメンテーション ファイル
//

#include "stdafx.h"
#include "CompDel.h"
#include "DlgConfig.h"
#include "DlgAbout.h"
#include "DlgConfirm.h"

#include "InstFunc.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDlgPDel プロパティ ページ

IMPLEMENT_DYNCREATE(CDlgPDel, CPropertyPage)

CDlgPDel::CDlgPDel() : CPropertyPage(CDlgPDel::IDD)
{
	//{{AFX_DATA_INIT(CDlgPDel)
	m_Date = FALSE;
	m_nOvwr = 0;
	m_rNull = -1;
	m_rZLen = -1;
	m_rRen = -1;
	//}}AFX_DATA_INIT
}

CDlgPDel::~CDlgPDel()
{
}

void CDlgPDel::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgPDel)
	DDX_Check(pDX, IDC_C_DATE, m_Date);
	DDX_Text(pDX, IDC_EDIT_NTIMES, m_nOvwr);
	DDV_MinMaxInt(pDX, m_nOvwr, 1, 5);
	DDX_Radio(pDX, IDC_R1_NULL, m_rNull);
	DDX_Radio(pDX, IDC_R2_ZLEN, m_rZLen);
	DDX_Radio(pDX, IDC_R3_NNAME, m_rRen);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDlgPDel, CPropertyPage)
	//{{AFX_MSG_MAP(CDlgPDel)
		// メモ: ClassWizard はこの位置に DDX および DDV の呼び出しコードを追加します。
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgPDel メッセージ ハンドラ
/////////////////////////////////////////////////////////////////////////////
// CDlgPDisp プロパティ ページ

IMPLEMENT_DYNCREATE(CDlgPDisp, CPropertyPage)

CDlgPDisp::CDlgPDisp() : CPropertyPage(CDlgPDisp::IDD)
{
	//{{AFX_DATA_INIT(CDlgPDisp)
	m_Confirm = FALSE;
	m_DispF = FALSE;
	m_NDispE = FALSE;
	m_NDispN = FALSE;
	m_Log = FALSE;
	//}}AFX_DATA_INIT
}

CDlgPDisp::~CDlgPDisp()
{
}

void CDlgPDisp::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgPDisp)
	DDX_Check(pDX, IDC_C_DISPC, m_Confirm);
	DDX_Check(pDX, IDC_C_DISPF, m_DispF);
	DDX_Check(pDX, IDC_C_NDISPE, m_NDispE);
	DDX_Check(pDX, IDC_C_NDISPN, m_NDispN);
	DDX_Check(pDX, IDC_C_DISPF2, m_Log);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDlgPDisp, CPropertyPage)
	//{{AFX_MSG_MAP(CDlgPDisp)
		// メモ: ClassWizard はこの位置に DDX および DDV の呼び出しコードを追加します。
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgPDisp メッセージ ハンドラ
/////////////////////////////////////////////////////////////////////////////
// CDlgPOther プロパティ ページ

IMPLEMENT_DYNCREATE(CDlgPOther, CPropertyPage)

CDlgPOther::CDlgPOther() : CPropertyPage(CDlgPOther::IDD)
{
	//{{AFX_DATA_INIT(CDlgPOther)
	m_Dummy = FALSE;
	m_DummySkip = FALSE;
	m_nFiles = 0;
	//}}AFX_DATA_INIT
}

CDlgPOther::~CDlgPOther()
{
}

void CDlgPOther::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgPOther)
	DDX_Control(pDX, IDC_EDIT_NFILES, m_ctrl_nFiles);
	DDX_Control(pDX, IDC_C_DMYSKIP, m_ctrl_dmyskip);
	DDX_Check(pDX, IDC_C_DMY, m_Dummy);
	DDX_Check(pDX, IDC_C_DMYSKIP, m_DummySkip);
	DDX_Text(pDX, IDC_EDIT_NFILES, m_nFiles);
	DDV_MinMaxInt(pDX, m_nFiles, 1, 99);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDlgPOther, CPropertyPage)
	//{{AFX_MSG_MAP(CDlgPOther)
	ON_BN_CLICKED(IDC_UNINSTALL, OnUninstall)
	ON_BN_CLICKED(IDC_VERINFO, OnVerinfo)
	ON_BN_CLICKED(IDC_C_DMY, OnCDmy)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgPOther メッセージ ハンドラ

void CDlgPOther::OnUninstall() 
{
	// TODO: この位置にコントロール通知ハンドラ用のコードを追加してください
	CString sAfxMsg;
	::RmUninstMnu();
	::MkUninstMnu();	// スタートメニューにアンインストールメニューを追加

	sAfxMsg.LoadString(AFX_STR_DLG_MKUNIN);	// アンインストール用のショートカットを作成しました
	MessageBox((LPCSTR)sAfxMsg, "CompleteDelete Information", MB_ICONINFORMATION|MB_OK);
}

void CDlgPOther::OnVerinfo() 
{
	// TODO: この位置にコントロール通知ハンドラ用のコードを追加してください
	// バージョン情報の表示
	CDlgAbout dlg;
	dlg.DoModal();
}


// コントロールを灰色表示にする（無効にする）処理
void CDlgPOther::OnCDmy() 
{
	// TODO: この位置にコントロール通知ハンドラ用のコードを追加してください
	if(!IsDlgButtonChecked(IDC_C_DMY))
	{
		m_ctrl_dmyskip.EnableWindow(FALSE);
		m_ctrl_nFiles.EnableWindow(FALSE);
	}
	else
	{
		m_ctrl_dmyskip.EnableWindow(TRUE);
		m_ctrl_nFiles.EnableWindow(TRUE);
	}
}

BOOL CDlgPOther::OnInitDialog() 
{
	CPropertyPage::OnInitDialog();
	
	// TODO: この位置に初期化の補足処理を追加してください
	if(!IsDlgButtonChecked(IDC_C_DMY))
	{
		m_ctrl_dmyskip.EnableWindow(FALSE);	
		m_ctrl_nFiles.EnableWindow(FALSE);
	}
	return TRUE;  // コントロールにフォーカスを設定しないとき、戻り値は TRUE となります
	              // 例外: OCX プロパティ ページの戻り値は FALSE となります
}
