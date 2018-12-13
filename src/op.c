#include <assert.h>
#include <stdarg.h>

#include "op.h"

typedef struct {
  uint8_t nargs;
  uint8_t len;
  uint8_t code[8];
  const char* name;
} cj_instruction;

#define CJ_OP(NAME, N, L, ...) \
  static const cj_instruction NAME = (cj_instruction){.name = "NAME",\
                                                      .nargs = N,\
                                                      .len = L,\
                                                      .code = {__VA_ARGS__}};

// of course its not that simple
CJ_OP(inc, 1, 1, 0)
CJ_OP(mov, 2, 1, 0)
CJ_OP(nop, 0, 1, 0x90)
CJ_OP(ret, 0, 1, 0xc3)
CJ_OP(aad, 0, 2, 0xd5, 0x0a)
CJ_OP(aam, 0, 2, 0xd4, 0x0a)
CJ_OP(aaf, 0, 1, 0x3f)

#undef CJ_OP

void cj_emit(cj_ctx* ctx, cj_instruction inst, uint64_t len, ...) {
  assert(len == inst.nargs);
  cj_operand ops[len];
  va_list vl;
  va_start(vl, len);
  for (uint8_t i = 0; i < len; i++) ops[i] = va_arg(vl, cj_operand);
  va_end(vl);
  cj_add_bytes(ctx, inst.code, inst.len);
}

void cj_inc(cj_ctx* ctx, cj_operand op) {
  cj_emit(ctx, inc, 1, op);
}

void cj_mov(cj_ctx* ctx, cj_operand op1, cj_operand op2) {
  cj_emit(ctx, mov, 2, op1, op2);
}

void cj_ret(cj_ctx* ctx) {
  cj_emit(ctx, ret, 0);
}

void cj_nop(cj_ctx* ctx) {
  cj_emit(ctx, nop, 0);
}
