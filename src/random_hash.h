#ifndef _RANDOM_HASH_N__
#define _RANDOM_HASH_N__

#include "cmph.h"

typedef struct __random_hash_data_t random_hash_data_t;
typedef struct __random_hash_config_data_t random_hash_config_data_t;

/* Config API */
random_hash_config_data_t *random_hash_config_new(void);
void random_hash_config_set_hashfuncs(cmph_config_t *mph, CMPH_HASH *hashfuncs);

/** \fn void random_hash_config_set_keys_per_bin(cmph_config_t *mph, cmph_uint32 keys_per_bin);
 *  \brief Allows to set the number of keys per bin.
 *  \param mph pointer to the configuration structure
 *  \param keys_per_bin value for the number of keys per bin 
 */
void random_hash_config_set_keys_per_bin(cmph_config_t *mph, cmph_uint32 keys_per_bin);

/** \fn void random_hash_config_set_b(cmph_config_t *mph, cmph_uint32 keys_per_bucket);
 *  \brief Allows to set the number of keys per bucket.
 *  \param mph pointer to the configuration structure
 *  \param keys_per_bucket value for the number of keys per bucket 
 */
void random_hash_config_set_b(cmph_config_t *mph, cmph_uint32 keys_per_bucket);
void random_hash_config_destroy(cmph_config_t *mph);


/* Chd algorithm API */
cmph_t *random_hash_new(cmph_config_t *mph, double c, double a);
void random_hash_load(FILE *fd, cmph_t *mphf);
int random_hash_dump(cmph_t *mphf, FILE *fd);
void random_hash_destroy(cmph_t *mphf);
cmph_uint32 random_hash_search(cmph_t *mphf, const char *key, cmph_uint32 keylen);

/** \fn void random_hash_pack(cmph_t *mphf, void *packed_mphf);
 *  \brief Support the ability to pack a perfect hash function into a preallocated contiguous memory space pointed by packed_mphf.
 *  \param mphf pointer to the resulting mphf
 *  \param packed_mphf pointer to the contiguous memory area used to store the resulting mphf. The size of packed_mphf must be at least cmph_packed_size() 
 */
void random_hash_pack(cmph_t *mphf, void *packed_mphf);

/** \fn cmph_uint32 random_hash_packed_size(cmph_t *mphf);
 *  \brief Return the amount of space needed to pack mphf.
 *  \param mphf pointer to a mphf
 *  \return the size of the packed function or zero for failures
 */ 
cmph_uint32 random_hash_packed_size(cmph_t *mphf);

/** cmph_uint32 random_hash_search(void *packed_mphf, const char *key, cmph_uint32 keylen);
 *  \brief Use the packed mphf to do a search. 
 *  \param  packed_mphf pointer to the packed mphf
 *  \param key key to be hashed
 *  \param keylen key legth in bytes
 *  \return The mphf value
 */
cmph_uint32 random_hash_search_packed(void *packed_mphf, const char *key, cmph_uint32 keylen);

#endif
