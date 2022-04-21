#include <iostream>
#include <mpich/mpi.h>
#include <vector>

std::vector<std::vector<int>> create_random_vectors(int, int);
std::vector<int> create_random_vector(int);

int main(int argc, char * argv[]) {
    int n = 5, m = 5;

//    std::cout << "Enter n: ";
//    std::cin >> n;
//
//    std::cout << "\nEnter m: ";
//    std::cin >> m;

    MPI_Status status;

    std::vector<std::vector<int>> vectors = create_random_vectors(n, m);
    std::vector<int> sums;

    MPI_Init(&argc, &argv);

    int dest, source;

    int proc_num, proc_rank;
    MPI_Comm_size( MPI_COMM_WORLD, &proc_num);
    MPI_Comm_rank( MPI_COMM_WORLD, &proc_rank);

    if (proc_rank == 0) {
        for (int i = 0; i < vectors.size(); i += proc_num - 1) {
            for (int j = 1; j < proc_num; j++) {
                dest = j;
                source = j;
                MPI_Send(vectors[i + j - 1].data(), m, MPI_INT, dest, 1,
                         MPI_COMM_WORLD);
                printf("%d send vector %d\n", proc_rank, i + j - 1);
                int sum = 0;
                MPI_Recv(&sum, 1, MPI_INT, source, 1,
                         MPI_COMM_WORLD, &status);
                printf("%d received sum %d\n", proc_rank, sum);

                sums.push_back(sum);
            }
        }

        int resultSum = 0;
        for (int sum : sums) {
            resultSum += sum;
        }
        printf("%d result sum %d", proc_rank, resultSum);
    } else {
        dest = 0;
        source = 0;

        while(true) {
            std::vector<int> vector(m);
            MPI_Recv(vector.data(), m, MPI_INT, source, 1,
                     MPI_COMM_WORLD, &status);
            printf("%d received vector\n", proc_rank);

            int sum = 0;
            for (int i : vector) {
                sum += i;
            }

            MPI_Send(&sum, 1, MPI_INT, dest, 1,
                     MPI_COMM_WORLD);
            printf("%d send sum %d\n", proc_rank, sum);
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