#include <iostream>

using namespace std;

void print(int a[], int sz) {
    cout << "sorted array: " << endl;
    for (int i = 0; i < sz;  i++ )
        cout << a[i] << " ";
    cout << endl;
}

void CountingSort(int arr[], int sz) {
    int i, j, k;
    int idx = 0;
    int min, max;

    min = max = arr[0];
    for(i = 1; i < sz; i++) {
        min = (arr[i] < min) ? arr[i] : min;
        max = (arr[i] > max) ? arr[i] : max;
    }

    k = max - min + 1;
    /* creates k buckets */
    int *B = new int [k];

//    cout << k << endl;

    for(i = 0; i < k; i++) B[i] = 0;

    for(i = 0; i < sz; i++)
        B[arr[i] - min]++;
    for(i = min; i <= max; i++)
        for(j = 0; j < B[i - min]; j++)
            arr[idx++] = i;

    print(arr,sz);

    delete [] B;
}

int main()
{
    int a[] = {5,9,3,9,10,9,2,4,13,10};
    const size_t sz = sizeof(a)/sizeof(a[0]);
    print(a,sz);
    CountingSort(a, sz);
}