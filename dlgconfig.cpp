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
	DDV_MinMaxInt(pDX, m_nOvwr, 1, 50);
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
	//}}AFX_DATA_INIT
}

CDlgPOther::~CDlgPOther()
{
}

void CDlgPOther::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgPOther)
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDlgPOther, CPropertyPage)
	//{{AFX_MSG_MAP(CDlgPOther)
	ON_BN_CLICKED(IDC_UNINSTALL, OnUninstall)
	ON_BN_CLICKED(IDC_VERINFO, OnVerinfo)
	ON_BN_CLICKED(IDC_BTN_QFILE_BROWSE, OnBtnQfileBrowse)
	ON_BN_CLICKED(IDC_BTN_QEXEC, OnBtnQexec)
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


/////////////////////////////////////////////////////////////////////////////
// CDlgPDel2 プロパティ ページ

IMPLEMENT_DYNCREATE(CDlgPDel2, CPropertyPage)

CDlgPDel2::CDlgPDel2() : CPropertyPage(CDlgPDel2::IDD)
{
	//{{AFX_DATA_INIT(CDlgPDel2)
	m_nBufferSize = 0;
	m_nOverrun = 0;
	m_nDodChar = 0;
	m_bNotRemove = FALSE;
	//}}AFX_DATA_INIT
}

CDlgPDel2::~CDlgPDel2()
{
}

void CDlgPDel2::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgPDel2)
	DDX_Text(pDX, IDC_EDIT_BUFFER, m_nBufferSize);
	DDV_MinMaxUInt(pDX, m_nBufferSize, 1, 2048);
	DDX_Text(pDX, IDC_EDIT_OVERRUN, m_nOverrun);
	DDV_MinMaxUInt(pDX, m_nOverrun, 0, 65536);
	DDX_Text(pDX, IDC_EDIT_DODCHAR, m_nDodChar);
	DDV_MinMaxUInt(pDX, m_nDodChar, 0, 255);
	DDX_Check(pDX, IDC_C_NOTREMOVE, m_bNotRemove);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDlgPDel2, CPropertyPage)
	//{{AFX_MSG_MAP(CDlgPDel2)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgPDel2 メッセージ ハンドラ



/////////////////////////////////////////////////////////////////////////////
// CDlgPDel3 プロパティ ページ

IMPLEMENT_DYNCREATE(CDlgPDel3, CPropertyPage)

CDlgPDel3::CDlgPDel3() : CPropertyPage(CDlgPDel3::IDD)
{
	//{{AFX_DATA_INIT(CDlgPDel3)
	m_AntiCache = FALSE;
	m_rAntiFolder = -1;
	m_nAnticacheSize = 0;
	m_bDummy = FALSE;
	m_bDummySkip = FALSE;
	m_nFiles = 0;
	m_bAntiOneShot = FALSE;
	//}}AFX_DATA_INIT
}

CDlgPDel3::~CDlgPDel3()
{
}

void CDlgPDel3::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgPDel3)
	DDX_Control(pDX, IDC_C_ANTI_ONETIME, m_ctrl_bAntiOneShot);
	DDX_Control(pDX, IDC_C_DMYSKIP, m_ctrl_bDummySkip);
	DDX_Control(pDX, IDC_EDIT_DUMMY_NFILES, m_ctrl_nFiles);
	DDX_Control(pDX, IDC_EDIT_ANTISIZE, m_ctrl_nAnticacheSize);
	DDX_Control(pDX, IDC_R1_ANTIDIR2, m_ctrl_rAntiFolder2);	// 手動で追加
	DDX_Control(pDX, IDC_R1_ANTIDIR, m_ctrl_rAntiFolder);
	DDX_Check(pDX, IDC_C_ANTICACHE, m_AntiCache);
	DDX_Radio(pDX, IDC_R1_ANTIDIR, m_rAntiFolder);
	DDX_Text(pDX, IDC_EDIT_ANTISIZE, m_nAnticacheSize);
	DDV_MinMaxUInt(pDX, m_nAnticacheSize, 1, 100);
	DDX_Check(pDX, IDC_C_DMY, m_bDummy);
	DDX_Check(pDX, IDC_C_DMYSKIP, m_bDummySkip);
	DDX_Text(pDX, IDC_EDIT_DUMMY_NFILES, m_nFiles);
	DDV_MinMaxUInt(pDX, m_nFiles, 1, 100);
	DDX_Check(pDX, IDC_C_ANTI_ONETIME, m_bAntiOneShot);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDlgPDel3, CPropertyPage)
	//{{AFX_MSG_MAP(CDlgPDel3)
	ON_BN_CLICKED(IDC_C_ANTICACHE, OnCAnticache)
	ON_BN_CLICKED(IDC_C_DMY, OnCDmy)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgPDel3 メッセージ ハンドラ

void CDlgPDel3::OnCAnticache() 
{
	// TODO: この位置にコントロール通知ハンドラ用のコードを追加してください
	if(!IsDlgButtonChecked(IDC_C_ANTICACHE))
	{
		m_ctrl_rAntiFolder.EnableWindow(FALSE);
		m_ctrl_rAntiFolder2.EnableWindow(FALSE);
		m_ctrl_nAnticacheSize.EnableWindow(FALSE);
		m_ctrl_bAntiOneShot.EnableWindow(FALSE);
	}
	else
	{
		m_ctrl_rAntiFolder.EnableWindow(TRUE);
		m_ctrl_rAntiFolder2.EnableWindow(TRUE);
		m_ctrl_nAnticacheSize.EnableWindow(TRUE);
		m_ctrl_bAntiOneShot.EnableWindow(TRUE);
	}
	
}

BOOL CDlgPDel3::OnInitDialog() 
{
	CPropertyPage::OnInitDialog();
	
	// TODO: この位置に初期化の補足処理を追加してください
	if(!IsDlgButtonChecked(IDC_C_ANTICACHE))
	{
		m_ctrl_rAntiFolder.EnableWindow(FALSE);
		m_ctrl_rAntiFolder2.EnableWindow(FALSE);
		m_ctrl_nAnticacheSize.EnableWindow(FALSE);
		m_ctrl_bAntiOneShot.EnableWindow(FALSE);
	}

	if(!IsDlgButtonChecked(IDC_C_DMY))
	{
		m_ctrl_bDummySkip.EnableWindow(FALSE);	
		m_ctrl_nFiles.EnableWindow(FALSE);
	}

	return TRUE;  // コントロールにフォーカスを設定しないとき、戻り値は TRUE となります
	              // 例外: OCX プロパティ ページの戻り値は FALSE となります
}

void CDlgPDel3::OnCDmy() 
{
	// TODO: この位置にコントロール通知ハンドラ用のコードを追加してください
	if(!IsDlgButtonChecked(IDC_C_DMY))
	{
		m_ctrl_bDummySkip.EnableWindow(FALSE);
		m_ctrl_nFiles.EnableWindow(FALSE);
	}
	else
	{
		m_ctrl_bDummySkip.EnableWindow(TRUE);
		m_ctrl_nFiles.EnableWindow(TRUE);
	}
}

void CDlgPOther::OnBtnQfileBrowse() 
{
	// TODO: この位置にコントロール通知ハンドラ用のコードを追加してください
	OPENFILENAME ofn;
	char f_path_buffer[_MAX_PATH];
	ZeroMemory(f_path_buffer, sizeof(_MAX_PATH-1));


	// Initialize OPENFILENAME
	ZeroMemory(&ofn, sizeof(OPENFILENAME));
	ofn.lStructSize = sizeof(OPENFILENAME);
	ofn.hwndOwner = this->m_hWnd;
	ofn.lpstrFile = f_path_buffer;
	ofn.nMaxFile = _MAX_PATH;
	ofn.lpstrFilter = "Text File (*.txt;*.asc)\0*.txt;*.asc\0Word Document (*.doc;*.dot)\0*.doc;*.dot\0Excel Workbook (*.xl*)\0*.xl*\0HTML file (*.htm;*.html)\0*.htm;*.html\0All Files (*.*)\0*.*\0\0";
	ofn.nFilterIndex = 5;
	ofn.lpstrFileTitle = f_path_buffer;
	ofn.nMaxFileTitle = _MAX_PATH;
	ofn.lpstrInitialDir = NULL;
	ofn.lpstrTitle = "Select Delete File";
	ofn.Flags = OFN_FILEMUSTEXIST;

	// 「ファイル」ダイアログを表示する
	if(GetSaveFileName(&ofn) != 0)
	{
		// 入力ボックスにファイル名を設定する
		SetDlgItemText(IDC_EDIT_QUICK_FNAME, ofn.lpstrFile);
	}
	
}

void CDlgPOther::OnBtnQexec() 
{
	// TODO: この位置にコントロール通知ハンドラ用のコードを追加してください
	CString sBuf;
	GetDlgItemText(IDC_EDIT_QUICK_FNAME, sBuf);

	if(sBuf == "") return ;		// ファイル名が入力されていないときは中止

	CCompDelApp *theApp;
	theApp = (CCompDelApp *)AfxGetApp();

	theApp->QuickDeleteExec(sBuf);
}
