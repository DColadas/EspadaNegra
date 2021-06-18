FROM gcc:latest

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
RUN /tmp/vcpkg/vcpkg install catch2 \
    boost-asio \
    boost-beast \
    nlohmann-json \
    fmt

# Copy project code and compile it
COPY . /EspadaNegra
WORKDIR /EspadaNegra
RUN mkdir -p build && \
    cd build && \
    cmake .. -DCMAKE_TOOLCHAIN_FILE=/tmp/vcpkg/scripts/buildsystems/vcpkg.cmake && \
    cmake --build . --target espadaNegra --

# Run it
ENTRYPOINT /EspadaNegra/build/src/espadaNegra 0.0.0.0 ${PORT:-8081}
