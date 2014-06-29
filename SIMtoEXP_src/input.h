
/************************************************************************/
double atom_to_nel(Tcl_Interp *interp,char atom,string err_nel);
double atom_to_nsl(Tcl_Interp *interp,char atom,string err_nsl);
int ReadHead(Tcl_Interp *interp,char dataf[]);
/********************************** Input *******************************/
// reads component parsing from command file
int ReadCmd(Tcl_Interp *interp, char dataf[], int head, Components *component);
int NK_LoadCmd(Tcl_Interp *interp, int objc, Tcl_Obj *const objv[], Components component[]);
//int model(Tcl_Interp *interp, int objc,Tcl_Obj *const objv[], Components component[]);
//int NK_model(Tcl_Interp *interp, int objc, Tcl_Obj *const objv[], Components component[]);
/************************************************************************/
// reads data from simulation files
int ReadDataNew(Tcl_Interp *interp,char newfile[],char dataf[],int head,Components component[],double xsim[]);
int ReadDataTOT(Tcl_Interp *interp,char dataf[],int head,double xsim[],double yed_tot[],double ynsld_tot[],
				Blt_Vector *yaff[]);
int NK_LoadDataNew(Tcl_Interp *interp,int objc, Tcl_Obj *const objv[], Components component[],
				   Blt_Vector *xBLT,Blt_Vector *ysim[],Blt_Vector *yed[],Blt_Vector *ynsld[],
				   Blt_Vector *yaff[]);
/************************************************************************/
int ReadDataExp(char dataf[],int head,double qFE[],double FE[],double eFE[]);
int NK_LoadDataX(Tcl_Interp *interp,int objc, Tcl_Obj *const objv[],
				 Blt_Vector *qFExblt[], Blt_Vector *xFEblt[], Blt_Vector *eFExblt[]);
int NK_LoadDataN(Tcl_Interp *interp,int objc, Tcl_Obj *const objv[],
				 Blt_Vector *qFEnblt[], Blt_Vector *nFEblt[], Blt_Vector *eFEnblt[]);
/************************************************************************/
struct SDPcomp {
	char name[16];
	double sdp[MaxPoint];
};
int ReadSDP(Tcl_Interp *interp,char dataf[],double xsdp[],SDPcomp comp[], int *Nc);
int NK_LoadSDPx(Tcl_Interp *interp,int objc, Tcl_Obj *const objv[],
				 Blt_Vector *xedEXP, Blt_Vector *yedEXP[]);
int NK_LoadSDPn(Tcl_Interp *interp,int objc, Tcl_Obj *const objv[],
				 Blt_Vector *xnsldEXP, Blt_Vector *ynsldEXP[]);
/************************************************************************/
