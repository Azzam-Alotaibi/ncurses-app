
# compile main.c window_handler.c window_handler.h into one app
# -g for more debugging infos
# -o for setting a name for the output
app: src/main.c src/window_handler.c src/line_operation_handler.c src/file_operation_handler.c src/errors.c
	gcc -g -o app src/main.c src/window_handler.c src/line_operation_handler.c src/file_operation_handler.c src/errors.c -lncurses -lmenu

# to clean the leftover
clean: 
	rm -f src/*.o app