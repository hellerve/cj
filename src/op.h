#pragma once

#include "ctx.h"

enum {
  CJ_REGISTER,
  CJ_CONSTANT
};

typedef struct {
  uint8_t type;
  union {
    uint64_t constant;
    const char* reg;
  };
} cj_operand;

void cj_inc(cj_ctx*, cj_operand);
void cj_mov(cj_ctx*, cj_operand, cj_operand);
void cj_nop(cj_ctx*);
void cj_ret(cj_ctx*);
