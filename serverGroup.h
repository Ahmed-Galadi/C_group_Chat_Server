#ifndef SERVERGROUP_H
# define SERVERGROUP_H

#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>
#include <sys/socket.h>
#include <sys/select.h>
#include <netinet/in.h>
#include <netdb.h>
#include <limits.h>

# define MAX_CLIENTS 1024
# define BUFFER_SIZE 1000000
# define CLIENT_ACCEPT_MSG "server: client %d just arrived\n"
# define CLIENT_LEFT_MSG "server: client %d just left\n"
# define CLIENT_MSG "client %d: %s\n"

typedef struct s_client {
	int id;
	char msg[BUFFER_SIZE];
} t_client;

typedef struct s_server {
	char recv_buffer[BUFFER_SIZE];
	char write_buffer[BUFFER_SIZE];
	int max_fd;
	int ids_count;
	int client_fd;
	int server_socket;
	fd_set read_fds, write_fds, all_fds;
	t_client clients[MAX_CLIENTS];
} t_server;

// ------------------- Utils ---------------------

void	putstr(int fd, char *str);
void	exit_error(char *err_msg, int max_fd);

// ------------------- Server functions ----------

t_server *server_init(int port);
void send_to_all(t_server *server, int client_fd);
void read_and_broadcast(t_server *server, int fd);
int  accept_client(t_server *server);
void client_left(t_server *server, int client_fd);
#endif
