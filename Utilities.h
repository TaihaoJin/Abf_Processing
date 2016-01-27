#pragma once
#include "nr.h"
#include <vector>
using namespace std;
class CUtilities
{
public:
	CUtilities(void);
	~CUtilities(void);
	void Correlatoin_Pearsn(vector <double> dv1, vector <double> dv2, double &zd, double &probd, double &rs);
	void Correlation_Spear(vector <double> dv1, vector <double> dv2, double &d, double &zd, double &probd, double &rs, double &probrs);
};
