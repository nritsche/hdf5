#include "h5dsm_example.h"

int main(int argc, char *argv[]) {
    uuid_t pool_uuid;
    char *pool_grp = "daos_tier0";
    hid_t file = -1, fapl = -1;

    (void)MPI_Init(&argc, &argv);
    (void)daos_init();

    if(argc != 3)
        PRINTF_ERROR("argc != 3\n");

    /* Parse UUID */
    if(0 != uuid_parse(argv[1], pool_uuid))
        ERROR;

    /* Set up FAPL */
    if((fapl = H5Pcreate(H5P_FILE_ACCESS)) < 0)
        ERROR;
    if(H5Pset_fapl_daosm(fapl, MPI_COMM_WORLD, MPI_INFO_NULL, pool_uuid, pool_grp) < 0)
        ERROR;

    /* Open file */
    if((file = H5Fopen_ff(argv[2], H5F_ACC_RDONLY, fapl, NULL)) < 0)
        ERROR;

    /* Close */
    if(H5Fclose_ff(file, -1) < 0)
        ERROR;
    if(H5Pclose(fapl) < 0)
        ERROR;

    printf("Success\n");
    (void)daos_fini();
    (void)MPI_Finalize();

    return 0;

error:
    H5E_BEGIN_TRY {
        H5Fclose_ff(file, -1);
        H5Pclose(fapl);
    } H5E_END_TRY;

    (void)daos_fini();
    (void)MPI_Finalize();
    return 1;
}
