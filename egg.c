#include <stdlib.h>  
 #include <stdio.h>  
 #include <string.h>  
 #include <unistd.h>     //close()  
 #include <fcntl.h>     //define O_WONLY and O_RDONLY  

#define MAX_BUF 64     //This is plenty large  
  
char* date_time()
{
    time_t current_time;
    char* c_time_string;
  
    /* Obtain current Epoch time. Epoch is the number of seconds that
     * have elapsed since January 1, 1970 (midnight UTC/GMT) 
     */
    current_time = time(NULL);
  
    if (current_time == ((time_t)-1))
    {
        printf("Error in computing current time.");
        return c_time_string;
    }
  
    /* Convert to local time format. */
    c_time_string = ctime(&current_time);
    

    if (NULL == c_time_string)
    {
        printf("Error in conversion of current time.");
        return c_time_string;
    }
  
    /* Print current Time and Date */

    return c_time_string;
}

 int readTemp()  
 {  
      int fd;          //file pointer  
      char buf[MAX_BUF];     //file buffer  
      char val[5];     //holds up to 4 digits for  value  
      
      //Create the file path by concatenating the  pin number to the end of the string  
      //Stores the file path name string into "buf"  
      snprintf(buf, sizeof(buf), "/sys/bus/i2c/drivers/bmp085/1-0077/temp0_input");     //Concatenate ADC file name  
      
		fd = open(buf, O_RDONLY);      
      //Will trigger if the i2c is not enabled  
      if (fd < 0) {  
           perror("LED - problem opening VALUE");  
      }//end if  
      
      read(fd, &val, 5);     //read temp ing val (up to 4 digits 0-1799)  
      close(fd);     //close file and stop reading  
      return atoi(val);     //returns an integer value (rather than ascii)  
 }//end read 

 int readPress()  
 {  
      int fd;          //file pointer  
      char buf[MAX_BUF];     //file buffer  
      char val[5];     //holds up to 4 digits for  value  
      
      //Create the file path by concatenating the  pin number to the end of the string  
      //Stores the file path name string into "buf"  
      snprintf(buf, sizeof(buf), "/sys/bus/i2c/drivers/bmp085/1-0077/pressure0_input");     //Concatenate ADC file name  
      
		fd = open(buf, O_RDONLY);      
      //Will trigger if the i2c is not enabled  
      if (fd < 0) {  
           perror("LED - problem opening VALUE");  
      }//end if  
      
      read(fd, &val, 5);     //read temp ing val (up to 4 digits 0-1799)  
      close(fd);     //close file and stop reading  
      return atoi(val);     //returns an integer value (rather than ascii)  
 }//end read 
 
void email(float tempF){
    
    if (tempF<99){
        system( "tail some-logfile.log | mail -s 'Temperature is below 99 Degress Fahrenheit - Please warm up ' reyes159@gmail.com");
		system("echo 1 > /sys/class/gpio/gpio49/value");

    }
    else if (tempF>102){
        system( "tail some-logfile.log | mail -s ' Temperature is over 102 degrees Fahrenheite - please cool down' reyes159@gmail.com");
		system("echo 1 > /sys/class/gpio/gpio49/value");

    }
    else if (tempF>99 && tempF<102){
        system( "tail some-logfile.log | mail -s 'Temperature between 99 and 102 degrees Fahrenheit - Happy Eggs' reyes159@gmail.com");
        system("echo 0 > /sys/class/gpio/gpio49/value");
    }
    else{
        system("echo 0 > /sys/class/gpio/gpio49/value");

    }
    
}

int main(){
    system("echo bmp085 0x77 > /sys/class/i2c-adapter/i2c-1/new_device");

	//gpio led setup
	//cat direction
	//echo 1 > value
	//echo 49 > export
	system("echo 49 > /sys/class/gpio/export");
	//cat direction
	system("echo out > /sys/class/gpio/gpio49/direction");
	printf("LED SETUP COMPLETE \n");
	
	
	time_t current_time,previous_time;
	time(&previous_time);
	float tempF;
	float tempC;
	while(1){
	
		printf("reading Temperature Celsius  =  .. %d \n ", readTemp() );
		tempC=readTemp()/10;
		tempF=(1.8* tempC+32);
		printf("reading Temperature Fahrenheit  =  .. %f \n ", tempF );
		printf("reading Pressure = .. %d \n ", readPress() );
        email(tempF);
        sleep(10);
        
	}
}


//http://www.techcrashcourse.com/2014/10/c-program-to-print-current-date-time.html
//http://dumb-looks-free.blogspot.com/2014/06/beaglebone-black-bbb-send-mail-via-gmail.html
//http://elinux.org/Beagleboard:BMP_on_the_Beagle_Bone_Black
//http://www.thebrokendesk.com/post/sending-tweets-from-the-beaglebone-black/
