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

//#define CATCH_CONFIG_MAIN 
//#include "catch.hpp"

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
	this->version = file_version;
	this->filename = filename;

	
	
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