/*
Implementation of Batcher's Bitonic sort for MPI
Copyright Daniel Louw <daniel@dline.co.za>
Original serial bitonic sort code obtained from: 
	http://groups.csail.mit.edu/cag/streamit/results/bitonic/code/c/bitonic.c
and modified to suit the purpuse.
*/

#include <iostream>
#include <vector>
#include <string.h>
#include "FileTool.hpp"
#include "BitonicSorter.hpp"
#include <mpi.h>

using namespace std;

/***********************************GLOBALS************************************/
string inFileName, outFileName;
FileTool ft;
int root_process = 0;

/*****************************FORWARD DECLARATIONS*****************************/
void printArray(vector<int> _inVec);
void printHeader();
bool handleInput(int argc, char ** argv);
bool is_power_of_2(int i);


   #define send_data_tag 2001
   #define return_data_tag 2002

int main(int argc, char **argv)
{

	// MPI related stuff.
    MPI_Status status;	
	int ierr = MPI_Init(&argc, &argv);
	int my_id, num_procs;
	ierr = MPI_Comm_rank(MPI_COMM_WORLD, &my_id);
    ierr = MPI_Comm_size(MPI_COMM_WORLD, &num_procs);
    if (!is_power_of_2(num_procs))
    {	
    	cout << "# of processors is not power of 2!\n";
    	MPI_Finalize();
    	return 0;
    }


    if (my_id == root_process)
 /*********************************MAIN PROCESS*********************************/
    /*
    * This is the main process.
   	* It's task is to divide the vector up in portions,
   	* send them out, collect the results,
   	* and do a final merge on the data.
   	*/
    {

		printHeader();
		if (!handleInput(argc, argv)) return 0;

		// populate the main vector with whatever is in the input file, and display
		// TODO make sure the data vector's length is a power of 2!
		vector<int> dataVector;
		ft.ReadIntLinesIntoVector(dataVector);
		printArray(dataVector);
		int amountOfDataPerProcess = dataVector.size() / num_procs;			

    	// Distribute a portion of the work to all the slave threads.
		for(int proc = 1; proc < num_procs; proc++)
		{

			int begin_index = proc * amountOfDataPerProcess;
			int end_index = ((1 + proc)*amountOfDataPerProcess) - 1;

			vector<int>::const_iterator begin_iter = dataVector.begin() + begin_index;
			vector<int>::const_iterator end_iter = dataVector.begin() + end_index;
			vector<int> tempVec(begin_iter, end_iter);

			// send the size of each slave thread's data buffer
			ierr = MPI_Send(
				&amountOfDataPerProcess,
				1,
				MPI_INT,
				proc,
				send_data_tag,
				MPI_COMM_WORLD);

			// send each thread a portion of the data
			ierr = MPI_Send(
				&tempVec.front(),
				amountOfDataPerProcess,
				MPI_INT,
				proc,
				send_data_tag,
				MPI_COMM_WORLD);
		} 
		MPI_Barrier(MPI_COMM_WORLD);


		// the master thread cannot just delegate and do nothing else
		// it must sort a portion of the data too!
		vector<int>::const_iterator begin_iter = dataVector.begin();
		vector<int>::const_iterator end_iter = dataVector.begin() + amountOfDataPerProcess;
		vector<int> masterWorkVector(begin_iter, end_iter);  

		BitonicSorter masterWorkSorter;
		masterWorkSorter.Init(dataVector);
		masterWorkSorter.Do();
		dataVector = masterWorkSorter.GetData();	



		// recieve all the sorted arrays from the slave threads.
		for(int proc = 1; proc < num_procs; proc++)
		{

		} 


    }
/*******************************************************************************/
    else	
/*********************************SLAVE PROCESS*********************************/
    /*
    * This is the slave process.
    * It's task is to recieve a portion of data to sort,
    * and return it.
    */
    {
		MPI_Barrier(MPI_COMM_WORLD);

		int amountOfDataPerProcess;
		// receive the size of the data
		ierr = MPI_Recv(
			&amountOfDataPerProcess,
			1,
			MPI_INT,
			root_process,
			send_data_tag,
			MPI_COMM_WORLD,
			&status);

		vector<int> slaveWorkVector;
		slaveWorkVector.resize(amountOfDataPerProcess);

		// receive the data.
		ierr = MPI_Recv(
			&slaveWorkVector.front(),
			amountOfDataPerProcess,
			MPI_INT,
			root_process,
			send_data_tag,
			MPI_COMM_WORLD,
			&status);





    }
/*******************************************************************************/



	
	




	// BitonicSorter sorter;
	// sorter.Init(dataVector);
	// sorter.Do();
	// dataVector = sorter.GetData();

	// printArray(dataVector);
	// ft.WriteVectorToFile(dataVector);











	MPI_Finalize();
	return 0;
}





void printArray(vector<int> _inVec) {
	for (auto elem : _inVec)
		cout << "| " << elem << " |";
	cout << endl;
}

void printHeader()
{
	string msg = "\
***************************************************************\n\
    Parallel implementation of Batcher's Bitonic MergeSort\n\
                     Daniël Louw - 29158011\n\
                 COS 786, Univeristy Of Pretoria\n\
***************************************************************\n\n";
	cout << msg << endl;	
}

bool handleInput(int argc, char ** argv)
{
	if (argc < 3)
	{
		cout << "Improper parameters specified, using default filenames" << endl;
		inFileName = "data/inputfile";
		outFileName = "data/outputfile";
	} else
	{
		inFileName = argv[1];
		outFileName = argv[2];
	}
	cout << "Using: " << inFileName << " for the input file, and" << endl;
	cout << "Using: " << outFileName << " for the output file." << endl;	
	// create and open the relevant files
	if (!ft.InitIOFile(inFileName,outFileName))
	{
		cout << "An error occured, exiting..." << endl;
		return false;
	}
	return true;
}

bool is_power_of_2(int i) {
	if ( i <= 0 ) {
		return 0;
	}
		return ! (i & (i-1));
}