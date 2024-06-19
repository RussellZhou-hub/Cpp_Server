#include<iostream>
#include<mariadb/mysql.h>

struct connection_details
{
    const char *server, *user, *password, *database;
};

MYSQL* mysql_connection_setup(connection_details mysql_details){
    MYSQL* connection = mysql_init(NULL);

    
    // MYSQL *		STDCALL mysql_real_connect(MYSQL *mysql, const char *host,
	// 				   const char *user,
	// 				   const char *passwd,
	// 				   const char *db,
	// 				   unsigned int port,
	// 				   const char *unix_socket,
	// 				   unsigned long clientflag);
    if(!mysql_real_connect(connection, mysql_details.server,mysql_details.user,mysql_details.password,mysql_details.database,0,NULL,0))
    {
        std::cout<<"Connection Error: "<< mysql_error(connection) << std::endl;
        exit(1);
    }

    return connection;
}

MYSQL_RES* mysql_execute_query(MYSQL *connection, const char *sql_query)
{
    if(mysql_query(connection,sql_query))
    {
        std::cout<<"MYSQL Query Error: "<< mysql_error(connection) << std::endl;
        exit(1);
    }

    return mysql_use_result(connection);
}