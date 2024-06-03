#include <iostream>
#include <vector>
#include <stack>
#include <queue>
#include <algorithm>
#include <limits.h>

using namespace std;

const int MAX_VERTICES = 32;
const int INF = INT_MAX;

struct Graph {
    int numVertices;
    int adjMatrix[MAX_VERTICES][MAX_VERTICES];
    vector<int> adjList[MAX_VERTICES];
};

struct Edge {
    int start, end, weight;
};

void initializeGraph(Graph& g, int numVertices) {
    g.numVertices = numVertices;
    for (int i = 0; i < MAX_VERTICES; ++i) {
        for (int j = 0; j < MAX_VERTICES; ++j) {
            g.adjMatrix[i][j] = 0;
        }
    }
}

void addEdgeMatrix(Graph& g, int start, int end, int weight = 1) {
    if (start >= g.numVertices || end >= g.numVertices) {
        cout << "Invalid edge: (" << start << ", " << end << "). Vertices must be in range [0, " << g.numVertices - 1 << "]" << endl;
        return;
    }
    g.adjMatrix[start][end] = weight;
    g.adjMatrix[end][start] = weight; 
}

void addEdgeList(Graph& g, int start, int end) {
    if (start >= g.numVertices || end >= g.numVertices) {
        cout << "Invalid edge: (" << start << ", " << end << "). Vertices must be in range [0, " << g.numVertices - 1 << "]" << endl;
        return;
    }
    g.adjList[start].push_back(end);
    g.adjList[end].push_back(start); 
}

void printGraphMatrix(const Graph& g) {
    cout << "Graph (Adjacency Matrix):" << endl;
    for (int i = 0; i < g.numVertices; ++i) {
        for (int j = 0; j < g.numVertices; ++j) {
            cout << g.adjMatrix[i][j] << " ";
        }
        cout << endl;
    }
}

void printGraphList(const Graph& g) {
    cout << "Graph (Adjacency List):" << endl;
    for (int i = 0; i < g.numVertices; ++i) {
        cout << i << ": ";
        for (auto v : g.adjList[i]) {
            cout << v << " ";
        }
        cout << endl;
    }
}

bool isConnectedMatrix(const Graph& g) {
    vector<bool> visited(g.numVertices, false);
    stack<int> stack;
    stack.push(0);

    while (!stack.empty()) {
        int vertex = stack.top();
        stack.pop();

        if (!visited[vertex]) {
            visited[vertex] = true;
            for (int i = 0; i < g.numVertices; ++i) {
                if (g.adjMatrix[vertex][i] != 0 && !visited[i]) {
                    stack.push(i);
                }
            }
        }
    }

    for (bool v : visited) {
        if (!v) return false;
    }

    return true;
}

void DFSMatrix(const Graph& g, int startVertex) {
    vector<bool> visited(g.numVertices, false);
    stack<int> stack;
    stack.push(startVertex);

    cout << "DFS (Matrix) starting from vertex " << startVertex << ": ";
    while (!stack.empty()) {
        int vertex = stack.top();
        stack.pop();

        if (!visited[vertex]) {
            cout << vertex << " ";
            visited[vertex] = true;
        }

        for (int i = 0; i < g.numVertices; ++i) {
            if (g.adjMatrix[vertex][i] != 0 && !visited[i]) {
                stack.push(i);
            }
        }
    }
    cout << endl;
}

void DFSList(const Graph& g, int startVertex) {
    vector<bool> visited(g.numVertices, false);
    stack<int> stack;
    stack.push(startVertex);

    cout << "DFS (List) starting from vertex " << startVertex << ": ";
    while (!stack.empty()) {
        int vertex = stack.top();
        stack.pop();

        if (!visited[vertex]) {
            cout << vertex << " ";
            visited[vertex] = true;
        }

        for (auto i = g.adjList[vertex].begin(); i != g.adjList[vertex].end(); ++i) {
            if (!visited[*i]) {
                stack.push(*i);
            }
        }
    }
    cout << endl;
}

void dijkstra(const Graph& g, int startVertex) {
    vector<int> dist(g.numVertices, INF);
    dist[startVertex] = 0;
    vector<bool> visited(g.numVertices, false);
    priority_queue<pair<int, int>, vector<pair<int, int>>, greater<pair<int, int>>> pq;
    pq.push({ 0, startVertex });

    while (!pq.empty()) {
        int u = pq.top().second;
        pq.pop();
        if (visited[u]) continue;

        visited[u] = true;

        for (int v = 0; v < g.numVertices; ++v) {
            if (g.adjMatrix[u][v] && !visited[v] && dist[u] + g.adjMatrix[u][v] < dist[v]) {
                dist[v] = dist[u] + g.adjMatrix[u][v];
                pq.push({ dist[v], v });
            }
        }
    }

    cout << "Dijkstra (Matrix) distances from vertex " << startVertex << ":" << endl;
    for (int i = 0; i < g.numVertices; ++i) {
        if (dist[i] == INF) {
            cout << "To vertex " << i << ": INF" << endl;
        }
        else {
            cout << "To vertex " << i << ": " << dist[i] << endl;
        }
    }
}

void topologicalSort(const Graph& g) {
    vector<int> in_degree(g.numVertices, 0);

    for (int i = 0; i < g.numVertices; i++) {
        for (auto v : g.adjList[i]) {
            in_degree[v]++;
        }
    }

    queue<int> q;
    for (int i = 0; i < g.numVertices; i++) {
        if (in_degree[i] == 0) {
            q.push(i);
        }
    }

    int count = 0;
    vector<int> top_order;

    while (!q.empty()) {
        int u = q.front();
        q.pop();
        top_order.push_back(u);

        for (auto v : g.adjList[u]) {
            if (--in_degree[v] == 0) {
                q.push(v);
            }
        }

        count++;
    }

    if (count != g.numVertices) {
        cout << "Graph has cycles, topological sorting is not possible." << endl;
        return;
    }

    cout << "Topological Sort: ";
    for (int i = 0; i < top_order.size(); i++) {
        cout << top_order[i] << " ";
    }
    cout << endl;
}

int findParent(vector<int>& parent, int u) {
    if (u != parent[u]) {
        parent[u] = findParent(parent, parent[u]);
    }
    return parent[u];
}

void unionSets(vector<int>& parent, int u, int v) {
    int parentU = findParent(parent, u);
    int parentV = findParent(parent, v);
    parent[parentU] = parentV;
}

void kruskalMST(int numVertices, vector<Edge>& edges) {
    sort(edges.begin(), edges.end(), [](Edge a, Edge b) {
        return a.weight < b.weight;
        });

    vector<int> parent(numVertices);
    for (int i = 0; i < numVertices; i++) {
        parent[i] = i;
    }

    vector<Edge> mst;
    for (Edge e : edges) {
        cout << "Processing edge: (" << e.start << ", " << e.end << ", " << e.weight << ")" << endl;
        if (findParent(parent, e.start) != findParent(parent, e.end)) {
            mst.push_back(e);
            unionSets(parent, e.start, e.end);
        }
    }

    cout << "Kruskal MST:" << endl;
    for (Edge e : mst) {
        cout << e.start << " - " << e.end << " (weight: " << e.weight << ")" << endl;
    }
}

void interactiveMode() {
    Graph graphMatrix;
    Graph graphList;
    vector<Edge> edges;

    cout << "Enter the number of vertices: ";
    int numVertices;
    cin >> numVertices;

    initializeGraph(graphMatrix, numVertices);
    initializeGraph(graphList, numVertices);

    while (true) {
        cout << "Select operation:\n1. Add edge\n2. Print Graph Matrix\n3. Print Graph List\n4. Check Connectivity\n5. Perform DFS\n6. Perform Dijkstra\n7. Perform Topological Sort\n8. Perform Kruskal MST\n9. Exit\n";
        int choice;
        cin >> choice;

        if (choice == 1) {
            int u, v, w;
            cout << "Enter edge (u v w): ";
            cin >> u >> v >> w;
            addEdgeMatrix(graphMatrix, u, v, w);
            addEdgeList(graphList, u, v);
            edges.push_back({ u, v, w });
        }
        else if (choice == 2) {
            printGraphMatrix(graphMatrix);
        }
        else if (choice == 3) {
            printGraphList(graphList);
        }
        else if (choice == 4) {
            cout << "Is the graph connected? ";
            if (isConnectedMatrix(graphMatrix)) {
                cout << "Yes" << endl;
            }
            else {
                cout << "No" << endl;
            }
        }
        else if (choice == 5) {
            int startVertex;
            cout << "Enter start vertex for DFS: ";
            cin >> startVertex;
            if (startVertex >= numVertices) {
                cout << "Invalid start vertex: " << startVertex << ". Must be in range [0, " << numVertices - 1 << "]" << endl;
            }
            else {
                DFSMatrix(graphMatrix, startVertex);
                DFSList(graphList, startVertex);
            }
        }
        else if (choice == 6) {
            int startVertex;
            cout << "Enter start vertex for Dijkstra: ";
            cin >> startVertex;
            if (startVertex >= numVertices) {
                cout << "Invalid start vertex: " << startVertex << ". Must be in range [0, " << numVertices - 1 << "]" << endl;
            }
            else {
                dijkstra(graphMatrix, startVertex);
            }
        }
        else if (choice == 7) {
            topologicalSort(graphList);
        }
        else if (choice == 8) {
            kruskalMST(numVertices, edges);
        }
        else if (choice == 9) {
            break;
        }
        else {
            cout << "Invalid choice, try again." << endl;
        }
    }
}

void demoMode() {
    Graph graphMatrix;
    Graph graphList;
    vector<Edge> edges;

    initializeGraph(graphMatrix, 5);
    initializeGraph(graphList, 5);

    addEdgeMatrix(graphMatrix, 0, 1);
    addEdgeMatrix(graphMatrix, 0, 2);
    addEdgeMatrix(graphMatrix, 1, 2);
    addEdgeMatrix(graphMatrix, 1, 3);
    addEdgeMatrix(graphMatrix, 3, 4);

    addEdgeList(graphList, 0, 1);
    addEdgeList(graphList, 0, 2);
    addEdgeList(graphList, 1, 2);
    addEdgeList(graphList, 1, 3);
    addEdgeList(graphList, 3, 4);

    edges.push_back({ 0, 1, 1 });
    edges.push_back({ 0, 2, 4 });
    edges.push_back({ 1, 2, 2 });
    edges.push_back({ 1, 3, 5 });
    edges.push_back({ 3, 4, 3 });

    printGraphMatrix(graphMatrix);

    cout << "Is the graph connected? ";
    if (isConnectedMatrix(graphMatrix)) {
        cout << "Yes" << endl;
    }
    else {
        cout << "No" << endl;
    }

    DFSMatrix(graphMatrix, 0);

    printGraphList(graphList);

    DFSList(graphList, 0);

    topologicalSort(graphList);

    dijkstra(graphMatrix, 0);

    kruskalMST(graphMatrix.numVertices, edges);
}

void benchmarkMode() {
    Graph graphMatrix;
    Graph graphList;
    vector<Edge> edges;

    int numVertices = 1000; 
    initializeGraph(graphMatrix, numVertices);
    initializeGraph(graphList, numVertices);

    for (int i = 0; i < numVertices - 1; ++i) {
        addEdgeMatrix(graphMatrix, i, i + 1, 1);
        addEdgeList(graphList, i, i + 1);
        edges.push_back({ i, i + 1, 1 });
    }

    printGraphMatrix(graphMatrix);

    cout << "Is the graph connected? ";
    if (isConnectedMatrix(graphMatrix)) {
        cout << "Yes" << endl;
    }
    else {
        cout << "No" << endl;
    }

    DFSMatrix(graphMatrix, 0);

    printGraphList(graphList);

    DFSList(graphList, 0);

    topologicalSort(graphList);

    dijkstra(graphMatrix, 0);

    kruskalMST(graphMatrix.numVertices, edges);
}

int main() {
    cout << "Select mode: 1. Demo 2. Interactive 3. Benchmark" << endl;
    int mode;
    cin >> mode;

    switch (mode) {
    case 1:
        demoMode();
        break;
    case 2:
        interactiveMode();
        break;
    case 3:
        benchmarkMode();
        break;
    default:
        cout << "Invalid mode selected" << endl;
    }

    return 0;
}
