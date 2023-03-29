// Physical memory allocator, intended to allocate
// memory for user processes, kernel stacks, page table pages,
// and pipe buffers. Allocates 4096-byte pages.

#include "types.h"
#include "defs.h"
#include "param.h"
#include "mmu.h"
#include "spinlock.h"

struct run {
  struct run *next;
};

struct {
  struct spinlock lock;
  struct run *freelist;
    uint free_pages;                // track free pages
  uint ref_cnt[PHYSTOP / PGSIZE]; // track reference count

} kmem;

extern char end[]; // first address after kernel loaded from ELF file

// Initialize free list of physical pages.
void
kinit(void)
{
  char *p;

  initlock(&kmem.lock, "kmem");
   p = (char *)PGROUNDUP((uint)end);
  kmem.free_pages = 0;
  for (; p + PGSIZE <= (char *)PHYSTOP; p += PGSIZE){
    kmem.ref_cnt[PADDR(p) / PGSIZE] = 0;
    kfree(p);}
}

// Free the page of physical memory pointed at by v,
// which normally should have been returned by a
// call to kalloc().  (The exception is when
// initializing the allocator; see kinit above.)
void
kfree(char *v)
{
  struct run *r;

  if((uint)v % PGSIZE || v < end || (uint)v >= PHYSTOP) 
    panic("kfree");
// Fill with junk to catch dangling refs.
  // memset(v, 1, PGSIZE);
  uint pa = PADDR(v);
  acquire(&kmem.lock);
  // if it's already freed then panic
  if (kmem.ref_cnt[pa / PGSIZE] <= 1)
  {
    kmem.ref_cnt[pa / PGSIZE] = 0;
    memset(v, 1, PGSIZE);
    r = (struct run *)v;
    kmem.free_pages++;
    r->next = kmem.freelist;
    kmem.freelist = r;
    release(&kmem.lock);
  }
  else
  {
    kmem.ref_cnt[pa / PGSIZE]--;
    release(&kmem.lock);
  }

  
}
// NEW
uint num_of_FreePages(void)
{
  acquire(&kmem.lock);

  uint num_free_pages = kmem.free_pages;

  release(&kmem.lock);

  return num_free_pages;
}

// Allocate one 4096-byte page of physical memory.
// Returns a pointer that the kernel can use.
// Returns 0 if the memory cannot be allocated.
char*
kalloc(void)
{
  struct run *r;

  acquire(&kmem.lock);
  r = kmem.freelist;
  if(r)
    kmem.freelist = r->next;
      kmem.ref_cnt[(uint)r / PGSIZE] = 1; // NEW
  kmem.free_pages--;   
  release(&kmem.lock);
  return (char*)r;
}

void increment_refC(uint pa)
{
  if (pa >= PHYSTOP || pa < PADDR(end))
    panic("increment_refC: invalid physical address");

  acquire(&kmem.lock);
  kmem.ref_cnt[pa / PGSIZE]++;
  release(&kmem.lock);
}
// NEW
void decrement_refC(uint pa)
{
  if (pa >= PHYSTOP || pa < PADDR(end))
    panic("decrement_refC: invalid physical address");

  acquire(&kmem.lock);
  kmem.ref_cnt[pa / PGSIZE]--;
  release(&kmem.lock);
}

uint get_refC(uint pa)
{
  if (pa >= PHYSTOP || pa < PADDR(end)) {
    // cprintf("bad pa %d\n", pa);
    panic("getReferenceCount"); 
  }

  uint count;
  acquire(&kmem.lock);
  count = kmem.ref_cnt[pa / PGSIZE];
  release(&kmem.lock);

  return count;

} 

