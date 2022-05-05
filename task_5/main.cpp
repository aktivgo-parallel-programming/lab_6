#include <iostream>
#include <mpich/mpi.h>
#include <vector>

std::vector<std::vector<int>> create_random_vectors(int, int);
std::vector<int> create_random_vector(int);

int main(int argc, char * argv[]) {
    MPI_Status status;

    MPI_Init(&argc, &argv);

    int proc_num, proc_rank;
    MPI_Comm_size( MPI_COMM_WORLD, &proc_num);
    MPI_Comm_rank( MPI_COMM_WORLD, &proc_rank);

    if (proc_rank == 0) {
        int n = 0, m = 0;

        std::cout << "Enter n: ";
        std::cin >> n;

        std::cout << "Enter m: ";
        std::cin >> m;

        std::vector<std::vector<int>> vectors = create_random_vectors(n, m);
        std::vector<int> sums;

        int countRows = n / (proc_num - 1);
        int countRowsLast = countRows + n % (proc_num - 1);
        for(int i = 1; i < proc_num; i++) {
            if (i != proc_num - 1) {
                MPI_Send(&countRows, 1, MPI_INT, i, 0,MPI_COMM_WORLD);
            } else {
                MPI_Send(&countRowsLast, 1, MPI_INT, i, 0,MPI_COMM_WORLD);
            }

            MPI_Send(&m, 1, MPI_INT, i, 1,MPI_COMM_WORLD);
        }

        for(int i = 0; i < vectors.size(); i++) {
            int targetThread = i % (proc_num - 1) + 1;

            if (i >= countRowsLast - 1) {
                targetThread = proc_num - 1;
            }

            MPI_Send(vectors[i].data(), m, MPI_INT, targetThread, targetThread,MPI_COMM_WORLD);
            printf("%d send row to %d\n", proc_rank, targetThread);

            int sum = 0;
            MPI_Recv(&sum, 1, MPI_INT, targetThread, targetThread,MPI_COMM_WORLD, &status);
            printf("%d received sum from %d\n", proc_rank, targetThread);

            sums.push_back(sum);
        }

        int resultSum = 0;
        for (int sum : sums) {
            resultSum += sum;
        }
        printf("%d result sum %d\n", proc_rank, resultSum);
    } else {
        int n;
        MPI_Recv(&n, 1, MPI_INT, 0, 0,MPI_COMM_WORLD, &status);
        printf("received n=%d\n", n);

        int m;
        MPI_Recv(&m, 1, MPI_INT, 0, 1,MPI_COMM_WORLD, &status);

        for(int i = 0; i < n; i++) {
            std::vector<int> vector(m);

            MPI_Recv(vector.data(), m, MPI_INT, 0, proc_rank, MPI_COMM_WORLD, &status);

            int sum = 0;
            for (int item : vector) {
                sum += item;
            }

            MPI_Send(&sum, 1, MPI_INT, 0, proc_rank,MPI_COMM_WORLD);
        }
    }

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
        vector.push_back(rand() % 10);
    }

    return vector;
}