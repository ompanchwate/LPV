#include <bits/stdc++.h>

using namespace std;
using namespace std::chrono;

// Sequential computation
void sequential_operations(vector<int> &arr, int &min_val, int &max_val, long long &sum, double &avg)
{
    min_val = *min_element(arr.begin(), arr.end());
    max_val = *max_element(arr.begin(), arr.end());
    sum = accumulate(arr.begin(), arr.end(), 0ll);
    avg = double(sum) / arr.size();
}

// Parallel computation using OpenMP
void parallel_operations(vector<int> &arr, int &min_val, int &max_val, long long &sum, double &avg)
{
    int n = arr.size();
    min_val = INT_MAX;
    max_val = INT_MIN;
    sum = 0;

// Parallel Min and Max using OpenMP
#pragma omp parallel for reduction(min : min_val) reduction(max : max_val)
    for (int i = 0; i < n; ++i)
    {
        min_val = min(min_val, arr[i]);
        max_val = max(max_val, arr[i]);
    }

// Parallel Sum using OpenMP
#pragma omp parallel for reduction(+ : sum)
    for (int i = 0; i < n; ++i)
    {
        sum += arr[i];
    }

    avg = double(sum) / n;
}

int main()
{
    // Generate a large vector with random numbers
    int n = 100000000;
    vector<int> arr(n);
    for (int i = 0; i < n; ++i)
    {
        arr[i] = n - i;
    }

    int min_val, max_val;
    long long sum;
    double avg;

    // Measure execution time for sequential operations
    auto start = high_resolution_clock::now();
    sequential_operations(arr, min_val, max_val, sum, avg);
    auto end = high_resolution_clock::now();
    duration<double> sequential_time = end - start;

    cout << "\nSequential Results:" << endl;
    cout << "Min: " << min_val << ", Max: " << max_val << ", Sum: " << sum << ", Average: " << avg << endl;
    cout << "Time taken for sequential: " << sequential_time.count() << " seconds" << endl;

    // Measure execution time for parallel operations
    start = high_resolution_clock::now();
    parallel_operations(arr, min_val, max_val, sum, avg);
    end = high_resolution_clock::now();
    duration<double> parallel_time = end - start;

    cout << "\nParallel Results:" << endl;
    cout << "Min: " << min_val << ", Max: " << max_val << ", Sum: " << sum << ", Average: " << avg << endl;
    cout << "Time taken for parallel: " << parallel_time.count() << " seconds" << endl;

    return 0;
}
