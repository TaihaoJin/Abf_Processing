// FittingSummary.cpp: implementation of the CFittingSummary class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "Abf_Processing.h"
#include "FittingSummary.h"
#include "math.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CFittingSummary::CFittingSummary()
{
}

CFittingSummary::~CFittingSummary()
{

}

void CFittingSummary::SummarizeFittings()
{
	ReadFittingResults();
	ExportCurves();
	vector <float> vectfP;
	vector <float> vectfT;
	vector <float> vectfTMean;
	vector <float> vectfTSem;
	vector <float> vectfPMean;
	vector <float> vectfPSem;
	vector <float> vectfMean;
	vectfMean.clear();
	structSemNode aNode;
	float fMean,fSem;
	int i;
	for(i=0;i<m_nNumComponents;i++)
	{
		vectfP.clear();
		vectfT.clear();
		for(int j=0;j<m_nNumFiles;j++)
		{
			vectfP.push_back(m_vectFitting[j].dWeights[i]);
			vectfT.push_back(m_vectFitting[j].dTimeConstants[i]);
			if(i==0)
			{
				vectfMean.push_back(m_vectFitting[j].dMean);
			}
		}
		aNode=CalSem(vectfP);
		vectfPMean.push_back(aNode.fMean);
		vectfPSem.push_back(aNode.fSem);

		aNode=CalSem(vectfT);
		vectfTMean.push_back(aNode.fMean);
		vectfTSem.push_back(aNode.fSem);

		if(i==0)
		{
			aNode=CalSem(vectfMean);
			fMean=aNode.fMean;
			fSem=aNode.fSem;
		}
	}

	CString NewExt="txt";
	CString sFile=ChangeExt(m_sFittingSummaryFile,NewExt);
	FILE* fp=fopen(sFile,"w");
	
	for(i=0;i<m_nNumComponents;i++)
	{
		fprintf(fp,"       TMean%1d        fTSem%1d ",i+1,i+1);
	}

	for(i=0;i<m_nNumComponents;i++)
	{
		fprintf(fp,"    PMean%1d      PSem%1d ",i+1,i+1);
	}
			
	fprintf(fp,"        TMean          TSem   N    FileName\n");
	
	for(i=0;i<m_nNumComponents;i++)
	{
		fprintf(fp," %12.1f  %12.1f ",vectfTMean[i],vectfTSem[i]);
	}

	for(i=0;i<m_nNumComponents;i++)
	{
		fprintf(fp," %9.5f  %9.5f ",vectfPMean[i],vectfPSem[i]);
	}
			
	fprintf(fp," %12.1f  %12.1f %3d    %s\n",fMean,fSem,m_nNumFiles,m_sFittingSummaryFile);
	fclose(fp);
}

void CFittingSummary::ReadFittingResults()
{
	CString sFittingSummaryName="aFile";
	CString sFileNames="bFile";
	char  sTemp[200];
	CFileDialog aFileDlg( true, NULL, NULL, OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, NULL, NULL );
	char* pch="Fitting Summary Files (*.FIT)\0*.FIT\0All Files\0*.*\0\0";
	aFileDlg.m_ofn.lpstrFilter=pch;
	float fFitting0,fFitting1,fScaling;

	if(aFileDlg.DoModal()==IDOK)
	{
		m_sFittingSummaryFile=aFileDlg.GetPathName();
		FILE* fp=fopen(m_sFittingSummaryFile,"r");
		fscanf(fp,"%s%d%s%d",sTemp,&m_nDwellLevel,sTemp,&m_nNumComponents);

		float fP,fS,fT,fMean;
		m_nNumFiles=0;
		CString aString="NewFile";
		while(!feof(fp))
		{
			CString EvlFileName;
			m_nNumFiles++;
			structFittingResultNode aNode;
			aNode.nDwellLevel=m_nDwellLevel;
			aNode.nNumComponents=m_nNumComponents;
			fscanf(fp,"%s",EvlFileName);
			aNode.sFileName=EvlFileName;
			fMean=0.f;
			fScaling=0.f;
			fFitting0=0.79;
			fFitting1=75000.82f;
//			fscanf(fp,"%s%f%s%f",sTemp,&fFitting0,sTemp,&fFitting1);
			for(int i=0;i<m_nNumComponents;i++)
			{
				fscanf(fp,"%f%f%f%s",&fP,&fS,&fT,sTemp);
				aNode.dWeights[i]=fP;
//				fS=fS/(exp(-fFitting0/fT)-exp(-fFitting1/fT));
//				fS=fS*fS;
				fScaling+=fP*(exp(-fFitting0/fT)-exp(-fFitting1/fT));
				aNode.dScalings[i]=fS;
				aNode.dTimeConstants[i]=fT;
				fMean+=fP*fT;
			}
			aNode.dMean=fMean;
			aNode.dCorrection=1./fScaling;
			m_vectFitting.push_back(aNode);
		}
		fclose(fp);
	}
}

CString CFittingSummary::ChangeExt(CString sFileName, CString sNewExt)
{
	int i=0,nLength=0;
	CString sNewName=sFileName;
	nLength=sFileName.GetLength();
	for(i=0;i<3;i++)
	{
		sNewName.SetAt(nLength-3+i,sNewExt[i]);
	}
	return sNewName;
}

void CFittingSummary::ExportCurves()
{
	int i,j;
	int nSize=m_vectFitting.size();
	structFittingResultNode aNode;
	CString sFileName;
	CString sNewExt="cuv";
	FILE* fp;
	float fTs,fTl,fTc;
	float fTemp,fScaling,fTimeConstant,dX1,dX2,dDelta,dX,fTemp0;
	dDelta=0.01f;
	float fBinWidth=0.05f;
	float fCorrection;
	for(i=0;i<nSize;i++)
	{
		sFileName=m_vectFitting[i].sFileName;
		sFileName=ChangeExt(sFileName,sNewExt);
		fp=fopen(sFileName,"w");
		{
			dX=-4.f;
			fCorrection=m_vectFitting[i].dCorrection;
			fprintf(fp,"  log(t)      time(ms)");
			for(j=0;j<m_nNumComponents;j++)
			{
				fprintf(fp,"   Comp:%1d",j+1);
			}
			fprintf(fp,"      Sum");

			for(j=1;j<m_nNumComponents;j++)
			{
				fTs=m_vectFitting[i].dTimeConstants[j-1];
				fTl=m_vectFitting[i].dTimeConstants[j];
				fTc=BurstDelimiter(fTs,fTl);
				fprintf(fp,"    Tc%1d:  %10.2f",j+1,fTc);
			}
			fprintf(fp,"\n");

			while(dX<7.)
			{
				dX+=dDelta;
				dX2=dX+fBinWidth;
				dX2=exp(dX2*log(10.));
				dX1=exp(dX*log(10.));
//				fprintf(fp,"%8.2f  %12.2f",dX+.5*fBinWidth, exp((dX+.5*fBinWidth)*log(10.)));
				fprintf(fp,"%8.2f  %12.2f",dX, exp(dX*log(10.)));

				fTemp=0.f;
				for(j=0;j<m_nNumComponents;j++)
				{
					fScaling=m_vectFitting[i].dScalings[j];
					fTimeConstant=m_vectFitting[i].dTimeConstants[j];
//					fTemp0=sqrt(fScaling*exp(-dX1/fTimeConstant)/fTimeConstant);
//					fTemp0=sqrt(fScaling*(exp(-dX1/fTimeConstant)-exp(-dX2/fTimeConstant)));
//					fTemp0=sqrt(fScaling*(exp(-dX1/fTimeConstant)-exp(-dX2/fTimeConstant)))*fBinWidth/dDelta;
//					fTemp0=sqrt(fScaling*(exp(-dX1/fTimeConstant)-exp(-dX2/fTimeConstant)))*m_vectFitting[i].dCorrection;
					fTemp0=sqrt(fScaling*(exp(-dX1/fTimeConstant)-exp(-dX2/fTimeConstant)));
					fprintf(fp," %8.2f",fTemp0);
					fTemp+=fTemp0*fTemp0;
				}
				fprintf(fp," %8.2f\n",sqrt(fTemp));
			}
		}
		fclose(fp);
	}
}

structSemNode CFittingSummary::CalSem(vector <float> vectfVect)
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


double CFittingSummary::CriticalFunction(float Ts, float Tl, float T)
{
	//Two components have equal proportion of mistakes
	double dValue=0.;
	dValue=1.-exp(-T/Tl)-exp(-T/Ts);
	return dValue;
}

double CFittingSummary::CriticalFunction1(float fTs, float fAs, float fTl, float fAl, float fT)
{
	//Two components have equal number of mistakes
	double dValue=0.;
	dValue=fAs*exp(-fT/fTs)-fAl*(1.f-exp(-fT/fTl));
	return dValue;
}

double CFittingSummary::CriticalFunction2(float fTs, float fAs, float fTl, float fAl, float fT)
{
	//Minimize the total number of mistakes
	double dValue=0.;
	dValue=(fAs/fTs)*exp(-fT/fTs)-(fAl/fTl)*exp(-fT/fTl);
	return dValue;
}

float CFittingSummary::BurstDelimiter(float fTs, float fTl)
{
	float fT1, fT2, fT;
	double dX,dX0;
	fT1=0.; 
	fT2=3.*fTl;
	fT=fT2;
	dX=CriticalFunction(fTs,fTl,fT);
//	fTs=4.9;
//	fTl=31.;
	dX0=dX;
	if(dX<0.)
	{
		dX0=-dX;
	}
	while(dX0>0.000001)
	{
		if(dX>0.)
		{
			fT2=fT;
		}
		else
		{
			fT1=fT;
		}
		fT=0.5*(fT1+fT2);
		dX=CriticalFunction(fTs,fTl,fT);
		dX0=dX;
		if(dX<0.)
		{
			dX0=-dX;
		}
	}
	return fT;
}
