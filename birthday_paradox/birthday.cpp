#include <iostream>
#include <stdlib.h>
#include <iomanip>
#include <string.h>
using namespace std;
#define precision 10

const double experiments = 5000.0;

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

double ret_result(int n, int days)   //Returns the probability of a random experiment
{
	bool visited[days];
	memset(visited, 0, sizeof visited);
	for(int i=0;i<n;i++)
	{
		int birthdate = rand()%days;
		if(visited[birthdate])
			return 1.0;
		visited[birthdate]=1;
	}
	return 0.0;	
}


int main(int argc, char const *argv[])
{
	int ch=1;

	while(ch)
	{
		cout<<"Enter choice:\n"\
		"0. Exit\n"\
		"1. Find probability\n"\
		"2. Do random experiments\n";

		cin>>ch;
		switch(ch)
		{
			case 0:
			{
				exit(0);
			}

			case 1:
			{
				int n;
				cout<<"Enter the number of people: ";
				cin>>n;  //Input number of people
				double days;
				cout<<"Enter the number of days: ";
				cin>>days;
				double probability = ret_probab(n, days);
				cout<<"Probability is: "<<setprecision(precision)<<probability<<endl<<endl;   //Sets precision upto 10 decimal points
				break;
			}

			case 2:
			{
				int n;
				cout<<"Enter the number of people for random experiments: ";
				cin>>n;

				double days;
				cout<<"Enter the number of days for random experiments: ";
				cin>>days;

				cout<<"Taking random experiments\n";

				double sum=0.0;
				for(int i=0;i<int(experiments);i++)
				{
					double result = ret_result(n, int(days));
					cout<<"Result of the experiment "<<(i+1)<<": "<<int(result)<<endl;
					sum+=result;
				}

				double experimental_mean_probability = sum/experiments;
				double expected_probability = ret_probab(n, days);
				double diff = expected_probability-experimental_mean_probability;
				double deviation;
				
				if(diff<0)
					deviation = -1.0*diff;

				cout<<endl<<"Experimental mean probability: "<<setprecision(precision)<<experimental_mean_probability<<endl;
				cout<<"Expected probability: "<<setprecision(precision)<<expected_probability<<endl;
				cout<<"Deviation from expected probability: "<<setprecision(precision)<<deviation<<endl<<endl;
				break;
			}

			default:
			{
				cout<<"Enter valid options!"<<endl<<endl;
			}

		}	
	
	}
	return 0;
}