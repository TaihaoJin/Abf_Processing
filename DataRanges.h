#pragma once

class CDataRanges
{
public:
	CDataRanges(void);
	/*
	short SHRT_MIN;
	short SHRT_MAX;
	unsigned short USHRT_MAX;
	int INT_MIN;
	int INT_MAX;
	unsigned int UINT_MAX;
	long LONG_MIN;
	long LONG_MAX;
	unsigned long ULONG_MAX;
	*/
	short m_stMinShort;
	short m_stMaxShort;
	unsigned short m_ustMaxUST;
	int m_nMinInt;
	int m_nMaxInt;
	unsigned int m_nMaxUInt;
	long m_lMinLong;
	long m_lMaxLong;
	unsigned long m_lMaxULong;
public:
	~CDataRanges(void);
	bool CDataRanges::IntRange(double dV);
};
