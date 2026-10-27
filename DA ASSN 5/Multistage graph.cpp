#include <iostream>
#include <vector>
#include <iomanip>
#include <limits>

using namespace std;

struct Edge
{
    int u, v;
    double baseCost, curCost;
    bool active;
};

void calculateDP(int stages, vector<int>& stageCount,
                 vector<int>& stageOf, vector<Edge>& edges,
                 vector< vector<int> >& adj,
                 vector<double>& bestCost,
                 vector<int>& nextNode)
{
    int N = stageOf.size();
    double INF = numeric_limits<double>::max();

    bestCost.assign(N, INF);
    nextNode.assign(N, -1);

    int lastStart = N - stageCount[stages - 1];

    for (int i = lastStart; i < N; i++)
        bestCost[i] = 0;

    for (int s = stages - 2; s >= 0; s--)
    {
        int start = 0;

        for (int i = 0; i < s; i++)
            start += stageCount[i];

        int end = start + stageCount[s];

        for (int u = start; u < end; u++)
        {
            for (int j = 0; j < (int)adj[u].size(); j++)
            {
                int index = adj[u][j];
                int v = edges[index].v;

                if (edges[index].active && bestCost[v] != INF)
                {
                    double candidate =
                        edges[index].curCost + bestCost[v];

                    if (candidate < bestCost[u])
                    {
                        bestCost[u] = candidate;
                        nextNode[u] = v;
                    }
                }
            }
        }
    }
}

void showRoute(int source, vector<int>& nextNode,
               vector<double>& bestCost)
{
    if (bestCost[source] == numeric_limits<double>::max())
    {
        cout << "No complete route available.\n";
        return;
    }

    cout << "Optimal Route: ";

    int current = source;
    cout << current;

    while (nextNode[current] != -1)
    {
        current = nextNode[current];
        cout << " -> " << current;
    }

    cout << "\nTotal route cost: "
         << bestCost[source] << "\n";
}

void processRequests(int count, int stages,
                     vector<int>& stageCount,
                     vector<int>& stageOf,
                     vector<Edge>& edges,
                     vector< vector<int> >& adj)
{
    vector<double> bestCost;
    vector<int> nextNode;

    calculateDP(stages, stageCount, stageOf, edges,
                adj, bestCost, nextNode);

    for (int i = 0; i < count; i++)
    {
        int source;

        cout << "\nEnter source node for delivery request "
             << i + 1 << ": ";
        cin >> source;

        if (source < 0 || source >= (int)stageOf.size() ||
            stageOf[source] != 0)
        {
            cout << "Invalid source. Source must be in Stage 0.\n";
            continue;
        }

        cout << "Delivery request " << i + 1 << ":\n";
        showRoute(source, nextNode, bestCost);
    }
}

int main()
{
    int stages;

    cout << "Enter number of stages: ";
    cin >> stages;

    if (stages < 2)
    {
        cout << "Number of stages must be at least 2.\n";
        return 0;
    }

    vector<int> stageCount(stages);
    vector<int> stageOf;
    int N = 0;

    cout << "Enter number of nodes in each stage ("
         << stages << " values): ";

    for (int i = 0; i < stages; i++)
    {
        cin >> stageCount[i];

        if (stageCount[i] <= 0)
        {
            cout << "Each stage must contain at least one node.\n";
            return 0;
        }

        for (int j = 0; j < stageCount[i]; j++)
        {
            stageOf.push_back(i);
            N++;
        }
    }

    int E;

    cout << "Enter number of edges: ";
    cin >> E;

    if (E < 0)
    {
        cout << "Invalid number of edges.\n";
        return 0;
    }

    vector<Edge> edges;
    vector< vector<int> > adj(N);

    cout << "Enter each directed edge as: u v cost\n";

    for (int i = 0; i < E; i++)
    {
        Edge e;

        cin >> e.u >> e.v >> e.baseCost;

        if (e.u < 0 || e.u >= N ||
            e.v < 0 || e.v >= N ||
            stageOf[e.u] >= stageOf[e.v] ||
            e.baseCost < 0)
        {
            cout << "Invalid edge. Edges must go to a later stage "
                 << "with non-negative cost.\n";
            return 0;
        }

        e.curCost = e.baseCost;
        e.active = true;

        edges.push_back(e);
        adj[e.u].push_back((int)edges.size() - 1);
    }

    cout << fixed << setprecision(2);

    int requests;

    cout << "\nEnter number of delivery requests: ";
    cin >> requests;

    if (requests < 0)
    {
        cout << "Invalid number of requests.\n";
        return 0;
    }

    processRequests(requests, stages, stageCount, stageOf,
                    edges, adj);

    int choice;

    do
    {
        cout << "\n1. Update traffic cost\n";
        cout << "2. Close a road\n";
        cout << "3. Reopen a road\n";
        cout << "4. Process delivery requests again\n";
        cout << "5. Exit\n";
        cout << "Enter choice: ";
        cin >> choice;

        if (choice == 1)
        {
            int u, v;
            double multiplier;
            bool found = false;

            cout << "Enter directed edge (u v): ";
            cin >> u >> v;

            cout << "Enter traffic multiplier: ";
            cin >> multiplier;

            if (u < 0 || u >= N || v < 0 || v >= N ||
                multiplier < 0)
            {
                cout << "Invalid input.\n";
                continue;
            }

            for (int i = 0; i < (int)edges.size(); i++)
            {
                if (edges[i].u == u && edges[i].v == v)
                {
                    edges[i].curCost =
                        edges[i].baseCost * multiplier;
                    found = true;
                }
            }

            if (found)
                cout << "Traffic cost updated.\n";
            else
                cout << "Edge not found.\n";
        }
        else if (choice == 2 || choice == 3)
        {
            int u, v;
            bool found = false;

            cout << "Enter directed edge (u v): ";
            cin >> u >> v;

            if (u < 0 || u >= N || v < 0 || v >= N)
            {
                cout << "Invalid node ID.\n";
                continue;
            }

            for (int i = 0; i < (int)edges.size(); i++)
            {
                if (edges[i].u == u && edges[i].v == v)
                {
                    edges[i].active = (choice == 3);
                    found = true;
                }
            }

            if (found)
                cout << "Road status updated.\n";
            else
                cout << "Edge not found.\n";
        }
        else if (choice == 4)
        {
            cout << "\nRecalculating routes with current costs...\n";

            cout << "Enter number of delivery requests: ";
            cin >> requests;

            if (requests < 0)
            {
                cout << "Invalid number of requests.\n";
                continue;
            }

            processRequests(requests, stages, stageCount,
                            stageOf, edges, adj);
        }
        else if (choice == 5)
        {
            cout << "Exiting program.\n";
        }
        else
        {
            cout << "Invalid choice.\n";
        }

    } while (choice != 5);

    return 0;
}
