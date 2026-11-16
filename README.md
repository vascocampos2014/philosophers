# philosophers

*Part of the 42 Lisboa common core.*

A solution to the classic **dining philosophers** problem in C, using POSIX threads and mutexes. Each philosopher is a thread. Each fork is a mutex shared with a neighbour. The simulation has to keep everyone fed with no data races and no deadlocks, and it has to report a death within 10 ms.

## Build

```sh
make        # builds ./philo
make clean / fclean / re
```

## Usage

```sh
./philo number_of_philosophers time_to_die time_to_eat time_to_sleep [must_eat_count]
```

All times are in milliseconds. With the optional `must_eat_count` the simulation stops once every philosopher has eaten that many times. Without it, it runs until someone dies.

```sh
./philo 5 800 200 200        # nobody should die
./philo 5 800 200 200 7      # stops after everyone has eaten 7 times
./philo 4 310 200 100        # a philosopher dies
./philo 1 800 200 200        # a single philosopher only has one fork, so dies
```

Each state change is printed as `timestamp_ms id action`:

```
0 1 has taken a fork
0 1 is eating
200 1 is sleeping
400 1 is thinking
```

## Design

- **No deadlock**: even and odd philosophers pick up their two forks in opposite orders, and their thread starts are staggered, so a circular wait can't form.
- **No data races**: every shared value (last meal time, meal count, the "simulation over" flag, stdout) sits behind its own mutex.
- **Monitor**: a dedicated monitor thread polls each philosopher's last meal time and stops the simulation as soon as one starves or everyone is full.
- **Precise sleeping**: `precise_usleep` sleeps in short 100 µs steps, which keeps the timing tight enough for the 10 ms death-report window.

## Layout

| Path | Purpose |
| --- | --- |
| `srcs/main.c` | Argument checks and simulation start |
| `srcs/init.c` | Table, forks and philosopher setup |
| `srcs/philo_life.c` | The eat / sleep / think routine |
| `srcs/set_state.c` | Thread-safe state printing and the stop flag |
| `srcs/helper_philo.c`, `srcs/aux_functions.c` | Time, sleep and parsing helpers |
| `srcs/handle_error.c` | Error reporting and cleanup |
