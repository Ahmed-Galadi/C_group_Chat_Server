#ifndef GROUP_S_H
# define GROUP_S_H

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

#endif