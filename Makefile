# A very rudimentary Makefile
# References:
# 	http://www.cs.colby.edu/maxwell/courses/tutorials/maketutor/
# 	https://stackoverflow.com/a/30602701
#
# INDENT EVERYTHING BY TAB; THIS IS A REQUIREMENT BY MAKEFILE.

CC		:=	gcc
CCFLAGS	:=	-Wall
SRC_DIR	:=	src
BIN_DIR	:=	bin
SRC		:=	$(wildcard $(SRC_DIR)/*.c)

all: $(BIN_DIR) osh
$(BIN_DIR):
	mkdir $@

osh: $(SRC)
	$(CC) $(CCFLAGS) -o $(BIN_DIR)/$@ $<

clean:
	rm -rf $(BIN_DIR)
