#include <iostream>
#include <iomanip>
#include <vector>
#include <cmath>
#include <random>
#include <chrono>
#include <limits>

typedef std::vector<double> d_array;
typedef std::vector<long long int> li_array;

class simulation {
    public:
        //public member functions
        //constructors
        simulation(int num_servers, double arrival_mean, double process_mean, int q_part);
		simulation(const simulation &cp): simulation(cp.t_process.size(), cp.a_mean, cp.p_mean, cp.part) {};
        //run simulation
        void advance(int time);
        //get results
        void print_jobs_waiting();
        int return_jobs_waiting();
        //functions to reset simulation
        void re_initialize();
        void re_seed();

    private:
        //private data
            //process times
            double t_next_arrival;
            d_array t_process;
            //counters
            li_array jobs_in_q;
            long long int total_jobs_arrived = 0;
            long long int total_jobs_departed = 0;
            //sim time
            double st = 0.0;
            //part indicator
            int part;
			//means (only needed to facilitate copying)
			double a_mean;
			double p_mean;
            //random number generator   (means to be changed in constructor)
            std::mt19937_64 rng;
            std::exponential_distribution<double> arrival_dist{ 1.0 };
            std::exponential_distribution<double> process_dist{1.0};
            std::uniform_int_distribution<int> U_dist{0, 1};

        //private member functions
            //initialization functions
            void initialize();
            void seed_prng();
            //setting simulation distribution means
            void change_arrival_dist_mean(double mean);        
            void change_process_dist_mean(double mean);
            //calculating time to next events
            double time_to_next_event();
            void update_time_to_events(double e_time);
            //functions for events
            void new_arrival();
            void job_processed(int server_index);
            //function to facilitate FCFS
            size_t select_shortest_queue();
};
