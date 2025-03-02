#!/bin/bash

docker_name="csed311"
port="1557"
hostname="CSED311"
username="csed311"
password="csed311"

docker rm -f $docker_name
docker build -t $docker_name .
docker run -itd -v $(pwd):/work --name $docker_name --hostname $hostname -p $port:22 $docker_name

docker exec -it $docker_name useradd --create-home --uid $(id -u) --shell /bin/bash --groups sudo $username
docker exec -it $docker_name /bin/bash -c "echo '$username:$password' | chpasswd"
docker exec -it $docker_name service ssh start
