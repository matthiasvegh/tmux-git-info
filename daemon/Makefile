
all: tmuxinfod unitTest

clean:
	rm -rf *.o tmuxinfod unitTest

tmuxinfod: main.cpp Context.hpp ConfigReader/ConfigReader.hpp SharedMap.hpp
	clang++ --std=c++1y main.cpp -o tmuxinfod

unitTest: unitTestMain.cpp SharedMap.hpp
	clang++ --std=c++1y unitTestMain.cpp -lboost_unit_test_framework -lrt -o unitTest
