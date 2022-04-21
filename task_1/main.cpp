#include <mpich/mpi.h>

int main(int argc, char **argv) {
    MPI_Init(&argc, &argv);

    int proc_num, proc_rank;
    MPI_Comm_size( MPI_COMM_WORLD, &proc_num);
    MPI_Comm_rank( MPI_COMM_WORLD, &proc_rank);

    printf("I am %d process from %d processes!\n", proc_rank, proc_num);

    MPI_Finalize();

    return 0;
}
