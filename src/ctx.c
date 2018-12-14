#define _DEFAULT_SOURCE
#include <stdlib.h>
#include <string.h>
#include <sys/mman.h>

#include "ctx.h"

cj_ctx* create_cj_ctx() {
  cj_ctx* res = malloc(sizeof(cj_ctx));
  res->mem = calloc(1, 32); // these are bytes, so this is fine; ugly?
  res->len = 0;
  res->size = 32;
  return res;
}

void grow_cj_ctx(cj_ctx* ctx) {
  uint64_t half = ctx->size;
  ctx->size *= 2;
  ctx->mem = realloc(ctx->mem, ctx->size);
  memset(ctx->mem+half, 0, half);
}

void destroy_cj_ctx(cj_ctx* ctx) {
  free(ctx->mem);
  free(ctx);
}

cj_fn create_cj_fn(cj_ctx* ctx) {
  if (!ctx->len) return NULL;
  void* mem = mmap(0, ctx->len, PROT_READ | PROT_WRITE,
                   MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);
  memcpy(mem, ctx->mem, ctx->len);
  mprotect(mem, ctx->len, PROT_READ | PROT_EXEC);
  return (cj_fn)mem;
}

void destroy_cj_fn(cj_ctx* ctx, cj_fn mem) {
  munmap((void*)mem, ctx->len);
}

void cj_add_u8(cj_ctx* ctx, uint8_t byte) {
  if (ctx->size < ctx->len) grow_cj_ctx(ctx);

  ctx->mem[ctx->len++] = byte;
}

void cj_add_u16(cj_ctx* ctx, uint16_t b2) {
  cj_add_u8(ctx, b2&0xff);
  cj_add_u8(ctx, (b2>>8)&0xff);
}

void cj_add_u32(cj_ctx* ctx, uint32_t b4) {
  cj_add_u16(ctx, b4&0xffff);
  cj_add_u16(ctx, (b4>>16)&0xffff);
}

void cj_add_u64(cj_ctx* ctx, uint64_t b8) {
  cj_add_u32(ctx, b8&0xffffffff);
  cj_add_u32(ctx, (b8>>32)&0xffffffff);
}

void cj_add_bytes(cj_ctx* ctx, uint8_t* bytes, uint64_t len) {
  for (uint64_t i = 0; i < len; i++) cj_add_u8(ctx, bytes[i]);
}
