CC = gcc
DFLAGS =
CFLAGS = -Wall -std=c11 -g $(DFLAGS) #-fsanitize=address
INCLUDES = -Iinclude
LIBPATHS =
LIBS =
LFLAGS =

#export LD_LIBRARY_PATH := /usr/local/lib64:/usr/lib/gcc4/x64/release/
#export PKG_CONFIG_PATH := /usr/local/lib64/pkgconfig

SRCS = $(wildcard src/*.c)

OBJS = $(SRCS:.cpp=.o)

MAIN = arachnid-test

.PHONY: clean

all:    $(MAIN)
		@echo  BlueArachnid built successfully.

$(MAIN): $(OBJS)
		$(CC) $(CFLAGS) $(INCLUDES) $(LIBPATHS) -o $(MAIN) $(OBJS) $(LFLAGS) $(LIBS)
.cpp.o:
		$(CC) $(CFLAGS) $(INCLUDES) -c $<  -o $@

clean:
		$(RM) *.o *~ $(MAIN)
		find src/ -name "*.o" -type f -delete

run:    ${MAIN}
	./arachnid-test
