#include "register.h"
#include "op.h"

int main() {
  cj_ctx* cj = create_cj_ctx();
  //cj_inc(cj, cj_r14);
  //cj_mov(cj, cj_r13, cj_r14);
  cj_nop(cj);
  cj_ret(cj);

  cj_fn f = create_cj_fn(cj);

  f();

  destroy_cj_fn(cj, f);
  destroy_cj_ctx(cj);
}
