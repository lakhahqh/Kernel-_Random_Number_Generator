KDIR = /lib/modules/`uname -r`/build
all: 
	make -C $(KDIR) M=`pwd`

clean:
	make -C $(KDIR) M=`pwd` clean
	rm u_test
test: u_test.c
	cc -o u_test u_test.c
in: 
	sudo insmod character_driver.ko
out: 
	sudo rmmod character_driver.ko
admin:
	sudo chmod 666 /dev/vchar_dev

status:
	clear
	sudo ./u_test
