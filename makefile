Warnings=-Wall -Wextra
ObjFiles=obj/ItemTree.o obj/Item.o obj/demo.o obj/Address.o
gtkflags=`pkg-config --cflags --libs gtk+-3.0` 
errflags=-Wall -Wextra 

all: bin/demo

bin/demo: obj/Item.o obj/Address.o obj/demo.o obj/ItemTree.o
	g++ ${gtkflags} ${Warnings} obj/Item.o obj/Address.o obj/demo.o obj/ItemTree.o -o $@

obj/ItemTree.o: src/ItemTree.cpp hdr/ItemTree.h hdr/Item.h hdr/Address.h
	g++ ${gtkflags} ${Warnings} -c $< -o $@

obj/Item.o: src/Item.cpp hdr/Item.h hdr/Address.h
	g++ ${gtkflags} ${Warnings} -c $< -o $@

obj/demo.o: src/demo.cpp hdr/demo.h hdr/Item.h hdr/Address.h hdr/ItemTree.h
	g++ ${gtkflags}  ${Warnings} -c $< -o $@

obj/Address.o: src/Address.cpp hdr/Address.h
	g++ ${gtkflags} ${Warnings} -c $< -o $@

.PHONY: clean
clean:
	rm -f bin/demo ${ObjFiles}

