/*
 * Scheduling 1e3 active users out of 1e6 total users while sending out a variable number of NACKS. 
 * The goal is to see how effective the modified CHD algorithm is.
 */

#include <cmph.h>

/* Run a particular hash algorithm */
int test(cmph_uint32* items_to_hash, cmph_uint32 items_len, CMPH_ALGO alg_n)
{
    cmph_t *hash;
    cmph_config_t *config;
    cmph_io_adapter_t *source;
    char filename[256];
    FILE* mphf_fd = NULL;

    source = cmph_io_struct_vector_adapter(
        items_to_hash,
        (cmph_uint32)sizeof(cmph_uint32),
        0,
        (cmph_uint32)sizeof(cmph_uint32),
        items_len
    );

    config = cmph_config_new(source);
    cmph_config_set_algo(config, alg_n);
    cmph_config_set_graphsize(config, 5.00);
    cmph_config_set_verbosity(config, 1);

    if (alg_n == CMPH_BRZ) {
        sprintf(filename, "%s_%u.mph", cmph_names[alg_n], items_len);
        mphf_fd = fopen(filename, "w");
        cmph_config_set_mphf_fd(config, mphf_fd);
    }
    hash = cmph_new(config);
    cmph_config_destroy(config);

    printf("Packed Size: %u bytes for %u keys\n",cmph_packed_size(hash), items_len);

    cmph_io_vector_adapter_destroy(source);   
    cmph_destroy(hash);

    if (alg_n == CMPH_BRZ) {
        fclose(mphf_fd);
    }
    return 0;
}

int main (void)
{
    cmph_uint32 total_users = 1000000;
    cmph_uint32 active_len = 1000;
    cmph_uint32 active[active_len];
    cmph_uint32 nack_len = 500;
    cmph_uint32 nack[nack_len];

    // Generate random vector of active users
    cmph_uint32 i;
    cmph_uint32 T[total_users];
    for(i = 0; i < active_len; i++) {
        active[i] = rand() % total_users;
        while(T[active[i]] == 1) {
            active[i] = rand() % total_users;
        }
        T[active[i]] = 1;
    }
    for(i = 0; i < nack_len; i++) {
        nack[i] = rand() % total_users;
        while(T[nack[i]] == 1) {
            nack[i] = rand() % total_users;
        }
        T[nack[i]] = 1;
    }

    printf("CHDN: %u ACTIVE, %u NACKS, %u TOTAL\n", active_len, nack_len, total_users);
    // TODO: Actually implement that algorithm...
    test(active, active_len, CMPH_CHD);

    return 0;
}
