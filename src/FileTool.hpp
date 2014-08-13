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

using namespace std;

class FileTool
{
	private:
		std::ifstream mIFile;
		std::ofstream mOFile;
	
	public:
		FileTool(string _iFileName, string _oFileName)
		{			
			mFile.open("bin/output/"+_fileName);
		}

		~FileTool()
		{
			if (mFile.is_open())
				CloseFile();
		}

		void CloseFile()
		{
			cout << "\nClosing current file..." << endl;
			mFile.close();
		}

		void WriteToFile(string _stringToWrite)
		{
			mFile << _stringToWrite;
		}


};

#endif