OBJS = test.o apply_line.o

test: $(OBJS)
	ld $(OBJS) -o $@

%.o: %.asm
	nasm -f elf $<

clean:
	rm $(OBJS)
