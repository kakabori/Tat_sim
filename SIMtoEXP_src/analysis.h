
/************************************************************************/
int symmetrize(double vector[], int N);
double integral(int from, int to, double *rhoW);
/************************************************************************/
// calculate Gibb's dividing surface
double gibbs(double x[], double y[], int Npoint, int direction);
int NK_gibbs(Tcl_Interp *interp, int objc, Tcl_Obj *const objv[]);
/************************************************************************/