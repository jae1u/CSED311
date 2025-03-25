VERILATOR_ARGS = -Wall --cc --exe -j 0 --trace -I./def -I./mod -I./test_mod

# mod_srcs = $(wildcard mod/*.v) $(wildcard test_mod/*.v)
# mod_names = $(basename $(notdir $(mod_srcs)))
# tb_srcs = $(patsubst %,test/tb_%.cpp,$(mod_names))
# tb_exes = $(patsubst %,obj_dir/V%,$(mod_names))

mod_srcs = $(wildcard mod/*.v) $(wildcard test_mod/*.v)
tb_srcs = $(wildcard test/tb_*.cpp)
mod_names = $(patsubst tb_%,%,$(basename $(notdir $(tb_srcs))))
tb_exes = $(patsubst %,obj_dir/V%,$(mod_names))



# ===== RULES =====


# run all testbenches
run: build
	@fail=0; \
	for exe in $(tb_exes); do \
		$$exe || fail=1; \
	done; \
	exit $$fail

# build all testbenches
build: verilate
	@fail=0; \
	for tb in $(notdir $(tb_exes)) ; do \
		echo "building $$tb..." ; \
		make -j -C obj_dir -f $$tb.mk >/dev/null || fail=1; \
	done; \
	exit $$fail

# verilate all modules
# (always verilate all modules, since verilator generates make dependency files)
verilate: $(mod_srcs)
	@fail=0; \
	for path in $(mod_srcs) ; do \
		filename=$${path##*/} ; \
		name=$${filename%%.*} ; \
		verilator $(VERILATOR_ARGS) test/tb_$$name.cpp $$path >/dev/null || fail=1; \
	done; \
	exit $$fail

# clean all artifacts
clean:
	rm -rf obj_dir logs

.PHONY: run verilate build clean
