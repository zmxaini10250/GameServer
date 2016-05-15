#include <mysql/mysql.h>
#include <stdio.h> int main() { MYSQL* conn;
    MYSQL_RES* res;
    MYSQL_ROW row;
    const char* server = "localhost";
    const char* user = "root";
    const char* password = "qqfo40dezs";
    const char* database = "Game";
    conn = mysql_init(NULL);
    if (!mysql_real_connect(conn, server, user, password, database, 0, NULL, CLIENT_MULTI_RESULTS))
    {
        fprintf(stderr, "%s\n\n", mysql_error(conn)); 
        exit(1); 
    } 

    const char *loginusername = "zmx";
    const char *loginpassword = "zmx123";
    const char *sqlmoudle = "select Game.LoginCheck('%s', '%s')";
    char sql[1024];
    sprintf(sql, sqlmoudle, loginusername, loginpassword);
    if (mysql_query(conn, sql))
    {
        fprintf(stderr, "%s\n\n", mysql_error(conn));
        exit(1);
    }
    do 
    { 
        if ((res = mysql_use_result(conn)))
        {
            int fieldnum = mysql_num_fields(res);
            while ((row = mysql_fetch_row(res)) != NULL)
            {
                for (int i = 0; i < fieldnum; ++i)
                {
                    printf("%s ", row[i]);
                }
            } 
        } 
    }while (!mysql_next_result(conn)); 
    mysql_free_result(res);
    mysql_close(conn);
    return 0;
}
