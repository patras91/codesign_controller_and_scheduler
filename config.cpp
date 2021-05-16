class config {

 public:

  double *K, *b;
  double *s;
  double C;
  perf *p;
  double **delay;
  int n;
  
  config(int numberOfControllers) {
    n = numberOfControllers;
    K = new double[n]; 
    b = new double[n];
    s = new double[n];
    p = new perf[n];
    delay = new double*[n];
    for(int i = 0; i < n; i++) {
      delay[i] = new double[3];
    }
	//K[0] = 0.01;
	//b[0] = 0.01;
	//s[0] = 1;
  }

  config(const config &c) {
	n = c.n;
    K = new double[n]; 
    b = new double[n];
    s = new double[n];
    p = new perf[n];
    delay = new double*[n];
    for(int i = 0; i < n; i++) {
      delay[i] = new double[3];
    }
	for(int i = 0; i < n; i++) {
	  K[i] = c.K[i];
	  b[i] = c.b[i];
	  s[i] = c.s[i];
      delay[i][0] = c.delay[i][0];
      delay[i][1] = c.delay[i][1];
      delay[i][2] = c.delay[i][2];
	}
    C = c.C;
  }

  bool equals(config *c) {
    bool equal = true;
    for(int i = 0; i < n ; i++) {
      if(K[i] != c->K[i] || b[i] != c->b[i] || s[i] != c->s[i]) {
        equal = false;
        return equal;
      }
    }
    return equal;
  }
  
  void print (FILE *f, int controller) {
    fprintf(f, "%lf %lf %lf %lf %lf %u\n", K[controller],b[controller], p[controller].p0, p[controller].p1, p[controller].p2, (unsigned int)(time(NULL) - startTime));
  }

  double **GetParameters(mxArray *parameters[], int controller, int stage) {
    double **param = new double*[2];
	param[0] = mxGetPr(parameters[0]);
    param[0][0] = K[controller];
    param[0][1] = b[controller];
    param[0][2] = controller + 1;
    param[0][3] = stage;
    param[1] = mxGetPr(parameters[1]);
    for(int i = 0; i < n; i++) {
      for(int j = 0; j < 3; j++) {
        param[1][3*i+j] = delay[i][j];
      }
    }
    return param; 
  }

  void SetPerformance(mxArray **performance, int controller) {
    double *P = (double *)mxGetData(performance[0]);
    p[controller].p0 = P[0];
    p[controller].p1 = P[1];
    p[controller].p2 = P[2];
  }
  
  bool IsLastConfig() {
	bool IsLastConfig = true;
    for(int i = 0; i < n; i++) {
	  if(K[i]<MAXK) {
	    IsLastConfig = false;
		break;
	  }
	  if(b[i]<MAXb) {
	    IsLastConfig = false;
		break;
	  }
	  if(s[i]<MAXs) {
	    IsLastConfig = false;
		break;
	  }
	}
	return IsLastConfig;
  }
  
  void calcC() {
    C = 0;
    for(int i = 0; i < n; i++) {
      C += s[i];
    }
  }
  
  void copy(config *c) {
    n = c->n;
	for(int i = 0; i < n; i++) {
	  K[i] = c->K[i];
	  b[i] = c->b[i];
	  s[i] = c->s[i];
      delay[i][0] = c->delay[i][0];
      delay[i][1] = c->delay[i][1];
      delay[i][2] = c->delay[i][2];
      p[i].p0 = c->p[i].p0;
      p[i].p1 = c->p[i].p1;
      p[i].p2 = c->p[i].p2;
	}
    C = c->C;
  }
  
};

class sconfig {

 public:

  double K, b;
  perf p;
  double *delay;
  
  sconfig(int nd, int delay) {
    delay = new double[nd];
    for(int i = 0; i < n; i++) {
      delay[i] = new double[3];
    }
	//K[0] = 0.01;
	//b[0] = 0.01;
	//s[0] = 1;
  }

  config(const config &c) {
	n = c.n;
    K = new double[n]; 
    b = new double[n];
    s = new double[n];
    p = new perf[n];
    delay = new double*[n];
    for(int i = 0; i < n; i++) {
      delay[i] = new double[3];
    }
	for(int i = 0; i < n; i++) {
	  K[i] = c.K[i];
	  b[i] = c.b[i];
	  s[i] = c.s[i];
      delay[i][0] = c.delay[i][0];
      delay[i][1] = c.delay[i][1];
      delay[i][2] = c.delay[i][2];
	}
    C = c.C;
  }

  bool equals(config *c) {
    bool equal = true;
    for(int i = 0; i < n ; i++) {
      if(K[i] != c->K[i] || b[i] != c->b[i] || s[i] != c->s[i]) {
        equal = false;
        return equal;
      }
    }
    return equal;
  }
  
  void print (FILE *f, int controller) {
    fprintf(f, "%lf %lf %lf %lf %lf %u\n", K[controller],b[controller], p[controller].p0, p[controller].p1, p[controller].p2, (unsigned int)(time(NULL) - startTime));
  }

  double **GetParameters(mxArray *parameters[], int controller, int stage) {
    double **param = new double*[2];
	param[0] = mxGetPr(parameters[0]);
    param[0][0] = K[controller];
    param[0][1] = b[controller];
    param[0][2] = controller + 1;
    param[0][3] = stage;
    param[1] = mxGetPr(parameters[1]);
    for(int i = 0; i < n; i++) {
      for(int j = 0; j < 3; j++) {
        param[1][3*i+j] = delay[i][j];
      }
    }
    return param; 
  }

  void SetPerformance(mxArray **performance, int controller) {
    double *P = (double *)mxGetData(performance[0]);
    p[controller].p0 = P[0];
    p[controller].p1 = P[1];
    p[controller].p2 = P[2];
  }
  
  bool IsLastConfig() {
	bool IsLastConfig = true;
    for(int i = 0; i < n; i++) {
	  if(K[i]<MAXK) {
	    IsLastConfig = false;
		break;
	  }
	  if(b[i]<MAXb) {
	    IsLastConfig = false;
		break;
	  }
	  if(s[i]<MAXs) {
	    IsLastConfig = false;
		break;
	  }
	}
	return IsLastConfig;
  }
  
  void calcC() {
    C = 0;
    for(int i = 0; i < n; i++) {
      C += s[i];
    }
  }
  
  void copy(config *c) {
    n = c->n;
	for(int i = 0; i < n; i++) {
	  K[i] = c->K[i];
	  b[i] = c->b[i];
	  s[i] = c->s[i];
      delay[i][0] = c->delay[i][0];
      delay[i][1] = c->delay[i][1];
      delay[i][2] = c->delay[i][2];
      p[i].p0 = c->p[i].p0;
      p[i].p1 = c->p[i].p1;
      p[i].p2 = c->p[i].p2;
	}
    C = c->C;
  }
  
};

