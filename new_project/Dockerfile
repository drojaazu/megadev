# Megadev dev container: the kit, cloned into MEGADEV_PATH, and the Megadev
# toolchain, built from the same branch with tools/build-toolchain.sh (which
# takes a while the first time). The toolchain's bin directory is on PATH, so
# the kit's default M68K_PREFIX (m68k-elf-) finds it. .devcontainer.json passes
# MEGADEV_BRANCH.

ARG OS_VERSION=13
ARG MEGADEV_REPO=https://github.com/drojaazu/megadev.git
ARG MEGADEV_BRANCH=master
ARG TOOLCHAIN_PATH=/opt/megadev-toolchain

FROM debian:${OS_VERSION}-slim AS base
RUN \
	apt-get update -y && \
	apt-get install -y \
		git \
		make \
		libgmp10 \
		libmpfr6 \
		libmpc3 \
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

FROM base AS toolchain

ARG MEGADEV_REPO
ARG MEGADEV_BRANCH
ARG TOOLCHAIN_PATH

RUN \
	apt-get update -y && \
	apt-get install -y \
		build-essential \
		curl \
		xz-utils \
		libgmp-dev \
		libmpfr-dev \
		libmpc-dev \
		&& \
	git clone --depth 1 --branch ${MEGADEV_BRANCH} ${MEGADEV_REPO} /tmp/megadev && \
	/tmp/megadev/tools/build-toolchain.sh ${TOOLCHAIN_PATH}

FROM base AS devcontainer

ARG MEGADEV_REPO
ARG MEGADEV_BRANCH
ARG TOOLCHAIN_PATH
ARG MEGADEV_PATH=/opt/megadev

COPY --from=toolchain ${TOOLCHAIN_PATH} ${TOOLCHAIN_PATH}

RUN \
	useradd -ms /bin/bash megadev && \
	git clone --branch ${MEGADEV_BRANCH} ${MEGADEV_REPO} ${MEGADEV_PATH} && \
	chown -R megadev:megadev ${MEGADEV_PATH} && \
	printf "    __  __________________    ____  _______    __\n   /  |/  / ____/ ____/   |  / __ \\/ ____/ |  / /\n  / /|_/ / __/ / / __/ /| | / / / / __/  | | / / \n / /  / / /___/ /_/ / ___ |/ /_/ / /___  | |/ /  \n/_/  /_/_____/\\____/_/  |_/_____/_____/  |___/   \n    Sega Mega Drive/Mega CD development kit\n\n" > /etc/motd \
		&& \
	echo "unset PROMPT_COMMAND" >> /home/megadev/.bashrc && \
	echo "PS1='\[\033[01;34m\]\w\[\033[00m\] # \[\]'" >> /home/megadev/.bashrc && \
	echo "cat /etc/motd" >> /home/megadev/.bashrc

USER megadev
WORKDIR /home/megadev

ENV PATH=${TOOLCHAIN_PATH}/bin:${PATH}
ENV MEGADEV_PATH=${MEGADEV_PATH}

CMD ["bash"]
