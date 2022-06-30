/*
** June 17, 2022
**
** The author disclaims copyright to this source code.  In place of
** a legal notice, here is a blessing:
**
**    May you do good and not evil.
**    May you find forgiveness for yourself and forgive others.
**    May you share freely, never taking more than you give.
**
***********************************************************************************************************************
**
** This file is part of the `slice` module. This part of the module contains implementations of functions used for
** creating, manipulating, and utilizing dynamic slice objects. Slice objects are meant to be a more convenient way to
** work with C arrays, which are, in general, limited in their use. Slices implemented here are fully featured,
** java.lang.ArrayList or std::vector type structures, fully capable of dynamically resizing themselves. They may be
** searched through, sorted efficiently, and inserted into at any point with simple function calls.
**
*/

#include "slice.h"

static void slice_resize(struct slice *s, u64 capacity) {
    if (capacity == s->length || capacity < 10) return;
    s->keys     = realloc(s->keys, sizeof(void *) * capacity);
    s->capacity = capacity;
}

static void slice_autoresize(struct slice *s) {
    if (s->length < (u64) floor((double) s->capacity / 4)) slice_resize(s, s->capacity / 2);
    if (s->capacity < 10 && s->length == s->capacity) slice_resize(s, s->capacity * 2 + 1);
    else slice_resize(s, ceil((double) s->capacity * 1.5));
}

__attribute__((nonnull))
u64 slice_len(const struct slice *s) {
    return s->length;
}

struct slice *slice(slice_cmpfunc compare, printfunc print) {
    struct slice *s = malloc(sizeof(struct slice));
    s->length   = 0;
    s->capacity = INITIAL_SLICE_CAPACITY;
    s->keys     = malloc(sizeof(void *) * s->capacity);
    s->compare  = compare;
    s->print    = print;
    return s;
}

void delete_slice(struct slice *s) {
    s->keys = NULL;
    free(s->keys);
    s->length   = 0;
    s->capacity = 0;
    s = NULL;
    free(s);
}

struct slice *make_slice(void *keys, u64 capacity, size_t size, slice_cmpfunc compare, printfunc print) {
    struct slice *s = slice(compare, print);
    slice_resize(s, (u64) ((double) capacity * 1.5));
    for (int i = 0; i < s->capacity; i++) s->keys[i] = keys + size * i;
    s->length = capacity;
    return s;
}

struct slice *subslice(struct slice *s, u64 start, u64 end) {
    struct slice *ss = slice(s->compare, s->print);
    slice_resize(ss, end - start);
    memcpy(ss->keys, &(s->keys[start]), sizeof(void *) * (end - start));
    ss->length = end - start;
    return ss;
}

void slice_insert_front(struct slice *s, void *key) {
    slice_autoresize(s);
    memcpy(&(s->keys[1]), s->keys, sizeof(void *) * s->length);
    s->length++;
    s->keys[0] = key;
}

void slice_insert_back(struct slice *s, void *key) {
    slice_autoresize(s);
    s->keys[s->length++] = key;
}

void *slice_delete_front(struct slice *s) {
    if (s->length == 0) return NULL;
    void *key = s->keys[0];
    s->keys[0] = NULL;
    free(s->keys[0]);
    memcpy(s->keys, &(s->keys[1]), sizeof(void *) * (s->length - 1));
    s->length--;
    slice_autoresize(s);
    return key;
}

void *slice_delete_back(struct slice *s) {
    if (s->length == 0) return NULL;
    void *key = s->keys[s->length - 1];
    s->keys[s->length - 1] = NULL;
    free(s->keys[s->length - 1]);
    s->length--;
    slice_autoresize(s);
    return key;
}

void slice_insert_index(struct slice *s, void *key, u64 index) {
    if (index > s->length) return;
    if (index == s->length) return slice_insert_back(s, key);
    if (index == 0) return slice_insert_front(s, key);
    slice_autoresize(s);
    memcpy(&(s->keys[index + 1]), &(s->keys[index]), sizeof(void *) * (s->length - index));
    s->keys[index] = key;
    s->length++;
}

void *slice_delete_index(struct slice *s, u64 index) {
    if (index >= s->length || s->length == 0) return NULL;
    void *key = s->keys[index];
    s->keys[index] = NULL;
    free(s->keys[index]);
    memcpy(&(s->keys[index]), &(s->keys[index + 1]), sizeof(void *) * (s->length - index - 1));
    s->length--;
    return key;
}

void slice_set_index(struct slice *s, void *key, u64 index) {
    if (index >= s->length) return;
    s->keys[index] = key;
}

void *slice_get_index(struct slice *s, u64 index) {
    if (index >= s->length) return NULL;
    return s->keys[index];
}

// void slice_from_array(struct slice *s, void **keys, u64 num_keys) {
//     slice_resize(s, (u64) ((double) num_keys * 1.5));
//     memcpy(s->keys, keys, sizeof(void *) * num_keys);
//     s->length = num_keys;
// }

struct slice *slice_from_array(slice_cmpfunc compare, printfunc print, void **keys, u64 num_keys) {
    struct slice *s = slice(compare, print);
    slice_resize(s, (u64) ((double) num_keys * 1.5));
    memcpy(s->keys, keys, sizeof(void *));
    s->length = num_keys;
    return s;
}

struct slice *
slice_from_primitive_array(slice_cmpfunc compare, printfunc print, void *keys, u64 num_keys, size_t key_size) {
    struct slice *s = slice(compare, print);
    slice_resize(s, (u64) ((double) num_keys * 1.5));
    for (int i = 0; i < 5; i++) {
        void *buf = malloc(key_size);
        memcpy(buf, keys + i * key_size, key_size);
        s->keys[i] = buf;
    }
    s->length = num_keys;
    return s;
}

void slice_join(struct slice *s1, struct slice *s2) {
    if (s1->length + s2->length >= s1->capacity) slice_resize(s1, s1->length + s2->length);
    memcpy(&(s1->keys[s1->length]), s2->keys, sizeof(void *) * s2->length);
    s1->length += s2->length;
    delete_slice(s2);
}

u64 slice_find_index(const struct slice *s, const void *key) {
    i64 start = 0, end = s->length - 1;
    while (start <= end) {
        u64 mid = (start + end) / 2;
        if (s->compare(s->keys[mid], key) == 0) return mid;
        if (s->compare(s->keys[mid], key) < 0) start = mid + 1;
        else end = mid - 1;
    }
    return end + 1;
}

void slice_print(struct slice *s) {
    if (!s || !s->keys) return;
    for (int i = 0; i < s->length; i++) if (s->keys[i] != NULL) s->print(s->keys[i]);
    printf("\n");
}

static void insertion_sort(struct slice *s) {
    int      j;
    for (int i = 0; i < s->length; i++) {
        void *key = s->keys[i];
        j              = i - 1;
        while (j >= 0 && s->compare(key, s->keys[j]) <= 0) {
            s->keys[j + 1] = s->keys[j];
            j--;
        }
        s->keys[j + 1] = key;
    }
}

static void merge(struct slice *s, struct slice *l, struct slice *r) {
    int i = 0, j = 0, k = 0;
    while (i < l->length && j < r->length) {
        if (s->compare(r->keys[j], l->keys[i]) >= 0) {
            slice_set_index(s, l->keys[i], k);
            i++;
        } else {
            slice_set_index(s, r->keys[j], k);
            j++;
        }
        k++;
    }
    while (i < l->length) {
        slice_set_index(s, l->keys[i], k);
        i++, k++;
    }
    while (j < r->length) {
        slice_set_index(s, r->keys[j], k);
        j++, k++;
    }
}

void slice_sort(struct slice *s) {
    if (s->length < 44) return insertion_sort(s);
    unsigned int mid = s->length / 2;
    struct slice *l  = subslice(s, 0, mid);
    struct slice *r  = subslice(s, mid, s->length);
    slice_sort(l);
    slice_sort(r);
    merge(s, l, r);
}

void slice_to_array(struct slice *s, void **array, u64 array_length) {
    memcpy(array, s->keys, sizeof(void *) * array_length);
}

void slice_to_primitive_array(struct slice *s, void *array, u64 array_length, size_t key_size) {
    for (int i = 0; i < s->length; i++) memcpy(array + i * key_size, s->keys[i], key_size);
}

i64 slice_search(struct slice *s, const void *key, u64 start, u64 end) {
    if (!s || !key || s->length == 0) return -1;
    u64 mid = (start + end) / 2;
    if (start == mid) {
        if (s->compare(s->keys[mid], key) == 0) return (i64) start;
        return -1;
    }
    if (s->compare(s->keys[mid], key) == 0) return (i64) mid;
    else if (s->compare(s->keys[mid], key) > 0) return slice_search(s, key, 0, mid);
    return slice_search(s, key, mid, end);
}