PARSING_SRCS := Settings.cpp Server.cpp Route.cpp
SOCKET_SRCS := Client.cpp Request.cpp Response.cpp CGI.cpp
SRCS		:=	main.cpp Print.cpp $(addprefix parsing/,$(PARSING_SRCS)) $(addprefix socket/,$(SOCKET_SRCS))
OBJS		:=	${SRCS:.cpp=.o} 

CC			:= c++
CXXFLAGS	:= -Wall -Wextra -Werror -I. -std=c++98  -ggdb3 -fsanitize=address
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