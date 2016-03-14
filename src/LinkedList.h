#pragma once

namespace FactUtilEmbedded
{

class DoublyLinkedList;
class SinglyLinkedList;
class SinglyLinkedListWithTail;

class SinglyLinkedNode
{
  friend SinglyLinkedList;
  friend SinglyLinkedListWithTail;
  friend DoublyLinkedList;

protected:
  SinglyLinkedNode* next = NULL;

public:
  SinglyLinkedNode* getNext() { return next; }
  void insertBetween(SinglyLinkedNode* referenceNodePrev, SinglyLinkedNode* referenceNode);
  void removeAfter(SinglyLinkedNode* referenceNode);
  void removeNext();
};

// beware: when using double-linked list , nodes assume next
// node is also a doubly-linked node
class DoublyLinkedNode : public SinglyLinkedNode
{
  friend DoublyLinkedList;

  DoublyLinkedNode* prev;
public:
  DoublyLinkedNode* getNext() { return (DoublyLinkedNode*) this->next; }
  DoublyLinkedNode* getPrev() { return prev; }

  void insert(DoublyLinkedNode* referenceNode);
  void remove();
};

template <class T>
class LinkedNode : public DoublyLinkedNode
{
public:
  T* getNext() { return next; }
  T* getPrev() { return prev; }
};


class SinglyLinkedList
{
protected:
  SinglyLinkedNode* head = NULL;

public:
  SinglyLinkedNode* getHead() { return head; }

  void insertAtBeginning(SinglyLinkedNode* node);
  void insertBetween(SinglyLinkedNode* referenceNodePrev, SinglyLinkedNode* referenceNode, SinglyLinkedNode* nodeToInsert);
  void insert(SinglyLinkedNode* referenceNode, SinglyLinkedNode* nodeToInsert);
  void add(SinglyLinkedNode* node);
  void remove(SinglyLinkedNode* node);

  uint16_t count()
  {
    uint16_t counter = 0;

    for(SinglyLinkedNode* node = head; node != NULL; node = node->next)
      counter++;

    return counter;
  }
};

class SinglyLinkedListWithTail : public SinglyLinkedList
{
protected:
  SinglyLinkedNode* tail;
public:
  void add(SinglyLinkedNode* node);
};

class DoublyLinkedList : public SinglyLinkedListWithTail
{
public:
  void add(DoublyLinkedNode* node);
  void insert(DoublyLinkedNode* referenceNode, DoublyLinkedNode* nodeToInsert);
  void remove(DoublyLinkedNode* node);

  DoublyLinkedNode* getHead() { return (DoublyLinkedNode*) head; }
  DoublyLinkedNode* getTail() { return (DoublyLinkedNode*) tail; }
};

template <class T>
class LinkedList : public DoublyLinkedList
{
  struct Special : T
  {
    Special* getNext() { return (Special*) DoublyLinkedNode::getNext(); }
    Special* getPrev() { return (Special*) DoublyLinkedNode::getPrev(); }
  };

public:
  Special* getHead() { return (Special*) DoublyLinkedList::getHead(); }
  Special* getTail() { return (Special*) DoublyLinkedList::getTail(); }
};
}
