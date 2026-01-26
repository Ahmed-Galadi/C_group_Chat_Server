#ifndef SRV_UTILS_H
# define SRV_UTILS_H

// FUNCTIONS
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
// -------------------- Auth -------------------------
void	client_auth(t_server *s, int client_fd, int bytes_read);
// ---------------- groups -----------------------
void	createGroup();
// --------------- client ----------------------
// ---------------- admin ----------------------

#endif