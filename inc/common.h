#ifndef COMMON_H
#define COMMON_H

#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<fcntl.h>
#include<sys/types.h>
#include<sys/stat.h>
#include<string.h>

// Buoc 1: Dia chi co dinh cua Server (Well-known address)
#define SERVER_FIFO "/tmp/fifo_server"

// Template de tao FIFO rieng tu cho tung Client dua tren PID
#define CLIENT_FIFO_TEMPLATE "/tmp/fifo_client_%d"

#define MAX_DATA_SIZE 256

// Buoc 4: Ky thuat dong goi ban tin (Message Boundary)
// Chua Header (client_pid, data_length) va Payload (data)

typedef struct {
	pid_t client_pid;	// ID cua tien trinh Client
	int data_length;	// Kich thuoc cua du lieu thuc te
	char data[MAX_DATA_SIZE]; // Du lieu thuc te
}Message;


#endif
