#include <iostream>
#include <vector>
#include <algorithm>
#include <string>
#include <iomanip>

using namespace std;

struct Item
{
    string name;
    double weight;
    double value;
    double ratio;
    bool divisible;

    double selectedWeight;
    double selectedValue;
    double fraction;
    string status;
};

// Sort items according to utility per kilogram
bool compare(const Item& a, const Item& b)
{
    return a.ratio > b.ratio;
}

int main()
{
    vector<Item> items;
    Item item;

    int n;
    double W;
    char choice;

    cout << " EMERGENCY RELIEF SUPPLY DISTRIBUTION :\n";

    cout << "Enter number of relief items: ";
    cin >> n;

    if (n <= 0)
    {
        cout << "Number of items must be positive.\n";
        return 0;
    }

    cout << "Enter boat capacity in kg: ";
    cin >> W;

    if (W <= 0)
    {
        cout << "Boat capacity must be positive.\n";
        return 0;
    }

    // Take item details from the user
    for (int i = 0; i < n; i++)
    {
        cout << "\nEnter details for Item " << i + 1 << ":\n";

        cout << "Item name: ";
        cin >> ws;
        getline(cin, item.name);

        cout << "Weight (kg): ";
        cin >> item.weight;

        if (item.weight <= 0)
        {
            cout << "Weight must be positive. Try again.\n";
            i--;
            continue;
        }

        cout << "Utility value: ";
        cin >> item.value;

        if (item.value < 0)
        {
            cout << "Utility value cannot be negative. Try again.\n";
            i--;
            continue;
        }

        cout << "Can this item be divided? (y/n): ";
        cin >> choice;

        if (choice == 'y' || choice == 'Y')
            item.divisible = true;
        else
            item.divisible = false;

        item.ratio = item.value / item.weight;

        item.selectedWeight = 0;
        item.selectedValue = 0;
        item.fraction = 0;
        item.status = "Not processed";

        items.push_back(item);
    }

    // Display the entered item details
    cout << "\n\nENTERED ITEM DETAILS :\n";

    cout << left
         << setw(22) << "Item Name"
         << setw(12) << "Weight"
         << setw(12) << "Value"
         << setw(12) << "Ratio"
         << setw(15) << "Divisible"
         << endl;

    cout << string(73, '-') << endl;

    for (int i = 0; i < (int)items.size(); i++)
    {
        cout << left
             << setw(22) << items[i].name
             << setw(12) << items[i].weight
             << setw(12) << items[i].value
             << setw(12) << fixed << setprecision(2)
             << items[i].ratio
             << setw(15)
             << (items[i].divisible ? "Yes" : "No")
             << endl;
    }

    cout << "\nBoat Capacity: " << W << " kg\n";

    // Sort items by descending utility-to-weight ratio
    sort(items.begin(), items.end(), compare);

    double remainingWeight = W;
    double totalValue = 0;

    // Apply the greedy selection algorithm
    for (int i = 0; i < (int)items.size(); i++)
    {
        if (remainingWeight <= 0)
        {
            items[i].status = "No capacity left";
            continue;
        }

        // Select the entire item if it fits
        if (items[i].weight <= remainingWeight)
        {
            items[i].selectedWeight = items[i].weight;
            items[i].selectedValue = items[i].value;
            items[i].fraction = 1.0;
            items[i].status = "Full item";

            remainingWeight -= items[i].weight;
            totalValue += items[i].value;
        }

        // Select a fraction if the item can be divided
        else if (items[i].divisible)
        {
            items[i].selectedWeight = remainingWeight;

            items[i].fraction =
                remainingWeight / items[i].weight;

            items[i].selectedValue =
                items[i].fraction * items[i].value;

            items[i].status = "Partial item";

            totalValue += items[i].selectedValue;
            remainingWeight = 0;
        }

        // Skip an indivisible item that does not fit
        else
        {
            items[i].status = "Skipped";
        }
    }

    // Display the final selection table
    cout << "\n\nRELIEF SUPPLY SELECTION \n";

    cout << left
         << setw(20) << "Item Name"
         << setw(10) << "Ratio"
         << setw(13) << "Total Wt."
         << setw(13) << "Taken Wt."
         << setw(13) << "Total Value"
         << setw(13) << "Gained"
         << setw(12) << "Fraction"
         << setw(20) << "Status"
         << endl;

    cout << string(114, '-') << endl;

    for (int i = 0; i < (int)items.size(); i++)
    {
        cout << left
             << setw(20) << items[i].name
             << setw(10) << fixed << setprecision(2)
             << items[i].ratio
             << setw(13) << items[i].weight
             << setw(13) << items[i].selectedWeight
             << setw(13) << items[i].value
             << setw(13) << items[i].selectedValue
             << setw(12) << items[i].fraction
             << setw(20) << items[i].status
             << endl;
    }

    cout << string(114, '-') << endl;

    cout << "\n FINAL RESULT \n";

    cout << "Boat Capacity       : "
         << W << " kg\n";

    cout << "Weight Used         : "
         << W - remainingWeight << " kg\n";

    cout << "Remaining Capacity  : "
         << remainingWeight << " kg\n";

    cout << "Maximum Utility     : "
         << fixed << setprecision(2)
         << totalValue << endl;

    return 0;
}
