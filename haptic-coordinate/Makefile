CC=gcc
CFLAGS=
LIBS = -lHDU -lHD -lrt -lncurses -lstdc++ -lm

TARGET=coordinate
HDRS=
SRCS=coordinate.cpp conio.c
OBJS=$(SRCS:.c=.o)

.PHONY: all
all: $(TARGET)

$(TARGET): $(SRCS)
	$(CC) $(CFLAGS) -o $@ $(SRCS) $(LIBS)

.PHONY: clean
clean:
	-rm -f ./$(TARGET)
