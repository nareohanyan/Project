#ifndef BUSES_1_CROSSROAD_H
#define BUSES_1_CROSSROAD_H

#define INFINITY 12000000

#include <climits>

class Crossroad {
private:
    char value;
    Crossroad *parent = nullptr;
    int distance = INFINITY;

public:
    Crossroad(const char &val);
    char getValue() const;
    int getDistance() const;
    Crossroad *getParent() const;
    void setDistance(int newDistance);
    void setParent(Crossroad *newParent);
    ~Crossroad();
};

#endif //BUSES_1_CROSSROAD_H