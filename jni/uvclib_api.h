#ifndef UVCLIB_API_H
#define UVCLIB_API_H

#include "v4l2_dyna_ctrls.h"



//Added by e-con for its UVC XU control

//Guid of e_cam33 extension unit : {A917C75D-4119-11DA-AE0E-000D56AC7B4C} 
#define UVC_GUID_ECAM_WEBCAM				{0x5D, 0xC7, 0x17, 0xA9, 0x19, 0x41, 0xDA, 0x11, 0xAE, 0x0E, 0x00, 0x0D, 0x56, 0xAC, 0x7B, 0x4C}

// V4L2 ID FOR UVC XU CONTROLS
#define V4L2_CID_BASE_ECAM				V4L2_CID_PRIVATE_BASE + 0x00ECCB00	//E-con Camera Control Base
#define V4L2_CID_REG_MODE_ECAM      			V4L2_CID_BASE_ECAM
#define V4L2_CID_FLIP_MODE_ECAM      			V4L2_CID_BASE_ECAM+1
#define V4L2_CID_EEPROM_MODE_ECAM			V4L2_CID_BASE_ECAM+2
#define V4L2_CID_FIRMWARE_MODE_ECAM			V4L2_CID_BASE_ECAM+3
#define V4L2_CID_FOCUS_MODE_ECAM 			V4L2_CID_BASE_ECAM+4

#define E_CAM33_USB_CHIP_ID				0x03

//EEPROM CONTROL REG_ADDRESS
#define EEPROM_DEVICE_ADDRESS_FOR_READ_OPERATION	0xA0
#define EEPROM_DEVICE_ADDRESS_FOR_WRITE_OPERATION 	0xA1
#define EEPROM_FW_START_ADDRESS				0x0000
#define EEPROM_FW_END_ADDRESS				0x4000
#define EEPROM_FWV_REG_ADDRESS1				0x0096
#define EEPROM_FWV_REG_ADDRESS2				0x0097
#define EEPROM_FWV_REG_ADDRESS3				0x0098
#define EEPROM_FWV_REG_ADDRESS4				0x0099

#define DMA_EEPROM_MODE_SELECT_REG_ADDR			0x810F

#define REG_CONTROL_REG_TYPE_CHIP_REGISTER		(0x0 << 0) 
#define REG_CONTROL_REG_TYPE_SENSOR_REGISTER		(0x1 << 0)
#define REG_CONTROL_REG_TYPE_MASK			(0xF << 0)
#define REG_CONTROL_REG_VALUE_TYPE_BYTE			(0x0 << 4)
#define REG_CONTROL_REG_VALUE_TYPE_WORD			(0x1 << 4)
#define REG_CONTROL_REG_VALUE_MASK			(0x1 << 4)
#define REG_CONTROL_READ_REGISTER			(0x0 << 5)
#define REG_CONTROL_WRITE_REGISTER			(0x1 << 5)
#define REG_CONTROL_READ_WRITE_MASK			(0x1 << 5)

#define SENSOR_DEVICE_ADDRESS				0x3C  // 7bit address
#define SENSOR_DEVICE_ADDRESS_FOR_READ_OPERATION	(SENSOR_DEVICE_ADDRESS << 1)
#define SENSOR_DEVICE_ADDRESS_FOR_WRITE_OPERATION	((SENSOR_DEVICE_ADDRESS << 1) | 0x01)


#define SENSOR_MODE_SELECT_REG_ADDR			0x8102
#define SENSOR_FOCUS_SELECTION_REG_ADDR_3M		0x3F00
#define SENSOR_FOCUS_STATUS_REG_ADDR_3M			0x3F01
#define SENSOR_MANUAL_FOCUS_REG_ADDR_3M			0x3F06
#define SENSOR_SELECT_STROBE_REG_ADDR_3M		0x307A

#define SENSOR_ID1_REG_ADDR				0x300A
#define SENSOR_ID2_REG_ADDR				0x300B

//OV5640
#define SENSOR_CMD_MAIN_REG_ADDR_5M			0x3022
#define SENSOR_CMD_ACK_REG_ADDR_5M			0x3023
#define SENSOR_CMD_PARA3_REG_ADDR_5M			0x3027
#define SENSOR_MANUAL_FOCUS_REG_ADDR_5M			0x3028
#define SENSOR_FW_STATUS_REG_ADDR_5M			0x3029
#define SENSOR_SELECT_STROBE_REG_ADDR_5M		0x3B00
#define SENSOR_ENABLE_STROBE_REG_ADDR_5M		0x3016
#define DATA_FOR_ENABLE_STROBE_OUTPUT			0x02
#define DATA_FOR_INITIAL				0x00

#define SENSOR_V_FLIP_CONTROL_REG_ADDR			0x307C
#define SENSOR_H_MIRROR_CONTROL_REG_ADDR		0x3090

#define DATA_FOR_SETTING_SINGLE_TRIGGER_MODE_3M		0x03
#define DATA_FOR_SETTING_MANUAL_FOCUS_MODE_3M		0x05
#define DATA_FOR_FREEZE_IN_AUTO_FOCUS_MODE_3M		0x06
#define DATA_FOR_UNFREEZE_IN_AUTO_FOCUS_MODE_3M		0x07

#define DATA_FOR_SETTING_CMD_ACK_5M			0x01
#define DATA_FOR_SETTING_SINGLE_TRIGGER_MODE		0x03
#define DATA_FOR_SETTING_CNT_AUTO_FOCUS_MODE		0x04
#define DATA_FOR_RELEASING_FOCUS_MODE			0x08
#define DATA_FOR_SETTING_MANUAL_FOCUS_POSITION		0x1A
#define DATA_FOR_GETTING_MANUAL_FOCUS_POSITION		0x1B

#define EXT_BUTTONS_REG_ADDR				0x8107
#define MIN_MANUAL_FOCUS_POSITON			0
#define MAX_MANUAL_FOCUS_POSITION			255

#define YEAR_FIRST_TWO_DIGITS 				"20"
#define DELAY						500000

#define LENGTH_OF(x) (sizeof(x)/sizeof(x[0]))

enum e_cam33_selector
{
	XU_HW_CONTROL_FLIP=1,
	XU_HW_CONTROL_AUTO_FOCUS,
	XU_HW_CONTROL_AUDIO_STREAMING_DETECTION,
	XU_HW_CONTROL_REGISTER,
	XU_HW_CONTROL_I2C_DEVICE,
	XU_HW_CONTROL_SENSOR_IDENTIFICATION,
	XU_HW_CONTROL_GET_FIRMWARE,
	XU_HW_CONTROL_GPIO,
	XU_HW_CONTROL_POWER_LINE_FREQUENCY,
	XU_HW_CONTROL_READ_SENSOR_2_2,
	XU_HW_CONTROL_WRITE_SENSOR_2_2,
	XU_HW_CONTROL_READ_SENSOR_3_1,
	XU_HW_CONTROL_WRITE_SENSOR_3_1,
	XU_HW_CONTROL_EXTERNAL_FLASH,
	XU_HW_CONTROL_READ_AND_WRITE_EXTERNAL_FLASH,
	XU_HW_CONTROL_SENSOR = 0x10,
	XU_HW_CONTROL_DMA_IIC = 0x18
};

/* UVC XU controls */
struct uvc_xu_control_info e_cam_xu_ctrls[] = {

//Added by e-con for its uvc xu controls
/* FLIP CONTROL */
	{
    		.entity   = UVC_GUID_ECAM_WEBCAM,
    		.selector = XU_HW_CONTROL_FLIP,
		.index    = 0,
		.size     = 1,
		.flags    = UVC_CONTROL_SET_CUR | UVC_CONTROL_GET_CUR | UVC_CONTROL_GET_MIN | UVC_CONTROL_GET_MAX | UVC_CONTROL_GET_RES | UVC_CONTROL_GET_DEF
  	},
/* GPIO CONTROL */
	{
    		.entity   = UVC_GUID_ECAM_WEBCAM,
    		.selector = XU_HW_CONTROL_REGISTER,
		.index    = 3,
		.size     = 4,
		.flags    = UVC_CONTROL_SET_CUR | UVC_CONTROL_GET_CUR | UVC_CONTROL_GET_MIN | UVC_CONTROL_GET_MAX | UVC_CONTROL_GET_RES | UVC_CONTROL_GET_DEF
  	},
/* EEPROM CONTROL */
	{
		.entity	  = UVC_GUID_ECAM_WEBCAM,
		.selector = XU_HW_CONTROL_I2C_DEVICE,
		.index    = 4,
		.size	  = 4,
		.flags    = UVC_CONTROL_SET_CUR | UVC_CONTROL_GET_CUR | UVC_CONTROL_GET_MIN | UVC_CONTROL_GET_MAX | UVC_CONTROL_GET_RES | UVC_CONTROL_GET_DEF
	},
/*GET FIRMWARE VERSION(CHIP ID) CONTROL*/
	{
		.entity	  = UVC_GUID_ECAM_WEBCAM,
		.selector = XU_HW_CONTROL_GET_FIRMWARE,
		.index    = 6,
		.size	  = 4,
		.flags    = UVC_CONTROL_SET_CUR | UVC_CONTROL_GET_CUR | UVC_CONTROL_GET_MIN | UVC_CONTROL_GET_MAX | UVC_CONTROL_GET_RES | UVC_CONTROL_GET_DEF
	},
/* SENSOR CONTROL */
	{
    		.entity   = UVC_GUID_ECAM_WEBCAM,
    		.selector = XU_HW_CONTROL_SENSOR,
		.index    = 15,
		.size     = 9,
		.flags    = UVC_CONTROL_SET_CUR | UVC_CONTROL_GET_CUR | UVC_CONTROL_GET_MIN | UVC_CONTROL_GET_MAX | UVC_CONTROL_GET_RES | UVC_CONTROL_GET_DEF
  	}
//End
};

struct uvc_xu_control_mapping e_cam_xu_mappings[] = {

//Added by e-con for its uvc xu controls
/* MAPPING FOR FLIP CONTROL */
	{
    		.id        = V4L2_CID_FLIP_MODE_ECAM,
    		.name      = "ORIENTATION CTRL",
    		.entity    = UVC_GUID_ECAM_WEBCAM,
    		.selector  = XU_HW_CONTROL_FLIP,
    		.size      = 8,
    		.offset    = 0,
    		.v4l2_type = V4L2_CTRL_TYPE_INTEGER,
    		.data_type = UVC_CTRL_DATA_TYPE_UNSIGNED
  	},
/* MAPPING FOR GPIO CONTROL */
	{
    		.id        = V4L2_CID_REG_MODE_ECAM,
    		.name      = "GPIO CTRL",
    		.entity    = UVC_GUID_ECAM_WEBCAM,
    		.selector  = XU_HW_CONTROL_REGISTER,
    		.size      = 8,
    		.offset    = 0,
    		.v4l2_type = V4L2_CTRL_TYPE_INTEGER,
    		.data_type = UVC_CTRL_DATA_TYPE_UNSIGNED
  	},
/* MAPPING FOR EEPROM CONTROL */
	{
		.id	   = V4L2_CID_EEPROM_MODE_ECAM,
		.name	   = "EEPROM ACCESS CTRL",
		.entity    = UVC_GUID_ECAM_WEBCAM,
		.selector  = XU_HW_CONTROL_I2C_DEVICE,
		.size	   = 8,
		.offset	   = 0,
		.v4l2_type = V4L2_CTRL_TYPE_INTEGER,
		.data_type = UVC_CTRL_DATA_TYPE_UNSIGNED
	},
/* MAPPING FOR GET FIRMWARE VERSION (CHIP ID) CONTROL */
	{
		.id	   = V4L2_CID_FIRMWARE_MODE_ECAM,
		.name	   = "e-CAM FW VERSION CTRL",
		.entity    = UVC_GUID_ECAM_WEBCAM,
		.selector  = XU_HW_CONTROL_GET_FIRMWARE,
		.size	   = 8,
		.offset	   = 0,
		.v4l2_type = V4L2_CTRL_TYPE_INTEGER,
		.data_type = UVC_CTRL_DATA_TYPE_UNSIGNED
	},
/* MAPPING FOR SENSOR CONTROL */
	{
    		.id        = V4L2_CID_FOCUS_MODE_ECAM,
    		.name      = "SENSOR CTRL",
    		.entity    = UVC_GUID_ECAM_WEBCAM,
    		.selector  = XU_HW_CONTROL_SENSOR,
    		.size      = 8,
    		.offset    = 0,
    		.v4l2_type = V4L2_CTRL_TYPE_INTEGER,
    		.data_type = UVC_CTRL_DATA_TYPE_UNSIGNED
  	}
//End
};

enum diagnostic_control_options
{
	DIAGNOSTIC_CNT_READ_SENSOR_ID1=1,
	DIAGNOSTIC_CNT_READ_SENSOR_ID2
};

enum focus_selection_options
{
	CNT_AUTO_FOCUS_FREEZE_SELECT=1,
	CNT_AUTO_FOCUS_UNFREEZE_SELECT,
	MANUAL_FOCUS_SELECT,
	AUTO_FOCUS_SELECT,
	SINGLE_TRIGGER_AUTO_FOCUS_SELECT,
	GET_FOCUS_STATUS
};



int initUvcCtrls();
int sensor_mode_selection(void);
int diagnostic_control(int);
unsigned char reg_read_eeprom(int);
int read_sensor_reg(unsigned int, unsigned char *);
int write_sensor_reg(unsigned int, unsigned char);
int write_gpo_register(unsigned int, unsigned char);
int read_key_status_register();
int clear_key_status_register(unsigned char);
int auto_focus_control();
int open_device(char *);
int close_device(char *);
void open_logfile();
void close_logfile();
void debug_message(char *);





#endif
