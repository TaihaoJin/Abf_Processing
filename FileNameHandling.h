// FileNameHandling.h: interface for the CFileNameHandling class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_FILENAMEHANDLING_H__93E54D85_A677_4751_B910_15CAE5678844__INCLUDED_)
#define AFX_FILENAMEHANDLING_H__93E54D85_A677_4751_B910_15CAE5678844__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CFileNameHandling  
{
public:
	bool FileExists(CString sFilename);
	bool correctExtension(CString sFileName, CString sExt, bool  bCaseSensitive);
	bool validFileToRead(CString sFileName, CString sExt);
	int getDigits(int n);
	CString ToString(float fNum, int nPrecision);
	CString ToString(int n);
	CString ExtendFileName(CString sFileName, int nNum);
	CString ExtendFileName(CString sFileName, float fNum, int nPrecision);
	CFileNameHandling();
	virtual ~CFileNameHandling();
	CString getFileName(CString sFileName);
	CString getFileExt(CString sFileName);
	CString ChangeFileExt(CString sFileName, CString sExt);
	CString ExtendFileName(CString sFileName, char ch);
	CString ExtendFileName(CString sFileName, CString sInsert);
};

#endif // !defined(AFX_FILENAMEHANDLING_H__93E54D85_A677_4751_B910_15CAE5678844__INCLUDED_)
