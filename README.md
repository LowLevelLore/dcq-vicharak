# Vicharak DCQ

A dynamic circular queue implemented in the kernel space of linux, using IOCTL calls.

## Dependencies

* Any linux disto
* linux-headers corrsponding to that distro and version (tip: always have the latest verison of headers and distr, wasted 2 hours here)
* superuser access

## How to use

**CUSTOM BUILD & TESTS**

1. **Clone this repo and change the folder**

   ```bash
   git clone <url> <path> && cd <path>
   ```
2. **Build the driver**

   ```bash
   make
   ```
3. **Insert the driver**

   ```bash
   sudo insmod driver.ko
   ```
4. **Make a node for driver/ mount the driver**

   ```bash
   sudo mknod /dev/vicharak__ c 64 0
   ```
5. **Run your tests**

   ```bash
   sudo ./config <new_size>
   ```

   **OR/AND**

   ```bash
   sudo ./push <string>
   ```

   **OR/AND**

   ```bash
   sudo ./pop 
   ```

   **NOTE: Run any sequence of these operations to perform your test, you can use the following command to see the kernel space logs**

   ```bash
   sudo dmesg | tail -40
   ```
6. **Remove the driver**

   ```bash
   sudo sudo rmmod driver.ko
   ```
7. **Unmount the driver**

   ```bash
   sudo rm -rf /dev/vicharak__
   ```
8. **Clean the folder**

   ```bash
   make clean
   ```

**PREDEFINED BUILD AND TESTS**

```bash
chmod +x run.sh
sudo ./run.sh
```


## References:

* [This legacy tutorial](https://github.com/Johannes4Linux/Linux_Driver_Tutorial_legacy)
* Various stackoverflow pages
* [TLDP](https://tldp.org/LDP/lkmpg/2.6/html/x569.html)
