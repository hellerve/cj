#pragma once

#include <stdint.h>

typedef void (*cj_fn)(void);

typedef struct {
  uint8_t* mem;
  uint64_t len;
  uint64_t size;
} cj_ctx;

cj_ctx* create_cj_ctx();
void cj_add_u8(cj_ctx*, uint8_t);
void cj_add_u16(cj_ctx*, uint16_t);
void cj_add_u32(cj_ctx*, uint32_t);
void cj_add_u64(cj_ctx*, uint64_t);
void cj_add_bytes(cj_ctx*, uint8_t*, uint64_t);
void destroy_cj_ctx(cj_ctx*);
cj_fn create_cj_fn();
void destroy_cj_fn(cj_ctx*, cj_fn);
