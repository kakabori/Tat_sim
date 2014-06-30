
/************************************************************************/
void NR::gaussj(Mat_IO_DP &a, Mat_IO_DP &b);
double calculation(Mat_DP &y, Mat_DP &b, int Nfix, int Ncomp, int Np, Components component[]);
/******************** Volume analysis ***********************************/
int NK_Normalize(Tcl_Interp *interp, Components component[], Blt_Vector *ysim[]);
int NK_fit(Tcl_Interp *interp, Components component[], Blt_Vector *yprob[], int Npoint);
/************************************************************************/
