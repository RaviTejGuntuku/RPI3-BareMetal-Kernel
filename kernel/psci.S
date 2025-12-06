// psci.S
    .text
    .align  4
    .global psci_cpu_on_asm

// uint64_t psci_cpu_on_asm(uint64_t fnid,
//                          uint64_t target_cpu,
//                          uint64_t entry,
//                          uint64_t context)
//
// AArch64 calling convention:
//   x0 = fnid
//   x1 = target_cpu
//   x2 = entry
//   x3 = context
// Return in x0.
psci_cpu_on_asm:
    smc #0
    ret