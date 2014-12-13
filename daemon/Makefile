
all: tmuxinfod tmuxinfo unitTest testClient

clean:
	rm -rf *.o tmuxinfod unitTest testClient

tmuxinfo: tmuxinfo.cpp SharedMap.hpp
	clang++ --std=c++1y tmuxinfo.cpp -lrt -o tmuxinfo

tmuxinfod: main.cpp Context.hpp ConfigReader/ConfigReader.hpp SharedMap.hpp
	clang++ --std=c++1y main.cpp -lrt -o tmuxinfod

unitTest: unitTestMain.cpp SharedMap.hpp
	clang++ --std=c++1y unitTestMain.cpp -lboost_unit_test_framework -lrt -o unitTest

testClient: SharedMapTestClient.cpp SharedMap.hpp
	clang++ --std=c++1y SharedMapTestClient.cpp -lrt -o testClient
