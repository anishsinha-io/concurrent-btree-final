#ifndef __INDEX
#define __INDEX

#include "ioutil.h"
#include "lehman-yao.h"
#include "errors.h"
#include "defaults.h"


i32 format_index(const char *path, struct b_link_header *header);
i32 read_index_header(const char *path, struct b_link_header *buf);

#endif