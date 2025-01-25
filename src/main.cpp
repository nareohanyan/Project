#include <iostream>
#include "City.h"

void test1() {
    City c(6);
    std::vector<std::pair<char, char> > streets;
    std::vector<int> durations;

    std::pair<char, char> p1 = std::make_pair('A', 'B');
    std::pair<char, char> p2 = std::make_pair('B', 'C');
    std::pair<char, char> p3 = std::make_pair('D', 'E');
    std::pair<char, char> p4 = std::make_pair('E', 'F');

    streets.push_back(p1);
    streets.push_back(p2);
    streets.push_back(p3);
    streets.push_back(p4);

    durations = {1, 2, 3, 5};
    c.addStreets(streets, durations);

    c.addBus('A', 'B');
    c.addBus('D', 'F');

    c.printCity();
    c.printShortestPaths();
    c.printBuses();

    c.constructStreet('A', 'D', 4);

    c.printCity();
    c.printShortestPaths();

    c.details(1);

}

void test2() {
    City c(4);
    std::vector<std::pair<char, char> > streets;
    std::vector<int> durations;

    std::pair<char, char> p1 = std::make_pair('A', 'B');

    streets.push_back(p1);

    durations = {2};
    c.addStreets(streets, durations);

    c.addBus('A', 'B');
    c.addBus('D', 'D');

    c.printCity();
    c.printShortestPaths();
    c.printBuses();

    c.constructStreet('A', 'D', 3);

    c.printCity();
    c.printShortestPaths();

    c.details(0);
}

void test3() {
    City c(6);
    std::vector<std::pair<char, char> > streets;
    std::vector<int> durations;

    std::pair<char, char> p1 = std::make_pair('A', 'B');
    std::pair<char, char> p2 = std::make_pair('A', 'D');
    std::pair<char, char> p3 = std::make_pair('B', 'E');
    std::pair<char, char> p4 = std::make_pair('D', 'E');
    std::pair<char, char> p5 = std::make_pair('E', 'F');

    streets.push_back(p1);
    streets.push_back(p2);
    streets.push_back(p3);
    streets.push_back(p4);
    streets.push_back(p5);

    durations = {1, 2, 3, 5, 4};
    c.addStreets(streets, durations);

    c.addBus('a', 'B');
    c.addBus('B', 'F');

    c.printCity();
    c.printShortestPaths();
    c.printBuses();

    c.constructStreet('c', 'f', 4);

    c.printCity();
    c.printShortestPaths();

    c.details(1);

}


int main() {
    test1();

    return 0;
}
