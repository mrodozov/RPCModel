#ifndef DB_CONNECTOR_H
#define DB_CONNECTOR_H

#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <new>
#include <stdlib.h>
#include <stdio.h>
#include <typeinfo>
#include <mysql/mysql.h>

using namespace std;

MYSQL * MysqlConnection (string server,string user,string password,string database);

// experimental

#endif
