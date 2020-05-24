program  = main.cpp
executable = main
headers = -I./headers/.
flags = --std=c++17
libraries = -lutil -lboost_iostreams -lboost_system -lboost_filesystem


build:
	g++ $(program) $(flags) $(headers) -o $(executable) $(libraries)
	@echo "Running Program\n---------------\n"

	@./$(executable)

clean:
	rm $(executable)
