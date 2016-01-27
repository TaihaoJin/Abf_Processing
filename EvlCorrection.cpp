// EvlCorrection.cpp: implementation of the CEvlCorrection class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "Abf_Processing.h"
#include "EvlCorrection.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CEvlCorrection::CEvlCorrection()
{
	m_stCurrentLevels[0][0]=0;
	m_stCurrentLevels[0][1]=0;
	m_stCurrentLevels[0][2]=1;
	m_stCurrentLevels[0][3]=2;
	m_stCurrentLevels[1][0]=3;
	m_stCurrentLevels[1][1]=0;
	m_stCurrentLevels[1][2]=4;
	m_stCurrentLevels[1][3]=5;
	m_stCurrentLevels[2][0]=6;
	m_stCurrentLevels[2][1]=7;
	m_stCurrentLevels[2][2]=0;
	m_stCurrentLevels[2][3]=8;
	m_stCurrentLevels[3][0]=9;
	m_stCurrentLevels[3][1]=10;
	m_stCurrentLevels[3][2]=11;
	m_stCurrentLevels[3][3]=0;
}

CEvlCorrection::~CEvlCorrection()
{

}

void CEvlCorrection::UpdateEventList(structEventListNode structNewEventList)
{
	m_structEventList=structNewEventList;
}
structEventListNode CEvlCorrection::GetEventList()
{
	return m_structEventList;
}
structpClampResultNode CEvlCorrection::ReadpClampResultFile()
{
	FILE *fpAbfIn;
	CString sAbfFileName;
	CFileDialog aFileDlg( true, NULL, NULL, OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, NULL, NULL );
	char* pch="text form of pClamp9 result Files (*.atf)\0*.atf\0";
	aFileDlg.m_ofn.lpstrFilter=pch;
	aFileDlg.m_ofn.lpstrTitle="Import a pClamp9 result file";
	char cTemp,cTemp0;
	cTemp0='Z';
	int nChar=0;
	int nLine=0;
	float fLine[26];
	float fTemp=0.f;
	int nLineLength=26;
	structpClampResultNode aNode;
	aNode.nLineLength=26;
	aNode.vfData.clear();
	aNode.nNumLines=0;
	int i;

	if(aFileDlg.DoModal()==IDOK)
	{
		m_sClampfitReultFileName=aFileDlg.GetPathName();
		fpAbfIn=fopen(m_sClampfitReultFileName,"r");
		while(!feof(fpAbfIn))
		{
			nChar++;
			fscanf(fpAbfIn,"%c",&cTemp);
			if(cTemp=='A'&&cTemp0=='p')
			{
				for(i=0;i<nLineLength;i++)
				{
					fscanf(fpAbfIn,"%f",&fTemp);
					aNode.vfData.push_back(fTemp);
				}
				nLine++;
			}
			cTemp0=cTemp;
		}
		aNode.nNumLines=nLine;
		fclose(fpAbfIn);
	}
	return aNode;
}


void CEvlCorrection::GetCorrectEvents()
{
	m_structpClampResultNode=ReadpClampResultFile();
	int nSize=m_structpClampResultNode.nNumLines;
	m_vstructCorrectEvents.clear();

	for(int i=0;i<nSize;i++)
	{
		structEventNode aEventNode=ExtractEventNode(i);
		m_vstructCorrectEvents.push_back(aEventNode);
	}

}

structEventNode CEvlCorrection::ExtractEventNode(int nI0)
{
	int i;
	float * pfData=new float[4];
	int * pnOrder=new int[4];
	int nLineLength=m_structpClampResultNode.nLineLength;
	structEventNode aEventNode;
	for(i=0;i<4;i++)
	{
		pfData[i]=m_structpClampResultNode.vfData[nI0*nLineLength+4+i];
		pnOrder[i]=i;
	}
	SortAsending(pfData,pnOrder,4);
	aEventNode.stLevel=m_stCurrentLevels[pnOrder[0]][pnOrder[1]];
	float fTimePerPoint=m_structEventList.fTimePerPoint;
	aEventNode.LevelStart=int(pfData[0]*1000./fTimePerPoint+0.5);
	aEventNode.LevelLength=int((pfData[1]-pfData[0])*1000./fTimePerPoint+0.5);
	aEventNode.fAmpDev=1.f;
	aEventNode.fAmpMean=float(aEventNode.stLevel);
	return aEventNode;
}

void CEvlCorrection::SortAsending(float* pfData,int *pnOrder,  int nSize)
{
	int i,j, nTemp, nSmallest;
	float fTemp, fSmallest;
	for(i=0;i<nSize;i++)
	{
		nSmallest=i;
		fSmallest=pfData[i];
		for(j=i+1;j<nSize;j++)
		{
			fTemp=pfData[j];
			if(fTemp<fSmallest)
			{
				fSmallest=fTemp;
				nSmallest=j;
			}
		}
		if(nSmallest>i)
		{
			fTemp=pfData[i];
			pfData[i]=fSmallest;
			pfData[nSmallest]=fTemp;
			pnOrder[i]=nSmallest;
		}
	}
}

void CEvlCorrection::CorrectEventList()
{
	GetCorrectEvents();
	int nSize=m_vstructCorrectEvents.size();
	int nPosition=0;
	for(int i=0;i<nSize;i++)
	{
		nPosition=ApplyCorrectEvent(m_vstructCorrectEvents[i],nPosition);
		//nPosition a safe index in the event eventlist from which the function ApplyCorrectEvent will search the event started before the input event and ended after the input event
	}
}

int CEvlCorrection::ApplyCorrectEvent(structEventNode aNode, int nPosition)
{
	int nStart0=aNode.LevelStart;
	int nLength0=aNode.LevelLength;
	short stLevel0=aNode.stLevel;
	int nLeftIndex=nPosition, nRightIndex=nPosition;
	int nSize=m_structEventList.vstructEventList.size();
	int nStart, nLength;
	int i,j;

	vector<structEventNode>::iterator theIterator, LeftIterator, RightIterator;

	for(i=0;i<nSize;i++)
	{
		nStart=m_structEventList.vstructEventList[i].LevelStart;
		nLength=m_structEventList.vstructEventList[i].LevelLength;
		if(nStart<=nStart0&&nStart+nLength>nStart0)
		{
			nLeftIndex=i;
		}
		if(nStart<=nStart0+nLength0-1&&nStart+nLength>nStart0+nLength0-1)
		{
			nRightIndex=i;
			break;
		}
	}

	if(nLeftIndex==nRightIndex)
	{
		structEventNode aTempNode=m_structEventList.vstructEventList[nRightIndex];
		theIterator=m_structEventList.vstructEventList.begin()+nRightIndex;
		m_structEventList.vstructEventList.insert(theIterator,aTempNode);
		nRightIndex++;
	}

	short stLevelL=m_structEventList.vstructEventList[nLeftIndex].stLevel;
	short stLevelR=m_structEventList.vstructEventList[nRightIndex].stLevel;

	if(stLevelL==stLevel0)
	{
		m_structEventList.vstructEventList[nLeftIndex].LevelLength=nStart0+nLength0-m_structEventList.vstructEventList[nLeftIndex].LevelStart;
		if(stLevel0==stLevelR)
		{
			m_structEventList.vstructEventList[nLeftIndex].LevelLength=m_structEventList.vstructEventList[nRightIndex].LevelStart+m_structEventList.vstructEventList[nRightIndex].LevelLength
																	-m_structEventList.vstructEventList[nLeftIndex].LevelStart;
			theIterator=m_structEventList.vstructEventList.begin()+nLeftIndex+1;
			for(i=nLeftIndex+1;i<=nRightIndex;i++)
			{
				m_structEventList.vstructEventList.erase(theIterator);
			}
		}
		else
		{
			m_structEventList.vstructEventList[nRightIndex].LevelLength=m_structEventList.vstructEventList[nRightIndex].LevelStart+m_structEventList.vstructEventList[nRightIndex].LevelLength
				                                                        -(nStart0+nLength0);
			m_structEventList.vstructEventList[nRightIndex].LevelStart=nStart0+nLength0;
			theIterator=m_structEventList.vstructEventList.begin()+nLeftIndex+1;
			for(i=nLeftIndex+1;i<nRightIndex;i++)
			{
				m_structEventList.vstructEventList.erase(theIterator);
				theIterator++;
			}
		}
	}
	else
	{
		m_structEventList.vstructEventList[nLeftIndex].LevelLength=nStart0-m_structEventList.vstructEventList[nLeftIndex].LevelStart;
		if(stLevel0==stLevelR)
		{
			m_structEventList.vstructEventList[nRightIndex].LevelLength=m_structEventList.vstructEventList[nRightIndex].LevelStart+m_structEventList.vstructEventList[nRightIndex].LevelLength
																	-nStart0;
			m_structEventList.vstructEventList[nRightIndex].LevelStart=nStart0;
			theIterator=m_structEventList.vstructEventList.begin()+nLeftIndex+1;
			for(i=nLeftIndex+1;i<nRightIndex;i++)
			{
				m_structEventList.vstructEventList.erase(theIterator);
			}
		}
		else
		{
			m_structEventList.vstructEventList[nRightIndex].LevelLength=m_structEventList.vstructEventList[nRightIndex].LevelStart+m_structEventList.vstructEventList[nRightIndex].LevelLength
				                                                        -(nStart0+nLength0);

			m_structEventList.vstructEventList[nRightIndex].LevelStart=nStart0+nLength0;
			theIterator=m_structEventList.vstructEventList.begin()+nRightIndex;
			m_structEventList.vstructEventList.insert(theIterator,aNode);
			theIterator=m_structEventList.vstructEventList.begin()+nLeftIndex+1;
			for(i=nLeftIndex+1;i<nRightIndex;i++)
			{
				m_structEventList.vstructEventList.erase(theIterator);
			}
		}
	}
	return nLeftIndex;
}

