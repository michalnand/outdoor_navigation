ELF = bin/image_recognition_test.elf

LIBS_PATH=$(HOME)/rysy
LIBS_RECOGNITION_PATH=../libs_recognition/

INC+= -I.
INC+= -I$(LIBS_PATH)/libs_common
INC+= -I$(LIBS_PATH)/libs_dataset
INC+= -I$(LIBS_PATH)/libs_nn
INC+= -I$(LIBS_PATH)/libs_experiment
INC+= -I$(LIBS_RECOGNITION_PATH)

LIBS+= $(LIBS_PATH)/libs_dataset/libs_dataset.a
LIBS+= $(LIBS_RECOGNITION_PATH)/libs_recognition.a
LIBS+= $(LIBS_PATH)/libs_experiment/libs_experiment.a
LIBS+= $(LIBS_PATH)/libs_nn/libs_nn.a
LIBS+= $(LIBS_PATH)/libs_common/libs_common.a


CC = g++
CU = nvcc
LD = g++
OBJCOPY = objcopy
OBJDUMP = objdump
SIZE = size

# DEGUBFLAGS = -g -pg

CFLAGS = $(DEGUBFLAGS) -O2 -Os -s -Wall -Wextra -pedantic -std=c++11 $(INC)
CUDAFLAGS = -O3 -D_FORCE_INLINES -ccbin=$(CXX) -Xcompiler -fPIC -std=c++11 $(INC)


OPENCV = `pkg-config opencv --cflags --libs`

LDFLAGS = $(DEGUBFLAGS) -lstdc++fs -lm -lpthread -pthread -std=c++11 -lglut -lGL -lGLU -lX11 -ljsoncpp -L/usr/local/cuda/lib64 -lcuda -lcudart $(OPENCV)


SRCS+= $(shell find -name '*.cpp')
SRCS_CUDA+= $(shell find -name '*.cu')


OBJS = $(SRCS:.cpp=.o)
OBJS_CUDA = $(SRCS_CUDA:.cu=.oc)

all: $(ELF)
	$(SIZE) $(ELF)

$(ELF): $(LIBS) $(OBJS) $(OBJS_CUDA)
	$(LD) -o $@ $(OBJS) $(OBJS_CUDA) $(LIBS) $(LDFLAGS)

%.o: %.cpp
	$(CC) -c $(CFLAGS) $< -o $@

%.oc: %.cu
	$(CU) -c $(CUDAFLAGS) $< -o $@

clean:
	rm $(OBJS) -rf
	rm $(OBJS_CUDA) -rf
	rm $(ELF) -rf
