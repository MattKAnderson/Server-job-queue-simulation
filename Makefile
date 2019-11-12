CXX = g++-9
CXXFLAGS = -O3 -fopenmp
DEPS = simulation.h statistic_func.h
OBJS = main.o simulation.o statistic_func.o
NAME = server_simulation

simulation : $(OBJS) $(DEPS)
	$(CXX) $(CXXFLAGS) -o $@ $(OBJS)

%.o : %.cpp $(DEPS)
	$(CXX) $(CXXFLAGS) -c $< -o $@ 

.PHONY: clean

clean: 
	rm -f *.o    
