#include <iostream>
#include <string>
#include <vector>
#include <stack>
#include <queue>
using namespace std;

long long pow(int a, int b){ // tính a^b
    if (b == 0) return 1;
    long long tmp = pow(a, b/2);
    if (b % 2) return tmp * tmp * a;
    return tmp * tmp;
}

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

bool HPath_DP(vector<vector<int>> graph, int n){ // đùng quy hoạch động trạng thái
    long long S = pow(2, n); // số lượng trạng thái
    vector<vector<bool>> dp(n, vector<bool>(S, 0));

    for (int i = 0; i < n; i++){
        dp[i][pow(2, i)] = 1;
    }

    for (long long i = 0; i < S; i++){ // duyệt qua tất cả các trạng thái
        for (int j = 0; j < n; j++){
            if ((i >> j) & 1){
                for (int k = 0; k < n; k++){
                    if (k != j && ((i >> k) & 1) && graph[j][k] != -1){
                        if (dp[k][i ^ pow(2, j)]){
                            dp[j][i] = 1;
                            break;
                        }
                    }
                }
            }
        }
    }

    for (int i = 0; i < n; i++){
        if (dp[i][S - 1]){
            return 1;
        }        
    }
    return 0;
}

void HPath(vector<vector<int>> graph, int n){
    vector<bool> visited(n, 0);
    bool result = HPath_DP(graph, n);
    if (result) cout << "Yes\n";
    else cout << "No\n";
};

bool HCycle_DP(vector<vector<int>> graph, int n){ // dùng quy hoạch động trạng thái
    long long S = pow(2, n);

    vector<vector<bool>> dp(n, vector<bool>(S, 0));

    dp[0][1] = 1;

    for (int i = 0; i < S; i++){
        for (int j = 0; j < n; j++){
            if ((i >> j) & 1){
                for (int k = 0; k < n; k++){
                    if (k != j && ((i >> k) & 1) && graph[j][k] != -1){
                        if (dp[k][i ^ pow(2, j)]){
                            dp[j][i] = 1;
                            break;
                        }
                    }
                }
            }
        }
    }

    for (int i = 1; i < n; i++){
        if (dp[i][S - 1] && graph[i][0] != -1){
            return 1;
        }
    }
    return 0;
}

void HCycle(vector<vector<int>> graph, int n){
    vector<bool> visited(n, 0);
    bool result = HCycle_DP(graph, n);
    if (result) cout << "Yes\n";
    else cout << "No\n";
};

void TSP_DFS(int vertex, int source, int cost, int& minCost, stack<int>& trace, stack<int>& minPath, vector<vector<int>> graph, int n, vector<bool>& visited, int remainVertices){
    if (cost >= minCost) return; // giảm bớt số lần tìm vô nghĩa khi vét cạn 
    trace.push(vertex);
    if (!remainVertices){
        if (graph[vertex][source] != -1){
            if (minCost > cost + graph[vertex][source]){
                minCost = cost + graph[vertex][source];
                minPath = trace;
            }
        }
    }
    visited[vertex] = 1;
    for (int i = 0; i < n; i++){
        if (!visited[i] && graph[vertex][i] != -1){ // đỉnh i chưa được thăm và có cung giữa đỉnh vertex và i
            TSP_DFS(i, source, cost + graph[vertex][i], minCost, trace, minPath, graph, n, visited, remainVertices - 1);
        }
    }
    visited[vertex] = 0;
    trace.pop();
}

void TSP(vector<vector<int>> graph, int n){
    vector<bool> visited(n, 0);
    int minCost = INT_MAX;
    stack<int> trace, minPath;
    TSP_DFS(0, 0, 0, minCost, trace, minPath, graph, n, visited, n - 1);

    if (!minPath.empty()){
        stack<int> s;
        while (!minPath.empty()){
            s.push(minPath.top());
            minPath.pop();
        }

        while (!s.empty()){
            cout << s.top() + 1 << ' ';
            s.pop();
        }
        cout << endl << minCost;
    }
    else cout << -1;
};

int main(int argc, char** argv){
    vector<vector<int>> graph;
    int n;
    readGraph(argv[2], graph, n);
    
    string action(argv[1]);
    if (action == "-HPath") HPath(graph, n);
    if (action == "-HCycle") HCycle(graph, n);
    if (action == "-TSP") TSP(graph, n);
}