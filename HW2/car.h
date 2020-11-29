using namespace std;

class Car{
    public:
        Car(int x,int y,int direction);
        int x;
        int y;
        int direction;
        int best_gain;
        float best_policy;
        int threshold_gain[20];
        float threshold_policy[20];
        int entropy_gain[20];
        float entropy_policy[20];
        int your_gain[20];
        float your_policy[20];
        int min_gain;
        float min_policy;
        int at_corner;
};