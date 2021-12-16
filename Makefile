CXX ?= g++
CXXFLAGS := -g -Wall -pedantic
OBJFLAGS := $(CXXFLAGS) -c
LDFLAGS := -lreadline

PREFIX ?= /usr/local

BIN_PATH := bin
SRC_PATH := src

SRC := $(foreach x, $(SRC_PATH), $(wildcard $(addprefix $(x)/*,cpp)))
OBJ := $(addprefix $(SRC_PATH)/, $(addsuffix .o, $(notdir $(basename $(SRC)))))

default: makedir all

$(BIN_PATH)/tt: $(OBJ)
	$(CXX)  $(CXXFLAGS) -o $@ $(OBJ) $(LDFLAGS)

$(SRC_PATH)/%.o: $(SRC_PATH)/%.cpp
	$(CXX) -DPREFIX=$(PREFIX) $(OBJFLAGS) -o $@ $<

makedir:
	@mkdir -p $(BIN_PATH)

all: $(BIN_PATH)/tt

clean:
	@rm -rf $(SRC_PATH)/*.o
	@rm -rf $(BIN_PATH)/tt

.PHONY: makedir clean
