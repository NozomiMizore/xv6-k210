#include "types.h"
#include "param.h"
#include "memlayout.h"
#include "riscv.h"
#include "defs.h"
#include "sbi.h"

volatile static int started = 0;

// start() jumps here in supervisor mode on all CPUs.
void
main(unsigned long hartid, unsigned long dtb_pa)
{
  
  printf("hart %d enter main()...\n", hartid);
  if (hartid == 0) {
    printf("\n");
    printf("xv6-k210 kernel is booting\n");
    printf("\n");

    kinit();         // physical page allocator
    kvminit();       // create kernel page table
    kvminithart();   // turn on paging
    trapinit();      // trap vectors
    trapinithart();  // install kernel trap vector
    timerinit();     // set up timer interrupt handler
    procinit();
    device_init(dtb_pa);
    // plicinit();      // set up interrupt controller
    // plicinithart();  // ask PLIC for device interrupts
    // binit();         // buffer cache
    // iinit();         // inode cache
    // fileinit();      // file table
    // virtio_disk_init(); // emulated hard disk
    // userinit();      // first user process
    sbi_set_extern_interrupt((uint64)test_extern_interrupt - 0xffffffff00000000);
    test_kalloc();    // test kalloc
    test_vm(hartid);       // test kernel pagetable
    test_getchar();     // test sbi_console_getchar

    
    // sbi_set_extern_interrupt((uint64)test_extern_interrupt);
    for(int i = 1; i < NCPU; i++) {
      unsigned long mask = 1 << i;
      sbi_send_ipi(&mask);
    }
  }

  device_init(dtb_pa);
  // sbi_set_extern_interrupt((uint64)test_extern_interrupt);
  sbi_set_extern_interrupt((uint64)test_extern_interrupt - 0xffffffff00000000);
  
  printf("hart 1 init done\n");
  while (1);
  // scheduler();
  
  /* if(cpuid() == 0){
    consoleinit();
    printfinit();
    printf("\n");
    printf("xv6 kernel is booting\n");
    printf("\n");
    kinit();         // physical page allocator
    kvminit();       // create kernel page table
    kvminithart();   // turn on paging
    procinit();      // process table
    trapinit();      // trap vectors
    trapinithart();  // install kernel trap vector
    plicinit();      // set up interrupt controller
    plicinithart();  // ask PLIC for device interrupts
    binit();         // buffer cache
    iinit();         // inode cache
    fileinit();      // file table
    virtio_disk_init(); // emulated hard disk
    userinit();      // first user process
    __sync_synchronize();
    started = 1;
  } else {
    while(started == 0)
      ;
    __sync_synchronize();
    printf("hart %d starting\n", cpuid());
    kvminithart();    // turn on paging
    trapinithart();   // install kernel trap vector
    plicinithart();   // ask PLIC for device interrupts
  }

  scheduler(); */        
}
