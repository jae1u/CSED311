FROM verilator/verilator:latest

ENV DEBIAN_FRONTEND=noninteractive
RUN apt update && apt upgrade -y
RUN apt install -y git vim nano

RUN apt install -y openssh-server sudo
EXPOSE 22

ENTRYPOINT ["/bin/bash"]
