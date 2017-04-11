CPP_PROG_FILES := board.cpp game.cpp moves.cpp

CPP_TEST_FILES := test/board_test.cpp

CC_FLAGS := --std=c++11

CC := clang++

all: tests program

tests:
	$(CC) -D TEST $(CC_FLAGS) $(CPP_PROG_FILES) $(CPP_TEST_FILES) -o test.bin
	./test.bin
program:
	clang $(CC) $(CC_FLAGS) $(CPP_FILES)
