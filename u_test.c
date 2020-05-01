#include<stdio.h>
#include<stdlib.h>
#include<errno.h>
#include<fcntl.h>
#include<string.h>
#include<unistd.h>
#include<sys/ioctl.h>
#include<sys/types.h>
#include<sys/stat.h>
#include<sys/select.h>
#include<sys/time.h>
#include<fcntl.h>
#include<errno.h>


#define DEVICE_NODE "/dev/vchar_dev"



#define BUFFER_LENGTH 4              ///< The buffer length (crude but fine)
     ///< The receive buffer from the LKM
 

int main()
{
   unsigned int ret, fd;
   unsigned int result;
   char option;

   printf("WELCOME TO THE DRIVER DEVICE INTERFACE \n");
   printf("Make your own choices: \n");
   printf("---------\n");
   printf("*Press 1 to open the device file\n");
   printf("*Press 2 to read the random 4 bytes number in the device file\n");
   printf("*Press 3 to exit device file\n");

   while(1){
   	printf("You choose: ");
  	scanf(" %c" ,&option);
   switch(option){
	case'1':
   		fd = open(DEVICE_NODE, O_RDWR);             // Open the device with read/write 	access
  		 if (fd < 0)
		{
     		 	printf("Failed to open device file!.........\n");
			return 0;
		}
   		else{
			printf("Device file opened!\n");
  		 }
     		break;
  	
	case'2':
   		printf("Reading from the device...\n");
   		ret = read(fd, &result, sizeof(BUFFER_LENGTH));        // Read the response from the LKM
   		printf("Random number is: %u\n", ret);
		break;
	

	case'3':	
   		close(fd); /////CLOSE the device
   		printf("Good bye!\n");
   		return 0;
	
	default:
		printf("Invalid input %c \n", option); 
		break;
	}
   };
}