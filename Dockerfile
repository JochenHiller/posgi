FROM ubuntu:23.10

LABEL org.opencontainers.image.authors="jo.hiller@gmail.com"


# Install needed tools
# https://stackoverflow.com/questions/26333823/clang-doesnt-see-basic-headers
RUN apt-get update \
  && apt-get install -y \
    locales \
    git vim emacs sudo nano \
    apt-utils dialog zsh \
    iproute2 procps lsb-release \
    iputils-ping net-tools \
    libncurses5 \
    bash-completion \
    wget curl telnet \
    python3-full python3-venv \
    build-essential clang clang-format clang-tidy lldb gcc g++ cmake cppcheck \
  && python3 -m venv /opt/python/venv \
  && /opt/python/venv/bin/pip3 install cmakelang cpplint \
  && rm -rf /var/lib/apt/lists/*

# TODO(jhi): include-what-you-use missing

ENV PATH="/opt/python/venv/bin:$PATH"

WORKDIR /app

CMD [ "/bin/bash" ]
