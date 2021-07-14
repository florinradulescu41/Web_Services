# Protocoale de comunicatii:
# Tema3
# Makefile

CFLAGS = -Wall -g

PORT =

IP_SERVER =

all: client

# Compileaza client.cpp
client: client.cpp

.PHONY: clean run_client

# Ruleaza client
run_client:
	./client ${IP_SERVER} ${PORT}

clean:
	rm -f client
