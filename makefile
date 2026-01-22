BINS = leakcount mem_shim.so testprogram

all: $(BINS)

%.so: %.c
	$(CC) $(CFLAGS) -shared -fPIC -o $@ $^ -ldl
	
%.out: %.c
	$(CC) $(CFLAGS) -o $@ $^
	
clean:
	rm -f $(BINS)
