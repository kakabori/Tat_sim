
/************************************************************************/
int sign(double x);
/********************** Fourier Transform *******************************/
struct atomff{
	double a[4],b[4],c;
	double fi0;
	double _atomff(double q);
};
struct FourierTransform {
	double *x,*sd,*yaff[MaxAFF];
	double *q,*FF;
	double water;
	double q0,q1,qstep;
	int symmetrized;
	int Nq;
	int interval();
	int AFFcorrection(Tcl_Interp *interp,Blt_Vector *yaff[],atomff *aff);
	int fourier(int left_margin, int right_margin);
	int fourier(int nid,atomff aff[],int left_margin, int right_margin);
};
int NK_fourierX(Tcl_Interp *interp, Blt_Vector *xBLT, Blt_Vector *yed,
			   Blt_Vector *yaff[], Blt_Vector *qFFxblt, Blt_Vector *xFFblt);
int NK_fourierN(Tcl_Interp *interp, Blt_Vector *xBLT, Blt_Vector *ynsld,
			   Blt_Vector *qFFnblt, Blt_Vector *nFFblt);
/************************************************************************/
