all: compile

compile:
	@gcc -o fork fork.c fork.h

run:
	@./fork

clean:
	@rm -f fork