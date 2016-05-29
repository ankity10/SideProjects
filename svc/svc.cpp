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

using namespace std;

class svc
{
	private:
		int version;
		string filename;
		char lines[20][10];
		string path_to_repo;
		string path_to_version;
		string path_to_masterfile;
		string path_to_master_head;
		string path_to_version_head;
		string path_to_current_version;
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


					this->fin_masterfile.open(path_to_masterfile);
					this->fin_master_head.open(path_to_master_head);
					this->fout_masterfile.open(path_to_masterfile);
					this->fout_master_head.open(path_to_master_head);
					this->fin_version_head.open(path_to_version_head);
					this->fout_version_head.open(path_to_version_head);
				}
				else //repo exist - first commit is already done
				{

				}
			}
		}
	}
}




int main(int argc, char const *argv[])
{
	ios_base::sync_with_stdio(false);
	svc obj("abc");	
	return 0;
}