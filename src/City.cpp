#include "City.h"

Crossroad* City::getCrossroad(const char &data) {
    auto crossroadIt = crossroads.find(data);
    if (crossroadIt != crossroads.end()) {
        return crossroadIt->second;
    }
    return nullptr;
}

void City::floydWarshall() {
    shortestPaths = city;
    for (int k = 0; k < numberOfCrossroads; k++) {
        for (int i = 0; i < numberOfCrossroads; i++) {
            if (shortestPaths[i][k] != INFINITY) {
                for (int j = 0; j < numberOfCrossroads; j++) {
                    if (shortestPaths[k][j] != INFINITY) {
                        shortestPaths[i][j] = std::min(shortestPaths[i][j], shortestPaths[i][k] + shortestPaths[k][j]);
                    }
                }
            }
        }
    }
}

void City::updateShortestPaths(int i, int j) {
    int duration = city[i][j];
    for (int m = 0; m < numberOfCrossroads; ++m) {
        for (int n = 0; n < numberOfCrossroads; ++n) {
            if (shortestPaths[m][i] == INFINITY || shortestPaths[j][n] == INFINITY)
                continue;

            shortestPaths[m][n] = std::min(shortestPaths[m][n], shortestPaths[m][i] + duration + shortestPaths[j][n]);
            shortestPaths[n][m] = std::min(shortestPaths[n][m], shortestPaths[n][j] + duration + shortestPaths[i][m]);
        }
    }
}

char City::convertToLowercase(char c) {
    if (c > 96 && c < 123) {
        c -= 32;
    }
    return c;
}

char City::intToChar(int i) {
    return (char) i + 'A';
}

int City::charToInt(char c) {
    return (int) c - 65;
}

City::City(int numOfCrossroads)
: numberOfCrossroads(numOfCrossroads),
  city(numberOfCrossroads,std::vector<int>(numberOfCrossroads, INFINITY)),
  shortestPaths(numberOfCrossroads,std::vector<int>(numberOfCrossroads, INFINITY)){
    for (int i = 0; i < numberOfCrossroads; ++i) {
        city[i][i] = 0;
        shortestPaths[i][i] = 0;
        char value = (char)(i + 65);
        Crossroad* newCrossroad = new Crossroad(value);
        crossroads[value] = newCrossroad;
        crossroadToStreets[newCrossroad] = std::unordered_set<Street *>();
    }
}

bool City::addStreets(std::vector<std::pair<char, char> > crossroadValues, std::vector<int> durations) {
    if (crossroadValues.size() != durations.size()) {
        return false;
    }

    for(int i = 0; i < crossroadValues.size(); ++i) {
        Crossroad *source = getCrossroad(convertToLowercase(crossroadValues[i].first));
        Crossroad *destination = getCrossroad(convertToLowercase(crossroadValues[i].second));

        if (source == nullptr || destination == nullptr) {
            return false;
        }

        Street *newStreet = new Street(source, destination, durations[i]);

        int m = charToInt(source->getValue());
        int n = charToInt(destination->getValue());
        city[m][n] = newStreet->getDuration();
        city[n][m] = newStreet->getDuration();
        shortestPaths[m][n] = newStreet->getDuration();
        shortestPaths[n][m] = newStreet->getDuration();

        if (crossroadToStreets.find(source) == crossroadToStreets.end()
            || crossroadToStreets.find(destination) == crossroadToStreets.end()) {
            return false;
        }

        crossroadToStreets[source].insert(newStreet);
        crossroadToStreets[destination].insert(newStreet);
    }

    for (auto it : crossroadToStreets) {
        for (auto itStreet : it.second) {
            std::cout << itStreet->getSource()->getValue() << " " << itStreet->getDestination()->getValue() << " "
            << itStreet->getDuration() << std::endl;
        }
    }

    floydWarshall();

    return true;
}

int City::addBus(const char &source, const char &destination) {

    if(source == destination) {
        throw std::invalid_argument("Trying to add bus from a crossroad to itself.");
    }

    char src = convertToLowercase(source);
    char dest = convertToLowercase(destination);
    int i = src - 65;
    int j = dest - 65;

    if((i >= numberOfCrossroads || j >= numberOfCrossroads) || shortestPaths[i][j] == INFINITY ) {
        std::string message = "There is no path between " + std::string(1 ,src) + " and "
                              + std::string(1,dest) + ". A bus can't be added.";
        throw std::invalid_argument(message);
    }

    Crossroad *first = getCrossroad(src);
    Crossroad *second = getCrossroad(dest);

    if (first == nullptr || second == nullptr) {
        throw std::invalid_argument("Attempting to access invalid crossroad");
    }

    std::string id = std::to_string(source) + std::to_string(destination);

    if(busIDs.find(id) != busIDs.end()) {
        return shortestPaths[i][j];
    }

    busIDs.insert(id);
    Bus *newBus = new Bus(first, second);
    indexToBus.push_back(newBus);
    newBus->setDuration(shortestPaths[i][j]);

    return newBus->getDuration();
}

std::vector<Bus *> City::constructStreet(const char &source, const char &destination, int duration) {
    int src = convertToLowercase(source);
    int dest = convertToLowercase(destination);
    int i = charToInt(src);
    int j = charToInt(dest);

    if((i >= numberOfCrossroads || j >= numberOfCrossroads)  || i == j || city[i][j] != INFINITY) {
        std::string message = "Either there is already a street between " + std::string(1 ,src) + " and "
                              + std::string(1,dest) + " or these crossroads don't exist";
        throw std::invalid_argument(message);
    }

    Crossroad* first = getCrossroad(src);
    Crossroad* second = getCrossroad(dest);

    if (first == nullptr || second == nullptr) {
        throw std::invalid_argument("Attempting to access invalid crossroad");
    }

    Street* newStreet = new Street(first, second, duration);

    if (city[i][j] != INFINITY && city[j][i] != INFINITY) {
        throw std::invalid_argument("Street already exists");
    }

    city[i][j] = duration;
    city[j][i] = duration;
    crossroadToStreets[first].insert(newStreet);
    crossroadToStreets[second].insert(newStreet);

    if (shortestPaths[i][j] > duration) {
        updateShortestPaths(i, j);

        for (Bus* current : indexToBus) {
            int m = charToInt(current->getSource()->getValue());
            int n = charToInt(current->getDestination()->getValue());

            if(current->getDuration() > shortestPaths[m][n]) {
                current->setDuration(shortestPaths[m][n]);
            }
        }
    }

    return indexToBus;
}
void City::printCity() {
    std::cout << "City:" << std::endl;
    for(int i = 0; i < numberOfCrossroads; ++i) {
        for(int j = 0; j < numberOfCrossroads; ++j) {
            std::cout << city[i][j] << " ";
        }
        std::cout << std::endl;
    }
    std::cout << std::endl;
}

void City::printShortestPaths() {
    std::cout << "Shortest Paths:" << std::endl;
    for(int i = 0; i < numberOfCrossroads; ++i) {
        for(int j = 0; j < numberOfCrossroads; ++j) {
            std::cout << shortestPaths[i][j] << " ";
        }
        std::cout << std::endl;
    }
    std::cout << std::endl;
}

void City::printBuses() {
    for (const auto& busPtr : indexToBus) {
        const Bus& bus = *busPtr;
        std::cout << "Bus: " << bus.getSource()->getValue() << " to " << bus.getDestination()->getValue() << ", Duration: " << bus.getDuration() << std::endl;
    }
    std::cout << std::endl;
}

void City::initDijkstra(Crossroad *source) {
    for(auto pair : crossroads) {
        pair.second->setDistance(INFINITY);
        pair.second->setParent(nullptr);
    }

    source->setDistance(0);
}

void City::relax(Street *street) {
    auto source = street->getSource();
    auto destination = street->getDestination();
    int duration = street->getDuration();

    if (source->getDistance() + duration < destination->getDistance()) {
        destination->setDistance(source->getDistance() + duration);
        destination->setParent(source);
    }
}


void City::dijkstra(Crossroad *source) {
    initDijkstra(source);

    std::unordered_set<Crossroad *> allCrossroads;
    for(auto crossroad : crossroads) {
        allCrossroads.insert(crossroad.second);
    }

    while(!allCrossroads.empty()) {
        Crossroad *shortestCrossroad = nullptr;
        int shortestDistance = INFINITY;

        for(auto crossroad : allCrossroads) {
            if(crossroad->getDistance() < shortestDistance) {
                shortestDistance = crossroad->getDistance();
                shortestCrossroad = crossroad;
            }
        }

        if(shortestCrossroad == nullptr) {
            return;
        }

        allCrossroads.erase(shortestCrossroad);
        if(crossroadToStreets.find(shortestCrossroad) != crossroadToStreets.end()) {
            for(auto &street : crossroadToStreets[shortestCrossroad]) {
                relax(street);
            }
        }
    }
}

void City::dijkstra(const char &sourceValue) {
    auto source = getCrossroad(sourceValue);

    if(source == nullptr) {
        throw std::invalid_argument("Source crossroad does not exist. ");
    } else {
        dijkstra(source);
    }
}

std::vector<char> City::details(int busIndex) {
    std::vector<char> path;

    if(busIndex >= indexToBus.size()) {
        throw std::invalid_argument("Size mismatch. ");
    }

    Bus *bus = indexToBus[busIndex];
    auto source = bus->getSource();
    auto dest = bus->getDestination();

    if(source == nullptr || dest == nullptr) {
        throw std::invalid_argument("Source or destination does not exist. ");
    }
        dijkstra(source->getValue());

    Crossroad *currentDestination = dest;

    while(currentDestination != nullptr) {
        path.insert(path.begin(), currentDestination->getValue());
        currentDestination = currentDestination->getParent();
    }

    printPath(path);
    return path;
}

void City::printPath(std::vector<char> &path) {
    std::cout << "Shortest paths" << std::endl;
    for (int i = 0; i < path.size(); ++i) {
        std::cout << path[i];
        if (i < path.size() - 1) {
            std::cout << " -> ";
        }
    }
    std::cout << std::endl;
}
