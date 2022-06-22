#include <check.h>

#include "queue.h"

typedef struct {
    process_t** ptr;
    size_t len;
} processes_t;

typedef struct {
    processes_t processes;
    size_t i;
} assert_processes_context_t;

static void assert_process(process_t* process, void* userdata)
{
    assert_processes_context_t* ctx = userdata;

    ck_assert_uint_ge(ctx->processes.len, ctx->i);

    ck_assert_ptr_eq(ctx->processes.ptr[ctx->i], process);

    ctx->i++;
}

static void assert_processes(queue_t* queue, processes_t processes)
{
    assert_processes_context_t ctx = { processes, 0 };

    queue_each(queue, assert_process, &ctx);
}

// It is valid to have pointers to any address, as long as we do not try to
// read or write to said address.

// NOLINTBEGIN(readability-magic-numbers)

#define P1 ((process_t*)1)
#define P2 ((process_t*)2)
#define P3 ((process_t*)3)
#define P4 ((process_t*)4)
#define P5 ((process_t*)5)
#define P6 ((process_t*)6)
#define P7 ((process_t*)7)
#define P8 ((process_t*)8)

static queue_t queue;

static void setup()
{
    queue_init(&queue);
}

static void teardown()
{
    queue_deinit(&queue);
}

START_TEST(test_queue_init_creates_an_empty_queue)
{
    ck_assert(queue_is_empty(&queue));
}
END_TEST

START_TEST(test_queue_deinit_does_nothing_for_an_empty_queue)
{
}
END_TEST

START_TEST(test_queue_deinit_frees_all_allocated_nodes)
{
    // This test must be validated with a memory leak checker like valgrind.

    ck_assert_int_eq(OK, queue_enqueue(&queue, P3));
    ck_assert_int_eq(OK, queue_enqueue(&queue, P2));
    ck_assert_int_eq(OK, queue_enqueue(&queue, P5));
    ck_assert_int_eq(OK, queue_enqueue(&queue, P6));
}
END_TEST

START_TEST(test_queue_enqueue_given_an_empty_queue_makes_the_queue_no_longer_empty)
{
    ck_assert_int_eq(OK, queue_enqueue(&queue, P5));

    ck_assert(!queue_is_empty(&queue));
}
END_TEST

static processes_t test_queue_enqueue_appends_the_process_and_the_end_of_the_queue_data[] = {
    { (process_t*[]) { P5, P2, P6, P8, P1, P3 }, 6 },
    { (process_t*[]) { P8 }, 1 },
    { (process_t*[]) { P1, P2, P3 }, 3 }
};

START_TEST(test_queue_enqueue_appends_the_process_and_the_end_of_the_queue)
{
    processes_t processes = test_queue_enqueue_appends_the_process_and_the_end_of_the_queue_data[_i];

    for (size_t i = 0; i < processes.len; ++i) {
        ck_assert_int_eq(OK, queue_enqueue(&queue, processes.ptr[i]));
    }

    assert_processes(&queue, processes);
}
END_TEST

START_TEST(test_queue_dequeue_makes_the_queue_empty_if_the_last_process_was_dequeued)
{
    ck_assert_int_eq(OK, queue_enqueue(&queue, P4));
    ck_assert_ptr_eq(P4, queue_dequeue(&queue));

    ck_assert(queue_is_empty(&queue));
}
END_TEST

static struct {
    processes_t processes;
    size_t dequeue_count;
} test_queue_dequeue_removes_the_beginning_elements_from_the_queue_data[] = {
    { { (process_t*[]) { P5, P2, P6, P8, P1, P3 }, 6 }, 3 },
    { { (process_t*[]) { P8 }, 1 }, 1 },
    { { (process_t*[]) { P1, P2, P3 }, 3 }, 2 }
};

START_TEST(test_queue_dequeue_removes_the_beginning_elements_from_the_queue)
{
    processes_t processes = test_queue_dequeue_removes_the_beginning_elements_from_the_queue_data[_i].processes;
    size_t dequeue_count = test_queue_dequeue_removes_the_beginning_elements_from_the_queue_data[_i].dequeue_count;

    for (size_t i = 0; i < processes.len; ++i) {
        ck_assert_int_eq(OK, queue_enqueue(&queue, processes.ptr[i]));
    }

    for (size_t i = 0; i < dequeue_count; ++i) {
        ck_assert_ptr_eq(processes.ptr[i], queue_dequeue(&queue));
    }

    assert_processes(&queue, (processes_t) { processes.ptr + dequeue_count, processes.len - dequeue_count });
}
END_TEST

START_TEST(test_queue_dequeue_returns_null_if_the_queue_is_empty)
{
    ck_assert_ptr_null(queue_dequeue(&queue));
}
END_TEST

static struct {
    processes_t starting_processes;
    processes_t dequeued_processes;
    processes_t ending_processes;
} test_queue_dequeue_if_dequeues_the_process_according_to_the_predicate_data[] = {
    { { (process_t*[]) { P1, P2 }, 2 },
        { (process_t*[]) { P2 }, 1 },
        { (process_t*[]) { P1 }, 1 } },
    { { (process_t*[]) { P1, P2 }, 2 },
        { (process_t*[]) { P1 }, 1 },
        { (process_t*[]) { P2 }, 1 } },
    { { (process_t*[]) { P1, P2, P3, P4, P5, P6, P7 }, 7 },
        { (process_t*[]) { P3, P5, P4 }, 3 },
        { (process_t*[]) { P1, P2, P6, P7 }, 4 } }
};

static bool dequeue_if_exact_process(process_t* process, void* userdata)
{
    return process == ((process_t*)userdata);
}

START_TEST(test_queue_dequeue_if_dequeues_the_process_according_to_the_predicate)
{
    processes_t starting_processes = test_queue_dequeue_if_dequeues_the_process_according_to_the_predicate_data[_i].starting_processes;
    processes_t dequeued_processes = test_queue_dequeue_if_dequeues_the_process_according_to_the_predicate_data[_i].dequeued_processes;
    processes_t ending_processes = test_queue_dequeue_if_dequeues_the_process_according_to_the_predicate_data[_i].ending_processes;

    for (size_t i = 0; i < starting_processes.len; ++i) {
        ck_assert_int_eq(OK, queue_enqueue(&queue, starting_processes.ptr[i]));
    }

    for (size_t i = 0; i < dequeued_processes.len; ++i) {
        ck_assert_ptr_eq(dequeued_processes.ptr[i], queue_dequeue_if(&queue, dequeue_if_exact_process, dequeued_processes.ptr[i]));
    }

    assert_processes(&queue, ending_processes);
}
END_TEST

START_TEST(test_dequeue_if_on_the_last_element_makes_the_queue_empty)
{
    ck_assert_int_eq(OK, queue_enqueue(&queue, P4));
    ck_assert_ptr_eq(P4, queue_dequeue_if(&queue, dequeue_if_exact_process, P4));
    ck_assert(queue_is_empty(&queue));
}
END_TEST

START_TEST(test_dequeue_if_returns_null_if_no_process_matches_the_predicate)
{
    ck_assert_int_eq(OK, queue_enqueue(&queue, P1));
    ck_assert_int_eq(OK, queue_enqueue(&queue, P2));
    ck_assert_int_eq(OK, queue_enqueue(&queue, P3));

    ck_assert_ptr_null(queue_dequeue_if(&queue, dequeue_if_exact_process, P5));
}
END_TEST

Suite* create_queue_suite()
{
    Suite* suite = suite_create("queue");

    TCase* queue_init_tcase = tcase_create("queue_init");
    suite_add_tcase(suite, queue_init_tcase);
    tcase_add_checked_fixture(queue_init_tcase, setup, teardown);

    tcase_add_test(queue_init_tcase, test_queue_init_creates_an_empty_queue);

    TCase* queue_deinit_tcase = tcase_create("queue_deinit");
    suite_add_tcase(suite, queue_deinit_tcase);
    tcase_add_checked_fixture(queue_deinit_tcase, setup, teardown);

    tcase_add_test(queue_deinit_tcase, test_queue_deinit_does_nothing_for_an_empty_queue);
    tcase_add_test(queue_deinit_tcase, test_queue_deinit_frees_all_allocated_nodes);

    TCase* queue_enqueue_tcase = tcase_create("queue_enqueue");
    suite_add_tcase(suite, queue_enqueue_tcase);
    tcase_add_checked_fixture(queue_enqueue_tcase, setup, teardown);

    tcase_add_test(queue_enqueue_tcase, test_queue_enqueue_given_an_empty_queue_makes_the_queue_no_longer_empty);
    tcase_add_loop_test(queue_enqueue_tcase, test_queue_enqueue_appends_the_process_and_the_end_of_the_queue, 0, 3);

    TCase* queue_dequeue_tcase = tcase_create("queue_dequeue");
    suite_add_tcase(suite, queue_dequeue_tcase);
    tcase_add_checked_fixture(queue_dequeue_tcase, setup, teardown);

    tcase_add_test(queue_dequeue_tcase, test_queue_dequeue_makes_the_queue_empty_if_the_last_process_was_dequeued);
    tcase_add_loop_test(queue_dequeue_tcase, test_queue_dequeue_removes_the_beginning_elements_from_the_queue, 0, 3);
    tcase_add_test(queue_dequeue_tcase, test_queue_dequeue_returns_null_if_the_queue_is_empty);

    TCase* queue_dequeue_if_tcase = tcase_create("queue_dequeue_if");
    suite_add_tcase(suite, queue_dequeue_if_tcase);
    tcase_add_checked_fixture(queue_dequeue_if_tcase, setup, teardown);

    tcase_add_loop_test(queue_dequeue_if_tcase, test_queue_dequeue_if_dequeues_the_process_according_to_the_predicate, 0, 3);
    tcase_add_test(queue_dequeue_if_tcase, test_dequeue_if_on_the_last_element_makes_the_queue_empty);
    tcase_add_test(queue_dequeue_if_tcase, test_dequeue_if_returns_null_if_no_process_matches_the_predicate);

    return suite;
}

// NOLINTEND(readability-magic-numbers)
