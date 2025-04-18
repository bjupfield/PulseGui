binH = bin
bin = $(wildcard bin)

#Remove x11 lib, it will not be needed, once we are done with swcWindow
libs = -lX11 -lGL -IglInterface -LglInterface/bin -lGLInterface -IswcWindow -LswcWindow/bin -lSWCWindow

main:
ifneq ($(bin), $(binH))
	mkdir $(binH)
endif
	$(MAKE) --directory=glInterface clean
	$(MAKE) --directory=swcWindow clean
	gcc -o window window.c $(libs)
	