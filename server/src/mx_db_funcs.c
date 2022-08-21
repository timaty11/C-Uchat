#include "../inc/server.h"


void mx_sql_table_create(void)
{
    sqlite3 *db;
    int rc = sqlite3_open("uchat.sqlite3", &db);
    char *sql = NULL, *err_msg = 0;
    if (rc != SQLITE_OK)
    {
        fprintf(stderr, "Cannot open database: %s\n", sqlite3_errmsg(db));
        sqlite3_close(db);
        exit(EXIT_FAILURE);
    }
    sql = mx_strjoin(sql, "PRAGMA encoding = \"UTF-8\";");

    sql = mx_strjoin(sql, "CREATE TABLE `users` ( `id` INTEGER PRIMARY KEY AUTOINCREMENT , `username` VARCHAR(10) NOT NULL , `name` VARCHAR(10) NOT NULL , `password` VARCHAR(16) NOT NULL , `status` VARCHAR(10) NOT NULL DEFAULT 'offline' );");
    sql = mx_strjoin(sql, "CREATE TABLE `chats` ( `id` INTEGER PRIMARY KEY AUTOINCREMENT , `title` VARCHAR(16) DEFAULT NULL , `members` VARCHAR(256) DEFAULT NULL);");
    sql = mx_strjoin(sql, "CREATE TABLE `members` ( `id` INTEGER PRIMARY KEY AUTOINCREMENT , `user_id` INT NOT NULL , `admin` BOOLEAN NOT NULL DEFAULT FALSE );");
    sql = mx_strjoin(sql, "CREATE TABLE `messages` ( `id` INTEGER PRIMARY KEY AUTOINCREMENT , `chat_id` INT NOT NULL , `user_id` INT NOT NULL, `time` VARCHAR(22) NULL DEFAULT NULL, `type` VARCHAR(10) NOT NULL DEFAULT 'TEXT' , `text` VARCHAR(4096) NULL DEFAULT NULL , `pic_path` VARCHAR(256) NULL DEFAULT NULL);");
    rc = sqlite3_exec(db, sql, 0, 0, &err_msg);
    if (rc != SQLITE_OK)
    {
        if (!mx_strcmp(err_msg, "table `users` already exists"))
        {
            mx_strdel(&sql);
            sqlite3_close(db);
            return;
        }
        fprintf(stderr, "Failed to select data\n");
        fprintf(stderr, "SQL error: %s\n", err_msg);
        sqlite3_free(err_msg);
        sqlite3_close(db);
        exit(EXIT_FAILURE);
    }
    mx_strdel(&sql);
    sqlite3_close(db);
}


void mx_add_user(char *user, char *name, char *pass)
{
    sqlite3 *db;
    int rc = sqlite3_open("uchat.sqlite3", &db);
    char *sql = NULL, *err_msg = 0;
    if (rc != SQLITE_OK)
    {
        fprintf(stderr, "Cannot open database: %s\n", sqlite3_errmsg(db));
        sqlite3_close(db);
        exit(EXIT_FAILURE);
    }
    char * cipered = sha256_string(pass);

    char *temp_str = mx_strnew(2048);
    mx_strcat(temp_str, "INSERT INTO `users` (`username`, `name`, `password`) VALUES('");
    mx_strcat(temp_str, user);
    mx_strcat(temp_str, "', '");
    mx_strcat(temp_str, name);
    mx_strcat(temp_str, "', '");
    mx_strcat(temp_str, cipered);
    mx_strcat(temp_str, "');");

    sql = mx_strjoin(sql, temp_str);
    
    rc = sqlite3_exec(db, sql, 0, 0, &err_msg);
    if (rc != SQLITE_OK)
    {
        fprintf(stderr, "Failed to select data\n");
        fprintf(stderr, "SQL error: %s\n", err_msg);
        sqlite3_free(err_msg);
        sqlite3_close(db);
        exit(EXIT_FAILURE);
    }
    mx_strdel(&sql);
    mx_strdel(&temp_str);
    sqlite3_close(db);
}


int mx_check_input(char *user, char *name, char *pass)
{
    //// CHECK FOR SUCH LOGIN IN DB
    sqlite3 *db;
	sqlite3_stmt *stmt;
	
	sqlite3_open("uchat.sqlite3", &db);

	if (db == NULL)
	{
		printf("Failed to open DB\n");
		return 1;
	}

	printf("Performing query...\n");
	sqlite3_prepare_v2(db, "SELECT username from users", -1, &stmt, NULL);
	
	printf("Got results:\n");
	while (sqlite3_step(stmt) != SQLITE_DONE)
    {
		int num_cols = sqlite3_column_count(stmt);
		
		for (int i = 0; i < num_cols; i++)
		{
            if (!mx_strcmp((char *)sqlite3_column_text(stmt, i), user))
            {
                sqlite3_finalize(stmt);
	            sqlite3_close(db);
                return 2;
            }
		}
	}
    sqlite3_finalize(stmt);
    sqlite3_close(db);

    //// CHECK LOGIN/NAME FOR SUSPENDED WORDS
    if (!mx_strcmp(user, "negr") || !mx_strcmp(name, "negr"))
    {
        return 3;
    }

    //// CHECK PASSWORD FOR ITS RELIABILITY
    if (!mx_strcmp(pass, "123"))
    {
        return 4;
    }

    
    return 1;
}


int mx_db_login(char *user, char *pass, char ** cur_user_name)
{
    sqlite3 *db;
	sqlite3_stmt *stmt;
	
	sqlite3_open("uchat.sqlite3", &db);

	if (db == NULL)
	{
		mx_printstr("Failed to open DB\n");
		return 1;
	}

    // TO DO - HASH PASS AND COMPARE WITH WHAT WAS IN DB
    char * cipered = sha256_string(pass);

	mx_printstr("Performing query...\n");
	sqlite3_prepare_v2(db, "SELECT username, password from users", -1, &stmt, NULL);
	
	mx_printstr("Got results!\n");
	while (sqlite3_step(stmt) != SQLITE_DONE)
    {
        mx_printstr("hyi ");
		int num_cols = sqlite3_column_count(stmt);
		
		for (int i = 0; i < num_cols; i++)
		{
            char *db_user = mx_strdup((char *)sqlite3_column_text(stmt, i));
            if (!mx_strcmp(db_user, user))
            {
                sqlite3_finalize(stmt);
                char *temp_str = mx_strnew(1024);
                temp_str = mx_strdup("SELECT password, name from users WHERE username='");
                mx_strcat(temp_str, db_user);
                mx_strcat(temp_str, "';");
                
                sqlite3_prepare_v2(db, temp_str, -1, &stmt, NULL);
                sqlite3_step(stmt);

                if (!mx_strcmp((char *)sqlite3_column_text(stmt, i), cipered))
                {
                    *cur_user_name = mx_strdup((char *)sqlite3_column_text(stmt, 1));
                    sqlite3_finalize(stmt);
                    sqlite3_close(db);
                    return 1;
                }
                else
                {
                    sqlite3_finalize(stmt);
                    sqlite3_close(db);
                    return 3;
                }
            }
		}
	}
    
    sqlite3_finalize(stmt);
    sqlite3_close(db);
    return 2;
}


char * mx_change_user_pass(char * user_id, char * old_pass, char * new_pass)
{
    sqlite3 *db;
	sqlite3_stmt *stmt;
	
	sqlite3_open("uchat.sqlite3", &db);

	if (db == NULL)
	{
		printf("Failed to open DB\n");
		return NULL;
	}

	printf("Performing query...\n");
    char *temp_str = mx_strnew(100);
    mx_strcat(temp_str, "SELECT password FROM USERS WHERE id = ");
    mx_strcat(temp_str, user_id);
    mx_strcat(temp_str, ";");
	sqlite3_prepare_v2(db, temp_str, -1, &stmt, NULL);
    
    mx_printint(sqlite3_column_count(stmt));
	
	mx_printstr("zaprashivaem: ");
    mx_printstr(temp_str);
    mx_printchar('\n');
    char * result = mx_strnew(MAXDATASIZE);
    mx_printstr("Got results\n");
    
    while (sqlite3_step(stmt) != SQLITE_DONE)
	{
        // Chat id
        mx_strcat(result, (char*)sqlite3_column_text(stmt, 0));
    }

    printf("result = %s\n", result);

    if (mx_strcmp(result, sha256_string(old_pass)))
    {
        mx_printstr("NEPRAVILNO\n");
        sqlite3_finalize(stmt);
        sqlite3_close(db);
        return "RESPONSE 0: INCORRECT OLD PASSWORD";
    }

    mx_strdel(&temp_str);
    sqlite3_finalize(stmt);
    sqlite3_close(db);


    int rc = sqlite3_open("uchat.sqlite3", &db);
    char *sql = NULL, *err_msg = 0;
    if (rc != SQLITE_OK)
    {
        fprintf(stderr, "Cannot open database: %s\n", sqlite3_errmsg(db));
        sqlite3_close(db);
        exit(EXIT_FAILURE);
    }
    
    temp_str = mx_strnew(2048);
    mx_strcpy(temp_str, "UPDATE `users` SET password = '");
    mx_strcat(temp_str, sha256_string(new_pass));
    mx_strcat(temp_str, "' WHERE id = ");
    mx_strcat(temp_str, user_id);
    mx_strcat(temp_str, ";");

    sql = mx_strjoin(sql, temp_str);
    printf("sql status zapros = %s\n", sql);
    
    rc = sqlite3_exec(db, sql, 0, 0, &err_msg);
    if (rc != SQLITE_OK)
    {
        fprintf(stderr, "Failed to update data\n");
        fprintf(stderr, "SQL error: %s\n", err_msg);
        sqlite3_free(err_msg);
        sqlite3_close(db);
        exit(EXIT_FAILURE);
    }

    mx_strdel(&sql);
    mx_strdel(&temp_str);
    sqlite3_close(db);

    return "RESPONSE 200: SUCCESSFULL PASSWORD UPDATE";
}


void mx_update_user_status(char * user_id, char * status)
{
    // UPDATE employees SET lastname = 'Smith' WHERE employeeid = 3;

    sqlite3 *db;
    int rc = sqlite3_open("uchat.sqlite3", &db);
    char *sql = NULL, *err_msg = 0;
    if (rc != SQLITE_OK)
    {
        fprintf(stderr, "Cannot open database: %s\n", sqlite3_errmsg(db));
        sqlite3_close(db);
        exit(EXIT_FAILURE);
    }
    
    char *temp_str = mx_strnew(2048);
    mx_strcpy(temp_str, "UPDATE `users` SET status = '");
    mx_strcat(temp_str, status);
    mx_strcat(temp_str, "' WHERE id = ");
    mx_strcat(temp_str, user_id);
    mx_strcat(temp_str, ";");

    sql = mx_strjoin(sql, temp_str);
    printf("sql status zapros = %s\n", sql);
    
    rc = sqlite3_exec(db, sql, 0, 0, &err_msg);
    if (rc != SQLITE_OK)
    {
        fprintf(stderr, "Failed to update data\n");
        fprintf(stderr, "SQL error: %s\n", err_msg);
        sqlite3_free(err_msg);
        sqlite3_close(db);
        exit(EXIT_FAILURE);
    }

    mx_strdel(&sql);
    mx_strdel(&temp_str);
    sqlite3_close(db);
}


char * mx_db_get_all_chats_for_user(char * user_id)
{
    sqlite3 *db;
	sqlite3_stmt *stmt;
	
	sqlite3_open("uchat.sqlite3", &db);

	if (db == NULL)
	{
		printf("Failed to open DB\n");
		return NULL;
	}

	printf("Performing query...\n");
    char *temp_str = mx_strnew(100);
    mx_strcat(temp_str, "SELECT * FROM chats WHERE members LIKE '%");
    mx_strcat(temp_str, mx_get_user_by_id(mx_atoi(user_id)));
    mx_strcat(temp_str, "%';");
	sqlite3_prepare_v2(db, temp_str, -1, &stmt, NULL);
    
    mx_printint(sqlite3_column_count(stmt));
	
	mx_printstr("zaprashivaem: ");
    mx_printstr(temp_str);
    mx_printchar('\n');
    char * result = mx_strnew(MAXDATASIZE * 50);
    mx_printstr("Got results\n");
    
    while (sqlite3_step(stmt) != SQLITE_DONE)
	{
        // Chat id
        mx_strcat(result, mx_itoa(sqlite3_column_int(stmt, 0)));
        mx_strcat(result, ";");

        // Title
        mx_strcat(result, (char *)sqlite3_column_text(stmt, 1));
        mx_strcat(result, ";");

        // Members
        mx_strcat(result, (char *)sqlite3_column_text(stmt, 2));
        mx_strcat(result, "|");
    }

    printf("result = %s\n", result);

    if (result[0] == '\0')
    {
        mx_printstr("Not found!\n");
        sqlite3_finalize(stmt);
        sqlite3_close(db);
        return "RESPONSE 0: NO MESSAGES FOUND";
    }

    sqlite3_finalize(stmt);
    sqlite3_close(db);
    return result;
}


int mx_db_create_chat(char * title, char * members)
{
    sqlite3 *db;
    // members = NULL;
    int rc = sqlite3_open("uchat.sqlite3", &db);
    char *sql = NULL, *err_msg = 0;
    if (rc != SQLITE_OK)
    {
        fprintf(stderr, "Cannot open database: %s\n", sqlite3_errmsg(db));
        sqlite3_close(db);
        exit(EXIT_FAILURE);
    }
    
    char *temp_str = mx_strnew(2048);
    mx_strcpy(temp_str, "INSERT INTO `chats` (`title`, `members`) VALUES('");
    mx_strcat(temp_str, title);
    mx_strcat(temp_str, "', '");
    mx_strcat(temp_str, members);
    mx_strcat(temp_str, "');");

    sql = mx_strjoin(sql, temp_str);
    printf("sql zapros = %s\n", sql);
    
    rc = sqlite3_exec(db, sql, 0, 0, &err_msg);
    if (rc != SQLITE_OK)
    {
        fprintf(stderr, "Failed to select data\n");
        fprintf(stderr, "SQL error: %s\n", err_msg);
        sqlite3_free(err_msg);
        sqlite3_close(db);
        exit(EXIT_FAILURE);
    }
    mx_strdel(&sql);
    mx_strdel(&temp_str);
    sqlite3_close(db);

    return mx_get_id_by_ChatTitle(title);
}


int mx_db_add_message(char * chat_id, char * user_id, char * mes_type, char * text, char * pic_path)
{
    sqlite3 *db;
    sqlite3_stmt *stmt;

    int rc = sqlite3_open("uchat.sqlite3", &db);
    if (rc != SQLITE_OK)
    {
        fprintf(stderr, "Cannot open database: %s\n", sqlite3_errmsg(db));
        sqlite3_close(db);
        exit(EXIT_FAILURE);
    }

    if (!mx_strcmp(mes_type, "TEXT"))
    {
        printf("shifryem - %s\n", text);
        char *temp = encrypted(text);

        char * temp1 = mx_strnew(256 * 5);
        int j = 0;
        for (int i = 0; i <= 256; i++)
        {
            char * sas = mx_strdup(mx_itoa(temp[i]));
            for (int z = 0; z < mx_strlen(sas); z++)
            {
                temp1[j] = sas[z];
                j++;
            }
            
            temp1[j] = ',';
            j++;
        }

        
        sqlite3_prepare_v2(db, "INSERT INTO `messages` (`chat_id`, `user_id`, `time`, `text`) VALUES(?,?,?,?)", 78, &stmt, NULL);

        if (stmt != NULL)
        {
            // Get cur time
            time_t cur_time = time(NULL);
            char *cur_time_string = ctime(&cur_time);

            // "Wed Feb  2 15:35:41 2022" - 25 symbols

            sqlite3_bind_int(stmt, 1, mx_atoi(chat_id));
            sqlite3_bind_int(stmt, 2, mx_atoi(user_id));
            sqlite3_bind_text(stmt, 3, &cur_time_string[4], 20, SQLITE_TRANSIENT);
            sqlite3_bind_text(stmt, 4, temp1, 256 * 5, SQLITE_TRANSIENT);
            sqlite3_step(stmt);
            sqlite3_finalize(stmt);
        }

        sqlite3_close(db);
        return 1;
    }
    else if (!mx_strcmp(mes_type, "STICKER"))
    {
        sqlite3_prepare_v2(db, "INSERT INTO `messages` (`chat_id`, `user_id`, `time`, `type`, `pic_path`) VALUES(?,?,?,?,?)", 92, &stmt, NULL);

        if (stmt != NULL)
        {
            // Get cur time
            time_t cur_time = time(NULL);
            char *cur_time_string = ctime(&cur_time);

            // "Wed Feb  2 15:35:41 2022" - 25 symbols

            sqlite3_bind_int(stmt, 1, mx_atoi(chat_id));
            sqlite3_bind_int(stmt, 2, mx_atoi(user_id));
            sqlite3_bind_text(stmt, 3, &cur_time_string[4], 20, SQLITE_TRANSIENT);
            sqlite3_bind_text(stmt, 4, "STICKER", 10, SQLITE_TRANSIENT);
            sqlite3_bind_text(stmt, 5, pic_path, 256, SQLITE_TRANSIENT);
            sqlite3_step(stmt);
            sqlite3_finalize(stmt);
        }

        sqlite3_close(db);
        return 1;
    }

    sqlite3_close(db);
    return 1;
}


int mx_get_user_id(char *user)
{
    int user_id;
    sqlite3 *db;
	sqlite3_stmt *stmt;
	
	sqlite3_open("uchat.sqlite3", &db);

	if (db == NULL)
	{
		printf("Failed to open DB\n");
		return 1;
	}

	printf("Performing query...\n");
    char *temp_str = mx_strnew(256);
    mx_strcat(temp_str, "SELECT id from users WHERE username='");
    mx_strcat(temp_str, user);
    mx_strcat(temp_str, "';");
	sqlite3_prepare_v2(db, temp_str, -1, &stmt, NULL);
	
	printf("Got results!\n");
    while(sqlite3_step(stmt) != SQLITE_DONE)
	{
        user_id = sqlite3_column_int(stmt, 0);
        printf("%i\n", user_id);
    }
    sqlite3_finalize(stmt);
    sqlite3_close(db);

    return user_id;
}


char * mx_get_user_by_id(int user_id)
{
    char *User = NULL;
    sqlite3 *db;
	sqlite3_stmt *stmt;
	
	sqlite3_open("uchat.sqlite3", &db);

	if (db == NULL)
	{
		printf("Failed to open DB\n");
		return NULL;
	}

	printf("Performing query...\n");
    char *temp_str = mx_strdup("SELECT name from users WHERE id=");
    mx_strcat(temp_str, mx_itoa(user_id));
    mx_strcat(temp_str, ";");
	sqlite3_prepare_v2(db, temp_str, -1, &stmt, NULL);
	
	printf("Got results!\n");
    while(sqlite3_step(stmt) != SQLITE_DONE)
	{
        User = mx_strdup((char *)sqlite3_column_text(stmt, 0));
        printf("%s\n", User);
    }

    sqlite3_finalize(stmt);
    sqlite3_close(db);
    return User;
}


char * mx_get_user_by_name(char * user_name)
{
    sqlite3 *db;
	sqlite3_stmt *stmt;
    char * response = mx_strnew(MAXDATASIZE);
	
	sqlite3_open("uchat.sqlite3", &db);

	if (db == NULL)
	{
		printf("Failed to open DB\n");
		return NULL;
	}

	printf("Performing query...\n");
    char *temp_str = mx_strnew(MAXDATASIZE);
    mx_strcat(temp_str, "SELECT id, name, status from users WHERE name LIKE '%");
    mx_strcat(temp_str, user_name);
    mx_strcat(temp_str, "%';");

	sqlite3_prepare_v2(db, temp_str, -1, &stmt, NULL);
	
	printf("Got results!\n");
    while(sqlite3_step(stmt) != SQLITE_DONE)
	{
        mx_strcat(response, mx_itoa(sqlite3_column_int(stmt, 0)));
        mx_strcat(response, ";");
        mx_strcat(response, (char *)sqlite3_column_text(stmt, 1));
        mx_strcat(response, ";");
        mx_strcat(response, (char *)sqlite3_column_text(stmt, 2));
        mx_strcat(response, "|");
    }

    printf("%s\n", response);
    sqlite3_finalize(stmt);
    sqlite3_close(db);
    return response;
}


int mx_get_id_by_ChatTitle(char * ChatTitle)
{
    int chat_id = 0;
    sqlite3 *db;
	sqlite3_stmt *stmt;
	
	sqlite3_open("uchat.sqlite3", &db);

	if (db == NULL)
	{
		printf("Failed to open DB\n");
		return 1;
	}

	printf("Performing query...\n");
    char *temp_str = mx_strnew(256);
    mx_strcat(temp_str, "SELECT id FROM chats WHERE title LIKE '%");
    mx_strcat(temp_str, ChatTitle);
    mx_strcat(temp_str, "%';");
	sqlite3_prepare_v2(db, temp_str, -1, &stmt, NULL);
	
	printf("Got results!\n");
    while(sqlite3_step(stmt) != SQLITE_DONE)
	{
        chat_id = sqlite3_column_int(stmt, 0);
        printf("%i\n", chat_id);
    }

    sqlite3_finalize(stmt);
    sqlite3_close(db);
    return chat_id;
}


char * mx_get_ChatData_by_id(char * chat_id)
{
    sqlite3 *db;
	sqlite3_stmt *stmt;
	
	sqlite3_open("uchat.sqlite3", &db);

	if (db == NULL)
	{
		printf("Failed to open DB\n");
		return NULL;
	}

	printf("Performing query...\n");
    char *temp_str = mx_strnew(256);
    mx_strcat(temp_str, "SELECT title, members FROM chats WHERE id=");
    mx_strcat(temp_str, chat_id);
    mx_strcat(temp_str, ";");
	sqlite3_prepare_v2(db, temp_str, -1, &stmt, NULL);
	
	printf("Got results!\n");
    char * result = mx_strnew(1024);
    while(sqlite3_step(stmt) != SQLITE_DONE)
	{
        mx_strcat(result, (char *)sqlite3_column_text(stmt, 0));
        mx_strcat(result, ";");
        mx_strcat(result, (char *)sqlite3_column_text(stmt, 1));

        printf("%s\n", result);
    }

    sqlite3_finalize(stmt);
    sqlite3_close(db);
    return result;
}


char * mx_get_last_message(char * chat_id, char * last_message_id)
{
    sqlite3 *db;
	sqlite3_stmt *stmt;
	
	sqlite3_open("uchat.sqlite3", &db);

	if (db == NULL)
	{
		printf("Failed to open DB\n");
		return NULL;
	}

	printf("Performing query...\n");
    char *temp_str = mx_strnew(100);
    mx_strcat(temp_str, "SELECT * FROM messages WHERE chat_id=");
    mx_strcat(temp_str, chat_id);
    
    mx_strcat(temp_str, " AND id > ");
    mx_strcat(temp_str, last_message_id);

    mx_strcat(temp_str, " ORDER BY id DESC;");
    printf("Select is: %s\n", temp_str);
	sqlite3_prepare_v2(db, temp_str, -1, &stmt, NULL);
    mx_strdel(&temp_str);
	
	mx_printstr("Got results: ");
    char * result = mx_strnew(MAXDATASIZE * 4);
    while (sqlite3_step(stmt) != SQLITE_DONE)
	{
        // Message ID
        mx_strcat(result, mx_itoa(sqlite3_column_int(stmt, 0)));
        mx_strcat(result, ";");

        // User name
        mx_strcat(result, mx_get_user_by_id(sqlite3_column_int(stmt, 2)));
        mx_strcat(result, ";");

        // Time
        mx_strcat(result, (char *)sqlite3_column_text(stmt, 3));
        mx_strcat(result, ";");

        // Message type
        mx_strcat(result, (char *)sqlite3_column_text(stmt, 4));
        mx_strcat(result, ";");

        if (!mx_strcmp((char *)sqlite3_column_text(stmt, 4), "TEXT"))
        {
            // Message text
            char *temp2 = mx_strnew(257);
            char **pizdec = mx_strsplit((char *)sqlite3_column_text(stmt, 5), ',');
            for (int i = 0; pizdec[i] != NULL; i++)
            {
                temp2[i] = mx_atoi(pizdec[i]);
            }

            mx_strcat(result, descrypted(temp2));
        }
        else if (!mx_strcmp((char *)sqlite3_column_text(stmt, 4), "STICKER"))
        {
            // Sticker path
            mx_strcat(result, (char *)sqlite3_column_text(stmt, 6));
            mx_strcat(result, ";");
        }

        mx_strcat(result, "|");        
    }
    if (result[0] == '\0')
    {
        mx_printstr("Not found!\n");
        mx_strcat(result, "RESPONSE 0: NO MESSAGES FOUND");
    }

    printf("========\ndb vernyla:\n%s\n", result);

    sqlite3_finalize(stmt);
    sqlite3_close(db);
    return result;
}


char * mx_get_last_50_messages(int chat_id)
{
    sqlite3 *db;
	sqlite3_stmt *stmt;
	
	sqlite3_open("uchat.sqlite3", &db);

	if (db == NULL)
	{
		printf("Failed to open DB\n");
		return NULL;
	}

	printf("Performing query...\n");
    char *temp_str = mx_strnew(100);
    mx_strcat(temp_str, "SELECT * FROM messages WHERE chat_id=");
    mx_strcat(temp_str, mx_itoa(chat_id));
    mx_strcat(temp_str, " ORDER BY id DESC LIMIT 50;");
	sqlite3_prepare_v2(db, temp_str, -1, &stmt, NULL);
    
    mx_printint(sqlite3_column_count(stmt));
	
	mx_printstr("Got results: ");
    char * result = mx_strnew(MAXDATASIZE * 50);
    mx_printstr("sozdali\n");
    
    while (sqlite3_step(stmt) != SQLITE_DONE)
	{
        // ID
        mx_strcat(result, mx_itoa(sqlite3_column_int(stmt, 0)));
        mx_strcat(result, ";");
        
        // CHAT_ID
        mx_strcat(result, mx_get_user_by_id(sqlite3_column_int(stmt, 2)));
        mx_strcat(result, ";");

        // USER_ID
        mx_strcat(result, mx_itoa(sqlite3_column_int(stmt, 2)));
        mx_strcat(result, ";");

        // TIME
        mx_strcat(result, (char *)sqlite3_column_text(stmt, 3));
        mx_strcat(result, ";");

        // TYPE
        mx_strcat(result, (char *)sqlite3_column_text(stmt, 4));
        mx_strcat(result, ";");

        // DATA
        if (!mx_strcmp((char *)sqlite3_column_text(stmt, 4), "TEXT"))
        {
            char *temp2 = mx_strnew(257);
            char **pizdec = mx_strsplit((char *)sqlite3_column_text(stmt, 5), ',');
            for (int i = 0; pizdec[i] != NULL; i++)
            {
                temp2[i] = mx_atoi(pizdec[i]);
            }

            mx_strcat(result, descrypted(temp2));
            mx_strcat(result, "|");
        }
        else if (!mx_strcmp((char *)sqlite3_column_text(stmt, 4), "TEXT"))
        {
            mx_strcat(result, (char *)sqlite3_column_text(stmt, 6));
            mx_strcat(result, "|");
        }
    }
    if (result[0] == '\0')
    {
        mx_printstr("Not found!\n");
        sqlite3_finalize(stmt);
        sqlite3_close(db);
        return "RESPONSE 0: NO MESSAGES FOUND";
    }

    sqlite3_finalize(stmt);
    sqlite3_close(db);
    return result;
}


char * mx_delete_message_by_id(int mes_id)
{
    sqlite3 *db;
	sqlite3_stmt *stmt;
	
	sqlite3_open("uchat.sqlite3", &db);

	if (db == NULL)
	{
		printf("Failed to open DB\n");
		return NULL;
	}

	printf("Performing query...\n");
    char *temp_str = mx_strnew(100);
    mx_strcat(temp_str, "DELETE FROM messages WHERE id=");
    mx_strcat(temp_str, mx_itoa(mes_id));
    mx_strcat(temp_str, ";");
	sqlite3_prepare_v2(db, temp_str, -1, &stmt, NULL);
    	
	mx_printstr("Got results: ");
    char * result = mx_strnew(256);
    mx_strcat(result, "RESPONSE 200: DELETED SUCCESSFULY");

    sqlite3_finalize(stmt);
    sqlite3_close(db);
    return result;
}


char * mx_add_user_to_chat_by_name(char * chat_id, char * user_name)
{
    // Get chat data by its id, then strcat it's data with user we want to add
    // then update the DB with new strings

    char * chat_data = mx_get_ChatData_by_id(chat_id);
    char ** chat_data_arr = mx_strsplit(chat_data, ';');

    mx_strdel(&chat_data);

    // Form new chat title and members
    
    char * new_title = mx_strnew(1024);
    char * new_members = mx_strnew(1024);

    mx_strcat(new_title, chat_data_arr[0]);
    mx_strcat(new_title, ",");
    mx_strcat(new_title, user_name);

    mx_strcat(new_members, chat_data_arr[1]);
    mx_strcat(new_members, ",");
    mx_strcat(new_members, user_name);

    sqlite3 *db;
    int rc = sqlite3_open("uchat.sqlite3", &db);
    char *sql = NULL, *err_msg = 0;
    if (rc != SQLITE_OK)
    {
        fprintf(stderr, "Cannot open database: %s\n", sqlite3_errmsg(db));
        sqlite3_close(db);
        exit(EXIT_FAILURE);
    }
    
    char *temp_str = mx_strnew(2048);
    mx_strcat(temp_str, "UPDATE `chats` SET title = '");
    mx_strcat(temp_str, new_title);
    mx_strcat(temp_str, "', members = '");
    mx_strcat(temp_str, new_members);
    mx_strcat(temp_str, "' WHERE id = ");
    mx_strcat(temp_str, chat_id);
    mx_strcat(temp_str, ";");

    sql = mx_strjoin(sql, temp_str);
    printf("sql add user to chat zapros = %s\n", sql);
    
    rc = sqlite3_exec(db, sql, 0, 0, &err_msg);
    if (rc != SQLITE_OK)
    {
        fprintf(stderr, "Failed to update data\n");
        fprintf(stderr, "SQL error: %s\n", err_msg);
        sqlite3_free(err_msg);
        sqlite3_close(db);
        exit(EXIT_FAILURE);
    }

    mx_strdel(&sql);
    mx_strdel(&temp_str);
    sqlite3_close(db);

    return user_name;
}


void mx_change_chat_title(char * chat_id, char * new_title)
{
    sqlite3 *db;
    int rc = sqlite3_open("uchat.sqlite3", &db);
    char *sql = NULL, *err_msg = 0;
    if (rc != SQLITE_OK)
    {
        fprintf(stderr, "Cannot open database: %s\n", sqlite3_errmsg(db));
        sqlite3_close(db);
        exit(EXIT_FAILURE);
    }
    
    char *temp_str = mx_strnew(2048);
    mx_strcat(temp_str, "UPDATE `chats` SET title = '");
    mx_strcat(temp_str, new_title);
    mx_strcat(temp_str, "' WHERE id = ");
    mx_strcat(temp_str, chat_id);
    mx_strcat(temp_str, ";");

    sql = mx_strjoin(sql, temp_str);
    printf("sql add user to chat zapros = %s\n", sql);
    
    rc = sqlite3_exec(db, sql, 0, 0, &err_msg);
    if (rc != SQLITE_OK)
    {
        fprintf(stderr, "Failed to update data\n");
        fprintf(stderr, "SQL error: %s\n", err_msg);
        sqlite3_free(err_msg);
        sqlite3_close(db);
        exit(EXIT_FAILURE);
    }

    mx_strdel(&sql);
    mx_strdel(&temp_str);
    sqlite3_close(db);
}


char * mx_hande_single_quotes(char * original)
{
    char * fixed_text = mx_strnew(MAXDATASIZE * 4);

    int j = 0;
    for (int i = 0; i < 513; i++)
    {
        if (original[i] != '\'')
        {
            fixed_text[j] = original[i];
            j++;
        }
        else
        {
            fixed_text[j] = '\'';
            j++;
            fixed_text[j] = original[i];
            j++;
        }
    }

    return fixed_text;
}

