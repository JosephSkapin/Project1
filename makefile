BINS = leakcount mem_shim.so sctracer 

all: $(BINS)

%.so: %.c
	$(CC) $(CFLAGS) -shared -fPIC -o $@ $^ -ldl
	
%.out: %.c
	$(CC) $(CFLAGS) -o $@ $^
	
clean:
	rm -f $(BINS)
