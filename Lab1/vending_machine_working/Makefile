VERILATOR_ARGS = --cc --exe -j 0 --trace -I./def -I./mod

mod_srcs = $(wildcard mod/*.v)
mod_names = $(patsubst mod/%.v,%,$(mod_srcs))
tb_srcs = $(patsubst %,test/tb_%.cpp,$(mod_names))
tb_exes = $(patsubst %,obj_dir/V%,$(mod_names))


# ===== RULES =====


# run all testbenches
run: build
	@for exe in $(tb_exes) ; do \
		$$exe ; \
	done


# build all testbenches
build: verilate
	@for tb in $(notdir $(tb_exes)) ; do \
		echo "building $$tb..." ; \
		make -j -C obj_dir -f $$tb.mk >/dev/null ; \
	done

# verilate all modules
# (always verilate all modules, since verilator generates make dependency files)
verilate: $(mod_srcs)
	@for m in $(mod_names) ; do \
		src=mod/$$m.v ; \
		verilator $(VERILATOR_ARGS) test/tb_$$m.cpp $$src >/dev/null ; \
	done


# clean all artifacts
clean:
	rm -rf obj_dir logs

.PHONY: run verilate build clean
