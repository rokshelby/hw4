CC = gcc 
#CFLAGS = -g -Wall
CFLAGS = -g -Wall -DNOTIMETEST
TARGET = oss
TARGET1 = child
OBJS = oss.o
OBJS1 = child.o
.SUFFIXES: .c .o

all : $(TARGET) $(TARGET1)

$(TARGET1): $(OBJS1)
	$(CC) -o $@ $(OBJS1) -lpthread

$(TARGET): $(OBJS)
	$(CC)-o $@ $(OBJS) -lpthread -lm

.c.o:
	$(CC) $(CFLAGS) -c $<

.PHONY: clean
clean:
	rm -f *.o *.log *~ *.txt $(TARGET) $(TARGET1) $(OBJS) $(OBJ1)

