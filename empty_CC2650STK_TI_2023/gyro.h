#define INIT_AVG_ARR_VALUE { 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0 }

struct avgArray {
    float arr[10];
    float avg;
    int i;
};

void initMPU920();
void initAvgArray(struct avgArray *avgArrayPar);
void updateAvgArray(struct avgArray *avgArrayPar, float val);
float calcAvg(float arr[]);
