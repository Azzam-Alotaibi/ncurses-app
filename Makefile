
# compile main.c/window_handler.c//line_operation_handler.c/file_operation_handler.c/errors.c into one app
# -g for more debugging infos
# -o for setting a name for the output
app: src/main.c src/window_handler.c src/line_operation_handler.c src/file_operation_handler.c src/errors.c
	gcc -o app src/main.c src/window_handler.c src/line_operation_handler.c src/file_operation_handler.c src/errors.c -lncurses -lmenu

# to clean the leftover
clean: 
	rm -f src/*.o app