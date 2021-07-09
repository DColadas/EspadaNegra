# Espada Negra
The Espada Negra online game (still unplayable!).

Based on the real [*Espada Negra: juego de mesa*](https://espadanegra.net/jdm.php) (you can [download](http://espadanegra.net/descargar.php?d=7) the printable deck!).

## Building 
### With Docker
Just run `docker-compose up` at the root of the project! This will create two containers --one serving the frontend, the other one deploying the Websocket server--. 

Once the build is up and running, access the web interface at `http://localhost:8080`. You can change the ports used by the containers by switching the values in `.env`.

### Locally
The project is still under development --during my free time :P-- and I'm constantly experimenting and changing the build process, so I won't bother writing a detailed description; just mimic the commands in the Dockerfiles if you want to develop locally. 

In general lines, the required technologies are the following:

- `C++20` compiler.
  - Tested with `gcc 11.1`, `clang 12.0`.
- At least `CMake 3.18`.
- `vcpkg`.
- `emscripten`.
- `Vue.js 2.6.11`.
