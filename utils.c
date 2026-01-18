#include "serverGroup.h"

void putstr(int fd, char *str) {
	while (*str) write(fd, str++, 1);
}

void	exit_error(char *err_msg, int max_fd) {
	if (!err_msg) putstr(2, "Error: Fatal Error!");
	else putstr(2, err_msg);
	for (int fd = 0; fd <= max_fd; fd++)
		close(fd);
	exit(EXIT_FAILURE);
}
