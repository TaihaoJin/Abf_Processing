// HouseValues.h: interface for the CHouseValues class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_HOUSEVALUES_H__C3BE5881_F316_4DB6_ACD5_D3BA9C53FF89__INCLUDED_)
#define AFX_HOUSEVALUES_H__C3BE5881_F316_4DB6_ACD5_D3BA9C53FF89__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CHouseValues  
{
public:
	void importParameters(CString sFileName);
	void ComputeHouseValue(CString sInputFileName);
	void updateParameters(CString sInputFileName);
	void exportMortgageStatus(CString sFileName);
	CHouseValues();
	virtual ~CHouseValues();
protected:
	void computeBreakdown();
	void init();
	void computeBalance();
	void MorgageComputer();
	CString m_sInputFilaName;
	float m_fPMIRate;
	float m_fPrice;//in dollars
	float m_fDownPay;//in proportion
	float m_fClosingCost;//in proportion
	float m_fInterestRate;//in proortion
	float m_fHOA;//dollar per month
	float m_fExtraUtility;//dollar per month
	float m_fMaintenance;//dollar per month
	float m_fTaxRate;
	float m_fHomeInsurance;//dollar per mont;
	float m_fPropertyTaxRate;//in proportion
	float m_fRentingIncome;//dollar per month
	int   m_nMonths;//number of months after purchasing
	float m_fEquity;//in dollar
	int	  m_nLoanDuration;//in year
	float m_fhaUFMIP;
	float *m_pfBalance;//balance at the end of each month
	float m_fLoanPaymentM;//monthly payment goes to the loan.
	float m_fMonthlyPayment;//Total monthly payment
	float m_fLoanAmount;//in dollar
	float *m_pfInterest;
	float *m_pfPMI;
	float *m_pfPropertyTax;
	float *m_pfMortgage;
	float m_fExtraPayment;
};

#endif // !defined(AFX_HOUSEVALUES_H__C3BE5881_F316_4DB6_ACD5_D3BA9C53FF89__INCLUDED_)
