#include <mpi.h>
#include <iostream>
using namespace std;
int main(){

int numprocs, rank, namelen;

int k;
double result1 = 0;
double result2 = 0;
char processor_name[MPI_MAX_PROCESSOR_NAME];

  MPI_Init(0,0);
  MPI_Comm_size(MPI_COMM_WORLD, &numprocs);
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  MPI_Get_processor_name(processor_name, &namelen);
if (rank == 0){
	k = 2;
	result1 = 2*k;
}
if (rank == 1){
	k = 3;
	result2 = 3*k;
}
  cout << "Process "<< rank << " on "<< processor_name<< " out of "<<numprocs << endl;
   cout << "Result1: " << result1 << ", Result2: " << result2 << endl;
  
  MPI_Finalize();
return 0;
}
