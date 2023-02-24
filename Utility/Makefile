CC := g++
OBJS = utility.o

all: $(OBJS)

-include $(OBJS:.o=.d)

%.o: %.cpp
	$(CC) -o $@ -c $< -MMD -MF $*.d

clean:
	rm -f *.o *.out *.d