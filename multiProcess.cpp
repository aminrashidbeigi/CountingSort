//
// Created by amin on 11/12/17.
//

#include <iostream>
#include <time.h>
#include <sys/wait.h>
#include <sys/mman.h>
#include <unistd.h>

#define SIZE 1000000

using namespace std;

int range;

int* CountingSort(int arr[], int sizeOfArray, int* sortedArray) {

    for (int i = 0; i < range; i++) {
        sortedArray[i] = 0;
    }
    for (int i = 0; i < sizeOfArray; i++) {
        sortedArray[arr[i]] += 1;
    }
    return sortedArray;
}

int main()
{
    clock_t start = clock();

    int a[SIZE];

    for (int m = 0; m < SIZE; m++) {
        a[m] = rand()%10;
    }

    int min = INT32_MAX;
    int max = -INT32_MAX;
    int leftArray[SIZE/2+1];
    int rightArray[SIZE/2+1];
    int leftCount = 0;
    int rightCount = 0;
    pid_t childs[2];

    for (int i = 0; i < SIZE; i++) {
        if (min > a[i])
            min = a[i];
        if (max < a[i])
            max = a[i];
    }

    range = max + 1;

    for (int j = 0; j < SIZE; j++) {
        if (j%2 == 0){
            leftArray[leftCount] = a[j];
            leftCount++;
        } else{
            rightArray[rightCount] = a[j];
            rightCount++;
        }
    }

//    int* leftSortedArray = new int [range];
//    int* rightSortedArray = new int [range];
    int* leftSortedArray = static_cast<int *>(mmap(NULL, sizeof(int) * (range) , PROT_READ | PROT_WRITE, MAP_SHARED | MAP_ANONYMOUS , -1, 0));
    int* rightSortedArray = static_cast<int *>(mmap(NULL, sizeof(int) * (range) , PROT_READ | PROT_WRITE, MAP_SHARED | MAP_ANONYMOUS , -1, 0));

    for (int n = 0; n < 2; n++) {
        childs[n] = fork();
        if (childs[n] == 0){
            if (n%2 == 0){
                leftSortedArray = CountingSort(leftArray, leftCount, leftSortedArray);
            } else {
                rightSortedArray = CountingSort(rightArray, rightCount, rightSortedArray);
            }
            return 0;
        } else {
            int a;
            waitpid(childs[n] , &a , 0);
        }
    }

    cout << leftSortedArray[0]<<endl;

    int* sortedArray = new int [range];
    for (int l = 0; l < range; l++) {
        sortedArray[l] = leftSortedArray[l] + rightSortedArray[l];
    }

    for (int k = 0; k < range; k++) {
        if (sortedArray[k] != 0) {
            for (int i = 0; i < sortedArray[k]; i++) {
                cout << k << " ";
            }
        }
    }

    clock_t stop = clock();
    double elapsed = (double) (stop - start) / CLOCKS_PER_SEC;
    cout << "\n" << "Time: " << elapsed << "\n";
}