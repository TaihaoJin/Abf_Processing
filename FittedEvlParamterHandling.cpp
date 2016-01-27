// FittedEvlParamterHandling.cpp: implementation of the CFittedEvlParamterHandling class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "Abf_Processing.h"
#include "FittedEvlParamterHandling.h"
#include "nr.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////


CFittedEvlParamterHandling::CFittedEvlParamterHandling()
{
}

CFittedEvlParamterHandling::CFittedEvlParamterHandling(vector <structFittingResultsCollectionNode> avCollection, structFittedParameterNode aFittedNode)
{
	m_bFreezeDev=true;
	m_avCollection=avCollection;
	m_nLevel=aFittedNode.nFittedLevel;
	SelectParameterNodes();
	CalMeanTimeConstants();
	TimeConstDistribution0();
	AssignComponents();
	ResolvingDegeneracy();
	RearrangeTW();
	OutputFittedDTParameters();
	int nMaxIteractions=6;
	int nIterations=0;

	while(Degenerated()&&nIterations<nMaxIteractions)
	{
		nIterations++;
		RefineFitting();
		AssignComponents();
		ResolvingDegeneracy();
		RearrangeTW();
		OutputFittedDTParameters(m_sFileName);
	}
}

CFittedEvlParamterHandling::~CFittedEvlParamterHandling()
{

}

void CFittedEvlParamterHandling::SelectParameterNodes()
{
	int nFiles=m_avCollection.size(),nLevels,nComponents,nTrials;
	m_avNodes.clear();
	int nLevel,nNumComponents,nSignificantComponents;
	double dA;
	m_avSignatures.clear();
	int nFileIndex,nLevelIndex,nComponentIndex,nTrialIndex,i,j,k;
	m_nMaxComponents=0;
	m_vdData.clear();
	for(nFileIndex=0;nFileIndex<nFiles;nFileIndex++)
	{
		nLevels=m_avCollection[nFileIndex].nNumFittingLevels;
		for(nLevelIndex=0;nLevelIndex<nLevels;nLevelIndex++)
		{
			nLevel=m_avCollection[nFileIndex].aTotalVVResults.at(nLevelIndex).at(0).at(0).nDwellLevel;
			if(nLevel==m_nLevel)
			{
				nComponents=m_avCollection[nFileIndex].aTotalVVResults[nLevelIndex].size();
				for(nComponentIndex=0;nComponentIndex<nComponents;nComponentIndex++)
				{
					nNumComponents=m_avCollection[nFileIndex].aTotalVVResults[nLevelIndex].at(nComponentIndex).at(0).nNumComponents;
					nSignificantComponents=m_avCollection[nFileIndex].aTotalVVResults[nLevelIndex].at(nComponentIndex).at(0).nSignificantComponents;
					if(nSignificantComponents<1)
						nSignificantComponents=1;
					if(nNumComponents==nSignificantComponents)
					{
						if(nNumComponents>m_nMaxComponents)
						{
							m_nMaxComponents=nNumComponents;
						}

						nTrials=m_avCollection[nFileIndex].aTotalVVResults[nLevelIndex].at(nComponentIndex).size();
						for(nTrialIndex=0;nTrialIndex<nTrials;nTrialIndex++)
						{
							dA=m_avCollection[nFileIndex].aTotalVVResults[nLevelIndex].at(nComponentIndex).at(nTrialIndex).dFittedArea;
							if(dA>0.05) 
							{
								structDTNodeSignature aSignatureNode;
								aSignatureNode.nComponentIndex=nComponentIndex;
								aSignatureNode.nFileIndex=nFileIndex;
								aSignatureNode.nLevelIndex=nLevelIndex;
								aSignatureNode.nTrialIndex=nTrialIndex;
								m_avSignatures.push_back(aSignatureNode);
								structFittingResultNode aNode=m_avCollection[nFileIndex].aTotalVVResults[nLevelIndex].at(nComponentIndex).at(nTrialIndex);
								m_avNodes.push_back(aNode);
								for(i=0;i<aNode.nNumComponents;i++)
								{
									m_vdData.push_back(aNode.dTimeConstants[i]);
								}
								break;
							}
						}
					}
				}
			}
		}
	}
}

vector <structDTNodeSignature> CFittedEvlParamterHandling::SelectParameterNodes(vector <structFittingResultsCollectionNode> m_avCollection, int m_nLevel)
{
	int nFiles=m_avCollection.size(),nLevels,nComponents,nTrials;
	m_avNodes.clear();
	int nLevel,nNumComponents,nSignificantComponents;
	double dA;
	m_avSignatures.clear();
	int nFileIndex,nLevelIndex,nComponentIndex,nTrialIndex,i,j,k;
	m_nMaxComponents=0;
	m_vdData.clear();
	for(nFileIndex=0;nFileIndex<nFiles;nFileIndex++)
	{
		nLevels=m_avCollection[nFileIndex].nNumFittingLevels;
		for(nLevelIndex=0;nLevelIndex<nLevels;nLevelIndex++)
		{
			nLevel=m_avCollection[nFileIndex].aTotalVVResults.at(nLevelIndex).at(0).at(0).nDwellLevel;
			if(nLevel==m_nLevel)
			{
				nComponents=m_avCollection[nFileIndex].aTotalVVResults[nLevelIndex].size();
				for(nComponentIndex=0;nComponentIndex<nComponents;nComponentIndex++)
				{
					nNumComponents=m_avCollection[nFileIndex].aTotalVVResults[nLevelIndex].at(nComponentIndex).at(0).nNumComponents;
					nSignificantComponents=m_avCollection[nFileIndex].aTotalVVResults[nLevelIndex].at(nComponentIndex).at(0).nSignificantComponents;
					if(nSignificantComponents<1)
						nSignificantComponents=1;
					if(nNumComponents==nSignificantComponents)
					{
						if(nNumComponents>m_nMaxComponents)
						{
							m_nMaxComponents=nNumComponents;
						}

						nTrials=m_avCollection[nFileIndex].aTotalVVResults[nLevelIndex].at(nComponentIndex).size();
						for(nTrialIndex=0;nTrialIndex<nTrials;nTrialIndex++)
						{
							dA=m_avCollection[nFileIndex].aTotalVVResults[nLevelIndex].at(nComponentIndex).at(nTrialIndex).dFittedArea;
							if(dA>0.05) 
							{
								structDTNodeSignature aSignatureNode;
								aSignatureNode.nComponentIndex=nComponentIndex;
								aSignatureNode.nFileIndex=nFileIndex;
								aSignatureNode.nLevelIndex=nLevelIndex;
								aSignatureNode.nTrialIndex=nTrialIndex;
								m_avSignatures.push_back(aSignatureNode);
								structFittingResultNode aNode=m_avCollection[nFileIndex].aTotalVVResults[nLevelIndex].at(nComponentIndex).at(nTrialIndex);
								m_avNodes.push_back(aNode);
								for(i=0;i<aNode.nNumComponents;i++)
								{
									m_vdData.push_back(aNode.dTimeConstants[i]);
								}
								break;
							}
						}
					}
				}
			}
		}
	}
	return m_avSignatures;
}


void CFittedEvlParamterHandling::TimeConstDistribution0()
{
	m_structTrialNode.nNumComponents=m_nMaxComponents;

	if(m_bFreezeDev)
	{
		m_structTrialNode.nFreeParameters=2*m_nMaxComponents-1;
	}
	else
	{
		m_structTrialNode.nFreeParameters=3*m_nMaxComponents-1;
	}
	m_structTrialNode.bFreezeDev=m_bFreezeDev;

	int i,j;
	for(i=0;i<m_nMaxComponents;i++)
	{ 
		m_structTrialNode.dMean[i]=m_dMeanTimeConstants[i];
		m_structTrialNode.dWeights[i]=1./double(m_nMaxComponents);
		if(i==0)
		{
			m_structTrialNode.dDev[i]=(m_dMeanTimeConstants[i+1]-m_dMeanTimeConstants[i])/4.;
		}
		else
		{
			m_structTrialNode.dDev[i]=(m_dMeanTimeConstants[i]-m_dMeanTimeConstants[i-1])/4.;
		}
	}

	int nSize=m_vdData.size();
	m_structTrialNode.nDataSize=nSize;
	m_structTrialNode.pdData=new double[nSize];
	for(i=0;i<nSize;i++)
	{
		m_structTrialNode.pdData[i]=m_vdData[i];
	}
	
//	CGaussianFitting aFitting(m_structTrialNode);
//	m_structFittedNode=aFitting.GetFittedNode();
	m_structFittedNode=m_structTrialNode;
}

void CFittedEvlParamterHandling::TimeConstDistribution()
{
	m_structTrialNode.nNumComponents=m_nMaxComponents;

	if(m_bFreezeDev)
	{
		m_structTrialNode.nFreeParameters=2*m_nMaxComponents-1;
	}
	else
	{
		m_structTrialNode.nFreeParameters=3*m_nMaxComponents-1;
	}
	m_structTrialNode.bFreezeDev=m_bFreezeDev;

	int i,j;
	for(i=0;i<m_nMaxComponents;i++)
	{ 
		m_structTrialNode.dMean[i]=m_dMeanTimeConstants[i];
		m_structTrialNode.dWeights[i]=1./double(m_nMaxComponents);
		if(i==0)
		{
			m_structTrialNode.dDev[i]=(m_dMeanTimeConstants[i+1]-m_dMeanTimeConstants[i])/4.;
		}
		else
		{
			m_structTrialNode.dDev[i]=(m_dMeanTimeConstants[i]-m_dMeanTimeConstants[i-1])/4.;
		}
	}

	int nSize=m_vdData.size();
	m_structTrialNode.nDataSize=nSize;
	m_structTrialNode.pdData=new double[nSize];
	for(i=0;i<nSize;i++)
	{
		m_structTrialNode.pdData[i]=m_vdData[i];
	}
	
	CGaussianFitting aFitting(m_structTrialNode);
	m_structFittedNode=aFitting.GetFittedNode();
}

void CFittedEvlParamterHandling::CalMeanTimeConstants()
{
	int i,j;
	for(i=0;i<20;i++)
	{
		m_dMeanTimeConstants[i]=0.;
	}
	
	int nSize=m_avNodes.size();
	int nNum=0;
	for(i=0;i<nSize;i++)
	{
		if(m_avNodes[i].nNumComponents==m_nMaxComponents)
		{
			nNum++;
			for(j=0;j<m_nMaxComponents;j++)
			{
				m_dMeanTimeConstants[j]+=m_avNodes[i].dTimeConstants[j];
			}
		}
	}

	for(j=0;j<m_nMaxComponents;j++)
	{
		m_dMeanTimeConstants[j]/=double(nNum);
	}
}


void CFittedEvlParamterHandling::AssignComponents()
{
	int i,j,k,k0,nSize,nNumComponents,nNumComponents0;
	m_vivComponents.clear();
	double dTimeConstant, dProb0, dProb, dMean;
	nSize=m_avNodes.size();
	nNumComponents0=m_structFittedNode.nNumComponents;
	m_vDegeneracy.clear();
	for(i=0;i<nSize;i++)
	{
		vector <int> aVect;
		nNumComponents=m_avNodes[i].nNumComponents;
		k0=-1;
		for(j=0;j<nNumComponents;j++)
		{
			dTimeConstant=m_avNodes.at(i).dTimeConstants[j];
			for(k=0;k<nNumComponents0;k++)
			{
				dMean=m_structFittedNode.dMean[k];
				if(dMean>dTimeConstant)
				{
					if(k==k0)
					{
//						aVect.push_back(k);
//						k0=k;
					}
					else
					{
						dProb0=GaussianProb(dTimeConstant,k-1);
						dProb=GaussianProb(dTimeConstant,k);
						if(dProb0>dProb)
						{
							aVect.push_back(k-1);
							if((k-1)==k0)
							{
								structDegeneracyNode aNode;
								aNode.nComponentindex=j-1;
								aNode.nFileIndex=i;
								aNode.nGausComponent=k-1;
								m_vDegeneracy.push_back(aNode);
							}								
							k0=k-1;
						}
						else
						{
							aVect.push_back(k);
							if(k==k0)
							{
								structDegeneracyNode aNode;
								aNode.nComponentindex=j-1;
								aNode.nFileIndex=i;
								aNode.nGausComponent=k;
								m_vDegeneracy.push_back(aNode);
							}
							k0=k;
						}
						break;
					}
				}
				else if(k==nNumComponents0-1)
				{
					aVect.push_back(k);
					if(k==k0)
					{
						structDegeneracyNode aNode;
						aNode.nComponentindex=j-1;
						aNode.nFileIndex=i;
						aNode.nGausComponent=k;
						m_vDegeneracy.push_back(aNode);
					}
					k0=k;
					break;
				}
			}
		}
		m_vivComponents.push_back(aVect);
	}
	FILE* fp=fopen("Degeneracy.txt","a");
	nSize=m_vDegeneracy.size();
	for(i=0;i<nSize;i++)
	{
		fprintf(fp,"%3d    %3d    %3d    %3d\n",i,m_vDegeneracy[i].nGausComponent,m_vDegeneracy[i].nFileIndex,m_vDegeneracy[i].nComponentindex);
	}
	fprintf(fp,"\n\n\n\n");
	fclose(fp);
}


void CFittedEvlParamterHandling::ResolvingDegeneracy()
{
	int i,j,k,nSize,nNumComponents,nNumComponents0;
	double dTimeConstant, dProb;
	nSize=m_vDegeneracy.size();
	nNumComponents0=m_structFittedNode.nNumComponents;
	const int n20=20;
	int nConfig[20];
	int nBestConfig[20];

	int nNumVacancy;
	int nFileIndex;
	int nIndex;
	double dLik,dMaxLik;
	vector <IntVect> nvvConfigurations;
	int nPosition;
	const double dTiny=4.9406564584125e-324;

	for(i=0;i<nSize;i++)
	{
		nFileIndex=m_vDegeneracy[i].nFileIndex;
		nNumComponents=m_avNodes[nFileIndex].nNumComponents;
		nNumVacancy=nNumComponents0-nNumComponents;
		nvvConfigurations.clear();
		nvvConfigurations=Configurations(nNumComponents0,nNumVacancy);
		int nNumConfigs=nvvConfigurations.size();
		for(j=0;j<nNumConfigs;j++)
		{
			for(k=0;k<nNumComponents0;k++)
			{
				nConfig[k]=1;
			}
			for(k=0;k<nNumVacancy;k++)
			{
				nIndex=nvvConfigurations.at(j)[k];
				nConfig[nIndex]=-1;
			}
			dLik=0.;
			for(k=0;k<nNumComponents0;k++)
			{
				if(nConfig[k]>0)
				{
					dTimeConstant=m_avNodes.at(nFileIndex).dTimeConstants[k];
					dProb=GaussianProb(dTimeConstant,k);
					if(dProb<dTiny) dProb=dTiny;
					dLik+=log(dProb);
				}
			}
			if(j==0)
			{
				dMaxLik=dLik;
				for(k=0;k<nNumComponents0;k++)
				{
					nBestConfig[k]=nConfig[k];
				}
			}
			else
			{
				if(dMaxLik<dLik)
				{
					for(k=0;k<nNumComponents0;k++)
					{
						nBestConfig[k]=nConfig[k];
					}
					dMaxLik=dLik;
				}
			}
		}
		nPosition=0;
		for(j=0;j<nNumComponents0;j++)
		{
			if(nBestConfig[j]==1)
			{
				m_vivComponents.at(nFileIndex)[nPosition]=j;
				nPosition++;
			}
		}
	}
}

typedef vector <int> IntVect;
vector <IntVect> CFittedEvlParamterHandling::Configurations(int nNumPositions, int nParticles)
{
	const int n20=20;
	int nL_Limit[n20];
	int nU_Limit[n20];
	int i,j,k;
	vector <IntVect> nvvConfigurations;
	int nPositions[n20];
	for(i=0;i<nParticles;i++)
	{
		nL_Limit[i]=i;
		nU_Limit[i]=nNumPositions-nParticles+i;
		nPositions[i]=i;
	}
	int nIndex=nParticles-1;
	vector <int> nvConfig;
	for(i=0;i<nParticles;i++)
	{
		nvConfig.push_back(nPositions[i]);
	}
	nvvConfigurations.push_back(nvConfig);

	while(nIndex>=0)
	{
		nIndex=nParticles-1;
		while(nPositions[nIndex]>=nU_Limit[nIndex])
			nIndex--;
		if(nIndex>=0)
		{
			nPositions[nIndex]++;
			for(i=nIndex+1;i<nParticles;i++)
			{
				nPositions[i]=nPositions[i-1]+1;
			}
			vector <int> nvConfig;
			for(i=0;i<nParticles;i++)
			{
				nvConfig.push_back(nPositions[i]);
			}
			nvvConfigurations.push_back(nvConfig);
		}
	}

//	if(nParticles==0)
//	{
//		vector <int> nvConfig;
//		nvvConfigurations.push_back(nvConfig);
//	}
//	else if(nParticles==nNumPositions)
//	{
//		vector <int> nvConfig;
//		for(i=0;i<nParticles;i++)
//		{
//			nvConfig.push_back(nU_Limit[i]);
//		}
//		nvvConfigurations.push_back(nvConfig);
//	}

	return nvvConfigurations;
}

double CFittedEvlParamterHandling::GaussianProb(double dX, int nIndex)
{
	if(nIndex<0||nIndex>=m_structFittedNode.nNumComponents)
	{
		return -1.;
	}
	else
	{
		const double dPi=3.1415926535897932384626433832795;
		double dMean, dDev, dProb, dExp, dDenorm;
		dMean=m_structFittedNode.dMean[nIndex];
		dDev=m_structFittedNode.dDev[nIndex];
		dExp=(dX-dMean)/dDev;
		dExp=-dExp*dExp/2.;
		dDenorm=2.*sqrt(2.*dPi)*dDev;
		dProb=exp(dExp)/dDenorm;
		return dProb;
	}
}

void CFittedEvlParamterHandling::OutputFittedDTParameters()
{
	CString sFileName;
	vector <CString> sEvlFileNames;
	CFileDialog aFileDlg( false, NULL, NULL, OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, NULL, NULL );
	aFileDlg.m_ofn.lpstrTitle="Output Fitted Parameters";
	char* pch="Fitted Parameter Files(*.twn)\0*.twn\0\0";
	aFileDlg.m_ofn.lpstrFilter=pch;
	if(aFileDlg.DoModal()==IDOK)
	{
		sFileName=aFileDlg.GetPathName();
		m_sFileName=sFileName;
		OutputFittedDTParameters(sFileName);
	}
}


void CFittedEvlParamterHandling::OutputFittedDTParameters(CString sFileName)
{
//	CString sFileName;
	vector <CString> sEvlFileNames;
	typedef vector <float> FVector;
	typedef vector <double> DVector;
	int i,j;
	int nComponents=m_structFittedNode.nNumComponents;
	int nLevel=m_avNodes.at(0).nDwellLevel;

	vector <FVector> fvectTimeConstants;
	vector <FVector> fvectWeights;
	vector <DVector> fvectBurstDelimiters;
	vector <int> nvScoreReanking;
	for(i=0;i<nComponents;i++)
	{
		vector <float> aV;
		vector <float> bV;
		vector <double> cV;
		fvectTimeConstants.push_back(aV);
		fvectWeights.push_back(bV);
		fvectBurstDelimiters.push_back(cV);
	}

	vector <double> dMTO;
	vector <double> dMTC;
	vector <double> dPo;
	vector <double> dLength;
	vector <double> dProbSingle;
	vector <double> dLogLik_R;
	vector <int> vnSig;
	vector <int> vnNumEvents;
	vector <double> dvArea;
	vector <structFRange> structvFRange;
	double dTemp;

	int nSize=m_avNodes.size();
	structFittingResultNode aNode;
	double dT,dW;
	for(i=0;i<nSize;i++)
	{
		aNode=m_avNodes_Rearranged.at(i);
		for(j=0;j<nComponents;j++)
		{
			dT=aNode.dTimeConstants[j];
			dW=aNode.dWeights[j];
			if(dT>0)
			{
				fvectTimeConstants.at(j).push_back(dT);
				fvectWeights.at(j).push_back(dW);
			}
		}

		aNode=m_avNodes[i];
		dMTO.push_back(aNode.dMeanDwellTimeO);
		dMTC.push_back(aNode.dMeanDwellTimeC);
		dLength.push_back(aNode.fRecordLength);
		dProbSingle.push_back(aNode.fProbSingle);
		dPo.push_back(aNode.fOpenProb);
		dLogLik_R.push_back(aNode.dLogLikelihood_R);
		sEvlFileNames.push_back(aNode.sCurFileName);
		nvScoreReanking.push_back(aNode.nScoreRanking);
		vnSig.push_back(aNode.nSignificantComponents);
		vnNumEvents.push_back(aNode.nEventsInRange);
		dvArea.push_back(aNode.dFittedArea);
		structvFRange.push_back(aNode.fFittingRange);
	}


	CString sMessage="Can not open the output file. Please check if this file is being used by other applications.";
	CFileDialog aFileDlg( false, NULL, NULL, OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, NULL, NULL );
	aFileDlg.m_ofn.lpstrTitle="Output Fitted Parameters";
	char* pch="Fitted Parameter Files(*.twn)\0*.twn\0\0";
	float fMean;
	double dFrequency[20];
	aFileDlg.m_ofn.lpstrFilter=pch;
//	if(aFileDlg.DoModal()==IDOK)
//	{
		FILE* fpOutput;
//		sFileName=aFileDlg.GetPathName();
		if(!(fpOutput=fopen(sFileName,"a")))
		{
			AfxMessageBox(sMessage,MB_OK,0);
		}
		else
		{
			nSize=m_vDegeneracy.size();
			for(i=0;i<nSize;i++)
			{
				fprintf(fpOutput,"%3d    %3d    %3d    %3d\n",i,m_vDegeneracy[i].nGausComponent,m_vDegeneracy[i].nFileIndex,m_vDegeneracy[i].nComponentindex);
			}
			fprintf(fpOutput,"\n\n\n\n");

			fprintf(fpOutput,"    i     Score     Level");
			for(i=0;i<nComponents;i++)
			{
				fprintf(fpOutput,"   TimeConstant%1d",i);
			}
			for(i=0;i<nComponents;i++)
			{
				fprintf(fpOutput,"     Weight%1d",i);
			}


			for(i=0;i<nComponents;i++)
			{
				fprintf(fpOutput,"       Frequency%1d",i);
			}

			fprintf(fpOutput,"  MDwellTime_O  MDwellTime_C    RecordLength   NumEvents  ProbSingle      LogLik_R  SigComponents            Po    EvlFile\n");
			nSize=m_avNodes.size();
			for(i=0;i<nSize;i++)
			{
				fprintf(fpOutput,"%5d%10d%10d",i,nvScoreReanking[i],nLevel);
				for(j=0;j<nComponents;j++)
				{
					fprintf(fpOutput,"  %14.5f",m_avNodes_Rearranged[i].dTimeConstants[j]);
				}
				for(j=0;j<nComponents;j++)
				{
					fprintf(fpOutput,"  %10.7f",m_avNodes_Rearranged[i].dWeights[j]);
				}

				Freq_Components(m_avNodes_Rearranged[i].dTimeConstants,m_avNodes_Rearranged[i].dWeights,nComponents,structvFRange[i]
					,vnNumEvents.at(i),dvArea[i],dFrequency,dLength.at(i));
				for(j=0;j<nComponents;j++)
				{
					if(m_avNodes_Rearranged[i].dWeights[j]>0.f)
					{
						fprintf(fpOutput,"  %15.7f",dFrequency[j]*1000.);
					}
					else
					{
						fprintf(fpOutput,"  %15.7f",-100.);
					}
				}

				fprintf(fpOutput,"  %12.4f",dMTO.at(i));
				fprintf(fpOutput,"  %12.4f",dMTC.at(i));
				fprintf(fpOutput,"  %14.2f",dLength.at(i));
				fprintf(fpOutput,"  %10d",vnNumEvents.at(i));
				fprintf(fpOutput,"  %10.3e",dProbSingle.at(i));
				fprintf(fpOutput,"  %12.4f",dLogLik_R.at(i));
				fprintf(fpOutput,"%15d",vnSig[i]);
				fprintf(fpOutput,"  %12.4f    %s\n",dPo.at(i),sEvlFileNames.at(i));
			}
			fprintf(fpOutput,"\n\n");

			Vec_IO_DP dMean(nSize);
			Vec_IO_DP dOrder(nSize);

			int i,j;
			for(i=0;i<nSize;i++)
			{
				dMean[i]=dPo.at(i);
				dOrder[i]=double(i);
			}
			NR::sort2(dMean,dOrder);

 			for(int nIndex=0;nIndex<nSize;nIndex++)
			{
				i=int(dOrder[nIndex]+0.5);
				fprintf(fpOutput,"%5d%10d%10d",i,nvScoreReanking[i],nLevel);
				for(j=0;j<nComponents;j++)
				{
					fprintf(fpOutput,"  %14.5f",m_avNodes_Rearranged[i].dTimeConstants[j]);
				}
				for(j=0;j<nComponents;j++)
				{
					fprintf(fpOutput,"  %10.7f",m_avNodes_Rearranged[i].dWeights[j]);
				}

				Freq_Components(m_avNodes_Rearranged[i].dTimeConstants,m_avNodes_Rearranged[i].dWeights,nComponents,structvFRange[i]
					,vnNumEvents.at(i),dvArea[i],dFrequency,dLength.at(i));
				for(j=0;j<nComponents;j++)
				{
					if(m_avNodes_Rearranged[i].dWeights[j]>0.f)
					{
						fprintf(fpOutput,"  %15.7f",dFrequency[j]*1000.);
					}
					else
					{
						fprintf(fpOutput,"  %15.7f",-100.);
					}
				}

				fprintf(fpOutput,"  %12.4f",dMTO.at(i));
				fprintf(fpOutput,"  %12.4f",dMTC.at(i));
				fprintf(fpOutput,"  %14.2f",dLength.at(i));
				fprintf(fpOutput,"  %10d",vnNumEvents.at(i));
				fprintf(fpOutput,"  %10.3e",dProbSingle.at(i));
				fprintf(fpOutput,"  %12.4f",dLogLik_R.at(i));
				fprintf(fpOutput,"%15d",vnSig[i]);
				fprintf(fpOutput,"  %12.4f    %s\n",dPo.at(i),sEvlFileNames.at(i));
			}
			fprintf(fpOutput,"\n\n");


			int nTemp;
			double dTimeConstants[20];
			for(i=0;i<nComponents;i++)
			{
				structSemNode aNode;
				aNode=CalSem(fvectTimeConstants.at(i));
				fMean=aNode.fMean;
				nTemp=aNode.nSampleSize;
				fprintf(fpOutput,"%12.6f    %12.6f    %10d\n",fMean,aNode.fSem,nTemp);
				dTimeConstants[i]=fMean;
				m_dMeanTimeConstants[i]=fMean;
			}
			fprintf(fpOutput,"\n\n");

			for(i=0;i<nComponents;i++)
			{
				structSemNode aNode;
				aNode=CalSem(fvectWeights.at(i));
				fMean=aNode.fMean;
				nTemp=aNode.nSampleSize;
				fprintf(fpOutput,"%12.6f    %12.6f    %10d\n",fMean,aNode.fSem,nTemp);
			}
			fprintf(fpOutput,"\n\n");


			for(i=0;i<m_structFittedNode.nNumComponents;i++)
			{
				fprintf(fpOutput,"  %f",m_structTrialNode.dMean[i]);
				fprintf(fpOutput,"  %f",m_structTrialNode.dWeights[i]);
				fprintf(fpOutput,"  %f\n",m_structTrialNode.dDev[i]);
			}

			fprintf(fpOutput,"\n\n");


			for(i=0;i<m_structFittedNode.nNumComponents;i++)
			{
				fprintf(fpOutput,"  %f",m_structFittedNode.dMean[i]);
				fprintf(fpOutput,"  %f",m_structFittedNode.dWeights[i]);
				fprintf(fpOutput,"  %f\n",m_structFittedNode.dDev[i]);
			}

			fprintf(fpOutput,"\ndLogLik:   %f\n", m_structFittedNode.dLogLik);


			vector <double> vdBurstDelimiters=BurstDelimiters(dTimeConstants,nComponents);
			for(i=0;i<nComponents-1;i++)
			{
				fprintf(fpOutput,"%12.2f    ",vdBurstDelimiters[i]);
			}
			fprintf(fpOutput,"\n\n");
		}

		fclose(fpOutput);
}

structSemNode CFittedEvlParamterHandling::CalSem(vector <double> vectfVect)
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

vector <double> CFittedEvlParamterHandling::BurstDelimiters(double dT[], int nDim)
{
	vector <double> dBurstDelimiters;
	double dTemp;
	for(int i=0;i<nDim-1;i++)
	{
		dTemp=BurstDelimiter(dT[i],dT[i+1]);
		dBurstDelimiters.push_back(dTemp);
	}
	return dBurstDelimiters;
}

double CFittedEvlParamterHandling::BurstDelimiter(double fTs, double fTl)
{
	double fT1, fT2, fT;
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

double CFittedEvlParamterHandling::BurstDelimiter(float fTs, float fTl)
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

structSemNode CFittedEvlParamterHandling::CalSem(vector <float> vectfVect)
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

double CFittedEvlParamterHandling::CriticalFunction(float Ts, float Tl, float T)
{
	//Two components have equal proportion of mistakes
	double dValue=0.;
	dValue=1.-exp(-T/Tl)-exp(-T/Ts);
	return dValue;
}

void CFittedEvlParamterHandling::RearrangeTW()
{
	int i,j,nSize,nComponents,nIndex;
	nSize=m_avNodes.size();
	m_avNodes_Rearranged.clear();
	for(i=0;i<nSize;i++)
	{
		structFittingResultNode aNode;

		for(j=0;j<20;j++)
		{
			aNode.dTimeConstants[j]=-100.;
			aNode.dWeights[j]=-1;
		}

		nComponents=m_avNodes[i].nNumComponents;
		for(j=0;j<nComponents;j++)
		{
			nIndex=m_vivComponents[i].at(j);
			aNode.dTimeConstants[nIndex]=m_avNodes[i].dTimeConstants[j];
			aNode.dWeights[nIndex]=m_avNodes[i].dWeights[j];
		}
		aNode.nNumComponents=m_avNodes[i].nNumComponents;
		m_avNodes_Rearranged.push_back(aNode);
	}
}

int CFittedEvlParamterHandling::HighestDegeneracy()
{
	int nComponents=m_structFittedNode.nNumComponents;
	int nHist[20];
	int i,j;
	for(i=0;i<nComponents;i++)
	{
		nHist[i]=0;
	}
	int nSize=m_vDegeneracy.size();
	int nIndex;
	for(i=0;i<nSize;i++)
	{
		nIndex=m_vDegeneracy[i].nGausComponent;
		nHist[nIndex]+=1;
	}

	int nMax=nHist[0], nMaxIndex=0;
	for(i=0;i<nComponents;i++)
	{
		if(nHist[i]>nMax)
		{
			nMax=nHist[i];
			nMaxIndex=i;
		}
	}
	return nMaxIndex;
}

void CFittedEvlParamterHandling::RefineFitting()
{
	int nH=HighestDegeneracy();
	double dLow=0.,dHigh=0.;
	int nSize=m_vDegeneracy.size();
	int i,nFileIndex,nComponentIndex,nDegeneracy;
	nDegeneracy=0;
	for(i=0;i<nSize;i++)
	{
		if(nH==m_vDegeneracy[i].nGausComponent)
		{
			nFileIndex=m_vDegeneracy[i].nFileIndex;
			nComponentIndex=m_vDegeneracy[i].nComponentindex;
			dLow+=m_avNodes[nFileIndex].dTimeConstants[nComponentIndex];
			dHigh+=m_avNodes[nFileIndex].dTimeConstants[nComponentIndex+1];
			nDegeneracy++;
		}
	}
	dLow/=double(nDegeneracy);
	dHigh/=double(nDegeneracy);

	double dTimeConstants[20];

	m_nFittedComponents=m_structTrialNode.nNumComponents+1;
	m_structTrialNode.nNumComponents=m_nFittedComponents;
	if(m_bFreezeDev)
	{
		m_structTrialNode.nFreeParameters=2*m_nFittedComponents-1;
	}
	else
	{
		m_structTrialNode.nFreeParameters=3*m_nFittedComponents-1;
	}

	for(i=0;i<nH;i++)
	{
		dTimeConstants[i]=m_dMeanTimeConstants[i];
	}

	dTimeConstants[nH]=dLow;
	dTimeConstants[nH+1]=dHigh;

	for(i=nH+2;i<m_nFittedComponents;i++)
	{
		dTimeConstants[i]=m_dMeanTimeConstants[i-1];
	}


	double dTempa[20];
	for(i=0;i<m_nFittedComponents;i++)
	{ 
		m_structTrialNode.dMean[i]=dTimeConstants[i];
		m_structTrialNode.dWeights[i]=1./double(m_nFittedComponents);
		if(i==0)
		{
			m_structTrialNode.dDev[i]=dTimeConstants[i]/4.;
		}
		else
		{
			m_structTrialNode.dDev[i]=(dTimeConstants[i]-dTimeConstants[i-1])/4.;
		}
		if(m_structTrialNode.dDev[i]<0.2) m_structTrialNode.dDev[i]=0.2;
	}

	nSize=m_vdData.size();
	m_structTrialNode.nDataSize=nSize;
	m_structTrialNode.pdData=new double[nSize];
	for(i=0;i<nSize;i++)
	{
		m_structTrialNode.pdData[i]=m_vdData[i];
	}
	
	RemoveEmpyComponents();
	CGaussianFitting aFitting(m_structTrialNode);
	m_structFittedNode=aFitting.GetFittedNode();
}

bool CFittedEvlParamterHandling::Degenerated()
{
	if(m_vDegeneracy.size()>0)
	{
		return true;
	}
	else
	{
		return false;
	}
}

void CFittedEvlParamterHandling::RemoveEmpyComponents()
{
	int nComponents=m_structTrialNode.nNumComponents;
	int i,j,nEmptyComponent;
	vector <int> nvEmptyComponents;
	for(i=0;i<nComponents;i++)
	{
		if(m_structTrialNode.dMean[i]<0.)
		{
			nvEmptyComponents.push_back(i);
		}
	}

	int nSize=nvEmptyComponents.size();
	for(i=0;i<nSize;i++)
	{
		nEmptyComponent=nvEmptyComponents[i];
		for(j=nEmptyComponent+1;j<nSize;j++)
		{
			m_structTrialNode.dMean[j-1]=m_structTrialNode.dMean[j];
			m_structTrialNode.dDev[j-1]=m_structTrialNode.dDev[j];
			m_structTrialNode.dWeights[j-1]=m_structTrialNode.dWeights[j];
		}
	}
	if(m_bFreezeDev)
	{
		m_structTrialNode.nFreeParameters-=2*nSize;
	}
	else
	{
		m_structTrialNode.nFreeParameters-=3*nSize;
	}
}

void CFittedEvlParamterHandling::Freq_Components(double *dT, double *dW, int nComponents, structFRange fFittingRange, 
												 int nEventsInRange, double dArea, double* dFrequency, float fLength)
{
	int nTotalEvents=nEventsInRange/dArea;
	for(int i=0;i<nComponents;i++)
	{
		dFrequency[i]=nTotalEvents*dW[i]*(exp(-fFittingRange.fLeft/dT[i])-exp(-fFittingRange.fRight/dT[i]))/fLength;
	}
}

CString CFittedEvlParamterHandling::ChangeFileExt(CString sFileName, CString sExt)
{
	CString sNewFileName=sFileName;
	int nLength=sFileName.GetLength();
	for(int i=0;i<3;i++)
	{
		sNewFileName.SetAt(nLength-3+i,sExt[i]);
	}
	return sNewFileName;	

}

