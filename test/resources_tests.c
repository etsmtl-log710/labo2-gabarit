#include <signal.h>

#include <check.h>

#include "resources.h"

// NOLINTBEGIN(readability-magic-numbers)

START_TEST(test_resources_check_with_enough_resources_returns_true)
{
    static struct {
        resources_t available;
        resources_t needed;
    } data[] = {
        { { 2, 1, 1, 2 }, { 0, 0, 0, 0 } },
        { { 2, 1, 1, 2 }, { 1, 0, 0, 0 } },
        { { 2, 1, 1, 2 }, { 0, 1, 1, 0 } },
        { { 2, 1, 1, 2 }, { 2, 0, 1, 2 } },
        { { 2, 1, 1, 2 }, { 2, 1, 1, 2 } },
    };

    resources_t* available = &data[_i].available;
    resources_t* needed = &data[_i].needed;

    ck_assert(resources_check(available, needed));
}
END_TEST

START_TEST(test_resources_check_without_enough_resources_returns_false)
{
    static struct {
        resources_t available;
        resources_t needed;
    } data[] = {
        { { 2, 1, 1, 2 }, { 3, 0, 0, 0 } },
        { { 2, 1, 1, 2 }, { 0, 4, 0, 0 } },
        { { 2, 1, 1, 2 }, { 0, 0, 5, 0 } },
        { { 2, 1, 1, 2 }, { 0, 0, 0, 3 } },
        { { 2, 1, 1, 2 }, { 1, 1, 2, 2 } },
        { { 2, 1, 1, 2 }, { 3, 2, 2, 5 } },
    };

    resources_t* available = &data[_i].available;
    resources_t* needed = &data[_i].needed;

    ck_assert(!resources_check(available, needed));
}
END_TEST

START_TEST(test_resources_lock_with_enough_resources_subtracts_the_needed_resources_from_the_available_resources)
{
    static struct {
        resources_t available;
        resources_t needed;
        resources_t resulting;
    } data[] = {
        { { 2, 1, 1, 2 }, { 0, 0, 0, 0 }, { 2, 1, 1, 2 } },
        { { 2, 1, 1, 2 }, { 1, 0, 0, 0 }, { 1, 1, 1, 2 } },
        { { 2, 1, 1, 2 }, { 0, 1, 1, 0 }, { 2, 0, 0, 2 } },
        { { 2, 1, 1, 2 }, { 2, 0, 1, 2 }, { 0, 1, 0, 0 } },
        { { 2, 1, 1, 2 }, { 2, 1, 1, 2 }, { 0, 0, 0, 0 } }
    };

    resources_t available = data[_i].available;
    resources_t* needed = &data[_i].needed;
    resources_t* resulting = &data[_i].resulting;

    resources_lock(&available, needed);

    ck_assert_uint_eq(resulting->printers, available.printers);
    ck_assert_uint_eq(resulting->scanners, available.scanners);
    ck_assert_uint_eq(resulting->modems, available.modems);
    ck_assert_uint_eq(resulting->drives, available.drives);
}
END_TEST

START_TEST(test_resources_unlock_adds_the_locked_resources_to_the_available_resources)
{
    static struct {
        resources_t available;
        resources_t locked;
        resources_t resulting;
    } data[] = {
        { { 0, 0, 0, 0 }, { 2, 1, 1, 2 }, { 2, 1, 1, 2 } },
        { { 1, 0, 0, 0 }, { 1, 1, 1, 2 }, { 2, 1, 1, 2 } },
        { { 0, 1, 1, 0 }, { 2, 0, 0, 2 }, { 2, 1, 1, 2 } },
        { { 2, 0, 1, 2 }, { 0, 1, 0, 0 }, { 2, 1, 1, 2 } },
        { { 2, 1, 1, 2 }, { 0, 0, 0, 0 }, { 2, 1, 1, 2 } }
    };

    resources_t available = data[_i].available;
    resources_t* locked = &data[_i].locked;
    resources_t* resulting = &data[_i].resulting;

    resources_unlock(&available, locked);

    ck_assert_uint_eq(resulting->printers, available.printers);
    ck_assert_uint_eq(resulting->scanners, available.scanners);
    ck_assert_uint_eq(resulting->modems, available.modems);
    ck_assert_uint_eq(resulting->drives, available.drives);
}
END_TEST

Suite* create_resources_suite()
{
    Suite* suite = suite_create("resources");

    TCase* resources_check_tcase = tcase_create("resources_check");
    suite_add_tcase(suite, resources_check_tcase);

    tcase_add_loop_test(resources_check_tcase, test_resources_check_with_enough_resources_returns_true, 0, 5);
    tcase_add_loop_test(resources_check_tcase, test_resources_check_without_enough_resources_returns_false, 0, 6);

    TCase* resources_lock_tcase = tcase_create("resources_lock");
    suite_add_tcase(suite, resources_lock_tcase);

    tcase_add_loop_test(resources_lock_tcase, test_resources_lock_with_enough_resources_subtracts_the_needed_resources_from_the_available_resources, 0, 5);

    TCase* resources_unlock_tcase = tcase_create("resources_unlock");
    suite_add_tcase(suite, resources_unlock_tcase);

    tcase_add_loop_test(resources_unlock_tcase, test_resources_unlock_adds_the_locked_resources_to_the_available_resources, 0, 5);

    return suite;
}

// NOLINTEND(readability-magic-numbers)
