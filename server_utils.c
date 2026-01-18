#include "serverGroup.h"

t_server *server_init(int port) {
	struct sockaddr_in serverAddr;
	bzero(&serverAddr, sizeof(serverAddr));

	t_server *output = malloc(sizeof(t_server));
	if (!output) exit_error(NULL, 2);
	serverAddr.sin_addr.s_addr = htonl(127 << 24 | 0 << 16 | 0 << 8 | 1);
	serverAddr.sin_family = AF_INET;
	serverAddr.sin_port = htons(port);
	
	output->server_socket = socket(AF_INET, SOCK_STREAM, 0);
	if (output->server_socket == -1)
		exit_error(NULL, 2);

	output->max_fd = output->server_socket;
	output->ids_count = 0;
	FD_ZERO(&(output->all_fds));
	FD_SET(output->server_socket, &(output->all_fds));

	if (bind(output->server_socket,(const struct sockaddr *)&serverAddr, sizeof(serverAddr)) == -1)
		exit_error(NULL, output->max_fd);
	if (listen(output->server_socket, SOMAXCONN) == -1)
		exit_error(NULL, output->max_fd);
	return (output);
}

int accept_client(t_server *server) {
	server->client_fd = accept(server->server_socket, 0, 0);
	if (server->client_fd < 0)
		return (0);
	if (server->max_fd < server->client_fd)
		server->max_fd = server->client_fd;
	FD_SET(server->client_fd, &(server->all_fds));
	server->clients[server->client_fd].id = (server->ids_count)++;
	sprintf(server->write_buffer, CLIENT_ACCEPT_MSG, server->client_fd);
	send_to_all(server, server->client_fd);
	return (1);
}

void	send_to_all(t_server *server, int client_fd) {
	for (int fd = 0; fd <= server->max_fd; fd++)
		if (FD_ISSET(fd, &(server->write_fds)) && fd != client_fd)
			if (send(fd, server->write_buffer, strlen(server->write_buffer), 0) == -1)
				continue;
}

void client_left(t_server *server, int client_fd) {
	sprintf(server->write_buffer, CLIENT_LEFT_MSG, client_fd);
	send_to_all(server, client_fd);
	FD_CLR(client_fd, &(server->all_fds));
	close(client_fd);
	bzero(server->clients[client_fd].msg, BUFFER_SIZE);
}

void read_and_broadcast(t_server *server, int current_fd) {
	int bytes_arrived = recv(current_fd, server->recv_buffer, BUFFER_SIZE, 0);
	if (bytes_arrived <= 0)
		client_left(server, current_fd);
	else {
		for (int i = 0, j = strlen(server->clients[current_fd].msg); i < bytes_arrived; i++, j++) {
			server->clients[current_fd].msg[j] = server->recv_buffer[i];
			if (server->clients[current_fd].msg[j] == '\n') {
				server->clients[current_fd].msg[j] = 0;
				sprintf(server->write_buffer, CLIENT_MSG, current_fd, server->clients[current_fd].msg);
				send_to_all(server, current_fd);
				j = -1;
				bzero(server->clients[current_fd].msg, BUFFER_SIZE);
			}
		}
	}
}
