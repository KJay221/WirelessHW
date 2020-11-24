using namespace std;

class Car{
    public:
        Car(int x,int y,int direction);
        int x;
        int y;
        int direction;
        int best_gain;
        float best_policy;
        int threshold_gain;
        float threshold_policy;
        int entropy_gain;
        float entropy_policy;
        int your_gain;
        float your_policy;
        int at_corner;
};