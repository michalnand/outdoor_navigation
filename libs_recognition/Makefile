OUTPUT_NAME = libs_recognition

LIBS_PATH=$(HOME)/rysy

INC+= -I.
INC+= -I$(LIBS_PATH)/libs_common
INC+= -I$(LIBS_PATH)/libs_dataset
INC+= -I$(LIBS_PATH)/libs_nn
INC+= -I$(LIBS_PATH)/libs_experiment

SRCS+= $(shell find . -name '*.cpp')
SRCS_CUDA+= $(shell find . -name '*.cu')

# CUDA_PATH = /usr/local/cuda-9.1/bin/

CC = g++
CU = nvcc
LD = g++
OBJCOPY = objcopy
OBJDUMP = objdump
SIZE = size
ARCHIVE = ar

# DEGUBFLAGS = -g -pg

CFLAGS = $(DEGUBFLAGS) -O2 -Os -fPIC -s -Wall -Wextra -pedantic -std=c++11 $(INC)

OBJS = $(SRCS:.cpp=.o)

all: $(OBJS) $(OBJS_CUDA)
	$(ARCHIVE) rc $(OUTPUT_NAME).a $(OBJS)
	$(CC) -shared -fPIC -o $(OUTPUT_NAME).so $(OBJS)

%.o: %.cpp
	$(CC) -c $(CFLAGS) $< -o $@

clean:
	rm $(OBJS) -rf
	rm $(OBJS_CUDA) -rf
	rm $(OUTPUT_NAME).* -rf
