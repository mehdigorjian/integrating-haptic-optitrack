# include=-I../../include
# libpath=-L../../lib
libs=#-lGLU -lGL -lglut -lstdc++ -lm -lglfw -lGLEW -ldl -lpthread -lgomp #-lNatNet -lHL -lHLU -lHDU -lHD
CXXFLAGS=-fopenmp
all:coordinate-transform

main: 
	g++ coordinate-transform.cpp $(CXXFLAGS) $(include) $(libpath) $(libs) -o main

.PHONY: clean
clean:
	@rm -f ./coordinate-transform
