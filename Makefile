CC = g++
CFLAGS = -Ofast -fPIC -Wall -std=c++0x


#if I did not separate src and include directorys could do INC_DIRS := $(shell find $(SRC_DIRS) -type d) instead
INCLUDES = -I./

#brew install opencv3  --with-ffmpeg
#$ export set PKG_CONFIG_PATH=/usr/local/Cellar/opencv/3.3.0_3/lib/pkgconfig:$PKG_CONFIG_PATH

OPENCV_CFLAGS = `pkg-config opencv --cflags`
OPENCV_LIBS = `pkg-config opencv --libs`

INCLUDES += $(OPENCV_CFLAGS) $(FFTW_CFLAGS)
LIBS = $(OPENCV_LIBS) $(FFTW_LIBS)

#find all the source files
HDRS = 
SRCS = main.cc 
PROG = green


all: $(PROG)

#------------------------------------------------------------------------------------------------------------------------------------------------
$(PROG): $(SRCS) $(HDRS)
	$(CC) $(SRCS) $(CFLAGS) $(INCLUDES)  -o  $(PROG) $(LIBS)

clean:
	rm -f $(PROG)
