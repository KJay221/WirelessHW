#include "car.h"

Car::Car(int x,int y,int direction){
    this->x=x;
    this->y=y;
    this->direction=direction;
    this->best_policy=-1;
    this->threshold_policy=-1;
    this->entropy_policy=-1;
    this->your_policy=-1;
    this->best_gain=-1;
    this->threshold_gain=-1;
    this->entropy_gain=-1;
    this->your_gain=-1;
    this->at_corner=0;
}