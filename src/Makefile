CXX = g++-9
INC_DIR = ../include
vpath %.h $(INC_DIR) 
CXXFLAGS = -O3 -fopenmp -I$(INC_DIR)
DEPS = simulation.h statistic_func.h
OBJS = main.o simulation.o statistic_func.o
SRC = ../src

simulation : $(OBJS) $(DEPS)
	$(CXX) $(CXXFLAGS) -o $@ $(OBJS)

%.o : %.cpp $(DEPS)
	$(CXX) $(CXXFLAGS) -c $< -o $@ 

.PHONY: clean

clean: 
	rm -f *.o    
