#ifndef LABS_3SEM_QUEUE_P_H
#define LABS_3SEM_QUEUE_P_H

#include <memory>
#include <iosfwd>

class QueueP {
public:
    QueueP() = default;
    QueueP(const QueueP &other);
    QueueP(QueueP &&other) noexcept ;
    QueueP& operator=(const QueueP &rhs);
    QueueP& operator=(QueueP &&rhs) noexcept;
    ~QueueP();

    bool isEmpty() const noexcept;
    void push(int value);
    void pop() noexcept;
    int top() const;
    size_t size() const;

    std::ostream& writeTo(std::ostream &ostrm) const;
    // std::istream& readFrom(std::istream &istrm);

private:
    struct Node {
        explicit Node(int val);
        std::unique_ptr<Node> next = nullptr;
        int value = 0;
    };

    std::unique_ptr<Node> head_ = nullptr;
    size_t size_ = 0;

    static const char left_bracket_ = '{', right_bracket_ = '}';
};

std::ostream& operator<<(std::ostream &ostrm, const QueueP &data);
// std::istream& operator>>(std::istream &istrm, QueueP &data);

#endif //LABS_3SEM_QUEUE_P_H
