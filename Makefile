CC = g++
CFLAGS = -Wall -Werror -Wextra -std=c++17 -g -I./headers
# -I/opt/homebrew/include

EXEC = s21_containers.a
EXEC_T = unit_tests
PATH_TO_TESTS = tests/

SRC = $(wildcard *.cc)
OBJ = $(patsubst %.cc, %.o, $(SRC))
SRC_T = $(wildcard $(PATH_TO_TESTS)*.cc)
OBJ_T = $(patsubst %.cc, %.o, $(SRC_T))

OS := $(shell uname -s)

all: $(EXEC)

$(EXEC): $(OBJ)
	ar rc $@ $(OBJ)
	ranlib $@

test: $(OBJ_T) $(EXEC)
ifeq ($(OS), Darwin)
	$(CC) $(CFLAGS) $(OBJ_T) $(EXEC) -o $(PATH_TO_TESTS)$(EXEC_T) -L/opt/homebrew/lib -lgtest -lgtest_main -lpthread
else
	$(CC) $(CFLAGS) $(OBJ_T) $(EXEC) -o $(PATH_TO_TESTS)$(EXEC_T) -lgtest -lgtest_main -lpthread
endif
	$(PATH_TO_TESTS)./$(EXEC_T)

%.o: %.cc
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -rf *.o $(EXEC) ./tests/*.o $(PATH_TO_TESTS)./$(EXEC_T)