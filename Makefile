NAME = serverGroup
SRCS = src/server/serverGroup.c src/server/server_utils.c src/comands/comands.c src/auth/auth.c utils/utils.c
HDRS = lib/colors/colors_d.h lib/colors/styles_d.h lib/funcs/general_utils.h lib/funcs/srv_utils.h \
lib/server/limits_d.h lib/server/msg_d.h lib/structs/s_defines.h lib/structs/s_funcs.h lib/structs/s_structs.h \
serverGroup.h
OBJS = $(SRCS:.c=.o)
CC = cc
CFLAGS = -Wall -Wextra -Werror 

$(NAME): $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) -o $(NAME)

all: $(NAME)

%.o: %.c $(HDRS)
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJS)

fclean: clean
	rm -f $(NAME)

re: fclean all