###############################################################################################################################
# Copyright 2017 Daniel Garcia Vaglio <degv364@gmail.com> Esteban Zamora Alvarado <estebanzacr.20@gmail.com>	                 
#
# Permission is hereby granted, free of charge, to any person obtaining a copy of this software and
# associated documentation files (the "Software"), to deal in the Software without restriction,
# including without limitation the rights to use, copy, modify, merge, publish, distribute,
# sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is
# furnished to do so, subject to the following conditions:
#
# The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.
#
# THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT
# NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
# NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES
# OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
# CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
###############################################################################################################################

CC= g++
TARGET= test.x

CFLAGS = -std=c++11 -Wall -c $(DEBUG) -pedantic -DTESTING
LFLAGS = -std=c++11 -Wall $(DEBUG) -pedantic -DTESTING

#Directories where to search for source and header files
SRC_DIRS= ../src/hi .
INC_DIRS= ../include/


# Include headers and libraries
INC_PARAMS := $(foreach d, $(INC_DIRS), -I$d)
LIB_PARAMS := -lgtest -pthread


#SRC= ../src/hi/hi_utils.cpp hi_tests.cpp ../src/hi/hi_state_machine.cpp periph.cpp
SRC := $(foreach dir, $(SRC_DIRS), $(wildcard $(dir)/*.cpp))
OBJ= $(notdir $(SRC:.cpp=.o))

#Directories that Make uses to search for target prerrequisites
VPATH := $(SRC_DIRS)


all: clean $(TARGET)

$(TARGET): $(OBJ)
	$(CC) $(LFLAGS) $(OBJ) -o $(TARGET) $(LIB_PARAMS) 

%.o: %.cpp
	$(CC) $(CFLAGS) $(INC_PARAMS) $< -o $@

clean:
	rm -f *~ *.o $(TARGET)
