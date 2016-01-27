// HouseValues.cpp: implementation of the CHouseValues class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "Abf_Processing.h"
#include "HouseValues.h"
#include "math.h"
#include "FileNameHandling.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CHouseValues::CHouseValues()
{

}

CHouseValues::~CHouseValues()
{

}

void CHouseValues::MorgageComputer()
{
	init();
	float x,x0=0.,x1=m_fLoanAmount;
	x=m_fLoanAmount/m_nMonths;
	m_fLoanPaymentM=x;
	computeBalance();
	float fBalance=m_pfBalance[m_nMonths-1];
	while(fabs(x-x0)>0.01){
		if(fBalance>0)
			x0=x;
		else
			x1=x;
		x=0.5*(x0+x1);
		if(fabs(x0-x1)<1){
			x=x;
		}
		m_fLoanPaymentM=x;
		computeBalance();
		fBalance=m_pfBalance[m_nMonths-1];
	}
	computeBreakdown();
}

void CHouseValues::computeBalance()
{
	float fPMIC=0.8*m_fLoanAmount;//80% of the loan amount
	int i;
	float fBalance=m_fLoanAmount;
	float fInterest,fPMI;
	for(i=0;i<m_nMonths;i++){
		fInterest=fBalance*m_fInterestRate;
		fBalance=fBalance+fInterest-m_fLoanPaymentM;
		m_pfBalance[i]=fBalance;
	}
}
/*
void CHouseValues::computeBalance()
{
	float fPMIC=0.8*m_fLoanAmount;//80% of the loan amount
	int i;
	float fBalance=m_fLoanAmount;
	float fInterest,fPMI;
	for(i=0;i<m_nMonths;i++){
		fInterest=fBalance*m_fInterestRate;
		if(fBalance>fPMIC)
			fPMI=m_fLoanAmount*m_fPMIRate;
		else
			fPMI=0;
		fBalance=fBalance+fInterest+fPMI-m_fLoanPaymentM;
		m_pfBalance[i]=fBalance;
	}
}*/

void CHouseValues::init()
{
	m_nMonths=m_nLoanDuration*12;
	m_fLoanAmount=m_fPrice*(1-m_fDownPay);
	m_pfBalance=new float[m_nMonths];
	m_pfInterest=new float[m_nMonths+1];
	m_pfPMI=new float[m_nMonths+1];
	m_pfPropertyTax=new float[m_nMonths+1];
	m_pfMortgage=new float[m_nMonths+1];
	m_fLoanAmount*=1.+m_fhaUFMIP;//FHA UFMIP (FHA up front mortgage insurance premium)
}

void CHouseValues::exportMortgageStatus(CString sOutputFileName)
{
	FILE* fp=fopen(sOutputFileName,"w");
	int i;
	float fPrinciple, fEquity,fTaxBenifit,fTotalTaxBenifit=0.,fNetCosts;
	float fPMI=m_fLoanAmount*m_fPMIRate;
	float fTotalInterest=0.,fTotalCosts=m_fPrice*m_fClosingCost;
	fprintf(fp,"Month  LoanPayment       PMI       Tax   Payment");
	fprintf(fp,"   Interest Principle     Balance      Equity");
	fprintf(fp,"  TaxBenifits  TotalInterest TotalTaxBenifit  TotalCosts");
	fprintf(fp,"     NetCost    EQV_Rent\n");
	float fPayment=m_fLoanPaymentM+fPMI+m_fPrice*m_fPropertyTaxRate+m_fHOA+m_fMaintenance+m_fExtraUtility+m_fHomeInsurance+m_fRentingIncome+m_fExtraPayment;
	for(i=0;i<m_nMonths;i++){
		if(m_pfBalance[i]<0) break;
		fPrinciple=m_fLoanPaymentM-m_pfInterest[i];
		fEquity=m_fLoanAmount-m_pfBalance[i]+m_fPrice*m_fDownPay;
		fprintf(fp,"%5d%13.2f%10.2f%10.2f%10.2f",i+1,m_fLoanPaymentM,fPMI,m_pfPropertyTax[i],fPayment);
		fprintf(fp,"%11.2f%10.2f%12.2f%12.2f",m_pfInterest[i],fPrinciple,m_pfBalance[i],fEquity);
		fTaxBenifit=(m_pfInterest[i]+m_pfPropertyTax[i])*m_fTaxRate;
		fTotalTaxBenifit+=fTaxBenifit;
		fTotalCosts+=fPayment;
		fTotalInterest+=m_pfInterest[i];
		fprintf(fp,"%13.2f%15.2f%16.2f%12.2f",fTaxBenifit,fTotalInterest,fTotalTaxBenifit,fTotalCosts);
		fNetCosts=fTotalCosts-fTotalTaxBenifit-fEquity;
		fprintf(fp,"%12.2f%12.2f\n",fNetCosts,fNetCosts/(i+1));
	}
	fclose(fp);
}

void CHouseValues::updateParameters(CString sInputFileName)
{
//	m_sInputFileName=sInputFileName;
}

void CHouseValues::ComputeHouseValue(CString sInputFileName)
{
	importParameters(sInputFileName);
	MorgageComputer();
	CFileNameHandling fnh;
	CString sOutputFileName=fnh.ExtendFileName(sInputFileName,"-output");
	exportMortgageStatus(sOutputFileName);
}

void CHouseValues::importParameters(CString sFileName)
{
	CString sTemp;
	FILE* fp=fopen(sFileName,"r");
	fscanf(fp,"%s%f%s%f%s%f%s%d",sTemp,&m_fPrice,sTemp,&m_fDownPay,sTemp,&m_fClosingCost,sTemp,&m_nLoanDuration);
	fscanf(fp,"%s%f%s%f%s%f%s%f%s%f",sTemp,&m_fInterestRate,sTemp,&m_fPropertyTaxRate,sTemp,&m_fPMIRate,sTemp,&m_fTaxRate,sTemp,&m_fhaUFMIP);
	fscanf(fp,"%s%f%s%f%s%f%s%f",sTemp,&m_fHOA,sTemp,&m_fExtraUtility,sTemp,&m_fMaintenance,sTemp,&m_fHomeInsurance);
	fscanf(fp,"%s%f%s%f%",sTemp,&m_fExtraPayment,sTemp,&m_fRentingIncome);
	m_fInterestRate/=12.;
	m_fPMIRate/=12.;
	m_fPropertyTaxRate/=12.;
}

void CHouseValues::computeBreakdown()
{
	float fPMIC=0.8*m_fLoanAmount;//80% of the loan amount
	int i;
	float fBalance=m_fLoanAmount;
	float fInterest,fPMI=m_fLoanAmount*m_fPMIRate;
	float fTotalInterest=0.,fTotalPMI=0.,fTotalTax=0.;
	float fPropertyTax=m_fPropertyTaxRate*m_fPrice;
	float fTotalPayment=0.;
	for(i=0;i<m_nMonths;i++){
		fInterest=fBalance*m_fInterestRate;
		fBalance=fBalance+fInterest-m_fLoanPaymentM-m_fExtraPayment;
		m_pfBalance[i]=fBalance;
		m_pfInterest[i]=fInterest;
		m_pfPropertyTax[i]=fPropertyTax;
		fTotalTax+=fPropertyTax;
		fTotalInterest+=fInterest;
		fTotalPMI+=fPMI;
		fTotalPayment+=fPMI+fInterest+m_fLoanPaymentM+fPropertyTax;
		m_pfMortgage[i]=fTotalPayment;
	}
	m_pfInterest[m_nMonths]=fTotalInterest;
	m_pfPMI[m_nMonths]=fTotalPMI;
	m_pfPropertyTax[m_nMonths]=fTotalTax;
	m_pfMortgage[m_nMonths]=fTotalPayment;
	float fMonthlyPayment=m_fLoanPaymentM+fPropertyTax;
}
/*
void CHouseValues::computeBreakdown()
{
	float fPMIC=0.8*m_fLoanAmount;//80% of the loan amount
	int i;
	float fBalance=m_fLoanAmount;
	float fInterest,fPMI;
	float fTotalInterest=0.,fTotalPMI=0.,fTotalTax=0.;
	float fPropertyTax=m_fPropertyTaxRate*m_fPrice;
	float fTotalPayment=0.;
	for(i=0;i<m_nMonths;i++){
		fInterest=fBalance*m_fInterestRate;
		if(fBalance>fPMIC)
			fPMI=m_fLoanAmount*m_fPMIRate;
		else
			fPMI=0;
		fBalance=fBalance+fInterest+fPMI-m_fLoanPaymentM;
		m_pfBalance[i]=fBalance;
		m_pfInterest[i]=fInterest;
		m_pfPMI[i]=fPMI;
		m_pfPropertyTax[i]=fPropertyTax;
		fTotalTax+=fPropertyTax;
		fTotalInterest+=fInterest;
		fTotalPMI+=fPMI;
		fTotalPayment+=fPMI+fInterest+m_fLoanPaymentM+fPropertyTax;
		m_pfMortgage[i]=fTotalPayment;
	}
	m_pfInterest[m_nMonths]=fTotalInterest;
	m_pfPMI[m_nMonths]=fTotalPMI;
	m_pfPropertyTax[m_nMonths]=fTotalTax;
	m_pfMortgage[m_nMonths]=fTotalPayment;
	float fMonthlyPayment=m_fLoanPaymentM+fPropertyTax;
}*/
