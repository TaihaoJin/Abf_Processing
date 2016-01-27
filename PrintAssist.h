
#pragma once
#include <vector>
using namespace std;

class CPrintAssist
{
public:
	void printNumberScientific(FILE* fp, double dV, int nPrecision, int nFieldWidth);
	CString ToStringScientific(double dValue, int nPrecision);
	CString readString(FILE* fp);
	CString CPrintAssist::ToString(double dNum, int nPrecision);
	CString CPrintAssist::ToString(int n);
	void printString(FILE* fp, CString sT, int nW);
	CPrintAssist(void);
	void printNumber(FILE* fp, double dT, int nWidth, int nPrecision);
	void endLine(FILE* fp);
	void IntegerReader(CString st, vector<int> &vnData);
	int getDigits(double dV);
	int getDigits(int N);
	int getPrintingLengthF(double dV, int nPrecision);
	double round(double dV, int nPrecision);
	int getPrintingPrecision(double dV, double dFraction);//to the accuracy of dFaction*dV 
	int getPrintingPrecisionF(double dV, int nMaxPrecision);//return the number significant number of digits after the decimal point
	bool StringDelimiter(char ch, bool &bQuotationOpened);
public:
	~CPrintAssist(void);
};
