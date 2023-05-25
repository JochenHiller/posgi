#!/usr/bin/env bash

CONTAINER_NAME=dev-container-posgi

echo "dev-container-start.sh"

# build docker image
docker build -f Dockerfile -t ${CONTAINER_NAME} .

# run docker, map current dir into container at /app
# map ports 8000/9000 from container to host
docker run -it -v "$(pwd)":/app ${CONTAINER_NAME} /bin/bash

