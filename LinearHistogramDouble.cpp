// LinearHistogramDouble.cpp: implementation of the CLinearHistogramDouble class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "Abf_Processing.h"
#include "LinearHistogramDouble.h"
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

CLinearHistogramDouble::CLinearHistogramDouble()
{
	m_pdHist=new double[1];
	m_nMode=0;
	m_dSum=0.;
	m_dCounts=0.;
	m_dSum2=0.;
	m_nWs=-1; //the window size for smoothing the histogram. the counts in the i-th bin is the average of the bins from (i-m_nWs) to (i+m_nWs).
}

CLinearHistogramDouble::~CLinearHistogramDouble()
{

}
void CLinearHistogramDouble::update(int nMode, double dMin, double dMax, double dDelta){
	m_nMode=nMode;
	update(dMin,dMax,dDelta);
}
void CLinearHistogramDouble::update(double dMin, double dMax, double dDelta){
	m_dDelta=dDelta;
	m_dBase=int(dMin/dDelta)*dDelta;
	if(m_dBase>dMin) m_dBase-=dDelta;
	delete m_pdHist;
	m_nDim=int((dMax-m_dBase)/dDelta)+1;
	m_pdHist=new double[m_nDim];
	for(int i=0;i<m_nDim;i++)
	{
		m_pdHist[i]=0.;
	}
	m_dSum=0.;
	m_dCounts=0.;
	m_dSum2=0.;
	m_nWs=-1;
	m_dBase0=m_dBase;
	m_dDelta0=m_dDelta;
	m_nDim0=m_nDim;
}

int CLinearHistogramDouble::getIndex(double dValue){
	int index=int((dValue-m_dBase)/m_dDelta);
	if(index>=m_nDim||index<0) index=-1;
	return index;
}

void CLinearHistogramDouble::addData(double dValue){
	double dV=dValue;
	if(m_nMode==1) dV=log(dV)/log(10.);
	int index=getIndex(dV);
	if(index>=0){
		m_pdHist[index]+=1.;
		m_dCounts+=1.;
		m_dSum+=dValue;
		m_dSum2+=dValue*dValue;
	}
}

double CLinearHistogramDouble::getTotalCounts(){
	return m_dCounts;
}

void CLinearHistogramDouble::addData(double dValue, double num){
	double dV=dValue;
	if(m_nMode==1) dV=log(dV)/log(10.);
	int index=getIndex(dV);
	if(index>=0){
		m_pdHist[index]+=num;
		m_dCounts+=num;
		m_dSum+=num*dValue;
		m_dSum2+=num*dValue*dValue;
	}
}
void CLinearHistogramDouble::addData(double *pdData, int firstIndex, int delta, int num){
	int index,i;
	double dV;
	for(i=0;i<num;i++)
	{
		index=firstIndex+i*delta;
		dV=pdData[index];
		addData(dV);
	}
}
void CLinearHistogramDouble::addData(float *pdData, int firstIndex, int delta, int num){
	int index;
	double dV;
	for(int i=0;i<num;i++)
	{
		index=firstIndex+i*delta;
		dV=pdData[index];
		addData(dV);
	}
}
void CLinearHistogramDouble::getParameters(double &dBase, double &dDelta, int &nDim){
	dDelta=m_dDelta;
	dBase=m_dBase;
	nDim=m_nDim;
}
void CLinearHistogramDouble::getOriginalParameters(double &dBase0, double &dDelta0, int &nDim0){
	dDelta0=m_dDelta0;
	dBase0=m_dBase0;
	nDim0=m_nDim0;
}
int CLinearHistogramDouble::getSize(){
	return m_nDim;
}
void CLinearHistogramDouble::smoothHistogram(int nWs){
	m_nWs=nWs;
	int i,j,n,ji,jf;
	double* pdHist=new double[m_nDim];
	for(i=0;i<m_nDim;i++){
		ji=i-m_nWs;
		jf=i+m_nWs;
		n=0;
		pdHist[i]=0.;
		for(j=ji;j<=jf;j++){
			if(j>=0&&j<m_nDim){
				n++;
				pdHist[i]+=m_pdHist[j];
			}
		}
		pdHist[i]/=n;
	}
	delete m_pdHist;
	m_pdHist=pdHist;
}
double CLinearHistogramDouble::getPosition(int index){
	return m_dBase+index*m_dDelta;
}
double CLinearHistogramDouble::getCounts(int index){
	double dCount=0.;
	if(index>=0&&index<m_nDim) dCount=m_pdHist[index];
	return dCount;
}
void CLinearHistogramDouble::copyHist(CLinearHistogramDouble aHist){
	aHist.getParameters(m_dBase,m_dDelta,m_nDim);
	m_dCounts=aHist.getTotalCounts();
	m_pdHist=new double[m_nDim];
	for(int i=0;i<m_nDim;i++){
		m_pdHist[i]=aHist.getCounts(i);
	}
	aHist.getOriginalParameters(m_dBase0,m_dDelta0,m_nDim0);
	m_dSum=aHist.getSum();
	m_dSum2=aHist.getSum2();
	m_sPositionTitle=aHist.getPositionTitle();
	m_sCountTitle=aHist.getCountTitle();
	m_dMaxCount=aHist.getMaxCount();
	m_nWs=aHist.getWS();
	m_nDim0=aHist.getDim0();
	m_dDelta0=aHist.getDelta0();
	m_dBase0=aHist.getBase0();
}
void CLinearHistogramDouble::shiftting(double shift){//(verified on 8/7/09)
	m_dBase+=shift;
}
void CLinearHistogramDouble::scaling(double scale){//(verified on 8/7/09)
	m_dBase*=scale;
	m_dDelta*=scale;
}
void CLinearHistogramDouble::resetBinPosition(int index, double position){//(verified on 8/7/09)
	double dShift=position-getPosition(index);
	shiftting(dShift);
}
void CLinearHistogramDouble::resetDelta(double delta){//(verified on 8/7/09)
	double scale=delta/m_dDelta;
	scaling(scale);
}

void CLinearHistogramDouble::resample(double dDelta){
	CLinearHistogramDouble newHist;
	double dmin=m_dBase;
	double dmax=m_dBase+m_nDim*m_dDelta;
	double x1,x2,count;
	newHist.update(dmin,dmax,dDelta);
	for(int i=0;i<m_nDim;i++){
		count=m_pdHist[i];
		x1=getPosition(i);
		x2=getPosition(i+1);
		newHist.addData(x1,x2,count);
	}
	copyHist(newHist);
}

void CLinearHistogramDouble::addData(double xi, double xf, double num){
	//this function is exclusively used for operations on the histograms. 
	//m_dSum and m_dSum2 will be adjusted independently, rather then adjusting 
	//within this function.
	int i1=getIndex(xi);
	if(i1<0) i1=0;
	int i2=getIndex(xf);
	if(i2>=m_nDim) i2=m_nDim-1;
	double p,x0,x1;
	for(int i=i1;i<=i2;i++){
		x0=getPosition(i);
		x1=x0+m_dDelta;
		if(xi>x0) x0=xi;
		if(xf<x1) x1=xf;
		p=(x1-x0)/(xf-xi);
		m_pdHist[i]+=p*num;
		m_dCounts+=p*num;
	}
}
double CLinearHistogramDouble::getCounts(double xi, double xf){//return the integration of the pdf from xi to xf
	int i1=getIndex(xi);
	int i2=getIndex(xf);
	double p,x0,x1,counts;
	counts=0.;
	for(int i=i1;i<=i2;i++){
		x0=getPosition(i);
		x1=x0+m_dDelta;
		if(xi>x0) x0=xi;
		if(xf<x1) x1=xf;
		p=(x1-x0)/m_dDelta;
		counts+=p*m_pdHist[i];
	}
	return counts;
}
void CLinearHistogramDouble::expandRange(double x){//expand the range of the histogram to accomodate the position x
	CLinearHistogramDouble newHist;
	double dMin=m_dBase+0.5*m_dDelta;
	double dMax=m_dBase+(m_nDim-1)*m_dDelta;
	if(x<dMin) dMin=x;
	if(x>dMax) dMax=x;
	newHist.update(dMin,dMax,m_dDelta);
	for(int i=0;i<m_nDim;i++){
		x=m_dBase+i*m_dDelta;
		newHist.addData(x,x+m_dDelta,m_pdHist[i]);
	}
	copyHist(newHist);
}
double CLinearHistogramDouble::getMin(){
	return m_dBase;
}
double CLinearHistogramDouble::getMax(){
	return m_dBase+(m_nDim-1)*m_dDelta;
}
double CLinearHistogramDouble::getDelta(){
	return m_dDelta;
}
void CLinearHistogramDouble::mergeHist(CLinearHistogramDouble hist){
	double dMin=hist.getMin();
	double dMax=hist.getMax();
	if(dMin>m_dBase) dMin=m_dBase;
	if(dMax<getMax()) dMax=getMax();
	CLinearHistogramDouble newHist;
	newHist.update(dMin,dMax,m_dDelta);
	int i,size;
	double xi,xf;
	for(i=0;i<m_nDim;i++){
		xi=getPosition(i);
		if(i<m_nDim-1) 
			xf=getPosition(i+1);
		else
			xf=xi+m_dDelta;

		newHist.addData(xi,xf,m_pdHist[i]);
	}
	size=hist.getSize();
	double delta=hist.getDelta();
	for(i=0;i<size;i++){

		xi=hist.getPosition(i);
		if(i<m_nDim-1) 
			xf=hist.getPosition(i+1);
		else
			xf=xi+hist.m_dDelta;

		newHist.addData(xi,xf,hist.getCounts(i));
	}
	newHist.updateSums(m_dSum+hist.getSum(),m_dSum2+hist.getSum2());
	copyHist(newHist);
}

void CLinearHistogramDouble::transformCoordinate(double shift, double scalingFactor)
{//shiftting first then scaling. This is order dependent.
	shiftting(shift);
	scaling(scalingFactor);
}

void CLinearHistogramDouble::getCoordinateTransformations(double &shift, double &scaling)
{//cumulative effect of all coordinate transforms represented by shiftting and scaling (the same order) from the 
 //original coordinate system(m_dBase0 and m_dDelta);
	scaling=m_dDelta/m_dDelta0;
	shift=m_dBase/scaling-m_dBase0;
}

void CLinearHistogramDouble::appendHist(CLinearHistogramDouble hist)
{
	int nSize=hist.getSize();
	double delta=hist.getDelta();
	double xi,xf;

	for(int i=0;i<nSize;i++){
		xi=hist.getPosition(i);
		xf=xi+delta;
		addData(xi,xf,hist.getCounts(i));
	}
	m_dSum+=hist.getSum();
	m_dSum2+=hist.getSum2();
}

void CLinearHistogramDouble::clearHist()
{
	for(int i=0;i<m_nDim;i++){
		m_pdHist[i]=0.;
	}
}
double CLinearHistogramDouble::getMaxCount(){
	double maxCount=0.;
	for(int i=0;i<m_nDim;i++){
		if(m_pdHist[i]>maxCount) maxCount=m_pdHist[i];
	}
	return maxCount;
}
void CLinearHistogramDouble::setTitles(CString sPositionTitle, CString sCountTitle){
		m_sPositionTitle=sPositionTitle;
		m_sCountTitle=sCountTitle;
}
CString CLinearHistogramDouble::getPositionTitle(){
	return m_sPositionTitle;
}
CString CLinearHistogramDouble::getCountTitle(){
	return m_sCountTitle;
}
int CLinearHistogramDouble::getPositionPrintingLength(){
	int len=m_sPositionTitle.GetLength();
	CPrintAssist cpa;
	int nPrecision=cpa.getPrintingPrecisionF(m_dDelta,10);
	int len1=cpa.getPrintingLengthF(m_dBase+m_nDim*m_dDelta,nPrecision);
	if(len1>len) len=len1;
	int len2=cpa.getPrintingLengthF(m_dBase,nPrecision);
	if(len2>len) len=len2;
	return len;
}
int CLinearHistogramDouble::getCountPrintingLength(){
	int len=m_sCountTitle.GetLength();
	int nPrecision=4;
	CPrintAssist cpa;
	int len1=cpa.getPrintingLengthF(m_dCounts,nPrecision);
	if(len1>len) len=len1;
	return len;
}
int CLinearHistogramDouble::getPositionPrecision(){
	CPrintAssist cpa;
	int nMaxPrecision=8;
	int nPrecision=cpa.getPrintingPrecisionF(m_dDelta,nMaxPrecision);
	return nPrecision;
}

int CLinearHistogramDouble::getCountPrecision(){
	int nPrecision=4;
	return nPrecision;
}

double CLinearHistogramDouble::getSum(){
	return m_dSum;
}
double CLinearHistogramDouble::getSum2(){
	return m_dSum2;
}

void CLinearHistogramDouble::getMeanSem(double &dMean, double &dSem){
	int nSize=int(m_dCounts+0.5);
	dMean=m_dSum/m_dCounts;
	double dSD=sqrt(m_dSum2/m_dCounts-dMean*dMean);
	dSem=0.;
	if(nSize>1) dSem=dSD/sqrt(m_dCounts-1);
}

int CLinearHistogramDouble::getWS(){
	return m_nWs;
}
double CLinearHistogramDouble::getDelta0(){
	return m_dDelta0;
}
double CLinearHistogramDouble::getBase0(){
	return m_dBase0;
}
int CLinearHistogramDouble::getDim0(){
	return m_nDim0;
}
int CLinearHistogramDouble::getDim(){
	return m_nDim;
}
void CLinearHistogramDouble::updateSums(double dSum, double dSum2){
	m_dSum=dSum;
	m_dSum2=dSum2;
}
int CLinearHistogramDouble::getMode(){
	return m_nMode;
}
