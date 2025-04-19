#include <sys/types.h>
#include <string.h>
#include <errno.h>
#include "dict.h"

static size_t CmlDict_hash(u_int8_t *p_key, size_t max)
{
    size_t digest = 0x43;
    for (size_t i = 0; p_key[i] != 0; i++)
        digest += p_key[i] * 31;
    return digest % max;
}

static size_t CmlDict_findKey(struct CmlDict_Dict *p_dict, u_int8_t *p_key)
{
    size_t i = CmlDict_hash(p_key, p_dict->size * 0.8);

    for (size_t j = i; j < p_dict->size && (j - i) < CmlDict_MAX_SEARCH; j++) {
        size_t headerOffset = j * CmlDict_HEADER_SIZE;
        char *p_currKeyField = (char *)(
            p_dict->buff +
            ((p_dict->buff[headerOffset + 3] << 8) | p_dict->buff[headerOffset + 4])
        );
        if ((p_dict->buff[headerOffset] & 0b1) && !strcmp(p_currKeyField, (char*) p_key))
            return j;
    }

    errno = ENOENT;
    return -1;
}

int CmlDict_get(struct CmlDict_Dict *p_dict, u_int8_t *p_key, struct CmlDict_Field *p_value)
{
    size_t i = CmlDict_findKey(p_dict, p_key);
    if (i == -1 && errno == ENOENT)
        return ENOENT;

    size_t headerOffset = i * CmlDict_HEADER_SIZE;
    size_t ref = (p_dict->buff[headerOffset + 1] << 8) | p_dict->buff[headerOffset + 2];
    if (p_dict->len <= ref)
        return errno = EINVAL;

    p_value->value = p_dict->buff + ref;
    p_value->flag = p_dict->buff[headerOffset];
    return 0;
}

inline int CmlDict_has(struct CmlDict_Dict *p_dict, u_int8_t *p_key)
{
    return CmlDict_findKey(p_dict, p_key) != -1 || errno != ENOENT;
}
