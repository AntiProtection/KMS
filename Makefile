CC	= g++
CFLAGS	= -O2
TARGET	= kms
SRCS	= main.cpp libkms.cpp rpc.cpp kms.cpp crypto.cpp
OBJS	= $(SRCS:.cpp=.o)
LIBS	= -lpthread

$(TARGET): $(OBJS)
	$(CC) -o $@ $^ $(LIBDIR) $(LIBS)
	
$(OBJS): $(SRCS)
	$(CC) $(CFLAGS) $(INCDIR) -c $(SRCS)

all:
	clean $(OBJS) $(TARGET)
clean:
	-rm -f $(OBJS) $(TARGET) *.d
