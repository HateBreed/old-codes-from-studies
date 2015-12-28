
SOURCES = project.c drawobj.c

have_file := $(wildcard project.exe)
ifeq ($(strip $(have_file)),)
OUT = project
else
OUT = project.exe
endif

all:
	@echo "---------------Define target---------------------"
	@echo "make              this help"
	@echo "make cygwin       build for cygwin under windows"
	@echo "make linux        build for Linux"
	@echo "make run          run program"
	@echo "make clean        remove build (clean)"
	@echo "-------------------------------------------------"

cygwin:
	gcc $(SOURCES) -L/usr/X11R6/lib -lGL -lopengl32 -lglu32 -lglut32 -o project.exe
	
linux:
	gcc $(SOURCES) -lm -lGL -lglut -lGLU -D LINUX -o project
run:
	./$(OUT)
	
clean:
	rm -v $(OUT)
	
