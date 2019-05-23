#include<stdio.h>
#include<stdlib.h>
#include<errno.h>
#include<fcntl.h>
#include<string.h>
#include<unistd.h>
 
#define BUFFER_LENGTH 256               

 
int main(){
   int result, fd;
   char SendMessage[BUFFER_LENGTH];
   printf("Starting device test code example...\n");
   fd = open("/dev/chardev", O_RDWR);             // Open the device with read/write access
   if (fd < 0){
      perror("Failed to open the device...");
      return errno;
   }
   printf("Type a short message to send to the kernel module:\n");
   scanf("%[^\n]%*c", SendMessage);  			// Read in a string (with spaces)
   printf("Sending  this '%s' message to the device...\n", SendMessage);   
   result = write(fd, SendMessage, strlen(SendMessage)); // Send the string to the LKM

   if (result < 0){
      perror("Failed to write the message to the device.");
      return errno;
   }
   printf("End of the program!\n");
   return 0;
}
