#include <iostream>
#include <mpich/mpi.h>
#include <vector>
#include <ctime>
#include <random>

std::vector<std::vector<int>> create_random_vectors(int, int);
std::vector<int> create_random_vector(int);
void proc_print_vector(int, int, const std::vector<int>&);

int main(int argc, char * argv[]) {
    srand(time(nullptr));
    MPI_Status status;

    MPI_Init(&argc, &argv);

    int proc_num, proc_rank;
    MPI_Comm_size( MPI_COMM_WORLD, &proc_num);
    MPI_Comm_rank( MPI_COMM_WORLD, &proc_rank);

    int num;
    std::vector<int> row = create_random_vector(proc_num);

    proc_print_vector(proc_rank, proc_num, row);

    MPI_Barrier(MPI_COMM_WORLD);

    for(int i = 0; i < proc_num; i++) {
        if (i == proc_rank) continue;
        MPI_Send(&row[i], 1, MPI_INT, i, i, MPI_COMM_WORLD);
        MPI_Recv(&num, 1, MPI_INT, i, proc_rank,MPI_COMM_WORLD, &status);
        row[i] = num;
    }

    proc_print_vector(proc_rank, proc_num, row);

    MPI_Finalize();

    return 0;
}

std::vector<std::vector<int>> create_random_vectors(int count, int size) {
    std::vector<std::vector<int>> vectors;

    for (int i = 0; i < count; ++i) {
        std::vector<int> vector = create_random_vector(size);
        vectors.push_back(vector);
    }

    return vectors;
}

std::vector<int> create_random_vector(int size) {
    std::vector<int> vector;

    for (int i = 0; i < size; ++i) {
        vector.push_back(i);
    }

    return vector;
}

void proc_print_vector(int proc_rank, int proc_num, const std::vector<int>& vector) {
    int buf = 0;
    MPI_Status status;

    if (proc_rank != 0) {
        MPI_Recv(&buf, 1, MPI_INT, proc_rank - 1, MPI_ANY_TAG,MPI_COMM_WORLD, &status);
    }
    for(int element : vector) {
        printf("%d ", element);
    }
    printf("\n");
    MPI_Send(&buf, 1, MPI_INT, proc_rank == proc_num - 1 ? 0 : proc_rank + 1, 0, MPI_COMM_WORLD);

    if (proc_rank == proc_num - 1) {
        printf("\n");
    }
}