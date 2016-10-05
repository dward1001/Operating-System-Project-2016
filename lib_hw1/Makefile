TARGET = bitmap hash list

PROGRAM = testlib

build: build.obj
	gcc -c main.c -o $(OBJS_DIR)/main.o
	gcc -o $(PROGRAM) $(OBJS_DIR)/*.o

build.obj : $(addsuffix .o, $(TARGET))


include rules.mk
