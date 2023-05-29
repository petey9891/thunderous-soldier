#/bin/bash

git pull

docker build -t thunderous-soldier .

docker-compose up -d

docker logs thunderous-soldier