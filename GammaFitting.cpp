// GammaFitting.cpp: implementation of the CGammaFitting class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "Abf_Processing.h"
#include "GammaFitting.h"
#include "structures.h"
#include "nr.h"
#include <vector>

using namespace std;

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif


//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CGammaFitting::CGammaFitting()
{

}

CGammaFitting::~CGammaFitting()
{

}

namespace GammaGlobal{
	structGammaFittingNode g_GammaTrialNode;
	structGammaFittingNode g_GammaResultNode;
	structGammaFittingConstraintsNode g_ConstraintsNode;
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

		int nTempDimR=g_ConstraintsNode.nDimR;

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
			ai=g_GammaTrialNode.dWeights[nTemp];
			aj=g_GammaTrialNode.dWeights[nTemp0];
			x[i+nTempDimR]=log(aj)-log(ai)+dG0;
		}
	};


	void RevTransform(Vec_I_DP &x)
	{
		double dG[20];
		double dG0=1.;
		int i;
		int nDimR=g_ConstraintsNode.nDimR;
		int nDimW=g_ConstraintsNode.nDimW;

		dG[0]=dG0;
		for(i=0;i<nDimW-1;i++)
		{
			dG[i+1]=x[nDimR+i];
		}

		double dZ=PartitionFunction(dG,nDimW);

		int nTemp0=g_ConstraintsNode.nvW0[0];
		int nTemp=0;

		for(i=0;i<nDimR;i++)
		{
			nTemp=g_ConstraintsNode.nvR0[i];
			g_GammaResultNode.dRol[nTemp]=g_GammaTrialNode.dRol[nTemp]*exp(x[i]-1);
		}

		float fW0=g_ConstraintsNode.fW0;

		for(i=0;i<nDimW;i++)
		{
			nTemp=g_ConstraintsNode.nvW0[i];
			g_GammaResultNode.dWeights[nTemp]=fW0*exp(-dG[i])/dZ;
		}

		double dTemp=0.;
		for(i=1;i<nDimW;i++)
		{
			nTemp=g_ConstraintsNode.nvW0[i];
			dTemp+=g_GammaResultNode.dWeights[nTemp];
		}

		g_GammaResultNode.dWeights[nTemp0]=fW0-dTemp;

		nDimR=g_ConstraintsNode.nNumComponents-nDimR;
		for(i=0;i<nDimR;i++)
		{
			nTemp=g_ConstraintsNode.nvRc[i];
			g_GammaResultNode.dRol[nTemp]=g_GammaTrialNode.dRol[nTemp];
		}

		nDimW=g_ConstraintsNode.nNumComponents-nDimW;
		for(i=0;i<nDimW;i++)
		{
			nTemp=g_ConstraintsNode.nvWc[i];
			g_GammaResultNode.dWeights[nTemp]=g_GammaTrialNode.dWeights[nTemp];
		}
	}

	double GammaDistribution(double dH, double dLamda, double dX)
	{
		double dResult=0.;
		double dGamma=NR::gammln(dH);
		dGamma=exp(dGamma);
		dResult=(exp((dLamda-1.)*log(dLamda*dX))*dLamda*exp(-dLamda*dX))/dGamma;
		return dResult;
	}

	double GammaDistribution_Area(double dH, double dLamda, double dX0, double dX1)
	{
		double dResult=0.;
		double dGamma=NR::gammln(dH);
		dGamma=exp(dGamma);
		dResult=NR::gammq(dH,dLamda*dX0)-NR::gammq(dH,dLamda*dX1);
		dResult/=dGamma;
		return dResult;
	}

	double LogLikGamma(Vec_I_DP &x)
	{
		//This function returns the negative value of the LogLikelihood
		const double dPi=3.1415926535897932384626433832795;
		int i,j;
		double dTemp=0.;

		double dLog10=log(10.);
		RevTransform(x);

		int nSize=g_vHistNode.size();
		int nComponents=g_GammaTrialNode.nNumComponents;
		int nFuncKey=g_GammaTrialNode.nFuncKey;
		double dX,dX0,dX1;
		const double dH=g_GammaTrialNode.dH;

		double dLogLik=0.;
		double dLik=0.,dExp=0.,dDenorm=0.;
		double dR,dW,dArea;

		dX0=g_GammaTrialNode.fFittingRange.fLeft;
		dX1=g_GammaTrialNode.fFittingRange.fRight;
		dArea=0.;
		for(j=0;j<nComponents;j++)
		{
			dR=g_GammaResultNode.dRol[j];
			dW=g_GammaResultNode.dWeights[j];
			dTemp=GammaDistribution_Area(dH,dR,dX0,dX1);
			dArea+=dTemp;
		}

		switch (nFuncKey)
		{
		case 1:
			for(i=0;i<nSize;i++)
			{
				dLik=0.;
				dX=g_vHistNode[i].dLeft;
				for(j=0;j<nComponents;j++)
				{
					dR=g_GammaResultNode.dRol[j];
					dW=g_GammaResultNode.dWeights[j];
					dTemp=GammaDistribution(dH,dR,dX);
					dLik+=dTemp;
				}
				dLogLik+=log(dLik*dArea)/dLog10;
			}
			break;
		case 2:
			for(i=0;i<nSize;i++)
			{
				dLik=0.;
				dX0=g_vHistNode[i].dLeft;
				dX1=g_vHistNode[i].dRight;
				for(j=0;j<nComponents;j++)
				{
					dR=g_GammaResultNode.dRol[j];
					dW=g_GammaResultNode.dWeights[j];
					dTemp=GammaDistribution_Area(dH,dR,dX0,dX1);
					dLik+=dTemp;
				}
				dLogLik+=log(dLik*dArea*g_vHistNode[i].dData)/dLog10;
			}
			break;
		}
		return -dLogLik;
	}
};

using namespace GammaGlobal;

CGammaFitting::CGammaFitting(structGammaFittingNode aNode, const vector <structHistNode> avHist0)
{
	int nComponents=aNode.nNumComponents;
	int nFreePrams=aNode.nFreeParameters;

	int i,nfunc,j;

	g_vHistNode=avHist0;
	Initialization(aNode);


	int MP=g_ConstraintsNode.nDimR+g_ConstraintsNode.nDimW;
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
	y[0]=LogLikGamma(x);


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
		
		dTemp=LogLikGamma(x);
		y[i]=dTemp;
	}

	NR::amoeba(p,y,FTOL,LogLikGamma,nfunc);
	
	for(i=0;i<nFreePrams;i++)
	{
		x[i]=p[0][i];
	}

	RevTransform(x);
	g_GammaResultNode.dLogLik=LogLikGamma(x);
}

void CGammaFitting::Initialization(structGammaFittingNode aNode)
{
	structGammaFittingConstraintsNode aConstraintsNode=HandlingConstraints(aNode);
	g_ConstraintsNode=aConstraintsNode;

	g_GammaTrialNode.nDataSize=aNode.nDataSize;
	g_GammaTrialNode.nNumComponents=aNode.nNumComponents;
	int nComponents=aNode.nNumComponents;
	int i;
	g_GammaTrialNode.nFreeParameters=aNode.nFreeParameters;
	for(i=0;i<nComponents;i++)
	{
		g_GammaTrialNode.dRol[i]=aNode.dRol[i];
		g_GammaTrialNode.dWeights[i]=aNode.dWeights[i];
		g_GammaTrialNode.bFreezeW[i]=aNode.bFreezeW[i];
		g_GammaTrialNode.bFreezeR[i]=aNode.bFreezeR[i];
	}
	int nSize=aNode.nDataSize;
	g_GammaTrialNode.pdData=new double[nSize];

	for(i=0;i<nSize;i++)
	{
		g_GammaTrialNode.pdData[i]=aNode.pdData[i];
	}

	g_GammaResultNode.nDataSize=aNode.nDataSize;
	g_GammaResultNode.nNumComponents=aNode.nNumComponents;
	g_GammaResultNode.pdData=new double[nSize];
	g_GammaResultNode.nFreeParameters=aNode.nFreeParameters;
	for(i=0;i<nComponents;i++)
	{
		g_GammaResultNode.dRol[i]=aNode.dRol[i];
		g_GammaResultNode.dWeights[i]=aNode.dWeights[i];
		g_GammaResultNode.bFreezeW[i]=aNode.bFreezeW[i];
		g_GammaResultNode.bFreezeR[i]=aNode.bFreezeR[i];
	}

	for(i=0;i<nSize;i++)
	{
		g_GammaResultNode.pdData[i]=aNode.pdData[i];
	}
}

structGammaFittingNode CGammaFitting::GetFittedNode()
{
	int i;
	float fTemp[20];
	int nComponents=g_GammaResultNode.nNumComponents;
//	for(i=0;i<nComponents;i++)
//	{
//		fTemp[i]=g_GammaResultNode.dRol[i];
//	}
	SortFittedNode();
//	for(i=0;i<nComponents;i++)
//	{
//		fTemp[i]=g_GammaResultNode.dRol[i];
//	}
	return g_GammaResultNode;
}

void CGammaFitting::SortFittedNode()
{
	int nComponents=g_GammaResultNode.nNumComponents;
	Vec_IO_DP dRol(nComponents);
	Vec_IO_DP dOrder(nComponents);
	double dWeights[20];
	int i,j;
	float fTemp[20];
	for(i=0;i<nComponents;i++)
	{
		dRol[i]=g_GammaResultNode.dRol[i];
		dWeights[i]=g_GammaResultNode.dWeights[i];
		dOrder[i]=double(i);
		fTemp[i]=dRol[i];
	}
	NR::sort2(dRol,dOrder);
	int nIndex;
	for(i=0;i<nComponents;i++)
	{
		nIndex=int(dOrder[i]+0.5);
		g_GammaResultNode.dRol[i]=dRol[i];
		fTemp[i]=dRol[i];
		g_GammaResultNode.dWeights[i]=dWeights[nIndex];
	}

//	for(i=0;i<nComponents;i++)
//	{
//		fTemp[i]=g_GammaResultNode.dRol[i];
//	}
}



structGammaFittingConstraintsNode CGammaFitting::HandlingConstraints(structGammaFittingNode aNode)
{
	structGammaFittingConstraintsNode aConstraintsNode;
	int i;
	int nComponents=aNode.nNumComponents;
	aConstraintsNode.nNumComponents=nComponents;
	aConstraintsNode.nvR0.clear();
	aConstraintsNode.nvRc.clear();
	aConstraintsNode.nvW0.clear();
	aConstraintsNode.nvWc.clear();
	float fWc=0.f;

	for(i=0;i<nComponents;i++)
	{
		if(aNode.bFreezeR[i])
		{
			aConstraintsNode.nvRc.push_back(i);
		}
		else
		{
			aConstraintsNode.nvR0.push_back(i);
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

	aConstraintsNode.nDimR=aConstraintsNode.nvR0.size();
	aConstraintsNode.nDimW=aConstraintsNode.nvW0.size();
	aConstraintsNode.fW0=1.-fWc;
	aConstraintsNode.fWc=fWc;
	return aConstraintsNode;
}

void CGammaFitting::Construct_x(structGammaFittingNode aNode, structGammaFittingConstraintsNode aConstraintsNode, Vec_DP &x)
{
	int NP=aConstraintsNode.nDimR+aConstraintsNode.nDimW-1;

    int i;
 	int nTemp=0;
	double dTemp;

	int nTempDimR=aConstraintsNode.nDimR;
	double dR=0.;
	for(i=0;i<nTempDimR;i++)
	{
		nTemp=aConstraintsNode.nvR0[i];
		dTemp=aNode.dRol[nTemp];
		dTemp/=g_GammaTrialNode.dRol[i];
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

