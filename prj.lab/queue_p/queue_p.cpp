#include <queue_p/queue_p.h>
#include <memory>
#include <stdexcept>
#include <iostream>

QueueP::QueueP(const QueueP &other) : head_(nullptr), size_(other.size_) {
    if (other.isEmpty())
        return;
    head_ = std::make_unique<Node>(other.top());
    Node *node = head_.get();
    Node *node_other = other.head_.get();
    while (node_other->next != nullptr) {
        node_other = node_other->next.get();
        node->next = std::make_unique<Node>(node_other->value);
        node = node->next.get();
    }
}

QueueP::QueueP(QueueP &&other) noexcept
    : head_(std::exchange(other.head_, nullptr)),
    size_(std::exchange(other.size_, 0)) {
}

QueueP &QueueP::operator=(const QueueP &rhs) {
    QueueP copy(rhs);
    head_ = std::exchange(copy.head_, nullptr);
    size_ = std::exchange(copy.size_, 0);
    return *this;
}

QueueP &QueueP::operator=(QueueP &&rhs) noexcept {
    head_ = std::exchange(rhs.head_, nullptr);
    size_ = std::exchange(rhs.size_, 0);
    return *this;
}

bool QueueP::isEmpty() const noexcept {
    return size_ == 0;
}

void QueueP::push(int value) {
    auto new_node = std::make_unique<Node>(value);

    if (isEmpty() || head_->value > value) {
        new_node->next = std::move(head_);
        head_ = std::move(new_node);
        ++size_;
        return;
    }

    Node *node = head_.get();
    while (node->next != nullptr && node->next->value <= value) {
        node = node->next.get();
    }
    new_node->next = std::move(node->next);
    node->next = std::move(new_node);
    ++size_;
}

void QueueP::pop() noexcept {
    if (isEmpty())
        return;
    head_ = std::move(head_->next);
    --size_;
}

int QueueP::top() const {
    if (isEmpty())
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

std::ostream &QueueP::writeTo(std::ostream &ostrm) const {
    ostrm << left_bracket_ << " ";
    Node *node = head_.get();
    while (node != nullptr) {
        ostrm << node->value << " ";
        node = node->next.get();
    }
    ostrm << right_bracket_ << std::endl;
    return ostrm;
}

QueueP::Node::Node(int val) : value(val), next(nullptr) {}

std::ostream &operator<<(std::ostream &ostrm, const QueueP &data) {
    return data.writeTo(ostrm);
}