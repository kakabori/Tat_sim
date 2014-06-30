/************************************************************************/
//	Simple program for the volume analysis of simulated profiles		//
//                    with a Tcl/Tk GUI                                 //
// (required Tcl/Tk and Blt packages to be installed)					//
/************************************************************************/
// It is based on Horia's original routine written in fortran			//
//c Reference:															//
//c Determination of component volumes of lipid bilayers				//
//c from simulations. 1997.												//
//c H. I. Petrache, S. E. Feller, and J. F. Nagle						//
//c Biophys. J., 72, 2237--2242.										//
/************************************************************************/
//								****									//
//		****		Norbert.Kucerka@nrc-cnrc.gc.ca		****			//
//								****									//
/************************************************************************/

#include <cmath>
#include <fstream>
#include <iostream>
#include <sstream>
#include <stdio.h>
#include <string>
#include <tcl.h>
#include <tk.h>
#include <blt.h>
#include "nr.h"
#include <time.h>
using namespace std;

#include "SIMtoEXP.h"
#include "input.h"
#include "output.h"
#include "volumes.h"
#include "fourier.h"
#include "scaling.h"
#include "analysis.h"

extern "C" int Blt_Init(Tcl_Interp *interp);

Blt_Vector *xBLT, *ysim[MaxComponent+1], *yprob[MaxComponent+1], *yed[MaxComponent+1], *ynsld[MaxComponent+1];
Blt_Vector *yaff[MaxAFF];
Blt_Vector *xedEXP,*yedEXP[MaxComponent+1], *xnsldEXP,*ynsldEXP[MaxComponent+1];
Blt_Vector *qFFxblt, *qFFnblt, *xFFblt, *nFFblt;
Blt_Vector *qFExblt[MaxEXP], *qFEnblt[MaxEXP], *xFEblt[MaxEXP], *eFExblt[MaxEXP], *nFEblt[MaxEXP], *eFEnblt[MaxEXP];
Components component[MaxComponent+1];

/************************************************************************/


/************************************************************************/
/************************************************************************/
int main(int argc, char *argv[]){
  Tcl_Interp *interp;
  interp=Tcl_CreateInterp();
  time_t now,seconds,license;
  seconds = time(&now);
  struct tm *ptm,wedding;
  ptm=gmtime(&now);
  cout << ptm->tm_year+1900 <<"/"<< ptm->tm_mon+1 <<"/"<< ptm->tm_mday << endl;
/* for not-fully-developed-versions only *
  // Lets ask people to renew the program on my slovak-wedding day
  wedding.tm_year = 2009 - 1900;
  wedding.tm_mon = 6 - 1;
  wedding.tm_mday = 26;
  wedding.tm_hour = 0;
  wedding.tm_min = 0;
  wedding.tm_sec = 0;
//  cout << wedding.tm_year+1900 <<"/"<< wedding.tm_mon+1 <<"/"<< wedding.tm_mday << endl;
  wedding.tm_mday+=1*365;
  license = mktime(&wedding);
//  cout << wedding.tm_year+1900 <<"/"<< wedding.tm_mon+1 <<"/"<< wedding.tm_mday << endl;
  if (seconds > license) {
	  string err = "Please contact Norbert.Kucerka@nrc.gc.ca";
	  cout << err << endl;
	  cin.get();
//	  char cmd[144];
//	  printf("tk_messageBox -title warning -message \"%s\" -type ok -icon warning",err.c_str());
//	  sprintf(cmd,"tk_messageBox -title warning -message \"%s\" -type ok -icon warning",err.c_str());
//	  Tcl_EvalEx(interp,cmd,-1,TCL_EVAL_GLOBAL);
	  return(0);
  }
*******************************************/
  Tk_MainEx(argc, argv, Tcl_AppInit,interp);
  return(0);
}
/************************************************************************/


/************************************************************************/
/************************ Initialization ********************************/
int Init_vectors(Tcl_Interp *interp)
{
  char vectorname[16];
  if (Blt_CreateVector(interp, "xsim", MaxPoint, &xBLT) != TCL_OK) return TCL_ERROR;
  for (register int i=0; i<MaxComponent+1; i++){
    sprintf(vectorname,"ysim%d",i);
    if (Blt_CreateVector(interp, vectorname, MaxPoint, &ysim[i]) != TCL_OK) return TCL_ERROR;
    sprintf(vectorname,"yprob%d",i);
    if (Blt_CreateVector(interp, vectorname, MaxPoint, &yprob[i]) != TCL_OK) return TCL_ERROR;
	sprintf(vectorname,"yed%d",i);
    if (Blt_CreateVector(interp, vectorname, MaxPoint, &yed[i]) != TCL_OK) return TCL_ERROR;
	sprintf(vectorname,"ynsld%d",i);
    if (Blt_CreateVector(interp, vectorname, MaxPoint, &ynsld[i]) != TCL_OK) return TCL_ERROR;
  }
  for (register int i=0; i<MaxAFF; i++){
    sprintf(vectorname,"yaff%d",i);
    if (Blt_CreateVector(interp, vectorname, MaxPoint, &yaff[i]) != TCL_OK) return TCL_ERROR;
  }
  if (Blt_CreateVector(interp, "qFFx", MaxFF, &qFFxblt) != TCL_OK) return TCL_ERROR;
  if (Blt_CreateVector(interp, "xFF", MaxFF, &xFFblt) != TCL_OK) return TCL_ERROR;
  if (Blt_CreateVector(interp, "qFFn", MaxFF, &qFFnblt) != TCL_OK) return TCL_ERROR;
  if (Blt_CreateVector(interp, "nFF", MaxFF, &nFFblt) != TCL_OK) return TCL_ERROR;
  if (Blt_CreateVector(interp, "xedEXP", MaxPoint, &xedEXP) != TCL_OK) return TCL_ERROR;
  if (Blt_CreateVector(interp, "xnsldEXP", MaxPoint, &xnsldEXP) != TCL_OK) return TCL_ERROR;
  for (register int i=0; i<MaxComponent+1; i++){
	sprintf(vectorname,"yedEXP%d",i);
	if (Blt_CreateVector(interp, vectorname, MaxPoint, &yedEXP[i]) != TCL_OK) return TCL_ERROR;
	sprintf(vectorname,"ynsldEXP%d",i);
	if (Blt_CreateVector(interp, vectorname, MaxPoint, &ynsldEXP[i]) != TCL_OK) return TCL_ERROR;
  }
  for (register int i=0; i<MaxEXP; i++){
	sprintf(vectorname,"qFEx%d",i);
    if (Blt_CreateVector(interp, vectorname, MaxFF, &qFExblt[i]) != TCL_OK) return TCL_ERROR;
	sprintf(vectorname,"xFE%d",i);
	if (Blt_CreateVector(interp, vectorname, MaxFF, &xFEblt[i]) != TCL_OK) return TCL_ERROR;
	sprintf(vectorname,"eFEx%d",i);
    if (Blt_CreateVector(interp, vectorname, MaxFF, &eFExblt[i]) != TCL_OK) return TCL_ERROR;
	sprintf(vectorname,"qFEn%d",i);
	if (Blt_CreateVector(interp, vectorname, MaxFF, &qFEnblt[i]) != TCL_OK) return TCL_ERROR;
	sprintf(vectorname,"nFE%d",i);
	if (Blt_CreateVector(interp, vectorname, MaxFF, &nFEblt[i]) != TCL_OK) return TCL_ERROR;
	sprintf(vectorname,"eFEn%d",i);
    if (Blt_CreateVector(interp, vectorname, MaxFF, &eFEnblt[i]) != TCL_OK) return TCL_ERROR;
  }
  return TCL_OK;
}
void linkvar(Tcl_Interp *interp){
	char varname[10];
	Tcl_LinkVar(interp,"MaxComponent",(char *)&MaxComponent, TCL_LINK_INT);
	Tcl_LinkVar(interp,"MaxEXP",(char *)&MaxEXP, TCL_LINK_INT);
	for(int i=0;i<MaxComponent;i++){
		sprintf(varname,"vol(%d)",i);
		Tcl_LinkVar(interp,varname,(char *)&component[i].volume, TCL_LINK_DOUBLE);
		sprintf(varname,"natpgr(%d)",i);
		Tcl_LinkVar(interp,varname,(char *)&component[i].natpgr, TCL_LINK_DOUBLE);
		sprintf(varname,"nat(%d)",i);
		Tcl_LinkVar(interp,varname,(char *)&component[i].nat, TCL_LINK_DOUBLE);
		sprintf(varname,"ngr(%d)",i);
		Tcl_LinkVar(interp,varname,(char *)&component[i].ngr, TCL_LINK_DOUBLE);
		sprintf(varname,"nelpgr(%d)",i);
		Tcl_LinkVar(interp,varname,(char *)&component[i].nelpgr, TCL_LINK_DOUBLE);
		sprintf(varname,"nslpgr(%d)",i);
		Tcl_LinkVar(interp,varname,(char *)&component[i].nslpgr, TCL_LINK_DOUBLE);
		sprintf(varname,"nel(%d)",i);
		Tcl_LinkVar(interp,varname,(char *)&component[i].nel, TCL_LINK_DOUBLE);
		sprintf(varname,"nsl(%d)",i);
		Tcl_LinkVar(interp,varname,(char *)&component[i].nsl, TCL_LINK_DOUBLE);
		sprintf(varname,"name(%d)",i);
		Tcl_LinkVar(interp,varname,(char *)&component[i].name, TCL_LINK_STRING);
		sprintf(varname,"fix(%d)",i);
		Tcl_LinkVar(interp,varname,(char *)&component[i].fix, TCL_LINK_INT);
	}
}
void updatevar(Tcl_Interp *interp){
	char varname[10];
	for(int i=0;i<MaxComponent;i++){
		sprintf(varname,"vol(%d)",i);
		Tcl_UpdateLinkedVar(interp,varname);
		sprintf(varname,"natpgr(%d)",i);
		Tcl_UpdateLinkedVar(interp,varname);
		sprintf(varname,"nat(%d)",i);
		Tcl_UpdateLinkedVar(interp,varname);
		sprintf(varname,"ngr(%d)",i);
		Tcl_UpdateLinkedVar(interp,varname);
		sprintf(varname,"nelpgr(%d)",i);
		Tcl_UpdateLinkedVar(interp,varname);
		sprintf(varname,"nslpgr(%d)",i);
		Tcl_UpdateLinkedVar(interp,varname);
		sprintf(varname,"nel(%d)",i);
		Tcl_UpdateLinkedVar(interp,varname);
		sprintf(varname,"nsl(%d)",i);
		Tcl_UpdateLinkedVar(interp,varname);
		sprintf(varname,"name(%d)",i);
		Tcl_UpdateLinkedVar(interp,varname);
		sprintf(varname,"fix(%d)",i);
		Tcl_UpdateLinkedVar(interp,varname);
	}
}
void runTCLcommand(Tcl_Interp *interp, int objc, Tcl_Obj *const objv[]){
	string str;
	str="";
	for (int i=1; i<objc; i++){
		str += Tcl_GetString(objv[i]);
		str += " ";
	}
//	cout << str << endl;
	Tcl_EvalEx(interp, str.c_str(), -1, TCL_EVAL_GLOBAL);
}
/* development */
void configure_axis(Tcl_Interp *interp, int objc, Tcl_Obj *const objv[]){
	string str;
	cout << objc << endl;
	str = Tcl_GetString(objv[1]);
	cout << str << endl;
	str += " axis configure ";
	cout << str << endl;
	str += Tcl_GetString(objv[2]);
	cout << str << endl;
	str += " ";
	str += Tcl_GetString(objv[3]);
	cout << str << endl;
	if (objc>4) {
		str += " ";
		str += Tcl_GetString(objv[4]);
		cout << str << endl;
	}
	Tcl_EvalEx(interp, str.c_str(), -1, TCL_EVAL_GLOBAL);
}
/************************************************************************/


/************************************************************************/
/******************* TCL_Commands wrappers ******************************/
int TCL_LoadCmd(ClientData clientData, Tcl_Interp *interp,int objc, Tcl_Obj *const objv[]){
	NK_LoadCmd(interp,objc,objv,component);
//	printf("LoadCmd completed\n");
    return TCL_OK;
}
/* old way; TCL_LoadCmd now
int TCL_model(ClientData clientData, Tcl_Interp *interp,int objc, Tcl_Obj *const objv[]){
	NK_model(interp,objc,objv,component);
//	printf("LoadCmd completed\n");
    return TCL_OK;
}
*/
int TCL_LoadDataNew(ClientData clientData, Tcl_Interp *interp,int objc, Tcl_Obj *const objv[]){
	NK_LoadDataNew(interp,objc,objv,component,xBLT,ysim,yed,ynsld,yaff);
//	printf("LoadDataNew completed\n");
    return TCL_OK;
}
int TCL_LoadDataX(ClientData clientData, Tcl_Interp *interp,int objc, Tcl_Obj *const objv[]){
	NK_LoadDataX(interp,objc,objv,qFExblt,xFEblt,eFExblt);
//	printf("LoadDataX completed\n");
    return TCL_OK;
}
int TCL_LoadDataN(ClientData clientData, Tcl_Interp *interp,int objc, Tcl_Obj *const objv[]){
	NK_LoadDataN(interp,objc,objv,qFEnblt,nFEblt,eFEnblt);
//	printf("LoadDataN completed\n");
    return TCL_OK;
}
int TCL_LoadSDPx(ClientData clientData, Tcl_Interp *interp,int objc, Tcl_Obj *const objv[]){
	NK_LoadSDPx(interp,objc,objv,xedEXP,yedEXP);
    return TCL_OK;
}
int TCL_LoadSDPn(ClientData clientData, Tcl_Interp *interp,int objc, Tcl_Obj *const objv[]){
	NK_LoadSDPn(interp,objc,objv,xnsldEXP,ynsldEXP);
    return TCL_OK;
}
int TCL_export(ClientData clientData, Tcl_Interp *interp,int objc, Tcl_Obj *const objv[]){
	NK_export(interp,objc,objv,component,xBLT,ysim,yprob,yed,ynsld,qFFxblt,qFFnblt,xFFblt,nFFblt);
	return TCL_OK;
}
int TCL_Normalize(ClientData clientData, Tcl_Interp *interp,int objc, Tcl_Obj *const objv[]){
	NK_Normalize(interp,component,ysim);
    return TCL_OK;
}
int TCL_fit(ClientData clientData, Tcl_Interp *interp,int objc, Tcl_Obj *const objv[]){
	NK_fit(interp,component,yprob,xBLT->numValues);
	return TCL_OK;
}
int TCL_fourierX(ClientData clientData, Tcl_Interp *interp,int objc, Tcl_Obj *const objv[]){
	NK_fourierX(interp,xBLT,yed[MaxComponent],yaff,qFFxblt,xFFblt);
	return TCL_OK;
}
int TCL_fourierN(ClientData clientData, Tcl_Interp *interp,int objc, Tcl_Obj *const objv[]){
	NK_fourierN(interp,xBLT,ynsld[MaxComponent],qFFnblt,nFFblt);
	return TCL_OK;
}
int TCL_scaling(ClientData clientData, Tcl_Interp *interp,int objc, Tcl_Obj *const objv[]){
	NK_scaling(interp,objc,objv,qFExblt,xFEblt,eFExblt,qFEnblt,nFEblt,eFEnblt,xBLT,yed[MaxComponent],yaff,ynsld[MaxComponent]);
	return TCL_OK;
}
int TCL_updatevar(ClientData clientData, Tcl_Interp *interp,int objc, Tcl_Obj *const objv[]){
	updatevar(interp);
	return TCL_OK;
}
int TCL_runTCLcommand(ClientData clientData, Tcl_Interp *interp,int objc, Tcl_Obj *const objv[]){
	runTCLcommand(interp,objc,objv);
	return TCL_OK;
}
int TCL_configure_axis(ClientData clientData, Tcl_Interp *interp,int objc, Tcl_Obj *const objv[]){
	configure_axis(interp,objc,objv);
	return TCL_OK;
}
int TCL_gibbs(ClientData clientData, Tcl_Interp *interp,int objc, Tcl_Obj *const objv[]){
	NK_gibbs(interp,objc,objv);
	return TCL_OK;
}
/************************************************************************/


/************************************************************************/
/******************* Command registration *******************************/
int Tcl_AppInit(Tcl_Interp *interp){
  if (Tcl_Init(interp) != TCL_OK) {
    printf("%s\n", interp->result);
    return TCL_ERROR;
  } 
  if (Tk_Init(interp) != TCL_OK) { 
    return TCL_ERROR; 
  }
  if (Blt_Init(interp) !=TCL_OK) {
    return TCL_ERROR;
  }
  Tcl_CreateObjCommand(interp,"fit", TCL_fit, (ClientData) NULL, (Tcl_CmdDeleteProc *) NULL);
  Tcl_CreateObjCommand(interp,"LoadDataX", TCL_LoadDataX, (ClientData) NULL, (Tcl_CmdDeleteProc *) NULL);
  Tcl_CreateObjCommand(interp,"LoadDataN", TCL_LoadDataN, (ClientData) NULL, (Tcl_CmdDeleteProc *) NULL);
  Tcl_CreateObjCommand(interp,"LoadSDPx", TCL_LoadSDPx, (ClientData) NULL, (Tcl_CmdDeleteProc *) NULL);
  Tcl_CreateObjCommand(interp,"LoadSDPn", TCL_LoadSDPn, (ClientData) NULL, (Tcl_CmdDeleteProc *) NULL);
  Tcl_CreateObjCommand(interp,"LoadCmd", TCL_LoadCmd, (ClientData) NULL, (Tcl_CmdDeleteProc *) NULL);
//  Tcl_CreateObjCommand(interp,"model", TCL_model, (ClientData) NULL, (Tcl_CmdDeleteProc *) NULL);
  Tcl_CreateObjCommand(interp,"LoadDataNew", TCL_LoadDataNew, (ClientData) NULL, (Tcl_CmdDeleteProc *) NULL);
  Tcl_CreateObjCommand(interp,"export", TCL_export, (ClientData) NULL, (Tcl_CmdDeleteProc *) NULL);
  Tcl_CreateObjCommand(interp,"normalize", TCL_Normalize, (ClientData) NULL, (Tcl_CmdDeleteProc *) NULL);
  Tcl_CreateObjCommand(interp,"fourierX", TCL_fourierX, (ClientData) NULL, (Tcl_CmdDeleteProc *) NULL);
  Tcl_CreateObjCommand(interp,"fourierN", TCL_fourierN, (ClientData) NULL, (Tcl_CmdDeleteProc *) NULL);
  Tcl_CreateObjCommand(interp,"scaling", TCL_scaling, (ClientData) NULL, (Tcl_CmdDeleteProc *) NULL);
  Tcl_CreateObjCommand(interp,"updatevar", TCL_updatevar, (ClientData) NULL, (Tcl_CmdDeleteProc *) NULL);
  Tcl_CreateObjCommand(interp,"runTCLcommand", TCL_runTCLcommand, (ClientData) NULL, (Tcl_CmdDeleteProc *) NULL);
  Tcl_CreateObjCommand(interp,"configure_axis", TCL_configure_axis, (ClientData) NULL, (Tcl_CmdDeleteProc *) NULL);
  Tcl_CreateObjCommand(interp,"gibbs", TCL_gibbs, (ClientData) NULL, (Tcl_CmdDeleteProc *) NULL);
  Init_vectors(interp);
  linkvar(interp);
  Tcl_EvalEx(interp,"source tcl/SIMtoEXP.tcl",-1,TCL_EVAL_GLOBAL);
  Tcl_EvalEx(interp,"wm title . \"SIMulation to EXPeriment by Norbert.Kucerka@nrc.gc.ca\"",-1,TCL_EVAL_GLOBAL);
  return TCL_OK;
}
/************************************************************************/
