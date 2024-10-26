# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: sdemaude <sdemaude@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2024/10/22 13:41:02 by sdemaude          #+#    #+#              #
#    Updated: 2024/10/26 13:09:50 by sdemaude         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME		= ircserv
CXX 		= c++
CXXFLAGS 	= -Wall -Wextra -Werror -g -std=c++98

SRCDIR		= srcs
SRCFILE		= main\
			  Server\
			  Client\
			  Channel\
			  command\
			  mode

OBJDIR		= objs
OBJS 		= $(addprefix $(OBJDIR)/,$(addsuffix .o,$(SRCFILE)))

all: $(OBJDIR) $(NAME)

$(OBJDIR):
	mkdir $(OBJDIR)

$(NAME): $(OBJS)
	$(CXX) $(CXXFLAGS) -o $(NAME) $(OBJS)

$(OBJDIR)/%.o: $(SRCDIR)/%.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	$(RM) $(OBJS)
	$(RM) -r $(OBJDIR)

fclean: clean
	$(RM) $(NAME)

re: fclean all

c: all clean

.PHONY:	all clean fclean re c
