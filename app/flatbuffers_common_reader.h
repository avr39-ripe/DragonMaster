#ifndef FLATBUFFERS_COMMON_READER_H
#define FLATBUFFERS_COMMON_READER_H

/* Generated by flatcc 0.5.0-pre FlatBuffers schema compiler for C by dvide.com */

/* Common FlatBuffers read functionality for C. */

#define PDIAGNOSTIC_IGNORE_UNUSED
#include "flatcc/portable/pdiagnostic_push.h"
#include "flatcc/flatcc_flatbuffers.h"


#define __flatbuffers_read_scalar_at_byteoffset(N, p, o) N ## _read_from_pe((uint8_t *)(p) + (o))
#define __flatbuffers_read_scalar(N, p) N ## _read_from_pe(p)
#define __flatbuffers_read_vt(ID, offset, t)\
flatbuffers_voffset_t offset = 0;\
{   flatbuffers_voffset_t id, *vt;\
    assert(t != 0 && "null pointer table access");\
    id = ID;\
    vt = (flatbuffers_voffset_t *)((uint8_t *)(t) -\
        __flatbuffers_soffset_read_from_pe(t));\
    if (__flatbuffers_voffset_read_from_pe(vt) >= sizeof(vt[0]) * (id + 3)) {\
        offset = __flatbuffers_voffset_read_from_pe(vt + id + 2);\
    }\
}
#define __flatbuffers_field_present(ID, t) { __flatbuffers_read_vt(ID, offset, t) return offset != 0; }
#define __flatbuffers_define_scalar_field(ID, N, NK, TK, T, V)\
static inline T N ## _ ## NK (N ## _table_t t)\
{ __flatbuffers_read_vt(ID, offset, t)\
  return offset ? __flatbuffers_read_scalar_at_byteoffset(TK, t, offset) : V;\
}\
static inline int N ## _ ## NK ## _is_present(N ## _table_t t)\
__flatbuffers_field_present(ID, t)\
__flatbuffers_define_scan_by_scalar_field(N, NK, T)
#define __flatbuffers_struct_field(T, ID, t, r)\
{\
    __flatbuffers_read_vt(ID, offset, t)\
    if (offset) {\
        return (T)((uint8_t *)(t) + offset);\
    }\
    assert(!(r) && "required field missing");\
    return 0;\
}
#define __flatbuffers_offset_field(T, ID, t, r, adjust)\
{\
    flatbuffers_uoffset_t *elem;\
    __flatbuffers_read_vt(ID, offset, t)\
    if (offset) {\
        elem = (flatbuffers_uoffset_t *)((uint8_t *)(t) + offset);\
        /* Add sizeof so C api can have raw access past header field. */\
        return (T)((uint8_t *)(elem) + adjust +\
              __flatbuffers_uoffset_read_from_pe(elem));\
    }\
    assert(!(r) && "required field missing");\
    return 0;\
}
#define __flatbuffers_vector_field(T, ID, t, r) __flatbuffers_offset_field(T, ID, t, r, sizeof(flatbuffers_uoffset_t))
#define __flatbuffers_table_field(T, ID, t, r) __flatbuffers_offset_field(T, ID, t, r, 0)
#define __flatbuffers_define_struct_field(ID, N, NK, T, r)\
static inline T N ## _ ## NK(N ## _table_t t)\
__flatbuffers_struct_field(T, ID, t, r)\
static inline int N ## _ ## NK ## _is_present(N ## _table_t t)\
__flatbuffers_field_present(ID, t)
#define __flatbuffers_define_vector_field(ID, N, NK, T, r)\
static inline T N ## _ ## NK(N ## _table_t t)\
__flatbuffers_vector_field(T, ID, t, r)\
static inline int N ## _ ## NK ## _is_present(N ## _table_t t)\
__flatbuffers_field_present(ID, t)
#define __flatbuffers_define_table_field(ID, N, NK, T, r)\
static inline T N ## _ ## NK(N ## _table_t t)\
__flatbuffers_table_field(T, ID, t, r)\
static inline int N ## _ ## NK ## _is_present(N ## _table_t t)\
__flatbuffers_field_present(ID, t)
#define __flatbuffers_define_string_field(ID, N, NK, r)\
static inline flatbuffers_string_t N ## _ ## NK(N ## _table_t t)\
__flatbuffers_vector_field(flatbuffers_string_t, ID, t, r)\
static inline int N ## _ ## NK ## _is_present(N ## _table_t t)\
__flatbuffers_field_present(ID, t)\
__flatbuffers_define_scan_by_string_field(N, NK)
#define __flatbuffers_vec_len(vec)\
{ return (vec) ? (size_t)__flatbuffers_uoffset_read_from_pe((flatbuffers_uoffset_t *)vec - 1) : 0; }
#define __flatbuffers_string_len(s) __flatbuffers_vec_len(s)
static inline size_t flatbuffers_vec_len(const void *vec)
__flatbuffers_vec_len(vec)
#define __flatbuffers_scalar_vec_at(N, vec, i)\
{ assert(flatbuffers_vec_len(vec) > (i) && "index out of range");\
  return __flatbuffers_read_scalar(N, &(vec)[i]); }
#define __flatbuffers_struct_vec_at(vec, i)\
{ assert(flatbuffers_vec_len(vec) > (i) && "index out of range"); return (vec) + (i); }
/* `adjust` skips past the header for string vectors. */
#define __flatbuffers_offset_vec_at(T, vec, i, adjust)\
{ const flatbuffers_uoffset_t *elem = (vec) + (i);\
  assert(flatbuffers_vec_len(vec) > (i) && "index out of range");\
  return (T)((uint8_t *)(elem) + (size_t)__flatbuffers_uoffset_read_from_pe(elem) + adjust); }
#define __flatbuffers_define_scalar_vec_len(N) \
static inline size_t N ## _vec_len(N ##_vec_t vec)\
{ return flatbuffers_vec_len(vec); }
#define __flatbuffers_define_scalar_vec_at(N, T) \
static inline T N ## _vec_at(N ## _vec_t vec, size_t i)\
__flatbuffers_scalar_vec_at(N, vec, i)
typedef const char *flatbuffers_string_t;
static inline size_t flatbuffers_string_len(flatbuffers_string_t s)
__flatbuffers_string_len(s)
typedef const flatbuffers_uoffset_t *flatbuffers_string_vec_t;
typedef flatbuffers_uoffset_t *flatbuffers_string_mutable_vec_t;
static inline size_t flatbuffers_string_vec_len(flatbuffers_string_vec_t vec)
__flatbuffers_vec_len(vec)
static inline flatbuffers_string_t flatbuffers_string_vec_at(flatbuffers_string_vec_t vec, size_t i)
__flatbuffers_offset_vec_at(flatbuffers_string_t, vec, i, sizeof(vec[0]))
typedef const void *flatbuffers_generic_table_t;
typedef const flatbuffers_uoffset_t *flatbuffers_generic_table_vec_t;
typedef flatbuffers_uoffset_t *flatbuffers_generic_table_mutable_vec_t;
static inline size_t flatbuffers_generic_table_vec_len(flatbuffers_generic_table_vec_t vec)
__flatbuffers_vec_len(vec)
static inline flatbuffers_generic_table_t flatbuffers_generic_table_vec_at(flatbuffers_generic_table_vec_t vec, size_t i)
__flatbuffers_offset_vec_at(flatbuffers_generic_table_t, vec, i, 0)
typedef struct flatbuffers_union {
    flatbuffers_utype_t type;
    flatbuffers_generic_table_t member;
} flatbuffers_union_t;
typedef struct flatbuffers_union_vec {
    const flatbuffers_utype_t *type;
    const flatbuffers_uoffset_t *member;
} flatbuffers_union_vec_t;
#define __flatbuffers_union_type_field(ID, t)\
{\
    __flatbuffers_read_vt(ID, offset, t)\
    return offset ? __flatbuffers_read_scalar_at_byteoffset(__flatbuffers_utype, t, offset) : 0;\
}
#define __flatbuffers_define_union_field(NS, ID, N, NK, T, r)\
static inline T ## _union_type_t N ## _ ## NK ## _type(N ## _table_t t)\
__## NS ## union_type_field(((ID) - 1), t)\
static inline NS ## generic_table_t N ## _ ## NK(N ## _table_t t)\
__## NS ## table_field(NS ## generic_table_t, ID, t, r)\
static inline int N ## _ ## NK ## _is_present(N ## _table_t t)\
__## NS ## field_present(ID, t)\
static inline T ## _union_t N ## _ ## NK ## _union(N ## _table_t t)\
{ T ## _union_t u = { 0, 0 }; u.type = N ## _ ## NK ## _type(t);\
  if (u.type == 0) return u; u.member = N ## _ ## NK (t); return u; }
#define __flatbuffers_define_union_vector_ops(NS, T)\
static inline size_t T ## _union_vec_len(T ## _union_vec_t uv)\
{ return NS ## vec_len(uv.type); }\
static inline T ## _union_t T ## _union_vec_at(T ## _union_vec_t uv, size_t i)\
{ T ## _union_t u = { 0, 0 }; size_t n = NS ## vec_len(uv.type);\
  assert(n > (i) && "index out of range"); u.type = uv.type[i];\
  /* Unknown type is treated as NONE for schema evolution. */\
  if (u.type == 0) return u;\
  u.member = NS ## generic_table_vec_at(uv.member, i); return u; }
#define __flatbuffers_define_union_vector(NS, T)\
typedef NS ## union_vec_t T ## _union_vec_t;\
__## NS ## define_union_vector_ops(NS, T)
#define __flatbuffers_define_union(NS, T)\
typedef NS ## union_t T ## _union_t;\
__## NS ## define_union_vector(NS, T)
#define __flatbuffers_define_union_vector_field(NS, ID, N, NK, T, r)\
__## NS ## define_vector_field(ID - 1, N, NK ## _type, T ## _vec_t, r)\
__## NS ## define_vector_field(ID, N, NK, flatbuffers_generic_table_vec_t, r)\
static inline T ## _union_vec_t N ## _ ## NK ## _union(N ## _table_t t)\
{ T ## _union_vec_t uv; uv.type = N ## _ ## NK ## _type(t); uv.member = N ## _ ## NK(t);\
  assert(NS ## vec_len(uv.type) == NS ## vec_len(uv.member)\
  && "union vector type length mismatch"); return uv; }
#include <string.h>
static size_t flatbuffers_not_found = (size_t)-1;
static size_t flatbuffers_end = (size_t)-1;
#define __flatbuffers_identity(n) (n)
#define __flatbuffers_min(a, b) ((a) < (b) ? (a) : (b))
/* Subtraction doesn't work for unsigned types. */
#define __flatbuffers_scalar_cmp(x, y, n) ((x) < (y) ? -1 : (x) > (y))
static inline int __flatbuffers_string_n_cmp(flatbuffers_string_t v, const char *s, size_t n)
{ size_t nv = flatbuffers_string_len(v); int x = strncmp(v, s, nv < n ? nv : n);
  return x != 0 ? x : nv < n ? -1 : nv > n; }
/* `n` arg unused, but needed by string find macro expansion. */
static inline int __flatbuffers_string_cmp(flatbuffers_string_t v, const char *s, size_t n) { (void)n; return strcmp(v, s); }
/* A = identity if searching scalar vectors rather than key fields. */
/* Returns lowest matching index or not_found. */
#define __flatbuffers_find_by_field(A, V, E, L, K, Kn, T, D)\
{ T v; size_t a = 0, b, m; if (!(b = L(V))) { return flatbuffers_not_found; }\
  --b;\
  while (a < b) {\
    m = a + ((b - a) >> 1);\
    v = A(E(V, m));\
    if ((D(v, (K), (Kn))) < 0) {\
      a = m + 1;\
    } else {\
      b = m;\
    }\
  }\
  if (a == b) {\
    v = A(E(V, a));\
    if (D(v, (K), (Kn)) == 0) {\
       return a;\
    }\
  }\
  return flatbuffers_not_found;\
}
#define __flatbuffers_find_by_scalar_field(A, V, E, L, K, T)\
__flatbuffers_find_by_field(A, V, E, L, K, 0, T, __flatbuffers_scalar_cmp)
#define __flatbuffers_find_by_string_field(A, V, E, L, K)\
__flatbuffers_find_by_field(A, V, E, L, K, 0, flatbuffers_string_t, __flatbuffers_string_cmp)
#define __flatbuffers_find_by_string_n_field(A, V, E, L, K, Kn)\
__flatbuffers_find_by_field(A, V, E, L, K, Kn, flatbuffers_string_t, __flatbuffers_string_n_cmp)
#define __flatbuffers_define_find_by_scalar_field(N, NK, TK)\
static inline size_t N ## _vec_find_by_ ## NK(N ## _vec_t vec, TK key)\
__flatbuffers_find_by_scalar_field(N ## _ ## NK, vec, N ## _vec_at, N ## _vec_len, key, TK)
#define __flatbuffers_define_scalar_find(N, T)\
static inline size_t N ## _vec_find(N ## _vec_t vec, T key)\
__flatbuffers_find_by_scalar_field(__flatbuffers_identity, vec, N ## _vec_at, N ## _vec_len, key, T)
#define __flatbuffers_define_find_by_string_field(N, NK) \
/* Note: find only works on vectors sorted by this field. */\
static inline size_t N ## _vec_find_by_ ## NK(N ## _vec_t vec, const char *s)\
__flatbuffers_find_by_string_field(N ## _ ## NK, vec, N ## _vec_at, N ## _vec_len, s)\
static inline size_t N ## _vec_find_n_by_ ## NK(N ## _vec_t vec, const char *s, int n)\
__flatbuffers_find_by_string_n_field(N ## _ ## NK, vec, N ## _vec_at, N ## _vec_len, s, n)
#define __flatbuffers_define_default_find_by_scalar_field(N, NK, TK)\
static inline size_t N ## _vec_find(N ## _vec_t vec, TK key)\
{ return N ## _vec_find_by_ ## NK(vec, key); }
#define __flatbuffers_define_default_find_by_string_field(N, NK) \
static inline size_t N ## _vec_find(N ## _vec_t vec, const char *s)\
{ return N ## _vec_find_by_ ## NK(vec, s); }\
static inline size_t N ## _vec_find_n(N ## _vec_t vec, const char *s, int n)\
{ return N ## _vec_find_n_by_ ## NK(vec, s, n); }
/* A = identity if searching scalar vectors rather than key fields. */
/* Returns lowest matching index or not_found. */
#define __flatbuffers_scan_by_field(b, e, A, V, E, L, K, Kn, T, D)\
{ T v; size_t i;\
  for (i = b; i < e; ++i) {\
    v = A(E(V, i));\
    if (D(v, (K), (Kn)) == 0) {\
       return i;\
    }\
  }\
  return flatbuffers_not_found;\
}
#define __flatbuffers_rscan_by_field(b, e, A, V, E, L, K, Kn, T, D)\
{ T v; size_t i = e;\
  while (i-- > b) {\
    v = A(E(V, i));\
    if (D(v, (K), (Kn)) == 0) {\
       return i;\
    }\
  }\
  return flatbuffers_not_found;\
}
#define __flatbuffers_scan_by_scalar_field(b, e, A, V, E, L, K, T)\
__flatbuffers_scan_by_field(b, e, A, V, E, L, K, 0, T, __flatbuffers_scalar_cmp)
#define __flatbuffers_scan_by_string_field(b, e, A, V, E, L, K)\
__flatbuffers_scan_by_field(b, e, A, V, E, L, K, 0, flatbuffers_string_t, __flatbuffers_string_cmp)
#define __flatbuffers_scan_by_string_n_field(b, e, A, V, E, L, K, Kn)\
__flatbuffers_scan_by_field(b, e, A, V, E, L, K, Kn, flatbuffers_string_t, __flatbuffers_string_n_cmp)
#define __flatbuffers_rscan_by_scalar_field(b, e, A, V, E, L, K, T)\
__flatbuffers_rscan_by_field(b, e, A, V, E, L, K, 0, T, __flatbuffers_scalar_cmp)
#define __flatbuffers_rscan_by_string_field(b, e, A, V, E, L, K)\
__flatbuffers_rscan_by_field(b, e, A, V, E, L, K, 0, flatbuffers_string_t, __flatbuffers_string_cmp)
#define __flatbuffers_rscan_by_string_n_field(b, e, A, V, E, L, K, Kn)\
__flatbuffers_rscan_by_field(b, e, A, V, E, L, K, Kn, flatbuffers_string_t, __flatbuffers_string_n_cmp)
#define __flatbuffers_define_scan_by_scalar_field(N, NK, T)\
static inline size_t N ## _vec_scan_by_ ## NK(N ## _vec_t vec, T key)\
__flatbuffers_scan_by_scalar_field(0, N ## _vec_len(vec), N ## _ ## NK, vec, N ## _vec_at, N ## _vec_len, key, T)\
static inline size_t N ## _vec_scan_ex_by_ ## NK(N ## _vec_t vec, size_t begin, size_t end, T key)\
__flatbuffers_scan_by_scalar_field(begin, __flatbuffers_min(end, N ## _vec_len(vec)), N ## _ ## NK, vec, N ## _vec_at, N ## _vec_len, key, T)\
static inline size_t N ## _vec_rscan_by_ ## NK(N ## _vec_t vec, T key)\
__flatbuffers_rscan_by_scalar_field(0, N ## _vec_len(vec), N ## _ ## NK, vec, N ## _vec_at, N ## _vec_len, key, T)\
static inline size_t N ## _vec_rscan_ex_by_ ## NK(N ## _vec_t vec, size_t begin, size_t end, T key)\
__flatbuffers_rscan_by_scalar_field(begin, __flatbuffers_min(end, N ## _vec_len(vec)), N ## _ ## NK, vec, N ## _vec_at, N ## _vec_len, key, T)
#define __flatbuffers_define_scalar_scan(N, T)\
static inline size_t N ## _vec_scan(N ## _vec_t vec, T key)\
__flatbuffers_scan_by_scalar_field(0, N ## _vec_len(vec), __flatbuffers_identity, vec, N ## _vec_at, N ## _vec_len, key, T)\
static inline size_t N ## _vec_scan_ex(N ## _vec_t vec, size_t begin, size_t end, T key)\
__flatbuffers_scan_by_scalar_field(begin, __flatbuffers_min(end, N ## _vec_len(vec)), __flatbuffers_identity, vec, N ## _vec_at, N ## _vec_len, key, T)\
static inline size_t N ## _vec_rscan(N ## _vec_t vec, T key)\
__flatbuffers_rscan_by_scalar_field(0, N ## _vec_len(vec), __flatbuffers_identity, vec, N ## _vec_at, N ## _vec_len, key, T)\
static inline size_t N ## _vec_rscan_ex(N ## _vec_t vec, size_t begin, size_t end, T key)\
__flatbuffers_rscan_by_scalar_field(begin, __flatbuffers_min(end, N ## _vec_len(vec)), __flatbuffers_identity, vec, N ## _vec_at, N ## _vec_len, key, T)
#define __flatbuffers_define_scan_by_string_field(N, NK) \
static inline size_t N ## _vec_scan_by_ ## NK(N ## _vec_t vec, const char *s)\
__flatbuffers_scan_by_string_field(0, N ## _vec_len(vec), N ## _ ## NK, vec, N ## _vec_at, N ## _vec_len, s)\
static inline size_t N ## _vec_scan_n_by_ ## NK(N ## _vec_t vec, const char *s, int n)\
__flatbuffers_scan_by_string_n_field(0, N ## _vec_len(vec), N ## _ ## NK, vec, N ## _vec_at, N ## _vec_len, s, n)\
static inline size_t N ## _vec_scan_ex_by_ ## NK(N ## _vec_t vec, size_t begin, size_t end, const char *s)\
__flatbuffers_scan_by_string_field(begin, __flatbuffers_min(end, N ## _vec_len(vec)), N ## _ ## NK, vec, N ## _vec_at, N ## _vec_len, s)\
static inline size_t N ## _vec_scan_ex_n_by_ ## NK(N ## _vec_t vec, size_t begin, size_t end, const char *s, int n)\
__flatbuffers_scan_by_string_n_field(begin, __flatbuffers_min( end, N ## _vec_len(vec) ), N ## _ ## NK, vec, N ## _vec_at, N ## _vec_len, s, n)\
static inline size_t N ## _vec_rscan_by_ ## NK(N ## _vec_t vec, const char *s)\
__flatbuffers_rscan_by_string_field(0, N ## _vec_len(vec), N ## _ ## NK, vec, N ## _vec_at, N ## _vec_len, s)\
static inline size_t N ## _vec_rscan_n_by_ ## NK(N ## _vec_t vec, const char *s, int n)\
__flatbuffers_rscan_by_string_n_field(0, N ## _vec_len(vec), N ## _ ## NK, vec, N ## _vec_at, N ## _vec_len, s, n)\
static inline size_t N ## _vec_rscan_ex_by_ ## NK(N ## _vec_t vec, size_t begin, size_t end, const char *s)\
__flatbuffers_rscan_by_string_field(begin, __flatbuffers_min(end, N ## _vec_len(vec)), N ## _ ## NK, vec, N ## _vec_at, N ## _vec_len, s)\
static inline size_t N ## _vec_rscan_ex_n_by_ ## NK(N ## _vec_t vec, size_t begin, size_t end, const char *s, int n)\
__flatbuffers_rscan_by_string_n_field(begin, __flatbuffers_min( end, N ## _vec_len(vec) ), N ## _ ## NK, vec, N ## _vec_at, N ## _vec_len, s, n)
#define __flatbuffers_define_default_scan_by_scalar_field(N, NK, TK)\
static inline size_t N ## _vec_scan(N ## _vec_t vec, TK key)\
{ return N ## _vec_scan_by_ ## NK(vec, key); }\
static inline size_t N ## _vec_scan_ex(N ## _vec_t vec, size_t begin, size_t end, TK key)\
{ return N ## _vec_scan_ex_by_ ## NK(vec, begin, end, key); }\
static inline size_t N ## _vec_rscan(N ## _vec_t vec, TK key)\
{ return N ## _vec_rscan_by_ ## NK(vec, key); }\
static inline size_t N ## _vec_rscan_ex(N ## _vec_t vec, size_t begin, size_t end, TK key)\
{ return N ## _vec_rscan_ex_by_ ## NK(vec, begin, end, key); }
#define __flatbuffers_define_default_scan_by_string_field(N, NK) \
static inline size_t N ## _vec_scan(N ## _vec_t vec, const char *s)\
{ return N ## _vec_scan_by_ ## NK(vec, s); }\
static inline size_t N ## _vec_scan_n(N ## _vec_t vec, const char *s, int n)\
{ return N ## _vec_scan_n_by_ ## NK(vec, s, n); }\
static inline size_t N ## _vec_scan_ex(N ## _vec_t vec, size_t begin, size_t end, const char *s)\
{ return N ## _vec_scan_ex_by_ ## NK(vec, begin, end, s); }\
static inline size_t N ## _vec_scan_ex_n(N ## _vec_t vec, size_t begin, size_t end, const char *s, int n)\
{ return N ## _vec_scan_ex_n_by_ ## NK(vec, begin, end, s, n); }\
static inline size_t N ## _vec_rscan(N ## _vec_t vec, const char *s)\
{ return N ## _vec_rscan_by_ ## NK(vec, s); }\
static inline size_t N ## _vec_rscan_n(N ## _vec_t vec, const char *s, int n)\
{ return N ## _vec_rscan_n_by_ ## NK(vec, s, n); }\
static inline size_t N ## _vec_rscan_ex(N ## _vec_t vec, size_t begin, size_t end, const char *s)\
{ return N ## _vec_rscan_ex_by_ ## NK(vec, begin, end, s); }\
static inline size_t N ## _vec_rscan_ex_n(N ## _vec_t vec, size_t begin, size_t end, const char *s, int n)\
{ return N ## _vec_rscan_ex_n_by_ ## NK(vec, begin, end, s, n); }
#define __flatbuffers_heap_sort(N, X, A, E, L, TK, TE, D, S)\
static inline void __ ## N ## X ## __heap_sift_down(\
        N ## _mutable_vec_t vec, size_t start, size_t end)\
{ size_t child, root; TK v1, v2, vroot;\
  root = start;\
  while ((root << 1) <= end) {\
    child = root << 1;\
    if (child < end) {\
      v1 = A(E(vec, child));\
      v2 = A(E(vec, child + 1));\
      if (D(v1, v2) < 0) {\
        child++;\
      }\
    }\
    vroot = A(E(vec, root));\
    v1 = A(E(vec, child));\
    if (D(vroot, v1) < 0) {\
      S(vec, root, child, TE);\
      root = child;\
    } else {\
      return;\
    }\
  }\
}\
static inline void __ ## N ## X ## __heap_sort(N ## _mutable_vec_t vec)\
{ size_t start, end, size;\
  size = L(vec); if (size == 0) return; end = size - 1; start = size >> 1;\
  do { __ ## N ## X ## __heap_sift_down(vec, start, end); } while (start--);\
  while (end > 0) { \
    S(vec, 0, end, TE);\
    __ ## N ## X ## __heap_sift_down(vec, 0, --end); } }
#define __flatbuffers_define_sort_by_field(N, NK, TK, TE, D, S)\
  __flatbuffers_heap_sort(N, _sort_by_ ## NK, N ## _ ## NK, N ## _vec_at, N ## _vec_len, TK, TE, D, S)\
static inline void N ## _vec_sort_by_ ## NK(N ## _mutable_vec_t vec)\
{ __ ## N ## _sort_by_ ## NK ## __heap_sort(vec); }
#define __flatbuffers_define_sort(N, TK, TE, D, S)\
__flatbuffers_heap_sort(N, , __flatbuffers_identity, N ## _vec_at, N ## _vec_len, TK, TE, D, S)\
static inline void N ## _vec_sort(N ## _mutable_vec_t vec) { __ ## N ## __heap_sort(vec); }
#define __flatbuffers_scalar_diff(x, y) ((x) < (y) ? -1 : (x) > (y))
#define __flatbuffers_string_diff(x, y) __flatbuffers_string_n_cmp((x), (const char *)(y), flatbuffers_string_len(y))
#define __flatbuffers_scalar_swap(vec, a, b, TE) { TE tmp = vec[b]; vec[b] = vec[a]; vec[a] = tmp; }
#define __flatbuffers_string_swap(vec, a, b, TE)\
{ TE ta, tb, d;\
  d = (TE)((a - b) * sizeof(vec[0]));\
  ta =  __flatbuffers_uoffset_read_from_pe(vec + b) - d;\
  tb =  __flatbuffers_uoffset_read_from_pe(vec + a) + d;\
  __flatbuffers_uoffset_write_to_pe(vec + a, ta);\
  __flatbuffers_uoffset_write_to_pe(vec + b, tb); }
#define __flatbuffers_define_sort_by_scalar_field(N, NK, TK, TE)\
  __flatbuffers_define_sort_by_field(N, NK, TK, TE, __flatbuffers_scalar_diff, __flatbuffers_scalar_swap)
#define __flatbuffers_define_sort_by_string_field(N, NK)\
  __flatbuffers_define_sort_by_field(N, NK, flatbuffers_string_t, flatbuffers_uoffset_t, __flatbuffers_string_diff, __flatbuffers_string_swap)
#define __flatbuffers_define_scalar_sort(N, T) __flatbuffers_define_sort(N, T, T, __flatbuffers_scalar_diff, __flatbuffers_scalar_swap)
#define __flatbuffers_define_string_sort() __flatbuffers_define_sort(flatbuffers_string, flatbuffers_string_t, flatbuffers_uoffset_t, __flatbuffers_string_diff, __flatbuffers_string_swap)
#define __flatbuffers_define_scalar_vector(N, T)\
typedef const T *N ## _vec_t;\
typedef T *N ## _mutable_vec_t;\
__flatbuffers_define_scalar_vec_len(N)\
__flatbuffers_define_scalar_vec_at(N, T)\
__flatbuffers_define_scalar_find(N, T)\
__flatbuffers_define_scalar_scan(N, T)\
__flatbuffers_define_scalar_sort(N, T)

#define __flatbuffers_define_integer_type(N, T, W)\
__flatcc_define_integer_accessors(N, T, W, flatbuffers_endian)\
__flatbuffers_define_scalar_vector(N, T)
__flatbuffers_define_scalar_vector(flatbuffers_bool, flatbuffers_bool_t)
__flatbuffers_define_scalar_vector(flatbuffers_uint8, uint8_t)
__flatbuffers_define_scalar_vector(flatbuffers_int8, int8_t)
__flatbuffers_define_scalar_vector(flatbuffers_uint16, uint16_t)
__flatbuffers_define_scalar_vector(flatbuffers_int16, int16_t)
__flatbuffers_define_scalar_vector(flatbuffers_uint32, uint32_t)
__flatbuffers_define_scalar_vector(flatbuffers_int32, int32_t)
__flatbuffers_define_scalar_vector(flatbuffers_uint64, uint64_t)
__flatbuffers_define_scalar_vector(flatbuffers_int64, int64_t)
__flatbuffers_define_scalar_vector(flatbuffers_float, float)
__flatbuffers_define_scalar_vector(flatbuffers_double, double)
static inline size_t flatbuffers_string_vec_find(flatbuffers_string_vec_t vec, const char *s)
__flatbuffers_find_by_string_field(__flatbuffers_identity, vec, flatbuffers_string_vec_at, flatbuffers_string_vec_len, s)
static inline size_t flatbuffers_string_vec_find_n(flatbuffers_string_vec_t vec, const char *s, size_t n)
__flatbuffers_find_by_string_n_field(__flatbuffers_identity, vec, flatbuffers_string_vec_at, flatbuffers_string_vec_len, s, n)
static inline size_t flatbuffers_string_vec_scan(flatbuffers_string_vec_t vec, const char *s)
__flatbuffers_scan_by_string_field(0, flatbuffers_string_vec_len(vec), __flatbuffers_identity, vec, flatbuffers_string_vec_at, flatbuffers_string_vec_len, s)
static inline size_t flatbuffers_string_vec_scan_n(flatbuffers_string_vec_t vec, const char *s, size_t n)
__flatbuffers_scan_by_string_n_field(0, flatbuffers_string_vec_len(vec), __flatbuffers_identity, vec, flatbuffers_string_vec_at, flatbuffers_string_vec_len, s, n)
static inline size_t flatbuffers_string_vec_scan_ex(flatbuffers_string_vec_t vec, size_t begin, size_t end, const char *s)
__flatbuffers_scan_by_string_field(begin, __flatbuffers_min(end, flatbuffers_string_vec_len(vec)), __flatbuffers_identity, vec, flatbuffers_string_vec_at, flatbuffers_string_vec_len, s)
static inline size_t flatbuffers_string_vec_scan_ex_n(flatbuffers_string_vec_t vec, size_t begin, size_t end, const char *s, size_t n)
__flatbuffers_scan_by_string_n_field(begin, __flatbuffers_min(end, flatbuffers_string_vec_len(vec)), __flatbuffers_identity, vec, flatbuffers_string_vec_at, flatbuffers_string_vec_len, s, n)
static inline size_t flatbuffers_string_vec_rscan(flatbuffers_string_vec_t vec, const char *s)
__flatbuffers_rscan_by_string_field(0, flatbuffers_string_vec_len(vec), __flatbuffers_identity, vec, flatbuffers_string_vec_at, flatbuffers_string_vec_len, s)
static inline size_t flatbuffers_string_vec_rscan_n(flatbuffers_string_vec_t vec, const char *s, size_t n)
__flatbuffers_rscan_by_string_n_field(0, flatbuffers_string_vec_len(vec), __flatbuffers_identity, vec, flatbuffers_string_vec_at, flatbuffers_string_vec_len, s, n)
static inline size_t flatbuffers_string_vec_rscan_ex(flatbuffers_string_vec_t vec, size_t begin, size_t end, const char *s)
__flatbuffers_rscan_by_string_field(begin, __flatbuffers_min(end, flatbuffers_string_vec_len(vec)), __flatbuffers_identity, vec, flatbuffers_string_vec_at, flatbuffers_string_vec_len, s)
static inline size_t flatbuffers_string_vec_rscan_ex_n(flatbuffers_string_vec_t vec, size_t begin, size_t end, const char *s, size_t n)
__flatbuffers_rscan_by_string_n_field(begin, __flatbuffers_min(end, flatbuffers_string_vec_len(vec)), __flatbuffers_identity, vec, flatbuffers_string_vec_at, flatbuffers_string_vec_len, s, n)
__flatbuffers_define_string_sort()
#define __flatbuffers_define_struct_scalar_field(N, NK, TK, T)\
static inline T N ## _ ## NK (N ## _struct_t t)\
{ return t ? __flatbuffers_read_scalar(TK, &(t->NK)) : 0; }\
__flatbuffers_define_scan_by_scalar_field(N, NK, T)
#define __flatbuffers_define_struct_struct_field(N, NK, T)\
static inline T N ## _ ## NK(N ## _struct_t t) { return t ? &(t->NK) : 0; }
/* If fid is null, the function returns true without testing as buffer is not expected to have any id. */
static inline int flatbuffers_has_identifier(const void *buffer, const char *fid)
{ flatbuffers_thash_t id, id2 = 0; if (fid == 0) { return 1; };
  strncpy((char *)&id2, fid, sizeof(id2));
  /* Identifier strings are always considered little endian. */
  id2 = __flatbuffers_thash_cast_from_le(id2);
  id = __flatbuffers_thash_read_from_pe(((flatbuffers_uoffset_t *)buffer) + 1);
  return id2 == 0 || id == id2; }
static inline int flatbuffers_has_type_hash(const void *buffer, flatbuffers_thash_t thash)
{ return thash == 0 || (__flatbuffers_thash_read_from_pe((flatbuffers_uoffset_t *)buffer + 1) == thash); }

static inline flatbuffers_thash_t flatbuffers_get_type_hash(const void *buffer)
{ return __flatbuffers_thash_read_from_pe((flatbuffers_uoffset_t *)buffer + 1); }

#define flatbuffers_verify_endian() flatbuffers_has_identifier("\x00\x00\x00\x00" "1234", "1234")
static inline void *flatbuffers_read_size_prefix(void *b, size_t *size_out)
{ if (size_out) { *size_out = (size_t)__flatbuffers_uoffset_read_from_pe(b); }
  return (uint8_t *)b + sizeof(flatbuffers_uoffset_t); }
/* Null file identifier accepts anything, otherwise fid should be 4 characters. */
#define __flatbuffers_read_root(T, K, buffer, fid)\
  ((!buffer || !flatbuffers_has_identifier(buffer, fid)) ? 0 :\
  ((T ## _ ## K ## t)(((uint8_t *)buffer) +\
    __flatbuffers_uoffset_read_from_pe(buffer))))
#define __flatbuffers_read_typed_root(T, K, buffer, thash)\
  ((!buffer || !flatbuffers_has_type_hash(buffer, thash)) ? 0 :\
  ((T ## _ ## K ## t)(((uint8_t *)buffer) +\
    __flatbuffers_uoffset_read_from_pe(buffer))))
#define __flatbuffers_nested_buffer_as_root(C, N, T, K)\
static inline T ## _ ## K ## t C ## _ ## N ## _as_root_with_identifier(C ## _ ## table_t t, const char *fid)\
{ const uint8_t *buffer = C ## _ ## N(t); return __flatbuffers_read_root(T, K, buffer, fid); }\
static inline T ## _ ## K ## t C ## _ ## N ## _as_typed_root(C ## _ ## table_t t)\
{ const uint8_t *buffer = C ## _ ## N(t); return __flatbuffers_read_root(T, K, buffer, C ## _ ## type_identifier); }\
static inline T ## _ ## K ## t C ## _ ## N ## _as_root(C ## _ ## table_t t)\
{ const char *fid = T ## _identifier;\
  const uint8_t *buffer = C ## _ ## N(t); return __flatbuffers_read_root(T, K, buffer, fid); }
#define __flatbuffers_buffer_as_root(N, K)\
static inline N ## _ ## K ## t N ## _as_root_with_identifier(const void *buffer, const char *fid)\
{ return __flatbuffers_read_root(N, K, buffer, fid); }\
static inline N ## _ ## K ## t N ## _as_root_with_type_hash(const void *buffer, flatbuffers_thash_t thash)\
{ return __flatbuffers_read_typed_root(N, K, buffer, thash); }\
static inline N ## _ ## K ## t N ## _as_root(const void *buffer)\
{ const char *fid = N ## _identifier;\
  return __flatbuffers_read_root(N, K, buffer, fid); }\
static inline N ## _ ## K ## t N ## _as_typed_root(const void *buffer)\
{ return __flatbuffers_read_typed_root(N, K, buffer, N ## _type_hash); }
#define __flatbuffers_struct_as_root(N) __flatbuffers_buffer_as_root(N, struct_)
#define __flatbuffers_table_as_root(N) __flatbuffers_buffer_as_root(N, table_)

#include "flatcc/portable/pdiagnostic_pop.h"
#endif /* FLATBUFFERS_COMMON_H */
