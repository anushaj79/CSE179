# -Wall is a flag to show all warnings
# -Iinclude is a flag to specify the include directory for header files
cflags = -Wall -Iinclude 
src = $(wildcard src/*.c) # locate all .c files in the src directory
obj = $(src:src/%.c=obj/%.o) # convert src/filename.c to obj/filename.o, hide object files in an obj directory
target = learn_makefile

# default rule to build the target executable
$(target): $(obj)
	$(CC) -o $@ $^

# rule to compile .c files to .o files, create obj directory if it doesn't exist
obj/%.o: src/%.c
	mkdir -p obj
	$(CC) $(cflags) -c $< -o $@

# clean up source and object dir after typing make clean
.PHONY: clean
clean:
	rm -rf obj $(target)