
#pragma once

#include <cstddef>
#include <stdexcept>
#include <utility>
#include <vector>

template <typename T>
struct SNode {
    T value;
    SNode* next;

    explicit SNode(const T& v, SNode* n = nullptr)
        : value(v), next(n) {}
};

template <typename T>
class SLinkedList {
public:
    SLinkedList();
    ~SLinkedList();
    SLinkedList(const SLinkedList& other);
    SLinkedList& operator=(const SLinkedList& other);

    void push_front(const T& value);
    void push_back(const T& value);
    bool pop_front();
    bool pop_back();

    T& front();
    const T& front() const;
    std::size_t size() const noexcept;
    bool empty() const noexcept;
    bool contains(const T& value) const;
    std::vector<T> to_vector() const;
    void clear();

private:
    SNode<T>* head_;
    std::size_t size_;
};

template <typename T>
SLinkedList<T>::SLinkedList()
    : head_(nullptr), size_(0) {}

// Destructor
template <typename T>
SLinkedList<T>::~SLinkedList() {
    clear();
}

// Copy Constructor
template <typename T>
SLinkedList<T>::SLinkedList(const SLinkedList& other)
    : head_(nullptr), size_(0) {
    try {
        for (SNode<T>* current = other.head_;
             current != nullptr;
             current = current->next) {
            push_back(current->value);
        }
    } catch (...) {
        clear();
        throw;
    }
}

// Assignment Operator
template <typename T>
SLinkedList<T>& SLinkedList<T>::operator=(
    const SLinkedList& other) {

    if (this != &other) {
        SLinkedList<T> temp(other);
        std::swap(head_, temp.head_);
        std::swap(size_, temp.size_);
    }

    return *this;
}

// Insert at Front
template <typename T>
void SLinkedList<T>::push_front(const T& value) {
    head_ = new SNode<T>(value, head_);
    ++size_;
}

// Insert at Back
template <typename T>
void SLinkedList<T>::push_back(const T& value) {
    SNode<T>* newNode = new SNode<T>(value);

    if (empty()) {
        head_ = newNode;
    } else {
        SNode<T>* current = head_;

        while (current->next != nullptr) {
            current = current->next;
        }

        current->next = newNode;
    }

    ++size_;
}

// Remove from Front
template <typename T>
bool SLinkedList<T>::pop_front() {
    if (empty()) {
        return false;
    }

    SNode<T>* temp = head_;
    head_ = head_->next;

    delete temp;
    --size_;

    return true;
}

// Remove from Back
template <typename T>
bool SLinkedList<T>::pop_back() {
    if (empty()) {
        return false;
    }

    if (head_->next == nullptr) {
        delete head_;
        head_ = nullptr;
    } else {
        SNode<T>* current = head_;

        while (current->next->next != nullptr) {
            current = current->next;
        }

        delete current->next;
        current->next = nullptr;
    }

    --size_;
    return true;
}

// Get Front Element
template <typename T>
T& SLinkedList<T>::front() {
    if (empty()) {
        throw std::out_of_range("SLinkedList is empty");
    }

    return head_->value;
}

// Get Front Element (Const)
template <typename T>
const T& SLinkedList<T>::front() const {
    if (empty()) {
        throw std::out_of_range("SLinkedList is empty");
    }

    return head_->value;
}

// Get Size
template <typename T>
std::size_t SLinkedList<T>::size() const noexcept {
    return size_;
}

// Check if Empty
template <typename T>
bool SLinkedList<T>::empty() const noexcept {
    return size_ == 0;
}

// Search for Value
template <typename T>
bool SLinkedList<T>::contains(const T& value) const {
    SNode<T>* current = head_;

    while (current != nullptr) {
        if (current->value == value) {
            return true;
        }

        current = current->next;
    }

    return false;
}

// Convert List to Vector
template <typename T>
std::vector<T> SLinkedList<T>::to_vector() const {
    std::vector<T> values;
    values.reserve(size_);

    for (SNode<T>* current = head_;
         current != nullptr;
         current = current->next) {
        values.push_back(current->value);
    }

    return values;
}

// Clear Entire List
template <typename T>
void SLinkedList<T>::clear() {
    while (head_ != nullptr) {
        SNode<T>* temp = head_;
        head_ = head_->next;
        delete temp;
    }

    size_ = 0;
}
