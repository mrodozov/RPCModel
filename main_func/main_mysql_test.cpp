/** Author: Mircho Rodozov mrodozov@cern.ch
 * Date 17.05.2013
 * Info added at SVN revision 80
 * 
*/

#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <new>
#include <stdlib.h>
#include <typeinfo>
#include <math.h>
#include <mysql/mysql.h>
#include "../core/DataObject.h"
#include "../core/ExtendedStrip.h"
#include "../core/ExtendedRoll.h"
#include "../core/Chip.h"
#include "../Applications.h"
#include "../ROOT/ROOT.h"
#include "../ServiceClasses/Service.h"
#include "../ServiceClasses/JSON/json/json.h"
#include "../ServiceClasses/DBconnector.h"

int main (int argc,char * argv[]){
  
  /**
   * 
   */
  
  MYSQL_RES *res;
  MYSQL_ROW row;
  
  string host="localhost",user="rodozov",pass="",dbname="test",query="INSERT INTO runsRate values (5,'JSON'),(6,'JSON')";
  MYSQL * conn = MysqlConnection(host,user,pass,dbname);
  if (mysql_query(conn, query.c_str())) {
      fprintf(stderr, "%s\n", mysql_error(conn));
      exit(1);
  }
  
  res = mysql_use_result(conn);
  printf("MySQL Tables in mysql database:\n");
  
  mysql_free_result(res);
  // close it
  mysql_close(conn);
  
  return 0;
}