TGT = main
CC = g++
SRC = $(foreach dir, ./, $(wildcard $(dir)/*.cpp))

$(TGT):$(SRC:.cpp=.o)
	$(CC) -g $^ -o $@

%.o:%.cpp
	$(CC) -g -c $< -o $@

clean:
	del -rf *.o *.exe
.PHONY:clean