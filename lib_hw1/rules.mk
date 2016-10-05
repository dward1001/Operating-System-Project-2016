OBJS_DIR = objs

$(addsuffix .o, $(TARGET)): %.o: .mkdir.o
	gcc -c $(basename $@).c -o $(OBJS_DIR)/$(basename $@).o

.PHONY: .mkdir.o
.mkdir.o:
	@mkdir -p $(OBJS_DIR)/
