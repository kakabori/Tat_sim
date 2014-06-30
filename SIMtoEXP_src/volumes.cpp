
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
#include "volumes.h"

/************************************************************************/
void NR::gaussj(Mat_IO_DP &a, Mat_IO_DP &b)
{
	int i,icol,irow,j,k,l,ll;
	DP big,dum,pivinv;

	int n=a.nrows();
	int m=b.ncols();
	Vec_INT indxc(n),indxr(n),ipiv(n);
	for (j=0;j<n;j++) ipiv[j]=0;
	for (i=0;i<n;i++) {
		big=0.0;
		for (j=0;j<n;j++)
			if (ipiv[j] != 1)
				for (k=0;k<n;k++) {
					if (ipiv[k] == 0) {
						if (fabs(a[j][k]) >= big) {
							big=fabs(a[j][k]);
							irow=j;
							icol=k;
						}
					}
				}
		++(ipiv[icol]);
		if (irow != icol) {
			for (l=0;l<n;l++) SWAP(a[irow][l],a[icol][l]);
			for (l=0;l<m;l++) SWAP(b[irow][l],b[icol][l]);
		}
		indxr[i]=irow;
		indxc[i]=icol;
		if (a[icol][icol] == 0.0) nrerror("gaussj: Singular Matrix");
		pivinv=1.0/a[icol][icol];
		a[icol][icol]=1.0;
		for (l=0;l<n;l++) a[icol][l] *= pivinv;
		for (l=0;l<m;l++) b[icol][l] *= pivinv;
		for (ll=0;ll<n;ll++)
			if (ll != icol) {
				dum=a[ll][icol];
				a[ll][icol]=0.0;
				for (l=0;l<n;l++) a[ll][l] -= a[icol][l]*dum;
				for (l=0;l<m;l++) b[ll][l] -= b[icol][l]*dum;
			}
	}
	for (l=n-1;l>=0;l--) {
		if (indxr[l] != indxc[l])
			for (k=0;k<n;k++)
				SWAP(a[k][indxr[l]],a[k][indxc[l]]);
	}
}
/************************************************************************/

/******************** Volume analysis ***********************************/
int NK_Normalize(Tcl_Interp *interp, Components component[], Blt_Vector *ysim[]){
	char cmd[144];
	int Ncomp=atoi(Tcl_GetVar(interp,"Ncomponent",1));
	int Np=ysim[0]->numValues;
	for (int i=0;i<Ncomp;i++){
		sprintf(cmd,"ngr(%d)",i);
		Tcl_UpdateLinkedVar(interp,cmd);
		sprintf(cmd,"natpgr(%d)",i);
		Tcl_UpdateLinkedVar(interp,cmd);
		sprintf(cmd,"nelpgr(%d)",i);
		Tcl_UpdateLinkedVar(interp,cmd);
		sprintf(cmd,"nslpgr(%d)",i);
		Tcl_UpdateLinkedVar(interp,cmd);
		sprintf(cmd,"nel(%d)",i);
		Tcl_UpdateLinkedVar(interp,cmd);
		sprintf(cmd,"nsl(%d)",i);
		Tcl_UpdateLinkedVar(interp,cmd);
	}
	double binsize=atof(Tcl_GetVar(interp,"Nlip",1))/2.*atof(Tcl_GetVar(interp,"area",1))*atof(Tcl_GetVar(interp,"binstep",1));
	sprintf(cmd,"%lf",binsize);
	Tcl_SetVar(interp,"binsize",cmd,1);
	for (int j=0; j<Ncomp; j++){
		for (int i=0; i<Np; i++){
			component[j].ysim[i]=component[j].ysimbak[i]/component[j].natpgr;
			if (atoi(Tcl_GetVar(interp,"norm",1))==0) component[j].ysim[i]/=(binsize==0 ? 1. : binsize);
		}
        Blt_ResetVector(ysim[j],component[j].ysim,Np,MaxPoint, TCL_VOLATILE);
	}
    return TCL_OK;
}
double calculation(Mat_DP &y,Mat_DP &b,int Nfix,int Ncomp,int left_margin,int right_margin,Components component[]){
	int N=Ncomp-Nfix;
//	FILE *output;
//	output=fopen("SIMtoEXP.fit","w"); 
//    fprintf(output,"        z");
//	for (int i=0; i<Ncomp; i++) fprintf(output," %9s",component[i].name);
	int active[MaxComponent],fixed[MaxComponent];
	for (int i=0, j=0, k=0; i<Ncomp; i++){
		if (component[i].fix==0){
			active[j]=i;
			j++;
		}else{
			fixed[k]=i;
			k++;
		}
	}
//	fprintf(output,"     total\n");
	Mat_DP a(N,N),bm(N,1);
	double fit;
    for (int ia=0; ia<N; ia++){
		for (int ja=0; ja<N; ja++){
            a[ia][ja] = 0.;
			for (int k=left_margin; k<=right_margin; k++){
				a[ia][ja] += (y[active[ia]][k])*(y[active[ja]][k]);
			}
//			cout << a[ia][ja] << " ";
		}
//		cout << endl;
	}
    for (int ib=0; ib<N; ib++){
        (bm[ib][0]) = 0.;
		for (int k=left_margin; k<=right_margin; k++) (bm[ib][0 ]) += (y[active[ib]][k]);
		for (int ib2=0; ib2<Nfix; ib2++){
            for (int k=left_margin; k<=right_margin; k++){
				(bm[ib][0 ]) -= component[fixed[ib2]].volume*y[active[ib]][k]*y[fixed[ib2]][k];
			}
		}
//		cout << bm[ib][0] << " ";
	}
//	cout << endl;
	NR::gaussj(a,bm);
//    for (int ib=0; ib<N; ib++){
//		cout << (bm[ib][0]) << " ";
//	}
//	cout << endl;
	double rms = 0.;
	for (int i=0; i<N; i++)	b[active[i]][0] = bm[i][0];
	for (int k=left_margin; k<=right_margin; k++){
	    fit = 0.;
        for (int i=0; i<Ncomp; i++) fit += (b[i][0])*(y[i][k]);
        rms += (fit-1.)*(fit-1.);
//		fprintf(output,"%9.7g",xsim[k]);
//		for (int i=0; i<Ncomp; i++) fprintf(output," %9.5g",(b[i][0])*(y[i][k]));
//        fprintf(output," %9.5g\n",fit);
	}
//	fclose(output);
    rms = sqrt(rms/(right_margin-left_margin+1-N));
	return rms;
}

int NK_fit(Tcl_Interp *interp, Components component[], Blt_Vector *yprob[], int Np) {
	char cmd[144];
	if (!atoi(Tcl_GetVar(interp,"LoadData",1))) {
		string err = "Load a simualtion first!";
		cout << err << endl;
		char cmd[144];
		sprintf(cmd,"tk_messageBox -title warning -message \"%s\" -type ok -icon warning",err.c_str());
		Tcl_EvalEx(interp,cmd,-1,TCL_EVAL_GLOBAL);
		return TCL_OK;
	}
//	FILE *output;
//	output=fopen("SIMtoEXP.otp","w"); 
//	fprintf(output,"Component		Volume[A^3]\n");
	printf("\nComponent		Volume[A^3]\n");
	int Ncomp=atoi(Tcl_GetVar(interp,"Ncomponent",1));
	for (int i=0;i<Ncomp;i++){
		sprintf(cmd,"create vol \"%s\" %d",component[i].name,i);
        Tcl_EvalEx(interp,cmd,-1,TCL_EVAL_GLOBAL);
	}
	sprintf(cmd,"create vol total %d",MaxComponent);
    Tcl_EvalEx(interp,cmd,-1,TCL_EVAL_GLOBAL);
//	Tcl_EvalEx(interp,"LoadData",-1,TCL_EVAL_GLOBAL);
	Mat_DP y(MaxComponent,MaxPoint),x(MaxComponent,1);
	int k=0;
	int left_margin = atoi(Tcl_GetVar(interp,"left_margin",1));
	int right_margin = atoi(Tcl_GetVar(interp,"right_margin",1));
	for (int j=0; j<Ncomp; j++){
		for (int i=0; i<Np; i++){
			component[j].yprob[i]=component[j].ysim[i];
			y[j][i]=component[j].yprob[i];
		}
		if (component[j].fix==1){
			x[j][0]=component[j].volume;
			k++;
		}
	}
	int Nfix=k;
	double rms;
	if (right_margin-left_margin+1<=Ncomp-Nfix) {
		string err = "Too few points for volume calculation!";
		cout << err << endl;
		char cmd[144];
		sprintf(cmd,"tk_messageBox -title warning -message \"%s\" -type ok -icon warning",err.c_str());
		Tcl_EvalEx(interp,cmd,-1,TCL_EVAL_GLOBAL);
		return TCL_OK;
	} else {
		rms=calculation(y,x,Nfix,Ncomp,left_margin,right_margin,component);
	}
//    for (int ib=0; ib<Ncomp; ib++){
//		cout << x[ib][0] << " ";
//	}
//	cout << endl;
//	Tcl_UpdateLinkedVar(interp,"rms");
	sprintf(cmd,"%lf",rms);
	Tcl_SetVar(interp,"rms",cmd,1);
	char active[2][6];
	sprintf(active[0],"free");
	sprintf(active[1],"fixed");
	double yprob_tot[MaxPoint];
	for (int i=0; i<Np; i++) {
		yprob_tot[i]=0;
//		yed_tot[i]=0;
//		ynsld_tot[i]=0;
	}
	for (int j=0; j<Ncomp; j++){
		component[j].volume=x[j][0];
//		fprintf(output,"%s		%lf	(%s)\n",component[j].name,component[j].volume,active[component[j].fix]);
		printf("%s		%lf	(%s)\n",component[j].name,component[j].volume,active[component[j].fix]);
		sprintf(cmd,"vol(%d)",j);
		Tcl_UpdateLinkedVar(interp,cmd);
		for (int i=0; i<Np; i++) {
			component[j].yprob[i]*=component[j].volume;
//			component[j].yed[i]=component[j].nel*component[j].yprob[i];
//			component[j].ynsld[i]=component[j].nsl*component[j].yprob[i];
		}
        Blt_ResetVector(yprob[j],component[j].yprob,Np,MaxPoint, TCL_VOLATILE);
//		Blt_ResetVector(yed[j],component[j].yed,Np,MaxPoint, TCL_VOLATILE);
//		Blt_ResetVector(ynsld[j],component[j].ynsld,Np,MaxPoint, TCL_VOLATILE);
		for (int i=0; i<Np; i++) {
			yprob_tot[i]+=component[j].yprob[i];
//			yed_tot[i]+=component[j].yed[i];
//			ynsld_tot[i]+=component[j].ynsld[i];
		}
	}
	printf("rms = %9.3g\n",rms);
//	fprintf(output,"gl.rms = %9.3g\n",gl.rms);
//	fprintf(output,"Ncomp = %d\n",Ncomp);
//	fprintf(output,"Npoint = %d\n",Np);
//	fclose(output);
	Blt_ResetVector(yprob[MaxComponent],yprob_tot,Np,MaxPoint, TCL_VOLATILE);
//	Blt_ResetVector(yed[MaxComponent],yed_tot,Np,MaxPoint, TCL_VOLATILE);
//	Blt_ResetVector(ynsld[MaxComponent],ynsld_tot,Np,MaxPoint, TCL_VOLATILE);
    return TCL_OK;
}
/************************************************************************/
