#ifndef _DBSERVER_H_
#define _DBSERVER_H_

#include <mysql/mysql.h>
#include "../ObjectPool/SingletonObject.hpp"

class CDBServer;

typedef CSingletonObject<CDBServer> DBServer;

class CDBServer
{
    public:
        int LoginCheck(const char* loginusername, const char* loginpassword);
    protected:
        const char* server = "localhost";
        const char* user = "root";
        const char* password = "root";
        const char* database = "Game";
        
        MYSQL *connect;
        int ConnectMysql();
        CDBServer(){}
        ~CDBServer(){}
};

#endif
