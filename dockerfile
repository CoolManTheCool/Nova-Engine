# Dockerfile
FROM ubuntu:latest
RUN apt update && apt install -y wget gnupg libyaml-dev

RUN wget -qO - https://packages.lunarg.com/lunarg-signing-key-pub.asc | apt-key add -
RUN wget -qO /etc/apt/sources.list.d/lunarg-vulkan-1.3.290-noble.list https://packages.lunarg.com/vulkan/1.3.290/lunarg-vulkan-1.3.290-noble.list
RUN apt update
RUN apt install -y vulkan-sdk

RUN apt install -y \
    cmake \
    build-essential \
    libvulkan-dev \
    vulkan-tools \
    libglfw3 \
    libglfw3-dev \
    libglm-dev 

WORKDIR /project

COPY run.sh run.sh

RUN chmod +x ./run.sh

CMD ["./run.sh", "-compile", "-release"]
