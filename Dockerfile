FROM henne90gen/opengl:3.1

RUN apt-get update && apt-get install -y ca-certificates

RUN echo "#!/usr/bin/env bash \n\
mkdir -p build \n\
cd build || exit 0 \n\
cmake .. -G Ninja -D CMAKE_BUILD_TYPE=Release -D CMAKE_C_COMPILER=/usr/bin/clang-13 -D CMAKE_CXX_COMPILER=/usr/bin/clang++-13\n\
cmake --build . RacingToHell\n" > /build.sh

RUN chmod +x /build.sh
CMD ["/build.sh"]
