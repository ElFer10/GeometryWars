main: main.cpp
	g++ --std=c++17 -w ./*.cpp -I/usr/local/Cellar/sfml/2.6.1/include -I. -o app -L/usr/local/Cellar/sfml/2.6.1/lib -lsfml-graphics -lsfml-window -lsfml-system && ./app

clean:
	rm -rf ./app
