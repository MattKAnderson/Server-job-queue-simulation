#include "statistic_func.h"

/*
	Function to calculate and return the mean of a collection of integer 
	values. 
*/
double mean_of_ints(std::vector<int>& ints){
	long long int sum = 0;
	for (int i : ints) 
		sum += i;
	return (static_cast<double>(sum) / static_cast<double>(ints.size()));
}
