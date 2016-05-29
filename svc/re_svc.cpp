/*
		FILE STRUCTURE:

					     .svc
						   |
					  filename_repo
					/               \
				   /				 \
				-masterfile           version
				-master_head        /         \
								ver_head	  -v1
											  -v2
											  -v3
												:
												:
*/

#include <iostream>
#include <fstream>
#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <ctype.h>
#include <string>  												

using namespace std;


//======================== testing header file starts ===========================

/** 1. uncomment the below two line for enableing the testing.
	2. Uncomment the test cases in Testing area.
	3. Comment the main function of file.
	4. Run make
	5. execute ./re_svc
**/

// #define CATCH_CONFIG_MAIN 
// #include "catch.hpp"


#define SIMPLE_DEBUG false

//======================= testing header file ends ==============================


//======================= Global functions starts ================================

bool isInteger(string str)
{
	string word = str;
	bool isNumber = true;
	for(string::const_iterator k = word.begin(); k != word.end(); ++k)
	{
	    isNumber = isNumber && isdigit(*k);
	}
	return isNumber;
}

//======================== Global functions end ================================





class svc
{
	private:
		int version;
		string filename;
		char lines[20][10];

		// path variables for 
		string path_to_repo;
		string path_to_version;
		string path_to_masterfile;
		string path_to_master_head;
		string path_to_version_head;
		string path_to_current_version;

		// file streams objects to read and write data
		ifstream fin_masterfile;
		ifstream fin_master_head;
		ofstream fout_masterfile;
		ofstream fout_master_head;
		ifstream fin_version_head;
		ofstream fout_version_head;
		ifstream fin_version;


	public:
		svc();
		svc(string);
		svc(string, int);
		bool dirExists(const char*);
		bool fileExists(const char*);
};




svc::svc()
{
	cout<<"Available commands:\n\
	svc init -- Initialize svc on current directory\n\
	svc filename -- To commit the changes of file 'filename'\n\
	svc filename version -- retrieval of the 'version'th commit of file 'filename'\n";
}

bool svc:: dirExists(const char *path)
{
    struct stat info;

    if(stat( path, &info ) != 0)
        return 0;
    else if(info.st_mode & S_IFDIR)
        return 1;
    return 0;
}

bool svc:: fileExists(const char *path)
{
    struct stat info;

    if(stat( path, &info ) != 0)
        return 0;
    else if(info.st_mode & S_IFREG)
        return 1;
    return 0;
}


svc::svc(string filename)
{
	this->filename = filename;

	if(filename=="init")
	{
		system("mkdir .svc");
		cout<<"Initialized svc on current directory\n";
	}
	else
	{

		if(!dirExists(".svc"))   // .svc doesn't exist
		{
			cout<<"Initialization not done! Use 'svc init' to initialize the current directory\n";
		}
		else   //.svc exists
		{
			if(!fileExists(this->filename.c_str()))   // filename doesn't exist
			{
				cout<<"File "<<this->filename<<" doesn't exist!\n";
			}
			else  // filename exists
			{
				this->path_to_repo = ".svc/"+this->filename+"_repo/";
				
				if(!dirExists(this->path_to_repo.c_str()))  //repo doesn't exist!
				{
					system(("mkdir "+this->path_to_repo).c_str()); //repo created - will be a first commit to repo

					//Initializing all paths
					path_to_masterfile = this->path_to_repo + "masterfile";
					path_to_master_head = this->path_to_repo + "master_head";
					path_to_version = this->path_to_repo + "version/";
					path_to_version_head = this->path_to_version + "version_head";


					// Creting files	
					system(("touch "+this->path_to_masterfile).c_str());
					system(("touch "+this->path_to_master_head).c_str());
					system(("touch "+this->path_to_version_head).c_str());

				}
				else //repo exist - first commit is already done
				{

				}
			}
		}
	}
}


svc::svc(string filename, int file_version){

	// Requested version
	this->version = file_version; 
	// SVC initialized file
	this->filename = filename;

	// real_version variable to decrement the requested version by 1 because versions are starting with 0
	int real_version = this->version-1;
	// convert to string
	string real_version_str = to_string(real_version);

	// Path calculation for required files
	this->path_to_masterfile = ".svc/"+this->filename+"_repo/masterfile";
	this->path_to_version = ".svc/"+this->filename+"_repo/version/"+"v"+real_version_str;
	this->path_to_version_head = ".svc/"+this->filename+"_repo/version/version_head";

	//===================== validations starts ================================
	
	// check if entered file exists or not
	if(!fileExists(this->filename.c_str()))
	{
		cout<<"fatal: '"+this->filename+"' doesn't exist, Please try again."<<endl;
		return;
	}

	// setup to read the version_head
	string version_head_str;
	fin_version_head.open(this->path_to_version_head);

	// versoin_head opened or not
	if(!fin_version_head.is_open())
	{
		cout<<"fatal: Unable to open '"+this->path_to_version_head+"', Please try again."<<endl;
		return;
	}

	getline(fin_version_head,version_head_str);
	int version_head_int = stoi(version_head_str);

	// check if entered version number exists or not
	if(version_head_int < this->version)
	{
		cout<<"fatal: Version doesn't exist yet, Number of versions available are: "+to_string(version_head_int)<<endl;
		return;
	}

	// check if masterfile exists or not
	if(!fileExists(this->path_to_masterfile.c_str()))
	{
		cout<<"fatal: Masterfile file: '"+this->path_to_masterfile+"' doesn't exist for '"+this->filename+"' file, Please try again."<<endl;
		return;
	}
	// check if version file exists or not
	if(!fileExists(this->path_to_version.c_str()))
	{
		cout<<"fatal: Version file: '"+this->path_to_version+"' doesn't exist, Please try again."<<endl;
		return;
	}
	// check if version_head exists or not
	if(!fileExists(this->path_to_version_head.c_str()))
	{
		cout<<"fatal: Version head: '"+this->path_to_version_head+"' doesn't exist, Please try again."<<endl;
		return;
	}

	//===================== validations starts ================================

	// Debug: variable printer
	if(SIMPLE_DEBUG)
	{
		cout<<"================ DEBUG INFO STARTS =============="<<endl;
		cout<<"Version path is = "<<this->path_to_version<<endl;
		cout<<"Masterfile path is = "<<this->path_to_masterfile<<endl;
		cout<<"================ DEBUG INFO ENDS ================"<<endl<<endl;

	}
	
	// buffer variable used to store the contents for requested version and then it will be printed
	string buffer=""; 

	// Opening the required files
	this->fin_masterfile.open(this->path_to_masterfile);
	this->fin_version.open(this->path_to_version);

	// masterfile opened or not
	if(!fin_masterfile.is_open())
	{
		cout<<"fatal: Unable to open '"+this->path_to_masterfile+"', Please try again."<<endl;
		return;
	}

	// version file opened or not
	if(!fin_version.is_open())
	{
		cout<<"fatal: Unable to open '"+this->path_to_version+"', Please try again."<<endl;
		return;
	}

	//============================ Reading loop starts ==========================
	// Reading loop: It will read each line of version file of requested version,
	// it will fetch corresponding line from masterfile and will store those lines
	// in 'buffer' (string) variable 
	while(!fin_version.eof())
	{
		int seek_position;
		string s;
		// variable s will have a line number from version file fin_version
		getline(fin_version,s);

		// string to int
		int line = stoi(s);
		// logic variable line, wiil be used to calculate seek_position
		line = line-1; 
		// seek_posyion calculation, multiply by 10 because each line has max 10 char including newline 
		seek_position = 10*line;
		// moving the read pointer
		fin_masterfile.seekg(seek_position);

		// s2 to store the line fetched from masterfile
		string s2;
		getline(fin_masterfile,s2);

		// appending s2 to buffer
		buffer+=s2+"\n";

		// Debug: variable printer
		if(SIMPLE_DEBUG)
		{
			cout<<"================ DEBUG INFO STARTS =============="<<endl;
			cout<<"Line number in string is = "<<s<<endl;
			cout<<"Line number in int is = "<<line<<endl;
			cout<<"Seek position is = "<<seek_position<<endl;
			cout<<"Line read from master is = "<<s2<<endl;
			cout<<"================ DEBUG INFO ENDS ================"<<endl<<endl;
		}

	}
	//============================ Reading loop ends ==========================

	// Printing the buffer
	cout<<buffer;
	
}



//============================== Test cases starts ==============================
// TEST_CASE( "isInteger is computed", "[factorial]" ) {
//     REQUIRE( isInteger("2312313123131") == true );
//     REQUIRE( isInteger("2") == true );
//     REQUIRE( isInteger("assd") == false );
//     REQUIRE( isInteger("ak2r4t3454") == false );
// }
//============================== Test cases ends ==============================


int main(int argc, char const *argv[])
{
	ios_base::sync_with_stdio(false);


	//============================== 'svc filename version' starts =====================
	// if two command line arguments are present
	// first argument is program file itself, remaining two are given by user
	if(argc==3){
		
		string filename = argv[1];
		string version_str = argv[2];

		if(isInteger(version_str))
		{
			int version_int = stoi(version_str);
			svc obj(filename, version_int);
		}
		else
		{
			cout<<"fatal: 'svc filename version' expects 'version' to be an integer";
		}
	}
	//============================== 'svc filename version' starts =====================

	
	
	cout<<endl;
	return 0;
}