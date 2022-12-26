BIN := ./bin
INC := ./include
SRC := ./src

CXX := g++ -std=c++17
CFLAGS := -Wall -g

ROOTCFLAGS  := $(shell root-config --cflags)
ROOTLDFLAGS := $(shell root-config --ldflags)
ROOTLIBS    := $(shell root-config --libs) -lEG

all: ${BIN}/integrate-phi-q2nuzhpt2 ${BIN}/bgreduction-pt2-q2nuzh ${BIN}/calculate-meanpt2 ${BIN}/calculate-broadening ${BIN}/print-plots-meanpt2

${BIN}/integrate-phi-q2nuzhpt2: ${SRC}/integrate-phi-q2nuzhpt2.cpp
	${CXX} -I${INC} ${ROOTCFLAGS} ${SRC}/integrate-phi-q2nuzhpt2.cpp -o ${BIN}/integrate-phi-q2nuzhpt2 ${ROOTLIBS}

${BIN}/bgreduction-pt2-q2nuzh: ${SRC}/bgreduction-pt2-q2nuzh.cpp
	${CXX} -I${INC} ${ROOTCFLAGS} ${SRC}/bgreduction-pt2-q2nuzh.cpp -o ${BIN}/bgreduction-pt2-q2nuzh ${ROOTLIBS}

${BIN}/calculate-meanpt2: ${SRC}/calculate-meanpt2.cpp
	${CXX} -I${INC} ${ROOTCFLAGS} ${SRC}/calculate-meanpt2.cpp -o ${BIN}/calculate-meanpt2 ${ROOTLIBS}

${BIN}/calculate-broadening: ${SRC}/calculate-broadening.cpp
	${CXX} -I${INC} ${ROOTCFLAGS} ${SRC}/calculate-broadening.cpp -o ${BIN}/calculate-broadening ${ROOTLIBS}

${BIN}/print-plots-meanpt2: ${SRC}/print-plots-meanpt2.cpp
	${CXX} -I${INC} ${ROOTCFLAGS} ${SRC}/print-plots-meanpt2.cpp -o ${BIN}/print-plots-meanpt2 ${ROOTLIBS}

clean:
	rm ${BIN}/*
