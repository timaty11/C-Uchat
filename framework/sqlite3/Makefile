NAME = sqlite3

CFLG = -std=c11 $(addprefix -W, all extra error pedantic) -g

SRCD = src
INCD = inc
OBJD = obj

INC = sqlite3.h \
	  sqtite3ext.h \
INCS = $(addprefix $(INCD)/, $(INC))

FILES = shell.c \
		sqlite3.c \


SRCS = $(addprefix $(SRCD)/, $(FILES))
OBJS = $(addprefix $(OBJD)/, $(FILES:%.c=%.o))

all: install

install: $(NAME)

$(NAME): $(OBJS)
	@ar rcs $@ $^
	@printf "\r\33[2K$@ \033[32;1mcreated\033[0m\n"

$(OBJD)/%.o: $(SRCD)/%.c $(INCS)
	@clang $(CFLG) -c $< -o $@ -I$(INCD)
	@printf "\r\33[2K$(NAME) \033[33;1mcompile \033[0m$(<:$(SRCD)/%.c=%) "

$(OBJS): | $(OBJD)

$(OBJD):
	@mkdir -p $@

clean:
	@rm -rf $(OBJD)
	@printf "$(OBJD)\t   \033[31;1mdeleted\033[0m\n"

uninstall: clean
	@rm -rf $(NAME)
	@printf "$(NAME) \033[31;1muninstalled\033[0m\n"

reinstall: uninstall install
