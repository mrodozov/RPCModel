#include "DBconnector.h"

using namespace std;

/*

MYSQL* MysqlConnection(string server, string user, string password, string database){  
  MYSQL *conn;
   conn = mysql_init(NULL);// a must
   // Connect to database 
   if (!mysql_real_connect(conn, server.c_str(),
         user.c_str(), password.c_str(), database.c_str(), 0, NULL, 0)) {
      fprintf(stderr, "%s\n", mysql_error(conn));
      exit(1);
   }
  return conn;
}

*/

// experimental