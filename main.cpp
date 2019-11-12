#include <iostream>
#include <vector>
#include "simulation.h"
#include "statistic_func.h"
#include "omp.h"

int main(){
	//multi-threading parameters
	const int num_threads = 4;
    //repeats
    const int num_reps_per_thread = 100;
	const int total_reps = num_threads * num_reps_per_thread;
	//simulation parameters
	const int num_servers = 2;
    const double a_mean = 6.0;
    const double p_mean = 10.0;
	const int sim_time = static_cast<const int>(a_mean) * 5000000;
    //rate parameter is 1 / mean for exponential distribution
	const double a_rate = 1.0 / a_mean;
	const double p_rate = 1.0 / p_mean;
	//results
	std::vector<int> jobs_queued(total_reps);
	//simulation objects for part 1 and part 2 respectively
	simulation part_1(num_servers, a_rate, p_rate, 1);
	simulation part_2(num_servers, a_rate, p_rate, 2);


	//simulation for part 1
	omp_set_num_threads(num_threads);
	omp_set_dynamic(0);
	#pragma omp parallel for firstprivate(part_1) //each thread gets a copy of the simulation object
	for (int i = 0; i < total_reps; i++) {
		part_1.advance(sim_time);
		jobs_queued[i] = part_1.return_jobs_waiting();
	}
	std::cout << "The mean number of jobs waiting in part 1 is: " << mean_of_ints(jobs_queued) << std::endl;

	//simulation for part 2
	omp_set_num_threads(num_threads);
	omp_set_dynamic(0);
	#pragma omp parallel for firstprivate(part_2) //each thread gets a copy of the simulation object
	for (int i = 0; i < total_reps; i++) {
		part_2.advance(sim_time);
		jobs_queued[i] = part_2.return_jobs_waiting();
	}
	std::cout << "The mean number of jobs waiting in part 2 is: " << mean_of_ints(jobs_queued) << std::endl;
	
}
