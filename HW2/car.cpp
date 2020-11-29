#include "car.h"

Car::Car(int x,int y,int direction){
    this->x=x;
    this->y=y;
    this->direction=direction;
    this->at_corner=0;
    //policy
    this->best_policy=-1;
    for(int i=0;i<20;i++){
        this->threshold_policy[i]=-1;
        this->entropy_policy[i]=-1;
        this->your_policy[i]=-1;
    }
    this->min_policy=-1;
    //gain
    this->best_gain=-1;
    for(int i=0;i<20;i++){
        this->threshold_gain[i]=-1;
        this->entropy_gain[i]=-1;
        this->your_gain[20]=-1;
    }
    this->min_gain=-1;
}