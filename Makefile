SRCS		:=	
OBJS		:=	${SRCS:.cpp=.o}

CC			:= c++
CXXFLAGS	:= -Werror -Wextra -Wall -I. -std=c++98
RM			:= rm -f

NAME		:= webserv

all:		$(NAME)

$(NAME):	$(OBJS)
			$(CC) $(CXXFLAGS) $(OBJS) -o $(NAME)

clean:		
			$(RM) $(OBJS) 

fclean:		clean
			$(RM) $(NAME)

re:			fclean $(NAME)

.PHONY:		all clean fclean re