#ifndef SERVER_S_H
# define SERVER_S_H

// SERVER STRUCT
typedef struct s_server {
	char recv_buffer[BUFFER_SIZE];
	char write_buffer[BUFFER_SIZE];
	int max_fd;
	int clients_count;
	int client_fd;
	int server_socket;
	fd_set read_fds, write_fds, all_fds;
	t_client clients[MAX_CLIENTS];
	t_group Groups[GROUP_MAX];
	int groups_count;
	int msgs_count;
} t_server;

#endif