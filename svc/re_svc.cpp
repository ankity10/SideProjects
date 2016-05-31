/**
						.svc
						  |
						  |
					filename_repo
						  |
			______________|___________
		   |		  	  |			  |
		   |			  |			  |
	  masterfile       mf_head    versions
	  								  |
	  							______|_____ _______ ........
	  						   |            |		|		!
	  						   |			|		|		!
	  					  version_head		v1		v2		vnth
**/

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

int str_to_int(string s)
{
	int sum=0;
	int p=1;
	for(int i=s.length()-1;i>=0;i--)
	{
		sum+=p*(s[i]-'0');
		p*=10;
	}
	return sum;
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
		string retrive();

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





string svc::retrive()
{
	// buffer variable used to store the contents for requested version and then it will be printed
	string buffer=""; 

	// Opening the required files
	this->fin_masterfile.open(this->path_to_masterfile);
	this->fin_version.open(this->path_to_version);

	// masterfile opened or not
	if(!this->fin_masterfile.is_open())
	{
		cout<<"fatal: Unable to open '"+this->path_to_masterfile+"', Please try again."<<endl;
		cout<<endl;
		exit (EXIT_FAILURE);
	}

	// version file opened or not
	if(!this->fin_version.is_open())
	{
		cout<<"fatal: Unable to open '"+this->path_to_version+"', Please try again."<<endl;
		cout<<endl;
		exit (EXIT_FAILURE);
	}

	//============================ Reading loop starts ==========================
	// Reading loop: It will read each line of version file of requested version,
	// it will fetch corresponding line from masterfile and will store those lines
	// in 'buffer' (string) variable 
	while(!this->fin_version.eof())
	{
		int seek_position;
		string s;
		// variable s will have a line number from version file fin_version
		getline(this->fin_version,s);

		// string to int
		int line = str_to_int(s);
		// logic variable line, wiil be used to calculate seek_position
		line = line-1; 
		// seek_posyion calculation, multiply by 10 because each line has max 10 char including newline 
		seek_position = 10*line;
		// moving the read pointer
		this->fin_masterfile.seekg(seek_position);

		// s2 to store the line fetched from masterfile
		string s2;
		getline(this->fin_masterfile,s2);

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
	// cout<<buffer;

	return buffer;

}






int main(int argc, char const *argv[])
{
	ios_base::sync_with_stdio(false);
	cout<<endl;


	//============================== svc retrival starts =====================
	// if two command line arguments are present
	// first argument is program file itself, remaining two are given by user
	if(argc==3)
	{
		
		string filename = argv[1];
		string version_str = argv[2];
		if(isInteger(version_str))
		{
			int version_int = str_to_int(version_str) ;
			svc obj(filename, version_int);
			string version_file = obj.retrive();
			cout<<version_file;
		}
		else
		{
			cout<<"fatal: 'svc filename version' expects 'version' to be an positive integer";
		}
	}
	//============================== svc retrival ends =====================

	
	
	cout<<endl<<endl;
	return 0;
}



//============================== Test cases starts ==============================
// TEST_CASE( "isInteger is computed", "[factorial]" ) {
//     REQUIRE( isInteger("2312313123131") == true );
//     REQUIRE( isInteger("2") == true );
//     REQUIRE( isInteger("assd") == false );
//     REQUIRE( isInteger("ak2r4t3454") == false );
// }
//============================== Test cases ends ==============================

