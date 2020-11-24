#include <iostream>
#include <cstdlib>
#include <random>
#include <ctime>
#include <math.h>
#include <fstream>
#include <vector>
#include "car.h"
using namespace std;

void generate_strength(int x,int y,int n,float (&data)[101][101][4]);

int main(){
    //compute every gain dBm
    float gain_dbm[101][101][4]={0};
    generate_strength(330,350,0,gain_dbm);
    generate_strength(640,310,1,gain_dbm);
    generate_strength(360,680,2,gain_dbm);
    generate_strength(660,660,3,gain_dbm);

    //car move random
    default_random_engine generator_move(time(NULL));
    uniform_real_distribution<double> uniform_distribution_move(0.0,1.0);
    //poisson random
    default_random_engine generator(time(NULL));
    uniform_real_distribution<double> uniform_distribution(0.0,1.0);
    float arrival_rate=0.5;
    float come_prob=1-exp(-arrival_rate*0.01);
    vector<Car> car_array;
    
    //system time for loop
    int car_sum=0;
    float average_power=0;
    int handoff_all=0;
    for(int system_time=0;system_time<86400;system_time++){
        //car move
        for(int i=0;i<car_array.size();i++){
            if(car_array[i].at_corner==10||car_array[i].at_corner==0){
                car_array[i].at_corner=1;
                float move_p=uniform_distribution_move(generator_move); 
                //straight
                if(move_p <= 0.6){
                    if(car_array[i].direction==0)
                        car_array[i].x+=10;
                    else if(car_array[i].direction==1)
                        car_array[i].y+=10;
                    else if(car_array[i].direction==2)
                        car_array[i].x-=10;
                    else if(car_array[i].direction==3)
                        car_array[i].y-=10;
                }
                //right
                else if(move_p > 0.6 && move_p <= 0.8){
                    if(car_array[i].direction==0){
                        car_array[i].y-=10;
                        car_array[i].direction=3;
                    }
                    else if(car_array[i].direction==1){
                        car_array[i].x+=10;
                        car_array[i].direction=0;
                    } 
                    else if(car_array[i].direction==2){
                        car_array[i].y+=10;
                        car_array[i].direction=1;
                    }
                    else if(car_array[i].direction==3){
                        car_array[i].x-=10;
                        car_array[i].direction=2;
                    }   
                }
                //left
                else if(move_p > 0.8 && move_p <= 1.0){
                    if(car_array[i].direction==0){
                        car_array[i].y+=10;
                        car_array[i].direction=1;
                    }
                    else if(car_array[i].direction==1){
                        car_array[i].x-=10;
                        car_array[i].direction=2;
                    } 
                    else if(car_array[i].direction==2){
                        car_array[i].y-=10;
                        car_array[i].direction=3;
                    }
                    else if(car_array[i].direction==3){
                        car_array[i].x+=10;
                        car_array[i].direction=0;
                    }   
                }
            }
            else{
                if(car_array[i].direction==0)
                    car_array[i].x+=10;
                else if(car_array[i].direction==1)
                    car_array[i].y+=10;
                else if(car_array[i].direction==2)
                    car_array[i].x-=10;
                else if(car_array[i].direction==3)
                    car_array[i].y-=10;
                car_array[i].at_corner++;
            }
        }

        //car remove
        int resize_number=1;
        for(int i=0;i<car_array.size()-resize_number+1;i++){
            if(car_array[i].at_corner==1)
                if(car_array[i].x>1000 || car_array[i].x<0 || car_array[i].y>1000 || car_array[i].y<0){
                    car_array[i]=car_array[car_array.size()-resize_number];
                    resize_number++;
                    i--;
                    car_sum--;
                }    
        }
        car_array.resize(car_sum,Car(0,0,0));

        //poisson arrive
        int car_number[4][9];
        for(int i=0;i<4;i++){
            for(int j=0;j<9;j++)
                car_number[i][j]=0;
        }
        for(int i=0;i<9;i++){
            for(float t=0;t<100;t++){
                if(uniform_distribution(generator)<come_prob){
                    car_number[0][i]++;
                    car_sum++;
                }
                if(uniform_distribution(generator)<come_prob){
                    car_number[1][i]++;
                    car_sum++;
                }                    
                if(uniform_distribution(generator)<come_prob){
                    car_number[2][i]++;
                    car_sum++;
                }
                if(uniform_distribution(generator)<come_prob){
                    car_number[3][i]++;
                    car_sum++;
                }
            }
        }

        //car enter
        car_array.reserve(car_sum);
        for(int i=0;i<9;i++){
            for(int j=0;j<car_number[0][i];j++)
                car_array.push_back(Car(0,(i+1)*100,0));
            for(int j=0;j<car_number[1][i];j++)
                car_array.push_back(Car((i+1)*100,0,1));
            for(int j=0;j<car_number[2][i];j++)
                car_array.push_back(Car(1000,(i+1)*100,2));
            for(int j=0;j<car_number[3][i];j++)
                car_array.push_back(Car((i+1)*100,1000,3));
        }

        //best policy
        bool first_hand_off=false;
        for(int i=0;i<car_array.size();i++){
            float strong_one=0;
            int get_gain=car_array[i].best_gain;
            if(get_gain==-1)
                first_hand_off=true;
            for(int j=0;j<4;j++){
                float get_dbm=gain_dbm[car_array[i].x/10][car_array[i].y/10][j];
                if(get_dbm>strong_one){
                    strong_one=get_dbm;
                    get_gain=j;
                }                  
            }
            if(get_gain!=car_array[i].best_gain){
                if(!first_hand_off)
                    handoff_all++;
            }
            car_array[i].best_gain=get_gain;
            car_array[i].best_policy=strong_one;
        }
        //cout<<car_array[0].best_policy<<" x: "<<car_array[0].x<<" y: "<<car_array[0].y<<"  "<<car_array[0].best_gain<<endl;

        //count power
        float one_time_power=0;
        for(int i=0;i<car_array.size();i++)
            one_time_power+=car_array[i].best_policy;
        average_power+=(one_time_power/car_array.size());
    }
    cout<<"handoff_all: "<<handoff_all<<endl;
    cout<<"average_power: "<<average_power/86400.0<<endl;
}

void generate_strength(int x,int y,int n,float (&data)[101][101][4]){
    for(int i=0;i<1010;i+=10){
        for(int j=0;j<1010;j+=10)
            data[i/10][j/10][n]=67-20*log10(sqrt(pow(x-i,2)+pow(y-j,2)));
    }
}