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
  cj_operand ops[len];
  va_list vl;
  va_start(vl, len);
  for (uint8_t i = 0; i < len; i++) ops[i] = va_arg(vl, cj_operand);
  va_end(vl);
  cj_add_bytes(ctx, inst.code, inst.len);
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
CJ_OP_META(inc, 1, (cj_variant){.operands={"r14"}, .codes={{0}}});
CJ_OP(mov, 2, 1, 0)
CJ_OP(nop, 0, 1, 0x90)
CJ_OP(ret, 0, 1, 0xc3)
CJ_OP(aad, 0, 2, 0xd5, 0x0a)
CJ_OP(aam, 0, 2, 0xd4, 0x0a)
CJ_OP(aaf, 0, 1, 0x3f)

#define EMIT(name, n, ...) cj_emit(ctx, name, n, ##__VA_ARGS__)

void cj_inc(cj_ctx* ctx, cj_operand op) {
  EMIT(inc, 1, op);
}

void cj_mov(cj_ctx* ctx, cj_operand op1, cj_operand op2) {
  EMIT(mov, 2, op1, op2);
}

void cj_ret(cj_ctx* ctx) {
  EMIT(ret, 0);
}

void cj_nop(cj_ctx* ctx) {
  EMIT(nop, 0);
}
#undef EMIT
#undef CJ_OP

