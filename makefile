CC = clang
CFLAGS = $(INCLUDES) -g -Wall -Wextra -Wpedantic
OPTIMIZE ?= O1

FILE = main
MAIN ?=  src/$(FILE).c
BIN = $(FILE)

SRC_DIR = src
INCL_DIR = include
INCLUDES = $$(find include -type d | sed 's/^/-I.\//' | tr '\n' ' ')

MKDIRS = $(SHELL) -c 'if [ ! -d bin/ ]; then mkdir bin/; fi; if [ ! -d dbg/ ]; then mkdir dbg/; fi'
REPLACE_FILES = $(SHELL) -c 'if [ -d dbg/$(BIN).dSYM/ ]; then rm -r dbg/$(BIN).dSYM/; fi; if [ -d bin/$(BIN).dSYM/ ]; then mv bin/$(BIN).dSYM/ dbg//; fi'

CCPFX = $(CC) $$(find . -type f -name '*.c') -o bin/$(BIN) $(CFLAGS) 

default: $(MAIN)
	$(MKDIRS)
	$(CCPFX) -$(OPTIMIZE)
	$(REPLACE_FILES)

fast:
	$(MKDIRS)
	$(CCPFX) -Ofast
	$(REPLACE_FILES)

debug:
	$(MKDIRS)
	$(CCPFX) -Og
	$(REPLACE_FILES)

size:
	$(MKDIRS)
	$(CCPFX) -Os
	$(REPLACE_FILES)

verbose: $(MAIN)
	$(MKDIRS)
	$(CCPFX) -$(OPTIMIZE) -v
	$(REPLACE_FILES)

clean: bin/$(BIN) dbg/$(BIN).dSYM/
	rm bin/$(BIN)
	rm -r dbg/$(BIN).dSYM/
