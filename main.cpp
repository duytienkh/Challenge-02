#include <iostream>
#include <string>
#include <vector>
using namespace std;

void readGraph(char* inputPath, vector<vector<int>>& graph, int& n){
    FILE* f = fopen(inputPath, "r");
    fscanf(f, "%d", &n);
    graph = vector<vector<int>>(n, vector<int>(n, -1));
    int u, v, cost;
    while (fscanf(f, "%d %d %d", &u, &v, &cost) > 0){
        u--;
        v--;
        graph[u][v] = cost;
        graph[v][u] = cost;
    }
    fclose(f);
}

bool HPath_DFS(int vertex, vector<vector<int>> graph, int n, vector<bool>& visited, int remainVertices){
    if (!remainVertices) return 1;
    visited[vertex] = 1; // đã thăm đỉnh vertex
    bool result = false;
    for (int i = 0; i < n; i++){
        if (!visited[i] && graph[vertex][i] != -1){ // đỉnh i chưa được thăm và có cung giữa đỉnh vertex và i
            result = HPath_DFS(i, graph, n, visited, remainVertices - 1);
            if (result) break;
        }
    }
    visited[vertex] = 0;
    return result;
}
void HPath(vector<vector<int>> graph, int n){
    bool result = false;
    vector<bool> visited(n, 0);
    for (int i = 0; i < n; i++){
        result = HPath_DFS(i, graph, n, visited, n - 1);
        if (result) break;
    }
    if (result) cout << "Yes\n";
    else cout << "No\n";
};
void HCycle(){};
void TSP(){};

int main(int argc, char** argv){
    vector<vector<int>> graph;
    int n;
    readGraph(argv[2], graph, n);
    
    string action(argv[1]);
    if (action == "-HPath") HPath(graph, n);
    if (action == "-HCycle") HCycle();
    if (action == "-TSP") TSP();
}