#ifndef _DBSERVER_H_
#define _DBSERVER_H_

#include <mysql/mysql.h>
#include "../ObjectPool/SingletonObject.hpp"
#include "../protobuf/ClientMessage.pb.h"

class CHeroAttribute;
class CDBServer;

typedef CSingletonObject<CDBServer> DBServer;

class CDBServer
{
    public:
        int LoginCheck(const char* LoginUsername, const char* LoginPassword, PBS2CLoginCheckRes& PBRes);
        int LoginRegister(const char* LoginUsername, const char* LoginPassword, PBS2CLoginRegisterRes& PBRes);
        int CreateUser(int LoginID, const char* PlayName, PBS2CCreateUserRes& PBRes);
        int DeleteUser(int LoginID, PBS2CDeleteUserRes& PBRes);
        int SavePlayerData(const DBPlayer& db);
        int GetPlayerData(int LoginID, DBPlayer& db);
        int GetHeroData(int HeroID, CHeroAttribute& hero);
    protected:
        const char* server = "localhost";
        const char* user = "root";
        const char* password = "root";
        const char* database = "Game";
        
        MYSQL *connect;
        int ConnectMysql();
        CDBServer();
        ~CDBServer(){}
};

#endif
