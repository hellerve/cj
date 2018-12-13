#include <assert.h>
#include <stdarg.h>

#include "op.h"

typedef struct {
  uint8_t nargs;
  uint8_t code;
  const char* name;
} cj_instruction;

#define CJ_OP(NAME, N, CODE) \
  static const cj_instruction NAME = (cj_instruction){.name = "NAME",\
                                                      .nargs = N,\
                                                      .code = CODE};

// of course its not that simple
CJ_OP(inc, 1, 0)
CJ_OP(mov, 2, 0)
CJ_OP(nop, 0, 0x90)
CJ_OP(ret, 0, 0xc3)

#undef CJ_OP

void cj_emit(cj_ctx* ctx, cj_instruction inst, uint64_t len, ...) {
  assert(len == inst.nargs);
  cj_operand ops[len];
  va_list vl;
  va_start(vl, len);
  for (uint8_t i = 0; i < len; i++) ops[i] = va_arg(vl, cj_operand);
  va_end(vl);
  cj_add_u8(ctx, inst.code);
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
