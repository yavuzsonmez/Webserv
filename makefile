Reset			=		\033[0m						# Text Reset
R				=		\033[0;31m					# Red
G				=		\033[0;32m					# Green
B				=		\033[0;34m					# Blue
P				=		\033[0;35m					# Purple
Y				=		\033[33;1m					# Yellow

NAME			=		webserv

HDRS			=		./inc/config_file/ConfigFileParsing.hpp \
						./inc/config_file/InvalidConfigurationFile.hpp \
						./inc/configuration_key/ConfigurationKey.hpp \
						./inc/configuration_key/ServerBlock.hpp \
						./inc/debugger/Singleton.hpp \
						./inc/debugger/DebuggerPrinter.hpp \
						./inc/http/headers.hpp \
						./inc/http/Request.hpp \
						./inc/http/Response.hpp \
						./inc/http/status.hpp \
						./inc/network/ClientSocket.hpp \
						./inc/network/ServerSocket.hpp \
						./inc/utility/colors.hpp \
						./inc/utility/utility.hpp \

ENTRY			=		./main.cpp

DEBUGGER		=		./src/debugger/DebuggerPrinter.cpp \

CONFIG_FILE		=		./src/config_file/ConfigFileParsing.cpp \
						./src/config_file/ConfigValidationHelper.cpp \
						./src/config_file/InvalidConfigurationFile.cpp \
						./src/configuration_key/ConfigurationKey.cpp \
						./src/configuration_key/ServerBlock.cpp \

HTTP			=		./src/http/Request.cpp \
						./src/http/Response.cpp \
						./src/http/Cgi.cpp \
						./src/http/Process.cpp \

NETWORK			=		./src/network/ClientSocket.cpp \
						./src/network/ServerSocket.cpp \

UTILS			=		./src/utility/get_file_content.cpp \
						./src/utility/is_file_accessible.cpp \
						./src/utility/get_abs_path.cpp \
						./src/utility/lower_upper_str.cpp \
						./src/utility/check_config_file.cpp \
						./src/utility/filter_characters.cpp \
						./src/utility/strip_from_str.cpp \
						./src/utility/get_file_name.cpp \
						./src/utility/get_server_block.cpp \
						./src/utility/validate_parantheses.cpp \
						./src/utility/split_on_delimiter.cpp \
						./src/utility/convert_configuration_key_type.cpp \
						./src/utility/inttohex.cpp \
						./src/utility/trim_whitespaces.cpp \
						./src/utility/validate_url.cpp \
						./src/utility/get_file_content_for_request.cpp \
						./src/utility/is_valid_fd.cpp \
						./src/utility/send_server_unavailable.cpp \
						./src/utility/stoi.cpp \
						./src/utility/kill_with_error.cpp \
						./src/utility/path_helper.cpp \

SRCS			=		$(ENTRY) $(DEBUGGER) $(CONFIG_FILE) $(HTTP) $(NETWORK) $(UTILS)

OBJS			=		$(SRCS:.cpp=.o)

FLAGS			=		-Werror -Wall -Wextra -g

# Here we define how every single file is being compiled.
# With MAKECMDGOALS we detect if we are running a debug build and then inject the defines.
ifeq ($(MAKECMDGOALS),rebug)
    FLAGS += -std=c++98 -D DEBUGMODE=1 -g -D ENABLE_LOGGING=1 -Wall -Wextra -Werror
else ifeq ($(MAKECMDGOALS),debug)
    FLAGS += -std=c++98 -D DEBUGMODE=1 -g -D ENABLE_LOGGING=1 -Wall -Wextra -Werror
else
    FLAGS += -std=c++98 -D DEBUGMODE=0 -D ENABLE_LOGGING=0 -Wall -Wextra -Werror
endif

.cpp.o			:
						@c++ -std=c++98 -Wall -Wextra -Werror -c $(FLAGS) $< -o $@

$(NAME)			:		$(OBJS) $(HDRS) | silence
						@c++ $(FLAGS) $(OBJS) -o $(NAME)
						@echo "$(G)$(NAME) has been created$(Reset)"

dev				:		$(OBJS) $(HDRS) | silence
						@c++ $(OBJS) -o $(NAME)
						@echo "$(R)Compiling without flags WARNING !$(Reset)"
						@echo "$(G)$(NAME) has been created$(Reset)"

debug				:	$(OBJS) $(HDRS) | silence
						@c++ $(OBJS) $(DEBUG) -o $(NAME)
						@echo "$(P)DEBUG MODE : address sanitizer$(Reset)"
						@echo "$(G)$(NAME) has been created$(Reset)"

silence:
						@:

valgrind			:	$(NAME)
						@echo "$(B)Checking for memory leaks..$(Reset)"
						@valgrind --leak-check=full $(NAME)

clean			:
						@rm -f $(OBJS)
						@echo "$(R)Objects have been removed 🗑$(Reset)"

fclean			:		clean
						@rm -f $(NAME)
						@echo "$(R)$(NAME) has been removed 🗑$(Reset)"

all				:		$(NAME)

re				:		fclean all

nologs			:		fclean all

rebug			:		fclean debug

.PHONY			:		clean fclean all re