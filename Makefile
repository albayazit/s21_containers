CC = g++
CFLAGS = -Wall -Werror -Wextra -std=c++17 -g -I/opt/homebrew/include -I./Set -I./Tree -I./Map -I./Multiset

EXEC_T = unit_tests
PATH_TO_TESTS = Tests/

# Исходные файлы тестов
SRC_T = $(wildcard $(PATH_TO_TESTS)*.cc)
OBJ_T = $(patsubst %.cc, %.o, $(SRC_T))

OS := $(shell uname -s)

all: test

# Компиляция тестов
test: $(OBJ_T)
ifeq ($(OS), Darwin)
	$(CC) $(CFLAGS) $(OBJ_T) -o $(PATH_TO_TESTS)$(EXEC_T) -L/opt/homebrew/lib -lgtest -lgtest_main -lpthread
else
	$(CC) $(CFLAGS) $(OBJ_T) -o $(PATH_TO_TESTS)$(EXEC_T) -lgtest -lgtest_main -lpthread
endif
	$(PATH_TO_TESTS)$(EXEC_T)

%.o: %.cc
	$(CC) $(CFLAGS) -c $< -o $@

# Очистка
clean:
	rm -rf $(OBJ_T) $(PATH_TO_TESTS)$(EXEC_T)

# Проверка утечек памяти (только для macOS)
leaks_test: $(OBJ_T)
ifeq ($(OS), Darwin)
	$(CC) $(CFLAGS) $(OBJ_T) -o $(PATH_TO_TESTS)$(EXEC_T) -L/opt/homebrew/lib -lgtest -lgtest_main -lpthread
	leaks --atExit -- $(PATH_TO_TESTS)$(EXEC_T)
else
	@echo "leaks is only available on macOS."
endif
