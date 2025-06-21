# MAKEFLAGS += --silent

.PHONY: default so a fast debug size verbose clean help

CC ?= clang
PREFIX ?= lib
OPTIMIZE ?= -O3
EXTRA_CFLAGS ?= 

CFLAGS = $(INCLUDES) -Wall -Wextra -Wpedantic

FILE = main
C_OUT = build
SRC_DIR = src
COMPILEDB = compile_commands.json

MAIN ?= $(SRC_DIR)/$(FILE).c

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
	$(CCPFX) $(OPTIMIZE)
	$(REPLACE_FILES)
	$(PRUNE)
	$(FIXCDB)
	echo 'Output dumped to $(C_OUT)/bin/'

a: $(MAIN)
	$(MKDIRS)
	for dir in $(SRC_DIR)/*/; do \
		dir=$${dir%*/}; \
		dirn=$${dir##*/}; \
		mkdir tmp && cd tmp; \
		clang -c -fPIC $$(find ../$$dir -type f -name '*.c') $$(find ../include -type d | sed 's/^/-I.\//') -Wall -Wextra -Wpedantic $(OPTIMIZE) $(EXTRA_CFLAGS); \
		cd ..; \
		ar r $(C_OUT)/lib/lib$${dirn}.a $$(find tmp -type f); \
		rm -r tmp; \
		echo "Created lib$${dirn}.a"; \
	done
	$(REPLACE_FILES)
	$(PRUNE)
	echo "Output dumped to \`$(C_OUT)/lib/'"

INSTALL_DIR = $(PREFIX)/lib

so: $(MAIN)
	$(MKDIRS)
	for dir in $(SRC_DIR)/*/; do \
		dir=$${dir%*/}; \
		dirn=$${dir##*/}; \
		clang -shared -fPIC $$(find $$dir -type f -name '*.c') -o $(C_OUT)/lib/lib$${dirn}.so $(CFLAGS) -Wl,-install_name,$(INSTALL_DIR)/lib$${dirn}.so -Wall -Wextra -Wpedantic $(OPTIMIZE) $(EXTRA_CFLAGS); \
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
	$(CCPFX) $(OPTIMIZE) -v
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
		if [ ! -d $(PREFIX) ]; then mkdir $(PREFIX); fi; \
		mkdir $(PREFIX)/lib $(PREFIX)/include ; \
		find $(C_OUT)/lib -type f -exec cp {} $(INSTALL_DIR) \; -exec echo Installed {} to $(INSTALL_DIR)/ \;; \
		mkdir -p $(PREFIX)/include/c/$(IINCL_DIR); \
		find $(INCL_DIR) -type f -exec $(SHELL) -c 'loc=$(PREFIX)/include/c/$(IINCL_DIR)/$$(echo {} | sed "s:^$(INCL_DIR)\/::" | sed "s:/.*$$::"); mkdir -p $$loc; cp {} $$loc' \; -exec echo Installed {} to $(PREFIX)/include/c/$(IINCL_DIR)/ \;; \
	fi
	echo 'All done'

uninstall:
	for dir in $(SRC_DIR)/*/; do \
		dir=$${dir%*/}; \
		name=$(PREFIX)/lib/lib$${dir##*/}; \
		if [ -f $${name}.a ]; then \
			rm $${name}.a; \
			echo "Uninstalled $${name}.a"; \
		fi; \
		if [ -f $${name}.so ]; then \
			rm $${name}.so; \
			echo "Uninstalled $${name}.so"; \
		fi; \
	done
	rm -rv $(PREFIX)/include/c/$(IINCL_DIR) | sed 's/^/Uninstalled /'
	echo 'All done'

define HELPDOC
TARGETS
	default  Build an executable to build/bin/
	a        Build static libraries (ar archives) to build/lib/
	so       Build shared libraries (.so) to build/lib
	all      Runs the default, a, and so targets
	fast     Build an -O3 executable to build/bin/
	debug    Build an executable with debug symbols
	size     Build an -Os executable (for supported compilers)
	verbose  Build with the -v flag
	clean    Clean the build folder
	install  Installs build cache files in build/ to PREFIX 
	help     Prints this help message

OPTIONS
	CC            C compiler [clang]
	PREFIX        Install prefix [lib]
	OPTIMIZE      Optimization flag [-O3]
	EXTRA_CFLAGS  Extra compile flags []

EXAMPLES
	Make shared libraries using a toolchain and install to /usr/local
		$ make so install CC=arm-linux-gnueabi-gcc PREFIX=/usr/local EXTRA_CFLAGS=-fPIC
endef

export HELPDOC
help:
	echo "$$HELPDOC"
