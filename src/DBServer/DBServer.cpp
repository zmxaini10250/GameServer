#include "DBServer.h"

#include <stdio.h>
#include <mysql/mysql.h>
#include <stdlib.h>

#include "../protobuf/ClientMessage.pb.h"

CDBServer::CDBServer()
{
    ConnectMysql();
}

int CDBServer::ConnectMysql()
{ 
    connect = mysql_init(NULL);
    if (!mysql_real_connect(connect, server, user, password, database, 0, NULL, CLIENT_MULTI_RESULTS))
    {
        fprintf(stderr, "%s\n\n", mysql_error(connect)); 
        return -1;
    } 
    return 0;
} 

int CDBServer::LoginCheck(const char* LoginUsername, const char* LoginPassword, PBS2CLoginCheckRes& PBRes)
{
    printf("LoginCheck\n");
    printf("username:%s password:%s\n", LoginUsername, LoginPassword);
    const char *sqlmoudle = "select Game.LoginCheck('%s', '%s')";

    MYSQL_RES* res;
    MYSQL_ROW row;

    char sql[1024];
    int result = 0;
    sprintf(sql, sqlmoudle, LoginUsername, LoginPassword);
    if (mysql_query(connect, sql))
    {
        fprintf(stderr, "%s\n\n", mysql_error(connect));
        return -1;
    }
    if ((res = mysql_use_result(connect)))
    {
        if ((row = mysql_fetch_row(res)) != NULL)
        {
            PBRes.set_logincheckresult(atoi(row[0]));
        }
        else
        {
            return -1;
        }
    }
    else
    {
        return -1;
    }
    mysql_free_result(res);
    printf("result:%d\n", PBRes.logincheckresult());
    return 0;
}

int CDBServer::LoginRegister(const char* LoginUsername, const char* LoginPassword, PBS2CLoginRegisterRes& PBRes)
{
    printf("LoginRegister\n");
    printf("username:%s password:%s\n", LoginUsername, LoginPassword);
    printf("result:\n");
}

int CDBServer::CreateUser(int LoginID, const char* PlayName, PBS2CCreateUserRes& PBRes)
{
}

int CDBServer::DeleteUser(int LoginID, PBS2CDeleteUserRes& PBRes)
{
}
