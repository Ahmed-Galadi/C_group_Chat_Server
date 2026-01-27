#include "../../serverGroup.h"

bool accept_client(t_server *s)
{
	int fd = accept(s->server_socket, NULL, NULL);
	if (fd < 0)
		return (false);
	if (fd >= MAX_CLIENTS) {
		close(fd);
		return false;
	}
	FD_SET(fd, &s->all_fds);
	if (fd > s->max_fd)
		s->max_fd = fd;

	s->clients[fd].client_fd = fd;
	s->clients[fd].name[0] = '\0';
	s->clients[fd].password[0] = '\0';
	s->clients[fd].msg[0] = '\0';
	s->clients[fd].state = WAITING_NAME;
	s->clients[fd].groups_count = 0;
	s->clients[fd].curr_grpId = -1;

	send_to_client(fd, "Enter Name:\n> ");
	return (true);
}

void	client_auth(t_server *s, int client_fd, int bytes_read) {
		if (s->clients[client_fd].state == WAITING_NAME) {
			int j = 0;
			for (; j < bytes_read && s->recv_buffer[j] != '\n' && j < MAX_NAME_SIZE - 1; j++)
				s->clients[client_fd].name[j] = s->recv_buffer[j];
			s->clients[client_fd].name[j] = '\0';
			s->clients[client_fd].state = WAITING_PASS;
			send_to_client(client_fd, "Enter Password:\n> ");
			return ;
		}
		if (s->clients[client_fd].state == WAITING_PASS) {
			int i = 0;
			for (; i < bytes_read && s->recv_buffer[i] != '\n' && i < MAX_PASSWORD_SIZE - 1; i++)
				s->clients[client_fd].password[i] = s->recv_buffer[i];
			s->clients[client_fd].password[i] = '\0';
			
			if (s->clients[client_fd].name[0] && s->clients[client_fd].password[0]) {
				s->clients[client_fd].state = LOGGED;
				s->clients[client_fd].id = s->clients_count++;
				snprintf(s->write_buffer, BUFFER_SIZE, CLIENT_ACCEPT_MSG, s->clients[client_fd].name);
				send_to_all(s, client_fd, s->write_buffer);
				send_to_client(client_fd, "\033[2J\033[H\n[you] => ");
			}
			return ;
		}
}