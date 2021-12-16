CXX ?= g++
CXXFLAGS := -g -Wall -pedantic
OBJFLAGS := $(CXXFLAGS) -c
LDFLAGS := -lreadline

PREFIX ?= /usr/local
USER_NAME ?= $(USER)
TRACKING_DIR ?= /home/$(USER)/track

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

install: all
	@echo "Installing tt at $(PREFIX) for user $(USER_NAME). Tracking directory will be $(TRACKING_DIR)."
	@mkdir -p $(PREFIX)/bin
	@mkdir -p $(PREFIX)/etc
	@cp bin/tt $(PREFIX)/bin
	@touch $(PREFIX)/etc/tt.conf;\
		echo "user_name=$(USER_NAME)" > $(PREFIX)/etc/tt.conf;\
		echo "tracking_directory=$(TRACKING_DIR)" >> $(PREFIX)/etc/tt.conf
	@echo "tt successfully installed. User name and tracking directory can be adjusted in $(PREFIX)/etc/tt.conf."
	

clean:
	rm -rf $(SRC_PATH)/*.o
	rm -rf $(BIN_PATH)/tt

.PHONY: makedir clean
