#include <iostream>
#include <vector>
#include "FileUtils.h"


typedef std::vector<std::vector<int>> GRAPH;

class Graph
{
private:
    GRAPH graph;
    // std::vector<Vertex> vertex;
    void buildGraph(const std::vector<Token> &tokens);
    int countUsers(const std::vector<Token> &tokens);
public:
    Graph(const std::string &file_name);
    bool addEdge(int from,int to);
    std::vector<int> getNeighbors(int from);
    bool hasEdge(int from,int to);
    void print();

};