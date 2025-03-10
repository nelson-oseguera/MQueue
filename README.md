# MQueue

This project is a C++ implementation of a dynamic priority queue system for managing manufacturing orders, using Skew Heaps and Leftist Heaps, configurable as either Min-Heap or Max-Heap. It allows flexible prioritization of orders based on user-defined criteria (priority functions), robust memory management, and supports advanced queue operations such as merges and structure/priority-function transformations. A comprehensive testing suite is also included for thorough verification.

Core Features

1. Order Priority Queue System
Dynamically manages manufacturing orders based on multiple attributes:
Customer name, FIFO order, process time, due date, slack time, material availability, importance, workforce, quantity.
Orders are prioritized using customizable priority functions to fit various business needs.
Queue can act as a Min-Heap (lowest priority value = highest priority) or Max-Heap (highest value = highest priority).

2. Configurable Heap Structures
Supports two heap types:
Skew Heap: Efficient self-adjusting binary heap.
Leftist Heap: Optimized for efficient merging, maintains Null Path Length (NPL) invariant.
Dynamic rebuilding when changing heap structure or priority function.

3. Object-Oriented Design
Order Class:
Represents an individual order with attributes used for prioritization.
Node Class:
Represents a heap node, wrapping an Order with left/right children and NPL (for Leftist Heaps).
MQueue Class:
Manages the priority queue with all core operations:
Insert/Remove Orders
Merge Queues
Set/Change Priority Function and Heap Structure
Clear and Memory Safety
Dump and Traverse Queue (Preorder Traversal)

4. Supported Operations
Operation	Description
insertOrder()	Insert a new order into the queue
getNextOrder()	Remove and return the order with highest priority
mergeWithQueue()	Merge two compatible queues
clear()	Clear and deallocate the entire queue
setPriorityFn()	Change priority function and rebuild heap
setStructure()	Change between Skew/Leftist and rebuild heap
dump()	Display queue structure (tree) for debugging
printOrderQueue()	Print queue in preorder traversal

5. Priority Functions
Two sample priority functions defined:
priorityFn1: Sum of material, importance, workforce, and quantity (MAXHEAP style).
priorityFn2: Sum of FIFO, process time, due time, and slack time (MINHEAP style).

6. Memory Management & Deep Copy
Full support for:
Copy Constructor: Deep copies of queues.
Assignment Operator: Safe deep assignment.
Destructor: Proper deallocation without leaks.

7. Robust Testing Suite (mytest.cpp)
Includes 18 automated tests covering:

Test Case	Purpose
Basic insertion/removal for Min/Max Heaps	Validate simple functionality
Large scale insertion/removal	Ensure performance and correctness under load
Leftist Heap NPL value verification	Ensure correct leftist properties
Copy constructor/Assignment (normal/empty cases)	Verify deep copying
Exception on dequeue from empty queue	Error handling correctness
Merging queues (normal/error)	Test merging, including incompatible queues
Priority function/structure change	Verify correct rebuilding

Skills & Concepts Demonstrated
Priority Queues using Skew and Leftist Heaps.
Dynamic restructuring of queues based on priority and structure.
Advanced memory management: deep copy, safe assignment, destructors.
Customizable priority functions.
Testing-driven development with comprehensive unit tests.
Random data generation for testing.
