#include <iostream>
#include <vector>
#include <queue>
#include <iomanip>
#include <utility>

using namespace std;

const double INF = 1e100;

struct Node
{
    int current;
    unsigned long long visited;
    vector<int> path;
    double cost, distance, time;
    double bound, timeBound;
};

struct CompareNode
{
    bool operator()(const Node &a, const Node &b) const
    {
        if (a.bound == b.bound)
            return a.timeBound > b.timeBound;

        return a.bound > b.bound;
    }
};

// Calculate an optimistic lower bound for remaining edges.
double calculateBound(
    int current,
    unsigned long long visited,
    double accumulated,
    bool useTime,
    int n,
    const vector< vector<double> > &dist,
    const vector< vector<double> > &fuel,
    const vector< vector<double> > &travel)
{
    double bound = accumulated;

    for (int s = 0; s < n; s++)
    {
        if (s != current &&
            (visited & (1ULL << s)))
            continue;

        double minimum = INF;

        for (int d = 0; d < n; d++)
        {
            bool unvisited = !(visited & (1ULL << d));

            if (d == s || (d != 0 && !unvisited))
                continue;

            if (dist[s][d] < 0)
                continue;

            double value;

            if (useTime)
                value = travel[s][d];
            else
                value = dist[s][d] * fuel[s][d];

            if (value < minimum)
                minimum = value;
        }

        if (minimum >= INF / 2)
            return INF;

        bound += minimum;
    }

    return bound;
}

int main()
{
    int n;

    cout << "Enter number of cities (2 to 63): ";
    cin >> n;

    if (!cin || n < 2 || n > 63)
    {
        cout << "Invalid number of cities.\n";
        return 0;
    }

    vector< vector<double> > dist(
        n, vector<double>(n, 0));
    vector< vector<double> > fuel(
        n, vector<double>(n, 0));
    vector< vector<double> > travel(
        n, vector<double>(n, 0));

    int i, j;

    cout << "\nEnter the DISTANCE matrix in kilometres.\n";
    cout << "Enter -1 for no road and 0 on the diagonal.\n";

    for (i = 0; i < n; i++)
    {
        for (j = 0; j < n; j++)
        {
            cin >> dist[i][j];

            if (!cin || dist[i][j] < -1 ||
                (i == j && dist[i][j] != 0))
            {
                cout << "Invalid distance matrix.\n";
                return 0;
            }
        }
    }

    cout << "\nEnter the FUEL COST PER KM matrix.\n";
    cout << "Enter 0 on the diagonal.\n";

    for (i = 0; i < n; i++)
    {
        for (j = 0; j < n; j++)
        {
            cin >> fuel[i][j];

            if (!cin || fuel[i][j] < 0 ||
                (i == j && fuel[i][j] != 0))
            {
                cout << "Invalid fuel cost matrix.\n";
                return 0;
            }
        }
    }

    cout << "\nEnter the TRAVEL TIME matrix in hours.\n";
    cout << "Enter 0 on the diagonal.\n";

    for (i = 0; i < n; i++)
    {
        for (j = 0; j < n; j++)
        {
            cin >> travel[i][j];

            if (!cin || travel[i][j] < 0 ||
                (i == j && travel[i][j] != 0))
            {
                cout << "Invalid travel time matrix.\n";
                return 0;
            }
        }
    }

    double maxTime;

    cout << "\nEnter maximum allowed delivery time (hours): ";
    cin >> maxTime;

    if (!cin || maxTime < 0)
    {
        cout << "Invalid delivery time limit.\n";
        return 0;
    }

    priority_queue<Node, vector<Node>, CompareNode> pq;

    Node root;
    root.current = 0;
    root.visited = 1ULL;
    root.path.push_back(0);
    root.cost = 0;
    root.distance = 0;
    root.time = 0;

    root.bound = calculateBound(
        0, root.visited, 0, false, n, dist, fuel, travel);

    root.timeBound = calculateBound(
        0, root.visited, 0, true, n, dist, fuel, travel);

    if (root.bound >= INF / 2 ||
        root.timeBound > maxTime)
    {
        cout << "\nNo feasible tour satisfies the time constraint.\n";
        return 0;
    }

    pq.push(root);

    double bestCost = INF;
    double bestDistance = 0;
    double bestTime = 0;
    vector<int> bestPath;

    while (!pq.empty())
    {
        Node node = pq.top();
        pq.pop();

        // Least-cost Branch and Bound pruning.
        if (node.bound >= bestCost)
            continue;

        if (node.timeBound > maxTime)
            continue;

        // All cities visited: return to starting city.
        if ((int)node.path.size() == n)
        {
            int last = node.current;

            if (dist[last][0] < 0)
                continue;

            double finalTime =
                node.time + travel[last][0];

            if (finalTime > maxTime)
                continue;

            double finalDistance =
                node.distance + dist[last][0];

            double finalCost =
                node.cost + dist[last][0] * fuel[last][0];

            if (finalCost < bestCost)
            {
                bestCost = finalCost;
                bestDistance = finalDistance;
                bestTime = finalTime;
                bestPath = node.path;
                bestPath.push_back(0);
            }

            continue;
        }

        // Expand the current node.
        for (i = 1; i < n; i++)
        {
            if (node.visited & (1ULL << i))
                continue;

            int current = node.current;

            if (dist[current][i] < 0)
                continue;

            Node child;
            child.current = i;
            child.visited =
                node.visited | (1ULL << i);

            child.path = node.path;
            child.path.push_back(i);

            child.distance =
                node.distance + dist[current][i];

            child.cost =
                node.cost + dist[current][i] * fuel[current][i];

            child.time =
                node.time + travel[current][i];

            if (child.time > maxTime)
                continue;

            child.bound = calculateBound(
                child.current, child.visited, child.cost,
                false, n, dist, fuel, travel);

            if (child.bound >= bestCost ||
                child.bound >= INF / 2)
                continue;

            child.timeBound = calculateBound(
                child.current, child.visited, child.time,
                true, n, dist, fuel, travel);

            if (child.timeBound > maxTime ||
                child.timeBound >= INF / 2)
                continue;

            pq.push(child);
        }
    }

    cout << fixed << setprecision(2);

    if (bestPath.empty())
    {
        cout << "\nNo feasible delivery tour found within the time limit.\n";
        return 0;
    }

    cout << "\n-------- OPTIMAL DELIVERY TOUR --------\n\n";
    cout << "Route: ";

    for (i = 0; i < (int)bestPath.size(); i++)
    {
        cout << "City " << bestPath[i] + 1;

        if (i + 1 < (int)bestPath.size())
            cout << " -> ";
    }

    cout << "\n\nMinimum total fuel cost: "
         << bestCost << "\n";

    cout << "Total distance: "
         << bestDistance << " km\n";

    cout << "Total delivery time: "
         << bestTime << " hours\n";

    cout << "Maximum allowed time: "
         << maxTime << " hours\n";

    cout << "Time constraint: Satisfied\n";
    cout << "Algorithm: LC Branch and Bound\n";
    cout << "Route status: OPTIMAL TOUR FOUND\n";

    return 0;
}
