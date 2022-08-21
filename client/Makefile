SRCS := $(wildcard src/*.c)
HDRS := $(wildcard inc/*.h)

BIN := uchat
LIB := libmx

GTK_FLAF := `pkg-config --cflags --libs gtk+-3.0`

LIB_DIR := ../framework/libmx/

CPPFLAGS := -Iinc -I libmx/inc -MMD -MP

SSL_DIR = ../framework/openssl
SSL_FILES:= ../local_lib/lib
SSL_INC_FILES:= $(SSL_DIR)

LIBS_FLAGS = -L$(SSL_FILES) -I$(SSL_INC_FILES)

SRC := $(wildcard $(SRC_DIR)/*.c)


CC := clang
WFLGS := $(addprefix -W, all extra error pedantic)
# LDFLAGS := -L/usr/local/opt/openssl/lib
# CPPFLAGS := -I/usr/local/opt/openssl/include

all: $(BIN) clean

$(BIN):
	@$(CC) $(GTK_FLAF) -std=c11 -lssl -lcrypto $(LDFLAGS) $(CPPFLAGS) $(WFLGS) $(SRCS) $(HDRS) $(LIB_DIR)$(LIB) $(LIBS_FLAGS)
	@printf "\r\33[2K$@ \033[32;1mcreated\033[0m\n"
	@mv a.out ../$@
	
$(OBJ_DIR):
	
	@mkdir -p $@

clean:
	@$(RM) -rf $(OBJ_DIR)
	@rm -rf inc/*.gch
	@rm -rf inc/*.gch.tmp
	@rm -rf *.d
	

uninstall: clean
	@$(RM) -f ../$(BIN)
	@rm -rf *.pem


reinstall: uninstall all
