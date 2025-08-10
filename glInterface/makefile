ie = bin
dump = $(wildcard $(ie))

fileI = $(wildcard *.c)
fileP = $(notdir $(fileI))
files = $(basename $(fileP))

filesO = $(addprefix $(ie)/, $(addsuffix .o, $(files)))

libs = -lX11 -lGL

main :
ifneq ($(dump),$(ie))
	mkdir $(ie)
endif
	for file in $(files); do \
		gcc -c $$file.c -o $(ie)/$$file.o; \
	done
	ar rc $(ie)/libGLInterface.a $(filesO)


.PHONY: clean
clean : 
	$(MAKE) main
	
	-for file in $(files); do \
		rm $(ie)/$$file.o; \
	done

debug :
ifneq ($(dump),$(ie))
	mkdir $(ie)
endif
	for file in $(files); do \
		gcc -g -c $$file.c -o $(ie)/$$file.o; \
	done
	ar rc $(ie)/libGLInterface.a $(filesO)
