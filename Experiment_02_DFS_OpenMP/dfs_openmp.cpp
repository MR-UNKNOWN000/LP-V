#include <iostream>
#include <vector>
#include <stack>
#include <omp.h>

using namespace std;

class Graph {
    int n;
    vector<vector<int> > adj;

public:
    Graph(int n) {
        this->n = n;
        adj.resize(n);
    }

    void addEdge(int u, int v) {
        adj[u].push_back(v);
        adj[v].push_back(u);
    }

    void dfsSequential(int start) {
        vector<bool> visited(n, false);
        stack<int> s;

        s.push(start);

        cout << "Sequential DFS: ";

        while (!s.empty()) {
            int node = s.top();
            s.pop();

            if (!visited[node]) {
                visited[node] = true;
                cout << node << " ";

                for (int i = adj[node].size() - 1; i >= 0; i--) {
                    int next = adj[node][i];

                    if (!visited[next]) {
                        s.push(next);
                    }
                }
            }
        }

        cout << endl;
    }

    void dfsParallel(int start) {
        vector<bool> visited(n, false);
        stack<int> s;

        s.push(start);

        cout << "Parallel DFS: ";

        while (!s.empty()) {
            int node;

            {
                node = s.top();
                s.pop();
            }

            if (!visited[node]) {
                {
                    visited[node] = true;
                    cout << node << " ";
                }

                for (int i = 0; i < adj[node].size(); i++) {
                    int next = adj[node][i];

                    {
                        if (!visited[next]) {
                            s.push(next);
                        }
                    }
                }
            }
        }

        cout << endl;
    }
};

int main() {
    int n, e;

    cout << "Enter number of vertices: ";
    cin >> n;

    cout << "Enter number of edges: ";
    cin >> e;

    Graph g(n);

    cout << "Enter edges (u v):" << endl;
    for (int i = 0; i < e; i++) {
        int u, v;
        cin >> u >> v;
        g.addEdge(u, v);
    }

    int start;
    cout << "Enter starting vertex: ";
    cin >> start;

    double startTime, endTime;

    startTime = omp_get_wtime();
    g.dfsSequential(start);
    endTime = omp_get_wtime();
    cout << "Sequential Time: " << (endTime - startTime) * 1000 << " ms" << endl;

    startTime = omp_get_wtime();
    g.dfsParallel(start);
    endTime = omp_get_wtime();
    cout << "Parallel Time: " << (endTime - startTime) * 1000 << " ms" << endl;

    return 0;
}
