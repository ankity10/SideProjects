#include <iostream>
#include <iomanip>
using namespace std;

double ret_probab(int n, double total_days)   //Returns the probability of minimum 2 of n people having birthday on same day.
{
	if(n>int(total_days))  //Pigeon hole principle
		return 1.0;

	double ans=1;
	for(double i=0;int(i)<n;i++)
	{
		double possible_days = total_days-i;
		double ratio = possible_days/total_days;
		ans*=ratio;
	}
	return (1.0-ans);
}

int main(int argc, char const *argv[])
{
	int n;
	cin>>n;  //Input number of people
	double probability = (ret_probab(n, 366) + 3.0*ret_probab(n, 365))/4.0; //One leap year in every 4 years

	cout<<setprecision(10)<<probability;   //Sets precision upto 10 decimal points
	return 0;
}