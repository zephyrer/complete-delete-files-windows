// ************************************************************
// Windows バージョン関連関数
//
// ************************************************************

#include "StdAfx.h"
#include "CheckWinVer.h"


// ************************************************************
// WindowsNT 系列と Windows 95 系列の判定
// WinNT 系列のとき TRUE
// ************************************************************
BOOL IsWinNT(void)
{
	OSVERSIONINFO ver;

	// OSバージョン
	ver.dwOSVersionInfoSize = sizeof(OSVERSIONINFO);
	::GetVersionEx((LPOSVERSIONINFO)&ver);


	if(ver.dwPlatformId == VER_PLATFORM_WIN32_NT)
		return TRUE;

	return FALSE;
}