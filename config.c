#include <fcntl.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <stdio.h>
#include <stdlib.h>
#include "driver.h"

int main(int args, char** argv) {
    if(args != 2){
      printf(RED);  
      printf("Invalid Arguments!!\nUSAGE: ./config <size>");
      printf(CRESET);
      return 1;

    }
    int fd = open(DRIVER_NAME, O_RDWR);
    if(fd < 0){
      printf(RED);
      printf("Cannot open driver\n");
      printf(CRESET);
      return 1;
    }
    int size = atoi(argv[1]);
    int ret = ioctl(fd, SET_SIZE_OF_QUEUE, & size);
    switch (ret) {
      case OK:
        printf(GRN);
        printf("./config -> PASSED\n");
        break;
      case INVALID_IP:
        printf(RED);
        printf("./config -> FAILED {INVALID_INPUT}\n");
        break;
      case ERROR:
        printf(RED);
        printf("./config -> FAILED {ERROR}\n");
        break;
      default:
        break;
    } 
    printf(CRESET);
    close(fd);
    return ret;
}

