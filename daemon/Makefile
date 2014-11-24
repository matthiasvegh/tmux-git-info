
all: tmuxinfod

clean:
	rm -rf *.o tmuxinfod

tmuxinfod: main.cpp Context.hpp ConfigReader/ConfigReader.hpp
	clang++ --std=c++1y main.cpp -o tmuxinfod

