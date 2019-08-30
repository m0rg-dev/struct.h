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

#define STRUCT_SIZE_VARS(name, ...) \
  int CAT(CAT(CAT(__size_, name), _), CARCDR(__VA_ARGS__)) = sizeof(CAR(__VA_ARGS__)); \
  WHEN(CHECK(CDR(__VA_ARGS__)))(OBSTRUCT(SSV_INDIRECT)()(name, CDRCDR(__VA_ARGS__)))
#define SSV_INDIRECT() STRUCT_SIZE_VARS

#define SUM_SIZES(...) \
  sizeof(CAR(__VA_ARGS__)) \
  WHEN(CHECK(CDR(__VA_ARGS__)))(+ OBSTRUCT(SUM_SIZES_INDIRECT)()(CDRCDR(__VA_ARGS__)))
#define SUM_SIZES_INDIRECT() SUM_SIZES

#define STRUCT_OFFSET_VARS(name, ...) \
  int CAT(CAT(CAT(__offset_, name), _), CARCDR(__VA_ARGS__)) = IF(CHECK(CDR(__VA_ARGS__)))(SUM_SIZES(CDRCDR(__VA_ARGS__)), 0); \
  WHEN(CHECK(CDR(__VA_ARGS__)))(OBSTRUCT(SOV_INDIRECT)()(name, CDRCDR(__VA_ARGS__)))
#define SOV_INDIRECT() STRUCT_OFFSET_VARS

#define STRUCT_CAST_FUNCS(name, ...) \
  CAR(__VA_ARGS__) CAT(CAT(CAT(__get_, name), _), CARCDR(__VA_ARGS__))(char*s){return*((CAR(__VA_ARGS__)*)(s+CAT(CAT(CAT(__offset_, name), _), CARCDR(__VA_ARGS__))));} \
  void CAT(CAT(CAT(__set_, name), _), CARCDR(__VA_ARGS__))(char*s,CAR(__VA_ARGS__) x){*((CAR(__VA_ARGS__)*)(s+CAT(CAT(CAT(__offset_, name), _), CARCDR(__VA_ARGS__))))=x;} \
  WHEN(CHECK(CDR(__VA_ARGS__)))(OBSTRUCT(SCF_INDIRECT)()(name, CDRCDR(__VA_ARGS__)))
#define SCF_INDIRECT() STRUCT_CAST_FUNCS


#define STRUCT(name, ...) \
  int __size_##name = EVAL(SUM_SIZES(__VA_ARGS__)); \
  EVAL(STRUCT_OFFSET_VARS(name, __VA_ARGS__)) \
  EVAL(STRUCT_CAST_FUNCS(name, __VA_ARGS__))

#define STRUCT_SIZEOF(type) __size_##type
#define STRUCT_MALLOC(type, name) char *name = malloc(STRUCT_SIZEOF(type))
#define STRUCT_GET(type, name, member) (CAT(CAT(CAT(__get_, type), _), member)(name))
#define STRUCT_SET(type, name, member, value) (CAT(CAT(CAT(__set_, type), _), member)(name, value))
#endif
