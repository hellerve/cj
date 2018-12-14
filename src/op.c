#include <assert.h>
#include <stdarg.h>
#include <string.h>

#include "op.h"

typedef struct {
  int ops_len;
  const char* operands[8];
  uint8_t codes_len;
  uint8_t codes[8];
} cj_variant;

typedef struct {
  uint8_t variantc;
  const char* name;
  cj_variant variants[32];
} cj_instruction;

int op_cmp(uint64_t al, const char** actual, uint64_t el, const char** exp) {
  if (al != el) return 0;

  for (uint64_t i = 0; i < al; i++) if (strcmp(actual[i], exp[i])) return 0;

  return 1;
}

void cj_emit(cj_ctx* ctx, cj_instruction inst, uint64_t len, ...) {
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

  if (inst.variantc == 1) {
    cj_variant var = inst.variants[0];
    if (op_cmp(len, ops, var.ops_len, var.operands)) {
      cj_add_bytes(ctx, var.codes, var.codes_len);
      return;
    }
    // TODO: how to error
  }
  // TODO: what now?
}

#define C ,
#define CJ_VARIANT(ol, ops, cl, cs) \
  (cj_variant){.ops_len=ol, .operands=ops, .codes_len=cl, .codes=cs}
#define CJ_OP(id, n, ...)\
  static const cj_instruction id = \
    (cj_instruction){.name = "id",\
                     .variantc = n,\
                     .variants = {__VA_ARGS__}};

// of course its not that simple
CJ_OP(nop, 1, CJ_VARIANT(0, {}, 1, {0x90}))
CJ_OP(ret, 1, CJ_VARIANT(0, {}, 1, {0xc3}))
//CJ_OP(aad, 1, CJ_VARIANT(1, {}, {2}, {{0xd5 C 0x0a}}))

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

EMIT0(ret, 0)
EMIT0(nop, 0)

#undef EMIT
#undef EMIT0
#undef CJ_OP
#undef CJ_OP_META
#undef CJ_VARIANT
#undef TYPIFY
#undef APPLY
#undef APPLY_1
#undef APPLY_2
#undef CAT
#undef PRIMITIVE_CAT
#undef NARGS
#undef NARGS_SEQ
#undef C
