VERILATOR_ARGS = --cc --exe -j 0 --trace -I./def -I./mod -I./test_mod

mod_srcs = $(wildcard mod/*.v) $(wildcard test_mod/*.v)
mod_names = $(basename $(notdir $(mod_srcs)))
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
# (always verilate all modules, since verilatoo_available_itemr generates make dependency files)
verilate: $(mod_srcs)
	@for path in $(mod_srcs) ; do \
		filename=$${path##*/} ; \
		name=$${filename%%.*} ; \
		verilator $(VERILATOR_ARGS) test/tb_$$name.cpp $$path >/dev/null ; \
	done


# clean all artifacts
clean:
	rm -rf obj_dir logs

.PHONY: run verilate build clean
