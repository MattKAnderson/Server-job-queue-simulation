#include "simulation.h"


simulation::simulation(int num_servers, double arrival_mean, double process_mean, int q_part){
    //resizing the vectors for the number of servers and changing uniform distribution range
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
void simulation::print_jobs_waiting(){
    //print jobs waiting at each server
    for (size_t i = 0; i < jobs_in_q.size(); i++)
        std::cout << "Jobs waiting at server " << i << ": " << jobs_in_q[i] << std::endl; 
    //print total jobs waiting
    std::cout << "Total jobs waiting: " << return_jobs_waiting() << std::endl;
}
int simulation::return_jobs_waiting(){
    long long unsigned sum = 0;
    for(auto iter = std::begin(jobs_in_q); iter != std::end(jobs_in_q); iter++)
        sum += *iter;
    return sum;
}
void simulation::re_initialize(){
    //normal initialization
    initialize();
    //additionally resetting counters, and times to zero
    total_jobs_arrived = 0;
    total_jobs_departed = 0;
    st = 0.0;
}
void simulation::re_seed(){
    seed_prng();
}

//private member functions (support for public functions)
void simulation::initialize(){
    //setting arrival times
    t_next_arrival = arrival_dist(rng);
    for(auto iter = std::begin(t_process); iter != std::end(t_process); iter++) 
        *iter = std::numeric_limits<double>::infinity();
    //setting initial jobs waiting to zero
    for(auto iter = jobs_in_q.begin(); iter != jobs_in_q.end(); iter++) 
        *iter = 0;
}
void simulation::seed_prng(){
    long long unsigned seed = static_cast<long long unsigned>(std::chrono::high_resolution_clock::now().time_since_epoch().count());
    rng.seed(seed);
}
void simulation::change_arrival_dist_mean(double mean){
    std::exponential_distribution<double> temp(mean);
    arrival_dist.param(temp.param());
}
void simulation::change_process_dist_mean(double mean){
    std::exponential_distribution<double> temp(mean);
    process_dist.param(temp.param()); 
}
double simulation::time_to_next_event(){
    double smallest = t_next_arrival;
    for (auto iter = std::begin(t_process); iter != std::end(t_process); iter++)
        if (*iter < smallest) 
            smallest = *iter;
    return smallest;
}
void simulation::update_time_to_events(double e_time){
    t_next_arrival -= e_time;
    for (auto iter = std::begin(t_process); iter != std::end(t_process); iter++)
        *iter -= e_time;
}
void simulation::new_arrival(){
    //handle case when there is a new arrival
    //is the simulation for problem part 1 or 2
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
void simulation::job_processed(int server_index){
    //handle case when there is a job processed from server indexed by i
    if (jobs_in_q[server_index] > 0) {
        //remove 1 job from the queue
        jobs_in_q[server_index] -= 1;
        //determine next completion time
        if (jobs_in_q[server_index] == 0) 
            t_process[server_index] = std::numeric_limits<double>::infinity();
        else
            t_process[server_index] = process_dist(rng);  
    } else {
        std::cout << "Attempting to complete job that does not exist!!" << std::endl;
        std::cin.get();
    }
}
size_t simulation::select_shortest_queue(){
    //method to find the server with the shortest queue and return it's index
    size_t short_queue = 0;
    for (size_t i = 1; i < jobs_in_q.size(); i++) 
        if (jobs_in_q[i] < jobs_in_q[short_queue]) 
            short_queue = i;
    return short_queue;
}
