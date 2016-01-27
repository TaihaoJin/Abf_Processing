#include "stdafx.h"
#include "PrintAssist.h"
#include "math.h"

CPrintAssist::CPrintAssist(void)
{
}

CPrintAssist::~CPrintAssist(void)
{
}
CString CPrintAssist::ToString(int n)
{
	CString s;
	int index=0;
	if(n<0){
		s+="-";
		n*=-1;
		index++;
	}	
	int digits=getDigits(n);
	int it;
	char ct;
	for(int i=0;i<digits;i++){
		it=n%10;
		n/=10;
		ct=it+48;
		s.Insert(index,ct);
	}
	return s;
}

CString CPrintAssist::ToString(double dNum, int nPrecision)
{
	CString s;
	if(dNum<0){
		s+="-";
		dNum*=-1;
	}

	int it;
	int i,j,n;
	double dF=0.5;
	for(i=0;i<nPrecision;i++){
		dF/=10.;
	}
	dNum+=dF;
	double dV=dNum,dV0;
	int nd=getDigits(dV),nd1;
	if(nd>0){
		vector <double> dVs;
		vector <char> chars;
		double dV=dNum;
		for(i=0;i<nd;i++){
			dVs.push_back(dV);
			dV/=10.;
		}
		dV0=0;
		n=0;
		char c;
		for(i=0;i<nd;i++){
			it=nd-1-i;
			dV=dVs[it]-dV0;
			n=int(dV);
			dV-=n;
			dNum=dV;
			s+=ToString(n);
			dV0+=n;
			dV0*=10;
		}

	}else{
		s+="0";
	}

	if(nPrecision>0){
		s+=".";
		for(i=0;i<nPrecision;i++){
			dNum*=10;
			it=int(dNum);
			dNum-=it;
			s+=ToString(it);
		}
	}
	return s;
}


int CPrintAssist::getDigits(int n)
{
	int digits=1;
	n/=10;
	while(n!=0){
		digits++;
		n/=10;
	}
	return digits;
}
int getPrecision(double dV){
	int np=0;
	return np;
}

void CPrintAssist::printString(FILE* fp, CString sT, int nW){
	int len=sT.GetLength();
	int spaces=nW-len;
	int i;
	for(i=0;i<spaces;i++){
		fprintf(fp,"%c",' ');
	}
	for(i=0;i<len;i++){
		fprintf(fp,"%c",sT[i]);
	}
}
void CPrintAssist::printNumber(FILE* fp, double dNum, int nWidth, int nPrecision){
	CString sT=ToString(dNum,nPrecision);
	printString(fp,sT,nWidth);
}
void CPrintAssist::endLine(FILE* fp){
	fprintf(fp,"\n");
}
void CPrintAssist::IntegerReader(CString st, vector<int> &vnData)
{
	vnData.clear();
	int len=st.GetLength(),length;
	int i,j,digits,factor,it;
	char c,c0;
	int index=0;
	bool bIntegerRead=false;
	length=0;
	c0=st[0];
	vector <int> vnDigits,keys;
	while(index<len){
		vnDigits.clear();
		bIntegerRead=false;
		c=st[index];
		while(c<'0'||c>'9'){
			index++;
			if(index>=len) break;
			c0=c;
			c=st[index];
		}
		digits=0;
		while(c>='0'&&c<='9'){
			if(!bIntegerRead) bIntegerRead=true;
			vnDigits.push_back(c-'0');
			digits++;
			index++;
			if(index>=len) break;
			c=st[index];
		}	
		if(bIntegerRead){
			factor=1;
			it=0;
			for(i=0;i<digits;i++){
				j=digits-1-i;
				it+=factor*vnDigits[j];
				factor*=10;
			}
			if(c0=='-') it=-it;
			vnData.push_back(it);
			length++;
		}
	}
}
int CPrintAssist::getDigits(double dNum){
	int nMaxIter=10, nIter;
	double dV=fabs(dNum);
	int nd=0;
	if(dV>1.){
		nIter=0;
		while(dV>1&&nIter<nMaxIter){
			nd++;
			dV/=10.;
			nIter++;
		}
	}else if(dV<1.){
		nIter=0;
		while(dV<1.&&nIter<nMaxIter){
			nd--;
			nIter++;
			dV*=10.;
		}
	}else{
		nd=1;
	}
	return nd;
}
int CPrintAssist::getPrintingLengthF(double dV, int nPrecision){
	int len=getDigits(int(dV));
	if(dV<0) len++;
	if(nPrecision>0) len+=nPrecision+1;
	return len;
}

int CPrintAssist::getPrintingPrecision(double dV, double dFraction){//to the accuracy of dFaction*dV 
	dV=fabs(dV)/dFraction;
	if(dV>1) return 0;
	int nd=getDigits(dV);
	return abs(nd);
}

double CPrintAssist::round(double dV, int nPrecision){
	int sign=1;
	if(dV<0) sign=-1;
	dV=fabs(dV);
	int factor=1;
	for(int i=0;i<nPrecision;i++){
		factor*=10;
	}
	dV*=factor;
	if((dV-int(dV))>=0.5) dV+=1.;
	dV/=factor;
	return dV;
}

int CPrintAssist::getPrintingPrecisionF(double dV, int nMaxPrecision){//
	dV=fabs(dV);
	int nd=0;
	int nCon=2;//nCon consecutive 0 or 9 
	while(dV<1.)
	{
		dV*=10;
		nd++;
		if(nd>nMaxPrecision) return nd;
	}
	int dig0,dig1;
	dig0=int(dV);
	dV-=dig0;
	dV*=10;
	nd++;
	dig1=int(dV);
	dV-=dig1;
	while(true){
		if(dig0==0&&dig1==0)
			return nd-2;
		if(dig0==9&&dig1==9)
			return nd-2;
		if(nd>nMaxPrecision) break;
		dV*=10;
		nd++;
		dig0=dig1;
		dig1=int(dV);
		dV-=dig1;
	}
	return nd;
}
bool CPrintAssist::StringDelimiter(char ch, bool &bQuotationOpened){
	if(bQuotationOpened&&ch=='"') return true;
	if(ch==' '&&!bQuotationOpened) return true;
	if(ch==10&&!bQuotationOpened) return true;
	return false;
}
CString CPrintAssist::readString(FILE* fp){
	vector <char> chars;
	char ch;
	bool bQuotationOpened=false;
	fscanf(fp,"%c",&ch);
	while(StringDelimiter(ch,bQuotationOpened)){
		if(feof(fp)) break;
		fscanf(fp,"%c",&ch);
	}//ch is the first non-delimiter character
	if(ch=='"') {
		bQuotationOpened=true;
		fscanf(fp,"%c",&ch);
	}
	while(!StringDelimiter(ch,bQuotationOpened)){
		if(feof(fp)) break;
		chars.push_back(ch);
		fscanf(fp,"%c",&ch);
	}
	int nSize=chars.size();
	CString st;
	st.GetBuffer(nSize+1);
	for(int i=0;i<nSize;i++){
		st+=chars[i];
	}
	st.ReleaseBuffer();
	return st;
}

CString CPrintAssist::ToStringScientific(double dValue, int nPrecision){
	int nd=getDigits(dValue);
	int nShift=0;
	double dF=.1;
	int nDel=-1;
	if(nd<0){
		dF=10.;
		nDel=1;
		nd++;
	}
	double dV=dValue;
	while(nd!=0){
		dV*=dF;
		nShift-=nDel;
		nd+=nDel;
	}
	CString ct=ToString(dV,nPrecision);
	ct+="E";
	ct+=ToString(nShift);
	return ct;
}

void CPrintAssist::printNumberScientific(FILE* fp, double dV, int nFieldWidth, int nPrecision){
	CString st=ToStringScientific(dV,nPrecision);
	int nMinFW=st.GetLength()+1;
	if(nFieldWidth<nMinFW) nFieldWidth=nMinFW;
	printString(fp,st,nFieldWidth);
}