#include "simulation.h"


simulation::simulation(int num_servers, 
 					   double arrival_mean, 
					   double process_mean, 
					   int q_part) 
{
    /*
	  resizing the vectors for the number of servers and changing uniform 
	  distribution range
	*/
    t_process.resize(num_servers);
    jobs_in_q.resize(num_servers);
    std::uniform_int_distribution<int> temp(0, num_servers - 1);
    U_dist.param(temp.param());
    
	//setting means for the exponential distributions
	a_mean = arrival_mean;
	p_mean = process_mean;
    change_arrival_dist_mean(arrival_mean);
    change_process_dist_mean(process_mean);
    
	//indicating which part of the homework problem is being worked on
    part = q_part;
    
	//initialize the simulation
    seed_prng();
    initialize();
}
/*
	Advances the simulation to the next event, takes event action, and updates
	all times to next events. 
*/
void simulation::advance(int time){
    double elapsed_time = 0.0;
    double delta_t;
    
	//iterate for given time
    while(elapsed_time < time) {
        
		//update event times
        delta_t = time_to_next_event();
        update_time_to_events(delta_t);
        
		//advance sim time
        st += delta_t;
        elapsed_time += delta_t;
        
		//check event cases
        if (t_next_arrival == 0.0) {
            total_jobs_arrived += 1;
            new_arrival();
        }
        else {
 
			//iterate over servers
            for (size_t i = 0; i < t_process.size(); i++) {
                if (t_process[i] == 0.0) {
                    total_jobs_departed += 1;
                    job_processed(i);
                    break;
                }
            }
        }
    }
}

/*
	Prints to terminal the number of jobs waiting at each server and the total
	number of jobs waiting at all servers.
*/
void simulation::print_jobs_waiting(){
    //print jobs waiting at each server
    for (size_t i = 0; i < jobs_in_q.size(); i++)
        std::cout << "Jobs waiting at server " << i << ": " << jobs_in_q[i] 
				  << std::endl; 
    //print total jobs waiting
    std::cout << "Total jobs waiting: " << return_jobs_waiting() << std::endl;
}

/*
	Sums jobs waiting over all job queues to return total number of jobs 
	waiting in the system. Note that the "1st job in queue" is the job being
	worked on and is not counted as a job waiting
*/
int simulation::return_jobs_waiting(){
    long long unsigned sum = 0;
    for(auto iter = std::begin(jobs_in_q); iter != std::end(jobs_in_q); iter++)
		if(*iter > 0) {
			sum += *iter - 1;
		}
    return sum;
}

/*
	Re-initializes the simulation environment by calling the initialize() 
	method and resetting all the counters (i.e. jobs arrived, jobs departed, 
	and simulation time elapsed) to zero.
*/
void simulation::re_initialize(){
    initialize();
    total_jobs_arrived = 0;
    total_jobs_departed = 0;
    st = 0.0;
}
/*
	A public facing function to re-seed the random number generator. Calls the 
	seed_prng() method.
*/
void simulation::re_seed(){
    seed_prng();
}

//private member functions

/*
	Initializes the simulation by generating the initial time to next events
	(arrival according to distribution, and job processing times to infinity)
	and initializing the jobs in queue to zero. 
*/
void simulation::initialize(){
    t_next_arrival = arrival_dist(rng);
    for(auto iter = std::begin(t_process); iter != std::end(t_process); iter++) 
        *iter = std::numeric_limits<double>::infinity();
    for(auto iter = jobs_in_q.begin(); iter != jobs_in_q.end(); iter++) 
        *iter = 0;
}

/*
	Seed the psuedo random number generator using the time since epoch
*/
void simulation::seed_prng(){
    long long unsigned seed = static_cast<long long unsigned>(std::chrono::high_resolution_clock::now().time_since_epoch().count());
    rng.seed(seed);
}
/*
	Change the arrival distribution mean to a new value
*/
void simulation::change_arrival_dist_mean(double mean){
    std::exponential_distribution<double> temp(mean);
    arrival_dist.param(temp.param());
}
/*
	Change the processing time distribution mean to a new value
*/
void simulation::change_process_dist_mean(double mean){
    std::exponential_distribution<double> temp(mean);
    process_dist.param(temp.param()); 
}
/*
	Finds and returns the smallest time to next event of all possible events.
*/
double simulation::time_to_next_event(){
    double smallest = t_next_arrival;
    for (auto iter = std::begin(t_process); iter != std::end(t_process); iter++)
        if (*iter < smallest) 
            smallest = *iter;
    return smallest;
}
/*
	Updates all times to next events given an elapsed time
*/
void simulation::update_time_to_events(double e_time){
    t_next_arrival -= e_time;
    for (auto iter = std::begin(t_process); iter != std::end(t_process); iter++)
        *iter -= e_time;
}
/*
	Assign a new job to one of the servers. For part 1, the job is randomly 
	assigned one of the servers. For part 2, the job is added to the server
	with the shortest queue. 
*/
void simulation::new_arrival(){
    if (part == 1) {
        
		//randomly assigning the job to one of the available servers
        size_t index = U_dist(rng);
        jobs_in_q[index] += 1;
        
		//check that that server now has a process time
        if (t_process[index] == std::numeric_limits<double>::infinity())
            t_process[index] = process_dist(rng);
        
		//create a new arrival time
        t_next_arrival = arrival_dist(rng);
    } else if (part == 2) {
        
		//assign the job to the server with the shortest queue
        size_t index = select_shortest_queue();
        jobs_in_q[index] += 1;
        
		//check that that server now has a process time
        if (t_process[index] == std::numeric_limits<double>::infinity())
            t_process[index] = process_dist(rng);
        
		//create a new arrival time
        t_next_arrival = arrival_dist(rng);
    } else {
        std::cout << "variable part has an invalid value: " << part << std::endl;
        std::cin.get();
    }
}

/*
	Remove a job from the queue for the server given by the index. A new time
	to next job completion is assigned according to the processing time 
	distribution. If there are no remaining jobs at the server, the new time 
	to next job completion is set to infinity
*/
void simulation::job_processed(int server_index){
    if (jobs_in_q[server_index] > 0) {
        jobs_in_q[server_index] -= 1;
        if (jobs_in_q[server_index] == 0) 
            t_process[server_index] = std::numeric_limits<double>::infinity();
        else
            t_process[server_index] = process_dist(rng);  
    } else {
        std::cout << "Attempting to complete job that does not exist!!" << std::endl;
        std::cin.get();
    }
}

/*
    method to find the server with the shortest queue and return it's index	
*/
size_t simulation::select_shortest_queue(){
    size_t short_queue = 0;
    for (size_t i = 1; i < jobs_in_q.size(); i++) 
        if (jobs_in_q[i] < jobs_in_q[short_queue]) 
            short_queue = i;
    return short_queue;
}
