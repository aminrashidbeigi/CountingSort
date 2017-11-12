//
// Created by amin on 11/12/17.
//

#include <iostream>
#include <time.h>

#define SIZE 1000000

using namespace std;

int range;

void print(int sortedArray[]) {

    for (int k = 0; k < range; k++) {
        if (sortedArray[k] != 0) {
            for (int i = 0; i < sortedArray[k]; i++) {
                cout << k << " ";
            }
        }
    }

    cout << "\n" ;
}

void CountingSort(int arr[]) {

    int sortedArray[range];
    for (int i = 0; i < range; i++) {
        sortedArray[i] = 0;
    }
    for (int i = 0; i < SIZE; i++) {
        sortedArray[arr[i]] += 1;
    }

    print(sortedArray);
}

int main()
{
    int a[SIZE];
    int min = INT32_MAX;
    int max = -INT32_MAX;
    clock_t start = clock();

    for (int m = 0; m < SIZE; m++) {
        a[m] = rand()%10;
    }

    for (int i = 0; i < SIZE; i++) {
        if (min > a[i])
            min = a[i];
        if (max < a[i])
            max = a[i];
    }

    range = max + 1;

    CountingSort(a);

    clock_t stop = clock();
    double elapsed = (double) (stop - start) / CLOCKS_PER_SEC;
    cout << "\n" << "Time: " << elapsed << "\n";
}