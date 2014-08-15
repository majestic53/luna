# Project Luna
# Copyright (C) 2014 David Jolly

CC=g++
FLAGS=-std=c++0x -O3 -funroll-all-loops
IN=./
LIB=libluna.a
OUT_BIN=./../../bin/
OUT_BUILD=./../../build/

all: build archive

archive:
	ar rcs $(OUT_BIN)$(LIB) $(OUT_BUILD)luna.o $(OUT_BUILD)luna_defines.o $(OUT_BUILD)luna_exception.o $(OUT_BUILD)luna_interpreter.o $(OUT_BUILD)luna_language.o $(OUT_BUILD)luna_lexer.o $(OUT_BUILD)luna_node.o $(OUT_BUILD)luna_parser.o $(OUT_BUILD)luna_scope.o $(OUT_BUILD)luna_token.o $(OUT_BUILD)luna_trace.o $(OUT_BUILD)luna_uuid.o

build: luna.o luna_defines.o luna_exception.o luna_interpreter.o luna_language.o luna_lexer.o luna_node.o luna_parser.o luna_scope.o luna_token.o luna_trace.o luna_uuid.o

luna.o: $(IN)luna.cpp $(IN)luna.h $(IN)luna_type.h
	$(CC) $(FLAGS) -c $(IN)luna.cpp -o $(OUT_BUILD)luna.o

luna_defines.o: $(IN)luna_defines.cpp $(IN)luna_defines.h
	$(CC) $(FLAGS) -c $(IN)luna_defines.cpp -o $(OUT_BUILD)luna_defines.o

luna_exception.o: $(IN)luna_exception.cpp $(IN)luna_exception.h $(IN)luna_exception_type.h
	$(CC) $(FLAGS) -c $(IN)luna_exception.cpp -o $(OUT_BUILD)luna_exception.o

luna_interpreter.o: $(IN)luna_interpreter.cpp $(IN)luna_interpreter.h $(IN)luna_interpreter_type.h
	$(CC) $(FLAGS) -c $(IN)luna_interpreter.cpp -o $(OUT_BUILD)luna_interpreter.o

luna_language.o: $(IN)luna_language.cpp $(IN)luna_language.h $(IN)luna_language_type.h
	$(CC) $(FLAGS) -c $(IN)luna_language.cpp -o $(OUT_BUILD)luna_language.o

luna_lexer.o: $(IN)luna_lexer.cpp $(IN)luna_lexer.h $(IN)luna_lexer_type.h
	$(CC) $(FLAGS) -c $(IN)luna_lexer.cpp -o $(OUT_BUILD)luna_lexer.o

luna_node.o: $(IN)luna_node.cpp $(IN)luna_node.h $(IN)luna_node_type.h
	$(CC) $(FLAGS) -c $(IN)luna_node.cpp -o $(OUT_BUILD)luna_node.o

luna_parser.o: $(IN)luna_parser.cpp $(IN)luna_parser.h $(IN)luna_parser_type.h
	$(CC) $(FLAGS) -c $(IN)luna_parser.cpp -o $(OUT_BUILD)luna_parser.o

luna_scope.o: $(IN)luna_scope.cpp $(IN)luna_scope.h $(IN)luna_scope_type.h
	$(CC) $(FLAGS) -c $(IN)luna_scope.cpp -o $(OUT_BUILD)luna_scope.o

luna_token.o: $(IN)luna_token.cpp $(IN)luna_token.h $(IN)luna_token_type.h
	$(CC) $(FLAGS) -c $(IN)luna_token.cpp -o $(OUT_BUILD)luna_token.o

luna_trace.o: $(IN)luna_trace.cpp $(IN)luna_trace.h $(IN)luna_trace_type.h
	$(CC) $(FLAGS) -c $(IN)luna_trace.cpp -o $(OUT_BUILD)luna_trace.o

luna_uuid.o: $(IN)luna_uuid.cpp $(IN)luna_uuid.h $(IN)luna_uuid_type.h
	$(CC) $(FLAGS) -c $(IN)luna_uuid.cpp -o $(OUT_BUILD)luna_uuid.o
