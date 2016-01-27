// GeometricFitting.cpp: implementation of the CGeometricFitting class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "Abf_Processing.h"
#include "GeometricFitting.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CGeometricFitting::CGeometricFitting()
{

}

CGeometricFitting::~CGeometricFitting()
{

}


namespace GeometricGlobal{
	structGeometricFittingNode g_GeometricTrialNode;
	structGeometricFittingNode g_GeometricResultNode;
	structGeometricFittingConstraintsNode g_ConstraintsNode;
	vector <structHistNode> g_vHistNode;

	CString PassingString(CString sString)
	{
		CString sNewString=sString;
		return sNewString;
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

	double Exp(double dX, double dY)
	{
		//Assuming dX>=0;
		double dTiny=4.9406564584125e-324;
		double dResult=0.;
		if(dX>dTiny)
		{
			dResult=exp(dY*log(dX));
		}
		else
		{
			dResult=1.;
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
			dTemp=g_GeometricTrialNode.dRol[i];
			x[i]=-log(dTemp/(1.-dTemp));
		}

		double ai,aj;
		double dG0=1.;
		int nTemp0=g_ConstraintsNode.nvW0[0];
		int nTemp=0;
		int nTempDimW=g_ConstraintsNode.nDimW;

		for(i=0;i<nTempDimW-1;i++)
		{
			nTemp=g_ConstraintsNode.nvW0[i+1];
			ai=g_GeometricTrialNode.dWeights[nTemp];
			aj=g_GeometricTrialNode.dWeights[nTemp0];
			x[i+nTempDimR]=log(aj)-log(ai)+dG0;
		}
	};

	void RevTransform(Vec_I_DP x)
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
		
		double dX0,dX1,dX2;
		for(i=0;i<nDimR;i++)
		{
			nTemp=g_ConstraintsNode.nvR0[i];
			dX0=x[i];
			g_GeometricResultNode.dRol[nTemp]=exp(-dX0)/(1.+exp(-dX0));
		}

		float fW0=g_ConstraintsNode.fW0;

		for(i=0;i<nDimW;i++)
		{
			nTemp=g_ConstraintsNode.nvW0[i];
			g_GeometricResultNode.dWeights[nTemp]=fW0*exp(-dG[i])/dZ;
		}

		double dTemp=0.;
		for(i=1;i<nDimW;i++)
		{
			nTemp=g_ConstraintsNode.nvW0[i];
			dTemp+=g_GeometricResultNode.dWeights[nTemp];
		}

		g_GeometricResultNode.dWeights[nTemp0]=fW0-dTemp;

		nDimR=g_ConstraintsNode.nNumComponents-nDimR;
		for(i=0;i<nDimR;i++)
		{
			nTemp=g_ConstraintsNode.nvRc[i];
			g_GeometricResultNode.dRol[nTemp]=g_GeometricTrialNode.dRol[nTemp];
		}

		nDimW=g_ConstraintsNode.nNumComponents-nDimW;
		for(i=0;i<nDimW;i++)
		{
			nTemp=g_ConstraintsNode.nvWc[i];
			g_GeometricResultNode.dWeights[nTemp]=g_GeometricTrialNode.dWeights[nTemp];
		}
	}


	double Area_Geometric(double dRol[], double dW[], int nDim, double dLeft, double dRight)
	{
		//This function return dArea/exp(dCompenation)
		double dArea=0.;
		double dX0,dX1;
		for(int i=0;i<nDim;i++)
		{
			dX0=dRol[i];
			dX1=dW[i];
			dArea+=dX1*(Exp(dX0,dLeft-1.)-Exp(dX0,dRight));
		}
		return dArea;
	}

	double Geometric(double dRol[], double dW[], int nDim, double dX)
	{
		//This function return dArea/exp(dCompenation)
		double dTiny=4.9406564584125e-324;
		double dResult=0.;
		double dX0,dX1;
		for(int i=0;i<nDim;i++)
		{
			dX0=dRol[i];
			dX1=dW[i];
			dResult+=dX1*(1-dX0)*Exp(dX0,dX-1.);
		}
		if(dResult<dTiny)
			dResult=dTiny;
		return dResult;
	}


	double FittedArea_Geometric()
	{
		double dX0,dX1;
		dX0=g_GeometricResultNode.fFittingRange.fLeft;
		dX1=g_GeometricResultNode.fFittingRange.fRight;
		return Area_Geometric(g_GeometricResultNode.dRol,g_GeometricResultNode.dWeights,g_GeometricResultNode.nNumComponents,dX0,dX1);
	}

	double LogLikGeometric(Vec_I_DP &x)
	{
		double dCompensation0,dCompensation1;
		//This function returns the negative value of the LogLikelihood
		const double dPi=3.1415926535897932384626433832795;
		int i,j;
		double dTemp=0.;

		double dLog10=log(10.);
		RevTransform(x);

		int nSize=g_vHistNode.size();
		int nComponents=g_GeometricTrialNode.nNumComponents;
		int nFuncKey=g_GeometricTrialNode.nFuncKey;
		double dX,dX0,dX1,dX2;

		double dLogLik=0.;
		double dLik=0.;
		double dRol[20],dW[20],dArea;

		dX0=g_GeometricTrialNode.fFittingRange.fLeft;
		dX1=g_GeometricTrialNode.fFittingRange.fRight;
		dArea=0.;

		for(j=0;j<nComponents;j++)
		{
			dRol[j]=g_GeometricResultNode.dRol[j];
			dW[j]=g_GeometricResultNode.dWeights[j];
		}
		dArea=Area_Geometric(dRol,dW,nComponents,dX0,dX1);
		switch (nFuncKey)
		{
		case 1:
			for(i=0;i<nSize;i++)
			{
				dLik=0.;
				dX=g_vHistNode[i].dLeft;
				dLik=Geometric(dRol,dW,nComponents,dX);
				dX2=g_vHistNode[i].dData;
				dLogLik+=dX2*log(dLik/dArea)/dLog10;
			}
			break;
		case 2:
			for(i=0;i<nSize;i++)
			{
				dLik=0.;
				dX0=g_vHistNode[i].dLeft;
				dX1=g_vHistNode[i].dRight;
				dLik=log(Area_Geometric(dRol,dW,nComponents,dX0,dX1)/dArea)*g_vHistNode[i].dData/dLog10;
				dLogLik+=dLik;
			}
			break;
		}
		return -dLogLik;
	}
};

using namespace GeometricGlobal;

CGeometricFitting::CGeometricFitting(structGeometricFittingNode aNode, const vector <structHistNode> avHist0)
{
	int nComponents=aNode.nNumComponents;
	int nFreePrams=aNode.nFreeParameters;

	int i,nfunc,j;

	Initialization(aNode, avHist0);


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

	y[0]=LogLikGeometric(x);

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
		
		dTemp=LogLikGeometric(x);
		y[i]=dTemp;
	}

	NR::amoeba(p,y,FTOL,LogLikGeometric,nfunc);
	
	for(i=0;i<NP;i++)
	{
		x[i]=p[0][i];
	}

	RevTransform(x);
	g_GeometricResultNode.dLogLik=-LogLikGeometric(x);
	g_GeometricResultNode.nIterations=nfunc;
	g_GeometricResultNode.dArea=FittedArea_Geometric();
	g_GeometricResultNode.dMeanCA=CalMean(g_GeometricResultNode);
//	double dRol[20];
//	for(i=0;i<20;i++)
//	{
//		dRol[i]=g_GeometricResultNode.dRol[i];
//	}
}

void CGeometricFitting::Initialization(structGeometricFittingNode aNode, const vector <structHistNode> avHist0)
{
	g_vHistNode=avHist0;

	structGeometricFittingConstraintsNode aConstraintsNode=HandlingConstraints(aNode);
	g_ConstraintsNode=aConstraintsNode;

	g_GeometricTrialNode.nNumComponents=aNode.nNumComponents;
	g_GeometricTrialNode.nFuncKey=aNode.nFuncKey;
	g_GeometricTrialNode.fFittingRange=aNode.fFittingRange;
	int nComponents=aNode.nNumComponents;
	int i;
	g_GeometricTrialNode.nFreeParameters=aNode.nFreeParameters;
	for(i=0;i<nComponents;i++)
	{
		g_GeometricTrialNode.dRol[i]=aNode.dRol[i];
		g_GeometricTrialNode.dWeights[i]=aNode.dWeights[i];
		g_GeometricTrialNode.bFreezeW[i]=aNode.bFreezeW[i];
		g_GeometricTrialNode.bFreezeR[i]=aNode.bFreezeR[i];
	}

	g_GeometricResultNode.nNumComponents=aNode.nNumComponents;
	g_GeometricResultNode.nFreeParameters=aNode.nFreeParameters;
	g_GeometricResultNode.nFuncKey=aNode.nFuncKey;
	g_GeometricResultNode.fFittingRange=aNode.fFittingRange;

	for(i=0;i<nComponents;i++)
	{
		g_GeometricResultNode.dRol[i]=aNode.dRol[i];
		g_GeometricResultNode.dWeights[i]=aNode.dWeights[i];
		g_GeometricResultNode.bFreezeW[i]=aNode.bFreezeW[i];
		g_GeometricResultNode.bFreezeR[i]=aNode.bFreezeR[i];
	}
}

structGeometricFittingNode CGeometricFitting::GetFittedNode()
{
	int i;
	float fTemp[20];
	SortFittedNode();
	return g_GeometricResultNode;
}

void CGeometricFitting::SortFittedNode()
{
	int nComponents=g_GeometricResultNode.nNumComponents;
	Vec_IO_DP dRol(nComponents);
	Vec_IO_DP dOrder(nComponents);
	double dWeights[20];
	int i,j;
	float fTemp[20];
	for(i=0;i<nComponents;i++)
	{
		dRol[i]=1./g_GeometricResultNode.dRol[i];
		dWeights[i]=g_GeometricResultNode.dWeights[i];
		dOrder[i]=double(i);
		fTemp[i]=dRol[i];
	}
	NR::sort2(dRol,dOrder);
	int nIndex;
	for(i=0;i<nComponents;i++)
	{
		nIndex=int(dOrder[i]+0.5);
		g_GeometricResultNode.dRol[i]=1./dRol[i];
		fTemp[i]=dRol[i];
		g_GeometricResultNode.dWeights[i]=dWeights[nIndex];
	}

//	for(i=0;i<nComponents;i++)
//	{
//		fTemp[i]=g_GeometricResultNode.dRol[i];
//	}
}



structGeometricFittingConstraintsNode CGeometricFitting::HandlingConstraints(structGeometricFittingNode aNode)
{
	structGeometricFittingConstraintsNode aConstraintsNode;
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

void CGeometricFitting::Construct_x(structGeometricFittingNode aNode, structGeometricFittingConstraintsNode aConstraintsNode, Vec_DP &x)
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
		dTemp/=g_GeometricTrialNode.dRol[i];
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

double CGeometricFitting::CalMean(structGeometricFittingNode aNode)
{
	int nDim=aNode.nNumComponents;
	double dMean=0.;
	double dRol;
	for(int i=0;i<nDim;i++)
	{
		dMean+=aNode.dWeights[i]/(1.-aNode.dRol[i]);
	}
	return dMean;
}
