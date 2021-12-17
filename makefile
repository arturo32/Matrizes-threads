# Compila o main.cpp
all:
	g++ -Wall -std=c++11 src/main.cpp -o main -pthread

# Apaga o arquivo executável
clean:
	rm -rf main