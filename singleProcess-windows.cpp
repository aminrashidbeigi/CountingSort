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

void CountingSort(int* arr) {

    int sortedArray[range];
    for (int i = 0; i < range; i++) {
        sortedArray[i] = 0;
    }
    for (int i = 0; i < SIZE; i++) {
        sortedArray[arr[i]] += 1;
    }
//    printArray(sortedArray);
}

int main()
{
    int* a = (int *)malloc(SIZE * sizeof(int));
    int min = INT32_MAX;
    int max = -INT32_MAX;
    clock_t start = clock();
    init();
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

    cout << "Time: " << elapsed << "s"<< endl;
    cout << "Memory Usage: " << getMemoryUsage()<< "bs" << endl;
    cout << "CPU Usage: " << getCurrentValue() << "%"<<endl;

}