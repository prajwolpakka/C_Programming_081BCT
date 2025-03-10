# How to play this game

## Compiling the game

use the following command to compile the code

```
gcc MainGame.c -o MainGame.exe -I include/ -L lib/ -lraylib -lopengl32 -lgdi32 -lwinmm
```

then run the game using 

```
./MainGame.exe
```

## Compile using make

Make sure you have gnu make install on you device by typing

```
make --version
```
if u don't have gnu make installed install it

### Windows
```
winget install ezwinports.make
```

### Mac
```
brew install make
```

### Linux
```
sudo apt install make
```

To compile and run the game simply type
```
make
```

