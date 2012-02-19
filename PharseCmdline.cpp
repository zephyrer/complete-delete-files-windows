// *******************************************************************
// プログラムに渡される引数のパス名を、ひとつずつに分離します
//
// " ... " のように囲まれたパス名は 「"」を取り去って分離
// パス名の間は「 」（スペース1個）で区切られていること
//
// 使い方
//
// 1. PharseCmdline.SetOrgStr(m_lpCmdLine); // 引数をセット
//
// 2. i = PharseCmdline.CountParam(); // 必要であれば、パスの数を取り出せる
//    ※ パスの数は 1, 2, 3 ... の 1からはじまる数である
//
// 3. while(){ sTmp1 = PharseCmdline.GetNextParam(); } // 順次パス名を取り出してゆく
//    ※ 注意 ： 全てのパス名を取り出し終えると、"" (NULL ストリング)が返る
//
// *******************************************************************

// PharseCmdline.cpp: CPharseCmdline クラスのインプリメンテーション
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "PharseCmdline.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// 構築/消滅
//////////////////////////////////////////////////////////////////////

CPharseCmdline::CPharseCmdline()
{
	m_sOrgStr = "";
	m_nCountTotal = 0;
	m_nCountCur = 0;
}

CPharseCmdline::~CPharseCmdline()
{

}

// *****************************************
// 対象文字列をクラス内のCString 変数に格納する
// *****************************************
BOOL CPharseCmdline::SetOrgStr(LPCSTR sBuf)
{
	if(strlen(sBuf))
	{
		m_sOrgStr = sBuf;
		m_nCountTotal = CountParam();
		return TRUE;
	}
	else
	{
		m_sOrgStr = "";
		return FALSE;
	}
}

// *****************************************
// パラメーターが何個あるか返す
//
// 「AAA BBB CCC」→ 3個
// 「"AAA BBB" CCC」→ 2個
// *****************************************
int CPharseCmdline::CountParam()
{
	long i;	// テンポラリ(文字列シーク用）
	long nStrLen = m_sOrgStr.GetLength();	// コマンドライン文字列の長さ
	int nCount = 0;		// スペースで区切ったパラメータの数を数える
	BOOL bInsideStr = FALSE;	// ダブルクォーテーションで囲まれた領域にいるとき

	if(nStrLen <= 0) return 0;		// 文字列の長さ０のとき

	for(i=0; i<nStrLen; i++)
	{
		if(m_sOrgStr.GetAt(i) == (char)'\"')
		{	// パラメータが " ... " のようにダブルクォーテーションで囲まれている場合
			if(!bInsideStr) bInsideStr = TRUE;	// " ... " 内部に入る
			else
			{
				if(i == nStrLen-1)
				{	// または、文字列終末の " の場合
					bInsideStr = FALSE;
					i++;
					nCount++;
				}
				else if(m_sOrgStr.GetAt(i+1) == ' ')
				{	// " ... " の最後の " は直後に スペースがついたら最後の " と認定
					// または、文字列終末の " の場合
					bInsideStr = FALSE;
					i++;
					nCount++;
				}
			}
		}
		else if(!bInsideStr && m_sOrgStr.GetAt(i) == ' ')
		{	// パラメータの間はスペースで区切られている場合
			nCount++;
		}
		else if(i == nStrLen-1)		// 文字列終端の場合
			nCount++;
	}
	return nCount;
}


// *****************************************
// 次のパラメータを「ひとつ」返す
//
// 取り出すべきパスが無くなれば NULL ストリングを返す
// *****************************************

LPCSTR CPharseCmdline::GetNextParam()
{

	if(m_nCountCur >= m_nCountTotal)
	{	// すでに、全てのパラメータを出し終わっているとき
		m_sOneParam = "";
		return (LPCSTR)m_sOneParam;
	}

	GetParamN(m_nCountCur + 1);		// 結果は m_sOneParam に入っている
	m_nCountCur++;		// 現在のパラメータ番号をインクリメント
	// *********
	// 先頭と末尾のダブルクォーテーションを取る
	// *********
	if(m_sOneParam.GetAt(0) == (char)'\"')
		m_sOneParam.Delete(0,1);	// 0 文字目からの1文字を削除
	long i = m_sOneParam.GetLength();
	if(m_sOneParam.GetAt(i-1) == (char)'\"')
		m_sOneParam.Delete(i-1,1);	// 最後の文字を削除
	return (LPCSTR)m_sOneParam;
}

// *****************************************
// n番目のパラメータを「ひとつ」返す
// n は 1 ～ m_nCountTotal
// *****************************************
LPCSTR CPharseCmdline::GetParamN(int n)
{
	CString sBuf;				// 取り出した文字列
	long nStrBegin = 0, nStrEnd = 0;	// 取り出す文字列の、全体の中での始点・終点
	long i;	// テンポラリ(文字列シーク用）
	long nStrLen = m_sOrgStr.GetLength();	// コマンドライン文字列の長さ
	int nCount = 0;		// スペースで区切ったパラメータの数を数える
	BOOL bInsideStr = FALSE;	// ダブルクォーテーションで囲まれた領域にいるとき

	if(n > m_nCountTotal)
	{	// トータルのパラメータ数を越えた要求のとき
		m_sOneParam = "";
		return (LPCSTR)m_sOneParam;
	}

	for(i=0; i<nStrLen && nCount < n; i++)		// nCount:0～m_nCountTotal-1, n:1～m_nCountTotal
	{
		if(m_sOrgStr.GetAt(i) == (char)'\"')
		{	// パラメータが " ... " のようにダブルクォーテーションで囲まれている場合
			if(!bInsideStr) bInsideStr = TRUE;	// " ... " 内部に入る
			else
			{
				if(i == nStrLen-1)
				{	// 文字列終末の " の場合
					bInsideStr = FALSE;
					i++;
					nCount++;
					if(i+2<nStrLen && nCount-1==n) nStrBegin = i+1;	// 指定番号の文字列の開始位置
					if(nCount==n) nStrEnd = i-1;	// 指定番号の文字列の終了位置
				}
				else if(m_sOrgStr.GetAt(i+1) == ' ')
				{	// " ... " の最後の " は直後に スペースがついたら最後の " と認定
					bInsideStr = FALSE;
					i++;
					nCount++;
					if(i+2<nStrLen && nCount==n-1) nStrBegin = i+1;	// 指定番号の文字列の開始位置
					if(nCount==n) nStrEnd = i-1;	// 指定番号の文字列の終了位置
				}
			}
		}
		else if(!bInsideStr && m_sOrgStr.GetAt(i) == ' ')
		{	// パラメータの間はスペースで区切られている場合
			nCount++;
			if(i+1<nStrLen && nCount==n-1) nStrBegin = i+1;	// 指定番号の文字列の開始位置
			if(nCount==n) nStrEnd = i-1;	// 指定番号の文字列の終了位置
		}
		else if(i == nStrLen-1)		// 文字列終端の場合
		{
			nCount++;
			if(nCount==n) nStrEnd = i;	// 指定番号の文字列の終了位置
		}
	}
	if(nStrEnd == 0) m_sOneParam = "";		// 指定番号の文字列なし （エラー）
	m_sOneParam = m_sOrgStr.Mid(nStrBegin, nStrEnd - nStrBegin + 1);

	return (LPCSTR)m_sOneParam;

}
