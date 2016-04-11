PROJECT = tdot

CC = g++
CFLAGS = -std=c++11 `pkg-config --cflags --libs opencv`
# CFLAGS for all configuration. cannot be overwritten by environment
override CFLAGS += -Wall -O3

RM = rm -f

CUDA_VERS = 6.5

C_INCL = ./src /usr/include/c++/4.8/ /usr/local/lib/ #/usr/local/include #./include /opt/opencv3/include
INCLUDES = $(addprefix -I, $(C_INCL))

C_LIB_DIRS = /opt/opencv3/lib				\
						 /usr/local/cuda-$(CUDA_VERS)/lib	 \
						 /opt/cuda-$(CUDA_VERS)/lib
LIB_DIRS = $(addprefix -L, $(C_LIB_DIRS))

OPENCV_LIBS = core cudaobjdetect cudaoptflow cudawarping cudaimgproc cudafilters cudaarithm\
					 highgui imgproc objdetect imgcodecs videoio

#core cudaoptflow highgui imgproc objdetect imgcodecs videoio
C_LIB = $(addprefix opencv_, $(OPENCV_LIBS)) \
				pthread v4l2

LIBS = $(addprefix -l, $(C_LIB))

CPP_SRC  = main.cpp \
				FaceDetector.cpp \
				EdgeDetector.cpp \
				OpticalFlow.cpp \
				faces.cpp

CPP_H    = $(wildcard $(C_INCL)/*.h)

CPP_OBJS = $(CPP_SRC:%.cpp=%.o)
DEPS = $(CPP_OBJS:%.o=%.d) \
			 $(CUDA_OBJS:%.o=%.d)

all: $(PROJECT)

%.o: %.cpp $(CPP_H)
	@echo 'Building file: $<'
	$(CC) $(CFLAGS) $(INCLUDES) -c -o "$@" "$<"
	@echo 'Finished building $<'
	@echo ' '

$(PROJECT): $(CPP_OBJS)
	@echo 'Linking file: $@'
	$(CC) -o $@ $(CFLAGS) $(INCLUDES) $(LIB_DIRS) $(CPP_OBJS) $(LIBS)
	@echo 'Finished building $@'
	@echo ' '

schroot:
	schroot -c exp -- make PREFIX="" $(PROJECT)

version:
	$(CC) --version

clean:
	$(RM) $(CPP_OBJS) $(DEPS) $(PROJECT)
