#ifndef BUSES_1_CITY_H
#define BUSES_1_CITY_H

#include "Street.h"
#include "Bus.h"
#include <iostream>
#include <unordered_map>
#include <unordered_set>
#include <vector>

class City {
private:
    int numberOfCrossroads;
    std::unordered_map<char, Crossroad *> crossroads;
    std::unordered_map<Crossroad*, std::unordered_set<Street*> > crossroadToStreets;
    std::vector<std::vector<int> > city;
    std::vector<std::vector<int> > shortestPaths;
    std::vector<Bus *> indexToBus;
    std::unordered_set<std::string> busIDs;

    Crossroad* getCrossroad(const char &data);
    void floydWarshall();
    void updateShortestPaths(int i, int j);
    char intToChar(int i);
    int charToInt(char c);
    char convertToLowercase(char c);

public:
    City(int numOfCrossroads);
    bool addStreets(std::vector<std::pair<char, char> > crossroadValues, std::vector<int> durations);
    int addBus(const char &source, const char &destination);
    std::vector<Bus *> constructStreet(const char &source, const char &destination, int duration);
    void initDijkstra(Crossroad *source);
    void relax(Street *street);
    void dijkstra(Crossroad *source);
    void dijkstra(const char &sourceValue);
    std::vector<char> details(int busIndex);
    void printCity();
    void printShortestPaths();
    void printBuses();
    void printPath(std::vector<char> &path);
};

#endif //BUSES_1_CITY_H
