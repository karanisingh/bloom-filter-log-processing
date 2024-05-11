# Compiler settings
CXX = g++
CXXFLAGS = -std=c++17 -Wall -Wextra -I./src

# Directories
CPUSRCDIR = src/cpu
TESTDIR = tests
BINDIR = bin
INPUTDIR = input
SCRIPTSDIR = scripts

# Input files
INPUT_FILE = $(INPUTDIR)/simple_logs.txt

# Executable and object files
MAIN_EXEC = $(BINDIR)/main
MAIN_SRC = main.cpp
MAIN_OBJ = $(BINDIR)/main.o

# Source files
CPU_SRC_FILES = $(CPUSRCDIR)/MurmurHash3.cpp \
            $(CPUSRCDIR)/bloom_filter.cpp \
            $(CPUSRCDIR)/log_processor.cpp

# Object files
CPU_OBJS = $(CPU_SRC_FILES:.cpp=.o)

# Targets
all: $(MAIN_EXEC) $(FPGA_EXEC) $(TEST_BLOOM_EXEC)

# Create the bin directory if it does not exist
$(BINDIR):
	mkdir -p $(BINDIR)

# Compile main program
$(MAIN_EXEC): $(MAIN_SRC) $(BINDIR) $(CPU_OBJS)
	$(CXX) $(CXXFLAGS) $(MAIN_SRC) $(CPU_OBJS) -o $@

# Compile individual source files
$(CPUSRCDIR)/%.o: $(CPUSRCDIR)/%.cpp $(CPUSRCDIR)/%.hpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Compile main.cpp if necessary
$(BINDIR)/%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Run the main program with specified input
# make run_main LOGFILE=input/simple_logs.txt
run_main: $(MAIN_EXEC)
	$(MAIN_EXEC) $(INPUT_FILE)

# Run a specific testbench
# not yet implemented
run_test_bloom: $(TEST_BLOOM_EXEC)
	$(TEST_BLOOM_EXEC)

# Create test inputs and paths
# make run_input_generator OUTPUT_FILE=input/2000_random_logs.txt NUM_ENTRIES=2000 NUM_TASKS=100
run_input_generator: 
	python3 $(SCRIPTSDIR)/log_generator.py $(OUTPUT_FILE) $(NUM_ENTRIES) $(NUM_TASKS)
	python3 $(SCRIPTSDIR)/enumerate_paths.py $(OUTPUT_FILE)
	

# Run rule violation generator for a input_PATH.txt file generated by make run_input_generator 
# make find_violations INPUT_FILE=input_file_PATH.txt ANCESTOR=17 DESCENDANT=44
find_violations:
	python3 $(SCRIPTSDIR)/find_rule_violations.py $(INPUT_FILE) $(ANCESTOR) $(DESCENDANT)


# Clean up generated files
clean:
	rm -rf $(BINDIR) $(CPUSRCDIR)/*.o $(TESTDIR)/*.o

.PHONY: all clean run_main run_input_generator find_violations
