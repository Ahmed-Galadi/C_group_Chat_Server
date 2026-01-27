#include "../serverGroup.h"

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

bool comp(const char *s1, const char *s2) {
	if (!s1 || !s2 || strlen(s1) != strlen(s2)) return (false);
	if (strncmp(s1, s2, strlen(s1)) == 0) return (true);
	return (false);
}

bool isIn(int id, int *ids, int ids_count) {
	for (int i = 0; i < ids_count; i++)
		if (ids[i] == id) return (true);
	return (false);
}