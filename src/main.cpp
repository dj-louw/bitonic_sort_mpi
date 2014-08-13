/*
* bitonic.c: C reference implementation of Bitonic Sort
* Chris Leger <clleger@cag.lcs.mit.edu>
* $Id: bitonic.c,v 1.1 2002/07/30 05:18:03 clleger Exp $
*/

/*
* Copied almost exactly from http://www.iti.fh-flensburg.de/lang/algorithmen/sortieren/bitonic/bitonicen.htm 
* That URL also has a nice explanation of the code and theory.
*/

//#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <math.h>

// new includes
#include <iostream>
#include <vector>

using namespace std;

void begin(void);
void init(void);
void print(void);
void sort(void);
void compare(int i, int j, int dir);
void bitonicMerge(int lo, int cnt, int dir);
void bitonicSort(int lo, int cnt, int dir);


/* Globals: */
static int numiters = 1;

int main(int argc, char **argv)
{

	begin();
	return 0;
}

#define N 32
vector<int> a;
//int a[N];         // the array to be sorted
const int ASCENDING = 1;
const int DESCENDING = 0;

void begin(void)
{
		init();
		printArray();
		sort();
		printArray();
}

/** Initialize array "a" with data **/
void init() {
	int i;
	for (i = 0; i < N; i++) {
		a.push_back(N-i);
		//a[i] = (N - i);
	}
}

/** Loop through array, printing out each element **/
void printArray() {
	for (auto elem : a)
		cout << elem << " | ";
	cout << endl;
}

/** A comparator is modelled by the procedure compare, where the
* parameter dir indicates the sorting direction. If dir is ASCENDING
* and a[i] > a[j] is true or dir is DESCENDING and a[i] > a[j] is
* false then a[i] and a[j] are interchanged.
**/
void compare(int i, int j, int dir)
{
	if (dir==(a[i]>a[j]))
	{
		int h=a[i];
		a[i]=a[j];
		a[j]=h;
	}
}

/** The procedure bitonicMerge recursively sorts a bitonic sequence in
* ascending order, if dir = ASCENDING, and in descending order
* otherwise. The sequence to be sorted starts at index position lo,
* the number of elements is cnt.
**/
void bitonicMerge(int lo, int cnt, int dir)
{
	if (cnt>1)
		{
		int k=cnt/2;
		int i;
		for (i=lo; i<lo+k; i++)
			compare(i, i+k, dir);
		bitonicMerge(lo, k, dir);
		bitonicMerge(lo+k, k, dir);
	}
}

/** Procedure bitonicSort first produces a bitonic sequence by
* recursively sorting its two halves in opposite directions, and then
* calls bitonicMerge.
**/
void bitonicSort(int lo, int cnt, int dir)
{
	if (cnt>1)
	{
		int k=cnt/2;
		bitonicSort(lo, k, ASCENDING);
		bitonicSort(lo+k, k, DESCENDING);
		bitonicMerge(lo, cnt, dir);
	}
}

/** When called with parameters lo = 0, cnt = a.length() and dir =
* ASCENDING, procedure bitonicSort sorts the whole array a.
**/
void sort()
{
	bitonicSort(0, N, ASCENDING);
}