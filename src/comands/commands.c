#include "serverGroup.h"

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
    client->groups_ids[client->groups_count] = s->Groups[s->groups_count].grpId;
    (s->groups_count)++;
    (client->groups_count)++;
}

bool is_member(t_server *s, t_client *c) {
    
}

void goto_group(t_server *s, t_client *client, char *groupName) {
    // TO DO
    // step - 0
    if (!groupName || !strlen(groupName)
        || strlen(groupName) < MAX_NAME_SIZE) {
        send_to_client(client->client_fd, "REJECTED: Invalid GROUP NAME!");
        return;
    }
    // step - 1 => step -2 => step - 3
    // -> make is_member
    if (!is_member(s, client, groupName)) {
        send_to_client(client->client_fd, "REJECTED: You are NOT a Member!");
        return;
    }
    // step - 4
    setColorMsg(s, client);
    // step - 5 => step - 6
    sendJoinedFeedback(s, client);
    // safety check
}