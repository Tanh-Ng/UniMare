all:
	g++ -I D:\SDL64\include -L D:\SDL64\lib -o main  main.cpp -lmingw32 -lSDL2main -lSDL2 -lSDL2_image