#include <iostream>
#include <string>
#include <vector>
#include <stack>
#include <queue>
using namespace std;

long long POW(int a, int b){ // tính a^b
    if (b == 0) return 1;
    long long tmp = POW(a, b/2);
    if (b % 2) return tmp * tmp * a;
    return tmp * tmp;
}

void readGraph(char* inputPath, vector<vector<int>>& graph, int& n){ // đọc đồ thị
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

void HPath_DP(vector<vector<int>> graph, int n){ // đùng quy hoạch động trạng thái
    long long S = POW(2, n); // số lượng trạng thái
    vector<vector<bool>> dp(n, vector<bool>(S, 0));

    for (int i = 0; i < n; i++){
        dp[i][POW(2, i)] = 1;
    }

    for (long long i = 0; i < S; i++){ // duyệt qua tất cả các trạng thái
        for (int j = 0; j < n; j++){
            if ((i >> j) & 1){ // nếu trạng thái đang xét có chứa đỉnh j
                for (int k = 0; k < n; k++){
                    if (k != j && ((i >> k) & 1) && graph[j][k] != -1){
                        if (dp[k][i ^ POW(2, j)]){
                            dp[j][i] = 1;
                            break;
                        }
                    }
                }
            }
        }
    }

    bool result = 0;
    for (int i = 0; i < n; i++){
        if (dp[i][S - 1]){
            result = 1;
            break;
        }        
    }
    if (result) cout << "Yes\n";
    else cout << "No\n";
}

void HCycle_DP(vector<vector<int>> graph, int n){ // dùng quy hoạch động trạng thái
    long long S = POW(2, n);

    vector<vector<bool>> dp(n, vector<bool>(S, 0));

    dp[0][1] = 1;

    for (int i = 0; i < S; i++){ // duyệt qua tất cả các trạng thái
        for (int j = 0; j < n; j++){
            if ((i >> j) & 1){ // nếu trạng thái đang xét có chứa đỉnh j
                for (int k = 0; k < n; k++){
                    if (k != j && ((i >> k) & 1) && graph[j][k] != -1){
                        if (dp[k][i ^ POW(2, j)]){
                            dp[j][i] = 1;
                            break;
                        }
                    }
                }
            }
        }
    }

    bool result = 0;
    for (int i = 1; i < n; i++){
        if (dp[i][S - 1] && graph[i][0] != -1){
            result = 1;
            break;
        }
    }
    if (result) cout << "Yes\n";
    else cout << "No\n";
}

void TSP_DP(vector<vector<int>> graph, int n){ // dùng quy hoạch động trạng thái
    long long S = POW(2, n);
    vector<vector<int>> dp(n, vector<int>(S, INT_MAX));

    dp[0][1] = 0;

    for (int s = 1; s < S; s++){ // duyệt qua tất cả các trạng thái
        for (int i = 0; i < n; i++){
            if ((s >> i) & 1){ // nếu trạng thái đang xét có chứa đỉnh i
                for (int j = 0; j < n; j++){
                    if (i != j && ((s >> j) & 1) && graph[i][j] != -1){
                        if (dp[j][s ^ POW(2, i)] != INT_MAX){
                            dp[i][s] = min(dp[i][s], dp[j][s ^ POW(2, i)] + graph[j][i]);
                        }
                    }
                }
            }
        }
    }

    int minCost = INT_MAX, endPoint = -1;
    for (int i = 1; i < n; i++){ // tìm chu trình có tổng trọng số bé nhất
        if (graph[0][i] == -1 || dp[i][S - 1] == INT_MAX) continue;
        if (minCost > dp[i][S - 1] + graph[0][i]){
            minCost = dp[i][S - 1] + graph[0][i];
            endPoint = i;
        }
    }
    if (minCost == INT_MAX){
        cout << -1;
    } else {
        stack<int> st;
        long long s = S - 1;
        while (endPoint != 0){ // thực hiện truy vết
            st.push(endPoint);
            int tmp;
            for (int i = 0; i < n; i++){
                if (dp[i][s ^ (1 << endPoint)] + graph[i][endPoint] == dp[endPoint][s]){
                    tmp = i;
                    break;
                }
            }
            s = s ^ (1 << endPoint);
            endPoint = tmp;
        }
        st.push(0);

        while (!st.empty()){
            cout << st.top() + 1 << ' ';
            st.pop();
        }
        cout << endl << minCost;
    }
}

int main(int argc, char** argv){
    vector<vector<int>> graph;
    int n;
    readGraph(argv[2], graph, n);

    if (n > 30) return 0; // Out of dp range, too long to find solution.
    
    string action(argv[1]);
    if (action == "-HPath") HPath_DP(graph, n);
    if (action == "-HCycle") HCycle_DP(graph, n);
    if (action == "-TSP") TSP_DP(graph, n);
}