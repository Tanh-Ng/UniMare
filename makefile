all:
	g++ -I D:\SDL64\include -L D:\SDL64\lib -o main  main.cpp Global.cpp Graphics.cpp Player.cpp GameObject.cpp Enemy.cpp -lmingw32 -lSDL2main -lSDL2 -lSDL2_image -lSDL2_mixer -lSDL2_ttf