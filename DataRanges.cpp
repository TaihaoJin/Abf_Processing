#include "stdafx.h"
#include "DataRanges.h"

CDataRanges::CDataRanges(void)
{
//	m_stMinShort=–32768;
	m_stMinShort=SHRT_MIN;
	m_stMaxShort=32767;
	m_ustMaxUST=65535;
	
//	m_nMinInt=–2147483648;
	m_nMinInt=INT_MIN;
	m_nMaxInt=2147483647;
	m_nMaxUInt=4294967295;
//	m_lMinLong=–2147483648;
	m_lMinLong=LONG_MIN;
	m_lMaxLong=2147483647;
	m_lMaxULong=4294967295;
}

CDataRanges::~CDataRanges(void)
{
}

bool CDataRanges::IntRange(double dV){
	return (dV>=INT_MIN&&dV<=INT_MAX);
}
