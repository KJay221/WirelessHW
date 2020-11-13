#include <iostream>
#include <cstdlib>
#include <random>
#include <ctime>
#include <math.h>
#include <fstream>
#include "car.h"
using namespace std;

void generate_strength(int x,int y,int n,float (&data)[100][100][4]);

int main(){
    //compute every gain dBm
    float gain_dbm[100][100][4]={0};
    generate_strength(330,350,0,gain_dbm);
    generate_strength(640,310,1,gain_dbm);
    generate_strength(360,680,2,gain_dbm);
    generate_strength(660,658,3,gain_dbm);

    //poisson random
    default_random_engine generator(time(NULL));
    uniform_real_distribution<double> uniform_distribution(0.0,1.0);
    float arrival_rate=0.5;
    float come_prob=1-exp(-arrival_rate*0.01);
    //system time for loop
    for(int system_time=0;system_time<20;system_time++){
        //poisson arrive
        int car_number[4][9];
        for(int i=0;i<4;i++){
            for(int j=0;j<9;j++)
                car_number[i][j]=0;
        }
        for(int i=0;i<9;i++){
            for(float t=0;t<100;t++){
                if(uniform_distribution(generator)<come_prob)
                    car_number[0][i]++;
                if(uniform_distribution(generator)<come_prob)
                    car_number[1][i]++;
                if(uniform_distribution(generator)<come_prob)
                    car_number[2][i]++;
                if(uniform_distribution(generator)<come_prob)
                    car_number[3][i]++;
            }
        }
    }
}

void generate_strength(int x,int y,int n,float (&data)[100][100][4]){
    for(int i=0;i<1000;i+=10){
        for(int j=0;j<1000;j+=10){
            data[i/10][j/10][n]=67-20*log10(sqrt(pow(x-i,2)+pow(y-j,2)));
        }
    }
}