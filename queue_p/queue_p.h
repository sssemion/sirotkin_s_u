#ifndef LABS_3SEM_QUEUE_P_H
#define LABS_3SEM_QUEUE_P_H

#include <memory>
#include <iosfwd>

class QueueP {
public:
    QueueP() = default;
    QueueP(const QueueP &other);
    QueueP& operator=(QueueP &rhs);
    ~QueueP();

    bool empty() const;
    void push(int value);
    void pop();
    int back() const;
    size_t size() const;

    std::ostream& writeTo(std::ostream &ostrm) const;
    std::istream& readFrom(std::istream &istrm);

private:
    struct Node {
        std::unique_ptr<Node> next = nullptr;
        int value = 0;
    };

    Node head_{};
    size_t size_ = 0;
};

std::ostream& operator<<(std::ostream &ostrm, const QueueP &data);
std::istream& operator>>(std::istream &istrm, QueueP &data);

#endif //LABS_3SEM_QUEUE_P_H
