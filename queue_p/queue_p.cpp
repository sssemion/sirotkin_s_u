#include "queue_p.h"
#include <memory>
#include <stdexcept>
#include <iostream>

QueueP::QueueP(const QueueP &other) {
    if (other.empty())
        return;
    head_ = std::make_unique<Node>(other.front());
    Node* node = head_.get();
    Node* node_other = other.head_->next.get();
    while (node_other->next != nullptr) {
        node->next = std::make_unique<Node>(node_other->value);
        node = node->next.get();
    }
}

QueueP& QueueP::operator=(const QueueP &rhs) {
    QueueP copy(rhs);
    std::swap(*this, copy);
    return *this;
}

bool QueueP::empty() const {
    return size_ == 0;
}

void QueueP::push(int value) {
    auto new_node = std::make_unique<Node>(value);

    if (empty() || head_->value > value) {
        head_->next = std::move(head_);
        head_ = std::move(new_node);
        ++size_;
        return;
    }

    Node* node = head_.get();
    while (node->next != nullptr && node->next->value <= value) {
        node = node->next.get();
    }
    new_node->next = std::move(node->next);
    node->next = std::move(new_node);
    ++size_;
}

void QueueP::pop() {
    if (empty())
        return;
    head_ = std::move(head_->next);
}

int QueueP::front() const {
    if (empty())
        throw std::exception("Empty queue");
    return head_->value;
}

size_t QueueP::size() const {
    return size_;
}

QueueP::~QueueP() {
    while (head_ != nullptr)
        head_ = std::move(head_->next);
}

std::ostream& QueueP::writeTo(std::ostream &ostrm) const {
    ostrm << left_bracket_ << " ";
    Node* node = head_.get();
    while (node != nullptr) {
        ostrm << node->value << " ";
        node = node->next.get();
    }
    ostrm << right_bracket_ << std::endl;
    return ostrm;
}

QueueP::Node::Node(int val) : value(val) {}

std::ostream& operator<<(std::ostream &ostrm, const QueueP &data) {
    return data.writeTo(ostrm);
}