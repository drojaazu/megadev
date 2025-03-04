FROM debian:12-slim
RUN apt-get update -y
RUN apt-get install -y git make binutils-m68k-linux-gnu cpp-m68k-linux-gnu gcc-m68k-linux-gnu genisoimage
RUN useradd -ms /bin/bash megadev

RUN echo "    __  __________________    ____  _______    __\n   /  |/  / ____/ ____/   |  / __ \\/ ____/ |  / /\n  / /|_/ / __/ / / __/ /| | / / / / __/  | | / / \n / /  / / /___/ /_/ / ___ |/ /_/ / /___  | |/ /  \n/_/  /_/_____/\\____/_/  |_/_____/_____/  |___/   \n    Sega Mega Drive/Mega CD development kit\n" > /etc/motd
RUN echo "cat /etc/motd\nuname -a\n" >> /etc/bash.bashrc

WORKDIR /opt
RUN git clone https://github.com/drojaazu/megadev


CMD ["/bin/bash"]

USER megadev
WORKDIR /home/megadev
