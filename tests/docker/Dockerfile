FROM ubuntu:16.04
MAINTAINER Tim Hawes tim@selfdidactic.com

RUN apt-get update
RUN apt-get -y install build-essential libsword-dev libreadline-dev cmake git
VOLUME /usr/share/sword/modules
VOLUME /usr/local/src
RUN echo "[Install]\n\
DataPath=/usr/local/src/tests/swordtest/sword\n\
" >/etc/sword.conf