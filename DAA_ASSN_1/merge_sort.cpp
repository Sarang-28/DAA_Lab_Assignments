#include <iostream>
#include <fstream>
#include <sstream>
#include <cstdlib>
using namespace std;

struct Record
{
    string userID;
    string app;
    int dailyMinutes;
    int posts;
    int likes;
    int follows;
};

Record arr[1000];
Record temp[1000];
int n = 0;

// Merge Function
void merge(int low, int mid, int high)
{
    int i = low;
    int j = mid + 1;
    int k = low;

    while (i <= mid && j <= high)
    {
        if (arr[i].dailyMinutes <= arr[j].dailyMinutes)
            temp[k++] = arr[i++];
        else
            temp[k++] = arr[j++];
    }

    while (i <= mid)
        temp[k++] = arr[i++];

    while (j <= high)
        temp[k++] = arr[j++];

    for (i = low; i <= high; i++)
        arr[i] = temp[i];
}

// Merge Sort
void mergeSort(int low, int high)
{
    if (low < high)
    {
        int mid = (low + high) / 2;

        mergeSort(low, mid);
        mergeSort(mid + 1, high);

        merge(low, mid, high);
    }
}

int main()
{
    ifstream file("social_media_usage.csv");

    if (!file)
    {
        cout << "File not found!" << endl;
        return 0;
    }

    string line;

    // Skip Header
    getline(file, line);

    while (getline(file, line) && n < 1000)
    {
        stringstream ss(line);
        string value;

        getline(ss, arr[n].userID, ',');
        getline(ss, arr[n].app, ',');

        getline(ss, value, ',');
        arr[n].dailyMinutes = atoi(value.c_str());

        getline(ss, value, ',');
        arr[n].posts = atoi(value.c_str());

        getline(ss, value, ',');
        arr[n].likes = atoi(value.c_str());

        getline(ss, value, ',');
        arr[n].follows = atoi(value.c_str());

        n++;
    }

    file.close();

    // Sort
    mergeSort(0, n - 1);

    // Display Output
    cout << "\nSorted by Daily_Minutes_Spent\n\n";
    cout << "User_ID\tApp\tMinutes\n";

    for (int i = 0; i < n; i++)
    {
        cout << arr[i].userID << "\t"
             << arr[i].app << "\t"
             << arr[i].dailyMinutes << endl;
    }

    return 0;
}
