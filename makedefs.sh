#! /bin/sh

# gather all .def files
# make rule entry for each based on the files inside
shopt -s nullglob
defpath="$1/*.def"
for def in $defpath; do
	echo "HERE: $def"
	type=$(echo "$def" | grep -Eo '\.[binsmd]{3}\.def$')
	src=$(grep --color=never -e .s$ -e .c$ "$def")
	rmod=$(grep --color=never -e .mmd$ -e .smd$ -e .bin$ "$def")
	mod=$(basename -s .def "$def")
	echo -n "\$(DISC_PATH)/$mod: "
	for f in $src; do
		echo -n "$f.o "
	done
	echo -en "\n"
	if [ $type = ".mmd.def" ]; then
		cfg="module_mmd.ld"
	elif [ $type = ".smd.def" ]; then
		cfg="module_smd.ld"
	elif [ $type = ".bin.def" ]; then
		cfg="module_bin.ld"
	fi

	echo -ne "\t\$(call link_module, \$^, \$@, \$(CFG_PATH)/$cfg"
	if [ ! -z $rmod ]; then
		echo -n ", $rmod"
	fi
	echo ")"
	echo
done
