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
    
    int car_sum=0;
    float average_power_best=0;
    float average_power_min=0;
    float average_power_threshold[20]={0};
    int handoff_best=0;
    int handoff_min=0;
    int handoff_threshold[20]={0};
    //graph variable
    int handoff_best_time[86400]={0};
    int handoff_threshold_time[86400]={0};

    //system time for loop
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

        //policy
        bool first_hand_off=false;
        int car_array_size=car_array.size();
        for(int i=0;i<car_array_size;i++){
            int car_x_index=car_array[i].x/10;
            int car_y_index=car_array[i].y/10;
            float strong_one=0;
            int get_gain=car_array[i].best_gain;
            if(get_gain==-1)
                first_hand_off=true;
            for(int j=0;j<4;j++){
                float get_dbm=gain_dbm[car_x_index][car_y_index][j];
                if(get_dbm>strong_one){
                    strong_one=get_dbm;
                    get_gain=j;
                }                  
            }
            //best policy
            if(car_array[i].best_policy<10 || car_array[i].best_policy<strong_one){
                if(get_gain!=car_array[i].best_gain && !first_hand_off){
                    handoff_best++;
                    handoff_best_time[i]++;
                }
                car_array[i].best_gain=get_gain;
                car_array[i].best_policy=strong_one;
            }
            else
                car_array[i].best_policy=gain_dbm[car_x_index][car_y_index][car_array[i].best_gain];
            
            //min policy
            if(car_array[i].min_policy<10){
                if(get_gain!=car_array[i].min_gain && !first_hand_off)
                    handoff_min++;
                car_array[i].min_gain=get_gain;
                car_array[i].min_policy=strong_one;
            }
            else
                car_array[i].min_policy=gain_dbm[car_x_index][car_y_index][car_array[i].min_gain];
            
            //threshold: start at t=11
            for(int t=0;t<20;t++){
                if(car_array[i].threshold_policy[t]<10 || (car_array[i].threshold_policy[t]<strong_one && car_array[i].threshold_policy[t]<t+11)){   
                    if(get_gain!=car_array[i].threshold_gain[t] && !first_hand_off){
                        handoff_threshold[t]++;
                        if(t==4)
                            handoff_threshold_time[i]++;
                    }    
                    car_array[i].threshold_gain[t]=get_gain;
                    car_array[i].threshold_policy[t]=strong_one;
                }
                else
                    car_array[i].threshold_policy[t]=gain_dbm[car_x_index][car_y_index][car_array[i].threshold_gain[t]];
            }
        }
        //cout<<car_array[0].best_policy<<" x: "<<car_array[0].x<<" y: "<<car_array[0].y<<"  "<<car_array[0].threshold_gain[0]<<endl;

        //count power
        float one_time_power_best=0;
        float one_time_power_min=0;
        float one_time_power_threshold[20]={0};
        for(int i=0;i<car_array_size;i++){
            one_time_power_best+=car_array[i].best_policy;
            one_time_power_min+=car_array[i].min_policy;
            for(int t=0;t<20;t++){
                one_time_power_threshold[t]+=car_array[i].threshold_policy[t];
            }
        }    
        average_power_best+=one_time_power_best/car_array_size;
        average_power_min+=one_time_power_min/car_array_size;
        for(int t=0;t<20;t++){
            average_power_threshold[t]+=one_time_power_threshold[t]/car_array_size;
        }
    }
    average_power_best/=86400.0;
    average_power_min/=86400.0;
    float average_power_middle=(average_power_min+average_power_best)/2;
    for(int t=0;t<20;t++){
        average_power_threshold[t]/=86400.0;
    }

    //output
    int best_T=0;
    cout<<"handoff_best: "<<handoff_best<<endl;
    cout<<"average_power_best: "<<average_power_best<<endl;
    cout<<"handoff_min: "<<handoff_min<<endl;
    cout<<"average_power_min: "<<average_power_min<<endl;
    for(int t=0;t<20;t++){
        if(average_power_threshold[t]>average_power_middle){
            best_T=t+11;
            cout<<"handoff_threshold t="<<t+11<<" :"<<handoff_threshold[t]<<endl;
            cout<<"average_power_threshold t="<<t+11<<" :"<<average_power_threshold[t]<<endl;
            break;
        }
    }
}

void generate_strength(int x,int y,int n,float (&data)[101][101][4]){
    for(int i=0;i<1010;i+=10){
        for(int j=0;j<1010;j+=10)
            data[i/10][j/10][n]=67-20*log10(sqrt(pow(x-i,2)+pow(y-j,2)));
    }
}