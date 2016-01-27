// ExponentialFitting.cpp: implementation of the CExponentialFitting class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "Abf_Processing.h"
#include "ExponentialFitting.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CExponentialFitting::CExponentialFitting()
{

}

CExponentialFitting::~CExponentialFitting()
{

}

namespace ExponentialGlobal{
	structExponentialFittingNode g_ExponentialTrialNode;
	structExponentialFittingNode g_ExponentialResultNode;
	structExponentialFittingConstraintsNode g_ConstraintsNode;
	vector <structHistNode> g_vHistNode;


	double PartitionFunction(double x[], int nDim)
	{
		double dResult=0.f;
		for(int i=0;i<nDim;i++)
		{
			dResult+=exp(-x[i]);
		}
		return dResult;
	}

	void Transform(Vec_DP &x)
	{
		int i;

		double dTemp;

		int nTempDimR=g_ConstraintsNode.nDimL;

		for(i=0;i<nTempDimR;i++)
		{
			x[i]=1.;
		}

		double ai,aj;
		double dG0=1.;
		int nTemp0=g_ConstraintsNode.nvW0[0];
		int nTemp=0;
		int nTempDimW=g_ConstraintsNode.nDimW;

		for(i=0;i<nTempDimW-1;i++)
		{
			nTemp=g_ConstraintsNode.nvW0[i+1];
			ai=g_ExponentialTrialNode.dWeights[nTemp];
			aj=g_ExponentialTrialNode.dWeights[nTemp0];
			x[i+nTempDimR]=log(aj)-log(ai)+dG0;
		}
	};

	void RevTransform(Vec_I_DP &x)
	{
		double dG[20];
		double dG0=1.;
		int i;
		int nDimL=g_ConstraintsNode.nDimL;
		int nDimW=g_ConstraintsNode.nDimW;

		dG[0]=dG0;
		for(i=0;i<nDimW-1;i++)
		{
			dG[i+1]=x[nDimL+i];
		}

		double dZ=PartitionFunction(dG,nDimW);

		int nTemp0=g_ConstraintsNode.nvW0[0];
		int nTemp=0;
		
		double dX0,dX1,dX2;
		for(i=0;i<nDimL;i++)
		{
			nTemp=g_ConstraintsNode.nvL0[i];
			dX0=g_ExponentialTrialNode.dLambda[nTemp];
			dX1=x[i];
			dX2=exp(x[i]-1);
			g_ExponentialResultNode.dLambda[nTemp]=dX0*dX2;
		}

		float fW0=g_ConstraintsNode.fW0;

		for(i=0;i<nDimW;i++)
		{
			nTemp=g_ConstraintsNode.nvW0[i];
			g_ExponentialResultNode.dWeights[nTemp]=fW0*exp(-dG[i])/dZ;
		}

		double dTemp=0.;
		for(i=1;i<nDimW;i++)
		{
			nTemp=g_ConstraintsNode.nvW0[i];
			dTemp+=g_ExponentialResultNode.dWeights[nTemp];
		}

		g_ExponentialResultNode.dWeights[nTemp0]=fW0-dTemp;

		nDimL=g_ConstraintsNode.nNumComponents-nDimL;
		for(i=0;i<nDimL;i++)
		{
			nTemp=g_ConstraintsNode.nvLc[i];
			g_ExponentialResultNode.dLambda[nTemp]=g_ExponentialTrialNode.dLambda[nTemp];
		}

		nDimW=g_ConstraintsNode.nNumComponents-nDimW;
		for(i=0;i<nDimW;i++)
		{
			nTemp=g_ConstraintsNode.nvWc[i];
			g_ExponentialResultNode.dWeights[nTemp]=g_ExponentialTrialNode.dWeights[nTemp];
		}
	}

	double Area_Exp(double dLambda[], double dW[], int nDim, double dLeft, double dRight, double &dCompensation)
	{
		//This function return dArea/exp(dCompenation)
		double dArea=0.;
		double dDelta=10.;
		double dTiny=4.9406564584125e-324;
		double dX0,dX1,dX2;
		double dLMax=-80;
		for(int i=0;i<nDim;i++)
		{
			dX0=exp(-dLeft*dLambda[i]);
			dX1=exp(-dRight*dLambda[i]);
			dX2=dW[i];
			dArea+=dX2*(dX0-dX1);
			if(dLambda[i]>dLMax) dLMax=dLambda[i];
		}
		dCompensation=0.;
		if(dArea<dTiny)
		{
			dArea=0.;
//			dCompensation=log(dTiny)+dLeft*dLMax+10.;
//			for(int i=0;i<nDim;i++)
//			{
//				dX0=exp(-dLeft*dLambda[i]+dCompensation);
//				dX1=exp(-dRight*dLambda[i]+dCompensation);
//				dX2=dW[i];
//				dArea+=dX2*(dX0-dX1);
//			}
//			dArea=dTiny;
		}
		return dArea;
	}

	double Area_Exp(double dLambda[], double dW[], int nDim, double dLeft, double dRight)
	{
		//This function return dArea/exp(dCompenation)
		double dArea=0.;
		double dDelta=10.;
		double dTiny=4.9406564584125e-324;
		double dX0,dX1,dX2;
		double dLMax=-80;
		for(int i=0;i<nDim;i++)
		{
//			dX0=exp(-dLeft*dLambda[i]);
//			dX1=exp(-dRight*dLambda[i]);
			dX0=exp(-dLeft*dLambda[i]);
			dX1=exp(-dRight*dLambda[i]);
			dX2=dW[i];
			dArea+=dX2*(dX0-dX1);
			if(dLambda[i]>dLMax) dLMax=dLambda[i];
		}
//		if(dArea<dTiny)
//		{
//			dArea=dTiny;
//		}
		return dArea;
	}

	double FittedArea_Exp()
	{
		double dX0,dX1;
		dX0=g_ExponentialResultNode.fFittingRange.fLeft;
		dX1=g_ExponentialResultNode.fFittingRange.fRight;
		return Area_Exp(g_ExponentialResultNode.dLambda,g_ExponentialResultNode.dWeights,g_ExponentialResultNode.nNumComponents,dX0,dX1);
	}

	double LogLikExponential(Vec_I_DP &x)
	{
		double dCompensation0,dCompensation1;
		//This function returns the negative value of the LogLikelihood
		const double dPi=3.1415926535897932384626433832795;
		int i,j;
		double dTemp=0.;

		double dLog10=log(10.);
		RevTransform(x);

		int nSize=g_vHistNode.size();
		int nComponents=g_ExponentialTrialNode.nNumComponents;
		int nFuncKey=g_ExponentialTrialNode.nFuncKey;
		double dX,dX0,dX1;

		double dLogLik=0.;
		double dLik=0.;
		double dLambda[20],dW[20],dArea;

		dX0=g_ExponentialTrialNode.fFittingRange.fLeft;
		dX1=g_ExponentialTrialNode.fFittingRange.fRight;
		dArea=0.;

		for(j=0;j<nComponents;j++)
		{
			dLambda[j]=g_ExponentialResultNode.dLambda[j];
			dW[j]=g_ExponentialResultNode.dWeights[j];
		}
//		dArea=Area_Exp(dLambda,dW,nComponents,dX0,dX1,dCompensation0);
		dArea=Area_Exp(dLambda,dW,nComponents,dX0,dX1);
		switch (nFuncKey)
		{
		case 1:
			for(i=0;i<nSize;i++)
			{
				dLik=0.;
				dX=g_vHistNode[i].dLeft;
				for(j=0;j<nComponents;j++)
				{
					dTemp=exp(-dLambda[i]*dX)*dW[i];
					dLik+=dTemp;
				}
				dLogLik+=log(dLik/dArea)/dLog10;
			}
			break;
		case 2:
			for(i=0;i<nSize;i++)
			{
				dLik=0.;
				dX0=g_vHistNode[i].dLeft;
				dX1=g_vHistNode[i].dRight;
//				dLik=(log(Area_Exp(dLambda,dW,nComponents,dX0,dX1,dCompensation1)/dArea)+dCompensation0-dCompensation1)*g_vHistNode[i].dData;
				dLik=log(Area_Exp(dLambda,dW,nComponents,dX0,dX1)/dArea)*g_vHistNode[i].dData/dLog10;
				dLogLik+=dLik;
			}
			break;
		}
		return -dLogLik;
	}
};

using namespace ExponentialGlobal;

CExponentialFitting::CExponentialFitting(structExponentialFittingNode aTrialNode, const vector <structHistNode> avHist0)
{
	//This function takes dLambda of aTrialNode as the trial values.
	int nComponents=aTrialNode.nNumComponents;
	int nFreePrams=aTrialNode.nFreeParameters;

	int i,nfunc,j;

	Initialization(aTrialNode, avHist0);

	int MP=g_ConstraintsNode.nDimL+g_ConstraintsNode.nDimW;
	int NP=MP-1;
    const DP FTOL=1.0e-10;
    Vec_DP x(NP),y(MP),temp(NP);
    Mat_DP p(MP,NP);

	Transform(temp);

	double dTemp;
	for(j=0;j<NP;j++)
	{
		dTemp=temp[j];
		x[j]=temp[j];
		p[0][j]=temp[j];
	}

	y[0]=LogLikExponential(x);

	for(i=1;i<MP;i++)
	{
		for(j=0;j<NP;j++)
		{
			if(j+1==i)
			{
				dTemp=1.1*temp[j];
			}
			else
			{
				dTemp=0.;
			}
			x[j]=dTemp;
			p[i][j]=dTemp;
		}
		
		dTemp=LogLikExponential(x);
		y[i]=dTemp;
	}

	NR::amoeba(p,y,FTOL,LogLikExponential,nfunc);
	
	for(i=0;i<NP;i++)
	{
		x[i]=p[0][i];
	}

	RevTransform(x);
	g_ExponentialResultNode.dLogLik=-LogLikExponential(x);
	g_ExponentialResultNode.nIterations=nfunc;
	g_ExponentialResultNode.dArea=FittedArea_Exp();
	g_ExponentialResultNode.dMeanTCA=CalMeanTCA(g_ExponentialResultNode);
	double dLambda[20];
	for(i=0;i<20;i++)
	{
		dLambda[i]=g_ExponentialResultNode.dLambda[i];
	}
}

void CExponentialFitting::Initialization(structExponentialFittingNode aNode, const vector <structHistNode> avHist0)
{
	g_vHistNode=avHist0;

	structExponentialFittingConstraintsNode aConstraintsNode=HandlingConstraints(aNode);
	g_ConstraintsNode=aConstraintsNode;

	g_ExponentialTrialNode.nNumComponents=aNode.nNumComponents;
	g_ExponentialTrialNode.nFuncKey=aNode.nFuncKey;
	g_ExponentialTrialNode.fFittingRange=aNode.fFittingRange;
	int nComponents=aNode.nNumComponents;
	int i;
	g_ExponentialTrialNode.nFreeParameters=aNode.nFreeParameters;
	for(i=0;i<nComponents;i++)
	{
		g_ExponentialTrialNode.dLambda[i]=aNode.dLambda[i];
		g_ExponentialTrialNode.dWeights[i]=aNode.dWeights[i];
		g_ExponentialTrialNode.bFreezeW[i]=aNode.bFreezeW[i];
		g_ExponentialTrialNode.bFreezeL[i]=aNode.bFreezeL[i];
	}

	g_ExponentialResultNode.nNumComponents=aNode.nNumComponents;
	g_ExponentialResultNode.nFreeParameters=aNode.nFreeParameters;
	g_ExponentialResultNode.nFuncKey=aNode.nFuncKey;
	g_ExponentialResultNode.fFittingRange=aNode.fFittingRange;

	for(i=0;i<nComponents;i++)
	{
		g_ExponentialResultNode.dLambda[i]=aNode.dLambda[i];
		g_ExponentialResultNode.dWeights[i]=aNode.dWeights[i];
		g_ExponentialResultNode.bFreezeW[i]=aNode.bFreezeW[i];
		g_ExponentialResultNode.bFreezeL[i]=aNode.bFreezeL[i];
	}
}

structExponentialFittingNode CExponentialFitting::GetFittedNode()
{
	AssignTimeConstants();
	SortFittedNode();
	return g_ExponentialResultNode;
}

void CExponentialFitting::AssignTimeConstants()
{
	int nDim=g_ExponentialResultNode.nNumComponents;
	for(int i=0;i<nDim;i++)
	{
		g_ExponentialResultNode.dTimeConstants[i]=1./g_ExponentialResultNode.dLambda[i];
	}
}

void CExponentialFitting::SortFittedNode()
{
	int nComponents=g_ExponentialResultNode.nNumComponents;
	Vec_IO_DP dLambda(nComponents);
	Vec_IO_DP dOrder(nComponents);
	double dWeights[20];
	int i,j;
	float fTemp[20];
	for(i=0;i<nComponents;i++)
	{
		dLambda[i]=1./g_ExponentialResultNode.dLambda[i];
		dWeights[i]=g_ExponentialResultNode.dWeights[i];
		dOrder[i]=double(i);
		fTemp[i]=dLambda[i];
	}
	if(nComponents>8)
	{
		i=i;
	}
	NR::sort2(dLambda,dOrder);
	int nIndex;
	for(i=0;i<nComponents;i++)
	{
		nIndex=int(dOrder[i]+0.5);
		g_ExponentialResultNode.dLambda[i]=1./dLambda[i];
		fTemp[i]=dLambda[i];
		g_ExponentialResultNode.dWeights[i]=dWeights[nIndex];
	}

//	for(i=0;i<nComponents;i++)
//	{
//		fTemp[i]=g_ExponentialResultNode.dLambda[i];
//	}
}



structExponentialFittingConstraintsNode CExponentialFitting::HandlingConstraints(structExponentialFittingNode aNode)
{
	structExponentialFittingConstraintsNode aConstraintsNode;
	int i;
	int nComponents=aNode.nNumComponents;
	aConstraintsNode.nNumComponents=nComponents;
	aConstraintsNode.nvL0.clear();
	aConstraintsNode.nvLc.clear();
	aConstraintsNode.nvW0.clear();
	aConstraintsNode.nvWc.clear();
	float fWc=0.f;

	for(i=0;i<nComponents;i++)
	{
		if(aNode.bFreezeL[i])
		{
			aConstraintsNode.nvLc.push_back(i);
		}
		else
		{
			aConstraintsNode.nvL0.push_back(i);
		}

		if(aNode.bFreezeW[i])
		{
			aConstraintsNode.nvWc.push_back(i);
			fWc+=aNode.dWeights[i];
		}
		else
		{
			aConstraintsNode.nvW0.push_back(i);
		}
	}

	aConstraintsNode.nDimL=aConstraintsNode.nvL0.size();
	aConstraintsNode.nDimW=aConstraintsNode.nvW0.size();
	aConstraintsNode.fW0=1.-fWc;
	aConstraintsNode.fWc=fWc;
	return aConstraintsNode;
}

void CExponentialFitting::Construct_x(structExponentialFittingNode aNode, structExponentialFittingConstraintsNode aConstraintsNode, Vec_DP &x)
{
	int NP=aConstraintsNode.nDimL+aConstraintsNode.nDimW-1;

    int i;
 	int nTemp=0;
	double dTemp;

	int nTempDimR=aConstraintsNode.nDimL;
	double dR=0.;
	for(i=0;i<nTempDimR;i++)
	{
		nTemp=aConstraintsNode.nvL0[i];
		dTemp=aNode.dLambda[nTemp];
		dTemp/=g_ExponentialTrialNode.dLambda[i];
		x[i]=log(dTemp)+1.;
	}

	double ai,aj;
	double dG0=1.;
	int nTemp0=aConstraintsNode.nvW0[0];
	int nTempDimW=aConstraintsNode.nDimW;

	for(i=0;i<nTempDimW-1;i++)
	{
		nTemp=aConstraintsNode.nvW0[i+1];
		ai=aNode.dWeights[nTemp];
		aj=aNode.dWeights[nTemp0];
		x[i+nTempDimR]=log(aj)-log(ai)+dG0;
	}
}


double CExponentialFitting::CalMeanTCA(structExponentialFittingNode aResultNode)
{
	//This function was re-checked on 4/30/2005
	double dTmin=aResultNode.fFittingRange.fLeft;
	double dBigTime=1.e99;
	double dL[20];
	double dW[20];
	int i;
	int nDim=aResultNode.nNumComponents;

	for(i=0;i<nDim;i++)
	{
		dL[i]=aResultNode.dLambda[i];
		dW[i]=aResultNode.dWeights[i];
	}

	double dArea=Area_Exp(dL, dW, nDim, dTmin, dBigTime);

	double dMean=0.;
	for(i=0;i<nDim;i++)
	{
		dMean+=dW[i]*exp(-dTmin*dL[i])/dL[i];
	}
	dMean/=dArea;
	dMean+=dTmin;
	return dMean;
}

