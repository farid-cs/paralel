#include <iostream>
#include <thread>
#include <vector>
#include <chrono> // For timing
#include <cstdlib> // For random number generation

using namespace std;
using namespace chrono;

// Function to compute a portion of the result matrix
void multiplyPortion(int startRow, int endRow, int rowsA, int colsA, int colsB, 
                     int A[][500], int B[][500], int C[][500]) {
    for (int i = startRow; i < endRow; ++i) {
        for (int j = 0; j < colsB; ++j) {
            C[i][j] = 0;
            for (int k = 0; k < colsA; ++k) {
                C[i][j] += A[i][k] * B[k][j];
            }
        }
    }
}

int main() {
    int rowsA, colsA, rowsB, colsB;

    // Input dimensions for Matrix A
    cout << "Enter the number of rows and columns for Matrix A: ";
    cin >> rowsA >> colsA;

    // Input dimensions for Matrix B
    cout << "Enter the number of rows and columns for Matrix B: ";
    cin >> rowsB >> colsB;

    // Check if multiplication is possible
    if (colsA != rowsB) {
        cout << "Matrix multiplication not possible. Number of columns in A must equal number of rows in B." << endl;
        return 0;
    }

    // Declare matrices
    int A[500][500], B[500][500], C[500][500] = {0}, C_parallel[500][500] = {0};

    // Randomly fill Matrix A
    cout << "Randomly filling Matrix A:" << endl;
    for (int i = 0; i < rowsA; ++i) {
        for (int j = 0; j < colsA; ++j) {
            A[i][j] = rand() % 1000; // Random number between 0 and 9
            cout << A[i][j] << " ";
        }
        cout << endl;
    }

    // Randomly fill Matrix B
    cout << "Randomly filling Matrix B:" << endl;
    for (int i = 0; i < rowsB; ++i) {
        for (int j = 0; j < colsB; ++j) {
            B[i][j] = rand() % 1000; // Random number between 0 and 9
            cout << B[i][j] << " ";
        }
        cout << endl;
    }

    // Measure Sequential Execution Time
    auto startSequential = high_resolution_clock::now();
    multiplyPortion(0, rowsA, rowsA, colsA, colsB, A, B, C);
    auto endSequential = high_resolution_clock::now();
    double sequentialTime = duration_cast<milliseconds>(endSequential - startSequential).count();
    cout << "Sequential Execution Time: " << sequentialTime << " ms" << endl;

    // Measure Parallel Execution Time
    int numThreads = 4;
    vector<thread> threads;
    int rowsPerThread = rowsA / numThreads;
    int extraRows = rowsA % numThreads;
    int startRow = 0;

    auto startParallel = high_resolution_clock::now();
    for (int t = 0; t < numThreads; ++t) {
        int endRow = startRow + rowsPerThread + (t < extraRows ? 1 : 0);
        threads.emplace_back(multiplyPortion, startRow, endRow, rowsA, colsA, colsB, A, B, C_parallel);
        startRow = endRow;
    }
    for (auto &th : threads) {
        th.join();
    }
    auto endParallel = high_resolution_clock::now();
    double parallelTime = duration_cast<milliseconds>(endParallel - startParallel).count();
    cout << "Parallel Execution Time: " << parallelTime << " ms" << endl;

    // Compute Speedup
    double speedup = sequentialTime / parallelTime;
    cout << "Speedup: " << speedup << endl;

    return 0;
}
