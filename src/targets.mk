build/strender: $(OBJECTS)
	@ $(MKBUILD)
	$(CC) $^ -o $@