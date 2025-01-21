#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <stdlib.h>

#include "driver.h"

int main(void) {
    int fd = open(DRIVER_NAME, O_RDWR);
    if(fd < 0){
      printf(RED);
      printf("Cannot open driver\n");
      printf(CRESET);
      return 1;
    }

    data * d = malloc(sizeof(data));
    if (!d){
      printf(RED);
      printf("Malloc failed\n");
      printf(CRESET);
      return 1;
    }

    d->length = 1024;
    d->data = malloc(1024); // I know this is unsafe !!
  
    if(! d->data){
      printf(RED);
      printf("Malloc failed\n");
      printf(CRESET);
      return 1;
    }

    int ret = ioctl(fd, POP_DATA, d);
    switch (ret) {
      case OK:
        printf(GRN);
        printf("./pop -> PASSED, POPPED {.length = %d, .data = %s}\n", d->length, d->data);
        break;
      case INVALID_IP:
        printf(RED);
        printf("./pop -> FAILED {INVALID_INPUT}\n");
        break;
      case ERROR:
        printf(RED);
        printf("./pop -> FAILED {ERROR}\n");
        break;
      default:
        break;
    }
    printf(CRESET);

    close(fd);
    free(d->data);
    free(d);
    return ret;
}
