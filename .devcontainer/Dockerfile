ARG OS_VERSION=12

FROM debian:${OS_VERSION}-slim AS base
RUN \
	apt-get update -y && \
	apt-get install -y \
		git \
		make \
		binutils-m68k-linux-gnu \
		cpp-m68k-linux-gnu \
		gcc-m68k-linux-gnu \
		genisoimage \
		&& \
	apt-get clean && \
	rm -rf /var/lib/apt/lists/*

FROM base AS devcontainer

ARG MEGADEV_PATH=/opt/megadev

RUN \
	useradd -ms /bin/bash megadev && \
	git clone https://github.com/drojaazu/megadev.git ${MEGADEV_PATH} && \
	chown -R megadev:megadev ${MEGADEV_PATH} && \
	echo "    __  __________________    ____  _______    __\n   /  |/  / ____/ ____/   |  / __ \\/ ____/ |  / /\n  / /|_/ / __/ / / __/ /| | / / / / __/  | | / / \n / /  / / /___/ /_/ / ___ |/ /_/ / /___  | |/ /  \n/_/  /_/_____/\\____/_/  |_/_____/_____/  |___/   \n    Sega Mega Drive/Mega CD development kit\n" > /etc/motd && \
	echo "cat /etc/motd\nuname -a\n" >> /etc/bash.bashrc

USER megadev
WORKDIR /home/megadev

ENV CC=/usr/bin/m68k-linux-gnu-gcc
ENV M68K_PREFIX=m68k-linux-gnu-
ENV MEGADEV_PATH=${MEGADEV_PATH}

CMD ["bash"]
