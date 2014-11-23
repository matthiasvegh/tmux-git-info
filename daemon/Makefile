
all: tmuxinfod

clean:
	rm -rf *.o tmuxinfod

tmuxinfod: main.cpp Context.hpp ConfigReader/ConfigReader.hpp
	clang++ --std=c++11 main.cpp -o tmuxinfod

