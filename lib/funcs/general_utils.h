#ifndef GENERAL_UTILS_H
# define GENERAL_UTILS_H

// FUNCTIONS
// ------------------- Utils ---------------------
void	putstr(int fd, char *str);
void	exit_error(char *err_msg, int max_fd);
bool    comp(const char *s1, const char *s2);
bool    isIn(int id, int *ids, int ids_count);

#endif