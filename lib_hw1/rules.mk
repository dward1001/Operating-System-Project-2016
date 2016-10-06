OBJS_DIR = objs

.PHONY: clean
clean:
	@rm -rf $(OBJS_DIR) $(PROGRAM)
	@echo "Cleaned."

$(addsuffix .o, $(TARGET)): %.o: .mkdir.o
	gcc -c $(basename $@).c -o $(OBJS_DIR)/$(basename $@).o

.PHONY: .mkdir.o
.mkdir.o:
	@mkdir -p $(OBJS_DIR)/
