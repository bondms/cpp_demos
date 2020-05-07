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

INT_DIR = int

BIN_DIR = bin
TARGET = application
BIN_TARGET = $(BIN_DIR)/$(TARGET)

APP_CPP_FILES := $(shell find $(APP_SRC_DIR) -name '*.cpp')
APP_OBJ_FILES := $(APP_CPP_FILES:$(APP_SRC_DIR)/%.cpp=$(INT_DIR)/%.o)
APP_DEP_FILES := $(APP_CPP_FILES:$(APP_SRC_DIR)/%.cpp=$(INT_DIR)/%.d)

DEP_FLAGS = -MT $@ -MMD -MP -MF $(INT_DIR)/$*.d
STD_FLAGS = --std=c++17 -pthread -fno-rtti
WARN_FLAGS = -Wall -Wextra -Werror

CXX = g++
CXXFLAGS = $(STD_FLAGS) $(DEP_FLAGS) $(WARN_FLAGS)
LDFLAGS = $(STD_FLAGS) $(WARN_FLAGS)

JSONCPP_DIR=../thirdparty/src/jsoncpp
JSONCPP_DIST_DIR=$(JSONCPP_DIR)/dist

.DEFAULT: all

.PHONY: all, clean

all: application

rebuild: clean all

application: $(BIN_TARGET)

clean:
	rm --force --recursive $(BIN_DIR) $(INT_DIR)

$(APP_OBJ_FILES): $(INT_DIR)/%.o: $(APP_SRC_DIR)/%.cpp $(INT_DIR)/%.d | $(INT_DIR)
	$(CXX) $(CXXFLAGS) -c -o $@ $< -isystem $(JSONCPP_DIST_DIR)

$(BIN_TARGET): $(APP_OBJ_FILES) $(JSONCPP_OBJ_FILE) | $(BIN_DIR)
	$(CXX) $(LDFLAGS) -o $@ $^

$(APP_DEP_FILES): $(INT_DIR)/%.d: ;

-include $(APP_DEP_FILES)

$(INT_DIR):
	mkdir --parents -- $@

$(BIN_DIR):
	mkdir --parents -- $@
