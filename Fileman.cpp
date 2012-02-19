// Fileman.cpp: CFileman �N���X�̃C���v�������e�[�V����
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "CompDel.h"
#include "Fileman.h"
#include "DlgNorm.h"
#include "DlgConfirm.h"

#include "PharseCmdline.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// �\�z/����
//////////////////////////////////////////////////////////////////////

CFileman::CFileman()
{
}

CFileman::~CFileman()
{

}

// ************************************************************
// �e������ϐ������o��
// ************************************************************
CString CFileman::GetFname()
{
	return sFname;
}

CString CFileman::GetFullPath()
{
	return sFullPath;
}

CString CFileman::GetPath()
{
	return sPath;
}

// ************************************************************
// �t���p�X��������ϐ��ɃZ�b�g�A����уp�X���̕���
// ���� buf : �t�@�C���̃t���p�X�i1�̃t�@�C���j
// �߂�l �F �t�H���_�Ȃǂ̏ꍇ FALSE, �폜�\�ȂƂ� TRUE
// ************************************************************
BOOL CFileman::SetFullPath(LPCSTR buf)
{	
	sFullPath = buf;	// �t���p�X���̃Z�b�g

	return PharsePath();
}

// ************************************************************
// �t���p�X���̕���
//  �t���p�X �� �p�X + �t�@�C����
// sPath : �p�X�� �i�� c:\Program Files\)
// sFname : �t�@�C���� (�� myfile~1.txt)
// sLFName : LFN�t�@�C���� (�� myfile text.txt)
// �� Windows2000�ł́AsFname �� sLFName �̈Ⴂ�͖���
//
// �߂�l �F �t�H���_�Ȃǂ̏ꍇ FALSE, �폜�\�ȂƂ� TRUE
// SetFullPath����Ă΂��֐�
// ************************************************************
BOOL CFileman::PharsePath()
{
	// �Ō�̃��[�g�E�R�����L���̈ʒu��T�����߂̕ϐ�
	int last_r=-1;
	// LFN �t�@�C���������p�̕ϐ�
	WIN32_FIND_DATA pFind;
	HANDLE hFind;

	sPath = "";		// �t�@�C���������n���ꂽ�Ƃ��̂��߂̏���
	// �t�H���_���̍Ō�� "\" �� ":"�̋L����ڈ��ɒT��
	// �Ō�� "\"�̈ʒu��擪 0�Ƃ����ʒu�ŕԂ��B���������āA1�����ĕ������Ƃ���B
	last_r = sFullPath.ReverseFind('\\') + 1;
	// MBCS�����Ƃ̋����́A2�o�C�g�ڂ�0x5c���܂ފ����Ŏ������Ƃ���A
	//  ���ʂȏ��������Ȃ��Ă��A�Ō��1�o�C�g�̃��[�g�L���̈ʒu��Ԃ��B
	if(last_r <= 0) last_r = sFullPath.ReverseFind(':') + 1;

	// �t�H���_���̐ݒ�B�Ō�̃��[�g�L���܂ŃR�s�[����
	sPath = sFullPath.Left(last_r);
	// �t�@�C���{�̖̂��O�̐ݒ�B
	sFname = sFullPath.Right(sFullPath.GetLength() - last_r);

	// LFN(�����t�@�C����)�������邽�߂̏���
	hFind = ::FindFirstFile(sFullPath, &pFind);
	if(hFind != INVALID_HANDLE_VALUE)
	{	// �t�@�C�������݂���
		sLFName = pFind.cFileName;
		isExist = TRUE;
		dwFileAttr = pFind.dwFileAttributes;
	}
	else
	{	// �t�@�C�������݂��Ȃ�
		isExist = FALSE;
		dwFileAttr = 0;
		return FALSE;
	}
	::FindClose(hFind);		// �����p�n���h�����N���[�Y

	// �����ΏۊO�̃t�@�C���E�t�H���_
	if((dwFileAttr & FILE_ATTRIBUTE_TEMPORARY)||
			(dwFileAttr & FILE_ATTRIBUTE_OFFLINE) ||
			(dwFileAttr & FILE_ATTRIBUTE_DIRECTORY))
		return FALSE;

	return TRUE;
}

CString CFileman::GetLFName()
{
	return sLFName;
}

// ************************************************************
// �폜���C���֐�
// �����̃t�@�C�������󂯎�邱�Ƃ��o����
// 
// ���� buf : �t�@�C�����̗��񕶎���i�X�y�[�X��؂�j
//       �� �F "f:\temp\aaa.txt f:\temp\bbb.txt f:\temp\ccc.txt"
// �߂�l : TRUE:����, FALSE:���s(buf��NULL�X�g�����O�j
// ************************************************************
BOOL CFileman::DeleteMain(LPCSTR buf)
{
	CPharseCmdline PharseCmdline;	// �R�}���h���C���؂蕪���N���X
	CDlgNorm *dlg;	// ���[�h���X�_�C�A���O�p

	PharseCmdline.SetOrgStr(buf);	// �R�}���h���C�����������͑ΏۂɃZ�b�g
	nFiles = PharseCmdline.CountParam();	// �����Ƃ��ė^����ꂽ�p�X���̐�

	if(nFiles <= 0) return FALSE;
	CString sTmp, sOrgFnameLog, fbuf, dlgMes, sAfxMsg;
	
	// �ŏ��̊m�F
	if(n_DispF)
	{
		CDlgConfirm dlgcnf;
		CString tmpbuf;
		::MessageBeep(MB_ICONQUESTION);
		// ****************** ������Ή� �f�o�b�N���b�Z�[�W�̃��[�h ******************
		sAfxMsg.LoadString(AFX_STR_DLG_CONFIRM);	// �u%d�̃t�@�C�����폜���܂��v
		tmpbuf.Format(sAfxMsg, nFiles);
		dlgcnf.m_mes = tmpbuf;
		if(!dlgcnf.DoModal())
			return TRUE;
	}

	// �u�폜�� ...�v�_�C�A���O�̍쐬
	if(!n_NDispN)
	{
		dlg = new CDlgNorm();
		if(dlg->Create() != TRUE)
		{
			::MessageBox(NULL, "Your Operating System is UnStable\r\nDialog Creation Error", "Error", MB_OK|MB_ICONEXCLAMATION|MB_TOPMOST);
			n_NDispN = 1;
		}
	}

	if(n_Log)
	{	// ���O�̋L�^
		// ****************** ������Ή� �f�o�b�N���b�Z�[�W�̃��[�h ******************
		sAfxMsg.LoadString(AFX_STR_LOG_INIT1);	// �u�폜���[�h :\r\n ���t�ύX:%s, �v
		sTmp.Format((LPCSTR)sAfxMsg, n_Date ? "ON":"OFF");
		*sLogStr += sTmp;
		// ****************** ������Ή� �f�o�b�N���b�Z�[�W�̃��[�h ******************
		sAfxMsg.LoadString(AFX_STR_LOG_INIT2);	// �u�㏑:%s, �㏑��:%d, �T�C�Y�ύX:%s, �v
		sTmp.Format((LPCSTR)sAfxMsg, n_rNull ? "NULL":"RND", n_nOvwr, n_rZLen ? "ON":"OFF");
		*sLogStr += sTmp;
		// ****************** ������Ή� �f�o�b�N���b�Z�[�W�̃��[�h ******************
		sAfxMsg.LoadString(AFX_STR_LOG_INIT3);	// �u���l�[���v
		if(n_rRen == 0) *sLogStr = *sLogStr + sAfxMsg + "OFF\r\n";
		else if(n_rRen == 1) *sLogStr = *sLogStr + sAfxMsg + "ON\r\n";
		else if(n_rRen == 2)  *sLogStr = *sLogStr + sAfxMsg + "ON(Win)\r\n";

		// ****************** ������Ή� �f�o�b�N���b�Z�[�W�̃��[�h ******************
		sAfxMsg.LoadString(AFX_STR_LOG_INIT4);	// �u%d �̃t�@�C���̏������J�n\r\n-----------------------------\r\n\r\n�v
		sTmp.Format((LPCSTR)sAfxMsg, nFiles);
		*sLogStr += sTmp;
	}


	OldPath = "";	// �_�~�[�t�@�C���쐬�p

	// ������؂蕪���āA�t�@�C�����ɕ������A�����֐��ɑ��� 
	for(int errN=0, i=1;;i++)
	{
		// �R�}���h���C���𕪗��������āA���̂ЂƂ̃p�X���𓾂�
		fbuf = PharseCmdline.GetNextParam();
		// �Ō�̃p�X���z�����ꍇ�́ANULL�����񂪕Ԃ�
		if(fbuf == "") break;

		// �p�X���̉�́i�p�X�ƃt�@�C�����̕����ƁA�ΏۊO�p�X���̏��O�j
		if(!SetFullPath(fbuf))
		{	// SetFullPath - PharsePath �� FALSE ��Ԃ����ꍇ
			if(!n_NDispE)
			{
				CString tmpbuf;
				// ****************** ������Ή� �f�o�b�N���b�Z�[�W�̃��[�h ******************
				sAfxMsg.LoadString(AFX_STR_DLG_EXCLUDE);	// �u�t�@�C�� '%s'\n���̃t�@�C���͊��S�폜�ΏۊO�ł��v
				tmpbuf.Format((LPCSTR)sAfxMsg, fbuf);
				::MessageBox(NULL, tmpbuf, "Information",
				MB_OK|MB_ICONINFORMATION|MB_TOPMOST);
				errN++;	// �_�C�A���O�\���p�̃J�E���^
			}
			if(n_Log)
			{	// ���O�̋L�^
				// ****************** ������Ή� �f�o�b�N���b�Z�[�W�̃��[�h ******************
				sAfxMsg.LoadString(AFX_STR_LOG_EXCLUDE);	// �u�ΏۊO : %s\r\n�v
				sTmp.Format((LPCSTR)sAfxMsg, (LPCSTR)GetFullPath());
				*sLogStr += sTmp;
			}
			continue;
		}
		// �_�C�A���O�̕\�����e����
		if(!n_NDispN)
		{
			// �_�C�A���O�Ƀt�@�C�������Z�b�g
			dlg->SetDlgItemText(IDC_LFNAME, sLFName);
			// �_�C�A���O�ɍ폜�ԍ���\��
			// ****************** ������Ή� �f�o�b�N���b�Z�[�W�̃��[�h ******************
			sAfxMsg.LoadString(AFX_STR_DLG_PRGRS);	// �u%d/%d ������ (�G���[ %d ��)�v
			dlgMes.Format((LPCSTR)sAfxMsg, i, nFiles, errN);
			dlg->SetDlgItemText(IDC_MES, dlgMes);
			// �_�C�A���O�̃v���O���X�R���g���[��
			dlg->m_Progress.SetPos((i*100)/nFiles);
			// �_�C�A���O���A�b�v�f�[�g
			dlg->RedrawWindow();
			dlg->UpdateWindow();
		}

		if(n_Log)
		{	// ���O�̋L�^
			// �t�@�C�����ύX���[�h�ŃI���W�i���t�@�C������������̂�h��
			sOrgFnameLog = GetFullPath();
		}
		// �t�@�C���폜
		nErrTrace = 0;
		if(!DeleteOneFile())
		{
			if(!n_NDispE)
			{
				switch(nErrTrace)
				{
					// ****************** ������Ή� �f�o�b�N���b�Z�[�W�̃��[�h ******************
					case 0 : sAfxMsg.LoadString(AFX_STR_ERR_0);break;	// �t�@�C�������������Ȃ�
					case 1 : sAfxMsg.LoadString(AFX_STR_ERR_1);break;	// �t�@�C��������ύX�ł��Ȃ�
					case 2 : sAfxMsg.LoadString(AFX_STR_ERR_2);break;	// �t�@�C�����J���Ȃ�
					case 3 : sAfxMsg.LoadString(AFX_STR_ERR_3);break;	// �t�@�C���ɏ������߂Ȃ�
					case 4 : sAfxMsg.LoadString(AFX_STR_ERR_4);break;	// �t�@�C���̒�����؂�̂Ă��Ȃ�
					case 5 : sAfxMsg.LoadString(AFX_STR_ERR_5);break;	// �t�@�C�������ς����Ȃ�
					case 6 : sAfxMsg.LoadString(AFX_STR_ERR_6);break;	// �t�@�C���̓��t���ύX�ł��Ȃ�
					case 7 : sAfxMsg.LoadString(AFX_STR_ERR_7);break;	// �t�@�C�����폜�ł��Ȃ�
					case 8 : sAfxMsg.LoadString(AFX_STR_ERR_8);break;	// �_�~�[�t�@�C�����쐬�ł��Ȃ�
					case 9 : sAfxMsg.LoadString(AFX_STR_ERR_9);break;	// �_�~�[�t�@�C�����폜�ł��Ȃ�
					default : sAfxMsg.LoadString(AFX_STR_ERR_N);break;	// ���m�F�̃G���[
				}
				// ****************** ������Ή� �f�o�b�N���b�Z�[�W�̃��[�h ******************
				sTmp.LoadString(AFX_STR_DLG_ERRCAUSE);		// �u�t�@�C���̊��S�폜���ɃG���[����\r\n�Ώۃt�@�C�� %s\r\n���� : %s\r\n\r\n�t�@�C���̍폜�͊������܂���ł����v
				dlgMes.Format((LPCSTR)sTmp, GetLFName(), (LPCSTR)sAfxMsg);
				::MessageBox(NULL, dlgMes, "Error",	MB_OK|MB_ICONSTOP|MB_TOPMOST);
			}
			errN++;	// �_�C�A���O�\���p�̃J�E���^
			if(n_Log)
			{	// ���O�̋L�^
				// ****************** ������Ή� �f�o�b�N���b�Z�[�W�̃��[�h ******************
				sAfxMsg.LoadString(AFX_STR_LOG_FAIL);	// �u���s (code=%d) : %s\r\n�v
				sTmp.Format((LPCSTR)sAfxMsg, nErrTrace, sOrgFnameLog);
				*sLogStr += sTmp;
			}
		}
		else
		{	
			if(n_Log)
			{	// ���O�̋L�^ �i�������j
				// ****************** ������Ή� �f�o�b�N���b�Z�[�W�̃��[�h ******************
				sAfxMsg.LoadString(AFX_STR_LOG_SUCCESS);	// �u���� : %s\r\n�v
				sTmp.Format((LPCSTR)sAfxMsg, sOrgFnameLog);
				*sLogStr += sTmp;
			}
		}
		// �_�~�[�t�@�C�����������ށi�t�@�C�������[�h�j
		if(n_Dummy && !n_DummySkip) DummyfileProcess();
		// �_�C�A���O�\���̂Ƃ��́A���₷�����߂̑҂�����
		if(!n_NDispN) ::Sleep(1000/nFiles);
	}
	// �_�~�[�t�@�C�����������ށi�t�@�C�������[�h�j
	if(n_Dummy) DummyfileProcess();
	// �_�C�A���O�̏���
	if(!n_NDispN)
	{
		dlg->DestroyWindow();
		delete dlg;
	}
	if(n_Log)
	{	// ���O�̋L�^ �i�������j
		// ****************** ������Ή� �f�o�b�N���b�Z�[�W�̃��[�h ******************
		sAfxMsg.LoadString(AFX_STR_LOG_COMPLETE);	// �u\r\n�I�� (���� %d ��, ���s+�ΏۊO %d ��)\r\n�v
		sTmp.Format((LPCSTR)sAfxMsg, i-1-errN, errN);
		*sLogStr += sTmp;
	}
	return TRUE;

}

// �t�@�C�����̕ύX�Ŏg�p����A�U�t�@�C����
char *dmyFnameCore[] = {"AWSRVR32","CMCT232","D3DRG16","INST32","JAVAPRXY",
		"MSCREATE","EXCEL8","XL8049","CP_1252","ICRSCRPT"};
char *dmyFnameExt[] = {"DLL","DAT","OCX","DRV","CAB","VBX","EXE","CAB","TMP","X3"};

// ************************************************************
// �t�@�C���̍폜�i���S�폜�j���C���֐�
// ���� : SetFullPath() �Őݒ肳�ꂽ�t�@�C�����폜����i1�t�@�C���j
//
// �߂�l �F TRUE:����, FALSE:���s
// ************************************************************
BOOL CFileman::DeleteOneFile()
{
	CFile fp;				// �t�@�C��
	CFileStatus fStatus;	// �t�@�C�����擾�A���t�ύX�Ɏg�p
	char buf[2050];			// �������݃f�[�^�̃o�b�t�@
	long li;				// �t�@�C���|�C���^�Ƃ��Ďg�p
	int si;					// int ���[�v�J�E���^�p
	nErrTrace = 0;
	CString sAfxMsg;		// ������Ή��p ���b�Z�[�W�����\�[�X���ǂݍ���

	srand( (unsigned)time( NULL ));

	try
	{
// �����ύX�i�S����������)
		if(!CFile::GetStatus(GetFullPath(), fStatus))
			return FALSE;	// ���s
		nErrTrace = 1;
		// ���[�h�I�����[�A�V�X�e���A�B������������
		fStatus.m_attribute = fStatus.m_attribute & (~(0x01 | 0x02 | 0x04));
		CFile::SetStatus(GetFullPath(), fStatus); // ���s�͗�O���X���[�����
		// *��Fattr = ::GetFileAttributes(GetFullPath()); ���g�p���Ă��悢
		// ****************** ������Ή� �f�o�b�N���b�Z�[�W�̃��[�h�ƕ\�� ******************
		sAfxMsg.LoadString(AFX_STR_DBG_ATTR);	// �u�t�@�C�����������v
		ConfirmMsgbox((LPCSTR)sAfxMsg);
		nErrTrace = 2;
		// �����ύX�i�S����������)�����܂�
// �f�[�^�㏑��
		if(!CFile::GetStatus(GetFullPath(), fStatus))
			return FALSE;	// ���s
		// ����I�[�v�����āA�N���[�Y����̂́A�����������݂����邽��
		if(n_rNull)
		{	// �����_���l�łQ��㏑��
			srand((unsigned)time(NULL));
			for(int i=0; i<n_nOvwr; i++)
			{	// i��J��Ԃ��ď㏑������
				if(!fp.Open(GetFullPath(), CFile::modeReadWrite|CFile::shareExclusive|CFile::typeBinary, NULL))
					return FALSE;
				nErrTrace = 3;
				for(li=0; li<(fStatus.m_size/2048)+1; li++)
				{
					for(int j=0; j<2048; j++)
						buf[j] = (unsigned char)rand();
					fp.Write(buf, 2048);
				}
				fp.Flush();
				fp.Close();
			}
		}
		else
		{
			for(int j=0; j<2048; j++) buf[j] = 0x0;
			for(int i=0; i<n_nOvwr; i++)
			{
				if(!fp.Open(GetFullPath(), CFile::modeReadWrite|CFile::shareExclusive|CFile::typeBinary, NULL))
					return FALSE;
				nErrTrace = 3;
				for(li=0; li<(fStatus.m_size/2048)+1; li++)
					fp.Write(buf, 2048);
				fp.Flush();
				fp.Close();
			}
		}
		// �o�b�t�@�������I�Ƀt���b�V�������邽�߁A�_�~�[�f�[�^����������
		if(!fp.Open(GetFullPath(), CFile::modeReadWrite|CFile::shareExclusive|CFile::typeBinary, NULL))
			return FALSE;
		fp.Write("X", 1);
		fp.Flush();
		fp.Close();
		// ****************** ������Ή� �f�o�b�N���b�Z�[�W�̃��[�h�ƕ\�� ******************
		sAfxMsg.LoadString(AFX_STR_DBG_OVER);	// �u�f�[�^�̏㏑�����������v
		ConfirmMsgbox((LPCSTR)sAfxMsg);
		nErrTrace = 4;
		// �f�[�^�㏑�� �����܂�
// �t�@�C���������[���ɐ؂�̂Ă�
		if(n_rZLen)
		{
			if(!fp.Open(GetFullPath(), CFile::modeCreate|CFile::modeReadWrite|CFile::shareExclusive|CFile::typeBinary, NULL))
				return FALSE;
			fp.Flush();
			fp.Close();
			// ****************** ������Ή� �f�o�b�N���b�Z�[�W�̃��[�h�ƕ\�� ******************
			sAfxMsg.LoadString(AFX_STR_DBG_ZEROLEN);	// �u�t�@�C�����[���v
			ConfirmMsgbox((LPCSTR)sAfxMsg);
		}
		nErrTrace = 5;
		// �t�@�C���������[���ɐ؂�̂Ă� �����܂�
// �t�@�C������ύX����
		if(n_rRen)
		{
			CString sNewFullPath;
			char sNewLFName[MAX_PATH];
			srand((unsigned)time(NULL));
			// �@�k�e�m�Ή��A���݂̃t�@�C���������S�����̂��߂̃t�@�C�����ύX
			// �Œ�15�����̃t�@�C�����Ƃ���
			for(int i=0;i<10;i++)
			{	// ���łɂ���t�@�C�����ɏd�Ȃ�Ȃ����߂̏d������
				WIN32_FIND_DATA pFind;
				HANDLE hFind;
				strcpy(sNewLFName, GetLFName());
				for(si=0; (unsigned char)sNewLFName[si]; si++)
					if(sNewLFName[si] == '.') break;
				if(si<7)
				{
					for(int i=0; i<8; i++)
						sNewLFName[i] = (unsigned char)GenerateRandom(0x41,0x5a);
					sNewLFName[8] = (char)0;
					strcat(sNewLFName, ".TMP");
				}
				else
				{
					for(int i=0; sNewLFName[i]; i++)
					{
						if(sNewLFName[i]!='.')
							sNewLFName[i] = (unsigned char)GenerateRandom(0x41,0x5a);
					}
				}

				sNewFullPath = GetPath() + sNewLFName;
				hFind = ::FindFirstFile(sNewFullPath, &pFind);
				if(hFind == INVALID_HANDLE_VALUE)
				{	// �t�@�C�������t����Ȃ���΁A���̃t�@�C�����Ɍ���
					::FindClose(hFind);		// �����p�n���h�����N���[�Y
					break;
				}
				else ::FindClose(hFind);	// �����p�n���h�����N���[�Y
			}
			if(i<9)
			{	// �t�@�C���������ׂĎg�p�ς݂̂Ƃ����O�ύX��������߂�
				CFile::Rename(GetFullPath(), sNewFullPath);
				SetFullPath(sNewFullPath);	// �t�@�C�����f�[�^������������
				// ****************** ������Ή� �f�o�b�N���b�Z�[�W�̃��[�h�ƕ\�� ******************
				sAfxMsg.LoadString(AFX_STR_DBG_FNAME);	// �u�t�@�C�������h���i�k�e�m�㏑���j�����v
				ConfirmMsgbox((LPCSTR)sAfxMsg);
			}
			// �AWindows�Ɏ��������O�ɕύX
			if(n_rRen == 2)
			{
				for(int i=0;i<10;i++)
				{	// ���łɂ���t�@�C�����ɏd�Ȃ�Ȃ����߂̏d������
					WIN32_FIND_DATA pFind;
					HANDLE hFind;
					CString dmyFname;
					sprintf(sNewLFName, "%s.%s", dmyFnameCore[GenerateRandom(0,9)],
						dmyFnameExt[GenerateRandom(0,9)]);
					dmyFname = GetPath() + sNewLFName;
					hFind = ::FindFirstFile(dmyFname, &pFind);
					if(hFind == INVALID_HANDLE_VALUE)
					{	// �t�@�C�������t����Ȃ���΁A���̃t�@�C�����Ɍ���
						::FindClose(hFind);		// �����p�n���h�����N���[�Y
						break;
					}
					else ::FindClose(hFind);	// �����p�n���h�����N���[�Y
				}
				if(i<9)
				{	// �t�@�C���������ׂĎg�p�ς݂̂Ƃ����O�ύX��������߂�
					sNewFullPath = GetPath() + sNewLFName;
					CFile::Rename(GetFullPath(), sNewFullPath);
					SetFullPath(sNewFullPath);	// �t�@�C�����f�[�^������������
				}
				// ****************** ������Ή� �f�o�b�N���b�Z�[�W�̃��[�h�ƕ\�� ******************
				sAfxMsg.LoadString(AFX_STR_DBG_WINNAME);	// �u�t�@�C�������h���iWindows�t�@�C�����j�����v
				ConfirmMsgbox((LPCSTR)sAfxMsg);
			}
		}
		nErrTrace = 6;
		// �t�@�C������ύX���� �����܂�
// �t�@�C���̓��t��ύX����
		if(n_Date)
		{
			if(!CFile::GetStatus(GetFullPath(), fStatus))
				return FALSE;	// ���s
			CTime dummyTime(1998,1,1,0,0,2);
			fStatus.m_ctime = fStatus.m_atime = fStatus.m_mtime = dummyTime;
			CFile::SetStatus(GetFullPath(), fStatus); // ���s�͗�O���X���[�����
			// ****************** ������Ή� �f�o�b�N���b�Z�[�W�̃��[�h�ƕ\�� ******************
			sAfxMsg.LoadString(AFX_STR_DBG_DATE);	// �u�t�@�C���̓��t�ύX�����v
			ConfirmMsgbox((LPCSTR)sAfxMsg);
		}
		nErrTrace = 7;
		// �t�@�C���̓��t��ύX���� �����܂�
// �t�@�C�����폜����
		CFile::Remove(GetFullPath());
		// ****************** ������Ή� �f�o�b�N���b�Z�[�W�̃��[�h�ƕ\�� ******************
		sAfxMsg.LoadString(AFX_STR_DBG_DELETE);	// �u�t�@�C���폜�����v
		ConfirmMsgbox((LPCSTR)sAfxMsg);
		nErrTrace = 8;
		// �t�@�C�����폜���� �����܂�
	}
	catch(CFileException *e)
	{	// �t�@�C���������̃G���[
		
		if(e->m_cause==CFileException::hardIO)
		{
			::MessageBox(NULL, "File I/O Error (Hardware Error)", "Error", MB_OK|MB_ICONSTOP|MB_TOPMOST);
		}
		return FALSE;
	}
	return TRUE;
}

// ************************************************************
// �����̍쐬�i���͈͓��j
// ���� : min, max (�����̉����l�A����l�j
//
// �߂�l �F min �` max �̗�����Ԃ� (�㉺���͊܂ށj
// ************************************************************
unsigned int CFileman::GenerateRandom(unsigned int min, unsigned int max)
{
	unsigned int i=0;
	do
	{
		i = ((rand() & 0xff) * (max-min+1))/0xff + min;
	}while(i<min || i>max);

	return i;
}

void CFileman::ConfirmMsgbox(CString mes)
{
	if(!n_Confirm) return;
	CString buf, sAfxMsg, sAfxMsg2;
	// ****************** ������Ή� �f�o�b�N���b�Z�[�W�̃��[�h�ƕ\�� ******************
	sAfxMsg.LoadString(AFX_STR_DLG_PRGRS_2);	// �u̧�فu%s�v�̍폜������\n%s�v
	buf.Format((LPCSTR)sAfxMsg, GetLFName(), mes);
	sAfxMsg2.LoadString(AFX_STR_DLG_PRGRS_3);	// �u���S�폜�����󋵁v
	::MessageBox(NULL, buf, (LPCSTR)sAfxMsg2, MB_OK|MB_ICONINFORMATION);
}

// ************************************************************
// �t�H���_���ɕ����̃_�~�[�t�@�C�����쐬���A��������
// �t�H���_�̍폜���Ղ̉�Ȃ𓦂�邽�߂́A��i
//
// ************************************************************
BOOL CFileman::DummyfileProcess()
{
	CFile fp;				// �t�@�C��
	char sNewLFName[MAX_PATH];
	char buf[MAX_PATH];
	CString sTmp, sAfxMsg, dlgMes;

// �h���t�@�C���𑽐��쐬����
	try
	{
		WIN32_FIND_DATA pFind;
		HANDLE hFind;
		CString dmyFname;
		// �_�~�[�t�@�C���̐擪5���������肷��
		for(int i=0; i<4; i++)
			sNewLFName[i] = (unsigned char)GenerateRandom(0x61,0x7a);
		sNewLFName[i] = 0x0;
		// �����p�t�@�C�������쐬
		sprintf(buf, "%s*.*", sNewLFName);
		dmyFname = GetPath() + buf;
		hFind = ::FindFirstFile(dmyFname, &pFind);
		if(hFind != INVALID_HANDLE_VALUE)
		{	// �_�~�[�t�@�C�����Ɗ����t�@�C��������v����Ƃ�
			::FindClose(hFind);		// �����p�n���h�����N���[�Y
			return FALSE;
		}
		::FindClose(hFind);		// �����p�n���h�����N���[�Y
		// �_�~�[�t�@�C���쐬
		for(i=0; i<n_nFiles; i++)
		{
			sprintf(buf, "%s%02d.tmp", sNewLFName, i);
			dmyFname = GetPath() + buf;
			if(!fp.Open(dmyFname, CFile::modeCreate|CFile::modeReadWrite|CFile::shareExclusive|CFile::typeBinary, NULL))
				return FALSE;
			fp.Close();
		}
		// ****************** ������Ή� �f�o�b�N���b�Z�[�W�̃��[�h�ƕ\�� ******************
		sAfxMsg.LoadString(AFX_STR_DBG_MKDUMMY);	// �u�_�~�[�t�@�C�����쐬�v
		ConfirmMsgbox((LPCSTR)sAfxMsg);
		nErrTrace = 9;
		// �_�~�[�t�@�C������
		for(i=0; i<n_nFiles; i++)
		{
			sprintf(buf, "%s%02d.tmp", sNewLFName, i);
			dmyFname = GetPath() + buf;
			CFile::Remove(dmyFname);
		}
		// ****************** ������Ή� �f�o�b�N���b�Z�[�W�̃��[�h�ƕ\�� ******************
		sAfxMsg.LoadString(AFX_STR_DBG_DLDUMMY);	// �u�_�~�[�t�@�C���������v
		ConfirmMsgbox((LPCSTR)sAfxMsg);
		nErrTrace = 10;
		// �h���t�@�C���𑽐��쐬���� �����܂�
		if(n_Log)
		{	// ���O�̋L�^ �i�������j
			// ****************** ������Ή� �f�o�b�N���b�Z�[�W�̃��[�h ******************
			sAfxMsg.LoadString(AFX_STR_LOG_DUMMY);	// �u �_�~�[�t�@�C���̍쐬 (%d ��)\r\n�v
			sTmp.Format((LPCSTR)sAfxMsg, n_nFiles);
			*sLogStr += sTmp;
		}
	}
	catch(CFileException *e)
	{	// �t�@�C���������̃G���[
		
		if(e->m_cause==CFileException::hardIO)
		{
			::MessageBox(NULL, "File I/O Error (Hardware Error)", "Error", MB_OK|MB_ICONSTOP|MB_TOPMOST);
		}
		else
		{
			// ****************** ������Ή� �f�o�b�N���b�Z�[�W�̃��[�h ******************
			sAfxMsg.LoadString(AFX_STR_ERR_8);	// �_�~�[�t�@�C�����쐬�ł��Ȃ�
			sTmp.LoadString(AFX_STR_DLG_ERRCAUSE);		// �u�t�@�C���̊��S�폜���ɃG���[����\r\n�Ώۃt�@�C�� %s\r\n���� : %s\r\n\r\n�t�@�C���̍폜�͊������܂���ł����v
			dlgMes.Format((LPCSTR)sTmp, GetLFName(), (LPCSTR)sAfxMsg);
			::MessageBox(NULL, dlgMes, "Error",	MB_OK|MB_ICONSTOP|MB_TOPMOST);
		}
		return FALSE;
	}
	return TRUE;
}