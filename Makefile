-include config.out

CXX ?= g++
CXXFLAGS := -std=c++11 -g -Wall -pedantic
OBJFLAGS := $(CXXFLAGS) -c
LDFLAGS := -lreadline

PREFIX ?= /usr/local
USER_NAME ?= $(USER)
TRACKING_DIR ?= $(HOME)/track

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
	@echo "PREFIX=$(PREFIX)" > config.out
	@echo "USER_NAME=$(USER_NAME)" >> config.out
	@echo "TRACKING_DIR=$(TRACKING_DIR)" >> config.out
install: all
	@echo "Installing tt at $(PREFIX) for user $(USER_NAME). Tracking directory will be $(TRACKING_DIR)."
	@mkdir -p $(PREFIX)/bin
	@mkdir -p $(PREFIX)/etc
	@cp bin/tt $(PREFIX)/bin
	@touch $(PREFIX)/etc/tt.conf;\
		echo "user_name=$(USER_NAME)" > $(PREFIX)/etc/tt.conf;\
		echo "tracking_directory=$(TRACKING_DIR)" >> $(PREFIX)/etc/tt.conf
	@echo "tt successfully installed. User name and tracking directory can be adjusted in $(PREFIX)/etc/tt.conf."
	
uninstall:
	rm -rf $(PREFIX)/bin/tt
	rm -rf $(PREFIX)/etc/tt.conf
	@echo "tt has been uninstalled. Note: tracking data is still present at $(TRACKING_DIR)"

clean:
	rm -rf $(SRC_PATH)/*.o
	rm -rf $(BIN_PATH)
	rm -rf config.out

.PHONY: makedir clean
