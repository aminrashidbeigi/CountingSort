//
// Created by amin on 11/12/17.
//

#include <iostream>
#include <time.h>
#include <windows.h>
#include <unistd.h>
#include "TCHAR.h"
#include "psapi.h"

#define SIZE 1000000

using namespace std;

int range;
static ULARGE_INTEGER lastCPU, lastSysCPU, lastUserCPU;
static int numProcessors;
static HANDLE self;

void init(){
    SYSTEM_INFO sysInfo;
    FILETIME ftime, fsys, fuser;

    GetSystemInfo(&sysInfo);
    numProcessors = sysInfo.dwNumberOfProcessors;

    GetSystemTimeAsFileTime(&ftime);
    memcpy(&lastCPU, &ftime, sizeof(FILETIME));

    self = GetCurrentProcess();
    GetProcessTimes(self, &ftime, &ftime, &fsys, &fuser);
    memcpy(&lastSysCPU, &fsys, sizeof(FILETIME));
    memcpy(&lastUserCPU, &fuser, sizeof(FILETIME));
}

double getCurrentValue(){
    FILETIME ftime, fsys, fuser;
    ULARGE_INTEGER now, sys, user;
    double percent;

    GetSystemTimeAsFileTime(&ftime);
    memcpy(&now, &ftime, sizeof(FILETIME));

    GetProcessTimes(self, &ftime, &ftime, &fsys, &fuser);
    memcpy(&sys, &fsys, sizeof(FILETIME));
    memcpy(&user, &fuser, sizeof(FILETIME));
    percent = (sys.QuadPart - lastSysCPU.QuadPart) +
              (user.QuadPart - lastUserCPU.QuadPart);
    percent /= (now.QuadPart - lastCPU.QuadPart);
    percent /= numProcessors;
    lastCPU = now;
    lastUserCPU = user;
    lastSysCPU = sys;

    return percent * 100;
}

SIZE_T getMemoryUsage(){
    PROCESS_MEMORY_COUNTERS_EX pmc;
    GetProcessMemoryInfo(GetCurrentProcess(), (PROCESS_MEMORY_COUNTERS*)&pmc, sizeof(pmc));
    return pmc.PrivateUsage;
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

LPCSTR CountingSort(int* arr, int sizeOfArray, int* sortedArray) {
    for (int i = 0; i < range; i++) {
        sortedArray[i] = 0;
    }
    for (int i = 0; i < sizeOfArray; i++) {
        sortedArray[arr[i]] += 1;
    }
}

int main()
{
    clock_t start = clock();
    STARTUPINFO si;
    PROCESS_INFORMATION pi;

    int* a = (int *)malloc(SIZE * sizeof(int));
    int* leftArray = (int *)malloc((SIZE/2+1) * sizeof(int));
    int* rightArray = (int *)malloc((SIZE/2+1) * sizeof(int));

    init();

    for (int m = 0; m < SIZE; m++) {
        a[m] = rand()%10;
    }

    int min = INT32_MAX;
    int max = -INT32_MAX;
    int leftCount = 0;
    int rightCount = 0;
    HANDLE childs[2];

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

    int* leftSortedArray = new int [range];
    int* rightSortedArray = new int [range];

    for (int n = 0; n < 2; n++) {
        if (n%2 == 0){
            childs[n] = (HANDLE) CreateProcess(
                    CountingSort(leftArray, leftCount, leftSortedArray),
                    NULL,
                    NULL,
                    NULL,
                    FALSE,
                    0,
                    NULL,
                    NULL,
                    &si,
                    &pi
            );
        } else {
            childs[n] = (HANDLE) CreateProcess(
                    CountingSort(rightArray, rightCount, rightSortedArray),
                    NULL,
                    NULL,
                    NULL,
                    FALSE,
                    0,
                    NULL,
                    NULL,
                    &si,
                    &pi
            );
        }
        WaitForSingleObject(childs[n],INFINITE);
    }

    int* sortedArray = new int [range];
    for (int l = 0; l < range; l++) {
        sortedArray[l] = leftSortedArray[l] + rightSortedArray[l];
    }

//    printArray(sortedArray);

    clock_t stop = clock();
    double elapsed = (double) (stop - start) / CLOCKS_PER_SEC;
    cout << "Time: " << elapsed << "s"<< endl;
    cout << "Memory Usage: " << getMemoryUsage()<< "bs" << endl;
    cout << "CPU Usage: " << getCurrentValue() << "%"<<endl;

}