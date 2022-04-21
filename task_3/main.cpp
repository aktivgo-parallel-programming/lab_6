#include <iostream>
#include <mpich/mpi.h>
#include <vector>

constexpr auto MSGLEN = 32768;
constexpr auto TAG_A = 100;
constexpr auto TAG_B = 200;

using namespace std;

int main(int argc, char * argv[]) {
    vector<float> message1(MSGLEN), message2(MSGLEN);
    int rank, dest, source, send_tag, recv_tag;

    MPI_Request request;

    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    for (int i = 0; i < MSGLEN; i++) {
        message1[i] = 1 - 2 * rank;
    }

    if (rank == 0) {
        dest = 1;
        source = 1;
        send_tag = TAG_A;
        recv_tag = TAG_B;
    }
    else if (rank == 1) {
        dest = 0;
        source = 0;
        send_tag = TAG_B;
        recv_tag = TAG_A;
    } else {
        return 0;
    }

    MPI_Isend(message1.data(), MSGLEN, MPI_FLOAT, dest, send_tag,
              MPI_COMM_WORLD, &request);
    cout << "Task " << rank << " has sent the message " << message1.data() << endl;


    MPI_Irecv(message2.data(), MSGLEN, MPI_FLOAT, source, recv_tag,
             MPI_COMM_WORLD, &request);
    cout << "Task " << rank << " has received the message " << message2.data() << endl;

    MPI_Finalize();

    return 0;
}