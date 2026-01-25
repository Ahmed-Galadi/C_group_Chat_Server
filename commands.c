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

void goto_group(t_server *s, t_client *client, char *groupName) {
    /*
        - is there any group with that name in the clients [groups_id].
        - check the admin list if the client is in the [admin_ids].
        - check the [clients_id] if not found in [admin_ids].
        - when found put the [grpId] in clients [curr_grpId].
        - if not found write to the client a feedback error message.
        - if client is admin write its name in defferent color when inside group.
    */
}