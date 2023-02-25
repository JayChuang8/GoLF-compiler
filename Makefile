CC := g++
SUBDIRS = Scanner Utility Parser
OBJS := main.o

all: golf

golf: $(OBJS)
	$(CC) -o $@ $^ $(wildcard $(addsuffix /*.o,$(SUBDIRS)))

-include $(OBJS:.o=.d)

main.o: main.cpp $(SUBDIRS)
	$(CC) -o $@ -c $< -MMD -MF $*.d

.PHONY: $(SUBDIRS)
$(SUBDIRS):
	$(MAKE) -C $@

clean:
	rm -f *.o *.out *.d