#include <bits/stdc++.h>
#include <omp.h>
#include <chrono>
using namespace std;
using namespace std::chrono;

// Sequential Bubble Sort
void bubbleSortSequential(vector<int> &arr)
{
    int n = arr.size();
    for (int i = 0; i < n - 1; i++)
        for (int j = 0; j < n - i - 1; j++)
            if (arr[j] > arr[j + 1])
                swap(arr[j], arr[j + 1]);
}

// Parallel Bubble Sort using Odd-Even Transposition
void bubbleSortParallel(vector<int> &arr)
{
    int n = arr.size();
#pragma omp parallel
    {
        for (int i = 0; i < n; i++)
        {
            int start = i % 2;
#pragma omp for
            for (int j = start; j < n - 1; j += 2)
            {
                if (arr[j] > arr[j + 1])
                    swap(arr[j], arr[j + 1]);
            }
        }
    }
}

// Sequential Merge Sort
void merge(vector<int> &arr, int l, int m, int r)
{
    vector<int> temp;    // temporary array
    int left = l;      // starting index of left half of arr
    int right = m + 1; // starting index of right half of arr

    // storing elements in the temporary array in a sorted manner//

    while (left <= m && right <= r)
    {
        if (arr[left] <= arr[right])
        {
            temp.push_back(arr[left]);
            left++;
        }
        else
        {
            temp.push_back(arr[right]);
            right++;
        }
    }

    // if elements on the left half are still left //

    while (left <= m)
    {
        temp.push_back(arr[left]);
        left++;
    }

    //  if elements on the right half are still left //
    while (right <= r)
    {
        temp.push_back(arr[right]);
        right++;
    }

    // transfering all elements from temporary to arr //
    for (int i = l; i <= r; i++)
    {
        arr[i] = temp[i - l];
    }
}

void mergeSortSequential(vector<int> &arr, int l, int r)
{
    if (l < r)
    {
        int m = l + (r - l) / 2;
        mergeSortSequential(arr, l, m);
        mergeSortSequential(arr, m + 1, r);
        merge(arr, l, m, r);
    }
}

void mergeSortParallel(vector<int> &arr, int l, int r, int depth = 0)
{
    if (l < r)
    {
        int m = (l + r) / 2;
        if (depth <= 3)
        {
#pragma omp parallel sections
            {
#pragma omp section
                mergeSortParallel(arr, l, m, depth + 1);
#pragma omp section
                mergeSortParallel(arr, m + 1, r, depth + 1);
            }
        }
        else
        {
            mergeSortSequential(arr, l, m);
            mergeSortSequential(arr, m + 1, r);
        }
        merge(arr, l, m, r);
    }
}

void printArray(vector<int> &arr)
{
    for (auto x : arr)
        cout << x << " ";
    cout << endl;
}

int main()
{
    int size = 1000;
    vector<int> arr(size);
    for (int i = 0; i < size; i++)
        arr[i] = size - i;

    vector<int> arr1 = arr, arr2 = arr, arr3 = arr, arr4 = arr;

    // Bubble Sort Sequential
    auto start = high_resolution_clock::now();
    bubbleSortSequential(arr1);
    auto end = high_resolution_clock::now();
    duration<double> time = end - start;
    cout << "Sequential Bubble Sort Time: " << time.count() << " seconds\n";

    // Bubble Sort Parallel
    start = high_resolution_clock::now();
    bubbleSortParallel(arr2);
    end = high_resolution_clock::now();
    time = end - start;
    cout << "Parallel Bubble Sort Time: " << time.count() << " seconds\n";

    // Merge Sort Sequential
    start = high_resolution_clock::now();
    mergeSortSequential(arr3, 0, size - 1);
    end = high_resolution_clock::now();
    time = end - start;
    cout << "Sequential Merge Sort Time: " << time.count() << " seconds\n";

    // Merge Sort Parallel
    start = high_resolution_clock::now();
    mergeSortParallel(arr4, 0, size - 1);
    end = high_resolution_clock::now();
    time = end - start;
    cout << "Parallel Merge Sort Time: " << time.count() << " seconds\n";

    return 0;
}
