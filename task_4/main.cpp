#include <iostream>
#include <mpich/mpi.h>

constexpr auto MSGLEN = 1;

int main(int argc, char * argv[]) {

    int dest, source;

    MPI_Status status;

    MPI_Init(&argc, &argv);

    int proc_num, proc_rank;
    MPI_Comm_size( MPI_COMM_WORLD, &proc_num);
    MPI_Comm_rank( MPI_COMM_WORLD, &proc_rank);

    int number = 0;
    int numberCopy;
    if (proc_rank == 0) {
        number = rand() % 10;
        numberCopy = number;
        dest = proc_rank + 1;
        source = proc_num - 1;

        MPI_Send(&number, MSGLEN, MPI_INT, dest, 1,
                 MPI_COMM_WORLD);
        printf("%d send number %d\n", proc_rank, number);

        MPI_Recv(&number, MSGLEN, MPI_INT, source, 1,
                 MPI_COMM_WORLD, &status);
        printf("%d received number %d\n", proc_rank, number);

        if (number - numberCopy == proc_num - 1) {
            std::cout << "Correct\n" << std::endl;
        } else {
            std::cout << "Error\n" << std::endl;
        }
    } else {
        if (proc_rank > 0 && proc_rank < proc_num - 1) {
            dest = proc_rank + 1;
            source = proc_rank - 1;
        } else {
            dest = 0;
            source = proc_rank - 1;
        }

        MPI_Recv(&number, MSGLEN, MPI_INT, source, 1,
                 MPI_COMM_WORLD, &status);
        printf("%d received number %d\n", proc_rank, number);

        number++;

        MPI_Send(&number, MSGLEN, MPI_INT, dest, 1,
                 MPI_COMM_WORLD);
        printf("%d send number %d\n", proc_rank, number);
    }

    MPI_Finalize();

    return 0;
}