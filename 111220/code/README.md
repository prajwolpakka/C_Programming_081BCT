# How to play this game

## Compiling the game

use the following command to compile the code

```cmd
gcc MainGame.c -o MainGame.exe -I include/ -L lib/ -lraylib -lopengl32 -lgdi32 -lwinmm
```

then run the game using

```cmd
./MainGame.exe
```

## Compile using make

Make sure you have gnu make install on you device
Open a terminal and type

```cmd
make --version
```

if make is installed then the version number should be echoed in the terminal

```cmd
GNU Make 4.4.1
```

if u don't have gnu make installed install it then type the following in the terminal

### Windows

```cmd
winget install ezwinports.make
```

### Mac

```brew
brew install make
```

### Linux

```cmd
sudo apt install make
```

To compile and run the game simply type

```cmd
make
```
