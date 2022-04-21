#include <mpich/mpi.h>

int main(int argc, char **argv) {
    MPI_Init(&argc, &argv);

    int proc_num, proc_rank;
    MPI_Comm_size( MPI_COMM_WORLD, &proc_num);
    MPI_Comm_rank( MPI_COMM_WORLD, &proc_rank);

    if (proc_rank == 0) {
        printf("%d processes\n", proc_num);
        return 0;
    }

    switch (proc_rank % 2) {
        case 0:
            printf("%d SECOND!\n", proc_rank);
            break;
        case 1:
            printf("%d FIRST!\n", proc_rank);
            break;
    }

    MPI_Finalize();

    return 0;
}
