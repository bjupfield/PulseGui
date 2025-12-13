binH = bin
bin = $(wildcard bin)
name = swcGui
tester = test
testerH = $(wildcard test)

#Remove x11 lib, it will not be needed, once we are done with swcWindow
#? what is the above line on about?
libs = -lX11 -lGL -IswcWindow -LswcWindow/bin -lSWCWindow -lxml2


main:
ifneq ($(bin), $(binH))
	mkdir $(binH)
endif
#	$(MAKE) --directory=glInterface clean
	$(MAKE) --directory=swcWindow clean
	gcc -o $(name) window.c $(libs) 

.PHONY: run
run :
	$(MAKE) main
	./$(name)

debug :
ifneq ($(bin), $(binH))
	mkdir $(binH)
endif
#	$(MAKE) --directory=glInterface debug
	$(MAKE) --directory=swcWindow debug
	gcc -g -o $(name) window.c $(libs)

test :
	$(MAKE) --directory=swcWindow clean
	gcc -o $(tester) window.c $(libs)

clean :
	$(MAKE) main
ifeq ($(tester), $(testerH))
	rm $(tester)
endif