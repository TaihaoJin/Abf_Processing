// NumeircalRecipes.cpp: implementation of the CNumeircalRecipes class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "Abf_Processing.h"
#include "NumeircalRecipes.h"
#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

double Trial(Vec_I_DP &x)
{
	return 0.6-NR::bessj0(SQR(x[0]-0.5)+SQR(x[1]-0.6)+SQR(x[2]-0.7));
}

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CNumeircalRecipes::CNumeircalRecipes()
{
}

CNumeircalRecipes::~CNumeircalRecipes()
{

}



double CNumeircalRecipes::func (Vec_I_DP &x)
{
	double dResult=0.6-NR::bessj0(SQR(x[0]-0.5)+SQR(x[1]-0.6)+SQR(x[2]-0.7));
	return dResult;
}

void CNumeircalRecipes::Test()
{
//        const int MP=4,NP=3;
        int MP=4,NP=3;
        const DP FTOL=1.0e-10;
        int i,nfunc,j;
        Vec_DP x(NP),y(MP);
        Mat_DP p(MP,NP);

        for (i=0;i<MP;i++) {
          for (j=0;j<NP;j++)
            x[j]=p[i][j]=(i == (j+1) ? 1.0 : 0.0);
          y[i]=Trial(x);
        }
		amoeba(p,y,FTOL,Trial,nfunc);
		FILE* fp=fopen("test.txt","w");

//        cout << endl << "Number of function evaluations: " << nfunc << endl;
        for (i=0;i<MP;i++) {
//         cout << setw(3) << i;
          for (j=0;j<NP;j++) fprintf(fp,"  %f",p[i][j]);
//          cout << setw(12) << y[i] << endl;
			fprintf(fp,"  %f\n",y[i]);
        }
//        cout << endl << "True minimum is at (0.5,0.6,0.7)" << endl;
		fclose(fp);
}

#include <cmath>
//#include "nr.h"
using namespace std;

namespace {
	inline void get_psum(Mat_I_DP &p, Vec_O_DP &psum)
	{
		int i,j;
		DP sum;

		int mpts=p.nrows();
		int ndim=p.ncols();
		for (j=0;j<ndim;j++) {
			for (sum=0.0,i=0;i<mpts;i++)
				sum += p[i][j];
			psum[j]=sum;
		}
	}
}

void CNumeircalRecipes::amoeba(Mat_IO_DP &p, Vec_IO_DP &y, const DP ftol, DP (*funk)(Vec_I_DP &), int &nfunk)
{
	const int NMAX=5000;
	const DP TINY=1.0e-10;
	int i,ihi,ilo,inhi,j;
	DP rtol,ysave,ytry;

	int mpts=p.nrows();
	int ndim=p.ncols();
	Vec_DP psum(ndim);
	nfunk=0;
	get_psum(p,psum);
	for (;;) {
		ilo=0;
		ihi = y[0]>y[1] ? (inhi=1,0) : (inhi=0,1);
		for (i=0;i<mpts;i++) {
			if (y[i] <= y[ilo]) ilo=i;
			if (y[i] > y[ihi]) {
				inhi=ihi;
				ihi=i;
			} else if (y[i] > y[inhi] && i != ihi) inhi=i;
		}
		rtol=2.0*fabs(y[ihi]-y[ilo])/(fabs(y[ihi])+fabs(y[ilo])+TINY);
		if (rtol < ftol) {
			SWAP(y[0],y[ilo]);
			for (i=0;i<ndim;i++) SWAP(p[0][i],p[ilo][i]);
			break;
		}
//		if (nfunk >= NMAX) nrerror("NMAX exceeded");
		nfunk += 2;
		ytry=NR::amotry(p,y,psum,funk,ihi,-1.0);
		if (ytry <= y[ilo])
			ytry=amotry(p,y,psum,funk,ihi,2.0);
		else if (ytry >= y[inhi]) {
			ysave=y[ihi];
			ytry=amotry(p,y,psum,funk,ihi,0.5);
			if (ytry >= ysave) {
				for (i=0;i<mpts;i++) {
					if (i != ilo) {
						for (j=0;j<ndim;j++)
							p[i][j]=psum[j]=0.5*(p[i][j]+p[ilo][j]);
						y[i]=funk(psum);
					}
				}
				nfunk += ndim;
				get_psum(p,psum);
			}
		} else --nfunk;
	}
}

DP CNumeircalRecipes::amotry(Mat_IO_DP &p, Vec_O_DP &y, Vec_IO_DP &psum, DP (__cdecl *funk)(Vec_I_DP &), const int ihi, const DP fac)
{
	int j;
	DP fac1,fac2,ytry;

	int ndim=p.ncols();
	Vec_DP ptry(ndim);
	fac1=(1.0-fac)/ndim;
	fac2=fac1-fac;
	for (j=0;j<ndim;j++)
		ptry[j]=psum[j]*fac1-p[ihi][j]*fac2;
	ytry=funk(ptry);
	if (ytry < y[ihi]) {
		y[ihi]=ytry;
		for (j=0;j<ndim;j++) {
			psum[j] += ptry[j]-p[ihi][j];
			p[ihi][j]=ptry[j];
		}
	}
	return ytry;
}

DP CNumeircalRecipes::bessj0(const DP x)
{
	DP ax,z,xx,y,ans,ans1,ans2;

	if ((ax=fabs(x)) < 8.0) {
		y=x*x;
		ans1=57568490574.0+y*(-13362590354.0+y*(651619640.7
			+y*(-11214424.18+y*(77392.33017+y*(-184.9052456)))));
		ans2=57568490411.0+y*(1029532985.0+y*(9494680.718
			+y*(59272.64853+y*(267.8532712+y*1.0))));
		ans=ans1/ans2;
	} else {
		z=8.0/ax;
		y=z*z;
		xx=ax-0.785398164;
		ans1=1.0+y*(-0.1098628627e-2+y*(0.2734510407e-4
			+y*(-0.2073370639e-5+y*0.2093887211e-6)));
		ans2 = -0.1562499995e-1+y*(0.1430488765e-3
			+y*(-0.6911147651e-5+y*(0.7621095161e-6
			-y*0.934945152e-7)));
		ans=sqrt(0.636619772/ax)*(cos(xx)*ans1-z*sin(xx)*ans2);
	}
	return ans;
}
