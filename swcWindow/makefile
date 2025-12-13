ie = bin
dump = $(wildcard $(ie))

foldersI = $(wildcard *)
folders = $(filter-out $(wildcard *.*),$(foldersI))
folder = $(filter-out makefile,$(folders))
folder += $() $()


fileI = $(foreach fold,$(folder),$(wildcard $(fold)/*.c)) $(wildcard *.c)
files = $(basename $(fileI))

filesO = $(addprefix $(ie)/, $(addsuffix .o, $(notdir $(files))))

libs = -lX11 -lGL
directory = -I/usr/include/libxml2



main : 
ifneq ($(dump),bin)
	mkdir bin
endif
	-gcc -c swcDef.h -o $(ie)/swcDef.o $(directory) $(libs)
	-for file in $(files); do \
		noDir="$${file##*/}"; \
		gcc -c $$file.c -o $(ie)/$$noDir.o $(directory) $(libs); \
	done
	ar rc $(ie)/libSWCWindow.a $(filesO) $(ie)/swcDef.o



.PHONY: clean
clean : 
	$(MAKE) main
	
	-for file in $(filesO); do \
		rm $$file; \
	done
	rm $(ie)/swcDef.o

debug : 
	-gcc -g -c swcDef.h -o $(ie)/swcDef.o $(directory) $(libs)
	-for file in $(files); do \
		noDir="$${file##*/}"; \
		gcc -g -c $$file.c -o $(ie)/$$noDir.o $(directory) $(libs); \
	done
	ar rc $(ie)/libSWCWindow.a $(filesO) $(ie)/swcDef.o
