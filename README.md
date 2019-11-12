# Server-job-queue-simulation
### Brief intro
A simulation of the number of jobs queued in an N-server system. Used to compare two methods of job queuing: a single shared queue and a private queue for each server in the system. Written in C++ using vim in WSL Fall 2019 as part of an Assignment for Computer Science 4E03 at McMaster university.

### What this project demonstrates
This small project is a brief demonstration of object-oriented programming, familiarity with the standard library, use of parallelization, and applying theory into practice. 

### Theory and Design Explanation
##### Design comments
I created a class for the simulation. The motivation behind this is that it simplifies implementing parallelization and comparing different parameters/scenarios. By using different instances of the class, two different simulation scenarios can be easily compared. Likewise, when parallelizing my code, each thread only needed it's own instance of the class.

##### Motivations
Accurately predicting hardware needs to meet desired performance saves money. Response time is a performance metric often used as a performance criteria for server systems. Response time is the sum of time spent waiting in queue and time being processed. While it can be easy to estimate mean job processing time, time spent waiting in queue less so. To determine an expected number of jobs in queue, Queuing theory is used. The simulation performed is a numerical application of queuing theory to calculate the expected number of jobs in queue for a system.

##### Queuing theory for modelling computer systems/networks
In Queuing theory, a system, whether a single computer, a server, or a network of servers, each computational unit is viewed as a node in a network. Each node processes jobs with a mean time following a statistical distribution, and it recieves jobs according to a statistical rate (typically exponential distribution and Poisson process). A node may or may not have a queue in which jobs can wait while the node is busy. Furthermore, nodes in parallel may or may not share a single queue. For simple systems, analytic solutions can be found. However, for more complicated solutions simulation is required.

##### Discrete event simulation
Computer systems are simulated using next-event time progression. This means that time in the simulation is advanced by the amount to the next event. In this simulation, events are "job arrived to the system", and "job is processed at node i". Times to the next events are initially computed by taking a random values from the distributions defining the time between those events. For example, in this simulation, the time between arrivals to the system is exponentially distributed (the arrivals follow a poisson process). The time that passes between two steps in the simulation is the smallest time to next event (ttne), and this value gets subtracted from ttne's for all events. For the event that happens, a new ttne is randomly picked from the distribution defining the time between events, if there is no condition preventing the event. For scenarios such as "there is no job at node i to be processed", the event "job is processed at node i" would be given a ttne of infinite until an event where a job arrives to that node. 



