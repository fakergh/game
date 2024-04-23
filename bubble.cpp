#include<iostream>
#include<ctime>
#include<cstdlib>
#include<omp.h>

using namespace std;

void bubblesort(int arr[], int n){
    for(int i = 0; i < n-1;++i){
        for(int j = 0; j<n-i-1; ++j){
            if(arr[j]> arr[j+1]){
                swap(arr[j],arr[j+1]);
            }
        }
    }
}

void bubble(int *, int);
void swap(int &, int &);

void bubble(int *a,int n){
    for(int i = 0;i <n; i++){
        int first = i%2;
        #pragma omp for shared(a,first)
        for(int j = first;j<n-1;j+=2){
            if(a[j]> a[j+1]){
                swap(a[j],a[j+1]);
            }
        }
    }
}

void swap(int &a, int &b){
    int test;
    test = a;
    a = b;
    b = test;
}

void merge(int arr[], int l, int m, int r){
    int i,j,k;
    int n1 = m-l+1;
    int n2 = r-m;

    int *L = new int[n1];
    int *R = new int[n2];

    for(int i = 0; i < n1;++i){
        L[i] = arr[l+i];
    }
    for(int j=0; j<n2;++j){
        R[j] = arr[m+l+j];
    }

    i = 0;
    j = 0;
    k = l;

    while(i<n1 && j<n2){
        if(L[i]<R[j]){
            arr[k] = L[i];
            ++i;
        }
        else{
            arr[k] = R[j];
            ++j;
        }
        ++k;
    }

    while(i < n1){
        arr[k] = L[i];
        ++i;
        ++k;
    }

    while(j < n2){
        arr[k] = R[j];
        ++j;
        ++k;
    }
    delete[] L;
    delete[] R;
}

void mergesort(int arr[], int l, int r){
    if(l<r){
        int m = l + (r-l)/2;
        #pragma omp parallel sections
        {
            #pragma omp section
            {
                mergesort(arr,l,m);
            }
            #pragma omp section
            {
                mergesort(arr,m+1,r);
            }
        }
        merge(arr,l,m,r);
    }
}

void print(int arr[], int size){
    for(int i = 0; i < size; ++i){
        cout<<arr[i]<<" ";
    }
    cout<<endl;
}

int main(){
    int n;
    int *a;

    cout<<"enter thr size of array: ";
    cin>>n;

    int *arr = new int[n];
    srand(time(0));
    for(int i = 0; i <n;++i){
        arr[i] = rand()%100;
    }
    a = new int[n];
    cout<<"enter elements:";
    for(int i = 0; i<n;++i){
        cin>>a[i];
    }
    bubble(a,n);
    cout<<"sorted array: ";
    for(int i = 0; i<n;i++){
        cout<<a[i]<<endl;
    }

    clock_t start = clock();
    bubblesort(arr,n);
    clock_t end = clock();

    double sbubbletime = double(end-start)/CLOCKS_PER_SEC;

    start = clock();
    #pragma omp parallel
    {
        bubblesort(arr,n);
    }
    end = clock();

    double pbubbletime = double(end-start)/CLOCKS_PER_SEC;

    start = clock();
    mergesort(arr,0,n-1);
    end = clock();

    double smergetime = double(end-start)/CLOCKS_PER_SEC;

    start = clock();
    #pragma omp parallel
    {
        #pragma omp single
        {
            mergesort(arr,0,n-1);
        }
    }
    end = clock();

    double pmergetime = double(end-start)/CLOCKS_PER_SEC;

    cout<<"sequential time: "<<sbubbletime<<" seconds"<<endl;
    cout<<"parallel time: "<<pbubbletime<<" seconds"<<endl;
    cout<<"sequential time: "<<smergetime<<" seconds"<<endl;
    cout<<"parallel time: "<<pmergetime<<" seconds"<<endl;

    delete []arr;
    return 0;
}