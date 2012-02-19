// PharseCmdline.h: CPharseCmdline クラスのインターフェイス
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_PHARSECMDLINE_H__C003E16C_8F4F_45D3_89A5_E8D76D44978A__INCLUDED_)
#define AFX_PHARSECMDLINE_H__C003E16C_8F4F_45D3_89A5_E8D76D44978A__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CPharseCmdline  
{
public:
	LPCSTR GetParamN(int n);
	CString m_sOneParam;
	LPCSTR GetNextParam(void);
	int CountParam(void);
	BOOL SetOrgStr(LPCSTR sBuf);
	CString m_sOrgStr;
	int m_nCountCur;
	int m_nCountTotal;
	CPharseCmdline();
	virtual ~CPharseCmdline();

};

#endif // !defined(AFX_PHARSECMDLINE_H__C003E16C_8F4F_45D3_89A5_E8D76D44978A__INCLUDED_)
