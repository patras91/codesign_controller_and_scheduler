class coDesign {

 public:
 
  int n;
  list<double*> basis;
  coDesign(int numberOfControllers) {
    n = numberOfControllers;
  }
/*
  perf SimulateCircuit (config conf) {
    perf p;
    unsigned int isseed = (unsigned int)time(NULL);
    srand(isseed);
    p.p0 = rand()%100;
    p.p1 = rand()%100;
    p.p2 = rand()%100;
  
    return p;

  }*/ 

  config* StartConfig() {
	  config *start = new config(n);
	  for(int i = 0; i<n; i++) {
	    start->K[i] = MINK;
		start->b[i] = MINb;
		start->s[i] = MINs; 
	  }
      start->calcC();
	  return start;
  }
  
  config* StartConfig(double *c, double **delay) {
	  config *start = new config(n);
	  for(int i = 0; i<n; i++) {
	    start->K[i] = MINK;
		start->b[i] = MINb;
		start->s[i] = c[i]; 
        for(int j = 0; j < 3; j++) {
          start->delay[i][j] = delay[i][j];
        }
	  }
      mexPrintf("here\n");
      start->calcC();
	  return start;
  }
    
  double* StartConfigSlots() {
	  double *start = new double[n];
	  for(int i = 0; i<n; i++) {
        start[i] = MINs; 
	  }
	  return start;
  }

  void Simulate(config *c, FILE *f, int controller, int stage) {

    mxArray *parameters[2];
    parameters[0] = mxCreateNumericMatrix(1, 4, mxDOUBLE_CLASS, mxREAL);
    parameters[1] = mxCreateNumericMatrix(1, 9, mxDOUBLE_CLASS, mxREAL);
    double **param = c->GetParameters(parameters, controller, stage);
  
    mxArray *outputPerf[1];

    outputPerf[0] =  mxCreateNumericMatrix(1, 3, mxSINGLE_CLASS, mxREAL);

    int nout = 1, nin = 2;

    mexCallMATLAB(nout, outputPerf, nin, parameters, "simulator_v3");
    c->SetPerformance(outputPerf, controller);
    c->print(f, controller);
  }

  bool NotDuplicate(list<config*> *q, config *c) {
    bool notDup = true;
    list<config*>::iterator it;
    
    for(it = q->begin(); it != q->end(); it++) {
      if((*it)->equals(c)) {
        notDup = false;
        return notDup;
      }
    }
    return notDup;
  }
  
  void GenerateChildrenForSlot(double *c, list<double*> *q) {
    int j = n-1;
    double *child = new double[n];
    for(int i = 0; i<n; i++) {
      child[i] = c[i];
    }
    while(child[j]>=MAXs) {
      j--;
    }
    child[j] = child[j]+STEPs;
    for(int k = j+1; k<n; k++) {
      child[k] = MINs;
    }
    q->push_back(child);
  }
  
  void GenerateChildren(config *c, list<config*> *q, int controller) {
    config *child = new config(*c);
	if(c->b[controller] < MAXb) {
      child->b[controller] = c->b[controller] + STEPb;
    } else {
      child->K[controller] = c->K[controller] + STEPK;
      child->b[controller] = MINb;
    }
    q->push_back(child);
  }

  bool IsLastSlotConfiguration(double *c) {
    for(int i = 0; i < n; i++) {
      if(c[i] < MAXs) {
        return false;
      }
    }
    return true;
  }
  
  bool IsLastConfiguration(config *c, int controller) {
	if(c->K[controller] < MAXK || c->b[controller] < MAXb) {
	    return false;
	}
    return true;
  }
  
  bool CalculateFeasibility(double *c, double **delay) {
    mxArray *parameters[1];
    parameters[0] = mxCreateNumericMatrix(1, n, mxDOUBLE_CLASS, mxREAL);

    double **param = new double*[1];

    param[0] = mxGetPr(parameters[0]);
    for(int i = 0; i < n; i++) {
      param[0][i] = c[i];
    }    

    mxArray *outputD[4];

    outputD[0] = mxCreateNumericMatrix(1, 1, mxSINGLE_CLASS, mxREAL);
    outputD[1] = mxCreateNumericMatrix(1, 3, mxSINGLE_CLASS, mxREAL);
    outputD[2] = mxCreateNumericMatrix(1, 3, mxSINGLE_CLASS, mxREAL);
    outputD[3] = mxCreateNumericMatrix(1, 3, mxSINGLE_CLASS, mxREAL);

    int nout = 4, nin = 1;

    mexCallMATLAB(nout, outputD, nin, parameters, "delayIndicator");

    //mexPrintf("Back to mex after calculating delay.\n ");
    
    double *discard = (double *)mxGetData(outputD[0]);
    
    if(discard[0]>0) {
      return false;
    } else {
      delay[0] = (double *)mxGetData(outputD[1]);
      delay[1] = (double *)mxGetData(outputD[2]);
      delay[2] = (double *)mxGetData(outputD[3]);
      return true;
    }
  }
  
  config* better(config *c1, config *c2, int i) {
    double sum1 = 0;
    double sum2 = 0;
    //for(int i = 0; i < n; i++) {
      sum1 += c1->p[i].p0/100 + c1->p[i].p1/10 + c1->p[i].p2/100;
      sum2 += c2->p[i].p0/100 + c2->p[i].p1/10 + c2->p[i].p2/100;
    //}
    
    if(sum2 < sum1) {
      c1->copy(c2);
    }
    return c1;      
  }
  
  config* FindBestPossibleForSlot(double *c, double **delay, FILE *f, int controller, int stage) {
    list<config*> q;
    config *start = StartConfig(c, delay);
    q.push_back(start);
    config *bestConfig = new config(*start);
    
    //for(int i = 0; i < n; i++) {
      bestConfig->p[controller].p0 = 99999999;
      bestConfig->p[controller].p1 = 99999999;
      bestConfig->p[controller].p2 = 99999999;
    //}
    
    while(!q.empty()) {
      config *c = q.front();
      Simulate(c, f, controller, stage);
      q.pop_front();
      bestConfig = better(bestConfig, c, controller);
      if(IsLastConfiguration(c,controller) == true) {
        break;
      } else {
        GenerateChildren(c, &q, controller);
      }
      delete c;
    }
    return bestConfig;
  }
  
  config FindBestConfig() {
    
    FILE **f = new FILE*[n];
    for (int i = 0; i < n; i++) {
      char fname[200];
      sprintf(fname, "v4/v4_controller%d_1.txt", i+1);
      f[i] = fopen(fname,"w");
    }
    list<double*> q;
    startTime = time(NULL);
	double *start = StartConfigSlots();

	q.push_back(start);

	while(!q.empty()) {
	  double *c = q.front();
      double **delay = new double*[n];
      for(int i = 0; i < n; i++) {
        delay[i] = new double[3];
      }
      bool feasible = CalculateFeasibility(c, delay);
	  q.pop_front();
      if(feasible == true) {
        for(int i = 0; i < n; i++) {
          for(int j = 0; j < n; j++) {
            fprintf(f[i], "%lf ", c[j]);
          }
          fprintf(f[i],"\n");
          for(int j = 0; j < 3; j++) {
            fprintf(f[i], "%lf ", delay[i][j]);
          }
          fprintf(f[i], "\n");
          FindBestPossibleForSlot(c, delay, f[i], i, 1);
        }
      } 
	  if(IsLastSlotConfiguration(c)) {
		mexPrintf("Reached Last Configuration");
	    break;
	  } else {
        GenerateChildrenForSlot(c, &q);
      }
	  delete c;
	}
    for(int i = 0; i<n;i++) {
      fclose(f[i]);
    }
  }
  
  double* middle (double *low, double *high) {
    double *res = new double[n];
    for(int i = 0; i < n; i++) {
      res[i] = (low[i] + high[i]) /2;
    }
    return res;
  }
  
  void UpdateInnerBasis(double *m) {
      
  }
  
  bool GetRelationship(double *mid, perf px) {
      return true;
  }
  
  void CalculateRecursiveBasis(double *start, double *end, perf px) {
     //low = start;
     //high = end;
     double *low, *high;
     for(int i = 0; i < n; i++) {
       low[i] = start[i];
       high[i] = end[i];
     }
     bool contBSearch = true;
     while(contBSearch ) {
       double *mid = middle(low, high);
       bool relation = GetRelationship(mid, px); //relation is true if a_mid >= px
       if(relation) {
           high = mid;
       } else {
           low = mid;
           UpdateInnerBasis(mid);
       }
     }
  }
  
  void CalculateBasis() {
     perf px;
     px.p0 = 10;
     px.p1 = 1;
     px.p2 = 20;
     
     double *start = new double[n];
     double *end = new double[n];
     
     for(int i = 0; i < n; i++) {
       start[i] = 0; 
       end[i] = 1;
     }
     
     CalculateRecursiveBasis(start, end, px);
     
  }
  void Delete(int* assign) {
  if (assign) {
	delete assign;
  }
}

int* getMidPoint(int* low, int* upp) {
  int* mid = new int[n];
  for (int i = 0; i < n; i++) {
	mid[i] = (low[i] + upp[i]) / 2;
  }
  return mid;
}

bool equals(int* a, int* b) {
  for (int i = 0; i < n; i++) {
	if (a[i] != b[i]) {
	  return 0;
	}
  }
  return 1;
}

int** encodedAssigns;
int numEncodedAssigns;

void fillEncodedAssigns(int n) {
  if (n == 2) {
	encodedAssigns[0][0] = 0;
	encodedAssigns[0][1] = 1;
	
	encodedAssigns[1][0] = 1;
	encodedAssigns[1][1] = 0;
	return;
  }
  if (n == 3) {
	encodedAssigns[0][0] = 0;
	encodedAssigns[0][1] = 2;
	encodedAssigns[0][2] = 1;
	
	encodedAssigns[1][0] = 1;
	encodedAssigns[1][1] = 0;
	encodedAssigns[1][2] = 2;
	
	encodedAssigns[2][0] = 2;
	encodedAssigns[2][1] = 1;
	encodedAssigns[2][2] = 0;
	return;
  }
  fillEncodedAssigns(n - 1);
  for (int i = 0; i < 2 * (n - 1) - 3; i++) {
	encodedAssigns[i][n - 1] = 2;
  }
  for (int i = 0; i < n - 1; i++) {
	encodedAssigns[2 * n - 5][i] = 0;
	encodedAssigns[2 * n - 4][i] = 1;
  }
  encodedAssigns[2 * n - 5][n - 1] = 1;
  encodedAssigns[2 * n - 4][n - 1] = 0;
}

void print(int* assign) {
  if (!assign) {
	printf("NULL\n");
	return;
  }
  for (int j = 0; j < n; j++) {
	printf("%d ", assign[j]);;
  }
  printf("\n");
}

void populateEncodedAssigns() {
  if (encodedAssigns) {
	return;
  }
  numEncodedAssigns = n == 2 ? n : 2 * n - 3;  
  encodedAssigns = new int*[numEncodedAssigns];
  for (int i = 0; i < numEncodedAssigns; i++) {
	encodedAssigns[i] = new int[n];
  }
  
  fillEncodedAssigns(n);
  /*
  for (int i = 0; i < numEncodedAssigns; i++) {
	print(encodedAssigns[i]);
  }*/
}

void deleteEncodedAssigns() {
  for (int i = 0; i < numEncodedAssigns; i++) {
	delete encodedAssigns[i];
  }
  delete encodedAssigns;
}

bool anyEquals(int* a, int* b) {
  for (int i = 0; i < n; i++) {
	if (a[i] == b[i]) {
	  return 1;
	}
  }
  return 0;
}

int *copy(int *elem) {
  int *newelem = new int[n];
  
  for(int i = 0; i < n; i++) {
    newelem[i] = elem[i];
  }
  
  return newelem;
}
//void FindBasisDCRec(int* cur_low, int* cur_upp, int* lower, int* upper, assignments* basis, assignments* ibasis, time_t start_time, FILE* fp, int new_n);

void GenerateChildInstances(int* sat, int* lower, int* upper, assignments* basis, assignments* ibasis, time_t start_time, int new_n) {
  //print(sat);
  if (new_n <= 1 || equals(sat, lower)) {
	if (!basis->IsPresent(sat)) {
	  basis->remove(sat);
      time_t end_time = time(NULL);
      //fprintf(fp, "%d %d\n", basis->Size() + 1, int(end_time - start_time));
      //fflush(fp);
      basis->add(sat);
    } else {
	  delete sat;
    }
    delete lower;
    delete upper;
    return;
  }
  int numEncodedAssigns = new_n == 2 ? 2 : 2 * new_n - 3;
  for (int i = 0; i < numEncodedAssigns; i++) {
	int* low = new int[n];
	int* upp = new int[n];
	int* uppm1 = new int[n];
	int neww_n = 0;
	int m = 0;
	bool isValid = 1;
	for (int j = 0; j < n; j++) {
	  //~ if (upper[j] - lower[j] <= 1) {
	  if (- upper[j] + lower[j] <= 1) {
		low[j] = lower[j];
		upp[j] = upper[j];
		//~ uppm1[j] = upp[j] - 1;
		uppm1[j] = upp[j] + 1;
		continue;
	  }
	  if (encodedAssigns[i][m] == 0) {
		low[j] = lower[j];
		upp[j] = sat[j];
		if (low[j] == upp[j]) {
		  isValid = 0;
		  break;
		}
	  } else if (encodedAssigns[i][m] == 1 || (numEncodedAssigns == 2 && i == 0 && m == 1)) {
		low[j] = sat[j];
		upp[j] = upper[j];
	  } else {
		low[j] = lower[j];
		upp[j] = upper[j];
	  }
	  //~ if (upp[j] - low[j] > 1) {
	  if (- upp[j] + low[j] > 1) {
		neww_n++;
	  }
	  m++;
	  //~ uppm1[j] = upp[j] - 1;
	  uppm1[j] = upp[j] + 1;
    }
    //assert(m == new_n);
    /*printf("--------s\n");
    print(low);
    print(upp);
    printf("neww n : %d\n", neww_n);*/
	if (!isValid || basis->IsPresent(low) || ibasis->IsPresent(uppm1)) {
	  delete low;
	  delete upp;
	  delete uppm1;
	} else {
	  delete uppm1;
      basis->add(copy(sat));
	  FindBasisDCRec(NULL, NULL, low, upp, basis, ibasis, start_time, neww_n);
    }
    //printf("--------e\n");
  }
  
  basis->remove(sat);
  if (!basis->IsPresent(sat)) {
    time_t end_time = time(NULL);
    //fprintf(fp, "%d %d\n", basis->Size() + 1, int(end_time - start_time));
    //fflush(fp);
    basis->add(sat);
  } else {
	delete sat;
  }
  delete lower;
  delete upper;
}

bool close(int* a, int* b) {
  for (int i = 0; i < n; i++) {
	if (a[i] -  b[i] > 1 || b[i] - a[i] > 1) {
	  return 0;
	}
  }
  return 1;
}

bool close(int* cur_bound, int* bound, int* mid) {
  if (cur_bound) {
    return close(cur_bound, mid);
  }
  return close(bound, mid);
}

bool equals(int* cur_bound, int* bound, int* mid) {
  if (cur_bound) {
    return close(cur_bound, mid);
  }
  return equals(bound, mid);
}

bool getF(int *integerDelay) {
  //config best = FindBestPossibleForSlot(c, delay, f, controller);
  double *c;
  c = new double[n];
  config *globalBest = new config(n);
  FILE **f = new FILE*[n];
  for (int i = 0; i < n; i++) {
    char fname[200];
    sprintf(fname, "results/controller%d.txt", i+1);
    f[i] = fopen(fname,"w");
  }
      
  double **delay = new double*[n];
  for(int i = 0; i < n; i++) {
    delay[i] = new double[3];
  }
  
  delay[0][0] = integerDelay[0]/3000.0;
  delay[0][1] = integerDelay[0]/3000.0;
  delay[0][2] = integerDelay[0]/3000.0;
  
  delay[1][0] = integerDelay[1]/2000.0;
  delay[1][1] = integerDelay[1]/2000.0;
  delay[1][2] = integerDelay[1]/2000.0;
  
  delay[2][0] = integerDelay[2]/2000.0;
  delay[2][1] = integerDelay[2]/2000.0;
  delay[2][2] = integerDelay[2]/2000.0;
  
  for(int i = 0; i < n; i++) {
    for(int j = 0; j < n; j++) {
      c[j] = 0;
      fprintf(f[i], "%lf ", c[j]);
    }
    fprintf(f[i],"\n");
    for(int j = 0; j < 3; j++) {
      fprintf(f[i], "%lf ", delay[i][j]);
      mexPrintf("\ndelay (%d ,%d) = %lf",i,j, delay[i][j]);
    }
    fprintf(f[i], "\n");
    config *best = FindBestPossibleForSlot(c, delay, f[i], i, 0);
    globalBest->K[i] = best->K[i];
    globalBest->b[i] = best->b[i];
    globalBest->p[i].p0 = best->p[i].p0;
    globalBest->p[i].p1 = best->p[i].p1;
    globalBest->p[i].p2 = best->p[i].p2;
  }
  
  for(int i = 0 ; i < n ; i ++) {
    fclose(f[i]);
  }
  
  double totalPerf = 0;
  for(int i = 0; i < n; i++) {
    totalPerf += globalBest->p[i].p0/100 + globalBest->p[i].p1/10 + globalBest->p[i].p2/100;
  }
  mexPrintf("Total Performance = %lf\n", totalPerf);
  if (totalPerf <= REFERENCE) {
    return true;
  } else {
    return false;
  }
}

void printD(int *elem) {
  if(elem == NULL) return;
  mexPrintf("(");
  for(int i = 0; i < 3; i++) {
    mexPrintf("%d,",elem[i]);
  }
  mexPrintf(")");
}

void FindBasisDCRec(int* cur_low, int* cur_upp, int* lower, int* upper, assignments* basis, assignments* ibasis, time_t start_time, int new_n) {
	printf("----------dc\n");
	printD(cur_low);
	printD(cur_upp);
	printD(lower);
	printD(upper);
	printf("---------dco\n");
  int* low = cur_low == NULL ? lower : cur_low;
  int* upp = cur_upp == NULL ? upper : cur_upp;
  int* mid = getMidPoint(low, upp);
  bool geq;       //mid geq REF
  if (basis->IsPresent(mid)) {
    geq = 1;
  } else if (ibasis->IsPresent(mid)) {
    geq = 0;
  } else {
    geq = getF(mid);  //question: will reach here only when mid is not present in ibasis?
    if (!geq) {
      ibasis->remove(mid);
      ibasis->add(copy(mid));
    }
  }
  //int f = getF(mid);
  if (geq) {  // f >= key) {
    Delete(cur_upp);
    if (!equals(cur_low, lower, mid)) {
      FindBasisDCRec(cur_low, mid, lower, upper, basis, ibasis, start_time, new_n);
	  return;
	}
	Delete(cur_low);
	GenerateChildInstances(mid, lower, upper, basis, ibasis, start_time, new_n);
	return;
  }
  Delete(cur_low);
  if (!close(cur_upp, upper, mid)) {
    FindBasisDCRec(mid, cur_upp, lower, upper, basis, ibasis, start_time, new_n);
	return;
  }
  delete mid;
  if (cur_upp) {
	GenerateChildInstances(cur_upp, lower, upper, basis, ibasis, start_time, new_n);
  } else {
	delete lower;
	delete upper;
  }
}

assignments* FindBasisDC(int *lower, int *upper) {
  time_t start_time;
  start_time = time(NULL);	
	
  assignments* basis = new assignments();
  assignments* ibasis = new assignments(1);
  int* low = copy(lower);
  int* upp = new int[n];
  int new_n = 0;
  for (int i = 0; i < n; i++) {
    //~ upp[i] = upper[i] + 1;
    upp[i] = upper[i] - 1;
    //~ if (upp[i] - low[i] < 1) {
    if (low[i] - upp[i] < 1) {
      printf("error\n");
    }
    //~ if (upp[i] - low[i] > 1) {
    if (low[i] - upp[i] > 1) {
      new_n++;
    }
  }
  encodedAssigns = NULL;
  populateEncodedAssigns();
  FindBasisDCRec(NULL, NULL, low, upp, basis, ibasis, start_time, new_n);
  deleteEncodedAssigns();
  time_t end_time = time(NULL);
  //fprintf(fp, "%d %d\n", basis->Size(), int(end_time - start_time));
  //fflush(fp);
  //delete ibasis;
  return ibasis;
}

void FindSchedulerSlots(assignments *ibasis) {
  int *intDelay = ibasis->GetDelay();
  
  FILE **f = new FILE*[n];
  for (int i = 0; i < n; i++) {
    char fname[200];
    sprintf(fname, "v4/v4_opt_controller%d_1.txt", i+1);
    f[i] = fopen(fname,"w");
  }
  list<double*> q;
  startTime = time(NULL);
  double *start = StartConfigSlots();

  q.push_back(start);
mexPrintf("second part begins...\n");
  while(!q.empty()) {
    mexPrintf("second part\n");
	double *c = q.front();
    double **delay = new double*[n];
    for(int i = 0; i < n; i++) {
      delay[i] = new double[3];
    }
    bool feasible = CalculateFeasibility(c, delay);
	q.pop_front();
    if(feasible == true) {
      mexPrintf("\n Feasible solution found");
      bool cont = true;
      for(int i = 0; i < n; i++) {
        double temp_delay = 0;
        for(int j = 0; j < 3; j++) {
          temp_delay += delay[i][j];
        }
        if(temp_delay > intDelay[i]/1000.0) {
          mexPrintf("\nFalse %lf %lf ",temp_delay, intDelay[i]/1000.0);
          cont = false;
        }
      }
        
      if(cont == true) {
        for(int i = 0; i < n; i++) {
          for(int j = 0; j < n; j++) {
            fprintf(f[i], "%lf ", c[j]);
          }
          fprintf(f[i],"\n");
          for(int j = 0; j < 3; j++) {
            fprintf(f[i], "%lf ", delay[i][j]);
          }
          fprintf(f[i], "\n");
          FindBestPossibleForSlot(c, delay, f[i], i, 1);
        }
        break;
      } 
    }
	  if(IsLastSlotConfiguration(c)) {
	    mexPrintf("Reached Last Configuration");
	    break;
	  } else {
        GenerateChildrenForSlot(c, &q);
      }
	  delete c;
	}
    for(int i = 0; i<n;i++) {
      fclose(f[i]);
    }
}

void FindBestConfigOpt()  {
  int *lower = new int[n];
  int *upper = new int[n];
 
  for(int i = 0; i < n; i++) {
    lower[i] = 140;
    upper[i] = 70;
  }
  
  assignments *ibasis = FindBasisDC(lower, upper);
  mexPrintf("Elements of the inner basis are \n");
  ibasis->print();
  FindSchedulerSlots(ibasis);
  
}

};

