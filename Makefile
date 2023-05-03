BIN := ./bin
INC := ./include
SRC := ./src

CXX := g++ -std=c++17
CFLAGS := -Wall -g

ROOTCFLAGS  := $(shell root-config --cflags)
ROOTLDFLAGS := $(shell root-config --ldflags)
ROOTLIBS    := $(shell root-config --libs) -lEG

all: ${BIN}/integrate-phi-q2nuzhpt2 ${BIN}/bgreduction-pt2-q2nuzh ${BIN}/calculate-meanpt2 ${BIN}/calculate-broadening \
	 ${BIN}/print-plot-meanpt2-q2nuzh ${BIN}/print-plot-meanpt2-q2-nu-zh-a13 ${BIN}/print-plot-broadening-q2nuzh \
	 ${BIN}/print-plot-broadening-q2-nu-zh-a13 ${BIN}/print-plot-broadening-q2nuzha13 ${BIN}/print-plot-acccorr-uncorr-comparison \
	 ${BIN}/print-plot-acccorr-accrccorr-comparison

${BIN}/integrate-phi-q2nuzhpt2: ${SRC}/integrate-phi-q2nuzhpt2.cpp
	${CXX} -I${INC} ${ROOTCFLAGS} ${SRC}/integrate-phi-q2nuzhpt2.cpp -o ${BIN}/integrate-phi-q2nuzhpt2 ${ROOTLIBS}

${BIN}/bgreduction-pt2-q2nuzh: ${SRC}/bgreduction-pt2-q2nuzh.cpp
	${CXX} -I${INC} ${ROOTCFLAGS} ${SRC}/bgreduction-pt2-q2nuzh.cpp -o ${BIN}/bgreduction-pt2-q2nuzh ${ROOTLIBS}

${BIN}/calculate-meanpt2: ${SRC}/calculate-meanpt2.cpp
	${CXX} -I${INC} ${ROOTCFLAGS} ${SRC}/calculate-meanpt2.cpp -o ${BIN}/calculate-meanpt2 ${ROOTLIBS}

${BIN}/calculate-broadening: ${SRC}/calculate-broadening.cpp
	${CXX} -I${INC} ${ROOTCFLAGS} ${SRC}/calculate-broadening.cpp -o ${BIN}/calculate-broadening ${ROOTLIBS}

${BIN}/print-plot-meanpt2-q2nuzh: ${SRC}/print-plot-meanpt2-q2nuzh.cpp
	${CXX} -I${INC} ${ROOTCFLAGS} ${SRC}/print-plot-meanpt2-q2nuzh.cpp -o ${BIN}/print-plot-meanpt2-q2nuzh ${ROOTLIBS}

${BIN}/print-plot-meanpt2-q2-nu-zh-a13: ${SRC}/print-plot-meanpt2-q2-nu-zh-a13.cpp
	${CXX} -I${INC} ${ROOTCFLAGS} ${SRC}/print-plot-meanpt2-q2-nu-zh-a13.cpp -o ${BIN}/print-plot-meanpt2-q2-nu-zh-a13 ${ROOTLIBS}

${BIN}/print-plot-broadening-q2nuzh: ${SRC}/print-plot-broadening-q2nuzh.cpp
	${CXX} -I${INC} ${ROOTCFLAGS} ${SRC}/print-plot-broadening-q2nuzh.cpp -o ${BIN}/print-plot-broadening-q2nuzh ${ROOTLIBS}

${BIN}/print-plot-broadening-q2nuzha13: ${SRC}/print-plot-broadening-q2nuzha13.cpp
	${CXX} -I${INC} ${ROOTCFLAGS} ${SRC}/print-plot-broadening-q2nuzha13.cpp -o ${BIN}/print-plot-broadening-q2nuzha13 ${ROOTLIBS}

${BIN}/print-plot-broadening-q2-nu-zh-a13: ${SRC}/print-plot-broadening-q2-nu-zh-a13.cpp
	${CXX} -I${INC} ${ROOTCFLAGS} ${SRC}/print-plot-broadening-q2-nu-zh-a13.cpp -o ${BIN}/print-plot-broadening-q2-nu-zh-a13 ${ROOTLIBS}

${BIN}/print-plot-acccorr-uncorr-comparison: ${SRC}/print-plot-acccorr-uncorr-comparison.cpp
	${CXX} -I${INC} ${ROOTCFLAGS} ${SRC}/print-plot-acccorr-uncorr-comparison.cpp -o ${BIN}/print-plot-acccorr-uncorr-comparison ${ROOTLIBS}

${BIN}/print-plot-acccorr-accrccorr-comparison: ${SRC}/print-plot-acccorr-accrccorr-comparison.cpp
	${CXX} -I${INC} ${ROOTCFLAGS} ${SRC}/print-plot-acccorr-accrccorr-comparison.cpp -o ${BIN}/print-plot-acccorr-accrccorr-comparison ${ROOTLIBS}

clean:
	rm ${BIN}/*
