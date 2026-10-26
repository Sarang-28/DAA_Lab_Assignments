#include <iostream>
#include <vector>
#include <queue>
#include <climits>
#include <functional>

using namespace std;

void dijkstra(int source,
              vector< vector< pair<int, int> > >& graph,
              vector<int>& dist,
              vector<int>& parent)
{
    int V = graph.size();

    dist.assign(V, INT_MAX);
    parent.assign(V, -1);
    dist[source] = 0;

    priority_queue< pair<int, int>,
        vector< pair<int, int> >,
        greater< pair<int, int> > > pq;

    pq.push(make_pair(0, source));

    while (!pq.empty())
    {
        int d = pq.top().first;
        int u = pq.top().second;
        pq.pop();

        if (d > dist[u])
            continue;

        for (int i = 0; i < (int)graph[u].size(); i++)
        {
            int v = graph[u][i].first;
            int w = graph[u][i].second;

            if (dist[u] != INT_MAX &&
                dist[v] > dist[u] + w)
            {
                dist[v] = dist[u] + w;
                parent[v] = u;
                pq.push(make_pair(dist[v], v));
            }
        }
    }
}

// Display optimal route
void showPath(int hospital, vector<int>& parent)
{
    vector<int> path;
    int node = hospital;

    while (node != -1)
    {
        path.push_back(node);
        node = parent[node];
    }

    cout << "Optimal Route: ";

    for (int i = (int)path.size() - 1; i >= 0; i--)
    {
        cout << path[i];

        if (i > 0)
            cout << " -> ";
    }

    cout << endl;
}

// Find nearest hospital
void findHospital(int source,
                  vector< vector< pair<int, int> > >& graph,
                  vector<int>& hospitals)
{
    vector<int> dist, parent;
    dijkstra(source, graph, dist, parent);

    int minTime = INT_MAX;
    int nearestHospital = -1;

    for (int i = 0; i < (int)hospitals.size(); i++)
    {
        int h = hospitals[i];

        if (dist[h] < minTime)
        {
            minTime = dist[h];
            nearestHospital = h;
        }
    }

    if (nearestHospital == -1)
    {
        cout << "No hospital reachable.\n";
    }
    else
    {
        cout << "\nNearest hospital is at node "
             << nearestHospital
             << " with travel time "
             << minTime << " minutes.\n";

        showPath(nearestHospital, parent);
    }
}

// Update traffic on an existing road
bool updateTraffic(vector< vector< pair<int, int> > >& graph,
                   int u, int v, int newTime)
{
    bool found = false;

    for (int i = 0; i < (int)graph[u].size(); i++)
    {
        if (graph[u][i].first == v)
        {
            graph[u][i].second = newTime;
            found = true;
        }
    }

    for (int i = 0; i < (int)graph[v].size(); i++)
    {
        if (graph[v][i].first == u)
            graph[v][i].second = newTime;
    }

    return found;
}

int main()
{
    int V, E;

    cout << "Enter number of intersections (vertices): ";
    cin >> V;

    cout << "Enter number of roads (edges): ";
    cin >> E;

    if (V <= 0 || E < 0)
    {
        cout << "Invalid input.\n";
        return 0;
    }

    vector< vector< pair<int, int> > > graph(V);

    cout << "\nEnter edges (u v w):\n";
    cout << "u = starting node, v = ending node, w = travel time\n\n";

    for (int i = 0; i < E; i++)
    {
        int u, v, w;
        cin >> u >> v >> w;

        if (u < 0 || u >= V || v < 0 || v >= V ||
            u == v || w < 0)
        {
            cout << "Invalid road. Enter again:\n";
            i--;
            continue;
        }

        graph[u].push_back(make_pair(v, w));
        graph[v].push_back(make_pair(u, w));
    }

    int source, H;

    cout << "\nEnter ambulance start location (source): ";
    cin >> source;

    if (source < 0 || source >= V)
    {
        cout << "Invalid source node.\n";
        return 0;
    }

    cout << "Enter number of hospitals: ";
    cin >> H;

    if (H <= 0 || H > V)
    {
        cout << "Invalid number of hospitals.\n";
        return 0;
    }

    vector<int> hospitals(H);

    cout << "Enter hospital nodes: ";

    for (int i = 0; i < H; i++)
    {
        cin >> hospitals[i];

        if (hospitals[i] < 0 || hospitals[i] >= V)
        {
            cout << "Invalid hospital node.\n";
            return 0;
        }
    }

    findHospital(source, graph, hospitals);

    int choice;

    do
    {
        cout << "\n1. Update traffic\n";
        cout << "2. Recalculate route\n";
        cout << "3. Exit\n";
        cout << "Enter choice: ";
        cin >> choice;

        if (choice == 1)
        {
            int u, v, newTime;

            cout << "Enter road endpoints (u v): ";
            cin >> u >> v;

            if (u < 0 || u >= V || v < 0 ||
                v >= V || u == v)
            {
                cout << "Invalid road endpoints.\n";
                continue;
            }

            cout << "Enter new travel time: ";
            cin >> newTime;

            if (newTime < 0)
            {
                cout << "Travel time cannot be negative.\n";
                continue;
            }

            if (updateTraffic(graph, u, v, newTime))
            {
                cout << "Traffic updated successfully.\n";
                findHospital(source, graph, hospitals);
            }
            else
            {
                cout << "Road not found.\n";
            }
        }
        else if (choice == 2)
        {
            findHospital(source, graph, hospitals);
        }
        else if (choice == 3)
        {
            cout << "Exiting program.\n";
        }
        else
        {
            cout << "Invalid choice.\n";
        }

    } while (choice != 3);

    return 0;
}
