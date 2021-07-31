# Delete the default suffixes (otherwise visible in 'make -d').
.SUFFIXES:

# Cancel source control implicit rules (otherwise visible in 'make -d').
%: %,v
%: RCS/%,v
%: RCS/%
%: s.%
%: SCCS/s.%

APP_SRC_DIR = app
LIB_SRC_DIR = lib
TST_SRC_DIR = tst
COMMON_SRC_DIR = ../Common/lib

INT_DIR = int

BIN_DIR = bin
TARGET = application
BIN_TARGET = $(BIN_DIR)/$(TARGET)

APP_CPP_FILES := $(shell find $(APP_SRC_DIR) -name '*.cpp')
APP_OBJ_FILES := $(APP_CPP_FILES:$(APP_SRC_DIR)/%.cpp=$(INT_DIR)/%.o)
APP_DEP_FILES := $(APP_CPP_FILES:$(APP_SRC_DIR)/%.cpp=$(INT_DIR)/%.d)

LIB_CPP_FILES := $(shell find $(LIB_SRC_DIR) -name '*.cpp')
LIB_OBJ_FILES := $(LIB_CPP_FILES:$(LIB_SRC_DIR)/%.cpp=$(INT_DIR)/%.o)
LIB_DEP_FILES := $(LIB_CPP_FILES:$(LIB_SRC_DIR)/%.cpp=$(INT_DIR)/%.d)

COMMON_CPP_FILES := $(shell find $(COMMON_SRC_DIR) -name '*.cpp')
COMMON_OBJ_FILES := $(COMMON_CPP_FILES:$(COMMON_SRC_DIR)/%.cpp=$(INT_DIR)/%.o)
COMMON_DEP_FILES := $(COMMON_CPP_FILES:$(COMMON_SRC_DIR)/%.cpp=$(INT_DIR)/%.d)

TST_BIN_DIR = $(BIN_DIR)/tst

TST_CPP_FILES := $(shell find $(TST_SRC_DIR) -name '*.cpp')
TST_OBJ_FILES := $(TST_CPP_FILES:$(TST_SRC_DIR)/%.cpp=$(INT_DIR)/%.o)
TST_DEP_FILES := $(TST_CPP_FILES:$(TST_SRC_DIR)/%.cpp=$(INT_DIR)/%.d)
TST_BIN_TARGETS = $(TST_CPP_FILES:$(TST_SRC_DIR)/%.cpp=$(TST_BIN_DIR)/%)
TST_RESULTS = $(TST_CPP_FILES:$(TST_SRC_DIR)/%.cpp=$(INT_DIR)/%.passed)

DEP_FLAGS = -MT $@ -MMD -MP -MF $(INT_DIR)/$*.d -iquote . -iquote ../Common/lib
STD_FLAGS = -std=c++17 -pthread -fno-rtti
WARN_FLAGS = -Wall -Wextra -Wshadow -Wnon-virtual-dtor -Wpedantic -Werror

CXX = g++
CXXFLAGS = $(STD_FLAGS) $(DEP_FLAGS) $(WARN_FLAGS)
LDFLAGS = $(STD_FLAGS) $(WARN_FLAGS)

GMOCK_SRC_DIR=../thirdparty/src/googletest/googlemock
GTEST_SRC_DIR=../thirdparty/src/googletest/googletest

GMOCK_BUILD_DIR=../thirdparty/build/googlemock/lib
GTEST_BUILD_DIR=../thirdparty/build/googletest/lib

GMOCK_LIB_DIR=$(GMOCK_BUILD_DIR)
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

$(LIB_OBJ_FILES): $(INT_DIR)/%.o: $(LIB_SRC_DIR)/%.cpp $(INT_DIR)/%.d | $(INT_DIR)
	$(CXX) $(CXXFLAGS) -c -o $@ $<

$(COMMON_OBJ_FILES): $(INT_DIR)/%.o: $(COMMON_SRC_DIR)/%.cpp $(INT_DIR)/%.d | $(INT_DIR)
	$(CXX) $(CXXFLAGS) -c -o $@ $<

$(BIN_TARGET): $(APP_OBJ_FILES) $(LIB_OBJ_FILES) $(COMMON_OBJ_FILES) | $(BIN_DIR)
	$(CXX) $(LDFLAGS) -o $@ $^

$(TST_OBJ_FILES): $(INT_DIR)/%.o: $(TST_SRC_DIR)/%.cpp $(INT_DIR)/%.d | $(INT_DIR)
	$(CXX) $(CXXFLAGS) -c -o $@ $< -isystem $(GMOCK_SRC_DIR)/include -isystem $(GTEST_SRC_DIR)/include

$(TST_BIN_DIR)/consumer_test : $(INT_DIR)/consumer_test.o $(INT_DIR)/event.o $(GTEST_LIB_DIR)/libgtest.a $(GTEST_LIB_DIR)/libgtest_main.a | $(TST_BIN_DIR)
	$(CXX) $(LDFLAGS) -o $@ $^

$(TST_RESULTS) : $(INT_DIR)/%.passed: $(TST_BIN_DIR)/%
	$<
	date > $@

$(APP_DEP_FILES): $(INT_DIR)/%.d: ;
$(LIB_DEP_FILES): $(INT_DIR)/%.d: ;
$(COMMON_DEP_FILES): $(INT_DIR)/%.d: ;
$(TST_DEP_FILES): $(INT_DIR)/%.d: ;

-include $(APP_DEP_FILES)
-include $(LIB_DEP_FILES)
-include $(COMMON_DEP_FILES)
-include $(TST_DEP_FILES)

$(INT_DIR):
	mkdir --parents -- $@

$(BIN_DIR):
	mkdir --parents -- $@

$(TST_BIN_DIR):
	mkdir --parents -- $@
