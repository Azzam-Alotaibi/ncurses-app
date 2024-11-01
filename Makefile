
# compile main.c menu_handler.c into app
app: main.c menu_handler.c
	gcc -o app main.c menu_handler.c -lncurses -lmenu