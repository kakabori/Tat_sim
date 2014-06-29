
#include <cmath>
#include <fstream>
#include <iostream>
#include <sstream>
#include <stdio.h>
#include <string.h>
#include <tcl.h>
#include <tk.h>
#include <blt.h>
#include <cstdlib>
#include <stdlib.h>
using namespace std;

#include "SIMtoEXP.h"
#include "input.h"
#include "fourier.h"
#include "analysis.h"

/************************************************************************/
double atom_to_nsl(Tcl_Interp *interp,char atom, string err){
	double tmp=0.0;
	char ctmp[6];
	const char *pctemp;
	int k=0;
	int end=0;
	int nid=0;
	nid=atoi(Tcl_GetVar(interp,"nid",1));
	while (end!=1 && k<nid){
		sprintf(ctmp,"id(%d)",k);
		pctemp = (Tcl_GetVar(interp,ctmp,1));
		if (atom==pctemp[0]){
			sprintf(ctmp,"b(%d)",k);
			tmp = atof(Tcl_GetVar(interp,ctmp,1));
			end=1;
		} else {
			k++;
		}		
	}
	if (end==0 && atom!='Z') {
//		char err[44],cmd[144];
//		sprintf(err,"WARNING: NSL not defined for %c",atom);
//		cout << err << endl;
//		sprintf(cmd,"tk_messageBox -title warning -message \"%s\" -type ok -icon warning",err);
//		Tcl_EvalEx(interp,cmd,-1,TCL_EVAL_GLOBAL);
		err = atom + " ";
	} else {
		err="";
	}
	return tmp;
}
double atom_to_nel(Tcl_Interp *interp,char atom, string err){
	double tmp=0.0;
	char ctmp[6];
	const char *pctemp;
	int k=0;
	int end=0;
	int nid=0;
	nid=atoi(Tcl_GetVar(interp,"nid",1));
	while (end!=1 && k<nid){
		sprintf(ctmp,"id(%d)",k);
		pctemp = (Tcl_GetVar(interp,ctmp,1));
		if (atom==pctemp[0]){
			sprintf(ctmp,"e(%d)",k);
			tmp = atof(Tcl_GetVar(interp,ctmp,1));
			end=1;
		} else {
			k++;
		}		
	}
	if (end==0 && atom!='Z') {
//		char err[44],cmd[144];
//		sprintf(err,"WARNING: nEL not defined for %c",atom);
//		cout << err << endl;
//		sprintf(cmd,"tk_messageBox -title warning -message \"%s\" -type ok -icon warning",err);
//		Tcl_EvalEx(interp,cmd,-1,TCL_EVAL_GLOBAL);
		err = atom + " "; 
	} else {
		err = "";
	}
	return tmp;
}
int ReadHead(Tcl_Interp *interp,char dataf[]){
	string line,subline;
	int head=0;
	ifstream fdat(dataf);
	if (!fdat) {
//		cout << "Error open file: "<< dataf << endl;
		return head;
	}
	getline(fdat,line);
	while (line[0]=='#'){
		subline=line.substr(1,line.size()-1);
		char *cline = new char[strlen(subline.c_str()) + 1];
		strcpy (cline, subline.c_str());
		Tcl_EvalEx(interp,cline,-1,TCL_EVAL_GLOBAL);
		delete[] cline;
		head++;
		getline(fdat,line);
	}
	fdat.close();
	return head;
}

/************************************************************************/

/************************************************************************/
/********************************** Input *******************************/

// reads component parsing from command file
int ReadCmd(Tcl_Interp *interp, char dataf[], int head, Components comp[],int report_err){
  string line,err_tmp,err_nel,err_nsl;
  err_nel="";
  err_nsl="";
  int Ncomp=0;
  ifstream fdat(dataf);
  if (!fdat) {
//	  cout << "Error open file: "<< dataf << endl;
	  return Ncomp;
  }
  int i,j=0,k=0;
  for (i=0; i<head; i++){
	  getline(fdat,line);
  }
  while ( !getline(fdat,line,'\n').eof() || line!="") {
//	cout << ">" << line << "<" << endl;
	if (line!="") {
	  char *cline = new char[strlen(line.c_str()) + 1];
	  strcpy (cline, line.c_str());
/* read a header first *
	  if (cline[0] == '*') {
		  int headend=0;
		  while ((!headend) && (!getline(fdat,line).eof())){
			  cline = new char[strlen(line.c_str()) + 1];
			  strcpy (cline, line.c_str());
			  if (cline[0] != '*'){
				  Tcl_EvalEx(interp,cline,-1,TCL_EVAL_GLOBAL);
				  //cout << cline << endl;
			  } else {
				  headend=1;
				  getline(fdat,line);
				  cline = new char[strlen(line.c_str()) + 1];
				  strcpy (cline, line.c_str());
				  //cout << cline << endl;
			  }
		  }
	  }
**********************/
	  char *tmp = strtok(cline,"	 ,");
	  strcpy(comp[j].name,tmp);
	  tmp = strtok(NULL,"	 ,");
	  int error=0;
	  if (tmp != NULL) {
//		  cout << tmp << endl;
		  char *natpgr = new char[strlen(tmp) + 1];
		  strcpy (natpgr, tmp);
//		  cout << natpgr << endl;
		  sscanf(natpgr,"%lf",&comp[j].natpgr);
		  delete[] natpgr;
		  if (comp[j].natpgr == 0) error=1;
	  }
	  k=0;
//	  cout << comp[j].natpgr << endl;
	  comp[j].id=j;
	  comp[j].nat=0;
	  comp[j].ngr=0;
	  comp[j].nel=0;
	  comp[j].nsl=0;
	  tmp = strtok(NULL,"	 ,");
	  while (tmp != NULL){
		  strcpy(comp[j].atom[k],tmp);
		  for (i=0; i<MaxAtomChar; i++){
			  if (comp[j].atom[k][i] >= 'a' && comp[j].atom[k][i] <= 'y') comp[j].atom[k][i] += 'A' - 'a';
		  }
//		  cout << "j=" << j << " k=" << k <<"    -"<< comp[j].atom[k] <<"-"<<  endl;
		  comp[j].nat ++;
		  comp[j].nel += atom_to_nel(interp,comp[j].atom[k][0],err_tmp);
		  err_nel += err_tmp;
		  comp[j].nsl += atom_to_nsl(interp,comp[j].atom[k][0],err_tmp);
		  err_nsl += err_tmp;
//		  cout << comp[j].nel << endl;
		  if (comp[j].atom[k][0]!=0) k++;
		  tmp = strtok(NULL,"	 ,");
	  }
	  if (k!=0 && error==0){
		  comp[j].Natom=k;
		  comp[j].ngr = comp[j].nat/comp[j].natpgr;
		  comp[j].nelpgr = comp[j].nel/comp[j].ngr;
		  comp[j].nslpgr = comp[j].nsl/comp[j].ngr;
//		  cout << "j=" << j <<"    "<< comp[j].Natom << endl;
		  j++;
	  }
	  line.clear();
	  delete[] cline;
	}
  }
  
  if (err_nel!="" && report_err==1){
	  err_nel = "WARNING: nEL not defined for " + err_nel;
	  cout << err_nel << endl;
	  err_nel = "tk_messageBox -title warning -message \"" + err_nel + "\" -type ok -icon warning";
	  char *cmd2 = new char[strlen(err_nel.c_str()) + 1];
	  strcpy(cmd2,err_nel.c_str());
	  Tcl_EvalEx(interp,cmd2,-1,TCL_EVAL_GLOBAL);
	  delete[] cmd2;
  }
  if (err_nsl!="" && report_err==1){
	  err_nsl = "WARNING: NSL not defined for " + err_nsl;
	  cout << err_nsl << endl;
	  err_nsl = "tk_messageBox -title warning -message \"" + err_nsl + "\" -type ok -icon warning";
	  char *cmd2 = new char[strlen(err_nsl.c_str()) + 1];
	  strcpy(cmd2,err_nsl.c_str());
	  Tcl_EvalEx(interp,cmd2,-1,TCL_EVAL_GLOBAL);
	  delete[] cmd2;
  }

  Ncomp=j;
  cout << "Number of components = " << Ncomp << endl;
  for (int j=0;j<Ncomp;j++){
	  printf("%-9s %6.1lf %6.1lf %6.1lf %6.1lf %6.1lf\n",comp[j].name,comp[j].nat,comp[j].natpgr,comp[j].ngr,comp[j].nelpgr,comp[j].nslpgr);
  }
  fdat.close();
  return Ncomp;
}
int NK_LoadCmd(Tcl_Interp *interp, int objc, Tcl_Obj *const objv[], Components component[]){
	char cmd[144];
	if(objc<2) return TCL_OK;
	char *dataf=Tcl_GetString(objv[1]);
	int report_err;
	Tcl_GetIntFromObj(interp,objv[2],&report_err);
	ifstream fdat(dataf);
	if (!fdat) {
		string err = "Error open file: " + string(dataf);
		cout << err << endl;
		sprintf(cmd,"tk_messageBox -title warning -message \"%s\" -type ok -icon warning",err.c_str());
		Tcl_EvalEx(interp,cmd,-1,TCL_EVAL_GLOBAL);
		return TCL_OK;
	}
	fdat.close();
	int Ncomp,head;
	head=ReadHead(interp,dataf);
    Ncomp=ReadCmd(interp,dataf,head,component,report_err);
	sprintf(cmd,"%d",Ncomp);
	Tcl_SetVar(interp,"Ncomponent",cmd,1);
	if (Ncomp!=0) Tcl_EvalEx(interp,"set LoadCmd 1",-1,TCL_EVAL_GLOBAL);
    for (int i=Ncomp; i<MaxComponent; i++){
		component[i].name=0;
		sprintf(cmd,"name(%d)",i);
		Tcl_UpdateLinkedVar(interp,cmd);

		sprintf(cmd,"set name(%d) {}",i);
        Tcl_EvalEx(interp,cmd,-1,TCL_EVAL_GLOBAL);
		sprintf(cmd,"delete sim %d",i);
        Tcl_EvalEx(interp,cmd,-1,TCL_EVAL_GLOBAL);
	}
	for (int i=0;i<Ncomp;i++){
		sprintf(cmd,"delete sim %d",i);
        Tcl_EvalEx(interp,cmd,-1,TCL_EVAL_GLOBAL);
		sprintf(cmd,"name(%d)",i);
		Tcl_UpdateLinkedVar(interp,cmd);
		sprintf(cmd,"natpgr(%d)",i);
		Tcl_UpdateLinkedVar(interp,cmd);
		sprintf(cmd,"nat(%d)",i);
		Tcl_UpdateLinkedVar(interp,cmd);
		sprintf(cmd,"ngr(%d)",i);
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
	for (int i=Ncomp;i<MaxComponent;i++){
		sprintf(cmd,"natpgr(%d)",i);
		Tcl_SetVar(interp,cmd,"0",1);
		sprintf(cmd,"nat(%d)",i);
		Tcl_SetVar(interp,cmd,"0",1);
		sprintf(cmd,"ngr(%d)",i);
		Tcl_SetVar(interp,cmd,"0",1);
		sprintf(cmd,"nelpgr(%d)",i);
		Tcl_SetVar(interp,cmd,"0",1);
		sprintf(cmd,"nslpgr(%d)",i);
		Tcl_SetVar(interp,cmd,"0",1);
		sprintf(cmd,"nel(%d)",i);
		Tcl_SetVar(interp,cmd,"0",1);
		sprintf(cmd,"nsl(%d)",i);
		Tcl_SetVar(interp,cmd,"0",1);
		sprintf(cmd,"vol(%d)",i);
		Tcl_SetVar(interp,cmd,"0",1);
	}
	printf("LoadCmd completed\n");
    return TCL_OK;
}
/************************************************************************/

// reads component parsing from command file
// by sourcing tcl file
/* old way; ReadCmd now
int model(Tcl_Interp *interp, int objc,Tcl_Obj *const objv[], Components comp[]){
  string line;
  int error=0;
  int j=0,i=0,k=0;
  char *tmp;
  while (i<objc){
	  error=0;
	  k=0;
	  strcpy(comp[j].name,Tcl_GetString(objv[i++]));
//	  cout << comp[j].name << endl;
  	  comp[j].id=j;
	  comp[j].nat=0;
	  comp[j].ngr=0;
	  comp[j].nel=0;
	  comp[j].nsl=0;
	  Tcl_GetDoubleFromObj(interp,objv[i++],&comp[j].natpgr);
	  if (comp[j].natpgr == 0) error=1;
	  tmp = Tcl_GetString(objv[i++]);
//	  cout << tmp << endl;
	  while (tmp[0]!='e' && tmp[1]!='n' && tmp[2]!='d'){
		  strcpy(comp[j].atom[k],tmp);
		  if (comp[j].atom[k][0] >= 'a' && comp[j].atom[k][0] <= 'z') comp[j].atom[k][0] += 'A' - 'a';
//		  cout << "j=" << j << " k=" << k <<"    -"<< comp[j].atom[k] <<"-"<<  endl;
		  comp[j].nat ++;
		  comp[j].nel += atom_to_nel(interp,comp[j].atom[k][0]);
		  comp[j].nsl += atom_to_nsl(interp,comp[j].atom[k][0]);
//		  cout << comp[j].nel << endl;
		  if (comp[j].atom[k][0]!=0) k++;
		  tmp = Tcl_GetString(objv[i++]);
//		  cout << tmp << endl;
	  }
	  if (k!=0 && error==0){
		  comp[j].Natom=k;
		  comp[j].ngr = comp[j].nat/comp[j].natpgr;
		  comp[j].nelpgr = comp[j].nel/comp[j].ngr;
		  comp[j].nslpgr = comp[j].nsl/comp[j].ngr;
//		  cout << "j=" << j <<"    "<< comp[j].Natom << endl;
		  j++;
	  }
  }
  int Ncomp=j;
  cout << "Number of components = " << Ncomp << endl;
  for (int j=0;j<Ncomp;j++){
	  printf("%-9s %6.1lf %6.1lf %6.1lf %6.1lf %6.1lf\n",comp[j].name,comp[j].nat,comp[j].natpgr,comp[j].ngr,comp[j].nelpgr,comp[j].nslpgr);
  }
  return Ncomp;
}
*/
/* old way; NK_LoadCmd now
int NK_model(Tcl_Interp *interp, int objc, Tcl_Obj *const objv[], Components component[]){
	char cmd[44];
	char* modelID;
	modelID = Tcl_GetString(objv[1]);
	cout << modelID << endl;
    int Ncomp=model(interp,objc-2,objv+2,component);
	sprintf(cmd,"%d",Ncomp);
	Tcl_SetVar(interp,"Ncomponent",cmd,1);
	if (Ncomp!=0) Tcl_EvalEx(interp,"set LoadCmd 1",-1,TCL_EVAL_GLOBAL);
    for (int i=Ncomp; i<MaxComponent; i++){
		component[i].name=0;
		sprintf(cmd,"name(%d)",i);
		Tcl_UpdateLinkedVar(interp,cmd);

		sprintf(cmd,"set name(%d) {}",i);
        Tcl_EvalEx(interp,cmd,-1,TCL_EVAL_GLOBAL);
		sprintf(cmd,"delete sim %d",i);
        Tcl_EvalEx(interp,cmd,-1,TCL_EVAL_GLOBAL);
	}
	for (int i=0;i<Ncomp;i++){
		sprintf(cmd,"delete sim %d",i);
        Tcl_EvalEx(interp,cmd,-1,TCL_EVAL_GLOBAL);
		sprintf(cmd,"name(%d)",i);
		Tcl_UpdateLinkedVar(interp,cmd);
		sprintf(cmd,"natpgr(%d)",i);
		Tcl_UpdateLinkedVar(interp,cmd);
		sprintf(cmd,"nat(%d)",i);
		Tcl_UpdateLinkedVar(interp,cmd);
		sprintf(cmd,"ngr(%d)",i);
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
//	for (int i=0; i<Ncomp; i++) cout << component[i].name << endl;
	printf("LoadCmd completed\n");
    return TCL_OK;
}
*/
/************************************************************************/

// reads data from simulation files
int ReadDataNew(Tcl_Interp *interp,char newfile[],char dataf[],int head,Components comp[],double xsim[],int report_err){
	string line;
	int Np=0;
	ifstream fdat(dataf);
	if (!fdat) {
//		cout << "Error open file: "<< dataf << endl;
		return Np;
	}
	int i,j,k,l,n,end;
	for (i=0; i<head; i++){
		getline(fdat,line);
//		cout << line << endl;
	}
	getline(fdat,line);
	char *cline = new char[strlen(line.c_str()) + 1];
	strcpy (cline, line.c_str());
	int Ncolumns=0;
	int Ncomp=atoi(Tcl_GetVar(interp,"Ncomponent",1));
	char *tmp = strtok(cline,"	 ,");
	while ( tmp != NULL ) {
		Ncolumns++;
		tmp = strtok(NULL,"	 ,");
	}
//	Columns column[MaxColumn];
	Columns *column=(Columns*)calloc(Ncolumns,sizeof(Columns));

	string err,err2,err_tmp,err_nel,err_nsl;
	err="";
	err2="";
	err_nel="";
	err_nsl="";
	strcpy (cline, line.c_str());
	tmp = strtok(cline,"	 ,");
	for (n=0; n<Ncolumns; n++) {
		strcpy(column[n].name,tmp);
		for (i=0; i<MaxAtomChar; i++){
			if (column[n].name[i] >= 'a' && column[n].name[i] <= 'z') column[n].name[i] += 'A' - 'a';
		}
//		cout << n <<" - "<< column[n].name << endl;
		j=0;
		k=0;
		l=0;
		end=0;
		for (j=0; j<Ncomp; j++) {
			for (k=0; k<comp[j].Natom; k++) {
				if ( string(column[n].name) == string(comp[j].atom[k]) ) {
					column[n].component[l]=comp[j].id;
					l++;
					if (newfile[0]=='n' && newfile[1]=='e' && newfile[2]=='w'){
						column[n].nel=atom_to_nel(interp,column[n].name[0],err_tmp);
						err_nel += err_tmp;
						column[n].nsl=atom_to_nsl(interp,column[n].name[0],err_tmp);
						err_nsl += err_tmp;
					} else if (newfile[0]=='r' && newfile[1]=='e') {
						cout << "An internal error" << endl;
//						column[n].nel=comp[j].nelpgr/comp[j].natpgr;
//						column[n].nsl=comp[j].nslpgr/comp[j].natpgr;
					}
				}
			}
		}
		column[n].Ncomp=l;
		if (l==0) {
			if (column[n].name[0]!='Z'){
//				err += "WARNING: Atom " + string(column[n].name) + " does not belong to any component";
				err += string(column[n].name) ;
				err += " ";
//				cout << err << endl;
//				char *cmd2 = new char[strlen(err.c_str()) + 1];
//				sprintf(cmd2,"tk_messageBox -title warning -message \"%s\" -type ok -icon warning",err.c_str());
//				Tcl_EvalEx(interp,cmd2,-1,TCL_EVAL_GLOBAL);
//				delete[] cmd2;
			}
			column[n].component[l]=MaxComponent;
		} else if (l>1) {
			err2 += string(column[n].name);
			err2 += " ";
		}
		tmp = strtok(NULL,"	 ,");
	}
	if (err!="" && report_err==1){
		err = "WARNING: Atoms " + err + "do not belong to any component";
		cout << err << endl;
		err = "tk_messageBox -title warning -message \"" + err + "\" -type ok -icon warning";
		char *cmd2 = new char[strlen(err.c_str()) + 1];
		strcpy(cmd2,err.c_str());
//		cout << cmd2 << endl;
		Tcl_EvalEx(interp,cmd2,-1,TCL_EVAL_GLOBAL);
		delete[] cmd2;
	}
	if (err2!="" && report_err==1){
		err2 = "WARNING: Atoms " + err2 + "are used more than once";
		cout << err2 << endl;
		err2 = "tk_messageBox -title warning -message \"" + err2 + "\" -type ok -icon warning";
		char *cmd2 = new char[strlen(err2.c_str()) + 1];
		strcpy(cmd2,err2.c_str());
//		cout << cmd2 << endl;
		Tcl_EvalEx(interp,cmd2,-1,TCL_EVAL_GLOBAL);
		delete[] cmd2;
	}
	if (err_nel!="" && report_err==1){
		err_nel = "WARNING: nEL not defined for " + err_nel;
		cout << err_nel << endl;
		err_nel = "tk_messageBox -title warning -message \"" + err_nel + "\" -type ok -icon warning";
		char *cmd2 = new char[strlen(err_nel.c_str()) + 1];
		strcpy(cmd2,err_nel.c_str());
//		cout << cmd2 << endl;
		Tcl_EvalEx(interp,cmd2,-1,TCL_EVAL_GLOBAL);
		delete[] cmd2;
	}
	if (err_nsl!="" && report_err==1){
		err_nsl = "WARNING: NSL not defined for " + err_nsl;
		cout << err_nsl << endl;
		err_nsl = "tk_messageBox -title warning -message \"" + err_nsl + "\" -type ok -icon warning";
		char *cmd2 = new char[strlen(err_nsl.c_str()) + 1];
		strcpy(cmd2,err_nsl.c_str());
//		cout << cmd2 << endl;
		Tcl_EvalEx(interp,cmd2,-1,TCL_EVAL_GLOBAL);
		delete[] cmd2;
	}
	
	delete[] cline;
	double data;
	j=0;
	for (n=1; n<Ncolumns; n++) column[n].partial=0.;
	while ( !getline(fdat,line,'\n').eof() || line!="") {
	  if (line!="") {
		istringstream iss2(line);
		string tmps;
		iss2 >> tmps;
		if (tmps=="partial") {
			for (n=1; n<Ncolumns; n++){
				iss2 >> data;
				column[n].partial=data;
//				cout << n <<" "<< data << endl;
			}
		} else {
		for (i=0; i<Ncomp; i++){
			comp[i].ysim[j] = 0;
			comp[i].yed[j] = 0;
			comp[i].ynsld[j] = 0;
		}
		istringstream iss(line);
		for (n=0; n<Ncolumns; n++){
			iss >> data;
//			cout << i <<" "<< data << endl;
			if (column[n].name[0]=='Z'){
				xsim[j]=atof(Tcl_GetVar(interp,"Zdirection",1))*(data-atof(Tcl_GetVar(interp,"Zcentre",1)));
			} else {
				for (l=0; l<column[n].Ncomp; l++){
					comp[column[n].component[l]].ysim[j] += data;
					comp[column[n].component[l]].yed[j] += ((atoi(Tcl_GetVar(interp,"partialc",1))?column[n].partial:0.)+column[n].nel)*data;
					comp[column[n].component[l]].ynsld[j] += column[n].nsl*data;
					comp[column[n].component[l]].ysimbak[j] = comp[column[n].component[l]].ysim[j];
				}
			}
//			cout << column[n].name[0] <<" "<< atom(column[n].name[0]) << endl;	
		}
		j++;
		line.clear();
		}
	  }
	}
	Np=j;
	double binstep = 0.;
	for (i=1; i<Np; i++){
		binstep += xsim[i]-xsim[i-1];
	}
	binstep = (Np<2 ? 0 : binstep/(Np-1));
	char cmd[44];
	sprintf(cmd,"%lf",binstep);
	Tcl_SetVar(interp,"binstep",cmd,1);
	cout << "Number of points = " << Np << endl;
	fdat.close();
	free(column);
	return Np;
}
int ReadDataTOT(Tcl_Interp *interp,char dataf[],int head,double xsim[],double yed_tot[],double ynsld_tot[],
				Blt_Vector *yaff[],int report_err){
	string line;
	int Np=0;
	ifstream fdat(dataf);
	if (!fdat) {
//		cout << "Error open file: "<< dataf << endl;
		return Np;
	}
	int i,j,k,n;
	for (i=0; i<head; i++){
		getline(fdat,line);
//		cout << line << endl;
	}
	getline(fdat,line);
	char *cline = new char[strlen(line.c_str()) + 1];
	strcpy (cline, line.c_str());
//	cout << cline << endl;
	int Ncolumns=0;
	char *tmp = strtok(cline,"	 ,");
	while ( tmp != NULL ) {
		Ncolumns++;
		tmp = strtok(NULL,"	 ,");
	}
//	Columns column[MaxColumn];
	Columns *column=(Columns*)calloc(Ncolumns,sizeof(Columns));

	string err_tmp,err_nel,err_nsl,err_aff;
	err_nel="";
	err_nsl="";
	strcpy (cline, line.c_str());
	tmp = strtok(cline,"	 ,");
	for (n=0; n<Ncolumns; n++) {
		strcpy(column[n].name,tmp);
		for (i=0; i<MaxAtomChar; i++){
			if (column[n].name[i] >= 'a' && column[n].name[i] <= 'z') column[n].name[i] += 'A' - 'a';
		}
//		cout << n <<" - "<< column[n].name << endl;
		j=0;
		k=0;
		column[n].nel=atom_to_nel(interp,column[n].name[0],err_tmp);
		err_nel += err_tmp;
		column[n].nsl=atom_to_nsl(interp,column[n].name[0],err_tmp);
		err_nsl += err_tmp;
		tmp = strtok(NULL,"	 ,");
	}
	delete[] cline;
	if (err_nel!="" && report_err==1){
		err_nel = "WARNING: nEL not defined for " + err_nel;
		cout << err_nel << endl;
		err_nel = "tk_messageBox -title warning -message \"" + err_nel + "\" -type ok -icon warning";
		char *cmd2 = new char[strlen(err_nel.c_str()) + 1];
		strcpy(cmd2,err_nel.c_str());
//		cout << cmd2 << endl;
		Tcl_EvalEx(interp,cmd2,-1,TCL_EVAL_GLOBAL);
		delete[] cmd2;
	}
	if (err_nsl!="" && report_err==1){
		err_nsl = "WARNING: NSL not defined for " + err_nsl;
		cout << err_nsl << endl;
		err_nsl = "tk_messageBox -title warning -message \"" + err_nsl + "\" -type ok -icon warning";
		char *cmd2 = new char[strlen(err_nsl.c_str()) + 1];
		strcpy(cmd2,err_nsl.c_str());
//		cout << cmd2 << endl;
		Tcl_EvalEx(interp,cmd2,-1,TCL_EVAL_GLOBAL);
		delete[] cmd2;
	}

	double data;
	double yaff_[MaxAFF][MaxPoint];
	err_aff="";
	j=0;
	for (n=1; n<Ncolumns; n++) column[n].partial=0.;
	while ( !getline(fdat,line,'\n').eof() || line!="") {
	  if (line!="") {
		istringstream iss2(line);
		string tmps;
		iss2 >> tmps;
		if (tmps=="partial") {
			for (n=1; n<Ncolumns; n++){
				iss2 >> data;
				column[n].partial=data;
//				cout << n <<" "<< data <<" "<< column[n].partial << endl;
			}
		} else {
//		ysim_tot[j]=0;
		yed_tot[j]=0;
		ynsld_tot[j]=0;
		for (i=0; i<MaxAFF; i++) yaff_[i][j]=0.0;
		istringstream iss(line);
		for (n=0; n<Ncolumns; n++){
			iss >> data;
//			cout << n <<" "<< data <<" "<< column[n].parcial << endl;
			if (column[n].name[0]=='Z'){
				xsim[j]=atof(Tcl_GetVar(interp,"Zdirection",1))*(data-atof(Tcl_GetVar(interp,"Zcentre",1)));
//				cout << column[n].name[0] << endl;
			} else {
				char ctmp[6];
				string ctemp;
				k=0;
				int end=0;
				int nid=0;
				nid=atoi(Tcl_GetVar(interp,"nid",1));
				while (end!=1 && k<nid){
					sprintf(ctmp,"id(%d)",k);
					ctemp=Tcl_GetVar(interp,ctmp,1);
					if (column[n].name[0]==ctemp[0]){
						yaff_[k][j] += ((atoi(Tcl_GetVar(interp,"partialc",1))?column[n].partial:0.)+column[n].nel)/column[n].nel*data;
						end=1;
					} else {
						k++;
					}		
				}
				if (end==0) {
//					char err[44],cmd[144];
//					sprintf(err,"WARNING: AFF not defined for %c",column[n].name[0]);
//					cout << err << endl;
//					sprintf(cmd,"tk_messageBox -title warning -message \"%s\" -type ok -icon warning",err);
//					Tcl_EvalEx(interp,cmd,-1,TCL_EVAL_GLOBAL);
					err_aff += column[n].name[0];
					err_aff += " ";
				}
				yed_tot[j] += ((atoi(Tcl_GetVar(interp,"partialc",1))?column[n].partial:0.)+column[n].nel)*data;
				ynsld_tot[j] += column[n].nsl*data;
//				cout << j <<" "<< n <<" "<< column[n].nel <<" "<< yed_tot[j] <<" "<< ynsld_tot[j] << endl;
			}
		}
//		cout << j <<" "<< yed_tot[j] <<""<< ynsld_tot[j] << endl;
		j++;
		line.clear();
		}
	  }
	}
	if (err_aff!="" && report_err==1){
		err_aff = "WARNING: AFF not defined for " + err_aff;
		cout << err_aff << endl;
		err_aff = "tk_messageBox -title warning -message \"" + err_aff + "\" -type ok -icon warning";
		char *cmd2 = new char[strlen(err_aff.c_str()) + 1];
		strcpy(cmd2,err_aff.c_str());
//		cout << cmd2 << endl;
		Tcl_EvalEx(interp,cmd2,-1,TCL_EVAL_GLOBAL);
		delete[] cmd2;
	}

	Np=j;
	for (i=0;i<MaxAFF;i++){
		Blt_ResetVector(yaff[i],yaff_[i],Np,MaxPoint, TCL_VOLATILE);
	}
	double binstep = 0.;
	for (i=1; i<Np; i++){
		binstep += xsim[i]-xsim[i-1];
	}
	binstep = (Np<2 ? 0 : binstep/(Np-1));
	char cmd[44];
	sprintf(cmd,"%lf",binstep);
	Tcl_SetVar(interp,"binstep",cmd,1);
//	Tcl_SetVar(interp,"Ncomponent","0",1);
	cout << "Number of points = " << Np << endl;
	fdat.close();
	free(column);
	return Np;
}
int NK_LoadDataNew(Tcl_Interp *interp,int objc, Tcl_Obj *const objv[], Components component[],
				   Blt_Vector *xBLT,Blt_Vector *ysim[],Blt_Vector *yed[],Blt_Vector *ynsld[],
				   Blt_Vector *yaff[]){
	char cmd[144];
	if(objc<3) return TCL_OK;
	char *dataf=Tcl_GetString(objv[2]);
//	cout << dataf << endl;
	int report_err;
	Tcl_GetIntFromObj(interp,objv[3],&report_err);
	ifstream fdat(dataf);
	if (!fdat) {
		string err = "Error open file: " + string(dataf);
		cout << err << endl;
		char cmd[144];
		sprintf(cmd,"tk_messageBox -title warning -message \"%s\" -type ok -icon warning",err.c_str());
		Tcl_EvalEx(interp,cmd,-1,TCL_EVAL_GLOBAL);
		return TCL_OK;
	}
	fdat.close();
	if (atoi(Tcl_GetVar(interp,"atomf",1))){
		FourierTransform Fourier;
		atomff aff[MaxAFF];
		int nid=Fourier.AFFcorrection(interp,yaff,aff);
		for (int k=0; k<nid; k++){
			aff[k].fi0 = aff[k]._atomff(0.0);
			sprintf(cmd,"set e(%d) %lf",k,aff[k].fi0);
			Tcl_EvalEx(interp,cmd,-1,TCL_EVAL_GLOBAL);
		}
	}
	double xsim[MaxPoint],yed_tot[MaxPoint],ynsld_tot[MaxPoint];
	int Np,head;
	head=ReadHead(interp,dataf);
	Np=ReadDataTOT(interp,dataf,head,xsim,yed_tot,ynsld_tot,yaff,report_err);
	if (atoi(Tcl_GetVar(interp,"symmetrized",1))){
		symmetrize(yed_tot,Np);
		symmetrize(ynsld_tot,Np);
		for (int k=0; k<MaxAFF; k++){
			double *yaff_=yaff[k]->valueArr;
			symmetrize(yaff_,Np);
			Blt_ResetVector(yaff[k],yaff_,Np,MaxPoint, TCL_VOLATILE);
		}
	}
	if (atoi(Tcl_GetVar(interp,"LoadCmd",1))) {
		Np=ReadDataNew(interp,Tcl_GetString(objv[1]),dataf,head,component,xsim,report_err);
		if (atoi(Tcl_GetVar(interp,"symmetrized",1))){
			for (int k=0; k<atoi(Tcl_GetVar(interp,"Ncomponent",1)); k++){
				symmetrize(component[k].ysim, Np);
				symmetrize(component[k].yed, Np);
				symmetrize(component[k].ynsld, Np);
				symmetrize(component[k].ysimbak, Np);
			}
		}
	}
	if (Np!=0) Tcl_EvalEx(interp,"set LoadData 1",-1,TCL_EVAL_GLOBAL);
//	Tcl_UpdateLinkedVar(interp,"binstep");
	int Ncomp=atoi(Tcl_GetVar(interp,"Ncomponent",1));
	if (Np==0) Ncomp=0;
	sprintf(cmd,"%d",Ncomp);
	Tcl_SetVar(interp,"Ncomponent",cmd,1);
	for (int i=0;i<Ncomp;i++){
		sprintf(cmd,"create sim \"%s\" %d",component[i].name,i);
        Tcl_EvalEx(interp,cmd,-1,TCL_EVAL_GLOBAL);
		Blt_ResetVector(ysim[i],component[i].ysim,Np,MaxPoint, TCL_VOLATILE);
		Blt_ResetVector(yed[i],component[i].yed,Np,MaxPoint, TCL_VOLATILE);
		Blt_ResetVector(ynsld[i],component[i].ynsld,Np,MaxPoint, TCL_VOLATILE);
	}
	sprintf(cmd,"create sim total %d",MaxComponent);
    Tcl_EvalEx(interp,cmd,-1,TCL_EVAL_GLOBAL);
	Blt_ResetVector(xBLT,xsim,Np,MaxPoint, TCL_VOLATILE);
//	Blt_ResetVector(ysim[MaxComponent],ysim_tot,Np,MaxPoint, TCL_VOLATILE);
	Blt_ResetVector(yed[MaxComponent],yed_tot,Np,MaxPoint, TCL_VOLATILE);
	Blt_ResetVector(ynsld[MaxComponent],ynsld_tot,Np,MaxPoint, TCL_VOLATILE);
	printf("LoadDataNew completed\n");
    return TCL_OK;
}
/************************************************************************/

int ReadDataExp(char dataf[],int head,double qFE[],double FE[],double eFE[]){
  char str[144];
  int Nqexp=0;
  ifstream fdat(dataf);
  if (!fdat) {
//	  cout << "Error open file: "<< dataf << endl;
	  return Nqexp;
  }
  int i,j=0;
  for (i=0; i<head; i++){
	  fdat.getline(str,144,'\n');
  }
  int tmp=0;
  while (fdat.getline(str,144,'\n')!=0){
	  tmp = sscanf(str,"%lf %lf %lf",&qFE[j],&FE[j],&eFE[j]);
	  if (tmp==2) {eFE[j] = 1.; j++;}
	  if (tmp==3) j++;
  }
  Nqexp=j;
  cout << "Number of exp points = " << Nqexp << endl;
  fdat.close();
  return Nqexp;
}
int NK_LoadDataX(Tcl_Interp *interp,int objc, Tcl_Obj *const objv[],
				 Blt_Vector *qFExblt[], Blt_Vector *xFEblt[], Blt_Vector *eFExblt[]){
	char cmd[144];
	if(objc<3) return TCL_OK;
	char *dataf=Tcl_GetString(objv[1]);
	ifstream fdat(dataf);
	if (!fdat) {
		string err = "Error open file: " + string(dataf);
		cout << err << endl;
		sprintf(cmd,"tk_messageBox -title warning -message \"%s\" -type ok -icon warning",err.c_str());
		Tcl_EvalEx(interp,cmd,-1,TCL_EVAL_GLOBAL);
		return TCL_OK;
	}
	fdat.close();
	double qFEx[MaxFF],xFE[MaxFF],eFEx[MaxFF];
	int kind;
	Tcl_GetIntFromObj(interp,objv[2],&kind);
	int Nqex,head;
	head=ReadHead(interp,dataf);
	Nqex=ReadDataExp(dataf,head,qFEx,xFE,eFEx);
	stringstream tmp;
	tmp << "create X \"" << dataf <<"\" "<< kind << endl;
	string cmd2=tmp.str();
	Tcl_EvalEx(interp,cmd2.c_str(),-1,TCL_EVAL_GLOBAL);
	Blt_ResetVector(qFExblt[kind],qFEx,Nqex,MaxFF, TCL_VOLATILE);
	Blt_ResetVector(xFEblt[kind],xFE,Nqex,MaxFF, TCL_VOLATILE);
	Blt_ResetVector(eFExblt[kind],eFEx,Nqex,MaxFF, TCL_VOLATILE);
	printf("LoadDataX completed\n");
    return TCL_OK;
}

int NK_LoadDataN(Tcl_Interp *interp,int objc, Tcl_Obj *const objv[],
				 Blt_Vector *qFEnblt[], Blt_Vector *nFEblt[], Blt_Vector *eFEnblt[]){
	char cmd[144];
	if(objc<3) return TCL_OK;
	char *dataf=Tcl_GetString(objv[1]);
	ifstream fdat(dataf);
	if (!fdat) {
		string err = "Error open file: " + string(dataf);
		cout << err << endl;
		sprintf(cmd,"tk_messageBox -title warning -message \"%s\" -type ok -icon warning",err.c_str());
		Tcl_EvalEx(interp,cmd,-1,TCL_EVAL_GLOBAL);
		return TCL_OK;
	}
	fdat.close();
	double qFEn[MaxFF],nFE[MaxFF],eFEn[MaxFF];
	int kind;
	Tcl_GetIntFromObj(interp,objv[2],&kind);
	int Nqen,head;
	head=ReadHead(interp,dataf);
    Nqen=ReadDataExp(dataf,head,qFEn,nFE,eFEn);
	stringstream tmp;
	tmp << "create N \"" << dataf <<"\" "<< kind << endl;
	string cmd2=tmp.str();
	Tcl_EvalEx(interp,cmd2.c_str(),-1,TCL_EVAL_GLOBAL);
	Blt_ResetVector(qFEnblt[kind],qFEn,Nqen,MaxFF, TCL_VOLATILE);
	Blt_ResetVector(nFEblt[kind],nFE,Nqen,MaxFF, TCL_VOLATILE);
	Blt_ResetVector(eFEnblt[kind],eFEn,Nqen,MaxFF, TCL_VOLATILE);
	printf("LoadDataN completed\n");
    return TCL_OK;
}
/************************************************************************/

int ReadSDP(Tcl_Interp *interp,char dataf[],double xsdp[],SDPcomp comp[], int *Nc){
  char str[144];
  int Np=0;
  ifstream fdat(dataf);
  if (!fdat) {
	  string err = "Error open file: " + string(dataf);
	  cout << err << endl;
	  char cmd[144];
	  sprintf(cmd,"tk_messageBox -title warning -message \"%s\" -type ok -icon warning",err.c_str());
	  Tcl_EvalEx(interp,cmd,-1,TCL_EVAL_GLOBAL);
	  return Np;
  }
  fdat.close();
  int head=ReadHead(interp,dataf);
  double a[MaxComponent+2];
  char c[MaxComponent+2][16];
  for (int i=0;i<MaxComponent+2;i++) c[i][0]='%';
  int i,j=0;
  fdat.clear();fdat.open(dataf);
  for (i=0; i<head; i++){
	  fdat.getline(str,144,'\n');
  }
  if (fdat.getline(str,144,'\n')!=0){
      sscanf(str,"%s %s %s %s %s %s %s %s %s %s %s %s",c[0],c[1],c[2],c[3],c[4],c[5],c[6],c[7],c[8],c[9],c[10],c[11]);
//      for (int i=0;i<12;i++) printf("%s %c \n",c[i],c[i][0]);
      while (j<MaxComponent+2 && c[j][0]!='%') {
//          printf("%d %s\n",j,c[j]);
          j++;
      }
  }
  int Ncomp=j-1;
  cout << "Number of components = " << Ncomp << endl;
  for (int i=0;i<Ncomp;i++){
	  sprintf(comp[i].name,c[i+1]);
	  printf("%s\n",comp[i].name);
  }
  *Nc = Ncomp;
  j=0;
  while (fdat.getline(str,144,'\n')!=0){
      sscanf(str,"%lf %lf %lf %lf %lf %lf %lf %lf %lf %lf %lf %lf",&a[0],&a[1],&a[2],&a[3],&a[4],&a[5],&a[6],&a[7],&a[8],&a[9],&a[10],&a[11]);
	  xsdp[j]=a[0];
	  for (int i=0;i<Ncomp;i++){
		  comp[i].sdp[j]=a[i+1];
	  }
      j++;
  }
  Np=j;
  cout << "Number of points = " << Np << endl;
  fdat.close();
  return Np;
}
int NK_LoadSDPx(Tcl_Interp *interp,int objc, Tcl_Obj *const objv[],
				Blt_Vector *xedEXP, Blt_Vector *yedEXP[]){
	char cmd[44];
	double xed[MaxPoint];
	SDPcomp component[MaxComponent+1];
	if(objc<2) return TCL_OK;
	int Ncomp=0;
	int Npx=ReadSDP(interp,Tcl_GetString(objv[1]),xed,component,&Ncomp);
	for (int i=0;i<Ncomp;i++){
		sprintf(cmd,"create edp \"%s\" %d",component[i].name,i);
        Tcl_EvalEx(interp,cmd,-1,TCL_EVAL_GLOBAL);
		Blt_ResetVector(yedEXP[i],component[i].sdp,Npx,MaxPoint, TCL_VOLATILE);
	}
	Blt_ResetVector(xedEXP,xed,Npx,MaxPoint, TCL_VOLATILE);
//	printf("LoadSDPx completed\n");
    return TCL_OK;
}

int NK_LoadSDPn(Tcl_Interp *interp,int objc, Tcl_Obj *const objv[],
				Blt_Vector *xnsldEXP, Blt_Vector *ynsldEXP[]){
	char cmd[44];
	double xnsld[MaxPoint];
	SDPcomp component[MaxComponent+1];
	if(objc<2) return TCL_OK;
	int Ncomp=0;
	int Npn=ReadSDP(interp,Tcl_GetString(objv[1]),xnsld,component,&Ncomp);
	for (int i=0;i<Ncomp;i++){
		sprintf(cmd,"create nsldp \"%s\" %d",component[i].name,i);
        Tcl_EvalEx(interp,cmd,-1,TCL_EVAL_GLOBAL);
		Blt_ResetVector(ynsldEXP[i],component[i].sdp,Npn,MaxPoint, TCL_VOLATILE);
	}
	Blt_ResetVector(xnsldEXP,xnsld,Npn,MaxPoint, TCL_VOLATILE);
//	printf("LoadSDPx completed\n");
    return TCL_OK;
}
/************************************************************************/
