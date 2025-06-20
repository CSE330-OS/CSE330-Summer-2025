test-read: test-read.c kmod_primitives.c dd_primitives.c
	gcc $^ -o $@

test-read-variable: test-read-variable.c kmod_primitives.c dd_primitives.c
	gcc $^ -o $@

test-write: test-write.c kmod_primitives.c dd_primitives.c
	gcc $^ -o $@

test-write-variable: test-write-variable.c kmod_primitives.c dd_primitives.c
	gcc $^ -o $@

clean:
	rm -rf test-read test-write \
	test-read-variable test-write-variable