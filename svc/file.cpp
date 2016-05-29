#include <iostream>
#include <fstream>
using namespace std;
int main(int argc, char const *argv[])
{
	ofstream fout;
	fout.open("abc.txt");
	fout<<1;
	fout.close();
	ifstream fin;
	fin.open("abc.txt");
	string s;
	getline(fin,s);
	cout<<s[0]-'0'+1;	
	return 0;
}