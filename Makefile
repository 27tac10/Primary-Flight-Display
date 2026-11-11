#intermediate Makefile example
#SRCS, PROG, and pet.o are the only place to update. 

SRCS=main.cpp Aircraft.cpp TextPFD.cpp
#OBJS=${SRCS:S/cpp$/o/g}
OBJS=${SRCS:.cpp=.o}
PROG=main

#create a compiler flags variable
CXXFLAGS=-Wall -std=c++23

.PHONY: all clean pack unpack

all: ${PROG}

# @ means the target
${PROG}: ${OBJS}
		${CXX} ${CXXFLAGS} -o $@ ${OBJS}

main.o: main.cpp Aircraft.h TextPFD.h FlightData.h
Aircraft.o: Aircraft.cpp Aircraft.h FlightData.h
TextPFD.o: TextPFD.cpp TextPFD.h Aircraft.h FlightData.h

clean: 
		@rm -f ${PROG} ${OBJS}

pack:
		@zip -j files.zip *.cpp *.h Makefile

unpack:
		@unzip -j files.zip