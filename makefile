build:
	g++ main.cpp --std=c++17 -I./headers/. -o main -lutil -lboost_iostreams -lboost_system -lboost_filesystem

clean:
	rm main
