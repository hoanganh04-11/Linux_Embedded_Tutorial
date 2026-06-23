# Trình biên dịch
CC = gcc
CFLAGS = -Iinc -Wall

# Tên file thực thi
SERVER_BIN = bin/server
CLIENT_BIN = bin/client

# Target mặc định: Build cả 2 chương trình
all: setup $(SERVER_BIN) $(CLIENT_BIN)

# Đảm bảo các thư mục obj và bin tồn tại trước khi build
setup:
	mkdir -p obj bin

# Compile Server
$(SERVER_BIN): obj/server.o
	$(CC) obj/server.o -o $(SERVER_BIN)

obj/server.o: src/server.c
	$(CC) $(CFLAGS) -c src/server.c -o obj/server.o

# Compile Client
$(CLIENT_BIN): obj/client.o
	$(CC) obj/client.o -o $(CLIENT_BIN)

obj/client.o: src/client.c
	$(CC) $(CFLAGS) -c src/client.c -o obj/client.o

# Dọn dẹp
clean:
	rm -rf obj/* bin/* /tmp/fifo_*
