// LinearHistogramDouble.h: interface for the CLinearHistogramDouble class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_LINEARHISTOGRAMDOUBLE_H__97EC2319_8BC5_40C0_BA1E_25937C28F268__INCLUDED_)
#define AFX_LINEARHISTOGRAMDOUBLE_H__97EC2319_8BC5_40C0_BA1E_25937C28F268__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#include <vector>
using namespace std;

class CLinearHistogramDouble  
{
public:
	int getMode();
	int getDim();
	void update(int nMode, double dMin, double dMax, double dDelta);
	void updateSums(double dSum, double dSum2);
	int CLinearHistogramDouble::getDim0();
	double CLinearHistogramDouble::getBase0();
	double CLinearHistogramDouble::getDelta0();
	int CLinearHistogramDouble::getWS();
	void getMeanSem(double &dMean, double &dSem);
	double getSum();
	double getSum2();
	void clearHist();
	void appendHist(CLinearHistogramDouble hist);
	void getCoordinateTransformations(double &shift, double& scaling);
	void transformCoordinate(double shift, double scalingFactor);
	CLinearHistogramDouble();
	virtual ~CLinearHistogramDouble();
	void update(double dMin, double dMax, double dDelta);
	void normalizePeakRange(int bins, int key);
	void resample(double dDelta);
	void getPeakRanges(double &dP1, double &dP2);
	void CLinearHistogramDouble::scaling(double scale);
	void resetDelta(double delta);
	void resetBinPosition(int index, double position);
	void shiftting(double shift);
	void smoothHistogram(int nWs);
	void copyHist(CLinearHistogramDouble aHist);
	double getCounts(int index);
	double getCounts(double x1, double x2);
	double getPosition(int index);
	void getOriginalParameters(double &dBase0, double &dDelta0, int &nDim0);
	void getParameters(double &dBase, double &dDelta, int &nDim);
	void addData(float *pdData, int firstIndex, int delta, int num);
	void addData(double *pdData, int firstIndex, int delta, int num);
	void addData(double dValue, double num);
	double getTotalCounts();
	int getSize();
	void addData(double dValue);
	int getIndex(double dValue);
	//dMax and dMin is the minimum and maximum of real data, respectively. dBase is starting point of the first bin.
	//The function update is used initialize the histogram.
	void mergeHist(CLinearHistogramDouble hist);
	double getDelta();
	double getMax();
	double getMin();
	void expandRange(double x);//expand the range of the histogram to accomodate the position x
	void addData(double xi, double xf, double num);
	double getMaxCount();
	void setTitles(CString sPositionTitle, CString CountTitle);
	CString getPositionTitle();
	CString getCountTitle();
	int getPositionPrintingLength();
	int getCountPrintingLength();
	int getCountPrecision();
	int getPositionPrecision();
protected:
	CString m_sPositionTitle, m_sCountTitle;
	double m_dBase,m_dDelta,*m_pdHist,m_dSum,m_dMaxCount,m_dSum2,m_dCounts;
	double m_dBase0, m_dDelta0;//The original base and scale values, reset only by the function update.
	int m_nDim, m_nDim0;
	int m_nWs; //the window size for smoothing the histogram. the counts in the i-th bin is the average of the bins from (i-m_nWs) to (i+m_nWs).
	int m_nMode;//0 for linear and 1 for logarithmic. The defaul mode is 0. It's only set by the function update. 
				//the parameters such m_dBase, m_dDelta are transformed into log scale by the function that calls update(...)
				//The only functions that differentiate the different modes are addData(...) functions. 
				//It is the responsibility of the functions that uses the histograms to interpret the positions of the 
				//bin boundaries according to the mode of the histgram. 
};

#endif // !defined(AFX_LINEARHISTOGRAMDOUBLE_H__97EC2319_8BC5_40C0_BA1E_25937C28F268__INCLUDED_)
