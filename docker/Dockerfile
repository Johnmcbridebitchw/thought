FROM debian:stretch
LABEL maintainer="Thought Developers <dev@thought.org>"
LABEL description="Dockerised ThoughtCore, built from Travis"

RUN apt-get update && apt-get -y upgrade && apt-get clean && rm -fr /var/cache/apt/*

COPY bin/* /usr/bin/
