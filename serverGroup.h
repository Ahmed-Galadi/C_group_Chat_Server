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
# define CLIENT_ACCEPT_MSG "\n---------------------------------------------------\nserver: [ %s ] just joined\n---------------------------------------------------\n[you] => "
# define CLIENT_LEFT_MSG "\n---------------------------------------------------\nserver: [ %s ] just left\n---------------------------------------------------\n[you] => "
# define CLIENT_MSG "\n---------------------------------------------------\n[%s]: %s\n---------------------------------------------------\n[you] => "
# define MAX_NAME_SIZE 32
# define MAX_PASSWORD_SIZE 24

enum loggin_state {WAITING_NAME, WAITING_PASS, LOGGED};

typedef struct s_client {
	int id;
	int client_fd;
	char name[MAX_NAME_SIZE];
	char msg[BUFFER_SIZE];
	char password[MAX_PASSWORD_SIZE];
	enum loggin_state state;
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
bool recv_client_data(t_server *server, int current_fd);
void	client_auth(t_server *s, int client_fd, int bytes_read);

#endif
