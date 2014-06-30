
/************************************************************************/
string extension(string filename,string ext);
string headline(Tcl_Interp *interp, int Ncomp, int Np);
/*********************** Output / Export ********************************/
int export_fit(Tcl_Interp *interp, string filename, Components component[],
			   Blt_Vector *xBLT, Blt_Vector *yprob[]);
int export_edp(Tcl_Interp *interp, string filename,
			   Blt_Vector *xBLT, Blt_Vector *yed[]);
int export_nsld(Tcl_Interp *interp, string filename,
			   Blt_Vector *xBLT, Blt_Vector *ynsld[]);
int export_sim(Tcl_Interp *interp, string filename,
			   Blt_Vector *xBLT, Blt_Vector *ysim[]);
int export_xff(Tcl_Interp *interp, string filename,
			   Blt_Vector *qFFxblt, Blt_Vector *xFFblt);
int export_nff(Tcl_Interp *interp, string filename,
			   Blt_Vector *qFFnblt, Blt_Vector *nFFblt);
int NK_export(Tcl_Interp *interp,int objc, Tcl_Obj *const objv[], Components component[],
			  Blt_Vector *xBLT,Blt_Vector  *ysim[],Blt_Vector  *yprob[],Blt_Vector  *yed[],Blt_Vector  *ynsld[],
			  Blt_Vector *qFFxblt,Blt_Vector  *qFFnblt,Blt_Vector  *xFFblt,Blt_Vector  *nFFblt);
/************************************************************************/
