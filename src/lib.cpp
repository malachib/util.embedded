#include "lib.h"

#ifdef AVR
#include <avr/pgmspace.h>
#endif

namespace Mal_Lib
{
Led::Led(uint8_t pinNumber, bool autoInitOutput) : DigitalPin(pinNumber)
{
    // I'd expect LEDs 99% of the time want to be in OUTPUT mode
	// TODO: notate... again not sure why i need to exclude PSoC here...
	// could be cuz of global init conflicts
#if !CYPRESS_PSOC && !WIN32
    if(autoInitOutput)
      SetMode(OUTPUT);
#endif
}

Led::Led() : Led(13, true)
{

}

void Profiler::Profile()
{
#ifdef AVR
  uint32_t currentTimestamp = micros();
#else
  uint32_t currentTimestamp = 0; // FIX: Obviously not yet functional
#endif
  uint16_t interval = currentTimestamp - lastTimestamp;

  lastTimestamp = currentTimestamp;
  if(iterator == 0xFFFF)
    overflowed = true;
  iterator++;

  if(interval > max)
  {
    max = interval;
    maxs.put(interval);
    if(spikeObserver != NULL && !spikeObserverLatch)
    {
      spikeObserver(*this);
      // spike observers tend to be time consuming, creating a subsequent spike
      // set a latch so that we don't catch the next spike and observe it too, creating
      // a "batch" of spike observers
      spikeObserverLatch = true;
      // reset this latch when we hit our frequency counter
    }
  }
  if(interval < min)
    min = interval;

  if(++frequencyCounter >= frequency)
  {
    spikeObserverLatch = false;
    buffer.put(interval);
    frequencyCounter = 0;
  }
}

uint16_t Profiler::getAverage()
{
  byte i;
  uint32_t average = 0;

  for(i = 0; i < profileCount; i++)
  {
    average += intervals[i];
  }

  return average /= profileCount;
}


/*
template <class T>
T CircularBuffer<T>::get()
{
  T value = peek();
  incrementPositionGet();
  return value;
}
*/

void ObservableArray::notify(IObservable* observable)
{
  IObserver** currentObserver = (IObserver**)observers;
  for(byte i = length; i-- > 0;)
  {
    (*currentObserver)->observe(observable);
    currentObserver++;
  }
}

// this presumes there's room to grow in the array! be careful!!
void ObservableArray::add(IObserver* observer)
{
  observers[length++] = observer;
}


void ObserverArray::observe(IObservable* subject)
{
  notify(subject);
}


void SinglyLinkedList::insertAtBeginning(SinglyLinkedNode* node)
{
  if(head != NULL)
  {
    node->next = head;
  }

  head = node;
}


// insert a node right after referenceNodePrev, and right before referenceNode
// remember since this is singly-linked list, we must explicitly know beforehand
// who the previous node is so that we can update prevNode->next
void SinglyLinkedNode::insertBetween(
  SinglyLinkedNode* referenceNodePrev,
  SinglyLinkedNode* referenceNode)
{
  // indicate our nodeToInsert sit just before referenceNode
  this->next = referenceNode;

  // now update the previous node, if available

  // if prevMode is NULL then we're at head and don't have
  // a previous node to update
  if(referenceNodePrev != NULL)
  // if prevNode exists, then we update it to no longer point to
  // node/referenceNode but rather our new nodeToInsert
  {
    referenceNodePrev->next = this;
  }
}

// remove *this* node clueing in that it appears right after referenceNodePrev
void SinglyLinkedNode::removeAfter(
  SinglyLinkedNode* referenceNodePrev)
{
  // TODO: assert that referenceNodePrev->next == this
  referenceNodePrev->next = this->next;
}

// removes the *next* node in the chain
void SinglyLinkedNode::removeNext()
{
  // TODO: assert that this->next is not null
  this->next = next->next;
}


// insert a node right after referenceNodePrev, and right before referenceNode
void SinglyLinkedList::insertBetween(
  SinglyLinkedNode* referenceNodePrev,
  SinglyLinkedNode* referenceNode,
  SinglyLinkedNode* nodeToInsert)
{
  nodeToInsert->insertBetween(referenceNodePrev, referenceNode);

  // if prevMode is NULL then we're at head and don't have
  // a previous node to update
  // (referenceNode == node == head)
  if(referenceNodePrev == NULL)
  {
    // if at head, be sure to update linked list head
    // TODO: make debug assertion to ensure referenceNode == head here
    head = nodeToInsert;
  }
}

// insert a node in front of referenceNode
void SinglyLinkedList::insert(
  SinglyLinkedNode* referenceNode,
  SinglyLinkedNode* nodeToInsert)
{
  // TODO: assert nodeToInsert has NULL for next value
  // TODO: assert referenceNode is not NULL, because for that we want 'add'
  SinglyLinkedNode* node = head;
  SinglyLinkedNode* referenceNodePrev = NULL;

  while(node != NULL)
  {
    // if we've found the referenceNode at our current node iterator
    if(node == referenceNode)
    {
      insertBetween(referenceNodePrev, referenceNode, nodeToInsert);
      return;
    }
    referenceNodePrev = node;
    node = node->next;
  }
}


// add a node to the end of the linked list, iterating forward
// if necessary
void SinglyLinkedList::add(SinglyLinkedNode* node)
{
  if(head == NULL)
  {
    head = node;
  }
  else
  {
    SinglyLinkedNode* tail = head;

    for(;
      tail->next != NULL;
      tail = tail->next)
    {

    }

    // TODO: assert that tail->next is NULL here
    tail->next = node;
  }

  #if DEBUG
    if(node->getNext() != NULL)
    {
      Serial << F("\r\nWarning: brand new node->next != NULL.  Forcing to NULL");
      node->next = NULL;
    }
  #endif
}


void SinglyLinkedList::remove(SinglyLinkedNode* nodeToRemove)
{
  if(nodeToRemove == head)
  {
    head = nodeToRemove->next;
  }
  else
  {
    SinglyLinkedNode* node = head;

    while(node != NULL)
    {
      // remove reference from immediately preceding node
      if(node->next == nodeToRemove)
      {
        node->removeNext();
        return;
      }

      node = node->next;
    }
  }

  // FIX: Need a better place to do this.  Ensures node is "cleaned out"
  nodeToRemove->next = NULL;
}

void SinglyLinkedListWithTail::add(SinglyLinkedNode* node)
{
  if(head == NULL)
  {
    head = node;
    tail = node;
  }
  else
  {
    // TODO: put debug assertions here to ensure tail->next was null
    // to start with
    tail->next = node;
    tail = node;
  }

#if DEBUG
  if(node->getNext() != NULL)
  {
    Serial << F("\r\nWarning: brand new node->next != NULL");
  }
#endif
}

void DoublyLinkedList::add(DoublyLinkedNode* node)
{
  if(head != NULL)
  {
    node->prev = getTail();
  }
  SinglyLinkedListWithTail::add(node);
}


// insert *this* in front of referenceNode
void DoublyLinkedNode::insert(DoublyLinkedNode* referenceNode)
{
  this->prev = referenceNode->getPrev();
  this->next = referenceNode;
  referenceNode->prev = this;
}

// insert nodeToInsert in front of referenceNode
// TODO: Make optimization #if for speed vs code
void DoublyLinkedList::insert(DoublyLinkedNode* referenceNode, DoublyLinkedNode* nodeToInsert)
{
#if OPTIMIZED_SPEED
  if(referenceNode != head)
    // is NULL when referenceNode == head, so only copy when it isn't
    nodeToInsert->prev = referenceNode->getPrev();
  else
    // we're replacing referenceNode who happens to be head node,
    // so reassign head node here
    head = nodeToInsert;

  nodeToInsert->next = referenceNode;
  referenceNode->prev = nodeToInsert;
#else
  if(referenceNode == head)
    head = nodeToInsert;

  nodeToInsert->insert(referenceNode);
#endif

}



}
