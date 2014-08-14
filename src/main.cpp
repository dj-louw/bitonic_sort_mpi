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




int main(int argc, char **argv)
{

	// MPI related stuff.
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
		vector<int> dataVector;
		ft.ReadIntLinesIntoVector(dataVector);
		printArray(dataVector);

    	int amountOfDataPerProcess = num_rows / num_procs;		
		for(an_id = 1; an_id < num_procs; an_id++)
		{
			start_item = an_id*amountOfDataPerProcess + 1;
			end_item   = (an_id + 1)*amountOfDataPerProcess;

			if((num_rows - end_item) < amountOfDataPerProcess)
				end_item = num_rows - 1;

			//num_rows_to_send = end_item - start_item + 1;

			// ierr = MPI_Send(
			// 	&num_rows_to_send,
			// 	1,
			// 	MPI_INT,
			// 	an_id,
			// 	send_data_tag,
			// 	MPI_COMM_WORLD);

			ierr = MPI_Send(
				&dataVector.front(),
				amountOfDataPerProcess,
				MPI_INT,
				an_id,
				send_data_tag,
				MPI_COMM_WORLD);
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

    }
/*******************************************************************************/



	
	




	BitonicSorter sorter;
	sorter.Init(dataVector);
	sorter.Do();

	dataVector = sorter.GetData();

	printArray(dataVector);
	ft.WriteVectorToFile(dataVector);











	
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