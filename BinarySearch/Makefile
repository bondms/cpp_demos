# Delete the default suffixes (otherwise visible in 'make -d').
.SUFFIXES:

# Cancel source control implicit rules (otherwise visible in 'make -d').
%: %,v
%: RCS/%,v
%: RCS/%
%: s.%
%: SCCS/s.%

SRC_DIR = src
APP_SRC_DIR = $(SRC_DIR)/app
TST_SRC_DIR = $(SRC_DIR)/tst

INT_DIR = int

BIN_DIR = bin
TARGET = application
BIN_TARGET = $(BIN_DIR)/$(TARGET)

APP_CPP_FILES := $(shell find $(APP_SRC_DIR) -name '*.cpp')
APP_OBJ_FILES := $(APP_CPP_FILES:$(APP_SRC_DIR)/%.cpp=$(INT_DIR)/%.o)
APP_DEP_FILES := $(APP_CPP_FILES:$(APP_SRC_DIR)/%.cpp=$(INT_DIR)/%.d)

TST_BIN_DIR = $(BIN_DIR)/tst

TST_CPP_FILES := $(shell find $(TST_SRC_DIR) -name '*.cpp')
TST_OBJ_FILES := $(TST_CPP_FILES:$(TST_SRC_DIR)/%.cpp=$(INT_DIR)/%.o)
TST_DEP_FILES := $(TST_CPP_FILES:$(TST_SRC_DIR)/%.cpp=$(INT_DIR)/%.d)
TST_BIN_TARGETS = $(TST_CPP_FILES:$(TST_SRC_DIR)/%.cpp=$(TST_BIN_DIR)/%)
TST_RESULTS = $(TST_CPP_FILES:$(TST_SRC_DIR)/%.cpp=$(INT_DIR)/%.passed)

DEP_FLAGS = -MT $@ -MMD -MP -MF $(INT_DIR)/$*.d
STD_FLAGS = --std=c++17 -pthread -fno-rtti
WARN_FLAGS = -Wall -Wextra -Werror

CXX = g++
CXXFLAGS = $(STD_FLAGS) $(DEP_FLAGS) $(WARN_FLAGS)
LDFLAGS = $(STD_FLAGS) $(WARN_FLAGS)

GMOCK_SRC_DIR=../thirdparty/src/googletest/googlemock
GTEST_SRC_DIR=../thirdparty/src/googletest/googletest

GMOCK_BUILD_DIR=../thirdparty/build/googlemock/lib
GTEST_BUILD_DIR=../thirdparty/build/googletest/lib

GTEST_LIB_DIR=$(GTEST_BUILD_DIR)

.DEFAULT: all

.PHONY: all, clean

all: application tests run_tests

rebuild: clean all

application: $(BIN_TARGET)

clean:
	rm --force --recursive $(BIN_DIR) $(INT_DIR)

tests: $(TST_BIN_TARGETS)

run_tests: $(TST_RESULTS)

$(APP_OBJ_FILES): $(INT_DIR)/%.o: $(APP_SRC_DIR)/%.cpp $(INT_DIR)/%.d | $(INT_DIR)
	$(CXX) $(CXXFLAGS) -c -o $@ $<

$(BIN_TARGET): $(APP_OBJ_FILES) | $(BIN_DIR)
	$(CXX) $(LDFLAGS) -o $@ $^

$(TST_OBJ_FILES): $(INT_DIR)/%.o: $(TST_SRC_DIR)/%.cpp $(INT_DIR)/%.d | $(INT_DIR)
	$(CXX) $(CXXFLAGS) -c -o $@ $< -isystem $(GMOCK_SRC_DIR)/include -isystem $(GTEST_SRC_DIR)/include

$(TST_BIN_DIR)/binary_search_test : $(INT_DIR)/binary_search_test.o $(GTEST_LIB_DIR)/libgtest.a $(GTEST_LIB_DIR)/libgtest_main.a | $(TST_BIN_DIR)
	$(CXX) $(LDFLAGS) -o $@ $^

$(TST_RESULTS) : $(INT_DIR)/%.passed: $(TST_BIN_DIR)/%
	$<
	date > $@

$(APP_DEP_FILES): $(INT_DIR)/%.d: ;
$(TST_DEP_FILES): $(INT_DIR)/%.d: ;

-include $(APP_DEP_FILES)
-include $(TST_DEP_FILES)

$(INT_DIR):
	mkdir --parents -- $@

$(BIN_DIR):
	mkdir --parents -- $@

$(TST_BIN_DIR):
	mkdir --parents -- $@
