app_dir     := $(shell pwd)
lib_dir     := $(app_dir)/../lib

lib_src     := _endpoint.c _logging.c
client_src  := $(addprefix $(lib_dir)/, $(lib_src))
client_src  += client.c

server_src  := $(addprefix $(lib_dir)/, $(lib_src))
server_src  += server.c

cflags      := -g -std=c99 -Wall -I $(lib_dir) -Wno-implicit-function-declaration 
targets     := server client

all: $(targets)

client : $(client_src)
	gcc $^ -o $@ $(cflags) -lpthread

server : $(server_src)
	gcc $^ -o $@ $(cflags)

clean:
	rm -f $(targets) *.o

.PHONY :clean
