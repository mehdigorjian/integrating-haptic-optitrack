include=-I../../include
libpath=-L../../lib
libs=-lNatNet -lGLU -lGL -lglut -lstdc++ -lm -lglfw -lGLEW -ldl -lpthread -lgomp #-lHL -lHLU -lHDU -lHD
CXXFLAGS=-fopenmp
all:multiRigidTracking

multiRigidTracking: 
	g++ multiRigidTracking.cpp $(CXXFLAGS) $(include) $(libpath) $(libs) -o multiRigidTracking

.PHONY: clean
clean:
	@rm -f ./multiRigidTracking
