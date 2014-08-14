#ifndef FILETOOL
#define FILETOOL

/*
FileTool.hpp
Basic file operation class providing file IO
Coyright Daniël Louw <daniel@dline.co.za> 2014
*/

#include <string>
#include <fstream>	
#include <iostream>
#include <iterator> // for istream_iterator
#include <algorithm> // for std::copy
#include <vector>

using namespace std;

class FileTool
{
	private:
		std::fstream mIFile, mOFile;

		bool OpenIFile(string _name)
		{
			mIFile.open(_name, ios::in);
			if (mIFile.fail())
			{
				cout << "error opening input file\n";
				return false;
			} 
			return true;
				
		}

		bool OpenOFile(string _name)
		{
			mOFile.open(_name, ios::out);
			if (mOFile.fail())
			{
				cout << "error opening output file\n";
				return false;
			} 
			return true;
		}
	
	public:
		/**
		* Constructors and Destructors
		*/
		FileTool(){}
		~FileTool()
		{
			if (mIFile.is_open()||mOFile.is_open())
				Close();
		}


		/**
		* File Initialization
		*/
		bool InitIFile(string _iFileName)
		{
			return OpenIFile(_iFileName);
		}
		bool InitOFile(string _oFileName)
		{
			return OpenOFile(_oFileName);
		}
		bool InitIOFile(string _iFileName, string _oFileName)
		{
			bool b1 = OpenIFile(_iFileName);
			bool b2 = OpenOFile(_oFileName);
			if (!b1)
				return false;
			else if (!b2)
				return false;
			else return true;			
		}

		/*
		* Close the files if they are still open
		*/
		void Close()
		{
			if (mIFile.is_open())
				mIFile.close();
			if (mOFile.is_open())
				mOFile.close();
		}

		/*
		* Write the contents of the string to the file
		*/
		void WriteToFile(string _stringToWrite)
		{
			mOFile << _stringToWrite;
		}
		/*
		* Same as WriteToFile but appends a newline to the end
		*/
		void WriteToFileNewLine(string _stringToWrite)
		{
			mOFile << _stringToWrite << endl;
		}

		/*
		* Read a line from the opened file
		*/
		bool ReadLineFromFile(string& _currentline)
		{			
			if (mIFile.is_open())
				getline(mIFile, _currentline);

			if (!mIFile.eof())
				return true;	
			else return false;
		}

		/*
		* Reads data from a file, casting each line to int and pusing onto a vector
		*/
		void ReadIntLinesIntoVector(vector<int>& _inVec)
		{
			istream_iterator<int> start(mIFile), end;
			vector<int> tempArray(start, end);
			_inVec = tempArray;			
		}

		/*
		* Writes the contents of a vector to the file, default delimination is a newline
		*/
		void WriteVectorToFile(vector<int> _inVec, string _delim = "\n")
		{
			copy(_inVec.begin(), _inVec.end(), ostream_iterator<int>(mOFile, _delim.c_str()));
		}


};

#endif
