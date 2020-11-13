#include <iostream>
#include <cstdlib>
#include <random>
#include <ctime>
#include <math.h>
#include <fstream>
#include "car.h"
using namespace std;

void generate_strength(int x,int y,int (&data)[100][100][4]);

int main(){
    int gain_dbm[100][100][4]={0};
    gain_dbm[0][0][1]=1;
    generate_strength(1,1,gain_dbm);
    Car carArray[2];
    carArray[0].threshold_gain=145;
    cout<<" "<<carArray[0].threshold_gain;
}

void generate_strength(int x,int y,int (&data)[100][100][4]){
    cout<<data[0][0][1];
}