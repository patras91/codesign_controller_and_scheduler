#include <stdio.h>
#include <mex.h>
#include <matrix.h>
#include <iostream>
#include <string.h>
using namespace std;
#include <time.h>
#include <stdlib.h>
#include <list>

#include "specs.hxx"
#include "assignments.cpp"
#include "Performance.cpp"
#include "config.cpp"
#include "codesign.cpp"


void mexFunction( int nlhs, mxArray *plhs[], int nrhs, const mxArray *prhs[] ) {
 
  cout << "Co-design of Controller and Scheduler"<<nrhs<<endl;
  cout << "Sunandita Patra"<<endl;
  cout << "sunandita.patra@cse.iitkgp.ernet.in"<<endl;
  
  mexPrintf("Co-design of Controller and Scheduler %d\n", nrhs);
  mexPrintf( "Sunandita Patra \n sunandita.patra@cse.iitkgp.ernet.in\n");
  double *np = (double *)mxGetData(prhs[0]);
  int n = (int)np[0];

  mexPrintf("Number of controllers = %d\n",n);
  coDesign COD(n);
  //config c = COD.FindBestConfig();
  COD.FindBestConfigOpt();

}

