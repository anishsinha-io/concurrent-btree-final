#ifndef __INDEX
#define __INDEX

#include "ioutil.h"
#include "lehman-yao.h"
#include "errors.h"
#include "defaults.h"


i32 format_index(const char *path, char *table_name, size_t node_size);
i32 read_index_header(const char *path, struct b_link_header *buf);
i32 write_index_page(const char *path, struct b_link_node *buf);
i32 read_index_page(const char *path, u64 loc, struct b_link_node *buf);
i32 b_link_insert(char *path, struct b_link_entry *entry);

#endif