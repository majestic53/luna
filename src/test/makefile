# Project Luna
# Copyright (C) 2014 David Jolly

CC=g++
EXE=luna
FLAGS=-std=c++0x -O3 -funroll-all-loops
IN=./
LIB=libluna.a
OUT_BIN=./../../bin/
OUT_BUILD=./../../build/

all: build

build:
	$(CC) $(FLAGS) $(IN)main.cpp $(IN)luna_product.h $(IN)luna_resource.h $(OUT_BIN)$(LIB) -o $(OUT_BIN)$(EXE)
