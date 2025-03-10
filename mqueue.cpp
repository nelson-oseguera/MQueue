#include "mqueue.h"
#include <iostream>
#include <stdexcept>

// Constructor implementation
MQueue::MQueue(prifn_t priFn, HEAPTYPE heapType, STRUCTURE structure)
    : m_heap(nullptr), m_size(0), m_priorFunc(priFn), m_heapType(heapType), m_structure(structure) {}

// Destructor implementation
MQueue::~MQueue() {
    clear();
}

// Copy constructor
MQueue::MQueue(const MQueue& rhs)
    : m_heap(nullptr), m_size(rhs.m_size), m_priorFunc(rhs.m_priorFunc),
      m_heapType(rhs.m_heapType), m_structure(rhs.m_structure) {
    m_heap = copyNodes(rhs.m_heap);
}

// Assignment operator
MQueue& MQueue::operator=(const MQueue& rhs) {
    if (this != &rhs) {  // Protect against self-assignment
        clear();

        m_priorFunc = rhs.m_priorFunc;
        m_heapType = rhs.m_heapType;
        m_structure = rhs.m_structure;
        m_size = rhs.m_size;

        Node* newHeap = copyNodes(rhs.m_heap);
        m_heap = newHeap;
    }
    return *this;
}

// Inserts an order into the queue
bool MQueue::insertOrder(const Order& input) {
    if (m_priorFunc(input) < 0) return false;
    Node* newNode = new Node(input);
    m_heap = merge(m_heap, newNode);
    ++m_size;
    return true;
}

// Retrieves the next order
Order MQueue::getNextOrder() {
    if (!m_heap) throw std::out_of_range("Queue is empty");
    Order nextOrder = m_heap->getOrder();
    Node* oldRoot = m_heap;
    m_heap = merge(m_heap->m_left, m_heap->m_right);
    delete oldRoot;
    --m_size;
    return nextOrder;
}

// Merges with another queue
void MQueue::mergeWithQueue(MQueue& rhs) {
    if (this == &rhs) throw std::domain_error("Cannot merge queue with itself.");
    if (m_priorFunc != rhs.m_priorFunc || m_structure != rhs.m_structure)
        throw std::domain_error("Queues must have the same priority function and structure.");
    m_heap = merge(m_heap, rhs.m_heap);
    m_size += rhs.m_size;
    rhs.m_heap = nullptr;
    rhs.m_size = 0;
}

// Clears the queue
void MQueue::clear() {
    deleteNodes(m_heap);
    m_heap = nullptr;
    m_size = 0;
}

// Recursively deletes nodes
void MQueue::deleteNodes(Node* node) {
    if (!node) return;
    deleteNodes(node->m_left);
    deleteNodes(node->m_right);
    delete node;
}

// Returns the number of orders
int MQueue::numOrders() const {
    return m_size;
}

// Prints the order queue
void MQueue::printOrderQueue() const {
    printPreOrder(m_heap);
    std::cout << std::endl;
}

// Returns the current priority function
prifn_t MQueue::getPriorityFn() const {
    return m_priorFunc;
}

// Sets a new priority function
void MQueue::setPriorityFn(prifn_t priFn, HEAPTYPE heapType) {
    m_priorFunc = priFn;
    m_heapType = heapType;
    rebuildHeap();
}

// Returns the heap type
HEAPTYPE MQueue::getHeapType() const {
    return m_heapType;
}

// Returns the structure type
STRUCTURE MQueue::getStructure() const {
    return m_structure;
}

// Sets the structure
void MQueue::setStructure(STRUCTURE structure) {
    m_structure = structure;
    rebuildHeap();
}

// Dumps the queue
void MQueue::dump() const {
    if (m_size == 0) {
        std::cout << "Empty heap.\n";
    } else {
        dump(m_heap);
    }
    std::cout << std::endl;
}

// Helper dump function
void MQueue::dump(Node* pos) const {
    if (pos != nullptr) {
        std::cout << "(";
        dump(pos->m_left);
        if (m_structure == SKEW)
            std::cout << m_priorFunc(pos->m_order) << ":" << pos->m_order.getCustomer();
        else
            std::cout << m_priorFunc(pos->m_order) << ":" << pos->m_order.getCustomer() << ":" << pos->m_npl;
        dump(pos->m_right);
        std::cout << ")";
    }
}

// Merges two nodes
Node* MQueue::merge(Node* node1, Node* node2) {
    if (!node1) return node2;
    if (!node2) return node1;

    if ((m_heapType == MINHEAP && m_priorFunc(node2->getOrder()) < m_priorFunc(node1->getOrder())) ||
        (m_heapType == MAXHEAP && m_priorFunc(node2->getOrder()) > m_priorFunc(node1->getOrder()))) {
        std::swap(node1, node2);
    }

    if (m_structure == SKEW) {
        node1->m_right = merge(node1->m_right, node2);
        std::swap(node1->m_left, node1->m_right);
    } else if (m_structure == LEFTIST) {
        node1->m_right = merge(node1->m_right, node2);
        if (!node1->m_left || node1->m_left->getNPL() < node1->m_right->getNPL()) {
            std::swap(node1->m_left, node1->m_right);
        }
        node1->setNPL(node1->m_right ? node1->m_right->getNPL() + 1 : 0);
    }

    return node1;
}

// Recursively copies nodes
Node* MQueue::copyNodes(Node* node) {
    if (!node) return nullptr;
    Node* newNode = new Node(node->getOrder());
    newNode->m_left = copyNodes(node->m_left);
    newNode->m_right = copyNodes(node->m_right);
    newNode->setNPL(node->getNPL());
    return newNode;
}

// Rebuilds the heap
void MQueue::rebuildHeap() {
    Order* tempOrders = new Order[m_size];
    int count = 0;

    while (m_heap) {
        tempOrders[count++] = getNextOrder();
    }

    for (int i = 0; i < count; ++i) {
        insertOrder(tempOrders[i]);
    }

    delete[] tempOrders;
}

// Prints preorder
void MQueue::printPreOrder(Node* node) const {
    if (node) {
        std::cout << node->getOrder() << " ";
        printPreOrder(node->m_left);
        printPreOrder(node->m_right);
    }
}

// Overloaded output operators
ostream& operator<<(ostream& sout, const Order& order) {
    sout  << "Customer: " << order.getCustomer()
          << ", importance: " << order.getImportance() 
          << ", quantity: " << order.getQuantity() 
          << ", order placement time: " << order.getFIFO();
    return sout;
}

ostream& operator<<(ostream& sout, const Node& node) {
    sout << node.m_order;
    return sout;
}