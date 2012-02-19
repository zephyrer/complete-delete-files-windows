// ************************************************************
// インストール・アンインストール関連
//
// ************************************************************

BOOL MkHklmLnk(void);		// レジストリにショートカット (HKLM, HKCU バージョン)
BOOL MkHkcuLnk(void);
BOOL RmHklmLnk(void);
BOOL RmHkcuLnk(void);
void MkStartMnu(void);		// スタートアップにショートカット（/startスイッチ付き)
void RmStartMnu(void);
void MkSendtoMnu(void);		// コンテキストメニューにショートカット
void RmSendtoMnu(void);
void MkDesktopMnu(void);	// デスクトップにショートカット
void RmDesktopMnu(void);
void MkProgramsMnu(void);	// スタートメニューにショートカット
void RmProgramsMnu(void);
void MkUninstMnu(void);		// スタートメニューに削除ショートカット (/deleteスイッチ付き)
void RmUninstMnu(void);
void MkHklmUninstMnu(void);		// レジストリ（コンパネ）に削除ショートカット (/deleteスイッチ付き)
void RmHklmUninstMnu(void);
void MkHkcuUninstMnu(void);		// レジストリ（コンパネ）に削除ショートカット (/deleteスイッチ付き)
void RmHkcuUninstMnu(void);
HRESULT CreateShellLink(LPCSTR pszShortcutFile,
							LPSTR pszLink, LPSTR pszDesc, LPSTR pszParam);
BOOL ChkHklmInstReg(void);
BOOL MkHklmInstReg(void);
BOOL RmHklmInstReg(void);

BOOL ChkHkcuInstReg(void);
BOOL MkHkcuInstReg(void);
BOOL RmHkcuInstReg(void);

void RmUserReg(void);

