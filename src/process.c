#include "process.h"

#include <assert.h>
#include <stddef.h>
#include <stdio.h>

static process_t* current_process = NULL;

void process_init_from_line(process_t* process, const char* line, size_t line_number)
{
    assert(process != NULL);
    assert(line != NULL);
    assert(line_number > 0);

    process->pid = line_number;
    process->status = NEW;
    process->executed_time = 0;
    process->waited_time = 0;

    int process_type;

    // NOLINTNEXTLINE(cert-err34-c)
    assert(sscanf(line, "%zu, %d, %zu, %zu, %zu, %zu, %zu",
               &process->arrival_time,
               &process_type,
               &process->execution_time,
               &process->resources.printers,
               &process->resources.scanners,
               &process->resources.modems,
               &process->resources.drives)
        == 7);

    process->type = process_type;
}

bool process_has_started(const process_t* process)
{
    assert(process != NULL);

    return process->executed_time > 0;
}

void process_schedule(process_t* process)
{
    assert(process != NULL);
    assert(process->status == NEW);

    process->status = READY;
}

void process_execute(process_t* process)
{
    assert(process != NULL);
    assert(process->status == READY);
    assert(current_process == NULL);

    process->status = RUNNING;
    current_process = process;
}

void process_terminate(process_t* process)
{
    assert(process != NULL);
    assert(process->status == RUNNING);
    assert(process == current_process);

    process->status = TERMINATED;
    current_process = NULL;
}

void process_suspend(process_t* process)
{
    assert(process != NULL);
    assert(process->status == RUNNING);
    assert(process == current_process);

    process->status = READY;
    current_process = NULL;
}

#define BLACK "\033[30m"
#define RED "\033[31m"
#define GREEN "\033[32m"
#define YELLOW "\033[33m"
#define BLUE "\033[34m"
#define MAGENTA "\033[35m"
#define CYAN "\033[36m"
#define WHITE "\033[37m"

#define ON_BLACK "\033[40m"
#define ON_RED "\033[41m"
#define ON_GREEN "\033[42m"
#define ON_YELLOW "\033[43m"
#define ON_BLUE "\033[44m"
#define ON_MAGENTA "\033[45m"
#define ON_CYAN "\033[46m"
#define ON_WHITE "\033[47m"

#define NORMAL "\033[0m"

#define NUM_COLOURS 32
static char* colours[NUM_COLOURS] = {
    BLACK ON_WHITE,
    CYAN ON_RED,
    GREEN ON_MAGENTA,
    BLUE ON_YELLOW,
    BLACK ON_CYAN,
    WHITE ON_RED,
    BLUE ON_GREEN,
    YELLOW ON_MAGENTA,
    BLACK ON_GREEN,
    YELLOW ON_RED,
    BLUE ON_CYAN,
    MAGENTA ON_WHITE,
    BLACK ON_YELLOW,
    GREEN ON_RED,
    BLUE ON_WHITE,
    CYAN ON_MAGENTA,
    WHITE ON_BLACK,
    RED ON_CYAN,
    MAGENTA ON_GREEN,
    YELLOW ON_BLUE,
    CYAN ON_BLACK,
    RED ON_WHITE,
    GREEN ON_BLUE,
    MAGENTA ON_YELLOW,
    GREEN ON_BLACK,
    RED ON_YELLOW,
    CYAN ON_BLUE,
    WHITE ON_MAGENTA,
    YELLOW ON_BLACK,
    RED ON_GREEN,
    WHITE ON_BLUE,
    MAGENTA ON_CYAN
};

void process_tick(process_t* process)
{
    assert(process != NULL);

    switch (process->status) {
    case READY:
        process->waited_time++;
        break;
    case RUNNING:
        process->executed_time++;

        char* colour = colours[process->pid % NUM_COLOURS];

        printf("%sP%zu:        tick %zu / %zu" BLACK NORMAL "\n", colour, process->pid, process->executed_time, process->execution_time);

        if (process->executed_time >= process->execution_time) {
            process_terminate(process);
        }

        break;
    case NEW:
    case TERMINATED:
        break;
    }
}
