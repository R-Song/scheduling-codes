/*
 * Scheduling 1e3 active users out of 1e6 total users while sending out a variable number of NACKS. 
 * The goal is to see how effective the modified CHD algorithm is.
 */

#include <cmph.h>


int test_joint_schedule_and_nack(cmph_uint32* items_to_hash, cmph_uint32 active_len, cmph_uint32 nack_len, double skew)
{
    cmph_t *hash;
    cmph_config_t *config;
    cmph_io_adapter_t *source;
    char filename[256];
    FILE* mphf_fd = NULL;
    cmph_uint32 items_len = active_len + nack_len;

    // Wrap keys
    source = cmph_io_struct_vector_adapter(
        items_to_hash,
        (cmph_uint32)sizeof(cmph_uint32),
        0,
        (cmph_uint32)sizeof(cmph_uint32),
        items_len
    );

    // Set up config
    config = cmph_config_new(source);
    cmph_config_set_algo(config, CMPH_CHD_PH_N);
    cmph_config_set_graphsize(config, 0.99);
    cmph_config_set_skew(config, skew);
    cmph_config_set_nack(config, nack_len);
    cmph_config_set_verbosity(config, 0);

    // Create perfect hash
    hash = cmph_new(config);
    cmph_config_destroy(config);
    printf("Packed Size: %u bytes for %u keys\n",cmph_packed_size(hash), items_len);
    cmph_io_vector_adapter_destroy(source);   
    cmph_destroy(hash);
    return 0;
}

int test_nack(cmph_uint32* items_to_hash, cmph_uint32 active_len, cmph_uint32 nack_len, double skew)
{
    cmph_t *hash;
    cmph_config_t *config;
    cmph_io_adapter_t *source;
    char filename[256];
    FILE* mphf_fd = NULL;
    cmph_uint32 items_len = active_len + nack_len;

    // Wrap keys
    source = cmph_io_struct_vector_adapter(
        items_to_hash,
        (cmph_uint32)sizeof(cmph_uint32),
        0,
        (cmph_uint32)sizeof(cmph_uint32),
        items_len
    );

    // Set up config
    config = cmph_config_new(source);
    cmph_config_set_algo(config, RANDOM_HASH);
    cmph_config_set_graphsize(config, 0.99);
    cmph_config_set_skew(config, skew);
    cmph_config_set_nack(config, nack_len);
    cmph_config_set_verbosity(config, 0);

    // Create perfect hash
    hash = cmph_new(config);
    cmph_config_destroy(config);
    printf("Packed Size: %u bytes for %u keys\n",cmph_packed_size(hash), items_len);
    cmph_io_vector_adapter_destroy(source);   
    cmph_destroy(hash);
    return 0;
}

int test_schedule(cmph_uint32* items_to_hash, cmph_uint32 items_len) {
    cmph_t *hash;
    cmph_config_t *config;
    cmph_io_adapter_t *source;
    char filename[256];
    FILE* mphf_fd = NULL;

    // Wrap keys
    source = cmph_io_struct_vector_adapter(
        items_to_hash,
        (cmph_uint32)sizeof(cmph_uint32),
        0,
        (cmph_uint32)sizeof(cmph_uint32),
        items_len
    );

    // Set up config
    config = cmph_config_new(source);
    cmph_config_set_algo(config, CMPH_CHD_PH);
    cmph_config_set_graphsize(config, 0.99);
    cmph_config_set_verbosity(config, 0);

    // Create perfect hash
    hash = cmph_new(config);
    cmph_config_destroy(config);
    printf("Packed Size: %u bytes for %u keys\n",cmph_packed_size(hash), items_len);
    cmph_io_vector_adapter_destroy(source);   
    cmph_destroy(hash);
    return 0;
}

int main (void)
{
    cmph_uint32 total_users = 1000000;
    cmph_uint32 active_len = 1000;
    cmph_uint32 nack_len = 1000;
    cmph_uint32 keys[active_len + nack_len];
    double skew[12] = {0.5, 0.6, 0.7, 0.8, 0.9, 1.0, 1.1, 1.2, 1.3, 1.4, 1.5, 1.6};

    // Generate random vector of active users
    cmph_uint32 i;
    cmph_uint32 T[total_users];
    for(i = 0; i < (active_len + nack_len); i++) {
        keys[i] = rand() % total_users;
        while(T[keys[i]] == 1) {
            keys[i] = rand() % total_users;
        }
        T[keys[i]] = 1;
    }

    // Test joint schedule and nack
    printf("Joint Schedule Nack: %u ACTIVE, %u NACKS, %u TOTAL\n\n", active_len, nack_len, total_users);
    float skew_factor = 1;
    test_joint_schedule_and_nack(keys, active_len, nack_len, skew_factor);
    printf("\n\n");

    // Test schedule followed by nack
    printf("Schedule then Nack: %u ACTIVE, %u NACKS, %u TOTAL\n\n", active_len, nack_len, total_users);
    test_nack(keys, active_len, nack_len, 1);
    test_schedule(keys, active_len);
    printf("\n\n"); 

    // Benchmark Against CHD_PH
    printf("CHD_PH Benchmark: %u\n", (active_len+nack_len));
    test_schedule(keys, active_len+nack_len);
    printf("\n\n");

    return 0;
}
