#include <unordered_map>
#include <vector>
#include <iostream>
#include "Pipe.h"
#include "CompressorStation.h"
struct Link {
    int inputId;
    int outputId;
};
class Manager {
private:
    std::unordered_map<int, Pipe> pipes;
    std::unordered_map<int, CompressorStation> stations;
    std::unordered_map<int, Link> links;
    int nextPipeId = 1;
    int nextStationId = 1;
    void addPipe();
    void addStation();
    void connectStations();
    void showNetwork();
    void topologicalSort();
    void showObjects();
    void searchPipes();
    void searchStations();
    void saveData();
    void loadData();
    template <typename T, typename Predicate>
    std::vector<int> findMatches(
        const std::unordered_map<int, T>& items,
        Predicate check
    ) {
        std::vector<int> ids;

        for (const auto& pair : items) {
            if (check(pair.second)) {
                pair.second.print();
                std::cout << std::endl;
                ids.push_back(pair.first);
            }
        }
        return ids;
    }
public:
    void run();
};
