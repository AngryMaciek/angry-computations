##### BASE IMAGE #####
FROM ubuntu:22.04

##### METADATA #####
LABEL base.image="ubuntu:22.04"
LABEL version="1.0.0"
LABEL software="efcomp"
LABEL software.description="Efficient computing in Python/R/C++"
LABEL software.website="https://github.com/AngryMaciek/efcomp"
LABEL software.license="GPL-3.0"
LABEL software.tags="C++, scientific-computing"
LABEL maintainer="Maciek Bak"
LABEL maintainer.email="wsciekly.maciek@gmail.com"

##### ENVIROMENTAL VARIABLES #####
ENV LANG C.UTF-8
ENV DEBIAN_FRONTEND noninteractive

##### COPY REPOSITORY #####
WORKDIR home/efcomp
COPY . .

##### SYSTEM PACKAGES #####
RUN apt update && apt upgrade -y \
  && apt install make cmake wget r-base python3-pip \
  libopenblas-dev liblapack-dev libeigen3-dev -y \
  && update-alternatives --install /usr/bin/python python /usr/bin/python3 1 \
  && apt autoremove -y \
  && apt clean -y \
  && rm -rf /var/lib/apt/lists/* /tmp/* /var/tmp/*

##### ARMA & CARMA LIBS #####
RUN \
  wget https://sourceforge.net/projects/arma/files/armadillo-12.4.0.tar.xz \
  && tar -xf armadillo-12.4.0.tar.xz \
  && cd armadillo-12.4.0 \
  && cmake . \
  && make \
  && make install \
  && cd .. \
  && rm -rf armadillo-12.4.0.tar.xz \
  && wget -O carma-v0.6.7.tar.gz https://github.com/RUrlus/carma/archive/refs/tags/v0.6.7.tar.gz \
  && tar -xzf carma-v0.6.7.tar.gz \
  && cd carma-0.6.7 \
  && mkdir build \
  && cd build \
  && cmake -DCARMA_INSTALL_LIB=ON .. \
  && cmake --build . --config Release --target install \
  && cd ../.. \
  && rm -rf carma-v0.6.7.tar.gz

##### PYTHON PACKAGES #####
RUN pip install numpy pybind11 jupyterlab bash_kernel python-language-server \
  && python -m bash_kernel.install \
  && jupyter lab --generate-config \
  && sed -i '$ a\c.ServerApp.allow_origin = "*"\nc.ServerApp.websocket_origin = "*"' /root/.jupyter/jupyter_lab_config.py

##### R PACKAGES #####
RUN R -e 'install.packages(c("Rcpp", "numDeriv", "RcppArmadillo", "RcppEigen"))'

##### EXPOSE PORTS #####
EXPOSE 8888


# Test compilation command:
#
# g++ -O2 -Wall
# -I/usr/include/eigen3/
# -I/home/efcomp/armadillo-12.4.0/include/
# -I/usr/local/include/carma/
# -I/usr/local/lib/python3.10/dist-packages/numpy/core/include/
# -shared -std=c++14 -fPIC `python3 -m pybind11 --includes` test.cpp
# -o functions`python3-config --extension-suffix` -llapack
