proj_dir=./
obj_dir=$(proj_dir)obj/
src_dir=./

#add required library here.
required_library=gstreamer-0.10 gtk+-2.0

#exclude files or dirs by path."" is needed when using a regular expression
exclude_path=

#exclude files or dirs by its name."" is needed when using a regular expression
exclude_name=doc 
include_suffix="*.c" "*.cpp"

#the target`s name must be the same as the file with main function.
target=Frequency
target_main_src_suffix=.cpp

cpp=g++
cc=gcc
ld=g++
CFLAGS=-c -Wall `pkg-config --cflags $(required_library)` 

LFLAGS=
libs= `pkg-config --libs $(required_library)`

#usually,you need not change the statements below

###################################################################################
override CFLAGS += -I./

exclude_path_str=$(foreach str,$(exclude_path), -path $(str) -o )

exclude_name_str_=$(foreach str,$(exclude_name), -name $(str) -o )
exclude_name_str=$(wordlist 1,$(shell expr $(words $(exclude_name_str_)) - 1),$(exclude_name_str_))

include_suffix_str_=$(foreach str,$(include_suffix), -name $(str) -o )
include_suffix_str=$(wordlist 1,$(shell expr $(words $(include_suffix_str_)) - 1),$(include_suffix_str_))

dir_src_file=$(shell find ./ \( \( $(exclude_path_str) $(exclude_name_str) \) -prune -o \( $(include_suffix_str) \) -type f -print \) )

src_file=$(foreach obj,$(dir_src_file),$(subst ./,, $(obj)))

obj_file=$(patsubst %, $(obj_dir)%o, $(src_file))

all:$(target)

$(target):%:$(obj_file) $(obj_dir)%$(target_main_src_suffix)o
	$(ld) $^ $(LFLAGS) $(libs) -o $@

$(obj_dir)%o:$(src_dir)%
	@mkdir -p $(obj_dir)$(dir $<)
	$(cpp) $(CFLAGS) $< -o $@

.PHONY:clean
clean:
	rm -f $(target) core
	rm -fR $(obj_dir)*

