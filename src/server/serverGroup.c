#include "../../serverGroup.h"

static void print_client_data(t_server *s) {
	for (int i = 0; i <= s->max_fd; i++) {
		if (s->clients[i].name[0] != '\0' && s->clients[i].password[0] != '\0') {
			t_client client = s->clients[i];
			char *is_logged = client.state == LOGGED || client.state == INSIDE ? "true" : "false";
			printf("{\n\tid: %d\n\tname: %s\n\tpassword: %s\n\tis_logged: %s\n}\n", client.id, client.name, client.password, is_logged);
		}
	}
}

int main(int argc, char *argv[]) {
	if (argc != 2) exit_error("Invalid Number of Arguments: Enter Port Number!\n", 2);

	t_server *server = server_init(atoi(argv[1]));
	int clients_count = server->clients_count;
	while (true) {
		server->write_fds = server->read_fds = server->all_fds;
		if (select(server->max_fd + 1,
			&(server->read_fds),
			&(server->write_fds), 0, 0) < 0)
			continue;
		for (int fd = 0; fd <= server->max_fd; fd++) {
			if (!FD_ISSET(fd, &(server->read_fds))) continue;
			if (fd == server->server_socket)
    			accept_client(server);
			else
    			recv_client_data(server, fd);
			if (server->clients_count > clients_count) {
				print_client_data(server);
				clients_count = server->clients_count;
			}
		}
	}
	return (0);
}


