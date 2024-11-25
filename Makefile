
# compile main.c menu_handler.c menu_handler.h into one app
# -g for more debugging infos
# -o for setting a name for the output
app: src/main.c src/menu_handler.c src/line_operation_handler.c src/file_operation_handler.c
	gcc -g -o app src/main.c src/menu_handler.c src/line_operation_handler.c src/file_operation_handler.c -lncurses -lmenu

# to clean the leftover
clean: 
	rm -f src/*.o app