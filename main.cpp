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

void readGraph(char* inputPath, vector<vector<int>>& graph, int& n){ // đọc đồ thị, biểu diễn bằng ma trận kề
    /*
        inputPath: tên file chứa dữ liệu
        graph: ma trận kề
        n: số đỉnh của đồ thị
    */
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

void HPath_DP(vector<vector<int>> graph, int n){ // tìm đường đi Hamilton
    /* 
        graph: ma trận kề
        n: số đỉnh của đồ thị
    */
    long long S = POW(2, n); // số lượng trạng thái
    vector<vector<bool>> dp(n, vector<bool>(S, 0)); // mảng chứa kết quả quy hoạch động, ban đầu không tồn tại đường đi nên tất cả các giá trị gán = 0

    for (int i = 0; i < n; i++){
        dp[i][POW(2, i)] = 1; // ban đầu tồn tại đường đi đi qua một đỉnh i và kết thúc tại đỉnh i
    }

    for (long long i = 0; i < S; i++){ // duyệt qua tất cả các trạng thái
        for (int j = 0; j < n; j++){ // duyệt qua tất cả các đỉnh
            if ((i >> j) & 1){ // nếu trạng thái đang xét có chứa đỉnh j
                for (int k = 0; k < n; k++){ // duyệt qua tất cả các đỉnh
                    if (k != j && ((i >> k) & 1) && graph[j][k] != -1){ // đỉnh k khác j, k có trong trạng thái đang xét và có cung nối giữa đỉnh j và k
                        if (dp[k][i ^ POW(2, j)]){ // nếu tồn tại đường đi qua tất cả các đỉnh ngoại trừ j trong trạng thái đang xét và kết thúc tại k
                            dp[j][i] = 1; // thì tồn tại đường đi qua các đỉnh trong trạng thái đang xét và kết thúc tại j
                            break; 
                        }
                    }
                }
            }
        }
    }

    bool result = 0;
    for (int i = 0; i < n; i++){ // duyệt qua tất cả các đỉnh
        if (dp[i][S - 1]){ // tồn tại đường đi qua tất cả các đỉnh và kết thúc tại i
            result = 1;
            break;
        }        
    }
    if (result) cout << "Yes\n";
    else cout << "No\n";
}

void HCycle_DP(vector<vector<int>> graph, int n){ // tìm chu trình Hamilton
    /*
        graph: ma trận kề
        n: số đỉnh của đồ thị
    */
    long long S = POW(2, n); // số lượng trạng thái 

    vector<vector<bool>> dp(n, vector<bool>(S, 0)); // mảng chứa kết quả quy hoạch động, ban đầu không tồn tại đường đi nên tất cả các giá trị gán = 0

    // vì tìm chu trình nên ta có thể chọn một đỉnh bất kỳ làm đỉnh xuất phát và làm đỉnh kết thúc
    // để tổng quát thì chọn đỉnh 0
    dp[0][1] = 1; // tồn tại đường đi qua một đỉnh 0 và kết thúc tại đỉnh 0

    for (int i = 0; i < S; i++){ // duyệt qua tất cả các trạng thái
        for (int j = 0; j < n; j++){ // duyệt qua tất cả các đỉnh
            if ((i >> j) & 1){ // nếu trạng thái đang xét có chứa đỉnh j
                for (int k = 0; k < n; k++){ // duyệt qua tất cả các đỉnh
                    if (k != j && ((i >> k) & 1) && graph[j][k] != -1){ // đỉnh k khác j, k có trong trạng thái đang xét và có cung nối giữa j và k
                        if (dp[k][i ^ POW(2, j)]){ // nếu tồn tại đường đi qua tất cả các đỉnh ngoại trừ j trong trạng thái đang xét và kết thúc tại k
                            dp[j][i] = 1; // thì tồn tại đường đi qua các đỉnh trong trạng thái đang xét và kết thúc tại j
                            break;
                        }
                    }
                }
            }
        }
    }

    bool result = 0;
    for (int i = 1; i < n; i++){ // duyệt qua tất cả các đỉnh
        if (dp[i][S - 1] && graph[i][0] != -1){ // nếu tồn tại đường đi qua tất cả các đỉnh và kết thúc tại i, đồng thời có cung nối giữa i và đỉnh xuất phát
            result = 1;
            break;
        }
    }
    if (result) cout << "Yes\n";
    else cout << "No\n";
}

void TSP_DP(vector<vector<int>> graph, int n){ // tìm đường đi TSP
    /*
        graph: ma trận kề
        n: số lượng đỉnh của đồ thị
    */
    long long S = POW(2, n); // số lượng trạng thái
    vector<vector<int>> dp(n, vector<int>(S, INT_MAX)); // mảng chứa kết quả quy hoạch động, ban đầu không tồn tại đường đi nên tất cả các giá trị chi phí gán = INT_MAX

    // tương tự như tìm chu trình Hamilton, chọn đỉnh xuất phát và kết thúc là đỉnh 0
    dp[0][1] = 0; // chi phí đường đi qua một đỉnh 0 và kết thúc tại đỉnh 0 là 0

    for (int s = 1; s < S; s++){ // duyệt qua tất cả các trạng thái
        for (int i = 0; i < n; i++){ // duyệt qua tất cả các đỉnh
            if ((s >> i) & 1){ // nếu trạng thái đang xét có chứa đỉnh i
                for (int j = 0; j < n; j++){ // duyệt qua tất cả các đỉnh
                    if (i != j && ((s >> j) & 1) && graph[i][j] != -1){ // nếu đỉnh i khác j, j có trong trạng thái đang xét, có cung nối giữa i và j
                        if (dp[j][s ^ POW(2, i)] != INT_MAX){ // nếu tồn tại đường đi qua các đỉnh trừ i trong trạng thái đang xét và kết thúc tại j
                            dp[i][s] = min(dp[i][s], dp[j][s ^ POW(2, i)] + graph[j][i]); // tối ưu chi phí của đường đi qua tất cả các đỉnh trong trạng thái và kết thúc tại i
                        }
                    }
                }
            }
        }
    }

    int minCost = INT_MAX, endPoint = -1; 
    for (int i = 1; i < n; i++){ // tìm chu trình có tổng trọng số bé nhất
        if (graph[0][i] == -1 || dp[i][S - 1] == INT_MAX) continue; // nếu không có cung nối giữa 0 và i hoặc không tồn tại đường đi qua tất cả các đỉnh và kết thúc tại i thì tìm tiếp
        if (minCost > dp[i][S - 1] + graph[0][i]){ // nếu đường đi này có chi phí nhỏ hơn thì cập nhật
            minCost = dp[i][S - 1] + graph[0][i];
            endPoint = i; // lưu lại đỉnh kết thúc của đường đi Hamilton
        }
    }
    if (minCost == INT_MAX){ // nếu không tồn tại chu trình Hamilton
        cout << -1;
    } else { // nếu tồn tại chu trình Hamilton
        stack<int> st; // ngăn xếp lưu đường đi của chu trình
        long long s = S - 1; // trạng thái chứa tất cả các đỉnh
        while (endPoint != 0){ // truy vết đến khi đỉnh đang xét = 0 thì dừng
            st.push(endPoint); // đẩy đỉnh cuối cùng vào ngăn xếp
            int tmp; // biến chứa đỉnh kề cuối cần tìm
            for (int i = 0; i < n; i++){ // duyệt qua tất cả các đỉnh
                if (dp[i][s ^ (1 << endPoint)] + graph[i][endPoint] == dp[endPoint][s]){ 
                // nếu chi phí đường đi qua các đỉnh ngoại trừ endPoint trong trạng thái s + chi phí đi từ i sang endPoint = chi phí đi qua các đỉnh 
                // trong trạng thái s và kết thúc tại endPoint thì đỉnh i là đỉnh liền trước của endPoint trong đường đi
                    tmp = i; // lưu đỉnh i
                    break;
                }
            }
            s = s ^ (1 << endPoint); // xóa endPoint ra khỏi trạng thái
            endPoint = tmp; // gán đỉnh liền trước endPoint thành endPoint
        }
        st.push(0); // thêm đỉnh 0 vào để trở thành chu trình

        while (!st.empty()){ // xuất chu trình
            cout << st.top() + 1 << ' ';
            st.pop();
        }
        cout << endl << minCost; // xuất chi phí nhỏ nhất
    }
}

int main(int argc, char** argv){
    vector<vector<int>> graph; // ma trận kề
    int n; // số đỉnh
    readGraph(argv[2], graph, n); // đọc đồ thị

    if (n > 30) return 0; // n > 32 thì không thể chứa được trạng thái (vì dùng KDL long long), nếu được thì thời gian thực thi cũng rất lâu
    
    string action(argv[1]);
    if (action == "-HPath") HPath_DP(graph, n);
    if (action == "-HCycle") HCycle_DP(graph, n);
    if (action == "-TSP") TSP_DP(graph, n);
}