#pragma once

#include "ctx.h"

typedef struct {
  const char* name;
} cj_operand;

void cj_inc(cj_ctx*, cj_operand);
void cj_mov(cj_ctx*, cj_operand, cj_operand);
void cj_nop(cj_ctx*);
void cj_ret(cj_ctx*);
