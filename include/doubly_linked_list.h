
#pragma once

#include <cstddef>
#include <stdexcept>
#include <utility>
#include <vector>

template <typename T>
struct DNode {
    T value;
    DNode* prev;
    DNode* next;

    explicit DNode(const T& v,
                   DNode* p = nullptr,
                   DNode* n = nullptr)
        : value(v), prev(p), next(n) {}
};

template <typename T>
class DLinkedList {
public:
    DLinkedList();
    ~DLinkedList();

    DLinkedList(const DLinkedList& other);
    DLinkedList& operator=(const DLinkedList& other);

    void push_front(const T& value);
    void push_back(const T& value);

    bool pop_front();
    bool pop_back();

    T& front();
    const T& front() const;

    T& back();
    const T& back() const;

    std::size_t size() const noexcept;
    bool empty() const noexcept;

    bool contains(const T& value) const;
    std::vector<T> to_vector() const;

    void clear();

private:
    DNode<T>* header_;
    DNode<T>* trailer_;
    std::size_t size_;

    void insert_between(const T& value,
                        DNode<T>* previous,
                        DNode<T>* next);

    void remove_node(DNode<T>* node);

    void swap(DLinkedList& other) noexcept;
};

// Constructor
template <typename T>
DLinkedList<T>::DLinkedList()
    : header_(nullptr),
      trailer_(nullptr),
      size_(0) {

    // Sentinel nodes do not contain list data.
    // This implementation uses a default T value.
    header_ = new DNode<T>(T{});

    try {
        trailer_ = new DNode<T>(T{});
    } catch (...) {
        delete header_;
        throw;
    }

    header_->next = trailer_;
    trailer_->prev = header_;
}

// Destructor
template <typename T>
DLinkedList<T>::~DLinkedList() {
    clear();

    delete header_;
    delete trailer_;
}

// Copy Constructor
template <typename T>
DLinkedList<T>::DLinkedList(
    const DLinkedList& other)
    : DLinkedList() {

    try {
        for (DNode<T>* current = other.header_->next;
             current != other.trailer_;
             current = current->next) {

            push_back(current->value);
        }
    } catch (...) {
        clear();
        delete header_;
        delete trailer_;
        throw;
    }
}

// Assignment Operator
template <typename T>
DLinkedList<T>& DLinkedList<T>::operator=(
    const DLinkedList& other) {

    if (this != &other) {
        DLinkedList<T> temp(other);
        swap(temp);
    }

    return *this;
}

// Swap List Contents
template <typename T>
void DLinkedList<T>::swap(
    DLinkedList& other) noexcept {

    std::swap(header_, other.header_);
    std::swap(trailer_, other.trailer_);
    std::swap(size_, other.size_);
}

// Insert Between Two Nodes
template <typename T>
void DLinkedList<T>::insert_between(
    const T& value,
    DNode<T>* previous,
    DNode<T>* next) {

    DNode<T>* newNode =
        new DNode<T>(value, previous, next);

    previous->next = newNode;
    next->prev = newNode;

    ++size_;
}

// Insert at Front
template <typename T>
void DLinkedList<T>::push_front(const T& value) {
    insert_between(value, header_, header_->next);
}

// Insert at Back
template <typename T>
void DLinkedList<T>::push_back(const T& value) {
    insert_between(value, trailer_->prev, trailer_);
}

// Remove a Node
template <typename T>
void DLinkedList<T>::remove_node(DNode<T>* node) {
    DNode<T>* previous = node->prev;
    DNode<T>* next = node->next;

    previous->next = next;
    next->prev = previous;

    delete node;
    --size_;
}

// Remove from Front
template <typename T>
bool DLinkedList<T>::pop_front() {
    if (empty()) {
        return false;
    }

    remove_node(header_->next);
    return true;
}

// Remove from Back
template <typename T>
bool DLinkedList<T>::pop_back() {
    if (empty()) {
        return false;
    }

    remove_node(trailer_->prev);
    return true;
}

// Get Front Element
template <typename T>
T& DLinkedList<T>::front() {
    if (empty()) {
        throw std::out_of_range("DLinkedList is empty");
    }

    return header_->next->value;
}

// Get Front Element (Const)
template <typename T>
const T& DLinkedList<T>::front() const {
    if (empty()) {
        throw std::out_of_range("DLinkedList is empty");
    }

    return header_->next->value;
}

// Get Back Element
template <typename T>
T& DLinkedList<T>::back() {
    if (empty()) {
        throw std::out_of_range("DLinkedList is empty");
    }

    return trailer_->prev->value;
}

// Get Back Element (Const)
template <typename T>
const T& DLinkedList<T>::back() const {
    if (empty()) {
        throw std::out_of_range("DLinkedList is empty");
    }

    return trailer_->prev->value;
}

// Get Size
template <typename T>
std::size_t DLinkedList<T>::size() const noexcept {
    return size_;
}

// Check if Empty
template <typename T>
bool DLinkedList<T>::empty() const noexcept {
    return size_ == 0;
}

// Search for Value
template <typename T>
bool DLinkedList<T>::contains(
    const T& value) const {

    for (DNode<T>* current = header_->next;
         current != trailer_;
         current = current->next) {

        if (current->value == value) {
            return true;
        }
    }

    return false;
}

// Convert List to Vector
template <typename T>
std::vector<T> DLinkedList<T>::to_vector() const {
    std::vector<T> values;
    values.reserve(size_);

    for (DNode<T>* current = header_->next;
         current != trailer_;
         current = current->next) {

        values.push_back(current->value);
    }

    return values;
}

// Clear Entire List
template <typename T>
void DLinkedList<T>::clear() {
    DNode<T>* current = header_->next;

    while (current != trailer_) {
        DNode<T>* next = current->next;
        delete current;
        current = next;
    }

    header_->next = trailer_;
    trailer_->prev = header_;
    size_ = 0;
}
