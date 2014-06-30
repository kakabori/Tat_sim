
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
#include "output.h"

/************************************************************************/
string extension(string filename,string ext){
	int dot=filename.find('.');
	if (dot!=string::npos) filename.resize(dot);
	return (filename+"."+ext);
}
string headline(Tcl_Interp *interp, int Ncomp, int Np){
	string tmp="        z";
	char cmd[44];
	char cmd2[44];
	for (int i=0; i<Ncomp; i++){
		sprintf(cmd,"name(%d)",i);
		sprintf(cmd2," %9s",Tcl_GetVar(interp,cmd,1));
		tmp += cmd2;
	}
	return tmp;
}
/************************************************************************/

/*********************** Output / Export ********************************/
int export_fit(Tcl_Interp *interp, string filename, Components comp[],
			   Blt_Vector *xBLT, Blt_Vector *yprob[]){
	FILE *output;
	char active[2][6];
	sprintf(active[0],"free");
	sprintf(active[1],"fixed");
	int dot=filename.find('.');
	if (dot!=string::npos) filename.resize(dot);
	output=fopen((filename+".otp").c_str(),"w"); 
	fprintf(output,"Component	nAtom	nAt/group	ngr	NumEl/gr	NeutSL/gr	Volume[A^3]\n");
	int Ncomp=atoi(Tcl_GetVar(interp,"Ncomponent",1));
	int Np=xBLT->numValues;
	for (int j=0; j<Ncomp; j++){
		fprintf(output,"%-9s	%.0lf	%.1lf		%.0lf	%.0lf		%.4e	%lf	(%s)\n",comp[j].name,comp[j].nat,comp[j].natpgr,comp[j].ngr,comp[j].nelpgr,comp[j].nslpgr,comp[j].volume,active[comp[j].fix]);
	}
	fprintf(output,"rms = %9.6g\n",atof(Tcl_GetVar(interp,"rms",1)));
	fprintf(output,"Ncomponent = %d\n",Ncomp);
	fprintf(output,"Npoint = %d\n",Np);
	fprintf(output,"area per lipid: %.2lf\n",atof(Tcl_GetVar(interp,"area",1)));
	fprintf(output,"Number of lipids in simulation: %.0lf\n",atof(Tcl_GetVar(interp,"Nlip",1)));
	fprintf(output,"bin step size: %lf\n",atof(Tcl_GetVar(interp,"binstep",1)));
	fprintf(output,"total bin volume: %lf\n",atof(Tcl_GetVar(interp,"binsize",1)));
	fclose(output);

	output=fopen((filename+".vpp").c_str(),"w"); 
    fprintf(output,"        z");
	for (int i=0; i<Ncomp; i++) fprintf(output," %9s",comp[i].name);
	fprintf(output,"     total\n");
	double *xsim=xBLT->valueArr;
	double *yprob_tot=yprob[MaxComponent]->valueArr;
	for (int k=0; k<Np; k++){
		fprintf(output,"%9.7g",xsim[k]);
		for (int i=0; i<Ncomp; i++) fprintf(output," %9.5g",comp[i].yprob[k]);
        fprintf(output," %9.5g\n",yprob_tot[k]);
	}
	fclose(output);
	return 0;
}
int export_edp(Tcl_Interp *interp, string filename,
			   Blt_Vector *xBLT, Blt_Vector *yed[]){
	FILE *output;
	output=fopen(extension(filename,"edp").c_str(),"w"); 
	int Ncomp=atoi(Tcl_GetVar(interp,"Ncomponent",1));
	int Np=xBLT->numValues;
	fprintf(output,"%s",headline(interp,Ncomp,Np).c_str());
	fprintf(output,"     total\n");
	double *xsim=xBLT->valueArr;
	double *yed_i[MaxComponent];
	for (int i=0; i<Ncomp; i++) yed_i[i]=yed[i]->valueArr; 
	double *yed_tot=yed[MaxComponent]->valueArr;
	for (int k=0; k<Np; k++){
		fprintf(output,"%9.7g",xsim[k]);
		for (int i=0; i<Ncomp; i++) fprintf(output," %9.5g",yed_i[i][k]);
        fprintf(output," %9.5g\n",yed_tot[k]);
	}
	fclose(output);
	return 0;
}
int export_nsld(Tcl_Interp *interp, string filename,
			   Blt_Vector *xBLT, Blt_Vector *ynsld[]){
	FILE *output;
	output=fopen(extension(filename,"nsld").c_str(),"w"); 
	int Ncomp=atoi(Tcl_GetVar(interp,"Ncomponent",1));
	int Np=xBLT->numValues;
	fprintf(output,"%s",headline(interp,Ncomp,Np).c_str());
	fprintf(output,"     total\n");
	double *xsim=xBLT->valueArr;
	double *ynsld_i[MaxComponent];
	for (int i=0; i<Ncomp; i++) ynsld_i[i]=ynsld[i]->valueArr; 
	double *ynsld_tot=ynsld[MaxComponent]->valueArr;
	for (int k=0; k<Np; k++){
		fprintf(output,"%9.7g",xsim[k]);
		for (int i=0; i<Ncomp; i++) fprintf(output," %9.5g",ynsld_i[i][k]);
        fprintf(output," %9.5g\n",ynsld_tot[k]);
	}
	fclose(output);
	return 0;
}
int export_sim(Tcl_Interp *interp, string filename,
			   Blt_Vector *xBLT, Blt_Vector *ysim[]){
	FILE *output;
	output=fopen(extension(filename,"sim").c_str(),"w"); 
	int Ncomp=atoi(Tcl_GetVar(interp,"Ncomponent",1));
	int Np=xBLT->numValues;
	fprintf(output,"%s",headline(interp,Ncomp,Np).c_str());
	fprintf(output,"\n");
	double *xsim=xBLT->valueArr;
	double *ysim_i[MaxComponent];
	for (int i=0; i<Ncomp; i++) ysim_i[i]=ysim[i]->valueArr; 
	for (int k=0; k<Np; k++){
		fprintf(output,"%9.7g",xsim[k]);
		for (int i=0; i<Ncomp; i++) fprintf(output," %9.5g",ysim_i[i][k]);
		fprintf(output,"\n");
	}
	fclose(output);
	return 0;
}
int export_xff(Tcl_Interp *interp, string filename,
			   Blt_Vector *qFFxblt, Blt_Vector *xFFblt){
	FILE *output;
	output=fopen(extension(filename,"xff").c_str(),"w"); 
	fprintf(output,"        q       FF\n");
	double *q=qFFxblt->valueArr;
	double *FF=xFFblt->valueArr;
	for (int k=0; k<xFFblt->numValues; k++){
		fprintf(output,"%9.7g %9.5g\n",q[k],FF[k]);
	}
	fclose(output);
	return 0;
}
int export_nff(Tcl_Interp *interp, string filename,
			   Blt_Vector *qFFnblt, Blt_Vector *nFFblt){
	FILE *output;
	output=fopen(extension(filename,"nff").c_str(),"w"); 
    fprintf(output,"        q       FF\n");
	double *q=qFFnblt->valueArr;
	double *FF=nFFblt->valueArr;
	for (int k=0; k<nFFblt->numValues; k++){
		fprintf(output,"%9.7g %9.5g\n",q[k],FF[k]);
	}
	fclose(output);
	return 0;
}

int NK_export(Tcl_Interp *interp,int objc, Tcl_Obj *const objv[], Components component[],
			  Blt_Vector *xBLT,Blt_Vector  *ysim[],Blt_Vector  *yprob[],Blt_Vector  *yed[],Blt_Vector  *ynsld[],
			  Blt_Vector *qFFxblt,Blt_Vector  *qFFnblt,Blt_Vector  *xFFblt,Blt_Vector  *nFFblt) {
	char *filename;
	char *kind;
	if(objc<3) return TCL_OK;
	kind=Tcl_GetString(objv[1]);
	filename=Tcl_GetString(objv[2]);
	switch (kind[0]){
		case 'f': export_fit(interp,string(filename),component,xBLT,yprob); break;
		case 'e': export_edp(interp,string(filename),xBLT,yed); break;
		case 'n': export_nsld(interp,string(filename),xBLT,ynsld); break;
		case 's': export_sim(interp,string(filename),xBLT,ysim); break;
		case '0': export_xff(interp,string(filename),qFFxblt,xFFblt); break;
		case '1': export_nff(interp,string(filename),qFFnblt,nFFblt); break;
	}
	return TCL_OK;
}
/************************************************************************/
