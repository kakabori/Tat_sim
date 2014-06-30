
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
#include "nr.h"
using namespace std;

#include "SIMtoEXP.h"
#include "scaling.h"
#include "fourier.h"

/************************************************************************/

/************************ scaling ***************************************/
double scaling(double *Yexp,double *Eexp,double *Ysim,int N,double *scale,double *bckgrnd,int SCfix,int BGfix){
	double SC=*scale,BG=*bckgrnd;
	double a11=0,a12=0,a22=0,b1=0,b2=0;
	for (int i=0; i<N; i++){
		a11+=Yexp[i]*Yexp[i]/Eexp[i]/Eexp[i];
		a12+=Yexp[i]/Eexp[i]/Eexp[i];
		a22+=1./Eexp[i]/Eexp[i];
		b1+=Yexp[i]*Ysim[i]/Eexp[i]/Eexp[i];
		b2+=Ysim[i]/Eexp[i]/Eexp[i];
	}
	double denominator=a12*a12-a11*a22;
	int Nfix = 0;
	if (!SCfix && BGfix) {
		SC=(a11==0?1.0:(b1-BG*a12)/a11);
		Nfix=1;
	}
	if (SCfix && !BGfix) {
		BG=(a22==0?1.0:(b2-SC*a12)/a22);
		Nfix=1;
	}
	if (!SCfix && !BGfix) {
		SC=(denominator==0?1.0:(a12*b2-a22*b1)/denominator);
		BG=(denominator==0?1.0:(a12*b1-a11*b2)/denominator);
		Nfix=2;
	}
	*scale=SC;
	*bckgrnd=BG;
	double tmpchi=0.;
	for (int i=0; i<N; i++){
		Yexp[i] = SC*Yexp[i]+BG;
		tmpchi+=(Ysim[i]-Yexp[i])*(Ysim[i]-Yexp[i])/Eexp[i]/Eexp[i];
	}
	tmpchi/=( (N-Nfix)<2 ? 1. : (N-Nfix) );
	return(tmpchi);
}

int NK_scaling(Tcl_Interp *interp, int objc, Tcl_Obj *const objv[],
			   Blt_Vector *qFExblt[], Blt_Vector *xFEblt[], Blt_Vector **eFExblt,
			   Blt_Vector *qFEnblt[], Blt_Vector *nFEblt[], Blt_Vector **eFEnblt,
			   Blt_Vector *xBLT, Blt_Vector *yed, Blt_Vector *yaff[], Blt_Vector *ynsld){
	char *kind,*scale;
	int type;
	if(objc<4) return TCL_OK;
	scale=Tcl_GetString(objv[1]);
	kind=Tcl_GetString(objv[2]);
	Tcl_GetIntFromObj(interp,objv[3],&type);
//	cout << kind <<" "<< type << endl;

	char cmd[44];
	sprintf(cmd,"%cWat",kind[0]);
//	cout << cmd;
	double wat = atof(Tcl_GetVar(interp,cmd,1));
//	cout << wat;
	int SCfix=1,BGfix=1;
	if (scale[0]=='y'){
		sprintf(cmd,"%cSCfix(%d)",kind[0],type);
		SCfix = atoi(Tcl_GetVar(interp,cmd,1));
		sprintf(cmd,"%cBGfix(%d)",kind[0],type);
		BGfix = atoi(Tcl_GetVar(interp,cmd,1));
	}
	sprintf(cmd,"%cscale(%d)",kind[0],type);
	double SC = atof(Tcl_GetVar(interp,cmd,1));
	sprintf(cmd,"%cbckgrnd(%d)",kind[0],type);
	double BG = atof(Tcl_GetVar(interp,cmd,1));
//	cout << wat <<" "<< SCfix <<" "<< BGfix <<" "<< SC <<" "<< BG << endl;

	FourierTransform Fourier;
	double *Yexp,*Eexp,*Ytmp;
	int Nexp=0;
	Blt_Vector *FEblt,*eFEblt;
	Fourier.x=xBLT->valueArr;
	Fourier.water=wat;
	switch (kind[0]){
		case 'x': 
			Fourier.Nq = Nexp = qFExblt[type]->numValues;
			Fourier.q=qFExblt[type]->valueArr;
			Fourier.sd=yed->valueArr;
			FEblt=xFEblt[type];
			eFEblt=eFExblt[type];
			break;
		case 'n': 
			Fourier.Nq = Nexp = qFEnblt[type]->numValues;
			Fourier.q=qFEnblt[type]->valueArr;
			Fourier.sd=ynsld->valueArr;
			FEblt=nFEblt[type];
			eFEblt=eFEnblt[type];
			break;
	}
	Ytmp=(double*)calloc(Nexp,sizeof(double));
	Fourier.FF=Ytmp;
	int left_margin = atoi(Tcl_GetVar(interp,"left_margin",1));
	int right_margin = atoi(Tcl_GetVar(interp,"right_margin",1));
	if (kind[0]=='x' && atoi(Tcl_GetVar(interp,"atomf",1))){
		atomff aff[MaxAFF];
		int nid=Fourier.AFFcorrection(interp,yaff,aff);
		Fourier.fourier(nid,aff,left_margin,right_margin);
	} else {
		Fourier.fourier(left_margin,right_margin);
	}
	Yexp=FEblt->valueArr;
	Eexp=eFEblt->valueArr;
	double chi2 = scaling(Yexp,Eexp,Ytmp,Nexp,&SC,&BG,SCfix,BGfix);
	
	char cmd2[44];
	sprintf(cmd2,"%.3g",SC);
	sprintf(cmd,"%cscale(%d)",kind[0],type);
	Tcl_SetVar(interp,cmd,cmd2,1);
	sprintf(cmd2,"%.3g",BG);
	sprintf(cmd,"%cbckgrnd(%d)",kind[0],type);
	Tcl_SetVar(interp,cmd,cmd2,1);
	sprintf(cmd2,"%.3g",chi2);
	sprintf(cmd,"%cCHI2(%d)",kind[0],type);
	Tcl_SetVar(interp,cmd,cmd2,1);

	Blt_ResetVector(FEblt,Yexp,Nexp,MaxFF, TCL_VOLATILE);
	free(Ytmp);
	return TCL_OK;
}
/************************************************************************/
