FROM gcc:latest AS build-cpp

# Install zip for vcpkg
RUN apt-get update && \
    apt-get upgrade -y && \
    apt-get install zip -y && \
    apt-get autoremove -y && \
    apt-get autoclean

# Install CMake
RUN cd /tmp && \
    wget https://github.com/Kitware/CMake/releases/download/v3.20.3/cmake-3.20.3-linux-x86_64.sh && \
    chmod +x cmake-3.20.3-linux-x86_64.sh && \
    ./cmake-3.20.3-linux-x86_64.sh --prefix=/usr/local --skip-license && \
    rm cmake-3.20.3-linux-x86_64.sh

# Install vcpkg
RUN cd /tmp && \
    git clone https://github.com/microsoft/vcpkg && \
    ./vcpkg/bootstrap-vcpkg.sh -disableMetrics

# Install emscripten
RUN cd /tmp && \
    git clone https://github.com/emscripten-core/emsdk.git && \
    cd emsdk && \
    ./emsdk install latest && \
    ./emsdk activate latest

# Install dependencies
# Making emsdk_env.sh work in Docker requires too much boilerplate, so set manually :P
ENV EMSDK=/tmp/emsdk  
RUN /tmp/vcpkg/vcpkg install boost-asio \
        boost-beast \
        nlohmann-json \
        fmt \
        --triplet wasm32-emscripten

# Compile the client
COPY CMakeLists.txt /EspadaNegra/
COPY cmake /EspadaNegra/cmake
COPY src /EspadaNegra/src
WORKDIR /EspadaNegra
RUN mkdir -p build && \
    cd build && \
    cmake .. \
        -DENABLE_TESTING=OFF \
        -DCMAKE_BUILD_TYPE=Release \
        -DCMAKE_TOOLCHAIN_FILE=/tmp/vcpkg/scripts/buildsystems/vcpkg.cmake \
        -DVCPKG_CHAINLOAD_TOOLCHAIN_FILE=/tmp/emsdk/upstream/emscripten/cmake/Modules/Platform/Emscripten.cmake \
        -DVCPKG_TARGET_TRIPLET=wasm32-emscripten \
    && cmake --build . --target espadaNegraClient --


FROM node:latest as build-node
COPY ./client /client
COPY --from=build-cpp /EspadaNegra/build/src/espadaNegraClient.* /client/public/
WORKDIR /client
RUN npm install
RUN npm run build

FROM nginx:latest as production
COPY --from=build-node /client/dist /usr/share/nginx/html
# COPY docker/web/mime.types /etc/nginx
ENTRYPOINT ["nginx", "-g", "daemon off;"]
