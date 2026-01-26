# Goto Group Implementation Roadmap

## Step 0 — Validate input

* Check if `groupName` is not NULL
* Optional: check length (`strlen(groupName) < MAX_NAME_SIZE`)

## Step 1 — Search for the group in client’s list

* Loop over `client->groups_ids` from `0` → `client->groups_count - 1`
* For each `grpId`, get `t_group *grp = &s->Groups[grpId]`
* Compare `grp->name` with `groupName`
* If found → store `grpId` and break
* If not found after the loop → send error to client (`"You are not a member of this group\n"`) and return

## Step 2 — Check membership / admin status

* Get the group using the found `grpId`
* Check if client is in `grp->admins_ids` or `grp->members_ids`
* If client is **not in either list** → send error to client and return

## Step 3 — Update client’s current group

* Set `client->curr_grpId = grpId`
* Optional: update any UI prompt like `[you] => `

## Step 4 — Handle admin display

* If client is in `admins_ids` → can add a different color for name when sending messages
* Otherwise → normal color

## Step 5 — Feedback to client

* Send message to client confirming group join
  Example: `"You joined group <groupName>\n[you] => "`

## Step 6 — Optional improvements

* Send a **"joined" message to other members** of the group
* Automatically **leave previous group** if `client->curr_grpId` was set
* Update any group-specific state, like last active timestamp or unread messages

## Step 7 — Safety checks

* Always check array bounds when looping `groups_ids`, `members_ids`, `admins_ids`
* Avoid using uninitialized variables
* Ensure ANSI color codes do not corrupt the message buffer

