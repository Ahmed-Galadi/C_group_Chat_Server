#include "../../serverGroup.h"

void createGroup(t_server *s, t_client *client, char *groupName) {
    if (s->groups_count >= GROUP_MAX) {
        send_to_client(client->client_fd, "REJECTED: searver's MAX GROUPS is reached!\n");
        return ;
    }
    if (client->groups_count >= MAX_CLIENT_GRP) {
        send_to_client(client->client_fd, "REJECTED: searver's MAX CLIENT GRP is reached!\n");
        return ;
    }
    if (!groupName) {
        send_to_client(client->client_fd, "REJECTED: GROUP Name is Mandatory!\n");
        return ;
    }
    if (strlen(groupName) > MAX_NAME_SIZE) {
        send_to_client(client->client_fd, "REJECTED: GROUP Name is too long (32 max)\n");
        return ;
    }
    // check for is groupName exists;
    // create group
    s->Groups[s->groups_count].grpId = s->groups_count;
    strcpy(s->Groups[s->groups_count].name, groupName);
    s->Groups[s->groups_count].members_count = 1;
    s->Groups[s->groups_count].admins_count = 1;
    s->Groups[s->groups_count].admins_ids[0] = client->id;
    s->Groups[s->groups_count].members_ids[0] = client->id;
    s->Groups[s->groups_count].msgs_count = 0;
    // link client with group
    client->groups_id[client->groups_count] = s->Groups[s->groups_count].grpId;
    (s->groups_count)++;
    (client->groups_count)++;
}

bool is_member(t_server *s, t_client *c, char *groupName, bool *is_admin) {
    // step - 1
    for (int i = 0; i < c->groups_count - 1; i++) {
        t_group *grp = &s->Groups[i];
        if (!comp(grp->name, groupName) && isIn(c->id, grp->members_ids, grp->members_count)) {
            if (isIn(c->id, grp->admins_ids, grp->admins_count))
               *is_admin = true;
            c->curr_grpId = grp->grpId;
            c->state = INSIDE;
            send_to_client(c->client_fd, "ACCEPTED: You Are Now Inside ");
            return (true);
        }
    }
    return (false);
}

// check the fd if its in 
bool is_grpFD(t_server *s, int curr_grpID, int fd) {
    for (int i = 0; i < s->groups_count; i++) {
        if (s->Groups[i].grpId == curr_grpID) {
            t_group *grp = &s->Groups[i];
            for (int j = 0; j < grp->members_count; j++) {
                t_client *c = getClientFD(s, fd);
                if (grp->members_ids[i] == c->id && c->curr_grpId == curr_grpID) 
                    return (true);
            }
        }
    }
    return (false);
}

void send_to_group(t_server *s, t_client *c, char *msg) {
	for (int fd = 0; fd <= s->max_fd; fd++) {
		if (!FD_ISSET(fd, &s->all_fds))
			continue;
		if (fd == c->client_fd || fd == s->server_socket || s->clients[fd].state != INSIDE)
			continue;
        if (is_grpFD(s, c->curr_grpId, fd))
            send(fd, msg, strlen(msg), 0);
	}
}

void sendJoinedFeedback(t_server *s, t_client *c, bool is_admin) {
    if (is_admin) {
        snprintf(s->write_buffer, BUFFER_SIZE,
		    ADMIN_GRP_MSG, c->name);
        send_to_group(s, c, s->write_buffer);
    } else {
        snprintf(s->write_buffer, BUFFER_SIZE,
	    	MEMBER_GRP_MSG, c->name);
        send_to_group(s, c, s->write_buffer);
   }
}

void goto_group(t_server *s, t_client *client, char *groupName) {
    bool is_admin = false;
    if (client->state == LOGGED || client->state == INSIDE) {
        if (!groupName || !strlen(groupName)
            || strlen(groupName) < MAX_NAME_SIZE) {
            send_to_client(client->client_fd, "REJECTED: Invalid GROUP NAME!");
            return;
        }
        if (!is_member(s, client, groupName, &is_admin)) {
            send_to_client(client->client_fd, "REJECTED: You are NOT a Member!");
            return;
        }
        sendJoinedFeedback(s, client, is_admin);
    } else  exit_error("FATAL: CLIENT wanna go to group even tho its not logged to server !\n", s->max_fd);
}