#include <iostream>
#include <vector>
#include <queue>
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

    void bfsSequential(int start) {
        vector<bool> visited(n, false);
        queue<int> q;

        visited[start] = true;
        q.push(start);

        cout << "Sequential BFS: ";

        while (!q.empty()) {
            int node = q.front();
            q.pop();

            cout << node << " ";

            for (int i = 0; i < adj[node].size(); i++) {
                int next = adj[node][i];

                if (!visited[next]) {
                    visited[next] = true;
                    q.push(next);
                }
            }
        }

        cout << endl;
    }

    void bfsParallel(int start) {
        vector<bool> visited(n, false);
        queue<int> q;

        visited[start] = true;
        q.push(start);

        cout << "Parallel BFS: ";

        while (!q.empty()) {
            int size = q.size();
            vector<int> level;

            for (int i = 0; i < size; i++) {
                int node = q.front();
                q.pop();
                level.push_back(node);
            }

            for (int i = 0; i < level.size(); i++) {
                cout << level[i] << " ";
            }

            for (int i = 0; i < level.size(); i++) {
                int node = level[i];

                for (int j = 0; j < adj[node].size(); j++) {
                    int next = adj[node][j];

                    {
                        if (!visited[next]) {
                            visited[next] = true;
                            q.push(next);
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
    g.bfsSequential(start);
    endTime = omp_get_wtime();
    cout << "Sequential Time: " << (endTime - startTime) * 1000 << " ms" << endl;

    startTime = omp_get_wtime();
    g.bfsParallel(start);
    endTime = omp_get_wtime();
    cout << "Parallel Time: " << (endTime - startTime) * 1000 << " ms" << endl;

    return 0;
}
