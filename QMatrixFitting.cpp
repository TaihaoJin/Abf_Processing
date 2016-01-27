// QMatrixFitting.cpp: implementation of the CQMatrixFitting class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "Abf_Processing.h"
#include "QMatrixFitting.h"
#include <cmath>

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CQMatrixFitting::CQMatrixFitting()
{

	m_dK0=1.e10;
	int i,j;
	for(i=0;i<200;i++)
	{
		for(j=0;j<200;j++)
		{
			m_dQMatrix[i][j]=0.;
			m_nQConnection[i][j]=-1;
		}
	}
	for(i=0;i<200;i++)
	{
		for(j=0;j<200;j++)
		{
			m_dFMatrix[i][j]=0.;
		}
	}
}

CQMatrixFitting::~CQMatrixFitting()
{

}

void CQMatrixFitting::ReadInput()
{
	int i,j,i0,j0,i1,j1;
	float dTemp;
	int nFixed1,nFixed2;
	int nNumScaledE1,nNumScaledE2;
	CString sTemp;
	CString sFileName="C:\\EPData\\QMaxtrixFitting\\Model1.inp";
	m_sInputFile=sFileName;
	FILE* fp=fopen(sFileName,"r");
	fscanf(fp,"%s%d",sTemp,&m_nNumLevels);
	m_vnStates.clear();
	int nTemp;
	m_sSimulationParameters.vsInputModel.clear();
	m_sSimulationParameters.vsLevelProperties.clear();
	m_sSimulationParameters.vvnStatesInLevels.clear();

	m_nNumStates=0;
	int nStatesInLevel;
//	fscanf(fp,"%s",sTemp);
	for(i=0;i<m_nNumLevels;i++)
	{
		fscanf(fp,"%s%d",sTemp,&nStatesInLevel);
		vector <int> anV;
		fscanf(fp,"%s",sTemp);
		for(j=0;j<nStatesInLevel;j++)
		{
			fscanf(fp,"%d", &nTemp);
			anV.push_back(nTemp);
		}
		m_vvnStatesInLevels.push_back(anV);
		m_vnStates.push_back(nStatesInLevel);
		m_nNumStates+=nStatesInLevel;
	}


	StateIndexConversion();
	fscanf(fp,"%s%d",sTemp,&m_nInitElements);
	m_vsQMConstraints.clear();
	m_vsInputElements.clear();
	m_vsInitialModel.clear();
	m_vsFinalModel.clear();
	float dV1,dV2;
	for(i=0;i<m_nInitElements;i++)
	{
		structQMModelNode aModelNode;
		structQMModelNode bModelNode;
		fscanf(fp,"%s%d%d%f%f%s%d%d%s%d%d",sTemp,&i0,&j0,&dV1,&dV2,sTemp,&nFixed1,&nFixed2,sTemp,&nNumScaledE1,&nNumScaledE2);
		i0=m_vnStateIndexI[i0-1];
		j0=m_vnStateIndexI[j0-1];//The state number i0, j0 start from 1.
		aModelNode.i0=i0;
		aModelNode.j0=j0;
		aModelNode.dV1=dV1;
		aModelNode.dV2=dV2;

		if(nFixed1>1)
			aModelNode.bFixf=true;
		else
			aModelNode.bFixf=false;

		if(nFixed2>1)
			aModelNode.bFixr=true;
		else
			aModelNode.bFixr=false;

		aModelNode.nNumScaledEf=nNumScaledE1;
		aModelNode.nNumScaledEr=nNumScaledE2;

		m_nQConnection[i0][j0]=1;
		m_nQConnection[j0][i0]=1;
		if(nFixed1>0) 
		{
			structQMConstraintNode aNode;
			aNode.dValue=dV1;
			aNode.nType=1;
			aNode.i0=i0;
			aNode.j0=j0;
			aNode.i1=-1;
			aNode.j1=-1;
			m_vsQMConstraints.push_back(aNode);			
		}

		if(nFixed2>0) 
		{
			structQMConstraintNode aNode;
			aNode.dValue=dV2;
			aNode.nType=1;
			aNode.i0=j0;
			aNode.j0=i0;
			aNode.i1=-1;
			aNode.j1=-1;
			m_vsQMConstraints.push_back(aNode);			
		}

		structQMElementNode aNode;
		aNode.i0=i0;
		aNode.j0=j0;
//		aNode.bFreeP=bFree;
		aNode.dV1=dV1;
		aNode.dV2=dV2;
		m_vsInputElements.push_back(aNode);
		for(j=0;j<nNumScaledE1;j++)
		{
			structQMConstraintNode aNode;
			aNode.nType=2;
			aNode.i0=i0;
			aNode.j0=j0;
			fscanf(fp,"%s%d%d%f",sTemp,&i1,&j1,&dTemp);
			i1=m_vnStateIndexI[i1-1];
			j1=m_vnStateIndexI[j1-1];//The state number i1, j1 start from 1.
			aNode.i1=i1;
			aNode.j1=j1;
			aNode.dValue=dTemp;
			m_vsQMConstraints.push_back(aNode);

			structScaledQMNode aSNode;
			m_nQConnection[i1][j1]=1;
			m_nQConnection[j1][i1]=1;
			aSNode.bForward=true;
			aSNode.dScale=dTemp;
			aSNode.i1=i1;
			aSNode.j1=j1;
			aModelNode.svScaledElements.push_back(aSNode);
		}
		for(j=0;j<nNumScaledE2;j++)
		{
			structQMConstraintNode aNode;
			aNode.nType=2;
			aNode.i0=j0;
			aNode.j0=i0;
			fscanf(fp,"%s%d%d%f",sTemp,&i1,&j1,&dTemp);
			aNode.i1=j1;
			aNode.j1=i1;
			aNode.dValue=dTemp;
			m_vsQMConstraints.push_back(aNode);
			//A pair of rate constant is assigned in the input file, the forward and reverse rate constants between states
			//i0 and j0. For type 2 constraint, the reverse rate constants between the two states i1 and j1 keep constant ration
			//dVale. Q(j0,i0)/Q(j1,i1)=dValue
			m_nQConnection[i1][j1]=1;
			m_nQConnection[j1][i1]=1;

			structScaledQMNode aSNode;
			m_nQConnection[i1][j1]=1;
			m_nQConnection[j1][i1]=1;
			aSNode.bForward=false;
			aSNode.dScale=dTemp;
			aSNode.i1=i1;
			aSNode.j1=j1;
			aModelNode.svScaledElements.push_back(aSNode);
		}
		bModelNode=aModelNode;
		m_vsInitialModel.push_back(aModelNode);
		m_vsFinalModel.push_back(bModelNode);
	}
	m_sSimulationParameters.vsInputModel=m_vsInitialModel;
	m_sSimulationParameters.nNumLevels=m_nNumLevels;
	m_sSimulationParameters.nNumStates=m_nNumStates;
	m_sSimulationParameters.vvnStatesInLevels=m_vvnStatesInLevels;

//	CString m_sEvlFileName="C:\\EPData\\QMaxtrixFitting\\Model1.inp";

//	m_cEvl.ReadEvl(m_sEvlFileName);
	fscanf(fp,"%s",m_sEvlFileName);
	CString sEvlOut,sDataOut;
	fscanf(fp,"%s",sEvlOut);
	fscanf(fp,"%s",sDataOut);
	float fLength,fInterval;
	fscanf(fp,"%s%f%s%f",sTemp,&fLength,sTemp,&fInterval);
	fscanf(fp,"%s",sTemp);
	float fAmp,fDev;
	int nLevel;
	for(i=0;i<m_nNumLevels;i++)
	{
		structLevelPropertyNode aNode;
		fscanf(fp,"%s%d%s%f%s%f",sTemp,nLevel,sTemp,&fAmp,sTemp,&fDev);
		aNode.fAmp=fAmp;
		aNode.fDev=fDev;
		aNode.nLevel=nLevel;
		m_sSimulationParameters.vsLevelProperties.push_back(aNode);
	}
}

void CQMatrixFitting::InitQMatrix()
{
	int i,j;
	int nSize=m_nNumStates;
	for(i=0;i<nSize;i++)
	{
		for(j=0;j<nSize;j++)
		{
			m_dQMatrix[i][j]=0;
		}
	}
}

void CQMatrixFitting::UpdateQMatrix()
{
	int nSize=m_vsFMElements.size();
	int i,j,i0,j0,nScaledE;
	structFMElementNode aNode;
	for(i=0;i<nSize;i++)
	{
		aNode=m_vsFMElements[i];
		i0=aNode.i0;
		j0=aNode.j0;
		if(i0!=j0)
		{
			m_dQMatrix[i0][j0]=m_dK0*exp(m_dFMatrix[i0][i0]-m_dFMatrix[i0][j0]);
			m_dQMatrix[j0][i0]=m_dK0*exp(m_dFMatrix[j0][j0]-m_dFMatrix[i0][j0]);
		}
	}

	for(i=0;i<m_nNumStates;i++)
	{
		m_dQMatrix[i][i]=0.;
		for(j=0;j<m_nNumStates;j++)
		{
			if(i!=j)
				m_dQMatrix[i][i]-=m_dQMatrix[i][j];
		}
	}
}

void CQMatrixFitting::Fitting(vector <double> dVectT, vector <double> &dVectF)
{
//	int MP=m_nNumFreeElements+1;
	int MP=dVectT.size()+1;
	int NP=MP-1;
	double dTempv[10];

//    const double FTOL=1.0e-10;
    const double FTOL=1.0e-5;
 //   Vec_DP x(NP),y(MP),temp(NP);
 //   Mat_DP p(MP,NP);

	int i,j,nfunc;

//	double x[NP],y[MP],temp[NP],p[MP][NP];
	dVect x,y,temp;
	vector <dVect> p;

	for(i=0;i<NP;i++)
	{
		x.push_back(0.);
		temp.push_back(0.);
	}

	for(i=0;i<MP;i++)
	{
		y.push_back(0.);
		dVect aVect;
		for(j=0;j<NP;j++)
		{
			aVect.push_back(0.);
		}
		p.push_back(aVect);
	}


	double dTemp;
	for(j=0;j<NP;j++)
	{
		dTemp=dVectT[j];
		x[j]=dTemp;
		p[0][j]=dVectT[j];
	}
//	need to initialize temp

	y[0]=LogLikelihood(x);

	for(j=0;j<NP;j++)
	{
		dTemp=p[0][j];
		dVectF[j]=dTemp;
		if(j<10)
			dTempv[j]=p[0][j];
	}
	dTemp=y[0];


	for(i=1;i<MP;i++)
	{
		for(j=0;j<NP;j++)
		{
			if(j+1==i)
			{
				dTemp=dVectT[j];
			}
			else
			{
				dTemp=0.;
			}
			x[j]=dTemp;
			p[i][j]=dTemp;
		}
		
		dTemp=LogLikelihood(x);
		y[i]=dTemp;
	}

	amoeba(p,y,FTOL,nfunc);	


	for(j=0;j<NP;j++)
	{
		dVectF[j]=p[0][j];
		if(j<10)
			dTempv[j]=p[0][j];
	}
	dTemp=y[0];
}

double CQMatrixFitting::LogLikelihood(dVect x)
{
	int nSize=x.size();
	double dValue=0.;
	for(int i=0;i<nSize;i++)
	{
		dValue+=fabs(i*i-x[i]);
	}

	return dValue;
}

void CQMatrixFitting::amoeba(vector <dVect> &p, dVect &y, double ftol, int &nfunk)
{
//void amoeba(vector <dVect> &p, dVect &y, double ftol, doulbe LogLikelihood(dVect &), int &nfunc);

//void NR::amoeba(Mat_IO_DP &p, Vec_IO_DP &y, const DP ftol, DP LogLikelihood(Vec_I_DP &), int &nfunk)
//{
	const int NMAX=5000;
	const double TINY=1.0e-10;
	int i,ihi,ilo,inhi,j;
	double rtol,ysave,ytry;

	int mpts=p.size();
	int ndim=p[0].size();
//	Vec_DP psum(ndim);

//	double psum[ndim];
	dVect psum;
	for(i=0;i<ndim;i++)
	{
		psum.push_back(0.);
	}

	nfunk=0;
	get_psum(p,psum);
	for (;;) {
		ilo=0;
		ihi = y[0]>y[1] ? (inhi=1,0) : (inhi=0,1);
		for (i=0;i<mpts;i++) {
			if (y[i] <= y[ilo]) ilo=i;
			if (y[i] > y[ihi]) {
				inhi=ihi;
				ihi=i;
			} else if (y[i] > y[inhi] && i != ihi) inhi=i;
		}
		rtol=2.0*fabs(y[ihi]-y[ilo])/(fabs(y[ihi])+fabs(y[ilo])+TINY);
		if (rtol < ftol) {
			SWAP(y[0],y[ilo]);
			for (i=0;i<ndim;i++) SWAP(p[0][i],p[ilo][i]);
			break;
		}
//		if (nfunk >= NMAX) nrerror("NMAX exceeded");
		nfunk += 2;
		ytry=amotry(p,y,psum,ihi,-1.0);
		if (ytry <= y[ilo])
			ytry=amotry(p,y,psum,ihi,2.0);
		else if (ytry >= y[inhi]) {
			ysave=y[ihi];
			ytry=amotry(p,y,psum,ihi,0.5);
			if (ytry >= ysave) {
				for (i=0;i<mpts;i++) {
					if (i != ilo) {
						for (j=0;j<ndim;j++)
							p[i][j]=psum[j]=0.5*(p[i][j]+p[ilo][j]);
						y[i]=LogLikelihood(psum);
					}
				}
				nfunk += ndim;
				get_psum(p,psum);
			}
		} else --nfunk;
	}
}

void CQMatrixFitting::get_psum(vector <dVect> &p, dVect &psum)
{
		int i,j;
		double sum;

		int mpts=p.size();
		int ndim=p[0].size();
		for (j=0;j<ndim;j++) {
			for (sum=0.0,i=0;i<mpts;i++)
				sum += p[i][j];
			psum[j]=sum;
		}
}

double CQMatrixFitting::amotry(vector <dVect> &p, dVect &y, dVect &psum, const int ihi, const double fac)
{
//DP NR::amotry(Mat_IO_DP &p, Vec_O_DP &y, Vec_IO_DP &psum, DP LogLikelihood(Vec_I_DP &), const int ihi, const DP fac)
//{
	int j;
	double fac1,fac2,ytry;

	int ndim=p[0].size();
//	Vec_DP ptry(ndim);
	dVect ptry;
	fac1=(1.0-fac)/ndim;
	fac2=fac1-fac;
	for (j=0;j<ndim;j++)
		ptry.push_back(psum[j]*fac1-p[ihi][j]*fac2);
	ytry=LogLikelihood(ptry);
	if (ytry < y[ihi]) {
		y[ihi]=ytry;
		for (j=0;j<ndim;j++) {
			psum[j] += ptry[j]-p[ihi][j];
			p[ihi][j]=ptry[j];
		}
	}
	return ytry;
}

void CQMatrixFitting::InitializeFMEList()
{
	//This function build the list of F matrix elements computed from the list of Q matrix elements 
	int i,j,i0,j0;
	int nSize=m_vsInputElements.size();
	vector <int> vnPool;
	vector <int> vnRemaining;
	for(i=0;i<nSize;i++)
	{
		vnRemaining.push_back(i);
	}
	structQMElementNode aNode;
	aNode=m_vsInputElements[0];
	TransferVectElement(vnRemaining,vnPool,0);
	i0=aNode.i0;
	j0=aNode.j0;
	structFMElementNode bNode;
	bNode.dValue=0.;
	bNode.i0=i0;
	bNode.j0=i0;
	bNode.nConstraintIndex=-1;
	m_nFMEPosition[i0][i0]=0;
	m_nFMEPosition[j0][i0]=1;
	m_nFMEPosition[i0][j0]=1;
	m_nFMEPosition[j0][j0]=2;
	structFMElementNode cNode;
	cNode.dValue=log(m_dK0/aNode.dV1);
	cNode.i0=i0;
	cNode.j0=j0;
	cNode.nConstraintIndex=-1;
	structFMElementNode dNode;
	dNode.dValue=log(aNode.dV2/aNode.dV1);
	dNode.i0=j0;
	dNode.j0=j0;
	dNode.nConstraintIndex=-1;
	m_dFMatrix[i0][i0]=bNode.dValue;
	m_dFMatrix[i0][j0]=cNode.dValue;
	m_dFMatrix[j0][i0]=cNode.dValue;
	m_dFMatrix[j0][j0]=dNode.dValue;
	m_vsFMElements.push_back(bNode);	
	m_vsFMElements.push_back(cNode);	
	m_vsFMElements.push_back(dNode);

	int nRemainingE=vnRemaining.size();
	int nPoolSize=vnPool.size();
	int nI,nJ;
	int nMaxIt=nSize;
	int nIt=0;
	int nIL,nIR,nJL,nJR,nE;
	double dV1,dV2;
	nE=3; //the number of the nodes in m_vsFMElements;
	while(nRemainingE>0&&nIt<nMaxIt)
	{
		nIt++;
		for(i=0;i<nRemainingE;i++)
		{
			for(j=0;j<nPoolSize;j++)
			{
				nI=vnRemaining[i];
				nJ=vnPool[j];
				nIL=m_vsInputElements[nI].i0;
				nIR=m_vsInputElements[nJ].i0;
				nJL=m_vsInputElements[nI].j0;
				nJR=m_vsInputElements[nJ].j0;

				if(nIL==nIR||nIL==nJR)
				{
					//The value of F(nIL,nIL) has been assigned already
					structFMElementNode eNode;
					structFMElementNode fNode;
					dV1=m_vsInputElements[nI].dV1;
					dV2=m_vsInputElements[nI].dV2;
					eNode.i0=nJL;
					eNode.j0=nJL;
					eNode.dValue=m_dFMatrix[nIL][nIL]+log(dV2/dV1);
					eNode.nConstraintIndex=-1;
					fNode.i0=nIL;
					fNode.j0=nJL;
					fNode.dValue=m_dFMatrix[nIL][nIL]+log(m_dK0/dV1);
					fNode.nConstraintIndex=-1;
					m_vsFMElements.push_back(eNode);
					m_nFMEPosition[nJL][nJL]=nE;
					nE++;
					m_vsFMElements.push_back(fNode);
					m_nFMEPosition[nIL][nJL]=nE;
					nE++;
					m_dFMatrix[nJL][nJL]=eNode.dValue;
					m_dFMatrix[nIL][nJL]=fNode.dValue;	
					m_dFMatrix[nJL][nIL]=fNode.dValue;	
					TransferVectElement(vnRemaining,vnPool,i);
					break;
				}

				if(nJL==nIR||nJL==nJR)
				{
					//The value of F(nJL,nJL) has been assigned already
					structFMElementNode gNode;
					structFMElementNode hNode;
					dV1=m_vsInputElements[nI].dV1;
					dV2=m_vsInputElements[nI].dV2;
					gNode.i0=nIL;
					gNode.j0=nIL;
					gNode.dValue=m_dFMatrix[nJL][nJL]+log(dV1/dV2);
					gNode.nConstraintIndex=-1;
					hNode.i0=nIL;
					hNode.j0=nJL;
					hNode.dValue=m_dFMatrix[nJL][nJL]+log(m_dK0/dV2);
					hNode.nConstraintIndex=-1;
					m_vsFMElements.push_back(gNode);
					m_nFMEPosition[nIL][nIL]=nE;
					nE++;
					m_vsFMElements.push_back(hNode);
					m_nFMEPosition[nIL][nJL]=nE;
					nE++;
					m_dFMatrix[nIL][nIL]=gNode.dValue;
					m_dFMatrix[nIL][nJL]=hNode.dValue;	
					m_dFMatrix[nJL][nIL]=hNode.dValue;	
					TransferVectElement(vnRemaining,vnPool,i);
					break;
				}
			}
		}
		nRemainingE=vnRemaining.size();
		nPoolSize=vnPool.size();
	}
}

void CQMatrixFitting::TransferVectElement(vector <int> &vnA, vector <int> &vnB, int nIA)
{
	int nSize=vnA.size();
	vnB.push_back(vnA[nIA]);
	EraseVectElement(vnA,nIA);
}

void CQMatrixFitting::ApplyConstraints()
{
	vector <nVect> vvnCPool;
	int i,j;
	int nNumConstraints=m_vsFMConstraints.size();

	//The following lines group the constraints into differents pools of connected (directly or indirectly) constraints. 
	//Two constraints are regarded to be connected if they share any common involved F Matrix elements.
	for(i=0;i<nNumConstraints;i++)
	{
		JoinConstraintPool(vvnCPool,i);
	}

	int nSpecifiedI=-1;
	int nIndex;
	structFMConstraintNode aNode;
	for(i=0;i<nNumConstraints;i++)
	{
		aNode=m_vsFMConstraints[i];
		nSpecifiedI=SpecifyConstraintIndex(aNode);
		if(nSpecifiedI<0)
		{
			//Problem, there are conflicted constraints, need to exit and output error message.
			exit(0);
		}
		else
		{
			aNode.nSpecifiedI=nSpecifiedI;
			nIndex=aNode.vnInvolvedFMEs[nSpecifiedI];
			ConstrainFME(nIndex,aNode.nCIndex);
			m_vsFMConstraints[i]=aNode;
		}
	}
	m_vvnCPool=vvnCPool;
}

void CQMatrixFitting::JoinConstraintPool(vector <nVect> &vvnCPool, int nIndex)
{
	int i0,j0,i1,j1,nCType;
	int nFE0[4],nFE1[4];
	structFMConstraintNode aNode;
	structFMConstraintNode bNode;
	aNode=m_vsFMConstraints[nIndex];
	i0=aNode.i0;
	j0=aNode.j0;
	nCType=aNode.nType;
	nFE0[0]=i0*m_nNumStates+i0;
	nFE0[1]=i0*m_nNumStates+j0;
	int nL1,nL2;
	nL1=2;
	if(nCType==2)
	{
		i1=aNode.i1;
		j1=aNode.j1;
		nFE0[2]=i1*m_nNumStates+i1;
		nFE0[3]=i1*m_nNumStates+j1;
		nL1=4;
	}
	int nSize0=vvnCPool.size();
	int nSize1;

	bool bJoined=false;
	int i,j,k,l,nPCI;
	int nPIndex;
	int nCommonL,nCommonR; //the index of the common F matrix element of the joining constrain (child) and the related 
					       //constrain who's already a pool member (the parent);
	
	for(i=0;i<nSize0;i++)
	{
		nSize1=vvnCPool[i].size();
		for(j=0;j<nSize1;j++)
		{
			aNode=m_vsFMConstraints[vvnCPool[i].at(j)];
			i0=aNode.i0;
			j0=aNode.j0;
			nCType=aNode.nType;
			nFE1[0]=i0*m_nNumStates+i0;
			nFE1[1]=i0*m_nNumStates+j0;
			nL2=2;
			if(nCType==2)
			{
				j1=aNode.j1;
				i1=aNode.i1;
				nFE1[2]=i1*m_nNumStates+i1;
				nFE1[3]=i1*m_nNumStates+j1;
				nL2=4;
			}
			for(k=0;k<nL1;k++)
			{
				for(l=0;l<nL2;l++)
				{
					if(nFE0[k]==nFE1[l])
					{
						bNode=aNode;//bNode is the parent constaint
						bJoined=true;
						nCommonL=k;
						nCommonR=l;
						nPIndex=i;
					}
				}
			}
		}
	}
	if(bJoined)
	{
		vvnCPool[nPIndex].push_back(nIndex);
		aNode=m_vsFMConstraints[nIndex];//aNode is the child constaint
		aNode.nPCI=vvnCPool[nPIndex].size()-1;
		aNode.nPIndex=nPIndex;
		aNode.structParent.nCommonStates=nCommonL;
		aNode.structParent.nParentIndex=bNode.nCIndex;
		aNode.structParent.nBirthIndex=bNode.vsChildren.size()-1;
		struct structFMCChildNode aChildNode;
		aChildNode.nChildIndex=nIndex;
		aChildNode.nCommonStates=nCommonR;
		aNode.vnFamilyTree=bNode.vnFamilyTree;
		aNode.vnFamilyTree.push_back(nIndex);
		bNode.vsChildren.push_back(aChildNode);
		m_vsFMConstraints[aNode.nCIndex]=aNode;
		m_vsFMConstraints[bNode.nCIndex]=bNode;
	}
	else
	{
		vector <int> anVect;
		anVect.push_back(nIndex);
		m_vsFMConstraints[nIndex].vnFamilyTree.push_back(nIndex);
		vvnCPool.push_back(anVect);
	}
}

void CQMatrixFitting::InitializeConstraints()
{
	int nSize=m_vsQMConstraints.size();
	m_vsFMConstraints.clear();
	int i,j,nIndex;
	structQMConstraintNode aNode;
	int i0,i1,j0,j1;

	for(i=0;i<nSize;i++)
	{
		aNode=m_vsQMConstraints[i];
		structFMConstraintNode bNode;
		i0=aNode.i0;
		i1=aNode.i1;
		j0=aNode.j0;
		j1=aNode.j1;
		bNode.i0=i0;
		bNode.j0=j0;
		bNode.i1=i1;
		bNode.j1=j1;
		bNode.nCIndex=i;
		bNode.nType=aNode.nType;
		bNode.vnFamilyTree.clear();
		bNode.vsChildren.clear();
		bNode.dValue=aNode.dValue;
		bNode.structParent.nBirthIndex=-1;
		bNode.structParent.nCommonStates=-1;
		bNode.structParent.nParentIndex=-1;

		if(bNode.nType==1)
		{
			bNode.vnInvolvedFMEs.push_back(m_nFMEPosition[i0][j0]);
			bNode.vnInvolvedFMEs.push_back(m_nFMEPosition[i0][i0]);
			bNode.vnFreeFME.push_back(0);
			bNode.vnFreeFME.push_back(1);
			nIndex=m_nFMEPosition[i0][i0];
			m_vsFMElements[nIndex].vnRelatedConstraints.push_back(i);
			nIndex=m_nFMEPosition[i0][j0];
			m_vsFMElements[nIndex].vnRelatedConstraints.push_back(i);
		}
		else if(bNode.nType==2)
		{
			bNode.vnInvolvedFMEs.push_back(m_nFMEPosition[i1][j1]);
			bNode.vnInvolvedFMEs.push_back(m_nFMEPosition[i1][i1]);
			bNode.vnFreeFME.push_back(0);
			bNode.vnFreeFME.push_back(1);
			nIndex=m_nFMEPosition[i1][i1];
			m_vsFMElements[nIndex].vnRelatedConstraints.push_back(i);
			nIndex=m_nFMEPosition[i1][j1];
			m_vsFMElements[nIndex].vnRelatedConstraints.push_back(i);

			bNode.vnInvolvedFMEs.push_back(m_nFMEPosition[i0][j0]);
			bNode.vnInvolvedFMEs.push_back(m_nFMEPosition[i0][i0]);
			bNode.vnFreeFME.push_back(2);
			bNode.vnFreeFME.push_back(3);
			nIndex=m_nFMEPosition[i0][i0];
			m_vsFMElements[nIndex].vnRelatedConstraints.push_back(i);
			nIndex=m_nFMEPosition[i0][j0];
			m_vsFMElements[nIndex].vnRelatedConstraints.push_back(i);
		}
		m_vsFMConstraints.push_back(bNode);
	}
}

int CQMatrixFitting::SpecifyConstraintIndex(structFMConstraintNode aNode)
{
	int nIndex;
	int i0,j0,i1,j1,i,j;
	i0=aNode.i0;
	j0=aNode.j0;
	if(aNode.vnFreeFME.empty())
		return FreeConstrainedFMElements(aNode);
	else
	{
		nIndex=aNode.vnFreeFME[0];
		EraseVectElement(aNode.vnFreeFME, 0);
		return nIndex;
	}
}

int CQMatrixFitting::FreeConstrainedFMElements(structFMConstraintNode aNode)
{
	int nPIndex=aNode.nPIndex;
	int nSize=m_vvnCPool[nPIndex].size();
	vector <int> vnPool=m_vvnCPool[nPIndex];
	int nIndex=AdjustableConstraint(vnPool);
	if(nIndex!=-1)
	{
		PassFreeCFME(m_vsFMConstraints[nIndex],aNode);
		return m_vsFMConstraints[aNode.nCIndex].vnFreeFME[0];
	}
	else
	{
		//There is not adjustable constraint in the pool
		return -1;
	}
}

int CQMatrixFitting::AdjustableConstraint(vector <int> vnPool)
{
	int nSize=vnPool.size();
	int nIndex=-1;
	int i;
	int nSpecifiedI=-1;
	structFMConstraintNode aNode;

	for(i=0;i<nSize;i++)
	{
		nIndex=vnPool[i];
		aNode=m_vsFMConstraints[nIndex];
		if(!aNode.vnFreeFME.empty())
		{
			return nIndex;
		}
	}
	//There is not adjustable constraint in the pool
	return -1;
}

void CQMatrixFitting::EraseVectElement(vector <int> &aVector, int nIndex)
{
	int nSize=aVector.size();
	for(int i=nIndex+1;i<nSize;i++)
	{
		aVector[i-1]=aVector[i];
	}
	aVector.pop_back();
}

	

void CQMatrixFitting::PassFreeCFME(structFMConstraintNode aDoner, structFMConstraintNode aAcceptor)
{
	int nCAIndex=CommonAncestor(aDoner,aAcceptor);
	vector <int> aVect=aDoner.vnFamilyTree;
	int nSize=aVect.size();
	int nIndex=FindVectElement(aVect, nCAIndex);
	int i;
	for(i=nSize-1;i>nIndex;i--)
	{
		UpPassFreeFME(aVect[i]);
	}
	//The common ancestor nCAIndex got a free involved F matrix element
	aVect=aAcceptor.vnFamilyTree;
	nIndex=FindVectElement(aVect, nCAIndex);
	for(i=nIndex+1;i<nSize;i++)
	{
		DownGrabFreeFME(aVect[i]);
	}
}

int CQMatrixFitting::FindVectElement(vector <int> aVect, int nI)
{
	int nSize=aVect.size();
	for(int i=0;i<nSize;i++)
	{
		if(aVect[i]==nI)
			return i;
	}
	//did not find nI in nVect;
	return -1;
}

int CQMatrixFitting::CommonAncestor(structFMConstraintNode aNode, structFMConstraintNode bNode)
{
	vector <int> nVect=aNode.vnFamilyTree;
	int nSize=nVect.size();
	int nIndex;
	for(int i=0;i<nSize;i++)
	{
		nIndex=nVect[i];
		if(FindVectElement(bNode.vnFamilyTree,nIndex)>=0)
		{
			return nIndex;
		}
	}
	//The two constraints do not belong to the same pool.
	return -1;
}

void CQMatrixFitting::UpPassFreeFME(int nIndex)
{
	structFMConstraintNode aNode=m_vsFMConstraints[nIndex];
	int nIP=aNode.structParent.nParentIndex;
	int nIC=aNode.structParent.nCommonStates;
	if(m_vsFMElements[nIC].nConstraintIndex>=0)
	{
		//The common element is already constrained
		if(aNode.vnFreeFME.size()>0)
		{
			int nCNI=aNode.vnFreeFME[0];
			int nFME=aNode.vnInvolvedFMEs[nCNI];
			aNode.nSpecifiedI=nCNI;
			ConstrainFME(nFME,aNode.nCIndex);
			FreeCFME(nIC);
		}
	}
	m_vsFMConstraints[nIndex]=aNode;
}

void CQMatrixFitting::DownGrabFreeFME(int nIndex)
{		
	structFMConstraintNode aNode=m_vsFMConstraints[nIndex];
	int nIP=aNode.structParent.nParentIndex;
	structFMConstraintNode bNode=m_vsFMConstraints[nIP];
	int nIB=aNode.structParent.nBirthIndex;
	int nIC=bNode.vsChildren[nIB].nCommonStates;
	if(m_vsFMElements[nIC].nConstraintIndex>=0)
	{
		//The common element is already constrained
		if(bNode.vnFreeFME.size()>0)
		{
			int nCNI=bNode.vnFreeFME[0];
			int nFME=bNode.vnInvolvedFMEs[nCNI];
			bNode.nSpecifiedI=nCNI;
			ConstrainFME(nFME,bNode.nCIndex);
			FreeCFME(nIC);
		}
	}
	m_vsFMConstraints[nIP]=bNode;
}

void CQMatrixFitting::ConstrainFME(int nFME, int nFMEC)
{
	int nFMEC1;
	structFMElementNode aNode;
	aNode=m_vsFMElements[nFME];
	aNode.nConstraintIndex=nFMEC;
	structFMConstraintNode bNode;
	int nSize=aNode.vnRelatedConstraints.size();
	int nI;
	for(int i=0;i<nSize;i++)
	{
		nFMEC1=aNode.vnRelatedConstraints[i];
		bNode=m_vsFMConstraints[nFMEC1];
		nI=FindVectElement(bNode.vnInvolvedFMEs,nFME);
		if(nI>=0)
		{
			EraseVectElement(bNode.vnInvolvedFMEs,nI);
		}
		bNode.nSpecifiedI=nI;
		m_vsFMConstraints[nFMEC1]=bNode;
	}
	m_vsFMElements[nFME]=aNode;
}

void CQMatrixFitting::FreeCFME(int nFME)
{
	int nFMEC;
	structFMElementNode aNode;
	aNode=m_vsFMElements[nFME];
	aNode.nConstraintIndex=-1;
	structFMConstraintNode bNode;
	int nSize=aNode.vnRelatedConstraints.size();
	int nI;
	for(int i=0;i<nSize;i++)
	{
		nFMEC=aNode.vnRelatedConstraints[i];
		bNode=m_vsFMConstraints[nFMEC];
		nI=FindVectElement(bNode.vnInvolvedFMEs,nFME);
		if(nI>=0)
		{
			bNode.vnFreeFME.push_back(nI);
		}
		m_vsFMConstraints[nFMEC]=bNode;
	}
}

void CQMatrixFitting::PrepareInput()
{
	ReadInput();
	InitializeFMEList();
	InitializeConstraints();
	ApplyConstraints();
	GenerateFreeFMEList();
	CalCFMElements();
	UpdateFMatrix();
	UpdateQMatrix();
	TrialMinimization();
	UpdataModel();
	CString sFile=ChangeFileExt(m_sInputFile,"Out");
	FILE* fp=fopen(sFile,"w");
	OutputModel(fp, m_vsInitialModel);
	fprintf(fp,"\n\nQ Matrix:\n");
	OutputMatrix(fp,m_dQMatrix,m_nNumStates);
	fprintf(fp,"\n\nF Matrix:\n");
	OutputMatrix(fp,m_dFMatrix,m_nNumStates);
	OutputModel(fp, m_vsFinalModel);
	fclose(fp);
}

void CQMatrixFitting::GenerateFreeFMEList()
{
	int nSize=m_vsFMElements.size();
	m_vnFreeFMElements.clear();
	m_vnCFMElements.clear();
	int nCFMEI=0;
	for(int i=0;i<nSize;i++)
	{
		if(m_vsFMElements[i].nConstraintIndex>=0)
		{
			m_vsFMElements[i].nCFMEIndex=nCFMEI;
			nCFMEI++;
			m_vnCFMElements.push_back(i);
		}
		else
		{
			m_vsFMElements[i].nCFMEIndex=-1;
			m_vnFreeFMElements.push_back(i);
		}
	}
}

void CQMatrixFitting::TrialMinimization()
{
	dVect aVect, bVect;
	for(int i=0;i<4;i++)
	{
		aVect.push_back(0.1*i);
	}
	bVect=aVect;
	Fitting(aVect,bVect);
}

void CQMatrixFitting::OutputModel(FILE *fp, vector <structQMModelNode> avsNodes)
{
	structQMModelNode aModelNode;
	int i,j,i0,j0,i1,j1;
	float dTemp;
	int nFixed1,nFixed2;
	int nNumScaledE1,nNumScaledE2;
	CString sTemp;
	fprintf(fp,"NumOfLevels: %2d\n",m_nNumLevels);
	int nTemp;

	structScaledQMNode aSNode;
	fprintf(fp,"NumOfStatesInLevel");
	for(i=0;i<m_nNumLevels;i++)
	{
		fprintf(fp,"%6d",m_vnStates[i]);
	}
	fprintf(fp,"\n");
	fprintf(fp,"NumOfInputElements: %4d\n",m_nInitElements);
	float dV1,dV2;
	for(i=0;i<m_nInitElements;i++)
	{
		aModelNode=avsNodes[i];
		i0=aModelNode.i0;
		j0=aModelNode.j0;
		dV1=aModelNode.dV1;
		dV2=aModelNode.dV2;
		if(aModelNode.bFixf)
			nFixed1=1;
		else
			nFixed1=0;

		if(aModelNode.bFixr)
			nFixed2=1;
		else
			nFixed2=0;

		nNumScaledE1=aModelNode.nNumScaledEf;
		nNumScaledE2=aModelNode.nNumScaledEr;

		fprintf(fp,"%3d  %3d  %12.4f  %12.4f Fixed: %2d %2d Scaled:  %2d  %2d",m_vnStateIndexO[i0],m_vnStateIndexO[j0],dV1,dV2,nFixed1,nFixed2,nNumScaledE1,nNumScaledE2);

		for(j=0;j<nNumScaledE1;j++)
		{
			aSNode=aModelNode.svScaledElements[j];
			fprintf(fp,"  ForwardScaledElements: %d  %d  %10.3f",m_vnStateIndexO[aSNode.i1],m_vnStateIndexO[aSNode.j1],aSNode.dScale);
		}
		for(j=0;j<nNumScaledE2;j++)
		{
			aSNode=aModelNode.svScaledElements[nNumScaledE1+j];
			fprintf(fp,"  ReverseScaledElements: %d  %d  %10.3f",m_vnStateIndexO[aSNode.i1],m_vnStateIndexO[aSNode.j1],aSNode.dScale);
		}
		fprintf(fp,"\n");
	}
}

void CQMatrixFitting::OutputMatrix(FILE *fp, double pdData[200][200], int nDim1)
{
	int i,j;
	fprintf(fp,"\n");
	fprintf(fp,"\n");
	for(i=0;i<nDim1;i++)
	{
		for(j=0;j<nDim1;j++)
		{
			fprintf(fp,"  %15.4f",pdData[m_vnStateIndexO[i]][m_vnStateIndexO[j]]);
		}
		fprintf(fp,"\n");
	}
	fprintf(fp,"\n");
}

void CQMatrixFitting::UpdataModel()
{
	int nSize=m_vsFinalModel.size();
	structQMModelNode aNode;
	for(int i=0;i<nSize;i++)
	{
		aNode=m_vsFinalModel[i];
		aNode.dV1=m_dQMatrix[aNode.i0][aNode.j0];
		aNode.dV2=m_dQMatrix[aNode.j0][aNode.i0];
		m_vsFinalModel[i]=aNode;
	}
}

CString CQMatrixFitting::ChangeFileExt(CString sFileName, CString sExt)
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


void CQMatrixFitting::CalCFMElements()
{
	int i,j,nSize;
	nSize=m_vnCFMElements.size();
	vector <dVect> a, u, v;
	dVect w, b, x;

	for(i=0;i<nSize;i++)
	{
		w.push_back(0.);
		b.push_back(0.);
		x.push_back(0.);
		dVect aVect, bVect,cVect;
		for(j=0;j<nSize;j++)
		{
			aVect.push_back(0.);
			bVect.push_back(0.);
			cVect.push_back(0.);
		}
		a.push_back(aVect);
		u.push_back(bVect);
		v.push_back(cVect);		
	}

	structFMConstraintNode aCNode0,aCNode1;
	structFMElementNode aENode, aENode1;
	int nEIndex,nEIndex1,nCFMEI;
	int nCIndex;
	for(i=0;i<nSize;i++)
	{
		nEIndex=m_vnCFMElements[i];
		aENode=m_vsFMElements[nEIndex];
		nCIndex=aENode.nConstraintIndex;
		aCNode0=m_vsFMConstraints[i];
		if(aCNode0.nType==1)
		{
			b[i]=-log(aCNode0.dValue/m_dK0);
			switch (aCNode0.nSpecifiedI)
			{
			case 0:
				nEIndex1=aCNode0.vnInvolvedFMEs[0];
				aENode1=m_vsFMElements[nEIndex1];
				nCFMEI=aENode1.nCFMEIndex;
				if(aENode1.nConstraintIndex>=0)
				{
					a[i][nCFMEI]=1.;//i==nCFMEI if the code is correct
				}
				else
				{
					b[i]-=aENode1.dValue;  //This case should not happen, but leave this here for a moment.
				}	
				
				nEIndex1=aCNode0.vnInvolvedFMEs[1];
				aENode1=m_vsFMElements[nEIndex1];
				nCFMEI=aENode1.nCFMEIndex;
				if(aENode1.nConstraintIndex>=0)
				{
					a[i][nCFMEI]=-1.;
				}
				else
				{
					b[i]+=aENode1.dValue;
				}					
				break;
					
			case 1:

				nEIndex1=aCNode0.vnInvolvedFMEs[0];
				aENode1=m_vsFMElements[nEIndex1];
				nCFMEI=aENode1.nCFMEIndex;
				if(aENode1.nConstraintIndex>=0)
				{
					a[i][nCFMEI]=1.;
				}
				else
				{
					b[i]-=aENode1.dValue; 
				}	
				
				nEIndex1=aCNode0.vnInvolvedFMEs[1];
				aENode1=m_vsFMElements[nEIndex1];
				nCFMEI=aENode1.nCFMEIndex;
				if(aENode1.nConstraintIndex>=0)
				{
					a[i][nCFMEI]=-1.;//i==nCFMEI if the code is correct
				}
				else
				{
					b[i]+=aENode1.dValue; //This case should not happen, but leave this here for a moment.
				}					
				break;

			default:
				break;
			}
		}
		else if(aCNode0.nType==2)
		{
			b[i]=-log(aCNode0.dValue);
			switch (aCNode0.nSpecifiedI)
			{
			case 0:
				nEIndex1=aCNode0.vnInvolvedFMEs[0];
				aENode1=m_vsFMElements[nEIndex1];
				nCFMEI=aENode1.nCFMEIndex;
				if(aENode1.nConstraintIndex>=0)
				{
					a[i][nCFMEI]=1.;//i==nCFMEI if the code is correct
				}
				else
				{
					b[i]-=aENode1.dValue;  //This case should not happen, but leave this here for a moment.
				}	
				
				nEIndex1=aCNode0.vnInvolvedFMEs[1];
				aENode1=m_vsFMElements[nEIndex1];
				nCFMEI=aENode1.nCFMEIndex;
				if(aENode1.nConstraintIndex>=0)
				{
					a[i][nCFMEI]=-1.;
				}
				else
				{
					b[i]+=aENode1.dValue;
				}					
				
				nEIndex1=aCNode0.vnInvolvedFMEs[2];
				aENode1=m_vsFMElements[nEIndex1];
				nCFMEI=aENode1.nCFMEIndex;
				if(aENode1.nConstraintIndex>=0)
				{
					a[i][nCFMEI]=-1.;
				}
				else
				{
					b[i]+=aENode1.dValue;
				}					
				
				nEIndex1=aCNode0.vnInvolvedFMEs[1];
				aENode1=m_vsFMElements[nEIndex1];
				nCFMEI=aENode1.nCFMEIndex;
				if(aENode1.nConstraintIndex>=0)
				{
					a[i][nCFMEI]=1.;
				}
				else
				{
					b[i]-=aENode1.dValue;
				}					
				break;
					
			case 1:
				nEIndex1=aCNode0.vnInvolvedFMEs[0];
				aENode1=m_vsFMElements[nEIndex1];
				nCFMEI=aENode1.nCFMEIndex;
				if(aENode1.nConstraintIndex>=0)
				{
					a[i][nCFMEI]=1.;//i==nCFMEI if the code is correct
				}
				else
				{
					b[i]-=aENode1.dValue;  //This case should not happen, but leave this here for a moment.
				}	
				
				nEIndex1=aCNode0.vnInvolvedFMEs[1];
				aENode1=m_vsFMElements[nEIndex1];
				nCFMEI=aENode1.nCFMEIndex;
				if(aENode1.nConstraintIndex>=0)
				{
					a[i][nCFMEI]=-1.;
				}
				else
				{
					b[i]+=aENode1.dValue;
				}					
				
				nEIndex1=aCNode0.vnInvolvedFMEs[2];
				aENode1=m_vsFMElements[nEIndex1];
				nCFMEI=aENode1.nCFMEIndex;
				if(aENode1.nConstraintIndex>=0)
				{
					a[i][nCFMEI]=-1.;
				}
				else
				{
					b[i]+=aENode1.dValue;
				}					
				
				nEIndex1=aCNode0.vnInvolvedFMEs[1];
				aENode1=m_vsFMElements[nEIndex1];
				nCFMEI=aENode1.nCFMEIndex;
				if(aENode1.nConstraintIndex>=0)
				{
					a[i][nCFMEI]=1.;
				}
				else
				{
					b[i]-=aENode1.dValue;
				}					
				break;

			case 2:
				nEIndex1=aCNode0.vnInvolvedFMEs[0];
				aENode1=m_vsFMElements[nEIndex1];
				nCFMEI=aENode1.nCFMEIndex;
				if(aENode1.nConstraintIndex>=0)
				{
					a[i][nCFMEI]=1.;//i==nCFMEI if the code is correct
				}
				else
				{
					b[i]-=aENode1.dValue;  //This case should not happen, but leave this here for a moment.
				}	
				
				nEIndex1=aCNode0.vnInvolvedFMEs[1];
				aENode1=m_vsFMElements[nEIndex1];
				nCFMEI=aENode1.nCFMEIndex;
				if(aENode1.nConstraintIndex>=0)
				{
					a[i][nCFMEI]=-1.;
				}
				else
				{
					b[i]+=aENode1.dValue;
				}					
				
				nEIndex1=aCNode0.vnInvolvedFMEs[2];
				aENode1=m_vsFMElements[nEIndex1];
				nCFMEI=aENode1.nCFMEIndex;
				if(aENode1.nConstraintIndex>=0)
				{
					a[i][nCFMEI]=-1.;
				}
				else
				{
					b[i]+=aENode1.dValue;
				}					
				
				nEIndex1=aCNode0.vnInvolvedFMEs[1];
				aENode1=m_vsFMElements[nEIndex1];
				nCFMEI=aENode1.nCFMEIndex;
				if(aENode1.nConstraintIndex>=0)
				{
					a[i][nCFMEI]=1.;
				}
				else
				{
					b[i]-=aENode1.dValue;
				}					
				break;

			case 3:
				nEIndex1=aCNode0.vnInvolvedFMEs[0];
				aENode1=m_vsFMElements[nEIndex1];
				nCFMEI=aENode1.nCFMEIndex;
				if(aENode1.nConstraintIndex>=0)
				{
					a[i][nCFMEI]=1.;//i==nCFMEI if the code is correct
				}
				else
				{
					b[i]-=aENode1.dValue;  //This case should not happen, but leave this here for a moment.
				}	
				
				nEIndex1=aCNode0.vnInvolvedFMEs[1];
				aENode1=m_vsFMElements[nEIndex1];
				nCFMEI=aENode1.nCFMEIndex;
				if(aENode1.nConstraintIndex>=0)
				{
					a[i][nCFMEI]=-1.;
				}
				else
				{
					b[i]+=aENode1.dValue;
				}					
				
				nEIndex1=aCNode0.vnInvolvedFMEs[2];
				aENode1=m_vsFMElements[nEIndex1];
				nCFMEI=aENode1.nCFMEIndex;
				if(aENode1.nConstraintIndex>=0)
				{
					a[i][nCFMEI]=-1.;
				}
				else
				{
					b[i]+=aENode1.dValue;
				}					
				
				nEIndex1=aCNode0.vnInvolvedFMEs[1];
				aENode1=m_vsFMElements[nEIndex1];
				nCFMEI=aENode1.nCFMEIndex;
				if(aENode1.nConstraintIndex>=0)
				{
					a[i][nCFMEI]=1.;
				}
				else
				{
					b[i]-=aENode1.dValue;
				}					
				break;

			default:
				nEIndex1=aCNode0.vnInvolvedFMEs[0];
				aENode1=m_vsFMElements[nEIndex1];
				nCFMEI=aENode1.nCFMEIndex;
				if(aENode1.nConstraintIndex>=0)
				{
					a[i][nCFMEI]=1.;//i==nCFMEI if the code is correct
				}
				else
				{
					b[i]-=aENode1.dValue;  //This case should not happen, but leave this here for a moment.
				}	
				
				nEIndex1=aCNode0.vnInvolvedFMEs[1];
				aENode1=m_vsFMElements[nEIndex1];
				nCFMEI=aENode1.nCFMEIndex;
				if(aENode1.nConstraintIndex>=0)
				{
					a[i][nCFMEI]=-1.;
				}
				else
				{
					b[i]+=aENode1.dValue;
				}					
				
				nEIndex1=aCNode0.vnInvolvedFMEs[2];
				aENode1=m_vsFMElements[nEIndex1];
				nCFMEI=aENode1.nCFMEIndex;
				if(aENode1.nConstraintIndex>=0)
				{
					a[i][nCFMEI]=-1.;
				}
				else
				{
					b[i]+=aENode1.dValue;
				}					
				
				nEIndex1=aCNode0.vnInvolvedFMEs[1];
				aENode1=m_vsFMElements[nEIndex1];
				nCFMEI=aENode1.nCFMEIndex;
				if(aENode1.nConstraintIndex>=0)
				{
					a[i][nCFMEI]=1.;
				}
				else
				{
					b[i]-=aENode1.dValue;
				}					
				break;
			}
		}
	}
	
	u=a;
	svdcmp(u,w,v);
	svbksb(u,w,v,b,x);
	for(i=0;i<nSize;i++)
	{
		nEIndex=m_vnCFMElements[i];
		m_vsFMElements[nEIndex].dValue=x[i];
	}
}


using namespace std;

//void CQMatrixFitting::svdcmp(Mat_IO_DP &a, Vec_O_DP &w, Mat_O_DP &v)
void CQMatrixFitting::svdcmp(vector <dVect> &a, dVect &w, vector <dVect> &v)
{

	int i,its,j,jj,k,l,nm;
	double anorm,c,f,g,h,s,scale,x,y,z;
	bool flag;

	int m=a.size();
	int n=a[0].size();
//	Vec_DP rv1(n);
	dVect rv1;
	for(i=0;i<n;i++)
	{
		rv1.push_back(0.);
	}
	g=scale=anorm=0.0;
	for (i=0;i<n;i++) {
		l=i+2;
		rv1[i]=scale*g;
		g=s=scale=0.0;
		if (i < m) {
			for (k=i;k<m;k++) scale += fabs(a[k][i]);
			if (scale != 0.0) {
				for (k=i;k<m;k++) {
					a[k][i] /= scale;
					s += a[k][i]*a[k][i];
				}
				f=a[i][i];
				g = -SIGN(sqrt(s),f);
				h=f*g-s;
				a[i][i]=f-g;
				for (j=l-1;j<n;j++) {
					for (s=0.0,k=i;k<m;k++) s += a[k][i]*a[k][j];
					f=s/h;
					for (k=i;k<m;k++) a[k][j] += f*a[k][i];
				}
				for (k=i;k<m;k++) a[k][i] *= scale;
			}
		}
		w[i]=scale *g;
		g=s=scale=0.0;
		if (i+1 <= m && i+1 != n) {
			for (k=l-1;k<n;k++) scale += fabs(a[i][k]);
			if (scale != 0.0) {
				for (k=l-1;k<n;k++) {
					a[i][k] /= scale;
					s += a[i][k]*a[i][k];
				}
				f=a[i][l-1];
				g = -SIGN(sqrt(s),f);
				h=f*g-s;
				a[i][l-1]=f-g;
				for (k=l-1;k<n;k++) rv1[k]=a[i][k]/h;
				for (j=l-1;j<m;j++) {
					for (s=0.0,k=l-1;k<n;k++) s += a[j][k]*a[i][k];
					for (k=l-1;k<n;k++) a[j][k] += s*rv1[k];
				}
				for (k=l-1;k<n;k++) a[i][k] *= scale;
			}
		}
		anorm=MAX(anorm,(fabs(w[i])+fabs(rv1[i])));
	}
	for (i=n-1;i>=0;i--) {
		if (i < n-1) {
			if (g != 0.0) {
				for (j=l;j<n;j++)
					v[j][i]=(a[i][j]/a[i][l])/g;
				for (j=l;j<n;j++) {
					for (s=0.0,k=l;k<n;k++) s += a[i][k]*v[k][j];
					for (k=l;k<n;k++) v[k][j] += s*v[k][i];
				}
			}
			for (j=l;j<n;j++) v[i][j]=v[j][i]=0.0;
		}
		v[i][i]=1.0;
		g=rv1[i];
		l=i;
	}
	for (i=MIN(m,n)-1;i>=0;i--) {
		l=i+1;
		g=w[i];
		for (j=l;j<n;j++) a[i][j]=0.0;
		if (g != 0.0) {
			g=1.0/g;
			for (j=l;j<n;j++) {
				for (s=0.0,k=l;k<m;k++) s += a[k][i]*a[k][j];
				f=(s/a[i][i])*g;
				for (k=i;k<m;k++) a[k][j] += f*a[k][i];
			}
			for (j=i;j<m;j++) a[j][i] *= g;
		} else for (j=i;j<m;j++) a[j][i]=0.0;
		++a[i][i];
	}
	for (k=n-1;k>=0;k--) {
		for (its=0;its<30;its++) {
			flag=true;
			for (l=k;l>=0;l--) {
				nm=l-1;
				if (fabs(rv1[l])+anorm == anorm) {
					flag=false;
					break;
				}
				if (fabs(w[nm])+anorm == anorm) break;
			}
			if (flag) {
				c=0.0;
				s=1.0;
				for (i=l;i<k+1;i++) {
					f=s*rv1[i];
					rv1[i]=c*rv1[i];
					if (fabs(f)+anorm == anorm) break;
					g=w[i];
					h=pythag(f,g);
					w[i]=h;
					h=1.0/h;
					c=g*h;
					s = -f*h;
					for (j=0;j<m;j++) {
						y=a[j][nm];
						z=a[j][i];
						a[j][nm]=y*c+z*s;
						a[j][i]=z*c-y*s;
					}
				}
			}
			z=w[k];
			if (l == k) {
				if (z < 0.0) {
					w[k] = -z;
					for (j=0;j<n;j++) v[j][k] = -v[j][k];
				}
				break;
			}
//			if (its == 29) nrerror("no convergence in 30 svdcmp iterations"); //need to change the error message output later
			x=w[l];
			nm=k-1;
			y=w[nm];
			g=rv1[nm];
			h=rv1[k];
			f=((y-z)*(y+z)+(g-h)*(g+h))/(2.0*h*y);
			g=pythag(f,1.0);
			f=((x-z)*(x+z)+h*((y/(f+SIGN(g,f)))-h))/x;
			c=s=1.0;
			for (j=l;j<=nm;j++) {
				i=j+1;
				g=rv1[i];
				y=w[i];
				h=s*g;
				g=c*g;
				z=pythag(f,h);
				rv1[j]=z;
				c=f/z;
				s=h/z;
				f=x*c+g*s;
				g=g*c-x*s;
				h=y*s;
				y *= c;
				for (jj=0;jj<n;jj++) {
					x=v[jj][j];
					z=v[jj][i];
					v[jj][j]=x*c+z*s;
					v[jj][i]=z*c-x*s;
				}
				z=pythag(f,h);
				w[j]=z;
				if (z) {
					z=1.0/z;
					c=f*z;
					s=h*z;
				}
				f=c*g+s*y;
				x=c*y-s*g;
				for (jj=0;jj<m;jj++) {
					y=a[jj][j];
					z=a[jj][i];
					a[jj][j]=y*c+z*s;
					a[jj][i]=z*c-y*s;
				}
			}
			rv1[l]=0.0;
			rv1[k]=f;
			w[k]=x;
		}
	}
}


//void NR::svbksb(Mat_I_DP &u, Vec_I_DP &w, Mat_I_DP &v, Vec_I_DP &b, Vec_O_DP &x)
void CQMatrixFitting::svbksb(vector <dVect> &u, dVect &w, vector <dVect> &v, dVect &b, dVect &x)
{
	int jj,j,i;
	double s;

	int m=u.size();
	int n=u[0].size();
//	Vec_DP tmp(n);
	dVect tmp;
	for(i=0;i<n;i++)
	{
		tmp.push_back(0.);
	}

	for (j=0;j<n;j++) {
		s=0.0;
		if (w[j] != 0.0) {
			for (i=0;i<m;i++) s += u[i][j]*b[i];
			s /= w[j];
		}
		tmp[j]=s;
	}
	for (j=0;j<n;j++) {
		s=0.0;
		for (jj=0;jj<n;jj++) s += v[j][jj]*tmp[jj];
		x[j]=s;
	}
}


DP CQMatrixFitting::pythag(const DP a, const DP b)
{
	DP absa,absb;

	absa=fabs(a);
	absb=fabs(b);
	if (absa > absb) return absa*sqrt(1.0+SQR(absb/absa));
	else return (absb == 0.0 ? 0.0 : absb*sqrt(1.0+SQR(absa/absb)));
}


void CQMatrixFitting::UpdateFMatrix()
{
	int nSize=m_vnFreeFMElements.size();
	int i,i0,j0,nIndex;
	for(i=0;i<nSize;i++)
	{
		nIndex=m_vnFreeFMElements[i];
		i0=m_vsFMElements[nIndex].i0;
		j0=m_vsFMElements[nIndex].j0;
		m_dFMatrix[i0][j0]=m_vsFMElements[nIndex].dValue;
		m_dFMatrix[j0][i0]=m_vsFMElements[nIndex].dValue;
	}

	nSize=m_vnCFMElements.size();
	for(i=0;i<nSize;i++)
	{
		nIndex=m_vnCFMElements[i];
		i0=m_vsFMElements[nIndex].i0;
		j0=m_vsFMElements[nIndex].j0;
		m_dFMatrix[i0][j0]=m_vsFMElements[nIndex].dValue;
		m_dFMatrix[j0][i0]=m_vsFMElements[nIndex].dValue;
	}
}

void CQMatrixFitting::GetQMatrix(double dQM[200][200], int &nNumLevels, int &nNumStates, vector <int> &vnStates)
{
	ReadInput();
	InitializeFMEList();
	InitializeConstraints();
	ApplyConstraints();
	GenerateFreeFMEList();
	CalCFMElements();
	UpdateFMatrix();
	UpdateQMatrix();
	nNumLevels=m_nNumLevels;
	vnStates=m_vnStates;
	nNumStates=m_nNumStates;
	for(int i=0;i<nNumStates;i++)
	{
		for(int j=0;j<nNumStates;j++)
		{
			dQM[i][j]=m_dQMatrix[i][j];
		}
	}
}

structDataSimulationNode CQMatrixFitting::GetDataSimulationParameters()
{
	ReadInput();
	InitializeFMEList();
	InitializeConstraints();
	ApplyConstraints();
	GenerateFreeFMEList();
	CalCFMElements();
	UpdateFMatrix();
	UpdateQMatrix();
	m_sSimulationParameters.vvdQMatrix.clear();
	for(int i=0;i< m_nNumStates;i++)
	{
		vector <int> anV;
		for(int j=0;j<m_nNumStates;j++)
		{
			anV.push_back(m_dQMatrix[i][j]);
		}
	}
	return m_sSimulationParameters;
}

void CQMatrixFitting::StateIndexConversion()
{
	int i,j;
	m_vnStateIndexO.clear();
	m_vnStateIndexI.clear();
	for(i=0;i<m_nNumStates;i++)
	{
		m_vnStateIndexO.push_back(i);	
		m_vnStateIndexI.push_back(i);	
	}

	int nSize,nTemp,nIndex;

	nIndex=0;
	for(i=0;i<m_nNumLevels;i++)
	{
		nSize=m_vvnStatesInLevels[i].size();
		for(j=0;j<nSize;j++)
		{
			nTemp=m_vvnStatesInLevels[i].at(j);
			m_vnStateIndexO[nIndex]=nTemp;	
			m_vnStateIndexI[nTemp-1]=nIndex;	
			nIndex++;
		}
	}
}
