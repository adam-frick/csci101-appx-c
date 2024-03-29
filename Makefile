src = $(wildcard *.c)
obj = $(src:.c=.o)
bin = appx_c

FLAGS = -Wall -std=c99
LDFLAGS = 

$(bin): $(obj)
	$(CC) -o $@ $^ $(FLAGS) $(LDFLAGS)
	rm -f $(obj)

.PHONY: clean
clean:
	rm -f $(obj) $(bin)
