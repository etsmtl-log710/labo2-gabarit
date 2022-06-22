#include <stdlib.h>

#include <check.h>

extern Suite* create_resources_suite();
extern Suite* create_queue_suite();

int main(void)
{
    SRunner* runner = srunner_create(NULL);
    srunner_add_suite(runner, create_resources_suite());
    srunner_add_suite(runner, create_queue_suite());

    srunner_run_all(runner, CK_ENV);
    int tests_failed = srunner_ntests_failed(runner);
    srunner_free(runner);

    return tests_failed > 0 ? EXIT_FAILURE : EXIT_SUCCESS;
}
