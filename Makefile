CPP_PROG_FILES := board.cpp game.cpp moves.cpp rule_checker.cpp turn.cpp splayer.cpp mplayer.cpp dumb_player.cpp interfaces.cpp parser.cpp arg_wrapper_classes.cpp network.cpp

CPP_OBJ_FILES := $(CPP_PROG_FILES:.cpp=.o)

CPP_TEST_FILES :=  test/rules_tests.cpp

TEST_MAIN_CPP := test/catch_main.cpp

SRCS := TEST_MAIN_CPP CPP_TEST_FILES CPP_PROG_FILES

CCX_FLAGS := --std=c++1z -O0 -g -fprofile-instr-generate -fcoverage-mapping -D TEST \
							-D_LIBCPP_ENABLE_CXX17_REMOVED_AUTO_PTR

CCX_FLAGS +=  -stdlib=libc++ -nostdinc++ \
          -I/usr/local/Cellar/llvm/HEAD-fdcdb2a/include/c++/v1 \
					-Ilib/boost_1_64_0 \
					-Ilib/PEGTL/include/tao \
		      -L/usr/local/Cellar/llvm/HEAD-fdcdb2a/lib \
	        -Wl,-rpath, /usr/local/Cellar/llvm/HEAD-fdcdb2a/lib \
					-l boost_system

CCX := clang++

all: tests program

tests: bin/test_main.o $(CPP_OBJ_FILES)
	$(CCX) $(CCX_FLAGS) $(CPP_TEST_FILES) $^ -o test.bin
	./test.bin

libcheesy.a: $(CPP_OBJ_FILES)
	ar -rv libcheesy.a $^

$(CPP_OBJ_FILES): %.o:%.cpp
	$(CC) $(CCX_FLAGS)  -c $< -o $@

bin/test_main.o :
	$(CC) $(CCX_FLAGS) -c test/catch_main.cpp -o $@

program:
	$(CC) $(CCX_FLAGS) $(CPP_FILES)

clean:
	rm -rf bin/* bin/*.	*.o *.bin

DEPDIR := .d
$(shell mkdir -p $(DEPDIR) >/dev/null)
DEPFLAGS = -MT $@ -MMD -MP -MF $(DEPDIR)/$*.Td

COMPILE.c = $(CC) $(DEPFLAGS) $(CFLAGS) $(CPPFLAGS) $(TARGET_ARCH) -c
COMPILE.cc = $(CXX) $(DEPFLAGS) $(CXXFLAGS) $(CPPFLAGS) $(TARGET_ARCH) -c
POSTCOMPILE = @mv -f $(DEPDIR)/$*.Td $(DEPDIR)/$*.d && touch $@

%.o : %.c
%.o : %.c $(DEPDIR)/%.d
	$(COMPILE.c) $(OUTPUT_OPTION) $<
	$(POSTCOMPILE)

%.o : %.cc
%.o : %.cc $(DEPDIR)/%.d
	$(COMPILE.cc) $(OUTPUT_OPTION) $<
	$(POSTCOMPILE)

%.o : %.cpp
%.o : %.cpp $(DEPDIR)/%.d
	$(COMPILE.cc) $(OUTPUT_OPTION) $<
	$(POSTCOMPILE)

$(DEPDIR)/%.d: ;
.PRECIOUS: $(DEPDIR)/%.d

include $(wildcard $(patsubst %,$(DEPDIR)/%.d,$(basename $(SRCS))))
