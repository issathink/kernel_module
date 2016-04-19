.PHONY: all 
#.SECONDARY:

ifneq ($(KERNELRELEASE),)

obj-m := mycmd.o
mycmd-objs := commandes.o commande_list.o commande_kill.o commande_meminfo.o commande_modinfo.o commande_wait.o

else
	
  KERNELDIR ?= /lib/modules/$(shell uname -r)/build
  PWD := $(shell pwd)

all :
	$(info obj-m : $(obj-m))
	make -C $(KERNELDIR) M=$(PWD) modules

clean:
	make -C $(KERNELDIR) M=$(PWD) clean

endif
