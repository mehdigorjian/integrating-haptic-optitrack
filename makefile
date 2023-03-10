INCLUDE=-I../../include -I/usr/include/QH #-I$(IMGUI_DIR)/include -I$(IMGUI_DIR)/backends
LIBS=-L../../lib
LDFLAGS=-lNatNet -ldl -lpthread -lgomp -lQHGLUTWrapper -lQH -lHL -lHLU -lHDU -lHD -lGL -lGLU -lglut -lrt -lncurses -lz -lstdc++ -lm
CXXFLAGS=-fopenmp #-std=c++11 -W -g -DNDEBUG -Dlinux -g -Wall -Wformat
TARGET=main
IMGUI_DIR=imgui

SRCS=main.cpp
#SRCS+=$(IMGUI_DIR)/include/imgui.cpp $(IMGUI_DIR)/include/imgui_demo.cpp $(IMGUI_DIR)/include/imgui_draw.cpp $(IMGUI_DIR)/include/imgui_tables.cpp $(IMGUI_DIR)/include/imgui_widgets.cpp
#SRCS+=$(IMGUI_DIR)/backends/imgui_impl_glut.cpp $(IMGUI_DIR)/backends/imgui_impl_opengl2.cpp

.PHONY: all
all: $(TARGET)

$(TARGET): $(SRCS)
	g++ $(CXXFLAGS) $(INCLUDE) $(LIBS) -o $@ $(SRCS) $(LDFLAGS)

.PHONY: clean
clean:
	@rm -f ./main
