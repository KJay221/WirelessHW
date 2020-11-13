#include <iostream>
#include <cstdlib>
#include <random>
#include <ctime>
#include <math.h>
#include <fstream>
using namespace std;

int main(){
    //prob
    int s_number[3]={1,5,10};
    float table[3][4][6];
    int t1=-1;
    for(int s=0;s<3;s++){
        t1++;
        int service_number=s_number[s];
        int t2=-1;
        for(float arrival_rate=0.01;arrival_rate<=10;arrival_rate*=10){
            t2++;
            int t3=-1;
            for(float service_rate=0.01;service_rate<=10.24;service_rate*=4){
                t3++;
                int* service=new int[service_number]();
                float d_time=0.01;
                if(arrival_rate<=0.1||service_rate<=0.1)
                    float d_time=0.1;
                if(arrival_rate<=0.01||service_rate<=0.01)
                    float d_time=1;

                float call_number=0;
                float blocking_number=0;
                float departure_number=0;

                default_random_engine generator(time(NULL)),out_generator(time(NULL));
                uniform_real_distribution<double> uniform_distribution(0.0,1.0);

                for(float t=0;t<100000/d_time;t++){
                    float come_prob=1-exp(-arrival_rate*d_time);
                    float come_rand=uniform_distribution(generator);
                    if(come_rand<come_prob){
                        int block=1;
                        call_number++;
                        for(int i=0;i<service_number;i++){
                            if(service[i]==0){
                                service[i]=1;
                                block=0;
                                break;
                            }
                        }
                        if(block)
                            blocking_number++;
                    }
                    for(int i=0;i<service_number;i++){
                        if(service[i]==1){
                            float leave_prob=1-exp(-service_rate*d_time);
                            float leave_rand=uniform_distribution(out_generator);
                            if(leave_rand<leave_prob){
                                service[i]--;
                                departure_number++;
                            }
                        }
                    }
                }
                float blocking_prob=blocking_number/call_number;
                free(service);
                table[t1][t2][t3]=blocking_prob;
            }
        }
    }
    //write file
    fstream  file;
    file.open("Blocking_Probability_Table.csv",ios::out);
    for(int i=0;i<3;i++){
        if(i==0){
            file<<"Server:1 ,";
        }
        else if(i==1){
            file<<"Server:5 ,";
        }
        else if(i==2){
            file<<"Server:10 ,";
        }
        float arrival_rate=0.01;
        float service_rate=0.01;
        for(int j=0;j<4;j++,arrival_rate*=10){
            for(int s=0;s<6;s++,service_rate*=4){
                if(j==3&&s==5)
                    file<<arrival_rate<<"/"<<service_rate<<"\n";
                else
                    file<<arrival_rate<<"/"<<service_rate<<",";
            }
            service_rate=0.01;
        }
        file<<"Erlang,";
        arrival_rate=0.01;
        service_rate=0.01;
        for(int j=0;j<4;j++,arrival_rate*=10){
            for(int s=0;s<6;s++,service_rate*=4){
                float output=arrival_rate/service_rate;
                if(j==3&&s==5)
                    file<<output<<"\n";
                else
                    file<<output<<",";
            }
            service_rate=0.01;
        }
        file<<"Blocking Prob,";
        for(int j=0;j<4;j++){
            for(int s=0;s<6;s++){
                if(j==3&&s==5)
                    file<<table[i][j][s]<<"\n";
                else
                    file<<table[i][j][s]<<",";
            }
        }
    }
    file.close();

    //Q=S
    //prob
    t1=-1;
    for(int s=0;s<3;s++){
        t1++;
        int service_number=s_number[s];
        int t2=-1;
        for(float arrival_rate=0.01;arrival_rate<=10;arrival_rate*=10){
            t2++;
            int t3=-1;
            for(float service_rate=0.01;service_rate<=10.24;service_rate*=4){
                t3++;
                int* service=new int[service_number]();
                float d_time=0.01;
                if(arrival_rate<=0.1||service_rate<=0.1)
                    float d_time=0.1;
                if(arrival_rate<=0.01||service_rate<=0.01)
                    float d_time=1;

                float call_number=0;
                float blocking_number=0;
                float departure_number=0;
                int queue=0;

                default_random_engine generator(time(NULL)),out_generator(time(NULL));
                uniform_real_distribution<double> uniform_distribution(0.0,1.0);

                for(float t=0;t<100000/d_time;t++){
                    float come_prob=1-exp(-arrival_rate*d_time);
                    float come_rand=uniform_distribution(generator);
                    if(come_rand<come_prob){
                        int block=1;
                        call_number++;
                        for(int i=0;i<service_number;i++){
                            if(service[i]==0){
                                service[i]=1;
                                block=0;
                                break;
                            }
                        }
                        if(block){
                            if(queue<=service_number)
                                queue++;
                            else
                                blocking_number++;
                        }
                    }
                    for(int i=0;i<service_number;i++){
                        if(service[i]==1){
                            float leave_prob=1-exp(-service_rate*d_time);
                            float leave_rand=uniform_distribution(out_generator);
                            if(leave_rand<leave_prob){
                                service[i]--;
                                departure_number++;
                            }
                        }
                        if(service[i]==0){
                            if(queue>0){
                                queue--;
                                service[i]=1;
                            }
                        }
                    }
                }
                float blocking_prob=blocking_number/call_number;
                //cout<<arrival_rate<<" , "<<service_rate<<" , "<<service_number<<endl;
                //cout<<"blocking probability = "<<blocking_prob<<endl;
                free(service);
                table[t1][t2][t3]=blocking_prob;
            }
        }
    }
    //write file
    file.open("Blocking_Probability_Table_Q=S.csv",ios::out);
    for(int i=0;i<3;i++){
        if(i==0){
            file<<"Server:1 ,";
        }
        else if(i==1){
            file<<"Server:5 ,";
        }
        else if(i==2){
            file<<"Server:10 ,";
        }
        float arrival_rate=0.01;
        float service_rate=0.01;
        for(int j=0;j<4;j++,arrival_rate*=10){
            for(int s=0;s<6;s++,service_rate*=4){
                if(j==3&&s==5)
                    file<<arrival_rate<<"/"<<service_rate<<"\n";
                else
                    file<<arrival_rate<<"/"<<service_rate<<",";
            }
            service_rate=0.01;
        }
        file<<"Erlang,";
        arrival_rate=0.01;
        service_rate=0.01;
        for(int j=0;j<4;j++,arrival_rate*=10){
            for(int s=0;s<6;s++,service_rate*=4){
                float output=arrival_rate/service_rate;
                if(j==3&&s==5)
                    file<<output<<"\n";
                else
                    file<<output<<",";
            }
            service_rate=0.01;
        }
        file<<"Blocking Prob,";
        for(int j=0;j<4;j++){
            for(int s=0;s<6;s++){
                if(j==3&&s==5)
                    file<<table[i][j][s]<<"\n";
                else
                    file<<table[i][j][s]<<",";
            }
        }
    }
    file.close();
}