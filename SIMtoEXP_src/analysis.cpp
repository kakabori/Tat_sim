
#include <cmath>
#include <fstream>
#include <iostream>
#include <tcl.h>
#include <tk.h>
#include <blt.h>
using namespace std;

#include "SIMtoEXP.h"
#include "analysis.h"

/************************************************************************/

int symmetrize(double vector[], int N){
	for (int i=0; i<int(N/2); i++){
		vector[i]=vector[N-1-i]=(vector[i]+vector[N-1-i])/2.;
	}
	return 1;
}

/************************************************************************/

double integral(int from, int to, double *rhoW){
	double sum=0;
	for (int i=from;i<=to;i++) sum+=rhoW[i];
	return sum;
}

double gibbs(double x[], double y[], int ENDpoint, int direction){
	int nDG=ENDpoint/2;
	double rint, lint;
	if (direction<0){
		rint=integral(nDG,ENDpoint,y);
		lint=(nDG-0+1)-integral(0,nDG,y);
	} else {
		rint=(ENDpoint-nDG+1)-integral(nDG,ENDpoint,y);
		lint=integral(0,nDG,y);
	}
//	cout << lint <<" "<< rint <<" "<< nDG << endl;
	int high=ENDpoint,low=0;
	while (high-low>1){
		if (rint>lint) {
			low=nDG;
			nDG+=(high-nDG)/2;
		} else {
			high=nDG;
			nDG+=(low-nDG)/2;
		}
		if (direction<0){
			rint=integral(nDG,ENDpoint,y);
			lint=(nDG-0+1)-integral(0,nDG,y);
		} else {
			rint=(ENDpoint-nDG+1)-integral(nDG,ENDpoint,y);
			lint=integral(0,nDG,y);
		}
//		cout << lint <<" "<< rint <<" "<< nDG <<" "<< high <<" "<< low << endl;
	}
//	cout << x[nDG] << endl;
	if (direction<0){
		if (abs(integral(low,ENDpoint,y) - ((low-0+1)-integral(0,low,y))) < abs(integral(high,ENDpoint,y) - ((high-0+1)-integral(0,high,y)))) {
//			cout << x[low] << endl;
			return x[low];
		} else {
//			cout << x[high] << endl;
			return x[high];
		}
	} else {
		if (abs((ENDpoint-low+1)-integral(low,ENDpoint,y) - integral(0,low,y)) < abs((ENDpoint-high+1)-integral(high,ENDpoint,y) - integral(0,high,y))) {
//			cout << x[low] << endl;
			return x[low];
		} else {
//			cout << x[high] << endl;
			return x[high];
		}
	}
}

int NK_gibbs(Tcl_Interp *interp, int objc, Tcl_Obj *const objv[]){
	Blt_Vector *px, *py;
	if (Blt_GetVector(interp,"xsim",&px) != TCL_OK) return TCL_ERROR;
	if (Blt_GetVector(interp,"ycombine",&py) != TCL_OK) return TCL_ERROR;
	if(objc<4) return TCL_OK;
	double min,max;
	int direction;
	Tcl_GetDoubleFromObj(interp,objv[1],&min);
	Tcl_GetDoubleFromObj(interp,objv[2],&max);
	Tcl_GetIntFromObj(interp,objv[3],&direction);
	int i=0,j=(px)->numValues-1;
	while (px->valueArr[i]<min && i<px->numValues-1){ i++; }
	while (px->valueArr[j]>max && j>0){ j--; }
	int Npoints=j+1-i;
	double *px_new=&(px->valueArr[i]);
	double *py_new=&(py->valueArr[i]);
//	cout << min <<" "<< max <<" "<< i <<" "<< j << endl;
	char str[20];
	sprintf(str,"%lf",gibbs(px_new,py_new,Npoints-1,direction));
//	cout << str << endl;
	Tcl_SetVar(interp,"Z_gibbs",str,TCL_LEAVE_ERR_MSG);
	
    return TCL_OK;
}
/************************************************************************/