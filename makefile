MAKEFLAGS += --silent

.PHONY: default so a fast debug size verbose clean

CC = clang
CFLAGS = $(INCLUDES) -Wall -Wextra -Wpedantic
OPTIMIZE ?= O1

FILE = main
C_OUT = build
SRC_DIR = src
COMPILEDB = compile_commands.json

MAIN ?=  $(SRC_DIR)/$(FILE).c

BIN = $(FILE)
INCL_DIR = include

INCLUDES = $$(find include -type d | sed 's/^/-I.\//')

define MKDIRS
/bin/sh -c ' \
if [ ! -d $(C_OUT) ]; then \
    mkdir $(C_OUT) $(C_OUT)/bin $(C_OUT)/lib $(C_OUT)/dbg; \
else \
    if [ ! -d $(C_OUT)/bin ]; then mkdir $(C_OUT)/bin; fi; \
    if [ ! -d $(C_OUT)/lib ]; then mkdir $(C_OUT)/lib; fi; \
    if [ ! -d $(C_OUT)/dbg ]; then mkdir $(C_OUT)/dbg; fi; \
fi'
endef

define REPLACE_FILES
/bin/sh -c ' \
if [ -d $(C_OUT)/dbg/$(BIN).dSYM/ ]; then \
    rm -r $(C_OUT)/dbg/$(BIN).dSYM/; \
fi; \
\
if [ -d $(C_OUT)/bin/$(BIN).dSYM ]; then \
    mv $(C_OUT)/bin/$(BIN).dSYM $(C_OUT)/dbg/; \
fi'
endef

PRUNE = $(SHELL) -c 'find $(C_OUT) -type d -empty -delete'

FIXCDB = $(SHELL) -c 'echo "[ $$(sed '\''$$ s/.$$//'\'' compile_commands.json) ]" >compile_commands.json'

# CCPFX = $(CC) $$(find $(SRC_DIR) -type f -name '*.c') -o $(C_OUT)/bin/$(BIN) $(CFLAGS) 
CCPFX = $(CC) -MJ $(COMPILEDB) $$(find $(SRC_DIR) -type f -name '*.c') -o $(C_OUT)/bin/$(BIN) $(CFLAGS) 

default: $(MAIN)
	$(MKDIRS)
	$(CCPFX) -$(OPTIMIZE)
	$(REPLACE_FILES)
	$(PRUNE)
	$(FIXCDB)
	echo 'Output dumped to $(C_OUT)/bin/'

IPREFIX=/usr/local

a: $(MAIN)
	$(MKDIRS)
	for dir in $(SRC_DIR)/*/; do \
		dir=$${dir%*/}; \
		dirn=$${dir##*/}; \
		mkdir tmp && cd tmp; \
		clang -c -fPIC $$(find ../$$dir -type f -name '*.c') $$(find ../include -type d | sed 's/^/-I.\//') -Wall -Wextra -Wpedantic; \
		cd ..; \
		ar r $(C_OUT)/lib/lib$${dirn}.a $$(find tmp -type f); \
		rm -r tmp; \
		echo "Created lib$${dirn}.a"; \
	done
	$(REPLACE_FILES)
	$(PRUNE)
	echo "Output dumped to \`$(C_OUT)/lib/'"

SO_INSTALL_PREFIX ?= $(IPREFIX)/lib

so: $(MAIN)
	$(MKDIRS)
	for dir in $(SRC_DIR)/*/; do \
		dir=$${dir%*/}; \
		dirn=$${dir##*/}; \
		clang -shared -fPIC $$(find $$dir -type f -name '*.c') -o $(C_OUT)/lib/lib$${dirn}.so $(CFLAGS) -Wl,-install_name,$(SO_INSTALL_PREFIX)/lib$${dirn}.so; \
		echo "Created lib$${dirn}.so"; \
	done
	$(REPLACE_FILES)
	$(PRUNE)
	echo "Output dumped to \`$(C_OUT)/lib/'"

all:
	$(MAKE) fast
	$(MAKE) a
	$(MAKE) so

fast: $(MAIN)
	$(MKDIRS)
	$(CCPFX) -Ofast
	$(REPLACE_FILES)
	$(PRUNE)
	$(FIXCDB)
	echo 'Output dumped to $(C_OUT)/bin/'

debug: $(MAIN)
	$(MKDIRS)
	$(CCPFX) -g
	$(REPLACE_FILES)
	$(PRUNE)
	$(FIXCDB)
	echo 'Output dumped to $(C_OUT)/bin/'

size: $(MAIN)
	$(MKDIRS)
	$(CCPFX) -Os
	$(REPLACE_FILES)
	$(PRUNE)
	$(FIXCDB)
	echo 'Output dumped to $(C_OUT)/bin/'

verbose: $(MAIN)
	$(MKDIRS)
	$(CCPFX) -$(OPTIMIZE) -v
	$(REPLACE_FILES)
	$(PRUNE)
	$(FIXCDB)
	echo 'Output dumped to $(C_OUT)/bin/'

clean:
	if [ -d $(C_OUT) ]; then rm -r $(C_OUT); echo "Removed build output \`$(C_OUT)'"; fi
	if [ -f compile_commands.json ]; then rm -r compile_commands.json; echo 'Removed compile_commands.json'; fi
	echo 'Build clean'

IINCL_DIR=c_dsa

install: 
	if [ ! -d $(C_OUT) ] || [ ! -d $(C_OUT)/lib ]; then \
		echo 'Nothing to install'; \
	else \
		find $(C_OUT)/lib -type f -exec cp {} $(IPREFIX)/lib \; -exec echo Installed {} to $(IPREFIX)/lib/ \;; \
		mkdir -p $(IPREFIX)/include/c/$(IINCL_DIR); \
		find $(INCL_DIR) -type f -exec $(SHELL) -c 'loc=$(IPREFIX)/include/c/$(IINCL_DIR)/$$(echo {} | sed "s:^$(INCL_DIR)\/::" | sed "s:/.*$$::"); mkdir -p $$loc; cp {} $$loc' \; -exec echo Installed {} to $(IPREFIX)/include/c/$(IINCL_DIR)/ \;; \
	fi
	echo 'All done'

uninstall:
	for dir in $(SRC_DIR)/*/; do \
		dir=$${dir%*/}; \
		name=$(IPREFIX)/lib/lib$${dir##*/}; \
		if [ -f $${name}.a ]; then \
			rm $${name}.a; \
			echo "Uninstalled $${name}.a"; \
		fi; \
		if [ -f $${name}.so ]; then \
			rm $${name}.so; \
			echo "Uninstalled $${name}.so"; \
		fi; \
	done
	rm -rv $(IPREFIX)/include/c/$(IINCL_DIR) | sed 's/^/Uninstalled /'
	echo 'All done'
