CXX = g++
DEPFLAGS = -MP -MD 
CXXFLAGS = -Wall -Wextra -Werror -std=c++17 -pedantic $(DEPFLAGS) -g
GCOVR_FLAGS = --coverage -fprofile-arcs -ftest-coverage -O0 -fPIC
VALGRIND_FLAGS = --leak-check=full --show-leak-kinds=all --track-origins=yes
GTEST_FLAGS =
OPEN =
OS = $(shell uname -s)
ifeq ($(OS), Linux)
	GTEST_FLAGS = -lgtest_main -lgtest -lpthread -lm
	OPEN = xdg-open
else
	GTEST_FLAGS = -lgtest_main -lgtest
	OPEN = open
endif

QMAKE = qmake
QT_INSTALL_PREFIX = $(shell $(QMAKE) -query QT_INSTALL_PREFIX)


PROJECT_NAME=SmartCalc_v2
PROJECT_DIR= $(PROJECT_NAME)/

APP = $(BUILD_DIR)$(PROJECT_NAME)

SRC_DIR=$(PROJECT_DIR)model/
TEST_DIR=tests/
APP_DIR=app/
BUILD_DIR=build/
DIST_DIR=dist/
REPORT_DIR=report/
LEAKS_DIR =leaks/
OBJ_DIR =objs/
LIB_DIR =lib/

LIB=$(LIB_DIR)$(PROJECT_NAME).a
SRC_FILES=$(shell find $(SRC_DIR) \( -name "*.cpp" \))
TEST_FILES=$(shell find $(TEST_DIR) \( -name "*.cpp" \))
TEST_DATA_FILES=$(wildcard $(TEST_DIR)data/*.dat)
FILES_TO_ANALYSE=$(shell find $(PROJECT_DIR) \( -name "*.cpp" -o -name "*.h" -o -name "*.tpp" \))

SRC_OBJS= $(addprefix $(OBJ_DIR)src/, $(notdir $(SRC_FILES:%.cpp=%.o)))
SRC_REPORT_OBJS = $(addprefix $(OBJ_DIR)$(REPORT_DIR), $(notdir $(SRC_FILES:%.cpp=%.o)))
TEST_OBJS = $(addprefix $(OBJ_DIR)$(TEST_DIR), $(notdir $(TEST_FILES:%.cpp=%.o)))

TEST_EXE = $(TEST_DIR)bin/test.out
TEST_COV_EXE = $(REPORT_DIR)bin/test_cov.out

-include $(SRC_OBJS:.o=.d) $(TEST_OBJS:.o=.d)

all: install

install: clean $(APP)

uninstall:
	@rm -rf build*

clean:
	find ./ \( -name "*.o" -o -name "*.gc*" -o -name "*.exe" -o -name "*.a" -o -name "*.out" -o -name "*.d" -o -name "*.pdf" -o -name "*.dvi" \) -delete
	rm -rf $(BUILD_DIR)
	rm -rf $(OBJ_DIR)
	rm -rf $(LEAKS_DIR)
	rm -rf $(REPORT_DIR)
	rm -rf $(DIST_DIR)
	rm -rf $(TEST_DIR)bin/
	rm -rf $(LIB_DIR)

$(APP): $(FILES_TO_ANALYSE)
	mkdir -p $(BUILD_DIR)
	cmake -DCMAKE_PREFIX_PATH=$(QT_INSTALL_PREFIX) -S $(PROJECT_DIR) -B $(BUILD_DIR)
	make -C $(BUILD_DIR)

run: $(APP)
	@./$(shell find $(BUILD_DIR) -name "$(PROJECT_NAME)")

$(PROJECT_DIR)manual.dvi: $(PROJECT_DIR)manual.tex
	#sudo apt install -y texinfo texlive xpdf
	texi2dvi $< -c -o $@ > /dev/null

$(PROJECT_DIR)manual.pdf: $(PROJECT_DIR)manual.tex
	#sudo apt install -y texinfo texlive xpdf
	texi2pdf $< -c -o $@ > /dev/null

dvi: $(PROJECT_DIR)manual.dvi
	xdvi $< 2>/dev/null

pdf: $(PROJECT_DIR)manual.pdf
	xpdf $< 2>/dev/null

dist:
	@mkdir -p $(DIST_DIR)
	@cp Makefile $(DIST_DIR)
	@cp -r $(PROJECT_DIR) $(DIST_DIR)
	tar -czvf $(DIST_DIR)$(PROJECT_NAME).tar.gz $(DIST_DIR)*

# By default, this target runs all tests.
#
# You can use 'test_name' parameter to run specific tests.
#
# To run a specific Test:
#     make tests test_name=MyTestSuite.MyTestCase
#
# To run a specific TestSuite:
#     make tests test_name=MyTestSuite
tests: $(TEST_EXE)
	./$< $(if $(test_name),--gtest_filter=$(test_name)*)

$(LIB): $(SRC_OBJS)
	@mkdir -p $(LIB_DIR)
	ar rcs $(LIB) $(SRC_OBJS)
	ranlib $(LIB)

# Rule to compile src files
$(OBJ_DIR)src/%.o: $(SRC_DIR)%.cpp
	@mkdir -p $(OBJ_DIR)src
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Rule to compile src files
$(OBJ_DIR)src/%.o: $(SRC_DIR)**/%.cpp
	@mkdir -p $(OBJ_DIR)src
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Rule to compile src files
$(OBJ_DIR)src/%.o: $(SRC_DIR)**/**/%.cpp
	@mkdir -p $(OBJ_DIR)src
	$(CXX) $(CXXFLAGS) -c $< -o $@


# Rule to compile src files for gcov
$(OBJ_DIR)$(REPORT_DIR)%.o: $(SRC_DIR)%.cpp
	@mkdir -p $(OBJ_DIR)$(REPORT_DIR)
	$(CXX) $(CXXFLAGS) $(GCOVR_FLAGS) -c $< -o $@

$(OBJ_DIR)$(REPORT_DIR)%.o: $(SRC_DIR)**/%.cpp
	@mkdir -p $(OBJ_DIR)$(REPORT_DIR)
	$(CXX) $(CXXFLAGS) $(GCOVR_FLAGS) -c $< -o $@

$(OBJ_DIR)$(REPORT_DIR)%.o: $(SRC_DIR)**/**/%.cpp
	@mkdir -p $(OBJ_DIR)$(REPORT_DIR)
	$(CXX) $(CXXFLAGS) $(GCOVR_FLAGS) -c $< -o $@

# Rule to compile test files
$(OBJ_DIR)$(TEST_DIR)%.o: $(TEST_DIR)%.cpp
	@mkdir -p $(OBJ_DIR)$(TEST_DIR)
	$(CXX) $(CXXFLAGS) -c $< -o $@

$(OBJ_DIR)$(TEST_DIR)%.o: $(TEST_DIR)**/%.cpp
	@mkdir -p $(OBJ_DIR)$(TEST_DIR)
	$(CXX) $(CXXFLAGS) -c $< -o $@

$(OBJ_DIR)$(TEST_DIR)%.o: $(TEST_DIR)**/**%.cpp
	@mkdir -p $(OBJ_DIR)$(TEST_DIR)
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Rule to compile test executable
$(TEST_EXE): $(TEST_OBJS) $(LIB) $(TEST_DATA_FILES)
	mkdir -p $(TEST_DIR)bin/
	$(CXX) $(CXXFLAGS) $(TEST_OBJS) $(LIB) $(GTEST_FLAGS) -o $@


gcov_report: $(SRC_REPORT_OBJS) $(TEST_OBJS)
	mkdir -p $(REPORT_DIR)bin
	$(CXX) $(CXXFLAGS) $(GCOVR_FLAGS) $^ -o $(TEST_COV_EXE) $(GTEST_FLAGS)
	$(TEST_COV_EXE) 
	gcovr -r . --html --html-details -o $(REPORT_DIR)coverage-report.html
	$(OPEN) $(REPORT_DIR)coverage-report.html

# Static analysis
cppcheck: $(FILES_TO_ANALYSE)
	cppcheck $^ --language=c++ --std=c++17 --enable=all --suppress=missingIncludeSystem --suppress=unusedFunction 

check_style:
	@echo "┏=========================================┓"
	@echo "┃  Checking your code for Google Style    ┃"
	@echo "┗=========================================┛"
	@find . \( -name '*.h' -o -name '*.hpp' -o -name '*.cpp' -o -name '*.tpp' \) -print0 | xargs -0 clang-format -n -style=Google

format_style:
	@echo "┏=========================================┓"
	@echo "┃  Formatting your code for Google Style  ┃"
	@echo "┗=========================================┛"
	@find . \( -name '*.h' -o -name '*.hpp' -o -name '*.cpp' -o -name '*.tpp' \) -print0 | xargs -0 clang-format -i -style=Google

valgrind: $(TEST_EXE)
	mkdir -p $(LEAKS_DIR)
	valgrind $(VALGRIND_FLAGS) --log-file=$(LEAKS_DIR)valgrind.log $(TEST_EXE)
	$(OPEN) $(LEAKS_DIR)valgrind.log
leaks: $(TEST_EXE)
	mkdir -p $(LEAKS_DIR)
	leaks -atExit -- $(TEST_EXE) > $(LEAKS_DIR)leaks_report.txt
	$(LEAKS_DIR)leaks_report.txt
rebuild: clean all


.PHONY: all install uninstall clean run dvi pdf dist tests gcov_report check_style format_style valgrind leaks rebuild 