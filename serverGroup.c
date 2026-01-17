#include "serverGroup.h"

int main(int argc, char *argv[]) {
	if (argc != 2) exit_error("Invalid Number of Arguments: Enter Port Number!\n", 2);

	t_server *server = server_init(atoi(argv[1]));
	while (1) {
		server->write_fds = server->read_fds = server->all_fds;
		if (select(server->max_fd + 1,
			&(server->read_fds),
			&(server->write_fds), 0, 0) < 0)
			continue;
		for (int fd = 0; fd <= server->max_fd; fd++) {
			if (!FD_ISSET(fd, &(server->read_fds))) continue;
			if (fd == server->server_socket)
				if (!accept_client(server)) continue;
		}
	}
	return (0);
}


