#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>
#include <sys/ioctl.h>
#include <string.h>

#include "driver.h"

int main(int args, char** argv) {
    if (args != 2){
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

    data * d = malloc(sizeof(data));
    if (!d){
      printf(RED);
      printf("Malloc failed\n");
      printf(CRESET);
      return 1;
    }

    d->length = strlen(argv[1]);
    d->data = malloc(d->length * sizeof(char));

    if(! d->data){
      printf(RED);
      printf("Malloc failed\n");
      printf(CRESET);
      return 1;
    }

    memcpy(d->data, argv[1], d->length);
    int ret = ioctl(fd, PUSH_DATA, d);

    switch (ret) {
      case OK:
      printf(GRN);
      printf("./push -> PASSED, PUSHED {.length = %d, .data = %s}\n", d->length, d->data);
        break;
      case INVALID_IP:
        printf(RED);
        printf("./push -> FAILED {INVALID_INPUT}, {.length = %d, .data = %s}\n", d->length, d->data);
        break;
      case ERROR:
        printf(RED);
        printf("./push -> FAILED {ERROR}, {.length = %d, .data = %s}\n", d->length, d->data);
        break;
      case FULL:
        printf(YEL);
        printf("./push -> FAILED {DCQ IS FULL}, {.length = %d, .data = %s}\n", d->length, d->data);
      default:
        break;
    } 
    printf(CRESET);

    close(fd);
    free(d->data);
    free(d);

    return ret;
}
