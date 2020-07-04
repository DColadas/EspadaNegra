# Espada Negra
The Espada Negra online game.

Based on the real [*Espada Negra: juego de mesa*](https://espadanegra.net/jdm.php) (you can [download](http://espadanegra.net/descargar.php?d=7) the printable deck!).

## Requirements
- `C++17` compiler.
  - Tested with `gcc 7.5.0`.
- At least `CMake 3.11`.
- At least [`Boost 1.66`](https://www.boost.org/users/download/) (needs the `Asio`, `Beast` and `Property Tree` libraries).
- [`Catch2`](https://github.com/catchorg/Catch2). There is a CMake script that downloads it automatically if it is not found.

## Compile
```
mkdir build
cd build
cmake ..
make
```
You can add your number of available threads to the command `make` to make the compilation faster, like `make -j8` (if you have 8 threads).

## Folder structure
- `client`: the web client that connects to the server to play the game.
- `cmake`: relevant CMake scripts and utilities to keep the root `CMakeLists.txt` more modular and clean. 
- `src`: the main C++ game server.
  - `Events`: defines the event hierarchies (`InputEvent` and `OutputEvent`) -classes with information about valid received player actions and output game state updates-.
  - `Game`: main game logic.
  - `IO`: main server. Manages client connections to it and the existing matches. 
  - `Logging`: simple module used to log relevant messages. Apart from the usual logging levels (`Error`, `Info`, `Debug` and `Trace` -a more in-depth debug level-) it also defines `Panic`, which aborts the program whenever an impossible critical state is reached.
  - `Parsing`: utilities to serialize and deserialize the events (`Events` module) to and from JSON format in order to communicate with the client.
  - `Utils`: helper libraries with useful functions use wherever in the rest of the codebase (random number generation, time libraries, text coloring...).
- `test`: relevant unit tests for the rest of the code.
