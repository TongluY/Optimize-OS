## Project Introduction
This project focuses on understanding Unix-style forking and x86 memory management through the implementation of a copy-on-write `fork()` system call and the design of a Stride Scheduler algorithm in the xv6 kernel.

## Features and Implementations

### 1. Copy-on-Write Fork() System Call
**Objective:** Implement a copy-on-write `fork()` system call in the xv6 kernel.

**Implementation:**
- Developed a trap handler for page faults.
- Augmented the physical memory management code.
- Manipulated page tables to support copy-on-write.

**Benefits:** 
This implementation significantly reduces the time required for forking large processes by avoiding unnecessary copying of memory pages until they are written to.

### 2. Stride Scheduler Algorithm
**Objective:** Design and implement a Stride Scheduler algorithm in the xv6 kernel.

**Implementation:**
- Assigned tickets to each process.
- Calculated the stride for each process based on the number of tickets.
- Scheduled the process with the minimum pass value in each time slot.
- Created two new system calls:
  - `settickets(int number)`: Sets the number of tickets for a process.
  - `getpinfo(struct pstat*)`: Returns information about running processes.

**Benefits:**
The Stride Scheduler algorithm improves CPU utilization and response time by fairly distributing CPU time among processes based on their assigned tickets.

## Performance Evaluation
Conducted experiments to demonstrate the performance improvements achieved by implementing the copy-on-write `fork()` and the Stride Scheduler.

Generated graphs showcasing:
- Reduction in time required for forking large processes.
- Enhanced CPU utilization.
- Improved response time of the system.

## Results and Conclusion
The implementation of copy-on-write and the Stride Scheduler in the xv6 kernel led to significant performance improvements. The experiments and graphs provide insights into further optimizing the implementation for even better system performance.

## Future Work
- Further optimization of the copy-on-write implementation.
- Enhancements in the Stride Scheduler algorithm.
- Additional experiments to explore other scheduling algorithms and memory management techniques.

## Contact Information
For any questions or contributions, please contact:

**Name:** Tonglu Yang  
**Email:** [tyang328@wisc.edu](mailto:tyang328@wisc.edu)

## Acknowledgments
