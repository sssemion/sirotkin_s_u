#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN

#include <queue_p.h>
#include <doctest.h>
#include <sstream>

TEST_SUITE("Priority queue QueueP testing") {
    TEST_CASE("Initialization, push, write") {
        std::ostringstream output("");
        QueueP queue;
        queue.push(2);
        CHECK(queue.size() == 1);
        output << queue;
        CHECK(output.str() == "{ 2 }\n");
        queue.push(3);
        CHECK(queue.size() == 2);
        output << queue;
        CHECK(output.str() == "{ 2 }\n{ 2 3 }\n");
        queue.push(1);
        CHECK(queue.size() == 3);
        output << queue;
        CHECK(output.str() == "{ 2 }\n{ 2 3 }\n{ 1 2 3 }\n");
    }

    TEST_CASE("Empty") {
        QueueP queue;
        CHECK(queue.isEmpty());
        queue.push(0);
        CHECK_FALSE(queue.isEmpty());
    }

    TEST_CASE("Pop") {
        QueueP queue;
        queue.push(4);
        queue.push(2);
        queue.push(5);
        queue.push(1);
        CHECK(queue.size() == 4);
        queue.pop();
        CHECK(queue.size() == 3);
        queue.pop();
        CHECK(queue.size() == 2);
        queue.pop();
        queue.pop();
        CHECK(queue.size() == 0);
        queue.pop();
        CHECK(queue.size() == 0);
    }

    TEST_CASE("Front") {
        QueueP queue;
        queue.push(3);
        CHECK(queue.top() == 3);
        queue.push(4);
        CHECK(queue.top() == 3);
        queue.push(1);
        CHECK(queue.top() == 1);
        queue.pop();
        CHECK(queue.top() == 3);
        queue.pop();
        CHECK(queue.top() == 4);
        queue.pop();
        CHECK_THROWS_WITH(queue.top(), "Empty queue");
    }

    TEST_CASE("Copying") {
        QueueP q1;
        q1.push(5);
        q1.push(3);
        q1.push(4);
        q1.push(1);
        q1.push(2);
        QueueP q2(q1);
        CHECK(q2.top() == 1);
        q2.pop();
        CHECK(q1.top() == 1);
        CHECK(q2.top() == 2);

        q2 = q1;
        CHECK(q2.top() == 1);
        q2.pop();
        CHECK(q1.top() == 1);
        CHECK(q2.top() == 2);
    }

    TEST_CASE("Moving") {
        QueueP q1;
        q1.push(5);
        q1.push(3);
        q1.push(4);
        q1.push(1);
        q1.push(2);
        QueueP q2(std::move(q1));
        CHECK(q1.isEmpty());
        CHECK(q2.top() == 1);
        q2.pop();
        CHECK(q2.top() == 2);

        q1 = std::move(q2);
        CHECK(q2.isEmpty());
        CHECK(q1.top() == 2);
        q1.pop();
        CHECK(q1.top() == 3);
    }
}