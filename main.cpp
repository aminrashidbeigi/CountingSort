#include <iostream>

using namespace std;

int range;

void print(int a[], int sz) {
    cout << "sorted array: " << endl;
    for (int i = 0; i < sz;  i++ )
        cout << a[i] << " ";
    cout << endl;
}

int* CountingSort(int arr[], int sz) {

    int* sortedArray = new int [range];
    for (int i = 0; i < sz; i++) {
        sortedArray[i] = 0;
    }
    for (int i = 0; i < sz; i++) {
        sortedArray[arr[i]] += 1;
    }

    print(sortedArray, range);
    return sortedArray;
}

int main()
{
    int a[] = {5,9,3,9,10,9,2,4,13,10};
    const size_t sz = sizeof(a)/sizeof(a[0]);
    int min = INT32_MAX;
    int max = -INT32_MAX;
    int leftArray[sz/2+1];
    int rightArray[sz/2+1];
    int leftCount = 0;
    int rightCount = 0;


    for (int i = 0; i < sz; i++) {
        if (min > a[i])
            min = a[i];
        if (max < a[i])
            max = a[i];
    }

    range = max + 1;

    for (int j = 0; j < sz; j++) {
        if (j%2 == 0){
            leftArray[leftCount] = a[j];
            leftCount++;
        } else{
            rightArray[rightCount] = a[j];
            rightCount++;
        }
    }

    for (int i = 0; i < leftCount;  i++ )
        cout << leftArray[i] << " ";
    cout << "\n" ;
    for (int i = 0; i < rightCount;  i++ )
        cout << rightArray[i] << " ";
    cout << "\n" ;


    int* leftSortedArray = CountingSort(leftArray, leftCount);
    int* rightSortedArray = CountingSort(rightArray, rightCount);

    int* sortedArray = new int [range];
    for (int l = 0; l < range; l++) {
        sortedArray[l] = leftSortedArray[l] + rightSortedArray[l];
    }

    print(sortedArray, range);

    for (int k = 0; k < range; k++) {
        if (sortedArray[k] != 0) {
            for (int i = 0; i < sortedArray[k]; i++) {
                cout << k << " ";
            }
        }
    }
//    print(a,sz);
}