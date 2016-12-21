CC=g++
OPTS=-g

all: predictorg predictor21264

predictorg : main.o traceread.o predictor.o
	$(CC) $(OPTS) -lm -o predictor main.o traceread.o predictor.o
predictor21264 : main.o traceread.o predictor21264.o
	$(CC) $(OPTS) -lm -o predictor21264 main.o traceread.o predictor21264.o

traceread.o: traceread.h traceread.cpp
	$(CC) $(OPTS) -c traceread.cpp

main.o : main.cpp
	$(CC) $(OPTS) -c main.cpp

predictor.o: predictor.h predictor.cpp
	$(CC) $(OPTS) -c predictor.cpp
	
predictor21264.o: predictor21264.h predictor21264.cpp
	$(CC) $(OPTS) -c predictor21264.cpp



clean:
	rm -f *.o predictorg predictor21264;




