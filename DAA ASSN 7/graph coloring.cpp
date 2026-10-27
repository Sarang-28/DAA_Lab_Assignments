
#include <iostream>
#include <vector>
#include <string>
#include <iomanip>

using namespace std;

string getColor(int c)
{
    string colors[] = {
        "Red", "Blue", "Green", "Yellow", "Orange",
        "Purple", "Pink", "Brown", "Cyan", "Violet"
    };

    if (c >= 0 && c < 10)
        return colors[c];

    return "ExtraColor";
}

int main()
{
    int V, E, i, j;

    cout << "ASSIGNMENT 7: EXAM TIMETABLE - GREEDY GRAPH COLORING\n";

    cout << "Enter number of courses: ";
    cin >> V;

    if (!cin || V <= 0)
    {
        cout << "Invalid number of courses.\n";
        return 0;
    }

    vector<string> course(V);

    cout << "Enter course names in ID order (one word each):\n";
    for (i = 0; i < V; i++)
    {
        cout << "Course " << i + 1 << ": ";
        cin >> course[i];
    }

    cout << "Enter number of conflicting course pairs: ";
    cin >> E;

    if (!cin || E < 0)
    {
        cout << "Invalid number of conflicts.\n";
        return 0;
    }

    vector< vector<int> > adj(V);
    vector< pair<int, int> > edges;

    cout << "Enter each pair of course IDs sharing a student:\n";
    cout << "(IDs range from 1 to " << V << ")\n";

    for (i = 0; i < E; i++)
    {
        int u, v;
        cout << "Pair " << i + 1 << ": ";
        cin >> u >> v;

        if (!cin || u < 1 || u > V || v < 1 || v > V || u == v)
        {
            cout << "Invalid course IDs.\n";
            return 0;
        }

        u--;
        v--;

        adj[u].push_back(v);
        adj[v].push_back(u);
        edges.push_back(make_pair(u, v));
    }

    cout << "\nCONFLICT TABLE\n";
    cout << "Course | Conflicting courses\n";

    for (i = 0; i < V; i++)
    {
        cout << course[i] << " | ";

        if (adj[i].empty())
            cout << "None";

        for (j = 0; j < (int)adj[i].size(); j++)
        {
            if (j > 0)
                cout << ", ";

            cout << course[adj[i][j]];
        }

        cout << "\n";
    }

    // Greedy coloring in course input order.
    vector<int> color(V, -1);
    vector<bool> used(V, false);
    int maxColor = -1;

    for (i = 0; i < V; i++)
    {
        for (j = 0; j < V; j++)
            used[j] = false;

        for (j = 0; j < (int)adj[i].size(); j++)
        {
            int neighbor = adj[i][j];

            if (color[neighbor] != -1)
                used[color[neighbor]] = true;
        }

        int c = 0;
        while (c < V && used[c])
            c++;

        color[i] = c;

        if (c > maxColor)
            maxColor = c;
    }

    int slots = maxColor + 1;

    cout << "\nEXAM COLOR ASSIGNMENT\n";
    cout << "Course | Assigned color\n";

    for (i = 0; i < V; i++)
        cout << course[i] << " | " << getColor(color[i]) << "\n";

    cout << "\nEXAMS GROUPED BY COLOR "
         << "(one color = one time slot)\n";

    for (i = 0; i < slots; i++)
    {
        cout << getColor(i) << " | ";
        bool first = true;

        for (j = 0; j < V; j++)
        {
            if (color[j] == i)
            {
                if (!first)
                    cout << ", ";

                cout << course[j];
                first = false;
            }
        }

        cout << "\n";
    }

    cout << "\nCONFLICT CHECK\n";
    cout << "Pair | Color 1 | Color 2 | Result\n";

    int violations = 0;

    for (i = 0; i < (int)edges.size(); i++)
    {
        int u = edges[i].first;
        int v = edges[i].second;

        bool pass = (color[u] != color[v]);

        cout << course[u] << " - " << course[v] << " | "
             << getColor(color[u]) << " | "
             << getColor(color[v]) << " | "
             << (pass ? "PASS" : "FAIL") << "\n";

        if (!pass)
            violations++;
    }

    cout << "\nSCHEDULING SUMMARY\n";
    cout << "Total courses scheduled: " << V << "\n";
    cout << "Total conflicts checked: " << E << "\n";
    cout << "Conflict violations: " << violations << "\n";
    cout << "Colors / time slots used: " << slots << "\n";
    cout << "Schedule validation: "
         << (violations == 0 ? "CONFLICT-FREE" : "CONFLICTS FOUND")
         << "\n";

    cout << "\nGreedy does not always give the minimum colors.\n";
    cout << "Time: O(V^2 + E) | Storage: O(V + E)\n";

    return 0;
}
