// LinearHistogramHandler.h: interface for the CLinearHistogramHandler class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_LINEARHISTOGRAMHANDLER_H__4B509F12_5C91_4C14_9E55_285796ED6B01__INCLUDED_)
#define AFX_LINEARHISTOGRAMHANDLER_H__4B509F12_5C91_4C14_9E55_285796ED6B01__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#include "LinearHistogramDouble.h"
#include <vector>
using namespace std;
class CLinearHistogramHandler  
{
public:
	void exportHistograms(FILE* fp, vector <CLinearHistogramDouble> hists, int nWS);
	void exportHistograms(FILE* fp, vector <CLinearHistogramDouble> hists, int nWS, int nIT);
	double getPeakPosition(int index);
	int getNumValleys();
	int getNumPeaks();
	int getMainPeak();
	CLinearHistogramHandler();
	virtual ~CLinearHistogramHandler();
	void findPeaks();
	void setPeakCriteria(double peakMinRatio);
	void update(CLinearHistogramDouble hist);
	double getValleyPosition(int index);
	vector <int> getPeakIndexess();
	int getFirstPeakPosition();
	int getLastPeakPosition();
	void findValleys();
	vector <int> findPeaks(int &numPeaks);
	double CLinearHistogramHandler::calPeakThreshold();
	vector <int> getValleyIndexes();
	vector <int> getPeakIndexes();
	void findSignificantPeaks(int *peakIndexes, int *valleyIndexes);
	void findSignificantPeaks();
	void findPeaks(int *peakIndexes, int *valleyIndexes, int &numPeaks);//find numPeaks most prominent (acording to the peak/valley difference) peaks. It will modify the numPeaks
																   //if the histogram contains less peaks than it.
	void removePeak(int peakIndex, int valleyIndex);
	void findLowestPeak(int &peakIndex, int &valleyIndex);
	bool removeLowestPeak(double threshold);
	void removeLowestPeak();
	int getLastSignificantPeak();
	void normalizePeakRange(double xi, double x2, CLinearHistogramDouble &hist);
	void normalizePeakRange(int bins, int key, CLinearHistogramDouble &hist);
	void transformCoordinate(int index1, double x1, int index2, double x2, CLinearHistogramDouble &hist);
	void exportHistograms(FILE* fp, vector <CLinearHistogramDouble> hists);
protected:
	double m_dBase,m_dDelta,*m_pdHist,m_dSum,m_dSum2,m_dCounts;
	int m_nDim,m_nPeaks,m_nValleys;
	double m_dPeakMinRatio;
	vector <int> m_vnPeakIndexes;
	vector <int> m_vnValleyIndexes;
};

#endif // !defined(AFX_LINEARHISTOGRAMHANDLER_H__4B509F12_5C91_4C14_9E55_285796ED6B01__INCLUDED_)
