CC := gcc
OBJS := a.o b.o

all: a.out

a.out: $(OBJS)
	$(CC) -o $@ $^

-include $(OBJS:.o=.d)

%.o: %.c
	$(CC) -o $@ -c $< -MMD -MF $*.d

clean:
	rm -f *.o *.out *.d

