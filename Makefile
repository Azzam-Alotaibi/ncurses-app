
# compile main.c menu_handler.c menu_handler.h into one app
# -g for more debugging infos
# -o for setting a name for the output
app: main.c menu_handler.c menu_handler.h 
	gcc -g -o app main.c menu_handler.c menu_handler.h -lncurses -lmenu

# to clean the leftover
clean: 
	rm -f *.o app