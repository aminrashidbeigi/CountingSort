//
// Created by amin on 11/12/17.
//

#include <iostream>
#include <time.h>
#include <unistd.h>
#include "sys/types.h"
#include "sys/sysinfo.h"
#include "stdlib.h"
#include "stdio.h"
#include "string.h"
#include <sys/wait.h>
#include <sys/mman.h>

#define SIZE 1000000

using namespace std;

int range;

int parseLine(char* line){
    int i = strlen(line);
    const char* p = line;
    while (*p <'0' || *p > '9') p++;
    line[i-3] = '\0';
    i = atoi(p);
    return i;
}

int getValue(){
    FILE* file = fopen("/proc/self/status", "r");
    int result = -1;
    char line[128];

    while (fgets(line, 128, file) != NULL){
        if (strncmp(line, "VmSize:", 7) == 0){
            result = parseLine(line);
            break;
        }
    }
    fclose(file);
    return result;
}


static unsigned long long lastTotalUser, lastTotalUserLow, lastTotalSys, lastTotalIdle;

void init(){
    FILE* file = fopen("/proc/stat", "r");
    fscanf(file, "cpu %llu %llu %llu %llu", &lastTotalUser, &lastTotalUserLow,
           &lastTotalSys, &lastTotalIdle);
    fclose(file);
}

double getCurrentValue(){
    double percent;
    FILE* file;
    unsigned long long totalUser, totalUserLow, totalSys, totalIdle, total;

    file = fopen("/proc/stat", "r");
    fscanf(file, "cpu %llu %llu %llu %llu", &totalUser, &totalUserLow,
           &totalSys, &totalIdle);
    fclose(file);

    if (totalUser < lastTotalUser || totalUserLow < lastTotalUserLow ||
        totalSys < lastTotalSys || totalIdle < lastTotalIdle){
        //Overflow detection. Just skip this value.
        percent = -1.0;
    }
    else{
        total = (totalUser - lastTotalUser) + (totalUserLow - lastTotalUserLow) +
                (totalSys - lastTotalSys);
        percent = total;
        total += (totalIdle - lastTotalIdle);
        percent /= total;
        percent *= 100;
    }

    lastTotalUser = totalUser;
    lastTotalUserLow = totalUserLow;
    lastTotalSys = totalSys;
    lastTotalIdle = totalIdle;

    return percent;
}


void printArray(int array[]){
    for (int k = 0; k < range; k++) {
        if (array[k] != 0) {
            for (int i = 0; i < array[k]; i++) {
                cout << k << " ";
            }
        }
    }
}


int* CountingSort(int* arr, int sizeOfArray, int* sortedArray) {

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

    int* a = (int *)malloc(SIZE * sizeof(int));
    int* leftArray = (int *)malloc((SIZE/2+1) * sizeof(int));
    int* rightArray = (int *)malloc((SIZE/2+1) * sizeof(int));

    for (int m = 0; m < SIZE; m++) {
        a[m] = rand()%10;
    }

    int min = INT32_MAX;
    int max = -INT32_MAX;
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


    int* sortedArray = new int [range];
    for (int l = 0; l < range; l++) {
        sortedArray[l] = leftSortedArray[l] + rightSortedArray[l];
    }


//    printArray(sortedArray);

    clock_t stop = clock();
    double elapsed = (double) (stop - start) / CLOCKS_PER_SEC;
    cout << "Time: " << elapsed << "s"<< endl;
    cout << "Memory Usage: " << getValue() << "kbs" << endl;
    cout << "CPU Usage: " << getCurrentValue() << "%"<<endl;
}