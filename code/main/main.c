#include "freertos/FreeRTOS.h"
#include "esp_wifi.h"
#include "esp_system.h"
#include "esp_event.h"
#include "esp_event_loop.h"
#include "nvs_flash.h"
#include "driver/gpio.h"
#include "driver/i2c.h"

/////////  I2C Setup
////////////////////////////////
#define I2C_MASTER_FREQ_HZ 100000

#define SDA_PIN 25
#define SCL_PIN 26

#define I2C_MASTER_NUMBER I2C_NUM_1 //I2C port number 

#define READ_BIT 0x1
#define WRITE_BIT 0x0




// The 7-bit I2C address of touchpad
#define SLAVE_ADDR 0x2A

//Acknowldeg?? <----Figure out what this does 
#define ACK_CHECK_EN 0x1
#define ACK_VAL 0x0
#define NACK_VAL 0x1

// READ and WRITE mask bytes
#define WRITE_MASK 0x80
#define READ_MASK 0xA0


//Test Registers to test
#define FIRMVER 0x01



/////// Trackpad Hardware setup
//////////////////////////////////

#define DR_PIN GPIO_NUM_4


#define PINNACLE_XMAX     2047    // max value Pinnacle can report for X
#define PINNACLE_YMAX     1535    // max value Pinnacle can report for Y
#define PINNACLE_X_LOWER  127     // min "reachable" X value
#define PINNACLE_X_UPPER  1919    // max "reachable" X value
#define PINNACLE_Y_LOWER  63      // min "reachable" Y value
#define PINNACLE_Y_UPPER  1471    // max "reachable" Y value
#define PINNACLE_X_RANGE  (PINNACLE_X_UPPER-PINNACLE_X_LOWER)
#define PINNACLE_Y_RANGE  (PINNACLE_Y_UPPER-PINNACLE_Y_LOWER)


///////// Struct to hold position data
////////////////////////////////////

typedef struct _posData { 

	uint16_t xValue;
	uint16_t yValue;
	uint16_t zValue;
	uint8_t buttonFlags;
	bool touchDown;


} posData_t;

posData_t posData;

//init the i2c master 
static void i2c_master_init()
{
	int i2c_master_port = I2C_MASTER_NUMBER;
	i2c_config_t conf; // Declare the i2c config struct

	conf.mode = I2C_MODE_MASTER;
	conf.sda_io_num = SDA_PIN;
	conf.sda_pullup_en = GPIO_PULLUP_ENABLE;  
	conf.scl_io_num = SCL_PIN;
	conf.scl_pullup_en = GPIO_PULLUP_ENABLE;	
	conf.master.clk_speed = I2C_MASTER_FREQ_HZ;
	
	i2c_param_config(i2c_master_port, &conf); // pass conf struct 
	i2c_driver_install(i2c_master_port, conf.mode, 0, 0, 0);
}

static void gpio_setup(){
	
	//Set the DR pin to input
	gpio_pad_select_gpio(DR_PIN);
	gpio_set_direction(DR_PIN, GPIO_MODE_OUTPUT);

}

//Handshake with the slave 
//data write is the byte that we will write.

static void RAP_WRITE( int8_t addr, uint8_t val)
{

	//send start conditions to get things started
	i2c_cmd_handle_t cmd = i2c_cmd_link_create();
	i2c_master_start(cmd); 


	//Send i2c write command to slave 
	i2c_master_write_byte(cmd, (SLAVE_ADDR << 1) | WRITE_BIT, ACK_CHECK_EN); 
	
	//Send the register and the value we want to write to OR'd together
	i2c_master_write_byte(cmd, WRITE_MASK | addr  , ACK_CHECK_EN);

	//Send the value to write to the register
	i2c_master_write_byte(cmd, val, ACK_CHECK_EN);

	// Stop this whole i2c business
	i2c_master_stop(cmd); 
	
	//Send the queued commands and wait 1 second max.
	i2c_master_cmd_begin(I2C_MASTER_NUMBER, cmd, 1000 / portTICK_RATE_MS);
	i2c_cmd_link_delete(cmd); //Delete the Command link


}

static void RAP_READ( uint8_t addr, uint8_t* data_h, uint8_t* data_l)
{
	
	i2c_cmd_handle_t cmd = i2c_cmd_link_create(); // Create cmd link to hold our commands
	i2c_master_start(cmd); //Generate a start signal on the clk line

	//write the handshake byte. Addr << 1 | 0 . Tells cirque we want to write. 
	i2c_master_write_byte(cmd, (SLAVE_ADDR << 1) | WRITE_BIT, ACK_CHECK_EN); 

	//Send the READ command (0xA0) OR'd with register address to be read.
	//This sets the register we will be reading from until we reset it
	i2c_master_write_byte(cmd, READ_MASK | addr, ACK_CHECK_EN); 

	//execute our commands
	i2c_master_cmd_begin(I2C_MASTER_NUMBER, cmd, 1000/ portTICK_RATE_MS); 
	
	//Stop the transmission and kill the cmd link
	i2c_master_stop(cmd); 
	i2c_cmd_link_delete(cmd); //Delete the command link


	//patience
	vTaskDelay(30 / portTICK_RATE_MS);

	//Start it up again
	cmd = i2c_cmd_link_create(); 
	i2c_master_start(cmd); 	
	//
	//Tell the pad what register we want to read (0x2A) | a
	i2c_master_write_byte(cmd,( SLAVE_ADDR << 1 ) | READ_BIT , ACK_CHECK_EN);	
	i2c_master_read_byte(cmd, data_h, ACK_VAL);
	i2c_master_read_byte(cmd, data_l, NACK_VAL);
	
	//Send all our commands
	i2c_master_cmd_begin(I2C_MASTER_NUMBER, cmd, 1000 / portTICK_RATE_MS);
	
	//clean up after ourselves.
	i2c_master_stop(cmd);
	i2c_cmd_link_delete(cmd);




}


esp_err_t event_handler(void *ctx, system_event_t *event)
{
    return ESP_OK;
}







void clearFlags()
{
	RAP_WRITE(0x02, 0x00);
	vTaskDelay(50 / portTICK_RATE_MS);	//delay 50 micorseconds
}

void setup()
{
	//init the I2c
	i2c_master_init();
	
	//enable data feed 
	RAP_WRITE(0x04, 0x01);
	
	//GIPO setup
	
		

	// Clear SW_CC flag 
	clearFlags();

	

	

}


bool DR_Asserted()
{
	//return read status of DR_PIN
	return false;
}


void app_main(void)
{
}


