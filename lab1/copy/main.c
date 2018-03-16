#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/types.h>
#include <stdio.h>
#include <unistd.h>

#include <memory.h>
#include <fcntl.h>

/*** parameter "-lpthread" is required when compiled with gcc ***/
#define BUFF_SIZE 128

int main(int argc, char const *argv[]) {
    if (argc != 3) {
        printf("parameter not correct!\n [path\\]copy sourceFilename destinationFileName");
        return 1;
    }
    const char *src = argv[1];
    const char *dest = argv[2];

    unsigned char buff[BUFF_SIZE];
    int fd_src = open(src,O_RDONLY);
//    int fd_dest = creat(dest,O_WRONLY);
    int fd_dest = open(dest,O_WRONLY|O_CREAT,0666);

    ssize_t temp,temp2;
    while ( (temp2=read(fd_src,buff,BUFF_SIZE)) == BUFF_SIZE ) {

        printf("temp2 = %d",temp2);
        if ( (temp=write(fd_dest,buff,BUFF_SIZE)) <= 0)  {

            printf("write error temp = %d",temp);
        }
    }
//    write(fd_dest,buff,strlen(buff));
    write(fd_dest,buff,temp2);
    printf("temp2 = %d",temp2);
//    write(fd_dest,buff, sizeof(char)*strlen(fd_dest));


    close(fd_dest);
    close(fd_src);
}
