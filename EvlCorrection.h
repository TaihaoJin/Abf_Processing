// EvlCorrection.h: interface for the CEvlCorrection class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_EVLCORRECTION_H__A4900EA8_3A96_4C9A_917D_6DB0C94B8E84__INCLUDED_)
#define AFX_EVLCORRECTION_H__A4900EA8_3A96_4C9A_917D_6DB0C94B8E84__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#include "structures.h"
class CEvlCorrection  
{
public:
	CEvlCorrection();
	virtual ~CEvlCorrection();
	void UpdateEventList(structEventListNode structNewEventList);
	structEventListNode GetEventList();
	void GetCorrectEvents();
	void CorrectEventList();
protected:
	int ApplyCorrectEvent(structEventNode aNode, int nPosition);
	void SortAsending(float* pfData,int *pnOrder,  int nSize);
	structEventNode ExtractEventNode(int nI0);
	structpClampResultNode ReadpClampResultFile();
	structEventListNode m_structEventList;
	vector <structEventNode> m_vstructCorrectEvents;
	CString m_sClampfitReultFileName;
	structpClampResultNode m_structpClampResultNode;
	short m_stCurrentLevels[4][4];
};

#endif // !defined(AFX_EVLCORRECTION_H__A4900EA8_3A96_4C9A_917D_6DB0C94B8E84__INCLUDED_)
