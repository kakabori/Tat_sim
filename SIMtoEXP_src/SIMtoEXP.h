
// maximal dimensions of arrays
static const int MaxPoint=2004; // maximal number of simulation points
static const int MaxComponent=10; // maximal number of functional components
static const int MaxAtom=1000; // maximal number of atoms in component
//static const int MaxColumn=100000; // maximal number of columns in simulation file - constraint lifted
static const int MaxContr=2; // maximal number of contrasts
static const int MaxFF=2001; // maximal number of FF points
static const int MaxEXP=5; // maximal number of experimental data sets
static const int MaxAFF=20; // maximal number of atomic form factors
static const int MaxAtomChar=10; // maximal number of characters in atom names

struct Components {
	int id;
	char *name;
	char atom[MaxAtom][MaxAtomChar];
	int Natom;
	double nat;		// total number of atoms in component
	double nel;		// total number of electrons in component
	double nsl;		// total neutron scattering length in component
	double ngr;		// number of groups per component
	double natpgr;	// number of atoms per component group = nat/ngr
	double nelpgr;	// number of electrons per component group = nel/ngr
	double nslpgr;	// neutron scattering length per component group = nsl/ngr
	double volume;	// of the component group
	short int fix;
//	double xsim[MaxPoint];
    double ysim[MaxPoint];
	double ysimbak[MaxPoint];
    double yprob[MaxPoint];
	double yed[MaxPoint];
	double ynsld[MaxPoint];
};
struct Columns {
	char name[MaxAtomChar];
	int component[MaxComponent];
	int Ncomp;
	double nsl;
	double nel;
	double partial;
};

enum atom {H, C, N, O, P, number_of_atoms};
struct scattering {
	char id;
	double nsl;
	double nel;
};
/************************************************************************/

int TCL_LoadCmd(ClientData clientData, Tcl_Interp *interp,int objc, Tcl_Obj *const objv[]);
int TCL_model(ClientData clientData, Tcl_Interp *interp,int objc, Tcl_Obj *const objv[]);
int TCL_LoadDataNew(ClientData clientData, Tcl_Interp *interp,int objc, Tcl_Obj *const objv[]);
int TCL_LoadData(ClientData clientData, Tcl_Interp *interp,int objc, Tcl_Obj *const objv[]);
int TCL_LoadDataX(ClientData clientData, Tcl_Interp *interp,int objc, Tcl_Obj *const objv[]);
int TCL_LoadDataN(ClientData clientData, Tcl_Interp *interp,int objc, Tcl_Obj *const objv[]);
int TCL_LoadSDPx(ClientData clientData, Tcl_Interp *interp,int objc, Tcl_Obj *const objv[]);
int TCL_LoadSDPn(ClientData clientData, Tcl_Interp *interp,int objc, Tcl_Obj *const objv[]);
int TCL_export(ClientData clientData, Tcl_Interp *interp,int objc, Tcl_Obj *const objv[]);
int TCL_Normalize(ClientData clientData, Tcl_Interp *interp,int objc, Tcl_Obj *const objv[]);
int TCL_fit(ClientData clientData, Tcl_Interp *interp,int objc, Tcl_Obj *const objv[]);
int TCL_fourier(ClientData clientData, Tcl_Interp *interp,int objc, Tcl_Obj *const objv[]);
int TCL_scaling(ClientData clientData, Tcl_Interp *interp,int objc, Tcl_Obj *const objv[]);
int TCL_gibbs(ClientData clientData, Tcl_Interp *interp,int objc, Tcl_Obj *const objv[]);
