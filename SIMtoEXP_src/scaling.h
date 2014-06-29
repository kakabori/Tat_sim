
/************************************************************************/
double scaling(double *Yexp,double *Eexp,double *Ysim,int N,double *SC,double *BG,int SCfix,int BGfix);
int NK_scaling(Tcl_Interp *interp, int objc, Tcl_Obj *const objv[],
			   Blt_Vector **qFExblt, Blt_Vector **xFEblt, Blt_Vector **eFExblt,
			   Blt_Vector **qFEnblt, Blt_Vector **nFEblt, Blt_Vector **eFEnblt,
			   Blt_Vector *xBLT, Blt_Vector *yed, Blt_Vector *yaff[], Blt_Vector *ynsld);
/************************************************************************/