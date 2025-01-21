obj-m += driver.o

CFLAGS = -Wall -Wextra -O3 -Wuninitialized

all:
	@make -C /lib/modules/$(shell uname -r)/build M=$(PWD) modules

clean:
	@make -C /lib/modules/$(shell uname -r)/build M=$(PWD) clean
	@rm -rf ./config ./pop ./push

install: 
	@sudo dmesg --clear
	@sudo insmod driver.ko
	@sudo mknod /dev/vicharak__ c 64 0

remove:
	@sudo rmmod driver.ko
	@sudo rm -rf /dev/vicharak__
	@make clean

test:
	@gcc ${CFLAGS} -o config config.c 
	@gcc ${CFLAGS} -o push push.c
	@gcc ${CFLAGS} -o pop pop.c 
