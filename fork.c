/* 

Write a c program that does the following:

    Forks off 2 child processes
    Child process instructions:
        At the start print its pid, and a random number in the range [1, 5]. e.g. "123531 5sec". These numbers should be different each time, but could match sometimes (1/5 of the time NOT ALL THE TIME!)
        Sleep for a random number of seconds equal to the random number printed earlier (do not re-random the value!)
        Each child calculates their own random, so they can each sleep for a different number of seconds
        At the end print a message "PID finished" e.g. "123531 finished after 5 seconds."

    Parent process instructions. Note the strings have _placeholders_ designated with underscores.
        Before forking, print out the initial message "_PID_ about to create 2 child processes."
        After the fork, wait for any child to finish
        Once a child has finished, print out a message. That includes its own PID and information about the child that exited. Then end the program.
        The message should be: "Main Process _PID_ is done. Child _EXITING_CHILD_PID_ slept for _SEC_ sec"
        The other child might still be running and can finish after the parent, that's intended behavior.


*/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <time.h>

int main() {
    pid_t pid, child_pid;
    int status;
    int n = 2;

    printf("%d about to create 2 child processes.\n", getpid());

    for (int i = 0; i < n; i++) {
        pid = fork();
        if (pid == 0) { // Child process
            srand(time(NULL) ^ (getpid() << 16)); // Seed random with unique value per process
            int random_secs = (rand() % 5) + 1; // Random number in range [1, 5]
            printf("%d %dsec\n", getpid(), random_secs);
            sleep(random_secs); // Sleep for the random duration
            printf("%d finished after %d seconds.\n", getpid(), random_secs);
            exit(random_secs); // Exit with the random seconds as the exit code
        } else if (pid < 0) { // Error during fork
            perror("Fork failed");
            exit(EXIT_FAILURE);
        }
    }

    // Parent process waits for one child to finish
    child_pid = wait(&status);
    if (WIFEXITED(status)) {
        int sleep_time = WEXITSTATUS(status);
        printf("Main Process %d is done. Child %d slept for %d sec\n", getpid(), child_pid, sleep_time);
    }

    // Parent process finishes here
    return 0;
}
