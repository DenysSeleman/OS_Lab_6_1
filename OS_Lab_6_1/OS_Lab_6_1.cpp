#include <iostream>
#include <vector>
#include <mutex>
#include <thread>
#include <time.h>
using namespace std;
using Matrix = vector< vector<int> >;
const int maxElement = 100;
mutex mtxCout;

Matrix generate_rand_matrix(int rows, int columns)
{
    Matrix res(rows, vector<int>(columns));
    for (auto& row : res)
        for (auto& el : row)
            el = rand() % (maxElement + 1);
    return res;
}

void print_matrix(Matrix& matrix)
{
    for (auto& row : matrix)
    {
        for (auto& el : row)
            cout << el << " ";
        cout << "\n";
    }
}

void calculate_element(int& el, Matrix& A, Matrix& B, int i, int j)
{
    if (A[i].size() != B.size())
    {
        cout << "Wrong dimensions of matrices!\n";
        exit(1);
    }

    el = 0;
    for (int g = 0; g < A[i].size(); g++)
        el += A[i][g] * B[g][j];

    mtxCout.lock();
    cout << "[" << i << ", " << j << "] = " << el << "\n";
    mtxCout.unlock();
}

int main()
{
    srand(time(NULL));
    int n, m, k;
    Matrix A;
    Matrix B;

    cout << "Enter dimensions of matrices (n, m, k): ";
    cin >> n >> m >> k;

    A = generate_rand_matrix(n, m);
    B = generate_rand_matrix(m, k);

    cout << "\nMatrix A[n x m]:\n";
    print_matrix(A);
    cout << "\nMatrix B[m x k]:\n";
    print_matrix(B);
    cout << "\n";

    vector<thread> threads;
    Matrix res(n, vector<int>(k));

    cout << "Calculations of elements:\n";
    for (int i = 0; i < n; i++)
        for (int j = 0; j < k; j++)
            threads.emplace_back(calculate_element, ref(res[i][j]), ref(A), ref(B), i, j);

    for (auto& thread : threads)
        thread.join();

    cout << "\nMatrix (A x B):\n";
    print_matrix(res);

    return 0;
}