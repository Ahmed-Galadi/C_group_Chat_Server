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
#include <stdbool.h>

# define MAX_CLIENTS 1024
# define BUFFER_SIZE 10024
# define CLIENT_ACCEPT_MSG "server: [ %s ] just arrived\n"
# define CLIENT_LEFT_MSG "server: [ %s ] just left\n"
# define CLIENT_MSG "client %d: %s\n"
# define MAX_NAME_SIZE 32
# define MAX_PASSWORD_SIZE 24

enum recv_flags {BROADCAST_MSG, DATA_RECV_NAME, DATA_RECV_PASS};

typedef struct s_client {
	int id;
	int client_fd;
	char name[MAX_NAME_SIZE];
	char msg[BUFFER_SIZE];
	char password[MAX_PASSWORD_SIZE];
	bool is_logged;
} t_client;

typedef struct s_server {
	char recv_buffer[BUFFER_SIZE];
	char write_buffer[BUFFER_SIZE];
	int max_fd;
	int clients_count;
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
void send_to_all(t_server *s, int sender_fd, char *msg);
bool  accept_client(t_server *server);
void client_left(t_server *server, int client_fd);
void send_to_client(int fd, char *msg);
bool recv_client_data(t_server *server, int current_fd, enum recv_flags flag);

#endif
