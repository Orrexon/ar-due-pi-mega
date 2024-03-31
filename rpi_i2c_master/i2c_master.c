
#define Check(expression, msg, obj) \
	if((expression)){\
		printf((msg),(obj));\
	}

#define Check_fatal(expression, msg) \
	if((expression)){\
		printf((msg));\
		goto CLEANUP;\
	}

#include <unistd.h>			//Needed for I2C port
#include <fcntl.h>			//Needed for I2C port
#include <sys/ioctl.h>			//Needed for I2C port
#include <linux/i2c-dev.h>		//Needed for I2C port

#include <stdint.h>
#include <stdio.h>
#include <errno.h>

int compare(char* A, char* B, int len){
	for(int i = 0; 
			i<len;
			++i){
		if(A[i]!=B[i]){
			return 0;
		}
	}
	return 1;
}


int main(){
	int file_i2c;
	int length;
	unsigned char buffer[60] = {0};
	int i2c_running = 1;
	int counter = 0;

	//----- OPEN THE I2C BUS -----
	char *filename = (char*)"/dev/i2c-1";
	Check_fatal((file_i2c = open(filename, O_RDWR)) < 0,"Failed to open the i2c bus\n");

	int addr = (0x55);          //<<<<<The I2C address of the slave
	Check_fatal(ioctl(file_i2c, I2C_SLAVE, addr) < 0,"Failed to acquire bus access and/or talk to slave.\n");
	float sleep_time = 0.01f;

	while(i2c_running){	
		char command[64] = {0};
		fgets(command, 64, stdin);
		char led13[5] = "led13";
		if(compare(command, led13, sizeof(led13)/sizeof(led13[0]) )){
			buffer[0] = 0x01;
		}
		else if(command[0] == '2'){
			buffer[0] = 0x02;
		}
		else if(command[0] == 'q'){
			i2c_running = 0;
			buffer[0] = 'q';
		}
		length = 1;	//<<< Number of bytes to write
		Check(write(file_i2c, buffer, length) != length,"Failed to write to the i2c bus.\n%d \n", errno)		//write() returns the number of bytes actually written, if it doesn't match then an error occurred (e.g. no response from the device)
		sleep(sleep_time);

		length = 1;	//<<< Number of bytes to read
		Check(read(file_i2c, buffer, length) != length,"Failed to read from the i2c bus.\n%d \n", errno)		//read() returns the number of bytes actually read, if it doesn't match then an error occurred (e.g. no response from the device)
		else
		{
			printf("Data read: %d\n", buffer[0]);
			if(buffer[0] == 0x01){//blink state
				sleep_time = 2.f;
			}
			else if(buffer[0] == 0x02){
				sleep_time = 0.02f;
			}
			else sleep_time = 0.01f;
		}
	}


CLEANUP:
	if(file_i2c){
		close(file_i2c);
	}
	return 0;

}
