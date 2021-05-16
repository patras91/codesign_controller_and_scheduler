class perf {

 public:

  double p0, p1, p2;

  void print() {
    cout <<" P0 = "<<p0<<", P1 = "<<p1<<", P2 = "<<p2<<endl;
	mexPrintf("P0 = %lf, P1 = %lf, P2 = %lf\n", p0, p1, p2);
  }
};

