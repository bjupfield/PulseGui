ie = bin
dump = $(wildcard $(ie))

fileI = $(wildcard *.c)
fileP = $(notdir $(fileI))
files = $(basename $(fileP))

filesO = $(addprefix $(ie)/, $(addsuffix .o, $(files)))

libs = -lX11 -lGLInterface -lGL -L../glInterface/bin



main : 
ifneq ($(dump),bin)
	mkdir bin
endif
	-gcc -c swcDef.h -o $(ie)/swcDef.o $(libs)
	-for file in $(files); do \
		gcc -c $$file.c -o $(ie)/$$file.o $(libs); \
	done
	ar rc $(ie)/libSWCWindow.a $(filesO) $(ie)/swcDef.o



.PHONY: clean
clean : 
	$(MAKE) main
	
	-for file in $(files); do \
		rm $(ie)/$$file.o; \
	done
	rm $(ie)/swcDef.o
