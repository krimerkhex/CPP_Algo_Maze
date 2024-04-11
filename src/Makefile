all: clean clean_tests uninstall dvi install tests dist

install: clean uninstall
	bash bash/install.sh
	mv Maze/Maze.app Maze.app

uninstall: clean
	rm -rf Maze.app

clean_tests:
	rm -rf test/*.o
	rm -rf test/s21_test
	rm -rf test/googletest-release-1.11.0

clean: clean_tests
	rm -rf build-*
	bash bash/clean.sh

tests: 
	g++ -lstdc++ -g -std=c++17 -Wall -Werror -Wextra -lgtest tests/s21_test.cc Maze/model/model.cc -o tests/s21_test
	./tests/s21_test

dist: install
	rm -rf Maze.tar.gz
	tar -zcf Maze.tar.gz Maze.app

dvi:
	open info/dvi.html
