// GaussianFitting.cpp: implementation of the CGaussianFitting class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "Abf_Processing.h"
#include "GaussianFitting.h"
#include "structures.h"
#include "nr.h"
#include <vector>

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif



using namespace std;
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CGaussianFitting::CGaussianFitting()
{
}

namespace GaussianGlobal{
	structGaussianFittingNode g_GaussianTrialNode;
	structGaussianFittingNode g_GaussianResultNode;

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
		double ai,aj;
		double dG0=1.;
		int i;
		int nComponents=g_GaussianTrialNode.nNumComponents;
		for(i=0;i<nComponents-1;i++)
		{
			ai=g_GaussianTrialNode.dWeights[i+1];
			aj=g_GaussianTrialNode.dWeights[0];
			if(!g_GaussianTrialNode.bFreezeDev)
			{
				x[i+2*nComponents]=log(aj)-log(ai)+dG0;
			}
			else
			{
				x[i+nComponents]=log(aj)-log(ai)+dG0;
			}
		}

		for(i=0;i<nComponents;i++)
		{
			x[i]=1.;
			if(!g_GaussianTrialNode.bFreezeDev)
			{
				x[nComponents]=1.;
				if(!g_GaussianTrialNode.bFreezeDev)
				{
					x[i+nComponents]=1.;
				}
			}
		}

	};

	void RevTransform(Vec_I_DP &x)
	{
		//First x[0] - x[nDim] are for dMean, x[nDim] - x[2*nDim-1] are for dW and the last nDim eliments are for dDev;
		double dG[20];
		double dW[20];
		double dDev[20];
		double dMean[20];
		double dG0=1.;

		int i;

//		FILE* fp=fopen("C:\\epdata\\test.txt","w");

		int nDim=g_GaussianTrialNode.nNumComponents;
		dG[0]=dG0;
		for(i=1;i<nDim;i++)
		{
			if(!g_GaussianTrialNode.bFreezeDev)
			{
				dG[i]=x[2*nDim+i-1];
			}
			else
			{
				dG[i]=x[nDim+i-1];
			}
		}
		double dZ=PartitionFunction(dG,nDim);

		double dTempa[20]; //for monitoring purpose

		double dTemp=0.;
		for(i=0;i<nDim;i++)
		{
			dMean[i]=g_GaussianTrialNode.dMean[i]*x[i];
			if(!g_GaussianTrialNode.bFreezeDev)
			{
				dDev[i]=g_GaussianTrialNode.dDev[i]*exp(x[i+nDim]-1.);
			}
			dTempa[i]=g_GaussianTrialNode.dMean[i];
		}

		for(i=1;i<nDim;i++)
		{
			dW[i]=exp(-dG[i])/dZ;
		}


		for(i=1;i<nDim;i++)
		{
			dTemp+=dW[i];
		}

		dW[0]=1.-dTemp;


		for(i=0;i<nDim;i++)
		{
			g_GaussianResultNode.dMean[i]=dMean[i];
			g_GaussianResultNode.dWeights[i]=dW[i];
			if(!g_GaussianTrialNode.bFreezeDev)
			{
				g_GaussianResultNode.dDev[i]=dDev[i];
			}
		}

		
		
		
//		for(i=0;i<g_GaussianResultNode.nDataSize;i++)
//		{
//			fprintf(fp,"  %f",g_GaussianTrialNode.pdData[i]);
//		}

//		fprintf(fp,"\n\n\n\n");
		
//		for(i=0;i<g_GaussianResultNode.nNumComponents;i++)
//		{
//			fprintf(fp,"  %f",g_GaussianTrialNode.dMean[i]);
//			fprintf(fp,"  %f",g_GaussianTrialNode.dWeights[i]);
//			fprintf(fp,"  %f\n",g_GaussianTrialNode.dDev[i]);
//		}
//		fprintf(fp,"\n\n\n\n");
//		for(i=0;i<g_GaussianResultNode.nFreeParameters;i++)
//		{
//			fprintf(fp,"  %f",x[i]);
//		}

//		fprintf(fp,"\n\n\n\n");

//		for(i=0;i<g_GaussianResultNode.nNumComponents;i++)
//		{
//			fprintf(fp,"  %f",g_GaussianResultNode.dMean[i]);
//			fprintf(fp,"  %f",g_GaussianResultNode.dWeights[i]);
//			fprintf(fp,"  %f\n",g_GaussianResultNode.dDev[i]);
//		}
//		fclose(fp);

	};

	double LogLikGaussian(Vec_I_DP &x)
	{
		//This function returns the negative value of the LogLikelihood
		const double dPi=3.1415926535897932384626433832795;
		int i,j;
		double dTemp=0.;

		double dLog10=log(10.);
		RevTransform(x);

		int nSize=g_GaussianTrialNode.nDataSize;
		int nComponents=g_GaussianTrialNode.nNumComponents;
		double dX;

		double dLogLik=0.;
		double dLik=0.,dExp=0.,dDenorm=0.;
		for(i=0;i<nSize;i++)
		{
			dLik=0.;
			for(j=0;j<nComponents;j++)
			{
				dDenorm=g_GaussianResultNode.dDev[j]*sqrt(2.*dPi);
				dExp=(g_GaussianResultNode.pdData[i]-g_GaussianResultNode.dMean[j])/g_GaussianResultNode.dDev[j];
				dExp=-dExp*dExp/2.;
				dTemp=exp(dExp)/dDenorm;
				dLik+=dTemp;
			}
			if(dLik<10e-288)
			{
				dLik=10e-288;
			}
			dLogLik+=log(dLik)/dLog10;
		}
		return -dLogLik;
	}
};

using namespace GaussianGlobal;

CGaussianFitting::CGaussianFitting(structGaussianFittingNode aNode)
{
	int nComponents=aNode.nNumComponents;
	int nFreePrams=aNode.nFreeParameters;

	int i,nfunc,j;

	Initialization(aNode);


	int MP=nFreePrams+1,NP=nFreePrams;
    const DP FTOL=1.0e-10;
    Vec_DP x(NP),y(MP),temp(nFreePrams);
    Mat_DP p(MP,NP);

	Transform(temp);

	double dTemp;
	for(j=0;j<NP;j++)
	{
		dTemp=temp[j];
		x[j]=temp[j];
		p[0][j]=temp[j];
	}
	y[0]=LogLikGaussian(x);


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
		
		dTemp=LogLikGaussian(x);
		y[i]=dTemp;
	}

	NR::amoeba(p,y,FTOL,LogLikGaussian,nfunc);
	
	for(i=0;i<nFreePrams;i++)
	{
		x[i]=p[0][i];
	}

	RevTransform(x);
	g_GaussianResultNode.dLogLik=LogLikGaussian(x);
}

CGaussianFitting::~CGaussianFitting()
{
}

void CGaussianFitting::Initialization(structGaussianFittingNode aNode)
{
	g_GaussianTrialNode.nDataSize=aNode.nDataSize;
	g_GaussianTrialNode.nNumComponents=aNode.nNumComponents;
	g_GaussianTrialNode.bFreezeDev=aNode.bFreezeDev;
	int nComponents=aNode.nNumComponents;
	int i;
	g_GaussianTrialNode.nFreeParameters=aNode.nFreeParameters;
	for(i=0;i<nComponents;i++)
	{
		g_GaussianTrialNode.dMean[i]=aNode.dMean[i];
		g_GaussianTrialNode.dWeights[i]=aNode.dWeights[i];
		g_GaussianTrialNode.nDFix[i]=aNode.nDFix[i];
		g_GaussianTrialNode.nWFix[i]=aNode.nWFix[i];
		g_GaussianTrialNode.dDev[i]=aNode.dDev[i];
	}
	int nSize=aNode.nDataSize;
	g_GaussianTrialNode.pdData=new double[nSize];
	for(i=0;i<nSize;i++)
	{
		g_GaussianTrialNode.pdData[i]=aNode.pdData[i];
	}

	g_GaussianResultNode.nDataSize=aNode.nDataSize;
	g_GaussianResultNode.bFreezeDev=aNode.bFreezeDev;
	g_GaussianResultNode.nNumComponents=aNode.nNumComponents;
	g_GaussianResultNode.pdData=new double[nSize];
	g_GaussianResultNode.nFreeParameters=aNode.nFreeParameters;
	for(i=0;i<nComponents;i++)
	{
		g_GaussianResultNode.dMean[i]=aNode.dMean[i];
		g_GaussianResultNode.dWeights[i]=aNode.dWeights[i];
		g_GaussianResultNode.nDFix[i]=aNode.nDFix[i];
		g_GaussianResultNode.nWFix[i]=aNode.nWFix[i];
		g_GaussianResultNode.dDev[i]=aNode.dDev[i];
	}

	for(i=0;i<nSize;i++)
	{
		g_GaussianResultNode.pdData[i]=aNode.pdData[i];
	}
}

structGaussianFittingNode CGaussianFitting::GetFittedNode()
{
	int i;
	float fTemp[20];
	int nComponents=g_GaussianResultNode.nNumComponents;
//	for(i=0;i<nComponents;i++)
//	{
//		fTemp[i]=g_GaussianResultNode.dMean[i];
//	}
	SortFittedNode();
//	for(i=0;i<nComponents;i++)
//	{
//		fTemp[i]=g_GaussianResultNode.dMean[i];
//	}
	return g_GaussianResultNode;
}

void CGaussianFitting::SortFittedNode()
{
	int nComponents=g_GaussianResultNode.nNumComponents;
	Vec_IO_DP dMean(nComponents);
	Vec_IO_DP dOrder(nComponents);
	double dWeights[20],dDev[20];
	int i,j;
	float fTemp[20];
	for(i=0;i<nComponents;i++)
	{
		dMean[i]=g_GaussianResultNode.dMean[i];
		dWeights[i]=g_GaussianResultNode.dWeights[i];
		dDev[i]=g_GaussianResultNode.dDev[i];
		dOrder[i]=double(i);
		fTemp[i]=dMean[i];
	}
	NR::sort2(dMean,dOrder);
	int nIndex;
	for(i=0;i<nComponents;i++)
	{
		nIndex=int(dOrder[i]+0.5);
		g_GaussianResultNode.dMean[i]=dMean[i];
		fTemp[i]=dMean[i];
		g_GaussianResultNode.dWeights[i]=dWeights[nIndex];
		g_GaussianResultNode.dDev[i]=dDev[nIndex];
	}

	for(i=0;i<nComponents;i++)
	{
		fTemp[i]=g_GaussianResultNode.dMean[i];
	}
}
