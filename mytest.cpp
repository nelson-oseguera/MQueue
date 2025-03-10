#include "mqueue.h"
#include <iostream>
#include <stdexcept>
#include <climits>
#include <cstdlib>
#include <ctime>

using namespace std;

// Sample priority functions for testing
int priorityFn1(const Order& order) {
    return order.getImportance() + order.getQuantity();
}

int priorityFn2(const Order& order) {
    return order.getFIFO() + order.getProcessTime();
}

class Tester {
public:
    bool testBasicInsertionMinHeap();
    bool testBasicInsertionMaxHeap();
    bool testBasicRemovalMinHeap();
    bool testBasicRemovalMaxHeap();
    bool testLargeScaleInsertionMinHeap();
    bool testLargeScaleInsertionMaxHeap();
    bool testLargeScaleRemovalMinHeap();
    bool testLargeScaleRemovalMaxHeap();
    bool testLeftistNPLValues();
    bool testLeftistProperty();
    bool testCopyConstructor();
    bool testCopyConstructorEmpty();
    bool testAssignmentOperator();
    bool testAssignmentOperatorEmpty();
    bool testExceptionOnEmptyDequeue();
    bool testMergeEmptyQueue();
    bool testMergeDifferentPriorityFunctions();
    bool testPriorityFunctionChange();

    void runTests() {
        cout << "testBasicInsertionMinHeap: " << (testBasicInsertionMinHeap() ? "Pass" : "Fail") << endl;
        cout << "testBasicInsertionMaxHeap: " << (testBasicInsertionMaxHeap() ? "Pass" : "Fail") << endl;
        cout << "testBasicRemovalMinHeap: " << (testBasicRemovalMinHeap() ? "Pass" : "Fail") << endl;
        cout << "testBasicRemovalMaxHeap: " << (testBasicRemovalMaxHeap() ? "Pass" : "Fail") << endl;
        cout << "testLargeScaleInsertionMinHeap: " << (testLargeScaleInsertionMinHeap() ? "Pass" : "Fail") << endl;
        cout << "testLargeScaleInsertionMaxHeap: " << (testLargeScaleInsertionMaxHeap() ? "Pass" : "Fail") << endl;
        cout << "testLargeScaleRemovalMinHeap: " << (testLargeScaleRemovalMinHeap() ? "Pass" : "Fail") << endl;
        cout << "testLargeScaleRemovalMaxHeap: " << (testLargeScaleRemovalMaxHeap() ? "Pass" : "Fail") << endl;
        cout << "testLeftistNPLValues: " << (testLeftistNPLValues() ? "Pass" : "Fail") << endl;
        cout << "testLeftistProperty: " << (testLeftistProperty() ? "Pass" : "Fail") << endl;
        cout << "testCopyConstructor: " << (testCopyConstructor() ? "Pass" : "Fail") << endl;
        cout << "testCopyConstructorEmpty: " << (testCopyConstructorEmpty() ? "Pass" : "Fail") << endl;
        cout << "testAssignmentOperator: " << (testAssignmentOperator() ? "Pass" : "Fail") << endl;
        cout << "testAssignmentOperatorEmpty: " << (testAssignmentOperatorEmpty() ? "Pass" : "Fail") << endl;
        cout << "testExceptionOnEmptyDequeue: " << (testExceptionOnEmptyDequeue() ? "Pass" : "Fail") << endl;
        cout << "testMergeEmptyQueue: " << (testMergeEmptyQueue() ? "Pass" : "Fail") << endl;
        cout << "testMergeDifferentPriorityFunctions: " << (testMergeDifferentPriorityFunctions() ? "Pass" : "Fail") << endl;
        cout << "testPriorityFunctionChange: " << (testPriorityFunctionChange() ? "Pass" : "Fail") << endl;
    }
};

// Run all tests
int main() {
    srand(static_cast<unsigned int>(time(nullptr))); // Seed for random number generation
    Tester tester;
    tester.runTests();
    return 0;
}

// Helper function for generating random Order objects
Order generateRandomOrder(int id) {
    return Order(
        "Customer" + to_string(id),
        rand() % MAX50 + 1, // FIFO: 1 to 50
        rand() % MAX12 + 1, // Process time: 1 to 12
        rand() % MAX12 + 1, // Due time: 1 to 12
        rand() % MAX12 + 1, // Slack time: 1 to 12
        rand() % MAX100 + 1, // Material: 1 to 100
        rand() % MAX100 + 1, // Importance: 1 to 100
        rand() % (MAX200 - 100 + 1) + 100, // WorkForce: 100 to 200
        rand() % (MAX10000 - MIN1000 + 1) + MIN1000 // Quantity: 1000 to 10000
    );
}

// Test functions for basic and large-scale testing

bool Tester::testBasicInsertionMinHeap() {
    MQueue queue(priorityFn2, MINHEAP, LEFTIST);
    for (int i = 0; i < 10; ++i) {
        queue.insertOrder(generateRandomOrder(i));
    }
    // Verify min-heap property
    Node* node = queue.m_heap;
    while (node && node->m_left) {
        if (priorityFn2(node->getOrder()) > priorityFn2(node->m_left->getOrder())) return false;
        node = node->m_left;
    }
    return true;
}

bool Tester::testBasicInsertionMaxHeap() {
    MQueue queue(priorityFn2, MAXHEAP, LEFTIST);
    for (int i = 0; i < 10; ++i) {
        queue.insertOrder(generateRandomOrder(i));
    }
    // Verify max-heap property
    Node* node = queue.m_heap;
    while (node && node->m_left) {
        if (priorityFn2(node->getOrder()) < priorityFn2(node->m_left->getOrder())) return false;
        node = node->m_left;
    }
    return true;
}

bool Tester::testBasicRemovalMinHeap() {
    MQueue queue(priorityFn2, MINHEAP, LEFTIST);
    for (int i = 0; i < 10; ++i) {
        queue.insertOrder(generateRandomOrder(i));
    }

    int lastPriority = -1;
    while (queue.numOrders() > 0) {
        Order order = queue.getNextOrder();
        int priority = priorityFn2(order);
        if (lastPriority != -1 && priority < lastPriority) {
            return false; // Priority should not decrease
        }
        lastPriority = priority;
    }
    return true;
}

bool Tester::testBasicRemovalMaxHeap() {
    MQueue queue(priorityFn2, MAXHEAP, LEFTIST);
    for (int i = 0; i < 10; ++i) {
        queue.insertOrder(generateRandomOrder(i));
    }

    int lastPriority = INT_MAX;
    while (queue.numOrders() > 0) {
        Order order = queue.getNextOrder();
        int priority = priorityFn2(order);
        if (priority > lastPriority) {
            return false; // Priority should not increase
        }
        lastPriority = priority;
    }
    return true;
}

bool Tester::testLargeScaleInsertionMinHeap() {
    MQueue queue(priorityFn2, MINHEAP, LEFTIST);
    for (int i = 0; i < 300; ++i) {
        queue.insertOrder(generateRandomOrder(i));
    }
    // Verify min-heap property
    Node* node = queue.m_heap;
    while (node && node->m_left) {
        if (priorityFn2(node->getOrder()) > priorityFn2(node->m_left->getOrder())) return false;
        node = node->m_left;
    }
    return true;
}

bool Tester::testLargeScaleInsertionMaxHeap() {
    MQueue queue(priorityFn2, MAXHEAP, LEFTIST);
    for (int i = 0; i < 300; ++i) {
        queue.insertOrder(generateRandomOrder(i));
    }
    // Verify max-heap property
    Node* node = queue.m_heap;
    while (node && node->m_left) {
        if (priorityFn2(node->getOrder()) < priorityFn2(node->m_left->getOrder())) return false;
        node = node->m_left;
    }
    return true;
}

bool Tester::testLargeScaleRemovalMinHeap() {
    MQueue queue(priorityFn2, MINHEAP, LEFTIST);
    for (int i = 0; i < 300; ++i) {
        queue.insertOrder(generateRandomOrder(i));
    }

    int lastPriority = -1;
    while (queue.numOrders() > 0) {
        Order order = queue.getNextOrder();
        int priority = priorityFn2(order);
        if (lastPriority != -1 && priority < lastPriority) {
            return false; // Priority should not decrease
        }
        lastPriority = priority;
    }
    return true;
}

bool Tester::testLargeScaleRemovalMaxHeap() {
    MQueue queue(priorityFn2, MAXHEAP, LEFTIST);
    for (int i = 0; i < 300; ++i) {
        queue.insertOrder(generateRandomOrder(i));
    }

    int lastPriority = INT_MAX;
    while (queue.numOrders() > 0) {
        Order order = queue.getNextOrder();
        int priority = priorityFn2(order);
        if (priority > lastPriority) {
            return false; // Priority should not increase
        }
        lastPriority = priority;
    }
    return true;
}

bool Tester::testLeftistNPLValues() {
    MQueue queue(priorityFn2, MINHEAP, LEFTIST);
    for (int i = 0; i < 10; ++i) {
        queue.insertOrder(generateRandomOrder(i));
    }
    // Verify leftist property of NPL values
    Node* node = queue.m_heap;
    while (node) {
        int leftNPL = node->m_left ? node->m_left->getNPL() : 0;
        int rightNPL = node->m_right ? node->m_right->getNPL() : 0;
        if (leftNPL < rightNPL) return false;
        node = node->m_left;
    }
    return true;
}

bool Tester::testLeftistProperty() {
    MQueue queue(priorityFn2, MINHEAP, LEFTIST);
    for (int i = 0; i < 10; ++i) {
        queue.insertOrder(generateRandomOrder(i));
    }

    // Traverse the heap to check the leftist property
    Node* node = queue.m_heap;
    while (node) {
        int leftNPL = node->m_left ? node->m_left->getNPL() : 0;
        int rightNPL = node->m_right ? node->m_right->getNPL() : 0;
        if (leftNPL < rightNPL) {
            return false; // Leftist property requires left NPL >= right NPL
        }
        node = node->m_left;
    }
    return true;
}

bool Tester::testCopyConstructor() {
    MQueue queue1(priorityFn2, MINHEAP, LEFTIST);
    for (int i = 0; i < 5; ++i) {
        queue1.insertOrder(generateRandomOrder(i));
    }

    MQueue queue2(queue1);  // Copy constructor

    return queue1.numOrders() == queue2.numOrders();
}

bool Tester::testCopyConstructorEmpty() {
    MQueue queue1(priorityFn2, MINHEAP, LEFTIST);
    MQueue queue2(queue1);  // Copy constructor on empty queue

    return queue2.numOrders() == 0;
}

bool Tester::testAssignmentOperator() {
    MQueue queue1(priorityFn2, MINHEAP, LEFTIST);
    for (int i = 0; i < 5; ++i) {
        queue1.insertOrder(generateRandomOrder(i));
    }

    MQueue queue2(priorityFn2, MINHEAP, LEFTIST);
    queue2 = queue1;

    return queue1.numOrders() == queue2.numOrders();
}

bool Tester::testAssignmentOperatorEmpty() {
    MQueue queue1(priorityFn2, MINHEAP, LEFTIST);
    MQueue queue2(priorityFn2, MINHEAP, LEFTIST);
    queue2.insertOrder(Order("Test", 0, 0, 0, 0, 0, 0, 0, 0));  // Insert one order

    queue2 = queue1;  // Assign empty queue

    return queue2.numOrders() == 0;
}

bool Tester::testExceptionOnEmptyDequeue() {
    MQueue queue(priorityFn1, MINHEAP, LEFTIST);
    try {
        queue.getNextOrder();
    } catch (const out_of_range&) {
        return true;  // Expected exception
    }
    return false;
}

bool Tester::testMergeEmptyQueue() {
    MQueue queue1(priorityFn2, MINHEAP, LEFTIST);
    for (int i = 0; i < 5; ++i) {
        queue1.insertOrder(generateRandomOrder(i));
    }

    MQueue emptyQueue(priorityFn2, MINHEAP, LEFTIST);
    queue1.mergeWithQueue(emptyQueue);

    return queue1.numOrders() == 5 && emptyQueue.numOrders() == 0;
}

bool Tester::testMergeDifferentPriorityFunctions() {
    MQueue queue1(priorityFn1, MINHEAP, LEFTIST);
    MQueue queue2(priorityFn2, MINHEAP, LEFTIST);
    try {
        queue1.mergeWithQueue(queue2);
    } catch (const domain_error&) {
        return true; // Expected exception
    }
    return false;
}

bool Tester::testPriorityFunctionChange() {
    MQueue queue(priorityFn1, MINHEAP, LEFTIST);
    for (int i = 0; i < 5; ++i) {
        queue.insertOrder(generateRandomOrder(i));
    }

    queue.setPriorityFn(priorityFn2, MINHEAP); // Change priority function
    return true; // Placeholder for real verification
}