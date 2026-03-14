ARG OS_VERSION=13

FROM debian:${OS_VERSION}-slim AS base
RUN \
	apt-get update -y && \
	apt-get install -y \
		git \
		make \
		binutils-m68k-linux-gnu \
		gcc-m68k-linux-gnu \
		libnewlib-dev \
		genisoimage \
		xxd \
		clang-format-19 \
		&& \
	apt-get clean && \
	rm -rf /var/lib/apt/lists/* && \
	update-alternatives \
				--install   /usr/bin/clang-format          clang-format          /usr/bin/clang-format-19 100 && \
	update-alternatives \
				--install   /usr/bin/clang-format-diff     clang-format-diff     /usr/bin/clang-format-diff-19 100

FROM base AS devcontainer

ARG MEGADEV_PATH=/opt/megadev

RUN \
	useradd -ms /bin/bash megadev && \
	git clone https://github.com/drojaazu/megadev.git ${MEGADEV_PATH} && \
	chown -R megadev:megadev ${MEGADEV_PATH} && \
	printf "    __  __________________    ____  _______    __\n   /  |/  / ____/ ____/   |  / __ \\/ ____/ |  / /\n  / /|_/ / __/ / / __/ /| | / / / / __/  | | / / \n / /  / / /___/ /_/ / ___ |/ /_/ / /___  | |/ /  \n/_/  /_/_____/\\____/_/  |_/_____/_____/  |___/   \n    Sega Mega Drive/Mega CD development kit\n" > /etc/motd \
		&& \
	echo "unset PROMPT_COMMAND" >> /home/megadev/.bashrc && \
	echo "PS1='\[\033[01;34m\]\w\[\033[00m\] # \[\]'" >> /home/megadev/.bashrc && \
	echo "cat /etc/motd" >> /home/megadev/.bashrc

USER megadev
WORKDIR /home/megadev

ENV CC=/usr/bin/m68k-linux-gnu-gcc
ENV M68K_PREFIX=m68k-linux-gnu-
ENV MEGADEV_PATH=${MEGADEV_PATH}

CMD ["bash"]
