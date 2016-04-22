.PHONY: all

MODULE=./module
CLIENT=./test_client
COMMON=./common

KERNELDIR ?= /tmp/linux-4.2.3

all: make_common make_module make_client

make_common:
	cp $(COMMON)/* $(MODULE)
	cp $(COMMON)/* $(CLIENT)

make_module:
	make -C $(MODULE) KERNELDIR=$(KERNELDIR)

make_client:
	make -C $(CLIENT)

clean: clean_module clean_client

clean_module:
	make -C $(MODULE) clean

clean_client:
	make -C $(CLIENT) clean
