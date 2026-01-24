#include "serverGroup.h"

void createGroup(t_server *s, t_client *client, char *groupName) {
    if (!groupName) {
        send_to_client(client->client_fd, "REJECTED: GROUP Name is Mandatory!\n");
        return ;
    }

    int name_len = strlen(groupName);
    if (name_len > MAX_NAME_SIZE) {
        send_to_client(client->client_fd, "REJECTED: GROUP Name is too long (32 max)\n");
        return ;
    }
    // create group
    s->Groups[s->groups_count].grpId = s->groups_count;
    strcpy(s->Groups[s->groups_count].name, groupName);
    s->Groups[s->groups_count].members_count = 0;
    s->Groups[s->groups_count].admins_count = 1;
    s->Groups[s->groups_count].admins_ids[0] = client->id;
    s->Groups[s->groups_count].msgs_count = 0;
    // link client with group
    client->groups_ids[client->groups_count] = s->Groups[s->groups_count].grpId;
    (s->groups_count)++;
    (client->groups_count)++;
}