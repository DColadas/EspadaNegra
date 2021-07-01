FROM gcc:latest

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

# Install dependencies
RUN /tmp/vcpkg/vcpkg install boost-asio \
    boost-beast \
    nlohmann-json \
    fmt

# Compile the server
COPY CMakeLists.txt /EspadaNegra/
COPY cmake /EspadaNegra/cmake
COPY src /EspadaNegra/src
WORKDIR /EspadaNegra
RUN mkdir -p build && \
    cd build && \
    cmake .. -DENABLE_TESTING=OFF \
        -DCMAKE_BUILD_TYPE=Release \
        -DCMAKE_TOOLCHAIN_FILE=/tmp/vcpkg/scripts/buildsystems/vcpkg.cmake && \
    cmake --build . --target espadaNegraServer --

# Run it
ENTRYPOINT /EspadaNegra/build/src/espadaNegraServer 0.0.0.0 ${PORT_SERVER:-8081}
