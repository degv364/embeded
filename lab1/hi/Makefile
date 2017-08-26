CFLAGS = -std=c++11 -Wall -c $(DEBUG) -pedantic
LFLAGS = -std=c++11 -Wall $(DEBUG) -pedantic 
CC= g++
TARGET= test.x
SRC= hi_utils.hpp hi_test_utils.cpp

all: clean $(TARGET)

$(TARGET): $(SRC)
	$(CC) -DTESTING $(CFLAGS) hi_utils.hpp -o hi_utils.o 
	$(CC)  -DTESTING $(LFLAGS) $(SRC) -o $(TARGET) -lgtest

clean:
	rm -f *~ *.o $(TARGET)
