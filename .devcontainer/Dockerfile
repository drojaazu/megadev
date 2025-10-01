FROM alpine:latest AS base
RUN \
	apk add --no-cache \
		alpine-sdk \
		git \
		bash \
		wget \
		clang16-extra-tools \
		xxd \
		xz \
		cdrkit

FROM base AS build

ARG RELEASE_BINUTILS=binutils-2.45
ARG RELEASE_GCC=gcc-15.2.0

WORKDIR /build
RUN \
	wget https://ftpmirror.gnu.org/gnu/binutils/${RELEASE_BINUTILS}.tar.xz && \
	wget https://ftpmirror.gnu.org/gnu/gcc/${RELEASE_GCC}/${RELEASE_GCC}.tar.xz && \
	tar -xf ${RELEASE_BINUTILS}.tar.xz && \
	tar -xf ${RELEASE_GCC}.tar.xz && \
	cd ${RELEASE_GCC} && \
	./contrib/download_prerequisites

WORKDIR /build/binutils
RUN \
	../${RELEASE_BINUTILS}/configure --prefix=/usr --program-prefix=m68k-linux-gnu- --target=m68k-elf --disable-nls --disable-multilib --enable-lto && \
	make -j $(nproc --all) && \
	make install

WORKDIR /build/gcc
RUN \
	../${RELEASE_GCC}/configure --prefix=/usr --program-prefix=m68k-linux-gnu- --target=m68k-elf --with-arch=m68k --with-cpu=m68000 --enable-languages=c --disable-nls --disable-multilib --disable-libssp --enable-lto --without-headers && \
	make -j $(nproc --all) && \
	make install

FROM build AS dev_container

ARG MEGADEV_PATH=/opt/megadev

RUN \
	adduser -D megadev && \
	git clone https://github.com/drojaazu/megadev.git ${MEGADEV_PATH} && \
	chown -R megadev:megadev ${MEGADEV_PATH} && \
	printf "    __  __________________    ____  _______    __\n   /  |/  / ____/ ____/   |  / __ \\/ ____/ |  / /\n  / /|_/ / __/ / / __/ /| | / / / / __/  | | / / \n / /  / / /___/ /_/ / ___ |/ /_/ / /___  | |/ /  \n/_/  /_/_____/\\____/_/  |_/_____/_____/  |___/   \n    Sega Mega Drive/Mega CD development kit\n" > /etc/motd \
		&& \
	echo "cat /etc/motd\n" >> /etc/bash/bashrc

USER megadev
WORKDIR /home/megadev

ENV CC=/usr/bin/m68k-linux-gnu-gcc
ENV M68K_PREFIX=m68k-linux-gnu-
ENV MEGADEV_PATH=${MEGADEV_PATH}

CMD ["/bin/sh"]
