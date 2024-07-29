#include <iostream>
#include <vector>
#include <unordered_map>
#include <algorithm>
#include "include/json.hpp"
using namespace std;

struct Song{
    string id;
};

struct Artist{
    string name;
    double familiarity;
    string terms;
};

struct Node {
    Song song;
    Artist artist;
    vector<Node*> neighbors;
};

class Graph{
private:
    unordered_map<string, Node> nodes;
public:

};




int main() {
    std::cout << "Hello, World!" << std::endl;
    return 0;
}


