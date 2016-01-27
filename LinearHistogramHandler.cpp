// LinearHistogramHandler.cpp: implementation of the CLinearHistogramHandler class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "Abf_Processing.h"
#include "LinearHistogramHandler.h"
#include "PrintAssist.h"
#include "math.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CLinearHistogramHandler::CLinearHistogramHandler()
{
	m_dPeakMinRatio=0.2;
}

CLinearHistogramHandler::~CLinearHistogramHandler()
{

}
void CLinearHistogramHandler::update(CLinearHistogramDouble aHist){
	aHist.getParameters(m_dBase,m_dDelta,m_nDim);
	m_dCounts=aHist.getTotalCounts();
	m_pdHist=new double[m_nDim];
	for(int i=0;i<m_nDim;i++){
		m_pdHist[i]=aHist.getCounts(i);
	}
	findPeaks();
	findValleys();
	m_dSum=aHist.getSum();
	m_dSum2=aHist.getSum2();
}
vector <int> CLinearHistogramHandler::getPeakIndexess(){
	findPeaks();
	return m_vnPeakIndexes;
}
int CLinearHistogramHandler::getLastPeakPosition(){
	findPeaks();
	int size=m_vnPeakIndexes.size();
	return m_vnPeakIndexes[size-1];
}
int CLinearHistogramHandler::getFirstPeakPosition(){
	findPeaks();
	if(m_vnPeakIndexes.size()>0) return m_vnPeakIndexes[0];
	return -1;
}
void CLinearHistogramHandler::setPeakCriteria(double peakMinRatio){
	m_dPeakMinRatio=peakMinRatio;
}
void CLinearHistogramHandler::findPeaks(){
	m_vnPeakIndexes.clear();
	double x0=m_pdHist[0];
	double x=m_pdHist[1];
	double x1;
	int i,j;
	for(i=1;i<m_nDim-1;i++){
		x1=m_pdHist[i+1];
		if((x-x1)>0&&(x-x0)>0){
			m_vnPeakIndexes.push_back(i);
		}
		if(x1!=x){
			x0=x;
			x=x1;
		}
	}
	m_nPeaks=m_vnPeakIndexes.size();
}
void CLinearHistogramHandler::findValleys(){
	m_vnValleyIndexes.clear();
	if(m_dCounts<0.001)
	{
		m_nValleys=0;
	}else{
		//empty histogram
		int peaks=m_vnPeakIndexes.size();
		int i,j,pmin,ji,jf;
		double dmin,x;
		if(peaks>0)ji=m_vnPeakIndexes[0];
		for(i=1;i<peaks;i++){
			jf=m_vnPeakIndexes[i];
			dmin=m_pdHist[ji];
			for(j=ji;j<=jf;j++){
				x=m_pdHist[j];
				if(x<dmin){
					dmin=x;
					pmin=j;
				}
			}
			m_vnValleyIndexes.push_back(pmin);
			ji=jf;
		}
		m_nValleys=m_vnValleyIndexes.size();
	}
}
void CLinearHistogramHandler::removeLowestPeak(){
	int peakIndex,valleyIndex;
	findLowestPeak(peakIndex,valleyIndex);
	removePeak(peakIndex,valleyIndex);//checked
}
bool CLinearHistogramHandler::removeLowestPeak(double threshold){
	int peakIndex,valleyIndex;
	bool removed=false;
	findLowestPeak(peakIndex,valleyIndex);
	if((m_pdHist[peakIndex]-m_pdHist[valleyIndex])<threshold){
		removePeak(peakIndex,valleyIndex);
		removed=true;
	}
	return removed;
}
void CLinearHistogramHandler::findLowestPeak(int &peakIndex, int &valleyIndex){
	int hmin,vmin,h,v;
	double dmin,x;
	bool first=true;
	int i;
	dmin=0;
	for(i=0;i<m_nValleys;i++){
		h=m_vnPeakIndexes[i];
		v=m_vnValleyIndexes[i];
		x=m_pdHist[h]-m_pdHist[v];
		if(first){
			dmin=x+1;
			first=false;
		}
		if(x<dmin){
			vmin=v;
			hmin=h;
			dmin=x;
		}
		h=m_vnPeakIndexes[i+1];
		x=m_pdHist[h]-m_pdHist[v];
		if(x<dmin){
			vmin=v;
			hmin=h;
			dmin=x;
		}
	}
	peakIndex=hmin;
	valleyIndex=vmin;//checked
}

void CLinearHistogramHandler::removePeak(int peakIndex, int valleyIndex){
	vector <int> vi,hi;
	int i,v,h;
	for(i=0;i<m_nValleys;i++){
		v=m_vnValleyIndexes[i];
		if(v!=valleyIndex) vi.push_back(v);
	}
	for(i=0;i<m_nPeaks;i++){
		h=m_vnPeakIndexes[i];
		if(h!=peakIndex) hi.push_back(h);
	}
	m_vnPeakIndexes.clear();
	m_vnPeakIndexes=hi;
	m_vnValleyIndexes.clear();
	m_vnValleyIndexes=vi;
	m_nValleys=vi.size();
	m_nPeaks=hi.size();//checked
}

void CLinearHistogramHandler::findPeaks(int *peakIndexes, int *valleyIndexes, int &numPeaks){//find numPeaks most prominent (acording to the peak/valley difference) peaks. It will modify the numPeaks
															   //if the histogram contains less peaks than it.
	if(numPeaks>m_nPeaks) numPeaks=m_nPeaks;
	int h,v;
	while(m_nPeaks>numPeaks){
		removeLowestPeak();
	}
	int i,j;
	for(i=0;i<m_nPeaks;i++){
		h=m_vnPeakIndexes[i];
		peakIndexes[i]=h;
	}
	for(i=0;i<m_nValleys;i++){
		v=m_vnValleyIndexes[i];
		valleyIndexes[i]=v;
	}
	findPeaks();
	findValleys();
}
void CLinearHistogramHandler::findSignificantPeaks(){
	if(m_dCounts>0.1){
		double threshold=calPeakThreshold();
		while(removeLowestPeak(threshold)){
		}
	}
}
void CLinearHistogramHandler::findSignificantPeaks(int *peakIndexes, int* valleyIndexes){
	findSignificantPeaks();
	vector <int> hi,vi;
	hi=getPeakIndexes();
	int size,i;
	size=hi.size();
	for(i=0;i<size;i++){
		peakIndexes[i]=hi[i];
	}
	vi=getValleyIndexes();
	size=vi.size();
	for(i=0;i<size;i++){
		valleyIndexes[i]=vi[i];
	}
}
int CLinearHistogramHandler::getLastSignificantPeak(){
	findSignificantPeaks();
	int size=m_vnPeakIndexes.size();
	return m_vnPeakIndexes[size-1];
}
vector <int> CLinearHistogramHandler::getPeakIndexes(){
	vector <int> hi;
	int size=m_vnPeakIndexes.size();
	for(int i=0;i<size;i++){
		hi.push_back(m_vnPeakIndexes[i]);
	}
	return hi;
}
vector <int> CLinearHistogramHandler::getValleyIndexes(){
	vector <int> vi;
	int size=m_vnValleyIndexes.size();
	for(int i=0;i<size;i++){
		vi.push_back(m_vnPeakIndexes[i]);
	}
	return vi;
}
double CLinearHistogramHandler::calPeakThreshold(){
	int* vi,*hi;
	int peaks=2;
	hi=new int[peaks];
	vi=new int[peaks-1];
	findPeaks(hi,vi,peaks);
	double x0,x,height;
	int v,h;
	v=vi[0];
	h=hi[0];
	x=m_pdHist[h];
	h=hi[1];
	x0=m_pdHist[h];
	if(x0<x) x=x0;
	height=x-m_pdHist[v];
	return height*m_dPeakMinRatio;
}
void CLinearHistogramHandler::normalizePeakRange(int bins, int key, CLinearHistogramDouble &hist){
	CLinearHistogramDouble hist0;
	hist0.copyHist(hist);
	update(hist);
	findSignificantPeaks();
	int n=m_vnPeakIndexes.size();
	int i1=m_vnPeakIndexes[0];
	int i2=m_vnPeakIndexes[n-1];
	double x1=0.;
	double x2=1.;
	if(key<0){
		x1=-1.;
		x2=0.;
	}
	transformCoordinate(i1,x1,i2,x2,hist);
	hist.resample(1./bins);
	update(hist0);
}
void CLinearHistogramHandler::normalizePeakRange(double x1, double x2, CLinearHistogramDouble &hist){
	CLinearHistogramDouble hist0;
	hist0.copyHist(hist);
	update(hist);
	findSignificantPeaks();
	int n=m_vnPeakIndexes.size();
	int i1=m_vnPeakIndexes[0];
	int i2=m_vnPeakIndexes[n-1];
	transformCoordinate(i1,x1,i2,x2,hist);
	update(hist0);
}
void CLinearHistogramHandler::transformCoordinate(int index1, double x1, int index2, double x2, CLinearHistogramDouble &hist){
	double base, delta;
	double d=(x2-x1)/(index2-index1);
	hist.resetDelta(d);
	hist.resetBinPosition(index1,x1);
}


int CLinearHistogramHandler::getMainPeak()
{

	int hmax,h;
	double dmax,x;
	bool first=true;
	int i;
	dmax=0;
	for(i=0;i<m_nPeaks;i++){
		h=m_vnPeakIndexes[i];
		x=m_pdHist[h];
		if(first){
			dmax=x;
			first=false;
			hmax=h;
		}
		if(x>dmax){
			hmax=h;
			dmax=x;
		}
	}
	return hmax;
}


int CLinearHistogramHandler::getNumPeaks()
{
	return m_nPeaks;
}

int CLinearHistogramHandler::getNumValleys()
{
	return m_nValleys;
}

double CLinearHistogramHandler::getPeakPosition(int index)
{
	int i=m_vnPeakIndexes[index];
	return m_dBase+i*m_dDelta;
}
double CLinearHistogramHandler::getValleyPosition(int index)
{
	int i=m_vnValleyIndexes[index];
	return m_dBase+i*m_dDelta;
}
void CLinearHistogramHandler::exportHistograms(FILE* fp, vector <CLinearHistogramDouble> hists){
	int nSize=hists.size();
	vector <int> nvPw,nvCw,nvPp,nvCp;
	vector <CString> sCTs;
	CString sPT;
	int nPw,nCw,nPp,nCp;
	int i,j,nDim;
	CLinearHistogramDouble hist;
	CPrintAssist cpa;
	double dMean, dSem;
	int nExtraSpace=2;
	for(i=0;i<nSize;i++){
		hist=hists[i];
		nPw=hist.getPositionPrintingLength()+nExtraSpace;
		nCw=hist.getCountPrintingLength()+nExtraSpace;
		nPp=hist.getPositionPrecision();
		nCp=hist.getCountPrecision();
		nvPw.push_back(nPw);
		nvCw.push_back(nCw);
		nvPp.push_back(nPp);
		nvCp.push_back(nCp);
		sCTs.push_back(hist.getCountTitle());
	}
//	cpa.printString(fp,sPT,nvPw[0]);
	int nMax=0;
	int nSum;
	int nMaxDim=0;
	for(i=0;i<nSize;i++){
		hist=hists[i];
		sPT=hists[i].getPositionTitle();
		nDim=hist.getDim();
		if(nDim>nMaxDim) nMaxDim=nDim;
		cpa.printString(fp,sPT,nvPw[i]);
		cpa.printString(fp,hist.getCountTitle(),nvCw[i]);
		nSum=hist.getTotalCounts();
		if(nSum>nMax) nMax=nSum;
	}
	int len=cpa.getDigits(nMax)+2;
	int len1=12;
	if(len<14) len=14;
	cpa.printString(fp,"TotalCounts",len);
	cpa.printString(fp,"Mean",len1);
	cpa.printString(fp,"Sem",len1);
	nDim=hists[0].getSize();
	cpa.endLine(fp);
	double dLog10=log(10.);
	int nMode;
	double dPosition;
	for(i=0;i<nMaxDim;i++){
		for(j=0;j<nSize;j++){
			dPosition=hists[j].getPosition(i);
			if(hists[j].getMode()==1) dPosition=exp(dPosition*dLog10);
			cpa.printNumber(fp,dPosition,nvPw[j],nvPp[j]);
			cpa.printNumber(fp,hists[j].getCounts(i)/hists[j].getTotalCounts(),nvCw[j],nvCp[j]);
		}
		if(i<nSize){
			cpa.printNumber(fp,hists[i].getTotalCounts(),len,0);
			hists[i].getMeanSem(dMean,dSem);
			cpa.printNumberScientific(fp,dMean,len1,4);
			cpa.printNumberScientific(fp,dSem,len1,4);
		}
		cpa.endLine(fp);
	}
}

void CLinearHistogramHandler::exportHistograms(FILE* fp, vector <CLinearHistogramDouble> hists, int nWS){
	int nSize=hists.size();
	int nIT=1;
	exportHistograms(fp,hists,nWS,nIT);
}
void CLinearHistogramHandler::exportHistograms(FILE* fp, vector <CLinearHistogramDouble> hists, int nWS, int nIT){
	int nSize=hists.size();
	int i,j;
	for(i=0;i<nSize;i++){
		for(j=0;j<nIT;j++){
			hists[i].smoothHistogram(nWS);
		}
	}
	exportHistograms(fp,hists);
}

