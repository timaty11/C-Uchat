SRCS := $(wildcard src/*.c)
HDRS := $(wildcard inc/*.h)

BIN := uchat_server 

LIB := libmx
SQLITE3 := sqlite3

SQLITE3_DIR := ../framework/sqlite3/
LIB_DIR := ../framework/libmx/

SSL_DIR = ../framework/openssl
SSL_FILES:= ../local_lib/lib
SSL_INC_FILES:= $(SSL_DIR)

LIBS_FLAGS = -L$(SSL_FILES) -I$(SSL_INC_FILES)

SRC := $(wildcard $(SRC_DIR)/*.c)
OBJ := $(SRC:$(SRC_DIR)/%.c=$(OBJ_DIR)/%.o)

CC := clang
CPPFLAGS := -Iinc -I libmx/inc -MMD -MP
WFLGS := $(addprefix -W, all extra error pedantic)
# LDFLAGS := -L/usr/local/opt/openssl/lib
# CPPFLAGS := -I/usr/local/opt/openssl/include

all: $(LIB) $(SQLITE3) $(BIN) clean

$(BIN): $(OBJ) 
	@$(CC) -std=c11 -lssl -lcrypto $(LDFLAGS) $(CPPFLAGS) $(WFLGS) $(SRCS) $(HDRS) $(LIB_DIR)$(LIB) $(SQLITE3_DIR)$(SQLITE3) $(LIBS_FLAGS)
	@printf "\r\33[2K$@ \033[32;1mcreated\033[0m\n"
	@mv a.out ../$@

$(LIB):
	@make -sC $(LIB_DIR)

$(SQLITE3):
	@make -sC $(SQLITE3_DIR)

$(OBJ_DIR):
	mkdir -p $@

clean:
	@$(RM) -rf $(OBJ_DIR)
	@make -C $(LIB_DIR) $@
	@make -C $(SQLITE3_DIR) $@
	@rm -rf inc/*.gch
	@rm -rf *.d


uninstall: clean
	@$(RM) -f ../$(BIN) $@
	@make -C $(LIB_DIR) $@
	@make -C $(SQLITE3_DIR) $@

reinstall: uninstall all
