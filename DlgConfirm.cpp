// DlgConfirm.cpp : インプリメンテーション ファイル
//

#include "stdafx.h"
#include "CompDel.h"
#include "DlgConfirm.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDlgConfirm ダイアログ


CDlgConfirm::CDlgConfirm(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgConfirm::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDlgConfirm)
	m_mes = _T("");
	//}}AFX_DATA_INIT
}


void CDlgConfirm::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgConfirm)
	DDX_Text(pDX, IDC_MESSAGE, m_mes);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDlgConfirm, CDialog)
	//{{AFX_MSG_MAP(CDlgConfirm)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgConfirm メッセージ ハンドラ

void CDlgConfirm::OnCancel() 
{
	// TODO: この位置に特別な後処理を追加してください。
	
	CDialog::OnCancel();
	EndDialog(FALSE);
}

void CDlgConfirm::OnOK() 
{
	// TODO: この位置にその他の検証用のコードを追加してください
	
	CDialog::OnOK();
	EndDialog(TRUE);
}
/////////////////////////////////////////////////////////////////////////////
// CDlgInstall ダイアログ


CDlgInstall::CDlgInstall(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgInstall::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDlgInstall)
	m_CONTEXT = FALSE;
	m_DESKTOP = FALSE;
	m_MENU = FALSE;
	m_UNIN = -1;
	//}}AFX_DATA_INIT
}


void CDlgInstall::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgInstall)
	DDX_Check(pDX, IDC_INST_CONTEXT, m_CONTEXT);
	DDX_Check(pDX, IDC_INST_DESKTOP, m_DESKTOP);
	DDX_Check(pDX, IDC_INST_MENU, m_MENU);
	DDX_Radio(pDX, IDC_RADIO_UNIN, m_UNIN);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDlgInstall, CDialog)
	//{{AFX_MSG_MAP(CDlgInstall)
	ON_BN_CLICKED(IDC_INSTHLP, OnInsthlp)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgInstall メッセージ ハンドラ

// ************************************************************
// ヘルプ表示関数
// HTMLヘルプに対応させるために、クラスに対して WM_HELP メッセージを送る
// (F1キーを押したときは、この関数は通らずに直接 this->WinHelp() 関数へ行く
//
// 最終的には CCompDelApp の WinHelp 関数へ渡され、HTMLヘルプを表示する
// ************************************************************

void CDlgInstall::OnInsthlp() 
{
	// TODO: この位置にコントロール通知ハンドラ用のコードを追加してください
		this->SendMessage(WM_HELP);

}

