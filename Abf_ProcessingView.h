// Abf_ProcessingView.h : interface of the CAbf_ProcessingView class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_ABF_PROCESSINGVIEW_H__829A080F_5FE1_11D4_83DE_00C04F200B5A__INCLUDED_)
#define AFX_ABF_PROCESSINGVIEW_H__829A080F_5FE1_11D4_83DE_00C04F200B5A__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


class CAbf_ProcessingView : public CScrollView
{
protected: // create from serialization only
	CAbf_ProcessingView();
	DECLARE_DYNCREATE(CAbf_ProcessingView)

// Attributes
public:
	CAbf_ProcessingDoc* GetDocument();

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAbf_ProcessingView)
	public:
	virtual void OnDraw(CDC* pDC);  // overridden to draw this view
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	protected:
	virtual void OnInitialUpdate(); // called first time after construct
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CAbf_ProcessingView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	//{{AFX_MSG(CAbf_ProcessingView)
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

#ifndef _DEBUG  // debug version in Abf_ProcessingView.cpp
inline CAbf_ProcessingDoc* CAbf_ProcessingView::GetDocument()
   { return (CAbf_ProcessingDoc*)m_pDocument; }
#endif

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_ABF_PROCESSINGVIEW_H__829A080F_5FE1_11D4_83DE_00C04F200B5A__INCLUDED_)
