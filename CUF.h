#if !defined(AFX_EVLNODE_H__729A080D_5FE1_11D4_83DE_00C04F201B5A__INCLUDED_)
#define AFX_EVLNODE_H__729A080D_5FE1_11D4_83DE_00C04F201B5A__INCLUDED_
#include <vector>

using namespace std;
typedef vector <float> VectF;
namespace CUF{
	double Log(double dX0, double dY0)
	{
		return log(dY0)/log(dX0);
	}

	double Exp(double dX0, double dY0)
	{
		//Assuming dX0>=0;
		double dTiny=4.9406564584125e-324;
		double dResult=0.;
		if(dX0>dTiny)
		{
			dResult=exp(dY0*log(dX0));
		}
		else
		{
			dResult=1.;
		}
		return dResult;
	}

	CString ChangeFileExt(CString sFileName, CString sExt)
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


	CString FileExt(CString sFileName)
	{
	//	char cTemp;
		CString sExt="abc";
		int nLength=sFileName.GetLength();
		for(int i=0;i<3;i++)
		{
			sExt.SetAt(i,sFileName[nLength-3+i]);
		}
		return sExt;	
	}

	CString PassingString(CString sString)
	{
		CString sNewString=sString;
		return sNewString;
	}

	void WriteAString(CString aString, FILE *fp)
	{
		int nMaxLength=1000;
		int nLength=aString.GetLength();
		if(nLength>nMaxLength)
		{
			nLength=nMaxLength;
		}
		fwrite(&nLength,sizeof(int),1,fp);
		for(int i=0;i<nLength;i++)
		{
			char cTemp=aString[i];
			fwrite(&cTemp,sizeof(char),1,fp);
		}
	}

	CString ReadAString(FILE *fp)
	{
		int nLength=0;
		CString aString;
		fread(&nLength,sizeof(int),1,fp);
		for(int i=0;i<nLength;i++)
		{
			char cTemp;
			fread(&cTemp,sizeof(char),1,fp);
			aString+=cTemp;
		}
		return aString;
	}

	structSemNode CalSem(vector <float> vectfVect)
	{
		int nSize=vectfVect.size();
		int i;

		float fMean=0.f,fSem=0.f;
		for(i=0;i<nSize;i++)
		{
			fMean+=vectfVect[i];
		}

		fMean/=float(nSize);
		for(i=0;i<nSize;i++)
		{
			fSem+=(fMean-vectfVect[i])*(fMean-vectfVect[i]);
		}

		if(nSize>1)
		{
			fSem/=float(nSize*(nSize-1));
			fSem=sqrt(fSem);
		}

		structSemNode aNode;

		aNode.fMean=fMean;
		aNode.fSem=fSem;
		aNode.nSampleSize=nSize;

		return aNode;
	}


	structSemNode CalSem(vector <double> vectfVect)
	{
		int nSize=vectfVect.size();
		int i;

		float fMean=0.f,fSem=0.f;
		for(i=0;i<nSize;i++)
		{
			fMean+=vectfVect[i];
		}

		fMean/=float(nSize);
		for(i=0;i<nSize;i++)
		{
			fSem+=(fMean-vectfVect[i])*(fMean-vectfVect[i]);
		}

		if(nSize>1)
		{
			fSem/=float(nSize*(nSize-1));
			fSem=sqrt(fSem);
		}

		structSemNode aNode;

		aNode.fMean=fMean;
		aNode.fSem=fSem;
		aNode.nSampleSize=nSize;

		return aNode;
	}

	structSemNode CalSem(vector <int> vectfVect)
	{
		int nSize=vectfVect.size();
		int i;

		float fMean=0.f,fSem=0.f;
		for(i=0;i<nSize;i++)
		{
			fMean+=vectfVect[i];
		}

		fMean/=float(nSize);

		for(i=0;i<nSize;i++)
		{
			fSem+=(fMean-vectfVect[i])*(fMean-vectfVect[i]);
		}

		if(nSize>1)
		{
			fSem/=float(nSize*(nSize-1));
			fSem=sqrt(fSem);
		}

		structSemNode aNode;

		aNode.fMean=fMean;
		aNode.fSem=fSem;
		aNode.nSampleSize=nSize;

		return aNode;
	}


	CString ExtendFileName(CString sFileName, char ch)
	{
		CString sNewFileName=sFileName;
		int nLength=sFileName.GetLength();
		sNewFileName.Insert(nLength-4,ch);
		return sNewFileName;
	}

	CString ExtendFileName(CString sFileName, CString sInsert)
	{
		CString sNewFileName=sFileName;
		int nLength=sInsert.GetLength();
		for(int i=0;i<nLength;i++)
		{
			sNewFileName=ExtendFileName(sNewFileName,sInsert[i]);
		}
		return sNewFileName;
	}


	void OutputFormattedTime(FILE *fpOutput, CString sComments, CTime aTime)
	{
		CString sTemp = aTime.Format( "%H:%M:%S On " );
		fprintf(fpOutput,"%s  %s ",sComments,sTemp);
		sTemp = aTime.Format( "%A, %B %d, %Y" );
		fprintf(fpOutput,"%s  \n",sTemp);
	}

	void OutputFormattedTimeSpan(FILE *fpOutput, CString sComments, CTimeSpan aTime)
	{
		CString sTemp = aTime.Format( "%D Days, %H Hours, %M Minutes and %S Seconds \n" );
		fprintf(fpOutput,"%s  %s ",sComments,sTemp);
	}
}

#endif // !defined(AFX_EVLNODE_H__729A080D_5FE1_11D4_83DE_00C04F201B5A__INCLUDED_)
