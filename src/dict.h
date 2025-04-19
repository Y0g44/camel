#ifndef __DICT_H
#define __DICT_H

#include <sys/types.h>
#include "def.h"

#define CmlDict_DICT_SIZE 1024
#define CmlDict_MAX_SEARCH 512
#define CmlDict_HEADER_SIZE 5

extern u_int8_t *CmlDict_DICT;

struct CmlDict_Dict {
    u_int8_t *buff;
    size_t len;
    size_t size;
};

struct CmlDict_Field {
    u_int8_t *value;
    u_int8_t flag;
};

int CmlDict_get(struct CmlDict_Dict *p_dict, u_int8_t *p_key, struct CmlDict_Field *p_value);
int CmlDict_has(struct CmlDict_Dict *p_dict, u_int8_t *p_key);

// void CmlDict_parse();

#endif
