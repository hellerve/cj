#include <assert.h>
#include <stdarg.h>

#include "op.h"

typedef struct {
  uint8_t nargs;
  uint8_t len;
  uint8_t code[8];
  const char* name;
} cj_instruction;

typedef struct {
  const char* operands[8];
  uint64_t codes[8][8];
} cj_variant;

typedef struct {
  uint8_t variantc;
  const char* name;
  cj_variant variants[32];
} cj_instruction_meta;

void cj_emit(cj_ctx* ctx, cj_instruction inst, uint64_t len, ...) {
  assert(len == inst.nargs);
  if (!len) return cj_add_bytes(ctx, inst.code, inst.len);

  const char* ops[len];
  const char* constant = "__constant__";
  va_list vl;
  va_start(vl, len);
  for (uint8_t i = 0; i < len; i++) {
    cj_operand op = va_arg(vl, cj_operand);
    if (op.type == CJ_REGISTER) ops[i] = op.reg;
    else if (op.type == CJ_CONSTANT) ops[i] = constant;
  }
  va_end(vl);

  // TODO: what now?
}

#define CJ_OP(id, n, l, ...) \
  static const cj_instruction id = (cj_instruction){.name = "id",\
                                                      .nargs = n,\
                                                      .len = l,\
                                                      .code = {__VA_ARGS__}};
#define CJ_OP_META(id, n, ...)\
  static const cj_instruction_meta id##_meta = \
    (cj_instruction_meta){.name = "id",\
                          .variantc = n,\
                          .variants = {__VA_ARGS__}};

// of course its not that simple
CJ_OP(inc, 1, 1, 0)
//CJ_OP_META(inc, 1, (cj_variant){.operands={"r14"}, .codes={{0}}});
CJ_OP(mov, 2, 1, 0)
CJ_OP(nop, 0, 1, 0x90)
CJ_OP(ret, 0, 1, 0xc3)
CJ_OP(aad, 0, 2, 0xd5, 0x0a)
CJ_OP(aam, 0, 2, 0xd4, 0x0a)
CJ_OP(aaf, 0, 1, 0x3f)

#define NARGS_SEQ(_1,_2,N,...) N
#define NARGS(...) NARGS_SEQ(__VA_ARGS__, 2, 1)

#define PRIMITIVE_CAT(x, y) x ## y
#define CAT(x, y) PRIMITIVE_CAT(x, y)

#define APPLY(macro, ...) CAT(APPLY_, NARGS(__VA_ARGS__))(macro, __VA_ARGS__)
#define APPLY_1(m, x1) m(x1)
#define APPLY_2(m, x1, x2) m(x1), m(x2)

#define TIPIFY(x) cj_operand x

#define EMIT0(name, n) \
  void cj_##name(cj_ctx* ctx) {\
    cj_emit(ctx, name, n);\
  }

#define EMIT(name, n, ...) \
  void cj_##name(cj_ctx* ctx, APPLY(TIPIFY, __VA_ARGS__)) {\
    cj_emit(ctx, name, n, __VA_ARGS__);\
  }

EMIT(inc, 1, op);
EMIT(mov, 2, op1, op2);
EMIT0(ret, 0);
EMIT0(nop, 0);

#undef EMIT
#undef EMIT0
#undef CJ_OP
#undef CJ_OP_META
#undef TYPIFY
#undef APPLY
#undef APPLY_1
#undef APPLY_2
#undef CAT
#undef PRIMITIVE_CAT
#undef NARGS
#undef NARGS_SEQ
