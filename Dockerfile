FROM verilator/verilator:latest

ENV DEBIAN_FRONTEND=noninteractive
RUN apt update && apt upgrade -y
RUN apt install -y git vim nano

RUN apt install -y openjdk-11-jdk unzip
WORKDIR /tmp
RUN git clone https://github.com/ericsonj/verilog-format.git && \
    cd verilog-format/bin/ && \
    mkdir -p /opt/verilog-format && \
    unzip verilog-format-LINUX.zip -d /opt/verilog-format/ && \
    cp /opt/verilog-format/verilog-format /usr/bin/ && \
    rm -rf /tmp/verilog-format

RUN apt install -y openssh-server sudo
EXPOSE 22

ENTRYPOINT ["/bin/bash"]
