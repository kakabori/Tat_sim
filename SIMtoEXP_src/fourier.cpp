
#include <cmath>
#include <fstream>
#include <iostream>
#include <sstream>
#include <stdio.h>
#include <string>
#include <tcl.h>
#include <tk.h>
#include <blt.h>
#include <cstdlib>
using namespace std;

#include "SIMtoEXP.h"
#include "fourier.h"

/************************************************************************/
int sign(double x){
  int t=1;
  if (x<0.0) t=-1;
  return t;
}
/************************************************************************/

/************************************************************************/
/********************** Fourier Transform *******************************/
double atomff::_atomff(double q){
	double fi=0.0;
	for (int i=0; i<4; i++){
		fi+=a[i]*exp(-b[i]*q*q/157.91367041742973790135185599802 /* (4*pi)^2 */ );
	}
	fi+=c;
	return fi;
}

int FourierTransform::interval(){
	Nq=int((q1-q0)/qstep);
	for (int i=0;i<Nq;i++){
		q[i]=q0+i*qstep;
	}
	return(Nq);
}

int FourierTransform::AFFcorrection(Tcl_Interp *interp,Blt_Vector *yaffBlt[],atomff *aff){
	int nid=atoi(Tcl_GetVar(interp,"nid",1));
	char cmd[10];
	for (int i=0; i<nid; i++) {
		yaff[i]=yaffBlt[i]->valueArr;
		sprintf(cmd,"ffa1(%d)",i);
		aff[i].a[0] = atof(Tcl_GetVar(interp,cmd,1));
		sprintf(cmd,"ffa2(%d)",i);
		aff[i].a[1] = atof(Tcl_GetVar(interp,cmd,1));
		sprintf(cmd,"ffa3(%d)",i);
		aff[i].a[2] = atof(Tcl_GetVar(interp,cmd,1));
		sprintf(cmd,"ffa4(%d)",i);
		aff[i].a[3] = atof(Tcl_GetVar(interp,cmd,1));
		sprintf(cmd,"ffb1(%d)",i);
		aff[i].b[0] = atof(Tcl_GetVar(interp,cmd,1));
		sprintf(cmd,"ffb2(%d)",i);
		aff[i].b[1] = atof(Tcl_GetVar(interp,cmd,1));
		sprintf(cmd,"ffb3(%d)",i);
		aff[i].b[2] = atof(Tcl_GetVar(interp,cmd,1));
		sprintf(cmd,"ffb4(%d)",i);
		aff[i].b[3] = atof(Tcl_GetVar(interp,cmd,1));
		sprintf(cmd,"ffc(%d)",i);
		aff[i].c = atof(Tcl_GetVar(interp,cmd,1));
	}
	return(nid);
}

int FourierTransform::fourier(int left_margin, int right_margin){
	for (int i=0;i<Nq;i++){
		double Re=0,Im=0;
		double dx;
		for (int j=left_margin;j<=right_margin;j++){
			if (j<right_margin) dx=x[j+1]-x[j];
			Re+=(sd[j]-water)*cos(q[i]*x[j])*dx;
			if (!symmetrized) Im+=(sd[j]-water)*sin(q[i]*x[j])*dx;
		}
		FF[i]=/*sign(Re)**/sqrt(Re*Re+Im*Im);
//		cout << q[i] <<" "<< FF[i] << endl;
	}
	return(0);
}

int FourierTransform::fourier(int nid,atomff aff[],int left_margin, int right_margin){
	for (int i=0;i<Nq;i++){
		double Re=0,Im=0;
		double dx;
		double fi[MaxAFF];
		for (int k=0; k<nid; k++){
			fi[k] = aff[k]._atomff(q[i]);
		}
		for (int j=left_margin;j<=right_margin;j++){
			if (j<right_margin) dx=x[j+1]-x[j];
			double sdj=0;
			for (int k=0; k<nid; k++) sdj += yaff[k][j]*fi[k];
			Re+=(sdj-water)*cos(q[i]*x[j])*dx;
			if (!symmetrized) Im+=(sdj-water)*sin(q[i]*x[j])*dx;
		}
		FF[i]=/*sign(Re)**/sqrt(Re*Re+Im*Im);
//		cout << q[i] <<" "<< FF[i] << endl;
	}
	return(0);
}

int NK_fourierX(Tcl_Interp *interp, Blt_Vector *xBLT, Blt_Vector *yed,
			   Blt_Vector *yaff[], Blt_Vector *qFFxblt, Blt_Vector *xFFblt){
	char cmd[44];
	int Np=xBLT->numValues;
	int left_margin = atoi(Tcl_GetVar(interp,"left_margin",1));
	int right_margin = atoi(Tcl_GetVar(interp,"right_margin",1));
	sprintf(cmd,"create fftX fourier 0");
    Tcl_EvalEx(interp,cmd,-1,TCL_EVAL_GLOBAL);
	FourierTransform Fourier;
	Fourier.symmetrized = atoi(Tcl_GetVar(interp,"symmetrized",1));
	Fourier.q0 = atof(Tcl_GetVar(interp,"xq0",1));
	Fourier.q1 = atof(Tcl_GetVar(interp,"xq1",1));
	Fourier.qstep = atof(Tcl_GetVar(interp,"xqstep",1));
	Fourier.water = atof(Tcl_GetVar(interp,"xWat",1));
	Fourier.x=xBLT->valueArr;
	Fourier.sd=yed->valueArr;
	Fourier.q=qFFxblt->valueArr;
	Fourier.FF=xFFblt->valueArr;
	int Nqx=Fourier.interval();
	int error=0;
	int nid=0;
	if (atoi(Tcl_GetVar(interp,"atomf",1))){
		atomff aff[MaxAFF];
		nid=Fourier.AFFcorrection(interp,yaff,aff);
		error=Fourier.fourier(nid,aff,left_margin,right_margin);
	} else {
		error=Fourier.fourier(left_margin,right_margin);
	}
//	for (int i=0; i<Nq; i++) cout << Fourier[0].q[i] <<" "<< Fourier[0].FF[i] << endl;

	Blt_ResetVector(qFFxblt,Fourier.q,Nqx,MaxFF, TCL_VOLATILE);
	Blt_ResetVector(xFFblt,Fourier.FF,Nqx,MaxFF, TCL_VOLATILE);
	return TCL_OK;
}
int NK_fourierN(Tcl_Interp *interp, Blt_Vector *xBLT, Blt_Vector *ynsld,
			   Blt_Vector *qFFnblt, Blt_Vector *nFFblt){
	char cmd[44];
	int Np=xBLT->numValues;
	int left_margin = atoi(Tcl_GetVar(interp,"left_margin",1));
	int right_margin = atoi(Tcl_GetVar(interp,"right_margin",1));
	sprintf(cmd,"create fftN fourier 0");
    Tcl_EvalEx(interp,cmd,-1,TCL_EVAL_GLOBAL);
	FourierTransform Fourier;
	Fourier.symmetrized = atoi(Tcl_GetVar(interp,"symmetrized",1));
	Fourier.q0 = atof(Tcl_GetVar(interp,"nq0",1));
	Fourier.q1 = atof(Tcl_GetVar(interp,"nq1",1));
	Fourier.qstep = atof(Tcl_GetVar(interp,"nqstep",1));
	Fourier.water = atof(Tcl_GetVar(interp,"nWat",1));
	Fourier.x=xBLT->valueArr;
	Fourier.sd=ynsld->valueArr;
	Fourier.q=qFFnblt->valueArr;
	Fourier.FF=nFFblt->valueArr;
	int Nqn=Fourier.interval();
	int error=Fourier.fourier(left_margin,right_margin);
	Blt_ResetVector(qFFnblt,Fourier.q,Nqn,MaxFF, TCL_VOLATILE);
	Blt_ResetVector(nFFblt,Fourier.FF,Nqn,MaxFF, TCL_VOLATILE);
	return TCL_OK;
}
/************************************************************************/
