
class assignments {
public:
    list<int*> val;
    int n;
    
    assignments() {
    }
    
    assignments(int x) {
      if(x == 1) {
        
      }
    }
    
    void add(int *element) {
      val.push_back(element);
    }
    
    bool equals(int* a, int* b) {
      for (int i = 0; i < n; i++) {
        if (a[i] != b[i]) {
           return 0;
    	}
      }
      return 1;
    }
    
    void remove(int *element) {
      list<int*>::iterator it = val.begin();
      while(it != val.end()) {
          if(equals(element, *it)) {
            it = val.erase(it);
          }
          it++;
      }
    }
    
    bool IsPresent(int *element) {
      list<int*>::iterator it = val.begin();
      //coDesign eq;
      while(it != val.end()) {
          if(equals(element, *it)) {
            return true;
          }
          it++;
      }
      return false;
    }
    
    int *GetDelay() {
      return val.front();
    }
    
    void printElement(int *elem) {
      mexPrintf("(");
      for(int i = 0; i < 3; i++) {
        mexPrintf("%d,", elem[i]);
      }
      mexPrintf(")");
    }
    
    void print() {
      size_t len = val.size();
      mexPrintf("\n Inner Basis of size %d\n", len);
      list<int*>::iterator it = val.begin();
      while(it != val.end()) {
        printElement(*it);  
        it++;
      }
    }
};

