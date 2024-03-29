#ifndef STRUCTS_H
#define STRUCTS_H

#define EVAL(...)  EVAL1(EVAL1(EVAL1(__VA_ARGS__)))
#define EVAL1(...) EVAL2(EVAL2(EVAL2(__VA_ARGS__)))
#define EVAL2(...) EVAL3(EVAL3(EVAL3(__VA_ARGS__)))
#define EVAL3(...) EVAL4(EVAL4(EVAL4(__VA_ARGS__)))
#define EVAL4(...) EVAL5(EVAL5(EVAL5(__VA_ARGS__)))
#define EVAL5(...) __VA_ARGS__

#define CAR(x, ...) x
#define CDR(x, ...) __VA_ARGS__
#define CARCDR(x, y, ...) y
#define CDRCDR(x, y, ...) __VA_ARGS__

#define CAT(a, ...) PRIMITIVE_CAT(a, __VA_ARGS__)
#define PRIMITIVE_CAT(a, ...) a ## __VA_ARGS__
#define CAT4(a, b, c, d) CAT(CAT(a, b), CAT(c, d))

#define EMPTY()
#define DEFER(id) id EMPTY()
#define OBSTRUCT(...) __VA_ARGS__ DEFER(EMPTY)()

#define IIF(c) PRIMITIVE_CAT(IIF_, c)
#define IIF_0(t, ...) __VA_ARGS__
#define IIF_1(t, ...) t

#define COMPL(b) PRIMITIVE_CAT(COMPL_, b)
#define COMPL_0 1
#define COMPL_1 0

#define CHECK_N(x, n, ...) n
#define CHECK(...) CHECK_N(__VA_ARGS__, 0,)
#define PROBE(x) x, 1,

#define NOT(x) CHECK(PRIMITIVE_CAT(NOT_, x))
#define NOT_0 PROBE(~)

#define BOOL(x) COMPL(NOT(x))
#define IF(c) IIF(BOOL(c))

#define EAT(...)
#define EXPAND(...) __VA_ARGS__
#define WHEN(c) IF(c)(EXPAND, EAT)

#define SUM_SIZES(...) \
  sizeof(CAR(__VA_ARGS__)) \
  WHEN(CHECK(CDR(__VA_ARGS__)))(+ OBSTRUCT(SUM_SIZES_INDIRECT)()(CDRCDR(__VA_ARGS__)))
#define SUM_SIZES_INDIRECT() SUM_SIZES

#define STRUCT_OFFSET_VARS(name, ...) \
  const int CAT4(__offset_, name, _, CARCDR(__VA_ARGS__)) = IF(CHECK(CDR(__VA_ARGS__)))(SUM_SIZES(CDRCDR(__VA_ARGS__)), 0); \
  WHEN(CHECK(CDR(__VA_ARGS__)))(OBSTRUCT(SOV_INDIRECT)()(name, CDRCDR(__VA_ARGS__)))
#define SOV_INDIRECT() STRUCT_OFFSET_VARS

#define STRUCT_TYPEDEFS(name, ...) \
  typedef CAR(__VA_ARGS__) CAT4(__type_, name, _, CARCDR(__VA_ARGS__)); \
  WHEN(CHECK(CDR(__VA_ARGS__)))(OBSTRUCT(ST_INDIRECT)()(name, CDRCDR(__VA_ARGS__)))
#define ST_INDIRECT() STRUCT_TYPEDEFS


#define STRUCT(name, ...) \
  const int __size_##name = EVAL(SUM_SIZES(__VA_ARGS__)); \
  EVAL(STRUCT_OFFSET_VARS(name, __VA_ARGS__)) \
  EVAL(STRUCT_TYPEDEFS(name, __VA_ARGS__))

#define STRUCT_SIZEOF(type) __size_##type
#define STRUCT_MALLOC(type, name) char *name = malloc(STRUCT_SIZEOF(type))
#define STRUCT_ACCESS(type, name, member) (*((CAT4(__type_, type, _, member) *) (name + CAT4(__offset_, type, _, member))))
#endif
