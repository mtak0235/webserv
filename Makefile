NAME = webserv
COMPILER = clang++
CFLAGS = -g3 -fsanitize=address -Wall -Wextra -Werror -std=c++98 -pedantic
CPP_VERSION = -std=c++98

DIR_CORE = ./core/
DIR_HTTP = ./http/
DIR_CGI = ./cgi/

DIR_OBJS = ./objs/

SRCS = main.cpp \
	Cluster.cpp \
	Config.cpp \
	Connector.cpp \
	Parser.cpp \
	Cgi.cpp \
	Debug.cpp \
	Request.cpp \
	Response.cpp \


all: $(DIR_OBJS) $(NAME)

clean:
	@rm -rf $(DIR_OBJS)

fclean: clean
	@rm -f $(NAME)

re: fclean all

OBJS = $(SRCS:%.cpp=$(DIR_OBJS)%.o)

$(DIR_OBJS):
	@mkdir objs

$(NAME): $(OBJS)
	@$(COMPILER) $(CFLAGS) $(CPP_VERSION) -I $(DIR_CORE) -I $(DIR_HTTP) -I $(DIR_CGI) $(OBJS) -o $(NAME)

$(OBJS): $(DIR_OBJS)

$(DIR_OBJS)%.o: $(DIR_CORE)%.cpp
	@$(COMPILER) $(CFLAGS) $(CPP_VERSION) -I $(DIR_CORE) -I $(DIR_HTTP) -I $(DIR_CGI)  -c $< -o $@

$(DIR_OBJS)%.o: $(DIR_EVENT)%.cpp
	@$(COMPILER) $(CFLAGS) $(CPP_VERSION) -I $(DIR_CORE) -I $(DIR_HTTP) -I $(DIR_CGI)  -c $< -o $@

$(DIR_OBJS)%.o: $(DIR_HTTP)%.cpp
	@$(COMPILER) $(CFLAGS) $(CPP_VERSION) -I $(DIR_CORE) -I $(DIR_HTTP) -I $(DIR_CGI)  -c $< -o $@

$(DIR_OBJS)%.o: $(DIR_CGI)%.cpp
	@$(COMPILER) $(CFLAGS) $(CPP_VERSION) -I $(DIR_CORE) -I $(DIR_HTTP) -I $(DIR_CGI)  -c $< -o $@

test: re
	@./$(NAME) a

.PHONY: all clean fclean re
