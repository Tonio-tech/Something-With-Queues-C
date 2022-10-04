// CMSC 341 - Fall 2021 - Project 3
// driver.cpp: sample driver for the MQueue class.

#include "mqueue.h"
using namespace std;
// Priority functions compute an integer priority for an order.  Internal
// computations may be floating point, but must return an integer.

int priorityFn1(const Order &order);
int priorityFn2(const Order &order);

class Tester {
  public:
    void basicCorrect();
    void testCopyConstructor();
    void testPriorFunc();
    void testRunTime();
    void testdomainError();
};


int main() {
  try{
    MQueue queue1(priorityFn1);
    MQueue queue2(priorityFn1);

    // Create some test orders and insert them into the queue
    //Order(string customer, int priority, int material, int quantity, int arrival)
    Order order1("Ford",1,1,1,20);
    Order order2("Honda",5,1,2,21);
    Order order3("Toyota",2,0,0,22);
    Order order4("GM",4,3,2,23);

    queue1.insertOrder(order3);
    queue1.insertOrder(order1);
    queue1.insertOrder(order2);
    queue1.insertOrder(order4);

    std::cout << "\nqueue1 dump after inserting 4 orders (Toyota, Honda, Ford, GM):" << std::endl;
    queue1.dump();
    

    Order order5("Ferrari",6,3,2,1);
    Order order6("Hyundai",3,0,0,10);

    queue2.insertOrder(order5);
    queue2.insertOrder(order6);

    std::cout << "\nqueue2 dump after inserting 2 students (Ferrari, Hyundai):" << std::endl;
    queue2.dump();

    queue1.mergeWithQueue(queue2);

    // Print the queue, change priority function, print again, change
    // again, print again.  Changing the priority function should cause
    // the heap to be rebuilt.

    std::cout << std::endl << "Contents of queue1 after merging with queue2:\n";
    queue1.printOrderQueue();
    std::cout << "\nDump queue1 after merging with queue2:\n";
    queue1.dump();

    queue1.setPriorityFn(priorityFn2);

    std::cout << std::endl << "Contents of queue1 after changing the priority function to priorityFn2:\n";
    queue1.printOrderQueue();
    std::cout << "\nDump queue1 after changing the priority function to priorityFn2:\n";
    queue1.dump();
    
    queue1.setPriorityFn(priorityFn1);

    std::cout << std::endl << "Contents of queue1 after changing the priority function back to priorityFn1:\n";
    queue1.printOrderQueue();
    std::cout << "\nDump queue1 after changing the priority function back to priorityFn1:\n";
    queue1.dump();

    // Remove and print orders in priority order

    std::cout << "\nFinal queue1 in priority order:\n";
    while (queue1.numOrders() > 0) {
      Order order = queue1.getNextOrder();
      // this will be printed in the ascending order (sorted)
      std::cout << "[" << priorityFn1(order) << "] " << order << std::endl;
    }

    cout << endl << endl << ":::::BEGINNING TESTS:::::" << endl << endl;

    Tester tester1;
    tester1.basicCorrect();
    tester1.testCopyConstructor();
    tester1.testPriorFunc();
    tester1.testRunTime();
    tester1.testdomainError();
  }
    catch(std::out_of_range &e){
      cout << e.what() << endl;
  }

  return 0;
}

int priorityFn1(const Order & order) {
  //priority value is determined based on some criteria
  //priority value falls in the range [1-58]
  //the smaller value means the higher priority
  int priority = order.getMaterialAvailability() + order.getQuantity() + order.getArrivalTime();
  return priority;
}

int priorityFn2(const Order & order) {
  //priority is determined by a production manager
  //priority value falls in the range [1-58]
  //the smaller value means the higher priority
  return order.getPMPriority();
}


// Basic correctness. Creating a queue, inserting orders, and reading them out in priority order functions correctly.
void Tester::basicCorrect() {
  cout << endl << endl << ":::TESTING BASIC CORRECTNESS:::" << endl << endl;
  MQueue queue1(priorityFn1);

  Order order1("Tesla",1,1,1,20);
  Order order2("Prius",5,1,2,21);
  Order order3("Volkswagon",2,0,0,22);
  Order order4("BMW",4,3,2,23);

  queue1.insertOrder(order3);
  queue1.insertOrder(order1);
  queue1.insertOrder(order2);
  queue1.insertOrder(order4);

  queue1.printOrderQueue();

  cout << endl << ":::END OF BASIC CORRECTNESS TEST:::" << endl << endl;
  //queue1.clear();
  //queue1.printOrderQueue();
}


// Correctness of copy constructor, assignment operator for normal and edge cases.
void Tester::testCopyConstructor() {
  cout << endl << endl << ":::TESTING COPY CONSTRUCTOR:::" << endl << endl;
  MQueue queue2(priorityFn1);

  Order order1("Nissan",1,2,3,20);
  Order order2("Mustang",4,2,2,21);
  Order order3("Jeep",2,2,3,22);
  Order order4("Subaru",3,3,2,23);

  queue2.insertOrder(order3);
  queue2.insertOrder(order1);
  queue2.insertOrder(order2);
  queue2.insertOrder(order4);

  cout << "Order Queue of queue2: " << endl;
  queue2.printOrderQueue();
  cout << endl;

  MQueue queue3(queue2);
  //should print the same info
  cout << "Order Queue of queue3 after copying queue2: " << endl;
  queue3.printOrderQueue();

  cout << endl << ":::END OF COPY CONSTRUCTOR TEST:::" << endl << endl;
  //queue3.clear();

}

// Can change the prioritization function using the setter method and the min-heap is rebuilt correctly. The following algorithm is an example for such a test:
    // Build a heap with a priority function and some data points; data points should have proper information so their priority change once the priority function is changed,
    // Change the priority function; it must rebuild the heap with the same data points,
    // Check whether the rebuilt heap satisfies the priority queue property.
void Tester::testPriorFunc() {
  cout << endl << ":::TESTING PRIORITY FUNCTION:::" << endl << endl;
  MQueue queue4(priorityFn2);

  Order order1("Fiat",6,1,4,21);
  Order order2("Cadallac",5,4,2,34);
  Order order3("Porsche",1,0,0,20);
  Order order4("Audi",1,3,4,49);

  queue4.insertOrder(order3);
  queue4.insertOrder(order1);
  queue4.insertOrder(order2);
  queue4.insertOrder(order4);

  cout << "Order Queue of queue4 before changing priority:" << endl;
  queue4.printOrderQueue();
  cout << endl;

  queue4.setPriorityFn(priorityFn1);

  cout << "Order Queue of queue4 after changing priority:" << endl;
  queue4.printOrderQueue();
  //queue4.clear();
  cout << endl << ":::END TEST OF PRIORITY FUNCTION:::" << endl << endl;
}

// Efficiency with large queues. Insertion and extraction with a large queue operates in logarithmic time, i.e. O(log n)
void Tester::testRunTime() {
  int counter = 0;
  float result1 = 0.0;
  float result2 = 0.0;
  float result3 = 0.0;
  float result4 = 0.0;
  float result5 = 0.0;
  clock_t start, stop;

  MQueue queue1(priorityFn1);
  MQueue queue2(priorityFn1);
  MQueue queue3(priorityFn1);
  MQueue queue4(priorityFn1);
  MQueue queue5(priorityFn1);
  Order order1("Toyota",1,2,3,24);


  while(counter < 100000) {
    start = clock();
    queue1.insertOrder(order1);
    counter++;
  }
  //queue1.clear();
  stop = clock();
  result1 = stop - start;//number of clock ticks the algorithm took
  cout << "Insertion of " << counter << " items took: " << result1 << " clock ticks ("<< result1/CLOCKS_PER_SEC << " seconds)!" << endl;


  counter = 0;

  while(counter < 200000) {
    start = clock();
    queue5.insertOrder(order1);
    counter++;
  }
  //queue5.clear();
  stop = clock();
  result2 = stop - start;//number of clock ticks the algorithm took
  cout << "Insertion of " << counter << " items took: " << result2 << " clock ticks ("<< result2/CLOCKS_PER_SEC << " seconds)!" << endl;


  counter = 0;

  while(counter < 400000) {
    start = clock();
    queue2.insertOrder(order1);
    counter++;
  }
  //queue2.clear();
  stop = clock();
  result3 = stop - start;//number of clock ticks the algorithm took
  cout << "Insertion of " << counter << " items took: " << result3 << " clock ticks ("<< result3/CLOCKS_PER_SEC << " seconds)!" << endl;


  counter = 0;

  while(counter < 800000) {
    start = clock();
    queue3.insertOrder(order1);
    counter++;
  }
  //queue3.clear();
  stop = clock();
  result4 = stop - start;//number of clock ticks the algorithm took
  cout << "Insertion of " << counter << " items took: " << result4 << " clock ticks ("<< result4/CLOCKS_PER_SEC << " seconds)!" << endl;


  counter = 0;

  while(counter < 1000000) {
    start = clock();
    queue4.insertOrder(order1);
    counter++; 
  } 
  //queue4.clear();
  stop = clock();
  result5 = stop - start;//number of clock ticks the algorithm took
  cout << "Insertion of " << counter << " items took: " << result5 << " clock ticks ("<< result5/CLOCKS_PER_SEC << " seconds)!" << endl;

}



// A domain_error exception is thrown If the user attempts to merge queues with different priority functions.
// Attempting to dequeue an empty queue throws a domain_error exception.
void Tester::testdomainError() {
  cout << endl << ":::TESTING DOMAIN ERRORS:::" << endl << endl;
  MQueue queue1(priorityFn1);

  Order order1("Toyota",2,2,4,34);
  Order order2("Honda",1,1,1,10);
  Order order3("Golf Cart",4,0,0,15);
  Order order4("Toy Car",3,4,4,45);

  queue1.insertOrder(order3);
  queue1.insertOrder(order1);
  queue1.insertOrder(order2);
  queue1.insertOrder(order4);

  MQueue queue2(priorityFn2);
  //merging with diff priority functions
  queue1.mergeWithQueue(queue2);
  
  //deleting an empty queue
  MQueue queue3(priorityFn1);
  queue3.clear();

  cout << endl << ":::END TEST OF DOMAIN ERRORS:::" << endl << endl;
}