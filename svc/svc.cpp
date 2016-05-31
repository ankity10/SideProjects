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



//================= class definition starts======================
class svc
{
	private:
		int version;
		string filename;
		string path_to_prev_version;

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

		ofstream fout_current_version;
		ifstream fin_filename;
		ifstream fin_prev_version;
		int master_head;


	public:
		svc();
		svc(string);
		svc(string, int);
		bool dirExists(const char*);
		bool fileExists(const char*);

		void initialize_all();
		void commit_ver0();
		void commit();
		int str_to_int(string);
		string itos(int);

		string retrive();

};
//================= class definition ends======================




//================= constructor starts======================
svc::svc()
{
	cout<<"Available commands:\n\
	svc init -- Initialize svc on current directory\n\
	svc filename -- To commit the changes of file 'filename'\n\
	svc filename version -- retrieval of the 'version'th commit of file 'filename'\n";
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
					initialize_all();

					// call commit func
					commit_ver0();

				}

				else //repo exist - first commit is already done
				{
					initialize_all();
					commit();	
				}
			}
		}
	}
}

svc::svc(string filename, int file_version){



	if(!dirExists(".svc"))   // .svc doesn't exist
	{
			cout<<"Initialization not done! Use 'svc init' to initialize the current directory\n";
			cout<<endl;
			exit(EXIT_FAILURE);
	}
	else   //.svc exists
	{

			// Requested version
		this->version = file_version; 
		// SVC initialized file
		this->filename = filename;

		// real_version variable to decrement the requested version by 1 because versions are starting with 0
		int real_version = this->version;
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
			cout<<"fatal: File '"+this->filename+"' doesn't exist, Please try again."<<endl;
			cout<<endl;
			exit (EXIT_FAILURE);
		}

		this->path_to_repo = ".svc/"+this->filename+"_repo/";
				
		if(!dirExists(this->path_to_repo.c_str()))  //repo doesn't exist!
		{
			cout<<"fatal: File repository for file '"+this->filename+"' not initialized, make atleast 1 commit using 'svc "+this->filename+"' to initialize file repository."<<endl;
			cout<<endl;
			exit(EXIT_FAILURE);
		}

		// setup to read the version_head
		string version_head_str;
		this->fin_version_head.open(this->path_to_version_head);

		// versoin_head opened or not
		if(!this->fin_version_head.is_open())
		{
			cout<<"fatal: Unable to open '"+this->path_to_version_head+"', Please try again."<<endl;
			cout<<endl;
			exit (EXIT_FAILURE);
		}

		getline(this->fin_version_head,version_head_str);
		int version_head_int = str_to_int(version_head_str);

		// check if entered version number exists or not
		if(version_head_int < this->version)
		{
			cout<<"fatal: Version doesn't exist yet, Number of versions available are: "+to_string(version_head_int)<<endl;
			cout<<endl;
			exit (EXIT_FAILURE);
		}
		// check if entered version number is leass than 0 or not
		if(this->version<0)
		{
			cout<<"fatal: Version number cannot be less then 0, version num"<<endl;
			cout<<endl;
			exit (EXIT_FAILURE);
		}

		// check if masterfile exists or not
		if(!fileExists(this->path_to_masterfile.c_str()))
		{
			cout<<"fatal: Masterfile file: '"+this->path_to_masterfile+"' doesn't exist for '"+this->filename+"' file, Please try again."<<endl;
			cout<<endl;
			exit (EXIT_FAILURE);
		}
		// check if version file exists or not
		if(!fileExists(this->path_to_version.c_str()))
		{
			cout<<"fatal: Version file: '"+this->path_to_version+"' doesn't exist, Please try again."<<endl;
			cout<<endl;
			exit (EXIT_FAILURE);
		}
		// check if version_head exists or not
		if(!fileExists(this->path_to_version_head.c_str()))
		{
			cout<<"fatal: Version head: '"+this->path_to_version_head+"' doesn't exist, Please try again."<<endl;
			cout<<endl;
			exit (EXIT_FAILURE);
		}

		//===================== validations ends ================================

		// Debug: variable printer
		if(SIMPLE_DEBUG)
		{
			cout<<"================ DEBUG INFO STARTS =============="<<endl;
			cout<<"Version path is = "<<this->path_to_version<<endl;
			cout<<"Masterfile path is = "<<this->path_to_masterfile<<endl;
			cout<<"================ DEBUG INFO ENDS ================"<<endl<<endl;

		}
	}
	
}
//================= constructor ends======================




//================= utils starts======================
int svc::str_to_int(string s)
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


string svc::itos(int n)
{
	int p=1;
	int len=0;
	while(n/p)
	{
		len++;
		p*=10;
	}
	string s;
	s.resize(len);
	for(int i=len-1;i>=0;i--)
	{
		s[i]=n%10+'0';
		n/=10;
	}
	return s;
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

//================= utils ends======================



//================= functionality code starts======================
void svc:: commit_ver0()
{
// Creating files	
	system(("touch "+this->path_to_masterfile).c_str());
	system(("touch "+this->path_to_master_head).c_str());
	system(("mkdir "+this->path_to_version).c_str());
	system(("touch "+this->path_to_version_head).c_str());

	this->path_to_current_version = this->path_to_version+"v0";
	
	system(("touch "+this->path_to_current_version).c_str());


//Opening files
	this->fin_masterfile.open(path_to_masterfile.c_str());
	this->fin_master_head.open(path_to_master_head.c_str());
	this->fout_masterfile.open(path_to_masterfile.c_str());
	this->fout_master_head.open(path_to_master_head.c_str());
	this->fin_version_head.open(path_to_version_head.c_str());
	this->fout_version_head.open(path_to_version_head.c_str());
	this->fin_filename.open(this->filename.c_str());

	
	this->fout_current_version.open(this->path_to_current_version.c_str());
	
	this->fout_version_head<<"0";
	this->master_head=0;

	fin_filename.seekg(0,ios::beg);
	while(!this->fin_filename.eof())
	{
		this->master_head++;
		this->fout_current_version<<this->master_head<<endl;

		string s;
		getline(this->fin_filename, s);
		this->fout_masterfile<<s;
		for(int i=s.length()+1;i<10;i++)
		{
			this->fout_masterfile<<" ";
		}
		this->fout_masterfile<<endl;
	}

	this->fout_master_head<<this->master_head;   //Updating master head

	//Closing all
	this->fin_masterfile.close();
	this->fin_master_head.close();
	this->fout_masterfile.close();
	this->fout_master_head.close();
	this->fin_version_head.close();
	this->fout_version_head.close();
	this->fin_filename.close();
	this->fout_current_version.close();
}


void svc::commit()
{
	this->fin_version_head.open(this->path_to_version_head.c_str());
	
	string version_head;
	getline(this->fin_version_head, version_head);
	this->fin_version_head.close();
	
	this->path_to_prev_version = path_to_version+"v"+version_head;
	this->fin_prev_version.open(this->path_to_prev_version.c_str());

	int ver_head = str_to_int(version_head);
	ver_head++;
	string current_version = "v"+itos(ver_head);
	this->fout_version_head.open(this->path_to_version_head.c_str());
	fout_version_head<<ver_head;
	this->fout_version_head.close();

	this->path_to_current_version = path_to_version+current_version;
	this->fout_current_version.open(path_to_current_version.c_str());
	this->fin_filename.open(filename.c_str());
	

	this->fout_masterfile.open(path_to_masterfile.c_str(),ios::app);
	this->fin_master_head.open(path_to_master_head.c_str());
	

	int file_last = 0;
	string last_line;
	this->fin_filename.seekg(0, ios::beg);
	while(!this->fin_filename.eof())
	{
		getline(fin_filename,last_line);
		file_last++;
	}
	fin_filename.close();
	// cout<<last_line<<endl;


	fin_prev_version.seekg(0, ios::beg);
	int prev_last=0;
	string temp;

	while(!this->fin_prev_version.eof() )
	{
		getline(this->fin_prev_version, temp);
		prev_last++;
		// cout<<temp<<endl;
	}
	fin_prev_version.close();
	prev_last--;

	if(SIMPLE_DEBUG)
	{
			cout<<"================ DEBUG INFO STARTS =============="<<endl;
			cout<<"Last line of current file is = "<<last_line<<endl;
			cout<<"Last line of previous file is = "<<temp<<endl;
			cout<<"Line no of last line of current file is = "<<file_last<<endl;
			cout<<"Line no of previuos verssion file is = "<<prev_last<<endl;
			cout<<"================ DEBUG INFO ENDS ================"<<endl<<endl;
	}

	// if(file_last>prev_last)  //Append the last line
	// {
		
		
	// 	fin_prev_version.open(path_to_prev_version.c_str());
	// 	// cout<<"Prev_last="<<prev_last<<endl;
	// 	int debug_prev_last = prev_last;

	// 	if(SIMPLE_DEBUG)
	// 	{
	// 		cout<<"================ DEBUG INFO STARTS =============="<<endl;
	// 		cout<<"Inside if of commit()  function case"<<endl;
	// 		cout<<"================ DEBUG INFO ENDS ================"<<endl<<endl;
	// 	}

	// 	while(prev_last--)
	// 	{
	// 		string s="";
	// 		getline(this->fin_prev_version, s);
	// 		// cout<<"s="<<s<<endl;
	// 		fout_current_version<<s<<endl;
	// 		if(SIMPLE_DEBUG)
	// 		{
	// 			cout<<"================ DEBUG INFO STARTS =============="<<endl;
	// 			cout<<(debug_prev_last-prev_last)<<"th line of previuos version file is "<<last_line<<endl;
	// 			cout<<"================ DEBUG INFO ENDS ================"<<endl<<endl;
	// 		}
	// 	}

		
	// 	fout_masterfile<<last_line;
	// 	for(int i=last_line.length()+1;i<10;i++)
	// 		fout_masterfile<<" ";
	// 	fout_masterfile<<endl;
		
	// 	string master_head;
	// 	fin_master_head>>master_head;
	// 	this->master_head=str_to_int(master_head);
	// 	this->master_head++;
	// 	master_head=itos(this->master_head);

	// 	this->fout_master_head.open(path_to_master_head.c_str());
	// 	this->fout_master_head<<master_head;

	// 	this->fout_current_version<<this->master_head<<endl;
	// }

	if(file_last>prev_last)   //Insert line in between
	{
		cout<<"In if!!\n";
		if(SIMPLE_DEBUG)
		{
			cout<<"================ DEBUG INFO STARTS =============="<<endl;
			cout<<"Inside if of commit()  function case"<<endl;
			cout<<"================ DEBUG INFO ENDS ================"<<endl<<endl;
		}
		this->fin_masterfile.open(this->path_to_masterfile.c_str());
		this->fin_prev_version.open(this->path_to_prev_version.c_str());
		this->fin_filename.open(this->filename.c_str());

		string prev_line;
		string file_line;
		string line_no;
		string temp;
		file_line.resize(9);

		getline(fin_prev_version, line_no);
		int line = str_to_int(line_no);

		int file_ptr = (line-1)*10;
		fin_masterfile.seekg(file_ptr);
		getline(fin_masterfile, prev_line);

		getline(fin_filename, temp);
		for(int i=0;i<temp.length();i++)
			file_line[i]=temp[i];
		for(int i=temp.length();i<9;i++)
			file_line[i]=' ';

		
		while(file_line==prev_line && prev_last)
		{
			prev_last--;
			fout_current_version<<line_no<<endl;

			getline(fin_filename, temp);
			for(int i=0;i<temp.length();i++)
				file_line[i]=temp[i];
			for(int i=temp.length();i<9;i++)
				file_line[i]=' ';

			getline(fin_prev_version, line_no);
			line = str_to_int(line_no);

			int file_ptr = (line-1)*10;
			fin_masterfile.seekg(file_ptr);
			getline(fin_masterfile, prev_line);
		}		

		fout_masterfile<<file_line<<endl;   //Adding the inserted line in masterfile
		string master_no;
		fin_master_head>>master_no;
		int master_line_no = str_to_int(master_no);
		master_line_no++;
		fout_master_head.open(path_to_master_head.c_str());
		fout_master_head<<master_line_no;   //Updating the master_head

		fout_current_version<<master_line_no<<endl; //Adding the inserted line to current version

		while(prev_last--)
		{
			fout_current_version<<line_no<<endl;
			getline(fin_prev_version, line_no);
		}
		
		this->fin_masterfile.close();

	}
	else    //Delete one line
	{
		// cout<<"In else!!";
		if(SIMPLE_DEBUG)
		{
			cout<<"================ DEBUG INFO STARTS =============="<<endl;
			cout<<"Inside else of commit() function case"<<endl;
			cout<<"================ DEBUG INFO ENDS ================"<<endl<<endl;
		}
		this->fin_masterfile.open(this->path_to_masterfile.c_str());
		/*
		List of files open:
		fin_prev_version
		fout_current_version
		fin_filename
		fin_masterfile
		*/
		this->fin_prev_version.open(path_to_prev_version.c_str());
		this->fin_filename.open(filename.c_str());

		string prev_line;
		string file_line;
		string line_no;
		string temp;
		file_line.resize(9);

		getline(fin_prev_version, line_no);
		int line = str_to_int(line_no);

		int file_ptr = (line-1)*10;
		fin_masterfile.seekg(file_ptr);
		getline(fin_masterfile, prev_line);

		if(!fin_filename.eof())
		{
			getline(fin_filename, temp);
			for(int i=0;i<temp.length();i++)
				file_line[i]=temp[i];
			for(int i=temp.length();i<9;i++)
				file_line[i]=' ';

			
			while(file_line==prev_line)
			{
				fout_current_version<<line_no<<endl;

				if(fin_filename.eof())
					break;

				getline(fin_filename, temp);
				for(int i=0;i<temp.length();i++)
					file_line[i]=temp[i];
				for(int i=temp.length();i<9;i++)
					file_line[i]=' ';

				getline(fin_prev_version, line_no);
				int line = str_to_int(line_no);

				int file_ptr = (line-1)*10;
				fin_masterfile.seekg(file_ptr);
				getline(fin_masterfile, prev_line);

				if(SIMPLE_DEBUG)
				{
					cout<<"================ DEBUG INFO STARTS =============="<<endl;
					cout<<"Inside Equal case while loop of else in commit() function"<<endl;
					cout<<"Line of previous version file is "<<prev_line<<endl;
					cout<<"Line of current version file is "<<file_line<<endl;
					cout<<"================ DEBUG INFO ENDS ================"<<endl<<endl;
				}

			}


			while(!fin_prev_version.eof() )
			{
				
				getline(fin_prev_version, temp);
				if(!fin_prev_version.eof())
					fout_current_version<<temp<<endl;

				if(SIMPLE_DEBUG)
				{
					cout<<"================ DEBUG INFO STARTS =============="<<endl;
					cout<<"Inside append case while loop of else in commit() function"<<endl;
					cout<<"Appending line of previous version file is "<<temp<<endl;
					cout<<"================ DEBUG INFO ENDS ================"<<endl<<endl;
				}
			}
		}

		this->fin_masterfile.close();

	}
	
	
	this->fout_masterfile.close();
	this->fin_master_head.close();
	this->fout_master_head.close();	
	this->fin_filename.close();
	this->fout_current_version.close();
	this->fin_prev_version.close();

}


void svc:: initialize_all()
{
	path_to_masterfile = this->path_to_repo + "masterfile";
	path_to_master_head = this->path_to_repo + "master_head";
	path_to_version = this->path_to_repo + "version/";
	path_to_version_head = this->path_to_version + "version_head";
}


string svc::retrive()
{
	// buffer variable used to store the contents for requested version and then it will be printed
	string buffer=""; 

	// Opening the required files
	this->fin_masterfile.open(this->path_to_masterfile);
	this->fin_prev_version.open(this->path_to_version);

	// masterfile opened or not
	if(!this->fin_masterfile.is_open())
	{
		cout<<"fatal: Unable to open '"+this->path_to_masterfile+"', Please try again."<<endl;
		cout<<endl;
		exit (EXIT_FAILURE);
	}

	// version file opened or not
	if(!this->fin_prev_version.is_open())
	{
		cout<<"fatal: Unable to open '"+this->path_to_version+"', Please try again."<<endl;
		cout<<endl;
		exit (EXIT_FAILURE);
	}

	//============================ Reading loop starts ==========================
	// Reading loop: It will read each line of version file of requested version,
	// it will fetch corresponding line from masterfile and will store those lines
	// in 'buffer' (string) variable 
	while(!this->fin_prev_version.eof())
	{
		int seek_position;
		string s;
		// variable s will have a line number from version file fin_prev_version
		getline(this->fin_prev_version,s);

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
//================= functionality code ends======================




//================= main starts======================
int main(int argc, char const *argv[])
{
	ios_base::sync_with_stdio(false);

	if(argc==1)
	{
		svc obj();
	}
	else if(argc==2)
	{
		svc obj(argv[1]);   //Testing			
	}
	//============================== svc retrival starts =====================
	// if two command line arguments are present
	// first argument is program file itself, remaining two are given by user
	else if(argc==3)
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

	return 0;
}
//================= main ends======================



//============================== Test cases starts ==============================
// TEST_CASE( "isInteger is computed", "[factorial]" ) {
//     REQUIRE( isInteger("2312313123131") == true );
//     REQUIRE( isInteger("2") == true );
//     REQUIRE( isInteger("assd") == false );
//     REQUIRE( isInteger("ak2r4t3454") == false );
// }
//============================== Test cases ends ==============================

