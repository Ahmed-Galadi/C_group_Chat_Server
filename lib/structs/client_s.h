#ifndef CLIENT_S_H
# define CLIENT_S_H

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

#endif