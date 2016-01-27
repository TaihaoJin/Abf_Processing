// FileNameHandling.cpp: implementation of the CFileNameHandling class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "Abf_Processing.h"
#include "FileNameHandling.h"
#include <sys/stat.h> 

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CFileNameHandling::CFileNameHandling()
{

}

CFileNameHandling::~CFileNameHandling()
{

}
CString CFileNameHandling::ExtendFileName(CString sFileName, CString sInsert)
{
	CString sNewFileName=sFileName;
	int nLength=sInsert.GetLength();
	for(int i=0;i<nLength;i++)
	{
		sNewFileName=ExtendFileName(sNewFileName,sInsert[i]);
	}
	return sNewFileName;
}


CString CFileNameHandling::ExtendFileName(CString sFileName, char ch)
{
	CString sNewFileName=sFileName;
	int nLength=sFileName.GetLength();
	sNewFileName.Insert(nLength-4,ch);
	return sNewFileName;
}

CString CFileNameHandling::ChangeFileExt(CString sFileName, CString sExt)
{
//	char cTemp;
	CString sNewFileName=sFileName;
	int nLength=sFileName.GetLength();
	for(int i=0;i<3;i++)
	{
		sNewFileName.SetAt(nLength-3+i,sExt[i]);
	}
	return sNewFileName;	
}

CString CFileNameHandling::getFileExt(CString sFileName)
{
//	char cTemp;
	int nLength=sFileName.GetLength();
	CString sExt=sFileName.Right(3);
	return sExt;	
}

CString CFileNameHandling::getFileName(CString sFileName)
{
//	char cTemp;
	int nLength=sFileName.GetLength();
	CString sName=sFileName.Left(nLength-4);
	return sName;	
}

CString CFileNameHandling::ExtendFileName(CString sFileName, float fNum, int nPrecision)
{
	CString s=ExtendFileName(sFileName,ToString(fNum,nPrecision));
	return s;
}

CString CFileNameHandling::ExtendFileName(CString sFileName, int nNum)
{
	CString s=ExtendFileName(sFileName,ToString(nNum));
	return s;
}

CString CFileNameHandling::ToString(int n)
{
	CString s;
	int index=0;
	if(n<0){
		s+="-";
		n*=-1;
		index++;
	}	
	int digits=getDigits(n);
	int it;
	char ct;
	for(int i=0;i<digits;i++){
		it=n%10;
		n/=10;
		ct=it+48;
		s.Insert(index,ct);
	}
	return s;
}

CString CFileNameHandling::ToString(float fNum, int nPrecision)
{
	CString s;
	if(fNum<0){
		s+="-";
		fNum*=-1;
	}
	int it=int(fNum);
	s+=ToString(it);
	if(nPrecision>0){
		int i;
		int factor=1;
		for(i=0;i<nPrecision;i++){
			factor*=10;
			fNum*=10;
		}
		it=int(fNum)%factor;
		s+=".";
		s+=ToString(it);
	}
	return s;
}

int CFileNameHandling::getDigits(int n)
{
	int digits=1;
	n/=10;
	while(n!=0){
		digits++;
		n/=10;
	}
	return digits;
}

bool CFileNameHandling::correctExtension(CString sFileName, CString sExt0, bool bCaseSensitive)
{
	bool bMatching=true;
	CString sExt=sFileName.Right(3);
	if(bCaseSensitive){
		bMatching=(sExt.Compare(sExt0)==0);
	}else{
		bMatching=(sExt.CompareNoCase(sExt0)==0);
	}
	return bMatching;
}

bool CFileNameHandling::FileExists(CString sFilename) {
  struct stat stFileInfo; 
  int nStatus; 
  nStatus = stat(sFilename.GetBuffer(),&stFileInfo); 
  sFilename.ReleaseBuffer(-1);
  if(nStatus == 0) { 
    return true; 
  }    
  return false; 
}
bool CFileNameHandling::validFileToRead(CString sFileName, CString sExt){
	if(!FileExists(sFileName)) return false;
	if(sExt!=getFileExt(sFileName)) return false;
	return true;
}