#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <getopt.h>
#include <unistd.h>

#include "error.h"
#include "process.h"
#include "scheduler.h"

enum {
    DEFAULT_PROCESSES_CAPACITY = 32,
    DEFAULT_QUANTUM = 1000,
    MICROSECONDS_PER_MILLISECONDS = 1000,
};

typedef struct options {
    const char* file;
    scheduler_type_t scheduler_type;
    size_t quantum;
    bool trace;
} options_t;

typedef struct processes {
    process_t* ptr;
    size_t len;
} processes_t;

#define FOR_EACH_PROCESS(process, processes) \
    for (process_t * (process) = (processes).ptr; (process) < (processes).ptr + (processes).len; ++(process))

static bool all_terminated(processes_t processes);

static error_t run_simulation(const options_t* options, processes_t processes)
{
    UNUSED(options);
    UNUSED(processes);

    // TODO: Créer l'instance de votre ordonnanceur.
    WARN("forgot to create scheduler");

    for (size_t current_tick = 0; !all_terminated(processes); ++current_tick) {
        FOR_EACH_PROCESS(process, processes)
        {
            if (process->arrival_time == current_tick) {
                // TODO: Le processus `process` vient d'arriver, il vous faut
                // l'envoyer à votre ordonnanceur.
                WARN("forgot to send arrived process %zu to scheduler at tick %zu", process->pid, current_tick);
            }
        }

        // TODO: Il faut avancer l'état de votre ordonnanceur.
        WARN("forgot to tick scheduler at tick %zu", current_tick);

        if (options->trace) {
            // TODO: Si vous le désirez, imprimez plus d'information sur l'état
            // des ordonnanceurs.
            WARN("forgot to trace scheduler at tick %zu", current_tick);
        }

        FOR_EACH_PROCESS(process, processes)
        {
            process_tick(process);
        }

        if (options->quantum > 0) {
            usleep(options->quantum * MICROSECONDS_PER_MILLISECONDS);
        }
    }

    // TODO: Déinitialisez l'instance de votre ordonnanceur.
    WARN("forgot to cleanup scheduler");

    return OK;
}

static void calculate_and_print_metrics(processes_t processes)
{
    UNUSED(processes);

    // TODO: Calculez et imprimez les métriques demandées dans l'énoncé
    // à la fin de l'exécution de votre simulation.
    //
    // Utilisez un moyenne mobile pour calculer les temps de réponse.

    WARN("`calculate_and_print_metrics` must be implemented");
}

// NOTE(Chargé): Vous n'avez pas besoin de modifier le code ci-dessous.

static bool all_terminated(processes_t processes)
{
    FOR_EACH_PROCESS(process, processes)
    {
        if (process->status != TERMINATED) {
            return false;
        }
    }

    return true;
}

static options_t options = {
    .file = NULL,
    .scheduler_type = FCFS_SCHEDULER,
    .quantum = DEFAULT_QUANTUM,
    .trace = false,
};

static void parse_options(int argc, char** argv);
static processes_t read_processes(FILE* file);

int main(int argc, char** argv)
{
    parse_options(argc, argv);

    error_t err;

    FILE* file = fopen(options.file, "r");
    if (file == NULL) {
        ERROR("failed to open the \"%s\" file", options.file);
    }

    processes_t processes = read_processes(file);
    (void)fclose(file);

    if (processes.ptr == NULL) {
        ERROR("failed to read processes");
    }

    err = run_simulation(&options, processes);
    if (err != OK) {
        free(processes.ptr);
        ERROR("failed to run simulation to completion: %s", error_to_str(err));
    }

    calculate_and_print_metrics(processes);

    free(processes.ptr);

    return EXIT_SUCCESS;
}

static void parse_options(int argc, char** argv)
{
    static const char* shortopts = ":f:s:q:ht";
    static const struct option longopts[] = {
        { "file", required_argument, NULL, 'f' },
        { "scheduler", required_argument, NULL, 's' },
        { "help", no_argument, NULL, 'h' },
        { "quantum", required_argument, NULL, 'q' },
        { "trace", no_argument, NULL, 't' },
        { NULL, 0, NULL, 0 }
    };

    typedef struct string_to_scheduler_type {
        const char* string;
        scheduler_type_t scheduler_type;
    } string_to_scheduler_type_t;

    static const string_to_scheduler_type_t schedulers[] = {
        { "fcfs", FCFS_SCHEDULER },
        { "priority", PRIORITY_SCHEDULER },
        { "hybrid", HYBRID_SCHEDULER },
        { "f", FCFS_SCHEDULER },
        { "p", PRIORITY_SCHEDULER },
        { "h", HYBRID_SCHEDULER },
        { NULL, 0 }
    };

    const string_to_scheduler_type_t* string_to_scheduler_type_it = schedulers;

    bool usage = false;

    while (true) {
        int code = getopt_long(argc, argv, shortopts, longopts, NULL);

        if (code == -1) {
            break;
        }

        switch (code) {
        case 'f':
            options.file = optarg;

            break;
        case 's':
            while (string_to_scheduler_type_it->string != NULL) {
                if (strcasecmp(string_to_scheduler_type_it->string, optarg) == 0) {
                    break;
                }

                string_to_scheduler_type_it++;
            }

            if (string_to_scheduler_type_it->string == NULL) {
                usage = true;
            } else {
                options.scheduler_type = string_to_scheduler_type_it->scheduler_type;
            }

            break;
        case 'q': {
            char* endptr;
            options.quantum = strtoull(optarg, &endptr, 0);

            if (*endptr != '\0') {
                usage = true;
            }
        }

        break;
        case 't':
            options.trace = true;
            break;
        case 'h':
        case '?':
        case ':':
            usage = true;

            break;
        default:
            WARN("getopt_long returned an unknown character code: %c", code);
            exit(EXIT_FAILURE);
            return;
        }
    }

    if (usage || options.file == NULL) {
        printf(
            "UTILISATION:\n"
            "\n"
            "\t%s --file <file> [--scheduler fcfs|priority|hybrid] [--quantum <ms>] [--help]\n"
            "\n"
            "DESCRIPTION:\n"
            "\n"
            "\tLance la simulation d'une stratégie d'ordonnancement.\n"
            "\n"
            "OPTIONS:\n"
            "\n"
            "\t--file <file>\n"
            "\t\tIndique à la simulation quel fichier d'entrées de processus à utiliser\n"
            "\n"
            "\t--scheduler fcfs|priority|hybrid\n"
            "\t\tIndique à la simulation le type d'ordonnanceur à utiliser. La valeur par défaut est \"fcfs\".\n"
            "\n"
            "\t--quantum <ms>\n"
            "\t\tDéfinit la longueur du quantum.  N'a pas d'impact sur l'ordonnancement, seulement sur la vitesse de la simulation.  La valeur par défaut est 1000, et la valeur 0 indique de ne pas attendre.\n"
            "\n"
            "\t--trace\n"
            "\t\tIndique à la simulation que nous désirons plus de détails à chaque quanta d'exécution sur l'état des ordonnanceurs.\n"
            "\n"
            "\t--help\n"
            "\t\tAffiche ce message d'erreur.\n",
            argv[0]);
        exit(EXIT_FAILURE);
    }
}

static processes_t read_processes(FILE* file)
{
    size_t processes_length = 0;
    size_t processes_capacity = DEFAULT_PROCESSES_CAPACITY;
    process_t* processes = malloc(sizeof(*processes) * processes_capacity);
    if (processes == NULL) {
        WARN("failed to allocate a list of processes");
        return (processes_t) { NULL, 0 };
    }

    char* line = NULL;
    size_t line_len = 0;

    while (getline(&line, &line_len, file) != -1) {
        if (processes_length >= processes_capacity) {
            processes_capacity *= 2;
            process_t* new_processes = realloc(processes, sizeof(*processes) * processes_capacity);
            if (new_processes == NULL) {
                free(processes);
                WARN("failed to reallocate the processes");
                return (processes_t) { NULL, 0 };
            }
            processes = new_processes;
        }

        process_t* process = &processes[processes_length++];
        process_init_from_line(process, line, processes_length);

        free(line);
        line = NULL;
        line_len = 0;
    }

    free(line);

    return (processes_t) { processes, processes_length };
}
