CFLAGS = -std=c++11 -Wall -c $(DEBUG) -pedantic -DTESTING
LFLAGS = -std=c++11 -Wall $(DEBUG) -pedantic -DTESTING
CC= g++
TARGET= test.x
SRC= hi_utils.hpp hi_tests.cpp hi_state_machine.cpp

all: clean $(TARGET)

$(TARGET): $(SRC)
	$(CC)  $(CFLAGS) hi_state_machine.cpp -o hi_state_machine.o
	$(CC)  $(CFLAGS) hi_utils.hpp -o hi_utils.o 
	$(CC)  $(LFLAGS) $(SRC) -o $(TARGET) -lgtest

clean:
	rm -f *~ *.o $(TARGET)
