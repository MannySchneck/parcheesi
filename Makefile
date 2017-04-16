CPP_PROG_FILES := board.cpp game.cpp moves.cpp rule_checker.cpp

CPP_TEST_FILES :=  test/rules_tests.cpp

TEST_MAIN_CPP := test/catch_main.cpp

CC_FLAGS := --std=c++11

CC := clang++

all: tests program

tests: bin/test_main.o
	$(CC) -D TEST $(CC_FLAGS) $(CPP_PROG_FILES) $(CPP_TEST_FILES) $< -o test.bin
	./test.bin

bin/test_main.o :
	$(CC) $(CC_FLAGS) -c test/catch_main.cpp -o $@

program:
	clang $(CC) $(CC_FLAGS) $(CPP_FILES)
