/*
 * memfd_create + fexecve "in-memory" execution
 */

#include <sys/syscall.h>    /* used to call SYS_memfd_create() */
#include <sys/mman.h>
#include <sys/stat.h>       /* fstat()                         */

#include <stdio.h>          /* standart input-output           */
#include <stdlib.h>
#include <unistd.h>

/* xor obfuscate algorithm */
#include "obfuscate.h"

// Macro used in memfd_create function
#define __NR_memfd_create 319

// Prototypes
int memfd_create(const char *name, unsigned int flags);
int initFD();
int copyFile(int fd, char* filename);
int anonFdExecute(int fd, char* args[]);

// Enviroment used in fexecve, in "execute" function
extern char **environ;

/* Function main with program arguments */
int main(int argc, char** argv) {
    if (argc < 2) {
        printf(AY_OBFUSCATE("Usage: %s 'yourfile.elf'\n"), (char*)argv[0]);
        return 0;
    }
    
    int fd = initFD();
    copyFile(fd, argv[1]);
    anonFdExecute(fd, argv);
	return 0;
}

/* Wrap the memfd_create syscall in a convienent function */
int memfd_create(const char *name, unsigned int flags) {
    return syscall(__NR_memfd_create, name, flags);
}

/* Used to init the anonymous fd and return */
int initFD() {
    int fd = memfd_create(AY_OBFUSCATE("[ksmd/1]"), MFD_CLOEXEC);
    
    if (fd == -1) 
	perror(AY_OBFUSCATE("memfd_create"));
        return fd;
}

/* Copy file to fd */
int copyFile(int fd, char* filename) {
	FILE *fp = fopen(filename, "rb");

	// Declare the stat structure and use lstat to get file info
	struct stat fileInfo;
	if (lstat(filename, &fileInfo) != 0) {
		perror(AY_OBFUSCATE("Error "));
		exit(1);
	}
	
	// Variable with filesize using the stat structure
	off_t filesize = fileInfo.st_size;

	// Buffer to "save" the file content
	unsigned char *buf = (unsigned char*)malloc(filesize);

	// Read the file from buffer
	size_t reader = fread(buf, 1, filesize, fp);
	
	// Write the file to fd
	write(fd, buf, reader);

	// Error handling
	if (fp == NULL) {
            return -1;
  	}
  	
  	// Close the file and freed buf
  	fclose(fp);
  	free(buf);
  	
	return 0;
}

int anonFdExecute(int fd, char* args[]) {
    /* Using fexecve to execute an elf in-memory with anonymous fd */
    if (fexecve (fd, args, environ) < 0) {
    	perror(AY_OBFUSCATE("Error "));
    	exit(1);
    }
    
    return 0;
}
