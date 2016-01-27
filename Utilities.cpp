#include "stdafx.h"
#include "Utilities.h"

CUtilities::CUtilities(void)
{
}

CUtilities::~CUtilities(void)
{
}
void Correlation_Spear(vector <double> dv1, vector <double> dv2, double &d, double &zd, double &probd, double &rs, double &probrs){
//	DP d, zd, probd, rs, probrs;
	int nRows=dv1.size();
	int i;
	Vec_DP data1(nRows),data2(nRows); //double nr array
	for(i=0;i<nRows;i++)
	{
		data1[i]=dv1[i];
	}

	rs=0.;
	probrs=-1.;
	probd=-1.;
	zd=0.;
	d=0.;

	for(i=0;i<nRows;i++)
	{
		data2[i]=dv2[i];
	}

	NR::spear(data1, data2, d, zd, probd, rs, probrs);
}
void Correlatoin_Pearsn(vector <double> dv1, vector <double> dv2, double &zd, double &probd, double &rs){
//	DP d, zd, probd, rs, probrs;
	int nRows=dv1.size();
	int i;
	Vec_DP data1(nRows),data2(nRows); //double nr array
	for(i=0;i<nRows;i++)
	{
		data1[i]=dv1[i];
	}

	rs=0.;
	probd=-1.;
	zd=0.;

	for(i=0;i<nRows;i++)
	{
		data2[i]=dv2[i];
	}

	NR::pearsn(data1,data2,rs,probd,zd);
}
