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

DEP_FLAGS = -MT $@ -MMD -MP -MF $(INT_DIR)/$*.d -iquote . -isystem $(ASIO_INCLUDE_DIR)
STD_FLAGS = -std=c++17 -pthread
WARN_FLAGS = -Wall -Wextra -Wshadow -Wnon-virtual-dtor -Wpedantic -Werror

CXX = g++
CXXFLAGS = $(STD_FLAGS) $(DEP_FLAGS) $(WARN_FLAGS)
LDFLAGS = $(STD_FLAGS) $(WARN_FLAGS)

ASIO_INCLUDE_DIR=../thirdparty/src/asio/asio/include

.DEFAULT: all

.PHONY: all, clean

all: application

rebuild: clean all

application: $(BIN_TARGET)

clean:
	rm --force --recursive $(BIN_DIR) $(INT_DIR)

$(APP_OBJ_FILES): $(INT_DIR)/%.o: $(APP_SRC_DIR)/%.cpp $(INT_DIR)/%.d | $(INT_DIR)
	$(CXX) $(CXXFLAGS) -c -o $@ $<

$(LIB_OBJ_FILES): $(INT_DIR)/%.o: $(LIB_SRC_DIR)/%.cpp $(INT_DIR)/%.d | $(INT_DIR)
	$(CXX) $(CXXFLAGS) -c -o $@ $<

$(BIN_TARGET): $(APP_OBJ_FILES) $(LIB_OBJ_FILES) | $(BIN_DIR)
	$(CXX) $(LDFLAGS) -o $@ $^

$(APP_DEP_FILES): $(INT_DIR)/%.d: ;
$(LIB_DEP_FILES): $(INT_DIR)/%.d: ;

-include $(APP_DEP_FILES)
-include $(LIB_DEP_FILES)

$(INT_DIR):
	mkdir --parents -- $@

$(BIN_DIR):
	mkdir --parents -- $@
