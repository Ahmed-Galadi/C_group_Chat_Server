#ifndef SERVERGROUP_H
# define SERVERGROUP_H

// STATE
enum e_state {WAITING_NAME, WAITING_PASS, LOGGED, INSIDE};

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

// SERVER INCLUDES
#include "lib/s_defines.h"
#include "lib/s_structs.h"
#include "lib/s_funcs.h"

#endif