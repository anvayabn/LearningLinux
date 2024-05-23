KDIR :=/lib/modules/6.8.0-31-generic/build
PWD :=$(shell pwd)

obj-m += spinlk.o

all:
	$(MAKE) -C $(KDIR) M=$(PWD) modules

clean:
	$(MAKE) -C $(KDIR) M=$(PWD) clean

