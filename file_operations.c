
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include <errno.h>
#include <sys/types.h>
#include "file_operations.h"

int create_and_write_file(const char *filename, const char *content) {
    int fd;             
    ssize_t bytes_written;       

  
    printf("Creating file: %s\n", filename);
    printf("Content to write: %s\n", content);

   
    fd = open(filename, O_CREAT | O_WRONLY | O_TRUNC, 0644);
    if (fd == -1) {
        perror("open (create_and_write_file)");
        return -1;
    }

    printf("File descriptor: %d\n", fd);

 
    size_t len = strlen(content);
    size_t total_written = 0;

    while (total_written < len) {
        bytes_written = write(fd, content + total_written, len - total_written);
        if (bytes_written == -1) {
            if (errno == EINTR) continue;     
            perror("write");
            close(fd);                          // best-effort close
            return -1;
        }
        total_written += (size_t)bytes_written;
    }

    printf("Successfully wrote %zu bytes to '%s'\n", total_written, filename);

    
    if (close(fd) == -1) {
        perror("close (create_and_write_file)");
        return -1;
    }
    printf("File closed successfully\n");
    return 0;
}

int read_file_contents(const char *filename) {
    int fd;                       
    char buffer[1024];            
    ssize_t bytes_read;           

   
    printf("Reading file: %s\n", filename);


    fd = open(filename, O_RDONLY);
    if (fd == -1) {
        perror("open (read_file_contents)");
        return -1;
    }

    printf("File descriptor: %d\n", fd);
    printf("\n--- Contents of '%s' ---\n", filename);


    for (;;) {
        bytes_read = read(fd, buffer, sizeof(buffer) - 1);
        if (bytes_read > 0) {
            buffer[bytes_read] = '\0'; // null-terminate so printf is safe
            printf("%s", buffer);
        } else if (bytes_read == 0) {
           
            break;
        } else {
            if (errno == EINTR) continue; 
            perror("read");
            close(fd);
            return -1;
        }
    }

    printf("\n--- End of file ---\n");

  
    if (close(fd) == -1) {
        perror("close (read_file_contents)");
        return -1;
    }
    printf("File closed successfully\n");
    return 0;
}

