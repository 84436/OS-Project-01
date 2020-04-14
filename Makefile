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

all: osh

osh: $(SRC) $(BIN_DIR)
	$(CC) $(CCFLAGS) -o $(BIN_DIR)/$@ $(SRC)

$(BIN_DIR):
	mkdir $@

clean:
	rm -rf $(BIN_DIR)
