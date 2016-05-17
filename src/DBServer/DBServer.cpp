#include "DBServer.h"

#include <stdio.h>
#include <mysql/mysql.h>
#include <stdlib.h>

int CDBServer::ConnectMysql()
{ 
    connect = mysql_init(NULL);
    if (!mysql_real_connect(connect, server, user, password, database, 0, NULL, CLIENT_MULTI_RESULTS))
    {
        fprintf(stderr, "%s\n\n", mysql_error(connect)); 
        exit(1); 
    } 
    return 0;
} 

int CDBServer::LoginCheck(const char* loginusername, const char* loginpassword)
{

    MYSQL_RES* res;
    MYSQL_ROW row;
    const char *sqlmoudle = "select Game.LoginCheck('%s', '%s')";
    char sql[1024];
    int result = 0;
    sprintf(sql, sqlmoudle, loginusername, loginpassword);
    if (mysql_query(connect, sql))
    {
        fprintf(stderr, "%s\n\n", mysql_error(connect));
        exit(1);
    }
    if ((res = mysql_use_result(connect)))
    {
        if ((row = mysql_fetch_row(res)) != NULL)
        {
            result = atoi(row[0]);
        }
        else
        {
            result = -1;
        }
    }
    else
    {
        result = -1;
    }
    mysql_free_result(res);
    return result;
}
