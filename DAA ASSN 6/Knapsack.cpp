
#include <iostream>
#include <vector>
#include <string>
#include <iomanip>

using namespace std;

struct Item
{
    string name;
    int weight, utility, priority, bonus, finalUtility;
    bool allocated;
};

int main()
{
    int N, T, i, j, t;

    cout << "DISASTER RELIEF RESOURCE ALLOCATION:\n";
    cout << "Enter number of relief items: ";
    cin >> N;

    if (!cin || N <= 0)
    {
        cout << "Invalid number of items.\n";
        return 0;
    }

    vector<Item> items(N);

    cout << "\nEnter item details: Name Weight Utility Priority (0/1)\n";

    for (i = 0; i < N; i++)
    {
        cout << "Item " << i + 1 << ": ";
        cin >> items[i].name >> items[i].weight
            >> items[i].utility >> items[i].priority;

        if (!cin || items[i].weight <= 0 ||
            items[i].utility < 0 ||
            (items[i].priority != 0 && items[i].priority != 1))
        {
            cout << "Invalid item details.\n";
            return 0;
        }

        items[i].bonus = items[i].priority == 1 ? 10 : 0;
        items[i].finalUtility = items[i].utility + items[i].bonus;
        items[i].allocated = false;
    }

    cout << "\nRELIEF ITEM INVENTORY:\n";
    cout << left << setw(12) << "Item"
         << setw(15) << "Weight (kg)"
         << setw(12) << "Utility"
         << setw(12) << "Priority"
         << setw(10) << "Bonus"
         << setw(15) << "Final Utility" << "\n";

    for (i = 0; i < N; i++)
    {
        cout << left << setw(12) << items[i].name
             << setw(15) << items[i].weight
             << setw(12) << items[i].utility
             << setw(12) << (items[i].priority == 1 ? "High" : "Normal")
             << setw(10) << items[i].bonus
             << setw(15) << items[i].finalUtility << "\n";
    }
    cout << "------------------------------------------------------------------------\n";

    cout << "\nEnter number of trucks: ";
    cin >> T;

    if (!cin || T <= 0)
    {
        cout << "Invalid number of trucks.\n";
        return 0;
    }

    int totalWeightAll = 0;
    int totalUtilityAll = 0;

    for (t = 0; t < T; t++)
    {
        int capacity;

        cout << "\nEnter capacity of truck " << t + 1 << " (kg): ";
        cin >> capacity;

        if (!cin || capacity < 0)
        {
            cout << "Invalid truck capacity.\n";
            return 0;
        }

        vector<int> chosen;
        int remainingCapacity = capacity;

        // Give high-priority items first preference.
        for (i = 0; i < N; i++)
        {
            if (!items[i].allocated &&
                items[i].priority == 1 &&
                items[i].weight <= remainingCapacity)
            {
                chosen.push_back(i);
                items[i].allocated = true;
                remainingCapacity -= items[i].weight;
            }
        }

        // Optimize the remaining capacity using 0/1 Knapsack.
        vector<int> available;

        for (i = 0; i < N; i++)
        {
            if (!items[i].allocated && items[i].priority == 0)
                available.push_back(i);
        }

        int M = (int)available.size();

        vector<vector<int> > dp(
            M + 1, vector<int>(remainingCapacity + 1, 0)
        );

        for (i = 1; i <= M; i++)
        {
            int index = available[i - 1];

            for (j = 0; j <= remainingCapacity; j++)
            {
                dp[i][j] = dp[i - 1][j];

                if (items[index].weight <= j)
                {
                    int newUtility =
                        items[index].finalUtility +
                        dp[i - 1][j - items[index].weight];

                    if (newUtility > dp[i][j])
                        dp[i][j] = newUtility;
                }
            }
        }

        // Recover the selected items.
        int capLeft = remainingCapacity;
        vector<int> dpChosen;

        for (i = M; i >= 1; i--)
        {
            if (dp[i][capLeft] != dp[i - 1][capLeft])
            {
                int index = available[i - 1];
                dpChosen.push_back(index);
                capLeft -= items[index].weight;
            }
        }

        for (i = (int)dpChosen.size() - 1; i >= 0; i--)
        {
            int index = dpChosen[i];
            chosen.push_back(index);
            items[index].allocated = true;
        }

        int truckWeight = 0;
        int truckUtility = 0;

        cout << "\n TRUCK " << t + 1
             << " - SELECTED ITEMS \n";
        cout << left << setw(12) << "Item"
             << setw(15) << "Weight (kg)"
             << setw(12) << "Utility"
             << setw(12) << "Priority"
             << setw(10) << "Bonus"
             << setw(15) << "Final Utility" << "\n";

        if (chosen.empty())
            cout << "No items selected.\n";

        for (i = 0; i < (int)chosen.size(); i++)
        {
            int index = chosen[i];

            cout << left << setw(12) << items[index].name
                 << setw(15) << items[index].weight
                 << setw(12) << items[index].utility
                 << setw(12) << (items[index].priority == 1 ? "High" : "Normal")
                 << setw(10) << items[index].bonus
                 << setw(15) << items[index].finalUtility << "\n";

            truckWeight += items[index].weight;
            truckUtility += items[index].finalUtility;
        }

    
        cout << "\nTRUCK " << t + 1 << " SUMMARY :\n";
        cout << "Truck Capacity       : " << capacity << " kg\n";
        cout << "Total Weight Loaded  : " << truckWeight << " kg\n";
        cout << "Remaining Capacity   : " << capacity - truckWeight << " kg\n";
        cout << "Items Selected       : " << chosen.size() << "\n";
        cout << "Maximum Utility      : " << truckUtility << "\n";

        totalWeightAll += truckWeight;
        totalUtilityAll += truckUtility;
    }

    cout << "\n FINAL ALLOCATION SUMMARY:\n";
    cout << "Total Trucks             : " << T << "\n";
    cout << "Total Weight Loaded      : " << totalWeightAll << " kg\n";
    cout << "Total Utility            : " << totalUtilityAll << "\n";

    cout << "\n REMAINING ITEMS \n";
    cout << left << setw(12) << "Item"
         << setw(15) << "Weight (kg)"
         << setw(15) << "Utility"
         << setw(15) << "Status" << "\n";


    for (i = 0; i < N; i++)
    {
        cout << left << setw(12) << items[i].name
             << setw(15) << items[i].weight
             << setw(15) << items[i].utility
             << setw(15) << (items[i].allocated ? "Allocated" : "Available")
             << "\n";
    }

    cout << "\nTime Complexity per truck: O(N*W)\n";
    cout << "Space Complexity per truck: O(N*W)\n";
    
    return 0;
}
