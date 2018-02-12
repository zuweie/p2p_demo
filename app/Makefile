app_dir     := $(shell pwd)
lib_dir     := $(app_dir)/../lib
inc_dir     := $(app_dir)/../inc
src_dir     := $(app_dir)/../src

src         := _endpoint.c _logging.c _msghandler.c _peer_manager.c cJSON.c 
client_src  := $(addprefix $(src_dir)/, $(src))
client_src  += client.c 

server_src  := $(addprefix $(src_dir)/, $(src))
server_src  += server.c 

cflags      := -g -std=c99 -Wall -I$(inc_dir) -L$(lib_dir) -lalg -Wno-implicit-function-declaration 
targets     := server client

all: $(targets)

client : $(client_src)
	gcc $^ -o $@ $(cflags) -lpthread

server : $(server_src)
	gcc $^ -o $@ $(cflags)

clean:
	rm -fr $(targets) *.o *.dSYM 

.PHONY :clean
