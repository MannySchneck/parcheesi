CPP_PROG_FILES := board.cpp game.cpp moves.cpp rule_checker.cpp turn.cpp
CPP_OBJ_FILES := $(CPP_PROG_FILES:.cpp=.o)

CPP_TEST_FILES :=  test/rules_tests.cpp

TEST_MAIN_CPP := test/catch_main.cpp

CC_FLAGS := --std=c++11 -O0 -g -fprofile-instr-generate -fcoverage-mapping -D TEST

CC := clang++

all: tests program

tests: bin/test_main.o $(CPP_OBJ_FILES)
	$(CC) $(CC_FLAGS) $(CPP_TEST_FILES) $^ -o test.bin
	./test.bin

$(CPP_OBJ_FILES): %.o:%.cpp
	$(CC) $(CC_FLAGS)  -c $< -o $@

bin/test_main.o :
	$(CC) $(CC_FLAGS) -c test/catch_main.cpp -o $@

program:
	$(CC) $(CC_FLAGS) $(CPP_FILES)

clean:
	rm -rf bin/* bin/*.o

