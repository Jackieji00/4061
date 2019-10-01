#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>

int main(void) {
    int fd[2];
    int nbytes;
    pid_t cpid;
    char base[] = "This is the base string.\n";
    char extra[] = "This string should be appended to the base and returned to the parent.\n";
    char buf[128];
    int fdd[2];

    pipe(fd);
    pipe(fdd);
    if ((cpid = fork()) == -1) {
        perror("fork");
        return 1;
    }

    if (cpid > 0) {
        // Parent process closes 'read' side of pipe
        close(fd[0]);
        // Send base string through 'write' side of pipe
        write(fd[1], base, (strlen(base)+1));
        // Wait for child to finish
        wait(NULL);
        close(fd[1]);

        //TODO Read the final string back from the child process, and print it
        close(fdd[1]);
        read(fdd[0],buf,sizeof(buf));
        printf("parent: %s\n",buf );
        close(fdd[1]);
    } else {
        // Child process closes the 'write' side of the pipe
        close(fd[1]);
        // Read bytes from 'read' side of pipe
        nbytes = read(fd[0], buf, sizeof(buf));
        printf("Received string: %s", buf);
        close(fd[0]);

        //TODO Append the 'extra' string to the base string, and send the result
        //     back to the parent process using pipes.
        strcat(buf,extra);
        close(fdd[0]);
      	write(fdd[1], buf, (strlen(buf)+1));
      	close(fdd[1]);
    }
    return 0;
}
