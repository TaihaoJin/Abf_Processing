// Abf_ProcessingView.cpp : implementation of the CAbf_ProcessingView class
//

#include "stdafx.h"
#include "Abf_Processing.h"

#include "Abf_ProcessingDoc.h"
#include "Abf_ProcessingView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CAbf_ProcessingView

IMPLEMENT_DYNCREATE(CAbf_ProcessingView, CScrollView)

BEGIN_MESSAGE_MAP(CAbf_ProcessingView, CScrollView)
	//{{AFX_MSG_MAP(CAbf_ProcessingView)
	//}}AFX_MSG_MAP
	// Standard printing commands
	ON_COMMAND(ID_FILE_PRINT, CScrollView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, CScrollView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, CScrollView::OnFilePrintPreview)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CAbf_ProcessingView construction/destruction

CAbf_ProcessingView::CAbf_ProcessingView()
{
	// TODO: add construction code here

}

CAbf_ProcessingView::~CAbf_ProcessingView()
{
}

BOOL CAbf_ProcessingView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	return CScrollView::PreCreateWindow(cs);
}

/////////////////////////////////////////////////////////////////////////////
// CAbf_ProcessingView drawing

void CAbf_ProcessingView::OnDraw(CDC* pDC)
{
	CAbf_ProcessingDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	// TODO: add draw code for native data here
}

void CAbf_ProcessingView::OnInitialUpdate()
{
	CScrollView::OnInitialUpdate();

	CSize sizeTotal;
	// TODO: calculate the total size of this view
	sizeTotal.cx = sizeTotal.cy = 100;
	SetScrollSizes(MM_TEXT, sizeTotal);
}

/////////////////////////////////////////////////////////////////////////////
// CAbf_ProcessingView printing

BOOL CAbf_ProcessingView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// default preparation
	return DoPreparePrinting(pInfo);
}

void CAbf_ProcessingView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add extra initialization before printing
}

void CAbf_ProcessingView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add cleanup after printing
}

/////////////////////////////////////////////////////////////////////////////
// CAbf_ProcessingView diagnostics

#ifdef _DEBUG
void CAbf_ProcessingView::AssertValid() const
{
	CScrollView::AssertValid();
}

void CAbf_ProcessingView::Dump(CDumpContext& dc) const
{
	CScrollView::Dump(dc);
}

CAbf_ProcessingDoc* CAbf_ProcessingView::GetDocument() // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CAbf_ProcessingDoc)));
	return (CAbf_ProcessingDoc*)m_pDocument;
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CAbf_ProcessingView message handlers


//DEL void CAbf_ProcessingView::OnFileExportLocalextremaofdatafile() 
//DEL {
//DEL 	// TODO: Add your command handler code here
//DEL 	
//DEL }
