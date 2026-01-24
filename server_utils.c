#include "serverGroup.h"


t_server *server_init(int port)
{
	struct sockaddr_in addr;
	t_server *s;

	bzero(&addr, sizeof(addr));
	addr.sin_family = AF_INET;
	addr.sin_addr.s_addr = INADDR_ANY;
	addr.sin_port = htons(port);

	s = malloc(sizeof(t_server));
	if (!s)
		exit_error(NULL, 2);
	bzero(s, sizeof(t_server)); 
	s->server_socket = socket(AF_INET, SOCK_STREAM, 0);
	if (s->server_socket < 0)
		exit_error(NULL, 2);

	if (bind(s->server_socket, (struct sockaddr *)&addr, sizeof(addr)) < 0)
		exit_error(NULL, s->server_socket);
	if (listen(s->server_socket, SOMAXCONN) < 0)
		exit_error(NULL, s->server_socket);

	FD_ZERO(&s->all_fds);
	FD_SET(s->server_socket, &s->all_fds);

	s->max_fd = s->server_socket;
	s->clients_count = 0;
	s->msgs_count = 0;
	s->groups_count = 0;
	return (s);
}





void send_to_all(t_server *s, int sender_fd, char *msg)
{
	for (int fd = 0; fd <= s->max_fd; fd++)
	{
		if (!FD_ISSET(fd, &s->all_fds))
			continue;
		if (fd == sender_fd || fd == s->server_socket || s->clients[fd].state != LOGGED)
			continue;
		send_to_client(fd, msg);
	}
}



void send_to_client(int fd, char *msg) {
    send(fd, msg, strlen(msg), 0);
}



bool recv_client_data(t_server *s, int fd)
{
	int n = recv(fd, s->recv_buffer, BUFFER_SIZE - 1, 0);
	if (n <= 0) {
		client_left(s, fd);
		return (false);
	}
	s->recv_buffer[n] = '\0';

	// ---- LOGIN PHASE ----
	if (s->clients[fd].state == WAITING_NAME || s->clients[fd].state == WAITING_PASS)
		return (client_auth(s, fd, n), true);
	// ---- CHAT PHASE ---- 
	if (s->clients[fd].state == LOGGED) {
		for (int i = 0; i < n; i++) {
			int j = strlen(s->clients[fd].msg);
			s->clients[fd].msg[j] = s->recv_buffer[i];
			s->clients[fd].msg[j + 1] = '\0';

			if (s->recv_buffer[i] == '\n')
			{
				snprintf(s->write_buffer, BUFFER_SIZE,
					CLIENT_MSG,
					s->clients[fd].name,
					s->clients[fd].msg);

				send_to_all(s, fd, s->write_buffer);
				(s->msgs_count)++;
				s->clients[fd].msg[0] = '\0';
			}
		}
	}
	return (true);
}



void client_left(t_server *s, int fd)
{
	if (s->clients[fd].name[0])
	{
		snprintf(s->write_buffer, BUFFER_SIZE,
			CLIENT_LEFT_MSG, s->clients[fd].name);
		send_to_all(s, fd, s->write_buffer);
	}

	FD_CLR(fd, &s->all_fds);
	close(fd);
	s->clients[fd].name[0] = '\0';
	s->clients[fd].msg[0] = '\0';
	s->clients[fd].msg[0] = '\0';
	s->clients[fd].state = WAITING_NAME;
}
