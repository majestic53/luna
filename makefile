# Project Luna
# Copyright (C) 2014 David Jolly

EXE_DIR=./src/test/
LIB_DIR=./src/core/
OUT_BIN=./bin/
OUT_BUILD=./build/

all: clean init lib exe

clean:
	rm -rf $(OUT_BIN)
	rm -rf $(OUT_BUILD)

exe:
	cd $(EXE_DIR) && make all

init:
	mkdir $(OUT_BIN)
	mkdir $(OUT_BUILD)

lib: 
	cd $(LIB_DIR) && make all
