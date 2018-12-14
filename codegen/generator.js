const asmdb = require("asmdb");
const x86 = new asmdb.x86util.X86DataBase().addDefault().map;

function mkoperands(operands) {
  return `{${operands.map(op=>op.reg||"__constant__")}}`;
}

function mkvariants(variants) {
  let strs = [];
  for (variant of variants) {
    let operands = variant.operands;
    let oc = variant.opcode;
    strs.push(`CJ_VARIANT(${operands.length}, ${mkoperands(operands)}, 1, ${oc})`);
  }
  return strs.join(', ');
}

for (group in x86) {
  let variants = x86[group];
  // TODO: do instructions always have the same operand count?
  let opl = variants[0].operands.length;
  console.log(`CJ_OP(${group}, ${variants.length}, ${mkvariants(variants)});`);
  console.log(`EMIT0(${group}, ${opl});`);
}
