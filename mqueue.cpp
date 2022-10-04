// CMSC 341 - Fall 2021 - Project 3 - MQueue Class 
//Domain errors and clear is segfaulting
#include "mqueue.h"
using namespace std;
using std::domain_error;

MQueue::MQueue(prifn_t priFn) {
  //This is the constructor for the MQueue class. It must be provided with a pointer to the prioritization function.
  
  m_heap = nullptr;
  m_size = 0;
  m_priorFunc = priFn;
}

MQueue::~MQueue() {
  //Destructor, all dynamically-allocated data must be deallocated.

  clear();
  m_heap = nullptr;
}

MQueue::MQueue(const MQueue& rhs) {
  //Copy constructor, must make a deep copy of the rhs object. It must function correctly if rhs is empty.
  //only copying one order, not all of them
  if(rhs.m_size == 0) {
    return;
  } else {
    m_heap = new Node(rhs.m_heap->getOrder());
    m_size = rhs.m_size;
    m_priorFunc = rhs.m_priorFunc;
  }
}


MQueue& MQueue::operator=(const MQueue& rhs) {
  //Assignment operator, remember to check for self-assignment and to free all dynamically allocated data members of the host. 
  //You should not call the copy constructor in the implementation of the assignment operator.

  if(this != &rhs) {
    delete [] m_heap;
    m_heap = new Node(rhs.m_heap->getOrder());
    m_size = rhs.m_size;
    m_priorFunc = rhs.m_priorFunc;
  }
  return *this;
  
}

void MQueue::insertOrder(const Order& input) {
  //Insert an Order into the queue. Must maintain the min-heap property

  if(m_heap == nullptr) {
    m_heap = new Node(input);
    this->m_size = this->m_size + 1;
  } else {
    Node * p2 = new Node(input);
    this->m_heap = mergeWithQueueHelp(this->m_heap, p2);
    this->m_size = this->m_size + 1;
  }
}

Order MQueue::getNextOrder() {
  //Extract (remove the node) and return the highest priority order from the queue. 
  //Must maintain the min-heap property. Should throw a domain_error exception if the queue is empty when the function is called.

  //Should throw a domain_error exception if the queue is empty when the function is called.

  if(m_heap != nullptr) {
    Node * temp = m_heap;
    Order order = temp->getOrder();
    m_heap = mergeWithQueueHelp(m_heap->m_left, m_heap->m_right);
    //remove the node and decrease the size
    delete temp;
    m_size--;
    return order;
  } 
  else {
    try {
      if(m_heap == nullptr) {
        throw(std::domain_error("Queue is empty"));
      }
    }
    catch (...) {
      cout << "The queue is empty" << endl;
    }
  }
}


void MQueue::mergeWithQueue(MQueue& rhs) {
  //Merge the host queue with the rhs; leaves rhs empty. Two skew heaps can only be merged if they have the same priority function. 
  //If the user attempts to merge queues with different priority functions, a domain_error exception should be thrown. 
  //This function requires protection against self-merging. Merging a heap with itself is not allowed.

  if(this->m_heap == rhs.m_heap) {
    //Merging a heap with itself is not allowed.
    return;
  }

  if(this->m_priorFunc == rhs.m_priorFunc) {
    m_heap = mergeWithQueueHelp(m_heap, rhs.m_heap);
    rhs.m_heap = nullptr;
    rhs.m_size = 0;
  } else {
    try {
      if(this->m_priorFunc != rhs.m_priorFunc) {
        throw(std::domain_error("Priority function not the same"));
      }
    }
    catch (...) {
      cout << "Priority function is not the same" << endl;
    }
  }
}

Node * MQueue::mergeWithQueueHelp(Node * p1, Node * p2) {
  //pass in node by node
  //rhs is p2
  //this is p1
  // If p1 is Null, return p2; similarly, if p2 is Null, return p1.
  // Assume that p1 has higher priority than p2; if not, swap, p1 and p2.
  // Swap the left and right subtrees of p1.
  // Recursively merge p2 and the left subtree of p1, replacing the left subtree of p1 with the result of the recursive merge.
  

  if(p1 == nullptr) {
    return p2;
  }

  if(p2 == nullptr) {
    return p1;
  }

  //if p1 priority is less than p2 swap p1 and p2, swap p1 left and p1 right no matter what, recusively merge left child p1 with p2
  // Assume that p1 has higher priority than p2; if not, swap, p1 and p2.
  //lower value means higher priority
  
  if(m_priorFunc(p1->getOrder()) > m_priorFunc(p2->getOrder())) { //>=???
    swap(p1, p2);
  }
  swap(p1->m_left, p1->m_right);
  p1->m_left = mergeWithQueueHelp(p1->m_left, p2);

  return p1;
}

void MQueue::clear() {
  //Clear the queue. Must delete all the nodes in the skew heap, leaving the skew heap empty.
  if(m_heap == nullptr) {
    try {
      if(m_heap == nullptr) {
        throw(std::domain_error("Queue is empty"));
      }
    }
    catch (...) {
      cout << "The queue is empty" << endl;
    }
  } else {
    Node * temp = m_heap;
    clearHelp(temp);
  }
}

void MQueue::clearHelp(Node * temp) {
  if(temp == nullptr) {
    return;
  }

  if(temp != nullptr) {
    clearHelp(temp->m_left);
    clearHelp(temp->m_right);
    delete temp;
  }
}
 
int MQueue::numOrders() const {
  //Return the current number of orders in the queue.
  return m_size;
}

void MQueue::printOrderQueue() const {
  //Print the contents of the queue using preorder traversal. 
  //Although the first order printed should have the highest priority, the remaining orders will not necessarily be in priority order. 
  //Please refer to the sample output file (driver.txt) for the format of this function's output.

  Node * temp = m_heap;
  printOrderQueueHelp(temp);
}

void MQueue::printOrderQueueHelp(Node * temp) const {
  if(temp == nullptr) {
    return;
  }

  if(temp != nullptr) {
    //preorder trasversal
    cout << "[" << m_priorFunc(temp->getOrder()) << "] " << temp->getOrder() << endl;
    printOrderQueueHelp(temp->m_left);
    printOrderQueueHelp(temp->m_right);
  }

}


prifn_t MQueue::getPriorityFn() const {
  //Get the current priority function.
  return m_priorFunc;
  
}

void MQueue::setPriorityFn(prifn_t priFn) {
  //Set a new priority function. It must rebuild the heap!

  //make a new queue and insert everything get order returns
  MQueue p1(priFn);

  while(m_heap != nullptr){
    p1.insertOrder(this->getNextOrder());
  }

  //copy everything over
  m_heap = p1.m_heap;
  m_size = p1.m_size;
  m_priorFunc = priFn;

  p1.m_heap = nullptr;
 
}

// overloaded insertion operator for Order class
ostream& operator<<(ostream& sout, const Order& order) {
  sout << "Order: " << order.getCustomer() << ", PM priority: " << order.getPMPriority()
       << ", quantity: " << order.getQuantityValue() 
       << ", material availability: " << order.getAvailabilityProbablity() << "%"
       << ", arrival time: " << order.getArrivalTime();
  return sout;
}

// overloaded insertion operator for Node class
ostream& operator<<(ostream& sout, const Node& node) {
  sout << node.getOrder();
  return sout;
}

// for debugging
void MQueue::dump() const
{
  if (m_size == 0) {
    cout << "Empty skew heap.\n" ;
  } else {
    dump(m_heap);
    cout << endl;
  }
}

// for debugging
void MQueue::dump(Node *pos) const {
  if ( pos != nullptr ) {
    cout << "(";
    dump(pos->m_left);
    cout << m_priorFunc(pos->m_order) << ":" << pos->m_order.getCustomer();
    dump(pos->m_right);
    cout << ")";
  }
}
