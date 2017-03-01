CC = g++
# CC = /usr/local/bin/g++-6
DFLAGS = -DMPI
CFLAGS = -w -Wall # -g
# INCLUDE = -I$(FFTWHOME)/include
# LDFLAGS = -lm -lfftw_mpi -lfftw3 -L$(FFTWHOME)/lib
INCLUDE = -I/usr/local/include
LDFLAGS = -lm -lfftw3_mpi -lfftw3 -L/usr/local/lib

SRCS = main.cpp input.cpp utils.cpp series.cpp system.cpp fftw_utils.cpp \
       component.cpp

TARGET = a.out

# OBJS = $(SRCS:.cpp=.o)

.cpp.o:
	$(CC) $(CFLAGS) $(DFLAGS) -c $(INCLUDE) $<

# $(TARGET): $(OBJS)
# 	$(CC) $(CFLAGS) -o $@ $(OBJS) $(LDFLAGS)

$(TARGET): $(SRCS)
	$(CC) $(CFLAGS) $(INCLUDE) -o $@ $(SRCS) $(LDFLAGS)
	ctags -f .tags -R .

clean:
	rm -f *.o $(TARGET)
