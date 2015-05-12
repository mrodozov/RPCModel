#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <new>
#include <stdlib.h>
/** non standart */
#include "DataObject.h"
#include "get_avg.h"

int main (int argc,char * argv[]){

  get_average_for_list_of_parts(argv[1],argv[2],argv[3]);
  
  return 0;
  
}
