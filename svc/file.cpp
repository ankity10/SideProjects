#include <iostream>
#include <fstream>
using namespace std;



int main(int argc, char const *argv[])
{
	ofstream fout;
	fout.open("write.txt");
	ifstream fin;
	fin.open("abc.txt");
	string s;

	fin.seekg(ios::beg);
	getline(fin, s);
	cout<<s.length();
	cout<<s[s.length()-1];
	// while(!fin.eof())
	// {
	// 	getline(fin, s);
	// 	fout<<s<<endl;
	// }
	
	fout.close();	
	fin.close();

	// fin.seekg(0,ios::end);
	// int x = fin.tellg();
	// x-=9;
	// fin.seekg(x);

	// string s;
	// getline(fin, s);
	// cout<<s;	

	return 0;
}