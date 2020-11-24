using namespace std;

class Car{
    public:
        Car(int x,int y,int direction);
        int x;
        int y;
        int direction;
        int best_gain;
        int threshold_gain;
        int entropy_gain;
        int your_gain;
        int at_coner;
};