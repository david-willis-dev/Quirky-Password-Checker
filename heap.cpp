// https://stepik.org/lesson/714340/step/1?auth=registration&unit=715144
#include <iostream>
#include <sstream>
using namespace std;
int readHeap(int* theheap)
{
    int nextVal;
    int size = 0;
    for(cin >> nextVal; nextVal != -1; size++) {
        theheap[size] = nextVal;
        cin >> nextVal;
    }
    return size;
}

void heapRemove(int* theheap, int& size)
{
    theheap[0] = theheap[--size];
    int elementEIndex = 0;
    int leftChildPos = 1;
    int rightChildPos = 2;
    while(leftChildPos < size || rightChildPos < size) {
        //First find the index of the smallest child
        int swapChildIndex;
        if(leftChildPos < size && rightChildPos >= size)
            swapChildIndex = leftChildPos;
        else if (rightChildPos < size && leftChildPos >= size) 
            swapChildIndex = rightChildPos;
        else if (leftChildPos < size && rightChildPos < size) {
            if(theheap[leftChildPos] <= theheap[rightChildPos])
                swapChildIndex = leftChildPos;
            else
                swapChildIndex = rightChildPos;
        }
        else
            return;
        if(theheap[elementEIndex] > theheap[swapChildIndex]) {
            int temp = theheap[elementEIndex];
            theheap[elementEIndex] = theheap[swapChildIndex];
            elementEIndex = swapChildIndex;
            theheap[swapChildIndex] = temp;
        }
        else
            return;
        leftChildPos = 2 * elementEIndex + 1;
        rightChildPos = 2 * elementEIndex + 2;
    }
}

void heapPrint(int* theheap, int size)
{
    for(int i = 0; i < size; i++)
        cout << theheap[i] << " ";
    cout << endl;
}
