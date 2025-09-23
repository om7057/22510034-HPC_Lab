#include <mpi.h>
#include <stdio.h>

int main(int argc, char **argv)
{
    MPI_Init(&argc, &argv);

    int world_size, world_rank;
    MPI_Comm_size(MPI_COMM_WORLD, &world_size);
    MPI_Comm_rank(MPI_COMM_WORLD, &world_rank);

    // associating a group with MPI_COMM_WORLD
    MPI_Group world_group;
    MPI_Comm_group(MPI_COMM_WORLD, &world_group);

    int group_size, group_rank;
    MPI_Group_size(world_group, &group_size);
    MPI_Group_rank(world_group, &group_rank);

    printf("Process %d\n", world_rank);
    printf("World size: %d\n", world_size);
    printf("Group Rank: %d\n", group_rank);
    printf("Group Size: %d\n", group_size);
    printf("Communicator: MPI_COMM_WORLD\n");

    MPI_Group_free(&world_group);

    MPI_Finalize();
    return 0;
}