NAME = webserv
COMPILER = clang++
CFLAGS = -Wall -Wextra -Werror -pedantic
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

all: $(NAME)

clean:
	@rm -f $(OBJS)

fclean: clean
	@rm -f $(NAME)

re: fclean all

OBJS = $(SRCS:%.cpp=$(DIR_OBJS)%.o)

$(OBJS): | $(DIR_OBJS)

$(NAME): $(OBJS)
	@$(COMPILER) $(CFLAGS) $(CPP_VERSION) -I $(DIR_CORE) -I $(DIR_HTTP) -I $(DIR_CGI) $(OBJS) -o $(NAME)

$(DIR_OBJS)%.o: $(DIR_CORE)%.cpp
	@$(COMPILER) $(CFLAGS) $(CPP_VERSION) -I $(DIR_CORE) -I $(DIR_HTTP) -I $(DIR_CGI)  -c $< -o $@

$(DIR_OBJS)%.o: $(DIR_HTTP)%.cpp
	@$(COMPILER) $(CFLAGS) $(CPP_VERSION) -I $(DIR_CORE) -I $(DIR_HTTP) -I $(DIR_CGI)  -c $< -o $@

$(DIR_OBJS)%.o: $(DIR_CGI)%.cpp
	@$(COMPILER) $(CFLAGS) $(CPP_VERSION) -I $(DIR_CORE) -I $(DIR_HTTP) -I $(DIR_CGI)  -c $< -o $@
	
.PHONY: all clean fclean re
