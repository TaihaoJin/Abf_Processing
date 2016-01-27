// EvlDwellingTimeFitting.cpp: implementation of the CEvlDwellingTimeFitting class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "Abf_Processing.h"
#include "EvlDwellingTimeFitting.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CEvlDwellingTimeFitting::CEvlDwellingTimeFitting()
{

}

CEvlDwellingTimeFitting::~CEvlDwellingTimeFitting()
{

}

#include "nr.h"

namespace EvlGlobal_CEvlDwellingTimeFitting {
	structDwellFittingTrialNode g_TrialNode;
	structEvlHistogramNode g_EvlHistNode;
	structFittingResultNode g_ResultNode;
	structEvlDTFittingConstraintsNode g_ConstraintsNode;

	double CumulatedDwellProb(double dT[], double dW[], int nComponents, double dTime)
	{
		double dResult=0.;
		double dTemp;
		for(int i=0;i<nComponents;i++)
		{
			dTemp=exp(-dTime/dT[i]);
			dTemp*=dW[i];
			dResult-=dTemp;
		}

		dResult+=1.;

		return dResult;
	}

	double CumulatedDwellProb_C(double dT[], double dW[], int nComponents, double dTime)
	{
		double dResult=0.;
		double dTemp;
		for(int i=0;i<nComponents;i++)
		{
			dTemp=exp(-dTime/dT[i]);
			dTemp*=dW[i];
			dResult+=dTemp;
		}
		return dResult;
	}

	double PartitionFunction(double x[], int nDim)
	{
		double dResult=0.f;
		for(int i=0;i<nDim;i++)
		{
			dResult+=exp(-x[i]);
		}
		return dResult;
	}

	double Area_Exp(double dT[], double dW[], int nDim, double dLeft, double dRight)
	{
		double dArea=0.;
		for(int i=0;i<nDim;i++)
		{
			dArea+=dW[i]*(exp(-dLeft/dT[i])-exp(-dRight/dT[i]));
		}
		return dArea;
	}

	void TimeConstantsAndWeights(double dT[], double dW[], Vec_I_DP &x)
	{
		double dG[20];
		double dG0=1.;
		int i;
		int nDimT=g_ConstraintsNode.nDimT;
		int nDimW=g_ConstraintsNode.nDimW;

		dG[0]=dG0;
		for(i=0;i<nDimW-1;i++)
		{
			dG[i+1]=x[nDimT+i];
		}

		double dZ=PartitionFunction(dG,nDimW);

		const int nTemp0=g_ConstraintsNode.nvW0[0];
		int nTemp=0;

		for(i=0;i<nDimT;i++)
		{
			nTemp=g_ConstraintsNode.nvT0[i];
			dT[nTemp]=g_TrialNode.fTimeConstants[nTemp]*exp(x[i]-1);
		}

		float fW0=g_ConstraintsNode.fW0;

		for(i=0;i<nDimW;i++)
		{
			nTemp=g_ConstraintsNode.nvW0[i];
			dW[nTemp]=fW0*exp(-dG[i])/dZ;
		}

		double dTemp=0.;
		for(i=1;i<nDimW;i++)
		{
			nTemp=g_ConstraintsNode.nvW0[i];
			dTemp+=dW[nTemp];
		}

		dW[nTemp0]=fW0-dTemp;

		nDimT=g_ConstraintsNode.nNumComponents-nDimT;
		for(i=0;i<nDimT;i++)
		{
			nTemp=g_ConstraintsNode.nvTc[i];
			dT[nTemp]=g_TrialNode.fTimeConstants[nTemp];
		}

		nDimW=g_ConstraintsNode.nNumComponents-nDimW;
		for(i=0;i<nDimW;i++)
		{
			nTemp=g_ConstraintsNode.nvWc[i];
			dW[nTemp]=g_TrialNode.fWeights[nTemp];
		}
	}

	double LogLikelihood_Transform(Vec_I_DP &x)
	{
		//This function returns the negative value of the LogLikelihood
		int nFittingLevel=g_TrialNode.nDwellLevel;
		int nSize=x.size();
		double dTimeConstants[20],dWeights[20];
		int i;
		double dTemp=0.;
		double dBigNegative=-9999999999999999999999999999999999999.;
		double dTiny=0.0000000000000000000000000000000000001;
		//Remembr to implement a soft boundary condition later at this point

		int nComponents=g_TrialNode.nNumComponents;
		TimeConstantsAndWeights(dTimeConstants,dWeights,x);

		int nNumBins=g_EvlHistNode.vectFLogHist.at(nFittingLevel).size();
		double dTmin=g_EvlHistNode.vectFRLogBins.at(nFittingLevel)[0].fLeft;
		double dTmax=g_EvlHistNode.vectFRLogBins.at(nFittingLevel)[nNumBins-1].fRight;
		double dLog10=log(10.);
		dTmin=exp(dTmin*dLog10);
		dTmax=exp(dTmax*dLog10);

//		double dArea=CumulatedDwellProb(dTimeConstants,dWeights,nComponents,dTmax);
//		dArea-=CumulatedDwellProb(dTimeConstants,dWeights,nComponents,dTmin);
		double dArea=Area_Exp(dTimeConstants,dWeights,nComponents,dTmin,dTmax);

		//For Sigworth
	//	for(i=0;i<nComponents;i++)
	//	{
	//		if(dTimeConstants[i]<dTmin) 
	//		{
	//			return -dBigNegative;
	//		}
	//	}

		double dLogLikelihood=0.,dLeft=0.,dRight=0.,dTemp1=0.;

		for(i=0;i<nNumBins;i++)
		{
			dLeft=g_EvlHistNode.vectFRLogBins.at(nFittingLevel)[i].fLeft;
			dRight=g_EvlHistNode.vectFRLogBins.at(nFittingLevel)[i].fRight;
			dLeft=exp(dLeft*dLog10);
			dRight=exp(dRight*dLog10);
			dTemp=Area_Exp(dTimeConstants,dWeights,nComponents,dLeft,dRight);
//			if(dTemp<dTiny) return -dBigNegative;
			dTemp1=g_EvlHistNode.vectFLogHist.at(nFittingLevel)[i];
			dTemp=dTemp1*log(dTemp/dArea)/dLog10;
			dLogLikelihood+=dTemp;
		}

		if(dArea<dTiny) return -dBigNegative;
			

		return -dLogLikelihood;
	}


	double LogLikelihood_HB_Transform(Vec_I_DP &x)
	{
		//This function returns the negative value of the LogLikelihood
		int nFittingLevel=g_TrialNode.nDwellLevel;
		int nSize=x.size();
		double dTimeConstants[20],dWeights[20];
		int i;
		double dTemp=0.;
		double dBigNegative=-9999999999999999999999999999999999999.;
		double dTiny=4.9406564584125e-324;
		float fTiny=1.40130e-045;
		//Remembr to implement a soft boundary condition later at this point

		int nComponents=g_TrialNode.nNumComponents;
		TimeConstantsAndWeights(dTimeConstants,dWeights,x);

		int nNumBins=g_EvlHistNode.vectFLogHist_HB.at(nFittingLevel).size();
		double dTmin=g_EvlHistNode.vectFRLogBins_HB.at(nFittingLevel)[0].fLeft;
		double dTmax=g_EvlHistNode.vectFRLogBins_HB.at(nFittingLevel)[nNumBins-1].fRight;
		double dLog10=log(10.);
		dTmin=exp(dTmin*dLog10);
		dTmax=exp(dTmax*dLog10);

//		double dArea=CumulatedDwellProb(dTimeConstants,dWeights,nComponents,dTmax);
//		dArea-=CumulatedDwellProb(dTimeConstants,dWeights,nComponents,dTmin);

		double dArea=Area_Exp(dTimeConstants,dWeights,nComponents,dTmin,dTmax);

	//	dArea=1.;

		double dLogLikelihood=0.,dLeft=0.,dRight=0.,dTemp1=0.;

		for(i=0;i<nNumBins;i++)
		{
			dLeft=g_EvlHistNode.vectFRLogBins_HB.at(nFittingLevel)[i].fLeft;
			dRight=g_EvlHistNode.vectFRLogBins_HB.at(nFittingLevel)[i].fRight;
			dLeft=exp(dLeft*dLog10);
			dRight=exp(dRight*dLog10);
			dTemp=Area_Exp(dTimeConstants,dWeights,nComponents,dLeft,dRight);
//			if(dTemp<dTiny) return -dBigNegative;
			dTemp1=g_EvlHistNode.vectFLogHist_HB.at(nFittingLevel)[i];
			dTemp=dTemp1*log(dTemp/dArea)/dLog10;
			dLogLikelihood+=dTemp;
		}

		if(dArea<dTiny) return -dBigNegative;
		return -dLogLikelihood;
	}

	double LogLikelihood_Transform(Vec_I_DP &x, int nFuncIndex)
	{
		//This function returns the negative value of the LogLikelihood
		int nFittingLevel=g_TrialNode.nDwellLevel;
		int nSize=x.size();
		double dTimeConstants[20],dWeights[20];
		int i;
		double dTemp=0.;
		double dBigNegative=-9999999999999999999999999999999999999.;
		double dTiny=4.9406564584125e-324;
		//Remembr to implement a soft boundary condition later at this point

		int nComponents=g_TrialNode.nNumComponents;
		TimeConstantsAndWeights(dTimeConstants,dWeights,x);

		int nNumBins=g_EvlHistNode.vectFLogHist_HB.at(nFittingLevel).size();
		double dTmin=g_EvlHistNode.vectFRLogBins_HB.at(nFittingLevel)[0].fLeft;
		double dTmax=g_EvlHistNode.vectFRLogBins_HB.at(nFittingLevel)[nNumBins-1].fRight;
		double dLog10=log(10.);
		dTmin=exp(dTmin*dLog10);
		dTmax=exp(dTmax*dLog10);

		double dArea=Area_Exp(dTimeConstants,dWeights,nComponents,dTmin,dTmax);
	//	dArea=1.;

		double dLogLikelihood=0.,dLeft=0.,dRight=0.,dTemp1=0.;

		switch (nFuncIndex)
		{
		case 1:
			for(i=0;i<nNumBins;i++)
			{
				dLeft=g_EvlHistNode.vectFRLogBins.at(nFittingLevel)[i].fLeft;
				dRight=g_EvlHistNode.vectFRLogBins.at(nFittingLevel)[i].fRight;
				dLeft=exp(dLeft*dLog10);
				dRight=exp(dRight*dLog10);
				dTemp=Area_Exp(dTimeConstants,dWeights,nComponents,dLeft,dRight);
				if(dTemp<dTiny) return -dBigNegative;
				dTemp1=g_EvlHistNode.vectFLogHist.at(nFittingLevel)[i];
				dTemp=dTemp1*log(dTemp/dArea)/dLog10;
				dLogLikelihood+=dTemp;
			}
			break;
		case 3:
			for(i=0;i<nNumBins;i++)
			{
				dLeft=g_EvlHistNode.vectFRLogBins_HB.at(nFittingLevel)[i].fLeft;
				dRight=g_EvlHistNode.vectFRLogBins_HB.at(nFittingLevel)[i].fRight;
				dLeft=exp(dLeft*dLog10);
				dRight=exp(dRight*dLog10);
				dTemp=Area_Exp(dTimeConstants,dWeights,nComponents,dLeft,dRight);
//				if(dTemp<dTiny) return -dBigNegative;
				dTemp1=g_EvlHistNode.vectFLogHist_HB.at(nFittingLevel)[i];
				dTemp=dTemp1*log(dTemp/dArea)/dLog10;
				dLogLikelihood+=dTemp;
			}
			break;
		}

		if(dArea<dTiny) return -dBigNegative;
		return -dLogLikelihood;
	}


	double ChiSQ_HB_Transform(Vec_I_DP &x)
	{
		//This function returns the negative value of the LogLikelihood
		int nDwellLevel=g_TrialNode.nDwellLevel;
		const double dLog10=log(10.);
		double dT[20],dW[20];
		int i=0;

		int nComponents=g_TrialNode.nNumComponents;
		TimeConstantsAndWeights(dT,dW,x);

		int nSize=g_EvlHistNode.vectFLogHist_HB.at(nDwellLevel).size();
		double dMin=g_EvlHistNode.vectFRLogBins_HB.at(nDwellLevel)[0].fLeft;
		dMin=exp(dMin*dLog10);
		double dMax=g_EvlHistNode.vectFRLogBins_HB.at(nDwellLevel)[nSize-1].fRight;
		dMax=exp(dMax*dLog10);
		double dChiSQ=0.,dTemp0=0.,dTemp1=0.,dTemp2;
		double dLeft,dRight;
		double dArea=Area_Exp(dT, dW, nComponents, dMin, dMax);
		double dEventsInRange=double(g_EvlHistNode.nEventsInRange_HB[nDwellLevel]);
		for(i=0;i<nSize;i++)
		{
			dLeft=g_EvlHistNode.vectFRLogBins_HB.at(nDwellLevel)[i].fLeft;
			dRight=g_EvlHistNode.vectFRLogBins_HB.at(nDwellLevel)[i].fRight;
			dLeft=exp(dLeft*dLog10);
			dRight=exp(dRight*dLog10);
			dTemp0=Area_Exp(dT, dW, nComponents, dLeft, dRight)/dArea;
			dTemp0*=dEventsInRange;
			dTemp1=g_EvlHistNode.vectFLogHist_HB.at(nDwellLevel)[i];
			dTemp2=dTemp1-dTemp0;
			dChiSQ+=dTemp2*dTemp2/dTemp0;
		}
		return dChiSQ;
	}


	double LogLikelihood_AP_Transform(Vec_I_DP &x)
	{
		//This function returns the negative value of the LogLikelihood
		//This function works on all-events instead of on histogram.

		int nFittingLevel=g_TrialNode.nDwellLevel;
		int nSize=x.size();
		double dTimeConstants[20],dWeights[20];
		int i,j;
		double dTemp=0.;
		double dBigNegative=-9999999999999999999999999999999999999.;
		double dTiny=4.9406564584125e-324;

		int nComponents=g_TrialNode.nNumComponents;

		int nNumBins=g_EvlHistNode.vectFLogHist.at(nFittingLevel).size();
		double dTmin=g_EvlHistNode.vectFRLogBins.at(nFittingLevel)[0].fLeft;
		double dTmax=g_EvlHistNode.vectFRLogBins.at(nFittingLevel)[nNumBins-1].fRight;
		double dLog10=log(10.);
		dTmin=exp(dTmin*dLog10);
		dTmax=exp(dTmax*dLog10);

		double dArea=CumulatedDwellProb_C(dTimeConstants,dWeights,nComponents,dTmin);
		if(dArea<dTiny) return -dBigNegative;		

		nSize=g_EvlHistNode.vectfDwellLength.at(nFittingLevel).size();
		double dLogLikelihood=0.,dTime=0.;
		double dW,dTol;
	//	double dLestLength=1000000000000000000.;

		for(i=0;i<nSize;i++)
		{
			dTemp=0.;
			dTime=g_EvlHistNode.vectfDwellLength.at(nFittingLevel)[i];
	//		if(dLestLength>dTime)
	//		{
	//			dLestLength=dTime;
	//		}
			for(j=0;j<nComponents;j++)
			{
				dW=dWeights[j];
				dTol=dTimeConstants[j];
				dTemp+=(dW*exp(-dTime/dTol))/dTol;
			}
			dLogLikelihood+=log(dTemp/dArea);
		}
		dLogLikelihood=dLogLikelihood/dLog10;

		return -dLogLikelihood;
	}

};

using namespace EvlGlobal_CEvlDwellingTimeFitting;


structFittingResultNode CEvlDwellingTimeFitting::FitEvl_Transform(int nFuncKey, structDwellFittingTrialNode aNode, structEvlHistogramNode structEvlHist)
{
	// nFuncKey 1=log: logarithmic bin, 2=AP: all events fitting:, 3=HB: hibrid bin
	m_structEvlHist=structEvlHist;
	int nComponents=aNode.nNumComponents;

	structFRange fFittingRange;
	fFittingRange=aNode.fFittingRange;

	structEvlDTFittingConstraintsNode aConstraintsNode=HandlingConstraints(aNode);

	g_TrialNode=aNode;
	g_EvlHistNode=m_structEvlHist;
	g_ConstraintsNode=aConstraintsNode;


	int MP=aConstraintsNode.nDimT+aConstraintsNode.nDimW,NP=MP-1;

    const DP FTOL=1.0e-10;
    int i,nfunc,j;
    Vec_DP x(NP),y(MP);
    Mat_DP p(MP,NP);

	double temp[40];
	double dTemp;

	int nTempDimT=aConstraintsNode.nDimT;

	for(i=0;i<nTempDimT;i++)
	{
		temp[i]=1.;
	}

	double ai,aj;
	double dG0=1.;
	int nTemp0=aConstraintsNode.nvW0[0];
	int nTemp=0;
	int nTempDimW=aConstraintsNode.nDimW;

	for(i=0;i<nTempDimW-1;i++)
	{
		nTemp=aConstraintsNode.nvW0[i+1];
		ai=aNode.fWeights[nTemp];
		aj=aNode.fWeights[nTemp0];
		temp[i+nTempDimT]=log(aj)-log(ai)+dG0;
	}

	for(j=0;j<NP;j++)
	{
		dTemp=temp[j];
		x[j]=dTemp;
		p[0][j]=dTemp;
	}

	switch (nFuncKey)
	{
		case 1:
			y[0]=LogLikelihood_Transform(x);
			break;
		case 2:
			y[0]=LogLikelihood_AP_Transform(x);
			break;
		case 3:
			y[0]=LogLikelihood_HB_Transform(x);
			break;
	}

	for(i=1;i<MP;i++)
	{
		for(j=0;j<NP;j++)
		{
			if(j+1==i)
			{
//				if(j<nComponents-1)
				{
					dTemp=1.1*temp[j];
//					dTemp=1.5*temp[j];
				}
//				else
				{
					dTemp=1.1*temp[j];
//					dTemp=0.;
				}
			}
			else
			{
//				dTemp=temp[j];
				dTemp=0.;
			}
			x[j]=dTemp;
			p[i][j]=dTemp;
		}
		switch (nFuncKey)
		{
			case 1:
				dTemp=LogLikelihood_Transform(x);
				y[i]=dTemp;
				break;
			case 2:
				dTemp=LogLikelihood_AP_Transform(x);
				y[i]=dTemp;
				break;
			case 3:
				dTemp=LogLikelihood_HB_Transform(x);
				y[i]=dTemp;
				break;
		}
	}


	switch (nFuncKey)
	{
		case 1:
			NR::amoeba(p,y,FTOL,LogLikelihood_Transform,nfunc);
			break;
		case 2:
			NR::amoeba(p,y,FTOL,LogLikelihood_AP_Transform,nfunc);
			break;
		case 3:
			NR::amoeba(p,y,FTOL,LogLikelihood_HB_Transform,nfunc);
			break;
	}
	
	structFittingResultNode bNode;

	int nNumComponents=aNode.nNumComponents;

	for(i=0;i<NP;i++)
	{
		x[i]=p[0][i];
	}


	TimeConstantsAndWeights(bNode.dTimeConstants,bNode.dWeights,x);

	Vec_IO_DP dTimeConstants(nNumComponents);
	Vec_IO_DP dWeights(nNumComponents);
	int nTrial=0;
	for(nTrial=0;nTrial<nNumComponents;nTrial++)
	{
		dTimeConstants[nTrial]=bNode.dTimeConstants[nTrial];
		dWeights[nTrial]=bNode.dWeights[nTrial];
	}

	NR::sort2(dTimeConstants,dWeights);

	double dT[20],dWT[20];

	for(nTrial=0;nTrial<nNumComponents;nTrial++)
	{
		bNode.dTimeConstants[nTrial]=dTimeConstants[nTrial];
		dT[nTrial]=dTimeConstants[nTrial];
		bNode.dWeights[nTrial]=dWeights[nTrial];
		dWT[nTrial]=dWeights[nTrial];
	}

	int nFittingLevel=aNode.nDwellLevel;
	int nNumBins=0;
	double dTmin=0.;
	double dTmax=0.;
	double dEventsInRange=0;

	nFittingLevel=aNode.nDwellLevel;
	for(i=0;i<=nFittingLevel;i++)
	{
		bNode.nLargestHybridBin[i]=m_structEvlHist.nLargestHybridBin[i];
		bNode.nLargestLogBin[i]=m_structEvlHist.nLargestLogBin[i];
	}


	switch (nFuncKey)
	{
		case 1:
			nNumBins=g_EvlHistNode.vectFLogHist.at(nFittingLevel).size();
			dTmin=g_EvlHistNode.vectFRLogBins.at(nFittingLevel)[0].fLeft;
			dTmax=g_EvlHistNode.vectFRLogBins.at(nFittingLevel)[nNumBins-1].fRight;
			for(i=0;i<nNumBins;i++)
			{
				dEventsInRange+=g_EvlHistNode.vectFLogHist.at(nFittingLevel)[i];
			}
			bNode.sBinMethod="Log";
			bNode.nLargestBin=bNode.nLargestLogBin[nFittingLevel];
			break;
		case 2:
			nNumBins=g_EvlHistNode.vectFLogHist.at(nFittingLevel).size();
			dTmin=g_EvlHistNode.vectFRLogBins.at(nFittingLevel)[0].fLeft;
			dTmax=g_EvlHistNode.vectFRLogBins.at(nFittingLevel)[nNumBins-1].fRight;
			for(i=0;i<nNumBins;i++)
			{
				dEventsInRange+=g_EvlHistNode.vectFLogHist.at(nFittingLevel)[i];
			}
			bNode.sBinMethod="AP";
			bNode.nLargestBin=bNode.nLargestLogBin[nFittingLevel];
			break;
		case 3:
			nNumBins=g_EvlHistNode.vectFLogHist_HB.at(nFittingLevel).size();
			dTmin=g_EvlHistNode.vectFRLogBins_HB.at(nFittingLevel)[0].fLeft;
			dTmax=g_EvlHistNode.vectFRLogBins_HB.at(nFittingLevel)[nNumBins-1].fRight;
			for(i=0;i<nNumBins;i++)
			{
				dEventsInRange+=g_EvlHistNode.vectFLogHist_HB.at(nFittingLevel)[i];
			}
			bNode.sBinMethod="Hybrid";
			bNode.nLargestBin=bNode.nLargestHybridBin[nFittingLevel];
			break;
	}

	double dLog10=log(10.);
	dTmin=exp(dTmin*dLog10);
	dTmax=exp(dTmax*dLog10);

	double dArea=CumulatedDwellProb(bNode.dTimeConstants,bNode.dWeights,nComponents,dTmax);
	dArea-=CumulatedDwellProb(bNode.dTimeConstants,bNode.dWeights,nComponents,dTmin);


	double dMean=0.;
	for(i=0;i<nComponents;i++)
	{
		dMean+=bNode.dTimeConstants[i]*bNode.dWeights[i];
	}

	int nEventsInRange=int(dEventsInRange+0.5);

	bNode.nFuncKey=nFuncKey;
	bNode.dFittedArea=dArea;
	bNode.fFittingRange.fLeft=dTmin;
	bNode.fFittingRange.fRight=dTmax;
	bNode.dLogLikelihood=-y[0];
	bNode.dMean=dMean;
	bNode.nDwellLevel=nFittingLevel;
	bNode.nEventsInRange=nEventsInRange;
	bNode.nNumComponents=nNumComponents;
	bNode.nNumIterations=nfunc;
	bNode.sFileName=aNode.sEvlFileName;
	bNode.nTrialNumber=aNode.nTrialNumber;
	bNode.dSDev=SDev(bNode.dTimeConstants,bNode.dWeights, nNumComponents, nFittingLevel, bNode.fFittingRange, nEventsInRange);
	bNode.dSSE=SSE(bNode.dTimeConstants,bNode.dWeights, nNumComponents, nFittingLevel, bNode.fFittingRange, nEventsInRange);
	bNode.dChiSQ=ChiSQ(bNode.dTimeConstants,bNode.dWeights, nNumComponents, nFittingLevel, bNode.fFittingRange, nEventsInRange);
	double dMeanDwellTimeH,dMeanDwellTimeHC;
	MeanDwellTimeHist(bNode.dTimeConstants,bNode.dWeights, nNumComponents, nFittingLevel, bNode.fFittingRange, dMeanDwellTimeH, dMeanDwellTimeHC, nEventsInRange);
	bNode.dMeanDwellTimeH=dMeanDwellTimeH;
	bNode.dMeanDwellTimeHC=dMeanDwellTimeHC;
	bNode.aTrialNode=aNode;
	bNode.nClusterIndex=-1;
//	bNode.dScalings=fScaling;
//	bNode.nNumEvents=m_nNumEvents;
//	Summarize();
//	structEvlSummaryNode aEvlSummaryNode=GetSummaryNode();
//	bNode.fRecordLength=aEvlSummaryNode.fRecordLength;
//	bNode.nNumEvents=aEvlSummaryNode.nNumEvents;
//	bNode.nMaxLevel=aEvlSummaryNode.nMaxLevel;
//	bNode.nNumChannels=aEvlSummaryNode.nNumChannels;
//	bNode.fOpenProb=aEvlSummaryNode.fOpenProb;
//	bNode.fMeanOpenTime=aEvlSummaryNode.fMeanOpenTime;
//	bNode.fProbSingle=aEvlSummaryNode.fProbSingle;
//	bNode.dMeanDwellTimeO=MeanDwellTimeInRange(bNode);
//	bNode.dMeanDwellTimeC=MeanFittedDwellTime(bNode);
//	bNode.aTrialNode=aNode;

//	ExportDwellCurves(bNode);

	for(i=0;i<nComponents;i++)
	{
		bNode.fConfidanceRangeT[i].fLeft=-1.;
		bNode.fConfidanceRangeT[i].fRight=-1.;
		bNode.fConfidanceRangeW[i].fLeft=-1.;
		bNode.fConfidanceRangeW[i].fRight=-1.;
		bNode.fConfidanceRangeTL[i].fLeft=-1.;
		bNode.fConfidanceRangeTL[i].fRight=-1.;
		bNode.fConfidanceRangeWL[i].fLeft=-1.;
		bNode.fConfidanceRangeWL[i].fRight=-1.;
	}

	structDwellFittingTrialNode aNode0=GetNextTrialNode(bNode);


	double dLogLik0=-LogLikelihood_Local(aNode0,nFuncKey);
	double dLogLik=bNode.dLogLikelihood;

//	if(aNode.bConfidanceRange)
//	{
//		bNode.bConfidanceRange=false;
//		bNode.aTrialNode.bConfidanceRange=false;
//		FitEvl_ConfidanceRange(nFuncKey, bNode, m_structEvlHist);
//	}

	return bNode;
}



double CEvlDwellingTimeFitting::CumulatedDwellProb(double dT[], double dW[], int nComponents, double dTime)
{
		double dResult=0.;
		double dTemp;
		for(int i=0;i<nComponents;i++)
		{
			dTemp=exp(-dTime/dT[i]);
			dTemp*=dW[i];
			dResult-=dTemp;
		}

		dResult+=1.;

		return dResult;
}


double CEvlDwellingTimeFitting::SSE(double dT[], double dW[], int nDim, int nDwellLevel, structFRange fRange, int nEventsInRange)
{
	int nSize=m_structEvlHist.vectFLogHist_HB.at(nDwellLevel).size();
	double dMin=fRange.fLeft;
	double dMax=fRange.fRight;
	double dSSE=0.,dTemp0=0.,dTemp1=0.,dTemp2;
	double dLeft,dRight;
	double dArea=Area_Exp(dT, dW, nDim, dMin, dMax);
	double dLog10=log(10.);
	int i;
	for(i=0;i<nSize;i++)
	{
		dLeft=m_structEvlHist.vectFRLogBins_HB.at(nDwellLevel)[i].fLeft;
		dRight=m_structEvlHist.vectFRLogBins_HB.at(nDwellLevel)[i].fRight;
		dLeft=exp(dLeft*dLog10);
		dRight=exp(dRight*dLog10);
		dTemp0=Area_Exp(dT, dW, nDim, dLeft, dRight)/dArea;
		dTemp0*=double(nEventsInRange);
		dTemp1=m_structEvlHist.vectFLogHist_HB.at(nDwellLevel)[i];
		dTemp2=dTemp1-dTemp0;
		dSSE+=dTemp2*dTemp2;
	}
	return dSSE;
}

double CEvlDwellingTimeFitting::ChiSQ(double dT[], double dW[], int nDim, int nDwellLevel, structFRange fRange, int nEventsInRange)
{
	int nSize=m_structEvlHist.vectFLogHist_HB.at(nDwellLevel).size();
	double dMin=fRange.fLeft;
	double dMax=fRange.fRight;
	double dChiSQ=0.,dTemp0=0.,dTemp1=0.,dTemp2;
	double dLeft,dRight;
	double dArea=Area_Exp(dT, dW, nDim, dMin, dMax);
	double dLog10=log(10.);
	int i;
	for(i=0;i<nSize;i++)
	{
		dLeft=m_structEvlHist.vectFRLogBins_HB.at(nDwellLevel)[i].fLeft;
		dRight=m_structEvlHist.vectFRLogBins_HB.at(nDwellLevel)[i].fRight;
		dLeft=exp(dLeft*dLog10);
		dRight=exp(dRight*dLog10);
		dTemp0=Area_Exp(dT, dW, nDim, dLeft, dRight)/dArea;
		dTemp0*=double(nEventsInRange);
		dTemp1=m_structEvlHist.vectFLogHist_HB.at(nDwellLevel)[i];
		dTemp2=dTemp1-dTemp0;
		dChiSQ+=dTemp2*dTemp2/dTemp0;
	}
	return dChiSQ;
}

void CEvlDwellingTimeFitting::MeanDwellTimeHist(double dT[], double dW[], int nDim, int nDwellLevel, structFRange fRange, double& dMeanH, double& dMeanHC, int nEventsInRange)
{
	int nSize=m_structEvlHist.vectFLogHist_HB.at(nDwellLevel).size();
	double dMin=fRange.fLeft;
	double dMax=fRange.fRight;
	double dTemp0=0.,dTemp1=0.;
	double dLeft,dRight;
	double dArea=Area_Exp(dT, dW, nDim, dMin, dMax);
	double dLog10=log(10.);
	int i;
	dMeanH=0.;
	dMeanHC=0.;
	double dTotal=0.;
	double dTotalC=0.;
	for(i=0;i<nSize;i++)
	{
		dLeft=m_structEvlHist.vectFRLogBins_HB.at(nDwellLevel)[i].fLeft;
		dRight=m_structEvlHist.vectFRLogBins_HB.at(nDwellLevel)[i].fRight;
		dLeft=exp(dLeft*dLog10);
		dRight=exp(dRight*dLog10);
		dTemp0=Area_Exp(dT, dW, nDim, dLeft, dRight)/dArea;
		dTemp0*=double(nEventsInRange);
		dTemp1=m_structEvlHist.vectFLogHist_HB.at(nDwellLevel)[i];
		dTotal+=dTemp1;
		dTotalC+=dTemp0;

		dMeanH+=dTemp1*0.5*(dLeft+dRight);
		dMeanHC+=dTemp0*0.5*(dLeft+dRight);
	}
	dMeanH/=dTotal;
	dMeanHC/=dTotalC;
}

double CEvlDwellingTimeFitting::SDev(double dT[], double dW[], int nDim, int nDwellLevel, structFRange fRange, int nEventsInRange)
{
	int nSize=m_structEvlHist.vectFLogHist_HB.at(nDwellLevel).size();
	double dLog10=log(10.);
	double dMin=fRange.fLeft;
	double dMax=fRange.fRight;
	double dTemp0=0.,dTemp1=0.,dTemp2;
	double dLeft,dRight;
	double dArea=Area_Exp(dT, dW, nDim, dMin, dMax);
	vector <double> vectdDev;
	int i;
	double dMeanDev=0.;
	for(i=0;i<nSize;i++)
	{
		dLeft=m_structEvlHist.vectFRLogBins_HB.at(nDwellLevel)[i].fLeft;
		dRight=m_structEvlHist.vectFRLogBins_HB.at(nDwellLevel)[i].fRight;
		dLeft=exp(dLeft*dLog10);
		dRight=exp(dRight*dLog10);
		dTemp0=Area_Exp(dT, dW, nDim, dLeft, dRight)/dArea;
		dTemp0*=double(nEventsInRange);
		dTemp1=m_structEvlHist.vectFLogHist_HB.at(nDwellLevel)[i];
		dTemp2=dTemp1-dTemp0;
		dMeanDev+=dTemp2;
		vectdDev.push_back(dTemp2);
	}

	dMeanDev/=double(nSize);

	double dSDev=0.;

	for(i=0;i<nSize;i++)
	{
		dTemp0=vectdDev[i]-dMeanDev;
		dSDev+=dTemp0*dTemp0;
	}

	dSDev=sqrt(dSDev/double(nSize));

	return dSDev;
}

double CEvlDwellingTimeFitting::Area_Exp(double dT[], double dW[], int nDim, double dLeft, double dRight)
{
	double dArea=0.;
	for(int i=0;i<nDim;i++)
	{
		dArea+=dW[i]*(exp(-dLeft/dT[i])-exp(-dRight/dT[i]));
	}
	return dArea;
}

int CEvlDwellingTimeFitting::EventsInRange(int nDwellLevel)
{
	int nSize=m_structEvlHist.vectFLogHist.at(nDwellLevel).size();
	double dTotal=0.;
	for(int i=0;i<nSize;i++)
	{		
		dTotal+=m_structEvlHist.vectFLogHist.at(nDwellLevel)[i];
	}
	return int(dTotal+0.5);
}

double CEvlDwellingTimeFitting::MeanFittedDwellTime(structFittingResultNode aResultNode)
{
	double dTmin=aResultNode.fFittingRange.fLeft;
	double dBigTime=1.e99;
	double *dT=aResultNode.dTimeConstants;
	double *dW=aResultNode.dWeights;
	int nDim=aResultNode.nNumComponents;
	double dArea=Area_Exp(dT, dW, nDim, dTmin, dBigTime);

	double dMean=0.;
	for(int i=0;i<nDim;i++)
	{
		dMean+=dT[i]*dW[i]*exp(-dTmin/dT[i]);
	}
	dMean/=dArea;
	dMean+=dTmin;
	return dMean;
}


structEvlDTFittingConstraintsNode CEvlDwellingTimeFitting::HandlingConstraints(structDwellFittingTrialNode aNode)
{
	structEvlDTFittingConstraintsNode aConstraintsNode;
	int i;
	int nComponents=aNode.nNumComponents;
	aConstraintsNode.nNumComponents=nComponents;
	aConstraintsNode.nvT0.clear();
	aConstraintsNode.nvTc.clear();
	aConstraintsNode.nvW0.clear();
	aConstraintsNode.nvWc.clear();
	float fWc=0.f;

	for(i=0;i<nComponents;i++)
	{
		if(aNode.bFreezeT[i])
		{
			aConstraintsNode.nvTc.push_back(i);
		}
		else
		{
			aConstraintsNode.nvT0.push_back(i);
		}

		if(aNode.bFreezeW[i])
		{
			aConstraintsNode.nvWc.push_back(i);
			fWc+=aNode.fWeights[i];
		}
		else
		{
			aConstraintsNode.nvW0.push_back(i);
		}
	}

	aConstraintsNode.nDimT=aConstraintsNode.nvT0.size();
	aConstraintsNode.nDimW=aConstraintsNode.nvW0.size();
	aConstraintsNode.fW0=1.-fWc;
	aConstraintsNode.fWc=fWc;
	return aConstraintsNode;
}


void CEvlDwellingTimeFitting::FitEvl_ConfidanceRange(int nFuncKey, structFittingResultNode &aResultNode, structEvlHistogramNode structEvlHist)
{
	FILE* fpOutput=fopen("C:\\EPData\\G4ST\\S176P\\EDTA\\SC\\ConfidenceRanges\\Log.txt","a");
	m_structEvlHist=structEvlHist;
	structDwellFittingTrialNode aNode0=GetNextTrialNode(aResultNode);
	const int nComponents=aNode0.nNumComponents,nFittingLevel=aResultNode.nDwellLevel,nTrialNumber=aResultNode.nTrialNumber;
	const double dLogRange=2.;

	CTime startTime = CTime::GetCurrentTime();
	fprintf(fpOutput,"Level: %1d    Comp: %1d  Trial:  %2d \n",nFittingLevel,nComponents,nTrialNumber);
	OutputFormattedTime(fpOutput,"		Started at:  ", startTime);

	const structEvlDTFittingConstraintsNode aConstraintsNode0=HandlingConstraints(aNode0);

//	g_TrialNode=aNode0;
//	g_ConstraintsNode=aConstraintsNode0;
//	g_EvlHistNode=m_structEvlHist;

	int i,j,nIndex;
	const double dLogLik0=-LogLikelihood_Local(aNode0,nFuncKey);
	const double dLogLik1=dLogLik0-dLogRange;
	double dX0=0.,dX1=0.,dXm=0.,dDelta=4.;
	const double dTol=0.001;
	const double dTolL=0.05;

	structDwellFittingTrialNode bTrialNode;
	int nIterations=0;
	const int nMaxIterations=10;

	CTime endTime = CTime::GetCurrentTime();
	CTimeSpan elapsedTime = endTime - startTime;

	structFittingResultNode bResultNode,ClosestNode;
	double dClosest=60000000.;
	double dDeltaL=0.;
	double dDeltaAbs=0.;
	double dLogLikI=0;
	for(nIndex=0;nIndex<nComponents;nIndex++)
	{
		if(!aNode0.bFreezeT[nIndex])
		{
			for(int nSign=-1;nSign<=1;nSign+=2)
			{
				startTime = CTime::GetCurrentTime();
				dClosest=800.;

				nIterations=0;
				bTrialNode=GetNextTrialNode(aResultNode);
				dX0=aResultNode.dLogLikelihood;
				InitialPointT(bTrialNode,nIndex, dLogLik1, dLogLikI, nSign,nFuncKey);
				SetConfidanceRangeTI(aResultNode,bTrialNode,dLogLikI,aConstraintsNode0,nIndex,nSign);
				bTrialNode.bFreezeT[nIndex]=true;
				bResultNode=FitEvl_Transform(nFuncKey, bTrialNode, structEvlHist);

				dDeltaL=fabs(bResultNode.dLogLikelihood-dLogLik1);
				if(dDeltaL<dClosest)
				{
					PassResultNodeTWL(ClosestNode,bResultNode);
					dClosest=dDeltaL;
				}

				bTrialNode=GetNextTrialNode(bResultNode);
				bTrialNode.bFreezeT[nIndex]=false;
				ResetGlobalConstraints(bTrialNode);
				if(bResultNode.dLogLikelihood<=dLogLik1)
				{
					i=i;
				}
				InitialPointT(bTrialNode,nIndex, dLogLik1, dLogLikI, nSign,nFuncKey);

				dX1=bResultNode.dLogLikelihood;
				dDelta=fabs(dX0-dX1);
				while(nIterations<nMaxIterations&&dDeltaL>dTolL)
				{
					nIterations++;
					bTrialNode.bFreezeT[nIndex]=true;
					bResultNode=FitEvl_Transform(nFuncKey, bTrialNode, structEvlHist);

					dDeltaL=fabs(bResultNode.dLogLikelihood-dLogLik1);
					if(dDeltaL<dClosest)
					{
						PassResultNodeTWL(ClosestNode,bResultNode);
						dClosest=dDeltaL;
					}

					bTrialNode=GetNextTrialNode(bResultNode);
					bTrialNode.bFreezeT[nIndex]=false;
					ResetGlobalConstraints(bTrialNode);
					if(bResultNode.dLogLikelihood<=dLogLik1)
					{
						i=i;
					}
					InitialPointT(bTrialNode,nIndex, dLogLik1, dLogLikI, nSign,nFuncKey);
					dX0=dX1;
					dX1=bResultNode.dLogLikelihood;
					dDelta=fabs(dX0-dX1);
				}
				SetConfidanceRangeT(aResultNode,ClosestNode,aConstraintsNode0,nIndex,nSign);

				fprintf(fpOutput,"	Confidance Range Calculation for TimeConstants i=%1d nSign=%4d  Iterations: %2d\n", nIndex, nSign, nIterations);
				endTime = CTime::GetCurrentTime();
				elapsedTime = endTime - startTime;
				OutputFormattedTime(fpOutput, "		Finished at:  ", endTime);
				OutputFormattedTimeSpan(fpOutput, "		Elapsed Time: ", elapsedTime);
			}
		}
	}

	int nSize=aConstraintsNode0.nvW0.size();
	double dW0=aConstraintsNode0.fW0;

	if(nSize>1)
	{
		for(i=0;i<nSize;i++)
		{
			nIndex=aConstraintsNode0.nvW0[i];
			for(int nSign=-1;nSign<=1;nSign+=2)
			{
				startTime = CTime::GetCurrentTime();
				dClosest=800.;
				nIterations=0;
				bTrialNode=GetNextTrialNode(aResultNode);
				InitialPointW(bTrialNode,nIndex, dLogLik1, dLogLikI, nSign,nFuncKey,aConstraintsNode0);
				SetConfidanceRangeWI(aResultNode,bTrialNode,dLogLikI,aConstraintsNode0,nIndex,nSign);
				dX0=aResultNode.dLogLikelihood;
				bTrialNode.bFreezeW[nIndex]=true;
				bResultNode=FitEvl_Transform(nFuncKey, bTrialNode, structEvlHist);

				dDeltaL=fabs(bResultNode.dLogLikelihood-dLogLik1);
				if(dDeltaL<dClosest)
				{
					PassResultNodeTWL(ClosestNode,bResultNode);
					dClosest=dDeltaL;
				}

				bTrialNode=GetNextTrialNode(bResultNode);
				bTrialNode.bFreezeW[nIndex]=false;
				ResetGlobalConstraints(bTrialNode);
				InitialPointW(bTrialNode,nIndex, dLogLik1, dLogLikI, nSign,nFuncKey,aConstraintsNode0);

				dX1=bResultNode.dLogLikelihood;
				dDelta=fabs(dX0-dX1);
				while(nIterations<nMaxIterations&&dDeltaL>dTolL)
				{
					nIterations++;
					bTrialNode.bFreezeW[nIndex]=true;
					bResultNode=FitEvl_Transform(nFuncKey, bTrialNode, structEvlHist);

					dDeltaL=fabs(bResultNode.dLogLikelihood-dLogLik1);
					if(dDeltaL<dClosest)
					{
						PassResultNodeTWL(ClosestNode,bResultNode);
						dClosest=dDeltaL;
					}

					bTrialNode=GetNextTrialNode(bResultNode);
					bTrialNode.bFreezeW[nIndex]=false;
					ResetGlobalConstraints(bTrialNode);
					InitialPointW(bTrialNode,nIndex, dLogLik1, dLogLikI, nSign,nFuncKey,aConstraintsNode0);
					dX0=dX1;
					dX1=bResultNode.dLogLikelihood;
					dDelta=fabs(dX0-dX1);
				}
				SetConfidanceRangeW(aResultNode,bResultNode,aConstraintsNode0,nIndex,nSign);
				fprintf(fpOutput,"	Confidance Range Calculation for Weights i=%1d nSign=%4d  Iterations: %2d\n", nIndex, nSign, nIterations);
				endTime = CTime::GetCurrentTime();
				elapsedTime = endTime - startTime;
				OutputFormattedTime(fpOutput, "		Finished at:  ", endTime);
				OutputFormattedTimeSpan(fpOutput, "		Elapsed Time: ", elapsedTime);
			}
		}
	}
	fclose(fpOutput);
}

void CEvlDwellingTimeFitting::Construct_x(structDwellFittingTrialNode aNode, structEvlDTFittingConstraintsNode aConstraintsNode, Vec_DP &x)
{
	int i;
 	int nTemp=0;
	double dTemp;

	int nTempDimT=aConstraintsNode.nDimT;
	for(i=0;i<nTempDimT;i++)
	{
		nTemp=aConstraintsNode.nvT0[i];
		dTemp=aNode.fTimeConstants[nTemp];
		dTemp/=g_TrialNode.fTimeConstants[i];
		x[i]=log(dTemp)+1.;
	}

	double ai,aj;
	double dG0=1.;
	int nTemp0=aConstraintsNode.nvW0[0];
	int nTempDimW=aConstraintsNode.nDimW;

	for(i=0;i<nTempDimW-1;i++)
	{
		nTemp=aConstraintsNode.nvW0[i+1];
		ai=aNode.fWeights[nTemp];
		aj=aNode.fWeights[nTemp0];
		x[i+nTempDimT]=log(aj)-log(ai)+dG0;
	}
}

void CEvlDwellingTimeFitting::InitialPointT(structDwellFittingTrialNode &aNode, int nIndex, double dLogLik0, double &dLogLikI, int nSign, int nFuncIndex)
{
	double dX0=aNode.fTimeConstants[nIndex],dX1=aNode.fTimeConstants[nIndex];
	double dLogLik=-LogLikelihood_Local(aNode,nFuncIndex);
	double dTol=0.01;
	int nIterations=0;
	int nMaxIterations=30;
	double dDelta=dLogLik-dLogLik0;
	double factor;
	double factor1[5]={0.999,0.99,0.9,0.5,0.2};
	double factor0[5]={1.0001,1.001,1.01,1.1,2.};

	double dFactor=1.;
	if(dLogLik<dLogLik0)
		nIterations=nMaxIterations; //Bad input, exit this function.

	switch (nSign)
	{
		case 1:
			while(dLogLik>dLogLik0&&nIterations<nMaxIterations)
			{
				dX0=dX1;
				if(nIterations<5)
					factor=factor0[nIterations];
				dX1=factor*dX0;
				aNode.fTimeConstants[nIndex]=dX1;
				dLogLik=-dFactor*LogLikelihood_Local(aNode,nFuncIndex);
				nIterations++;
			}
			break;
		case -1: 
			while(dLogLik>dLogLik0&&nIterations<nMaxIterations)
			{
				dX0=dX1;
				if(nIterations<5)
					factor=factor1[nIterations];
				dX1=factor*dX0;
				aNode.fTimeConstants[nIndex]=dX1;
				dLogLik=-dFactor*LogLikelihood_Local(aNode,nFuncIndex);
				nIterations++;
			}
			break;
	}

	double dXm=0.;
	double dLogLikX0=dLogLik0+20.;
	int nIterations1=0;
	while(dDelta>dTol&&nIterations<nMaxIterations&&nIterations1<nMaxIterations)
	{
		dXm=0.5*(dX0+dX1);
		aNode.fTimeConstants[nIndex]=dXm;
		dLogLik=-LogLikelihood_Local(aNode,nFuncIndex);
		if(dLogLik>dLogLik0)
		{
			dX0=dXm;
			dLogLikX0=dLogLik;
		}
		else
		{
			dX1=dXm;
		}
		dDelta=dLogLikX0-dLogLik0;
		nIterations1++;
	}
	dLogLikI=dLogLikX0;
	aNode.fTimeConstants[nIndex]=dX0;
}

void CEvlDwellingTimeFitting::InitialPointW(structDwellFittingTrialNode &aNode, int nIndex, double dLogLik0, double &dLogLikI, int nSign, int nFuncIndex, structEvlDTFittingConstraintsNode aConstraintsNode0)
{
	double dX0=aNode.fWeights[nIndex],dX1=aNode.fWeights[nIndex];
	double dLogLik=-LogLikelihood_Local(aNode,nFuncIndex);
	double dTol=0.01;
	double dW0=aConstraintsNode0.fW0;
	int nDimW=aConstraintsNode0.nvW0.size();
	double dScale,factor;
	double factor1[5]={0.999,0.99,0.9,0.5,0.2};
	double factor0[5]={1.0001,1.001,1.01,1.1,2.};
	int nIterations=0;
	int nMaxIterations=30;
	int i,nTemp;
	double dDelta=dLogLik-dLogLik0;

	double dFactor=1.;
	if(dLogLik<dLogLik0)
		nIterations=nMaxIterations; //Bad input, exit this function.
//	if(dLogLik<dLogLik0)
//	{
//		nSign=-nSign;
//		dFactor=-1.;
//		dLogLik0=-dLogLik0;
//		dLogLik=-dLogLik;
//	}

	switch (nSign)
	{
		case 1:
			while(dLogLik>dLogLik0&&dX1<dW0&&nIterations<nMaxIterations)
			{
				dDelta=dLogLik-dLogLik0;
				dX0=dX1;
				if(nIterations<5)
					factor=factor0[nIterations];
				dX1=factor*dX0;
				nIterations++;
				if(dX1>dW0)
				{
					dX1=dX0+0.5*(dW0-dX0);
				}
				aNode.fWeights[nIndex]=dX1;
				dScale=(dW0-dX1)/(dW0-dX0);
				for(i=0;i<nDimW;i++)
				{
					nTemp=aConstraintsNode0.nvW0[i];
					if(nTemp!=nIndex)
					{
						aNode.fWeights[nTemp]*=dScale;
					}
				}
				dLogLik=-dFactor*LogLikelihood_Local(aNode,nFuncIndex);
				nIterations++;
			}
			break;
		case -1: 
			while(dLogLik>dLogLik0&&nIterations<nMaxIterations)
			{
				dDelta=dLogLik-dLogLik0;
				dX0=dX1;
				if(nIterations<5)
					factor=factor1[nIterations];
				dX1=factor*dX0;
				nIterations++;
				aNode.fWeights[nIndex]=dX1;
				dScale=(dW0-dX1)/(dW0-dX0);
				for(i=0;i<nDimW;i++)
				{
					nTemp=aConstraintsNode0.nvW0[i];
					if(nTemp!=nIndex)
					{
						aNode.fWeights[nTemp]*=dScale;
					}
				}
				dLogLik=-dFactor*LogLikelihood_Local(aNode,nFuncIndex);
				nIterations++;
			}
			break;
	}

	double dXm=0.;
	double dXm0=dX1;
	double dLogLikX0=dLogLik0+20.;
	int nIterations1=0;
	while(dDelta>dTol&&nIterations<nMaxIterations&&nIterations1<nMaxIterations)
	{
		dXm=0.5*(dX0+dX1);
		aNode.fWeights[nIndex]=dXm;
		dScale=(dW0-dXm)/(dW0-dXm0);
		for(i=0;i<nDimW;i++)
		{
			nTemp=aConstraintsNode0.nvW0[i];
			if(nTemp!=nIndex)
			{
				aNode.fWeights[nTemp]*=dScale;
			}
		}
		dLogLik=-LogLikelihood_Local(aNode,nFuncIndex);
		if(dLogLik>dLogLik0)
		{
			dX0=dXm;
			dLogLikX0=dLogLik;
		}
		else
		{
			dX1=dXm;
		}
		dDelta=dLogLikX0-dLogLik0;
		dXm0=dXm;
		nIterations1++;
	}
	aNode.fWeights[nIndex]=dX0;
	dScale=(dW0-dX0)/(dW0-dXm0);
	dLogLikI=dLogLikX0;
	for(i=0;i<nDimW;i++)
	{
		nTemp=aConstraintsNode0.nvW0[i];
		if(nTemp!=nIndex)
		{
			aNode.fWeights[nTemp]*=dScale;
		}
	}
}

void CEvlDwellingTimeFitting::ConfidanceRangeW(structDwellFittingTrialNode aNode, structFittingResultNode &aResultNode, int nIndex, 
											double dLogLik0, int nSign, int nFuncIndex, structEvlDTFittingConstraintsNode aConstraintsNode0)
{
	//This Function is under construction
	double dX0=aNode.fWeights[nIndex],dX1=aNode.fWeights[nIndex];
	double dLogLik=-LogLikelihood_Local(aNode,nFuncIndex);
	double dWc=aConstraintsNode0.fW0;
	double dTol=0.01;
	switch (nSign)
	{
		case 1:
			dX1=1.5*dX0;
			if(dX1>=dWc)
			{
				dX1=dWc;
			}
			aNode.fWeights[nIndex]=dX1;
			aResultNode=FitEvl_Transform(nFuncIndex, aNode, m_structEvlHist);
			aNode=GetNextTrialNode(aResultNode);
			dLogLik=aResultNode.dLogLikelihood;
			while(dLogLik>dLogLik0&&dX1<dWc)
			{
				dX0=dX1;
				aNode.fWeights[nIndex]=dX1;
				aResultNode=FitEvl_Transform(nFuncIndex, aNode, m_structEvlHist);
				aNode=GetNextTrialNode(aResultNode);
				dLogLik=aResultNode.dLogLikelihood;
			}
			dX1=aConstraintsNode0.fW0;
			break;
		case -1: 
			dX1=0.;
			break;
	}

	double dXm=0.5*(dX0+dX1);
	aNode.bFreezeW[nIndex]=true;
	while(fabs((dX1-dX0)/dXm)>dTol)
	{
		aNode.fWeights[nIndex]=dXm;
		aResultNode=FitEvl_Transform(nFuncIndex, aNode, m_structEvlHist);
		aNode=GetNextTrialNode(aResultNode);
		dLogLik=aResultNode.dLogLikelihood;

		if(dLogLik>dLogLik0)
		{
			dX0=dXm;
		}
		else
		{
			dX1=dXm;
		}
		dXm=0.5*(dX0+dX1);
	}
	aNode.fWeights[nIndex]=dXm;
	aNode.bFreezeW[nIndex]=false;
}

structDwellFittingTrialNode CEvlDwellingTimeFitting::GetNextTrialNode(structFittingResultNode bNode)
{
	structDwellFittingTrialNode aNode;
	aNode=bNode.aTrialNode;
	int nComponents=aNode.nNumComponents;
	for(int i=0;i<nComponents;i++)
	{
		aNode.fTimeConstants[i]=bNode.dTimeConstants[i];
		aNode.fWeights[i]=bNode.dWeights[i];
	}
	return aNode;
}

structDwellFittingTrialNode CEvlDwellingTimeFitting::GetNextTrialNode(structDwellFittingTrialNode aNode, Vec_DP &x)
{
	structDwellFittingTrialNode aNode0=g_TrialNode;	
	structEvlDTFittingConstraintsNode aConstraintsNode0=g_ConstraintsNode;
	structEvlDTFittingConstraintsNode aConstraintsNode=HandlingConstraints(aNode);
//	g_TrialNode=aNode;
	g_ConstraintsNode=aConstraintsNode;
	double dT[20];
	double dW[20];

	TimeConstantsAndWeights(dT,dW,x);
	int nComponents=aNode.nNumComponents;
	for(int i=0;i<nComponents;i++)
	{
		aNode.fTimeConstants[i]=dT[i];
		aNode.fWeights[i]=dW[i];
	}

	g_TrialNode=aNode0;
	g_ConstraintsNode=aConstraintsNode0;
	return aNode;
}

void CEvlDwellingTimeFitting::RemoveAllConstraints(structDwellFittingTrialNode &aNode)
{
	structDwellFittingTrialNode bNode=aNode;
	int nComponents=aNode.nNumComponents;
	for(int i=0;i<nComponents;i++)
	{
		aNode.bFreezeT[i]=false;
		aNode.bFreezeW[i]=false;
	}
}

void CEvlDwellingTimeFitting::ResetConstraint(structDwellFittingTrialNode &aNode, const structEvlDTFittingConstraintsNode aConstraintsNode0)
{
	int nComponents=aNode.nNumComponents;
	int nIndex,i;
	int nSize=aConstraintsNode0.nvT0.size();
	for(i=0;i<nSize;i++)
	{
		nIndex=aConstraintsNode0.nvT0[i];
		aNode.bFreezeT[nIndex]=false;
	}

	nSize=aConstraintsNode0.nvTc.size();
	for(i=0;i<nSize;i++)
	{
		nIndex=aConstraintsNode0.nvTc[i];
		aNode.bFreezeT[nIndex]=true;
	}

	nSize=aConstraintsNode0.nvW0.size();
	for(i=0;i<nSize;i++)
	{
		nIndex=aConstraintsNode0.nvW0[i];
		aNode.bFreezeW[nIndex]=false;
	}

	nSize=aConstraintsNode0.nvWc.size();
	for(i=0;i<nSize;i++)
	{
		nIndex=aConstraintsNode0.nvWc[i];
		aNode.bFreezeW[nIndex]=true;
	}
}

void CEvlDwellingTimeFitting::RemoveConstraint(structDwellFittingTrialNode &aNode, int nIndex)
{

}

void CEvlDwellingTimeFitting::SetConstraint(structDwellFittingTrialNode &aNode, structEvlDTFittingConstraintsNode aConstraintsNode, int nIndex)
{
	int nDimT=aConstraintsNode.nDimT;
	int nTemp;
	if((nIndex<nDimT))
	{
		nTemp=aConstraintsNode.nvT0[nIndex];
		aNode.bFreezeT[nTemp]=true;
	}
	else
	{
		nTemp=aConstraintsNode.nvW0[nIndex-nDimT];
		aNode.bFreezeT[nTemp]=true;
	}
}

void CEvlDwellingTimeFitting::ResetConstraint(structDwellFittingTrialNode &aNode, const structDwellFittingTrialNode bNode)
{
	for(int i=0;i<20;i++)
	{
		aNode.bFreezeT[i]=bNode.bFreezeT[i];
		aNode.bFreezeW[i]=bNode.bFreezeW[i];
	}
}

void CEvlDwellingTimeFitting::SetConfidanceRange(structFittingResultNode &aResultNode, structFittingResultNode bResultNode, structEvlDTFittingConstraintsNode aConstraintsNode, int nIndex)
{
	int nDimT=aConstraintsNode.nDimT;
	int nTemp;
	if((nIndex<nDimT))
	{
		nTemp=aConstraintsNode.nvT0[nIndex];
		if(aResultNode.dTimeConstants[nTemp]>bResultNode.dTimeConstants[nTemp])
		{
			aResultNode.fConfidanceRangeT[nTemp].fLeft=bResultNode.dTimeConstants[nTemp];
			aResultNode.fConfidanceRangeTL[nTemp].fLeft=bResultNode.dLogLikelihood;
		}
		else
		{
			aResultNode.fConfidanceRangeT[nTemp].fRight=bResultNode.dTimeConstants[nTemp];
			aResultNode.fConfidanceRangeTL[nTemp].fRight=bResultNode.dLogLikelihood;
		}
	}
	else
	{
		nTemp=aConstraintsNode.nvW0[nIndex-nDimT];
		if(aResultNode.dTimeConstants[nTemp]>bResultNode.dTimeConstants[nTemp])
		{
			aResultNode.fConfidanceRangeW[nTemp].fLeft=bResultNode.dTimeConstants[nTemp];
			aResultNode.fConfidanceRangeWL[nTemp].fLeft=bResultNode.dLogLikelihood;
		}
		else
		{
			aResultNode.fConfidanceRangeW[nTemp].fRight=bResultNode.dTimeConstants[nTemp];
			aResultNode.fConfidanceRangeWL[nTemp].fRight=bResultNode.dLogLikelihood;
		}
	}
}

void CEvlDwellingTimeFitting::SetConfidanceRangeT(structFittingResultNode &aResultNode, structFittingResultNode bResultNode, structEvlDTFittingConstraintsNode aConstraintsNode, int nIndex,int nSign)
{
	double dTemp=0.;
	if(nSign<0)
	{
		aResultNode.fConfidanceRangeT[nIndex].fLeft=bResultNode.dTimeConstants[nIndex];
		aResultNode.fConfidanceRangeTL[nIndex].fLeft=bResultNode.dLogLikelihood;
	}
	else
	{
		aResultNode.fConfidanceRangeT[nIndex].fRight=bResultNode.dTimeConstants[nIndex];
		aResultNode.fConfidanceRangeTL[nIndex].fRight=bResultNode.dLogLikelihood;
	}
}

void CEvlDwellingTimeFitting::SetConfidanceRangeTI(structFittingResultNode &aResultNode, structDwellFittingTrialNode bTrialNode, double dLogLikI, structEvlDTFittingConstraintsNode aConstraintsNode, int nIndex,int nSign)
{
	double dTemp=0.;
	if(nSign<0)
	{
		aResultNode.fConfidanceRangeTI[nIndex].fLeft=bTrialNode.fTimeConstants[nIndex];
		aResultNode.fConfidanceRangeTLI[nIndex].fLeft=dLogLikI;
	}
	else
	{
		aResultNode.fConfidanceRangeTI[nIndex].fRight=bTrialNode.fTimeConstants[nIndex];
		aResultNode.fConfidanceRangeTLI[nIndex].fRight=dLogLikI;
	}
}

void CEvlDwellingTimeFitting::SetConfidanceRangeW(structFittingResultNode &aResultNode, structFittingResultNode bResultNode, structEvlDTFittingConstraintsNode aConstraintsNode, int nIndex, int nSign)
{
	if(nSign<0)
	{
		aResultNode.fConfidanceRangeW[nIndex].fLeft=bResultNode.dWeights[nIndex];
		aResultNode.fConfidanceRangeWL[nIndex].fLeft=bResultNode.dLogLikelihood;
	}
	else
	{
		aResultNode.fConfidanceRangeW[nIndex].fRight=bResultNode.dWeights[nIndex];
		aResultNode.fConfidanceRangeWL[nIndex].fRight=bResultNode.dLogLikelihood;
	}
}

void CEvlDwellingTimeFitting::SetConfidanceRangeWI(structFittingResultNode &aResultNode, structDwellFittingTrialNode bTrialNode, double dLogLikI, structEvlDTFittingConstraintsNode aConstraintsNode, int nIndex, int nSign)
{
	if(nSign<0)
	{
		aResultNode.fConfidanceRangeWI[nIndex].fLeft=bTrialNode.fWeights[nIndex];
		aResultNode.fConfidanceRangeWLI[nIndex].fLeft=dLogLikI;
	}
	else
	{
		aResultNode.fConfidanceRangeWI[nIndex].fRight=bTrialNode.fWeights[nIndex];
		aResultNode.fConfidanceRangeWLI[nIndex].fRight=dLogLikI;
	}
}

void CEvlDwellingTimeFitting::PassResultNodeTWL(structFittingResultNode &aResultNode, structFittingResultNode bResultNode)
{
	aResultNode.dLogLikelihood=bResultNode.dLogLikelihood;
	double dTemp;
	int nComp=bResultNode.nNumComponents;
	aResultNode.nNumComponents=nComp;
	for(int i=0;i<nComp;i++)
	{
		aResultNode.dWeights[i]=bResultNode.dWeights[i];
		aResultNode.dTimeConstants[i]=bResultNode.dTimeConstants[i];
		dTemp=bResultNode.dTimeConstants[i];
	}
}


double CEvlDwellingTimeFitting::GetTW(Vec_DP x, structEvlDTFittingConstraintsNode aConstraintsNode, int nIndex)
{
	structEvlDTFittingConstraintsNode aConstraintsNode0=g_ConstraintsNode;
	g_ConstraintsNode=aConstraintsNode;

	double dResult;
	double dTimeConstants[20],dWeights[20];
	TimeConstantsAndWeights(dTimeConstants,dWeights,x);

	int nDimT=aConstraintsNode.nDimT;
	int nTemp;

	if((nIndex<nDimT))
	{
		nTemp=aConstraintsNode.nvT0[nIndex];
		dResult=dTimeConstants[nTemp];
	}
	else
	{
		nTemp=aConstraintsNode.nvW0[nIndex-nDimT];
		dResult=dWeights[nTemp];
	}

	g_ConstraintsNode=aConstraintsNode0;
	return dResult;
}

void CEvlDwellingTimeFitting::ResetGlobalConstraints(structDwellFittingTrialNode aNode)
{
	structEvlDTFittingConstraintsNode aConstraintsNode=HandlingConstraints(aNode);
	g_ConstraintsNode=aConstraintsNode;
}

double CEvlDwellingTimeFitting::LogLikelihood_Local(structDwellFittingTrialNode aNode, int nFuncIndex)
{
	//This function returns the negative value of the LogLikelihood
	const double dBigNegative=-9999999999999999999999999999999999999.;
	const double dTiny=4.9406564584125e-324;

	int nFittingLevel=aNode.nDwellLevel;
	double dTimeConstants[20],dWeights[20];
	int i;
	for(i=0;i<20;i++)
	{
		dTimeConstants[i]=aNode.fTimeConstants[i];
		dWeights[i]=aNode.fWeights[i];
	}
	double dTemp=0.;
	//Remembr to implement a soft boundary condition later at this point

	int nComponents=aNode.nNumComponents;

	//	dArea=1.;

	double dArea,dTmin,dTmax;
	double dLogLikelihood=0.,dLeft=0.,dRight=0.,dTemp1=0.;
	double dLog10=log(10.);
	int nNumBins=0;

	switch (nFuncIndex)
	{
	case 1:
		nNumBins=m_structEvlHist.vectFLogHist.at(nFittingLevel).size();
		dTmin=m_structEvlHist.vectFRLogBins.at(nFittingLevel)[0].fLeft;
		dTmax=m_structEvlHist.vectFRLogBins.at(nFittingLevel)[nNumBins-1].fRight;
		dTmin=exp(dTmin*dLog10);
		dTmax=exp(dTmax*dLog10);

		dArea=Area_Exp(dTimeConstants,dWeights,nComponents,dTmin,dTmax);
		for(i=0;i<nNumBins;i++)
		{
			dLeft=m_structEvlHist.vectFRLogBins.at(nFittingLevel)[i].fLeft;
			dRight=m_structEvlHist.vectFRLogBins.at(nFittingLevel)[i].fRight;
			dLeft=exp(dLeft*dLog10);
			dRight=exp(dRight*dLog10);
			dTemp=Area_Exp(dTimeConstants,dWeights,nComponents,dLeft,dRight);
			if(dTemp<dTiny) return -dBigNegative;
			dTemp1=m_structEvlHist.vectFLogHist.at(nFittingLevel)[i];
			dTemp=dTemp1*log(dTemp/dArea)/dLog10;
			dLogLikelihood+=dTemp;
		}
		break;
	case 3:
		nNumBins=m_structEvlHist.vectFLogHist_HB.at(nFittingLevel).size();
		dTmin=m_structEvlHist.vectFRLogBins_HB.at(nFittingLevel)[0].fLeft;
		dTmax=m_structEvlHist.vectFRLogBins_HB.at(nFittingLevel)[nNumBins-1].fRight;
		dTmin=exp(dTmin*dLog10);
		dTmax=exp(dTmax*dLog10);

		dArea=Area_Exp(dTimeConstants,dWeights,nComponents,dTmin,dTmax);
		for(i=0;i<nNumBins;i++)
		{
			dLeft=m_structEvlHist.vectFRLogBins_HB.at(nFittingLevel)[i].fLeft;
			dRight=m_structEvlHist.vectFRLogBins_HB.at(nFittingLevel)[i].fRight;
			dLeft=exp(dLeft*dLog10);
			dRight=exp(dRight*dLog10);
			dTemp=Area_Exp(dTimeConstants,dWeights,nComponents,dLeft,dRight);
			if(dTemp<dTiny) return -dBigNegative;
			dTemp1=m_structEvlHist.vectFLogHist_HB.at(nFittingLevel)[i];
			dTemp=dTemp1*log(dTemp/dArea)/dLog10;
			dLogLikelihood+=dTemp;
		}
		break;
	}

	if(dArea<dTiny) return -dBigNegative;
	return -dLogLikelihood;
}


void CEvlDwellingTimeFitting::OutputFormattedTime(FILE *fpOutput, CString sComments, CTime aTime)
{
	CString sTemp = aTime.Format( "%H:%M:%S On " );
	fprintf(fpOutput,"%s  %s ",sComments,sTemp);
	sTemp = aTime.Format( "%A, %B %d, %Y" );
	fprintf(fpOutput,"%s  \n",sTemp);
}

void CEvlDwellingTimeFitting::OutputFormattedTimeSpan(FILE *fpOutput, CString sComments, CTimeSpan aTime)
{
	CString sTemp = aTime.Format( "%D Days, %H Hours, %M Minutes and %S Seconds \n" );
	fprintf(fpOutput,"%s  %s ",sComments,sTemp);
}

