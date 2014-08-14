#ifndef BITONICSORTER
#define BITONICSORTER
/*
BitonicSorterMPI.hpp
Implementation of Batcher's Bitonic sort for MPI
Copyright Daniel Louw <daniel@dline.co.za>
Original serial bitonic sort code obtained from: 
	http://groups.csail.mit.edu/cag/streamit/results/bitonic/code/c/bitonic.c
and modified to suit the purpuse.
*/

#include <vector>


class BitonicSorter
{
private:
	vector<int> mDataToSort;
	const int ASCENDING;// = 1;
	const int DESCENDING;// = 0; 


public:

	BitonicSorter(): ASCENDING(1), DESCENDING(0) {};
	~BitonicSorter(){};

	void Init(vector<int> _dataToSort)
	{
		mDataToSort = _dataToSort;
	}

	vector<int> GetData()
	{
		return mDataToSort;
	}

	void Do()
	{
		bitonicSort(0, mDataToSort.size(), ASCENDING);
	}

	/** A comparator is modelled by the procedure compare, where the
	* parameter dir indicates the sorting direction. If dir is ASCENDING
	* and a[i] > a[j] is true or dir is DESCENDING and a[i] > a[j] is
	* false then a[i] and a[j] are interchanged.
	**/
	void compare(int i, int j, int dir)
	{
		if (dir==(mDataToSort[i]>mDataToSort[j]))
		{
			int h=mDataToSort[i];
			mDataToSort[i]=mDataToSort[j] ;
			mDataToSort[j]=h;
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


};


























#endif