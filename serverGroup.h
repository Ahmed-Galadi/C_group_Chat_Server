#ifndef SERVERGROUP_H
# define SERVERGROUP_H

// INCLUDES
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>
#include <sys/socket.h>
#include <sys/select.h>
#include <netinet/in.h>
#include <netdb.h>
#include <limits.h>
#include <stdbool.h>

// COLORS
// Reset
#define RESET       "\033[0m"
// Regular colors (foreground)
#define BLACK       "\033[30m"
#define RED         "\033[31m"
#define GREEN       "\033[32m"
#define YELLOW      "\033[33m"
#define BLUE        "\033[34m"
#define MAGENTA     "\033[35m"
#define CYAN        "\033[36m"
#define WHITE       "\033[37m"
// Bright colors (foreground)
#define BBLACK      "\033[90m"
#define BRED        "\033[91m"
#define BGREEN      "\033[92m"
#define BYELLOW     "\033[93m"
#define BBLUE       "\033[94m"
#define BMAGENTA    "\033[95m"
#define BCYAN       "\033[96m"
#define BWHITE      "\033[97m"
// Background colors
#define BG_BLACK    "\033[40m"
#define BG_RED      "\033[41m"
#define BG_GREEN    "\033[42m"
#define BG_YELLOW   "\033[43m"
#define BG_BLUE     "\033[44m"
#define BG_MAGENTA  "\033[45m"
#define BG_CYAN     "\033[46m"
#define BG_WHITE    "\033[47m"
// Bright background colors
#define BG_BBLACK   "\033[100m"
#define BG_BRED     "\033[101m"
#define BG_BGREEN   "\033[102m"
#define BG_BYELLOW  "\033[103m"
#define BG_BBLUE    "\033[104m"
#define BG_BMAGENTA "\033[105m"
#define BG_BCYAN    "\033[106m"
#define BG_BWHITE   "\033[107m"
// Styles
#define BOLD        "\033[1m"
#define DIM         "\033[2m"
#define UNDERLINE   "\033[4m"
#define BLINK       "\033[5m"
#define REVERSE     "\033[7m"
#define HIDDEN      "\033[8m"

// SERVER LIMITS
# define MAX_CLIENTS 256
# define BUFFER_SIZE 10024
# define CLIENT_ACCEPT_MSG "\n---------------------------------------------------\nserver: [ %s ] just joined\n---------------------------------------------------\n[you] => "
# define CLIENT_LEFT_MSG "\n---------------------------------------------------\nserver: [ %s ] just left\n---------------------------------------------------\n[you] => "
# define CLIENT_MSG "\n---------------------------------------------------\n[%s]: %s\n---------------------------------------------------\n[you] => "
# define MAX_NAME_SIZE 32
# define MAX_PASSWORD_SIZE 24
# define MAX_MEMBERS 50
# define MAX_ADMINS 3
# define MAX_CLIENT_GRP 5
# define GROUP_MAX 100

// STATE
enum e_state {WAITING_NAME, WAITING_PASS, LOGGED, INSIDE};

// STRUCTS
// GROUP STRUCT
typedef struct s_group {
	int grpId;
	char name[MAX_NAME_SIZE];
	int members_ids[MAX_MEMBERS];
	int admins_ids[MAX_ADMINS];
	int members_count;
	int admins_count;
	int msgs_count;
}	t_group;
// CLIENT STRUCT
typedef struct s_client {
	int id;
	int client_fd;
	char name[MAX_NAME_SIZE];
	char msg[BUFFER_SIZE];
	char password[MAX_PASSWORD_SIZE];
	enum e_state state;
	int groups_id[MAX_CLIENT_GRP];
	int groups_count;
	int curr_grpId;
	bool is_public;
} t_client;
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
void	client_auth(t_server *s, int client_fd, int bytes_read);
void	createGroup();

#endif
