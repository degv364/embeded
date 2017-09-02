CFLAGS = -std=c++11 -Wall -c $(DEBUG) -pedantic -DTESTING
LFLAGS = -std=c++11 -Wall $(DEBUG) -pedantic -DTESTING
CC= g++
TARGET= test.x
SRC= ../include/hi/hi_utils.hpp hi_tests.cpp ../src/hi/hi_state_machine.cpp

all: clean $(TARGET)

$(TARGET): $(SRC)
	$(CC)  $(CFLAGS) ../src/hi/hi_state_machine.cpp -o hi_state_machine.o
	$(CC)  $(CFLAGS) ../include/hi/hi_utils.hpp -o hi_utils.o 
	$(CC)  $(LFLAGS) $(SRC) -o $(TARGET) -lgtest

clean:
	rm -f *~ *.o $(TARGET)
