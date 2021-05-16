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

void FindBasisDCRec(int* cur_low, int* cur_upp, int* lower, int* upper, assignments* basis, assignments* ibasis, time_t start_time, FILE* fp, int new_n);

void GenerateChildInstances(int* sat, int* lower, int* upper, assignments* basis, assignments* ibasis, time_t start_time, FILE* fp, int new_n) {
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
	  if (upper[j] - lower[j] <= 1) {
		low[j] = lower[j];
		upp[j] = upper[j];
		uppm1[j] = upp[j] - 1;
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
	  if (upp[j] - low[j] > 1) {
		neww_n++;
	  }
	  m++;
	  uppm1[j] = upp[j] - 1;
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
	  FindBasisDCRec(NULL, NULL, low, upp, basis, ibasis, start_time, fp, neww_n);
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

void FindBasisDCRec(int* cur_low, int* cur_upp, int* lower, int* upper, assignments* basis, assignments* ibasis, time_t start_time, FILE* fp, int new_n) {
	/*printf("----------dc\n");
	print(cur_low);
	print(cur_upp);
	print(lower);
	print(upper);
	printf("---------dco\n");*/
  int* low = cur_low == NULL ? lower : cur_low;
  int* upp = cur_upp == NULL ? upper : cur_upp;
  int* mid = getMidPoint(low, upp);
  bool geq;
  if (basis->IsPresent(mid)) {
	geq = 1;
  } else if (ibasis->IsPresent(mid)) {
	geq = 0;
  } else {
    geq = getF(mid);
    if (!geq) {
	  ibasis->remove(mid);
	  ibasis->add(copy(mid));
	}
  }
  //int f = getF(mid);
  if (geq) {  // f >= key) {
	Delete(cur_upp);
	if (!equals(cur_low, lower, mid)) {
      FindBasisDCRec(cur_low, mid, lower, upper, basis, ibasis, start_time, fp, new_n);
	  return;
	}
	Delete(cur_low);
	GenerateChildInstances(mid, lower, upper, basis, ibasis, start_time, fp, new_n);
	return;
  }
  Delete(cur_low);
  if (!close(cur_upp, upper, mid)) {
    FindBasisDCRec(mid, cur_upp, lower, upper, basis, ibasis, start_time, fp, new_n);
	return;
  }
  delete mid;
  if (cur_upp) {
	GenerateChildInstances(cur_upp, lower, upper, basis, ibasis, start_time, fp, new_n);
  } else {
	delete lower;
	delete upper;
  }
}

assignments* FindBasisDC(FILE* fp) {
  time_t start_time;
  start_time = time(NULL);	
	
  assignments* basis = new assignments();
  assignments* ibasis = new assignments(1);
  int* low = copy(lower);
  int* upp = new int[n];
  int new_n = 0;
  for (int i = 0; i < n; i++) {
	upp[i] = upper[i] + 1;
	if (upp[i] - low[i] < 1) {
	  printf("error\n");
	}
	if (upp[i] - low[i] > 1) {
	  new_n++;
	}
  }
  encodedAssigns = NULL;
  populateEncodedAssigns();
  FindBasisDCRec(NULL, NULL, low, upp, basis, ibasis, start_time, fp, new_n);
  deleteEncodedAssigns();
  time_t end_time = time(NULL);
  //fprintf(fp, "%d %d\n", basis->Size(), int(end_time - start_time));
  //fflush(fp);
  delete ibasis;
  return basis;
}
