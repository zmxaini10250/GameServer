#include "DBServer.h"

#include <stdio.h>
#include <mysql/mysql.h>
#include <stdlib.h>
#include <google/protobuf/message.h>
#include <google/protobuf/wire_format.h>

#include "../protobuf/ClientMessage.pb.h"
#include "../Hero/Hero.h"
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
    const char *sqlmoudle = "select Game.LoginRegister('%s', '%s')";

    MYSQL_RES* res;
    MYSQL_ROW row;

    char sql[1024];
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
            PBRes.set_loginregisterresult(atoi(row[0]));
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
    printf("result:%d\n", PBRes.loginregisterresult());
    printf("result:\n");
    return 0;
}

int CDBServer::CreateUser(int LoginID, const char* PlayName, PBS2CCreateUserRes& PBRes)
{
    printf("CreateUser\n");
    printf("Loginid:%d playername:%s\n", LoginID, PlayName);
    const char *sqlmoudle = "select Game.PlayerCreate('%d', '%s')";

    MYSQL_RES* res;
    MYSQL_ROW row;

    char sql[1024];
    sprintf(sql, sqlmoudle, LoginID, PlayName);
    if (mysql_query(connect, sql))
    {
        fprintf(stderr, "%s\n\n", mysql_error(connect));
        return -1;
    }
    if ((res = mysql_use_result(connect)))
    {
        if ((row = mysql_fetch_row(res)) != NULL)
        {
            PBRes.set_createuserresult(atoi(row[0]));
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
    printf("result:%d\n", PBRes.createuserresult());
    printf("result:\n");
    return 0;
}

int CDBServer::DeleteUser(int LoginID, PBS2CDeleteUserRes& PBRes)
{
    printf("DeleteUser\n");
    printf("Loginid:%d\n", LoginID);
    const char *sqlmoudle = "select Game.PlayerDelete('%d')";

    MYSQL_RES* res;
    MYSQL_ROW row;

    char sql[1024];
    sprintf(sql, sqlmoudle, LoginID);
    if (mysql_query(connect, sql))
    {
        fprintf(stderr, "%s\n\n", mysql_error(connect));
        return -1;
    }
    if ((res = mysql_use_result(connect)))
    {
        if ((row = mysql_fetch_row(res)) != NULL)
        {
            PBRes.set_deleteuserresult(atoi(row[0]));
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
    printf("result:%d\n", PBRes.deleteuserresult());
    printf("result:\n");
    return 0;
}

int CDBServer::SavePlayerData(const DBPlayer& db)
{
    printf("SavePlayerData\n");
    printf("PlayerID:%d\n", db.playerid());
    const char *sqlmoudle = "select Game.PlayerSave('%d', '%d', '%d' ,'";
    MYSQL_RES* res;
    MYSQL_ROW row;
    
    int size = db.hero().GetCachedSize();
    char *buffer = new char[size + 1];
    db.SerializeToArray(buffer, size + 1);
    char *sql = new char[size + 1024 + 1];

    memset(sql, 0, size + 1024 + 1);
    int result = 0;

    sprintf(sql, sqlmoudle, db.playerid(), db.gold(), db.empirical());
    int length = strlen(sql);
    memcpy(sql + length, buffer, size);
    memcpy(sql + length + size, "')", 2);
    if (mysql_query(connect, sql))
    {
        fprintf(stderr, "%s\n\n", mysql_error(connect));
        return -1;
    }
    if ((res = mysql_use_result(connect)))
    {
        if ((row = mysql_fetch_row(res)) != NULL)
        {
            result = (atoi(row[0]));
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
    printf("result:%d\n", result);
    return result;
}

int CDBServer::GetPlayerData(int LoginID, DBPlayer& db)
{
    printf("GetPlayerData\n");
    printf("LoginID:%d\n", LoginID);
    const char *sqlmoudle = "SELECT `Game`.`player`.`idplayer`, `Game`.`player`.`playername`, `Game`.`player`.`gold`, `Game`.`player`.`empirical`, `Game`.`player`.`playerhero` FROM `Game`.`player` where `Game`.`player`.`idlogin`=%d";
    MYSQL_RES* res;
    MYSQL_ROW row;

    char sql[1024];
    sprintf(sql, sqlmoudle, LoginID);
    if (mysql_query(connect, sql))
    {
        fprintf(stderr, "%s\n\n", mysql_error(connect));
        return -1;
    }
    if ((res = mysql_use_result(connect)))
    {
        if ((row = mysql_fetch_row(res)) != NULL)
        {
            db.set_playerid(atoi(row[0]));
            db.set_playername(row[1]);
            db.set_gold(atoi(row[2]));
            db.set_empirical(atoi(row[3]));
            if (mysql_fetch_lengths(res)[4] != 0)
            {
                db.mutable_hero()->ParseFromArray(row[4], mysql_fetch_lengths(res)[4]);
            }
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
    return 0;
}

int CDBServer::GetHeroData(int HeroID, CHeroAttribute& hero)
{
    printf("GetHeroData\n");
    printf("HeroID:%d\n", HeroID);
    const char *sqlmoudle = "SELECT `hero`.`speed`, `hero`.`defend`, `hero`.`avoid`, `hero`.`attach`, `hero`.`basehealthpoint`, `hero`.`healthpointgrow` FROM `Game`.`hero` where `hero`.`idhero`='%d'";
    MYSQL_RES* res;
    MYSQL_ROW row;

    char sql[1024];
    sprintf(sql, sqlmoudle, HeroID);
    if (mysql_query(connect, sql))
    {
        fprintf(stderr, "%s\n\n", mysql_error(connect));
        return -1;
    }
    if ((res = mysql_use_result(connect)))
    {
        if ((row = mysql_fetch_row(res)) != NULL)
        {
            hero.SetSpeed(atoi(row[0]));
            hero.SetDefend(atoi(row[1]));
            hero.SetAvoid(atoi(row[2]));
            hero.SetAttach(atoi(row[3]));
            hero.SetBaseHealthPoint(atoi(row[4]));
            hero.SetHealthPointGrow(atoi(row[5]));
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

    return 0;
}
