#include <kernel.h>

void sys_shutdown() {
  __asm__ volatile("hlt");
}

void syscall_handler(uint32_t eax, uint32_t ebx, uint32_t ecx, uint32_t edx) {
  
}

void init_syscalls() {
  for (int i = 0; i < NUM_SYSCALLS; i++) {
    syscall_t call;
    call.syscall = i;
    call.handler = 0;
    kernel_data.syscall_table[i] = call;
  }

  kernel_data.syscall_table[0].handler = &sys_shutdown;

  idt_set_gate(0x80, (unsigned long int)&syscall_gate, 0x8, 0x8e); //Setup sys_int handler (int 0x80)
}

void print_mmap(multiboot_info_t *mbt) {
  multiboot_memory_map_t *mmap = (multiboot_memory_map_t*)mbt->mmap_addr;
  while ((uint32_t)mmap < (uint32_t)(mbt->mmap_addr + mbt->mmap_length)) {
      switch (mmap->type) {
      case 1:
          printk("[");
          k_printhex(mmap->addr);
          printk(" - ");
          k_printhex(mmap->addr + mmap->len);
          printk("] Available\n");
          break;
      case 2:
          printk("[");
          k_printhex(mmap->addr);
          printk(" - ");
          k_printhex(mmap->addr + mmap->len);
          printk("] Reserved\n");
          break;
      case 3:
          printk("[");
          k_printhex(mmap->addr);
          printk(" - ");
          k_printhex(mmap->addr + mmap->len);
          printk("] ACPI Reserved\n");
          break;
      case 4:
          printk("NVS\n");
          break;
      case 5:
          printk("Bad RAM\n");
          break;
      }
      mmap = (multiboot_memory_map_t*)((unsigned int)mmap + mmap->size + sizeof(mmap->size));
  }
}
