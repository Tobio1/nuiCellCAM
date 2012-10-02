#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <unistd.h>
#include <assert.h>
#include <fcntl.h>      
#include <errno.h>
#include <malloc.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/ioctl.h>
#include <time.h>
#include <linux/kernel.h>
#include <android/log.h>

#include "videodev2.h"
#include "uvclib_api.h"

int uvc_fd = -1;
FILE *fstream = NULL;
char g_buff[256];
bool g_debugmode = SET;
int g_current_device = ecam_5M;

/************************************************************************************************************
 *  
 *  MODULE TYPE	:	LIBRARY API				
 *  Name	:	InitLibrary
 *  Parameter1	:	char *
 *  Parameter2	:	
 *  Returns	:	int
 *  Description	: 	to initializes the e-con cam library  	
 ************************************************************************************************************/
int InitLibrary(char *DeviceName)
{
	int ret = 0, i = 0;
		
	time_t ltime; /* calendar time */
	ltime = time(NULL); /* get current cal time */
	errno = 0;

//Open the log file to save the debug messages.
	open_logfile();

	debug_message("*****************************************\n");
	debug_message("   ***UVC WEBCAM APPLICATION STARTS***   \n");
	sprintf(g_buff,"       AT %s",asctime(localtime(&ltime)));
	debug_message(g_buff);
	debug_message("*****************************************\n");

	if(DeviceName == NULL)
	{
		debug_message("Device Name is empty\n");
		ret = FAILURE;
		goto end_of_InitLibrary;
	}
	
//Open the video device for UVC XU control application
	ret = open_device(DeviceName);
	if(ret < 0)
	{
		sprintf(g_buff,"Failed to open video device %s\n", DeviceName);
		debug_message(g_buff);
		goto end_of_InitLibrary;
	}

//Init the uvc controls by adding and mapping it to the library.
	ret = initUvcCtrls();
	if(ret < 0)
	{
		debug_message("Error in reading the UVC Controls\n");
		goto end_of_InitLibrary;
	} 

//Select the Sensor Mode as 9 byte transfer
	ret = sensor_mode_selection();
	if(ret < 0)
	{
		debug_message("Failed to do sensor mode selection \n");
		goto end_of_InitLibrary;
	}

// Add the UVC XU Control for SENSOR
	i=(LENGTH_OF(e_cam_xu_ctrls)-1);
	if ((ret=ioctl(uvc_fd, UVCIOC_CTRL_ADD, &e_cam_xu_ctrls[i])) < 0 )
	{
		sprintf(g_buff, "Adding control for %s\n", e_cam_xu_mappings[i].name);
		debug_message(g_buff);
      		if ((errno != EEXIST) && (errno != EACCES))
		{
        		sprintf(g_buff, "Uvcioc ctrl add error: errno=%d (retval=%d) %s \n", errno, ret,strerror(errno));
			debug_message(g_buff);
			goto end_of_InitLibrary;
      		}
		else if (errno == EACCES)
		{
			printf("need admin previledges for adding extension controls\n");
			printf("please run 'ecamguvcview' as root (or with sudo)\n");
			return FAILURE;
		}
		else
		{
			ret = 0;
        		sprintf(g_buff, "Control for %s already exists\n", e_cam_xu_mappings[i].name);
			debug_message(g_buff);
      		}
    	}
	else
	{
		sprintf(g_buff,"FUNC %s(): LINE %d: Success in Adding the UVC Control for %s \n",__FUNCTION__,__LINE__,e_cam_xu_mappings[i].name);
		debug_message(g_buff);
	}

// Map the added UVC XU Control for SENSOR
	i=(LENGTH_OF(e_cam_xu_mappings)-1);
	if ((ret=ioctl(uvc_fd, UVCIOC_CTRL_MAP, &e_cam_xu_mappings[i])) < 0)
	{
      		sprintf(g_buff, "Mapping controls for %s\n", e_cam_xu_mappings[i].name);
		debug_message(g_buff);
		if ((errno != EEXIST) && (errno != EACCES))
		{
        		sprintf(g_buff, "uvcioc ctrl map error: errno=%d (retval=%d) %s \n", errno, ret,strerror(errno));
			debug_message(g_buff);
			goto end_of_InitLibrary;
      		}
		else if (errno == EACCES)
		{
			printf("need admin previledges for adding extension controls\n");
			printf("please run 'ecamguvcview' as root (or with sudo)\n");
			return FAILURE;
		}
		else
		{
			ret = 0;
		        sprintf(g_buff,"Mapping for %s already exists\n", e_cam_xu_mappings[i].name);
			debug_message(g_buff);
      		}
    	}
	else
	{
		sprintf(g_buff, "FUNC %s(): LINE %d: Success in Mapping the UVC Control for %s \n",__FUNCTION__,__LINE__,e_cam_xu_mappings[i].name);
		debug_message(g_buff);
	}


end_of_InitLibrary:
	if(ret < 0)
	{
		close_device(DeviceName);
		close_logfile();
		return FAILURE;
	}

	return SUCCESS;
}

/************************************************************************************************************
 *  
 *  MODULE TYPE	:	LIBRAY API 				
 *  Name	:	DeInitLibrary
 *  Parameter1	:	UINT8*
 *  Parameter2	:	
 *  Returns	:	int
 *  Description	:       to release all the extension unit objects and other internal library objects		
 ************************************************************************************************************/
int DeInitLibrary(char *DeviceName)
{
	close_device(DeviceName);
	close_logfile();

	return SUCCESS;
}

/************************************************************************************************************
 *  
 *  MODULE TYPE	:	LIBRAY API 				
 *  Name	:	AFFreezeControl
 *  Parameter1	:	int
 *  Parameter2	:	
 *  Returns	:	int
 *  Description	:       to select the Focus mode of the sensor	
 ************************************************************************************************************/
int AFFreezeControl(int FreezeControl)
{
	int AFStatus,ret;
	switch(FreezeControl)
	{
		case AF_CONT_FREEZE_MODE:
		{
			if(auto_focus_control(CNT_AUTO_FOCUS_FREEZE_SELECT) == SUCCESS)
			{
				usleep(DELAY);
				ret = GetAFStatus(&AFStatus);
				if(ret == FAILURE)
				{
					debug_message("Auto Focus Toggle Fails at Get AF status \n");
					return FAILURE;
				}
				if(AFStatus == AF_CONT_FROZEN)
				{
					return AF_CONT_FREEZE_MODE;
				}
				else
					return FAILURE;
			}
			else
			{
				return FAILURE;
			}
		}
		break;
		case AF_CONT_UNFREEZE_MODE:
		{
			if(auto_focus_control(CNT_AUTO_FOCUS_UNFREEZE_SELECT) == SUCCESS)
			{
				usleep(DELAY);
				ret = GetAFStatus(&AFStatus);
				if(ret == FAILURE)
				{
					debug_message("Auto Focus Toggle Fails at Get AF status \n");
					return FAILURE;
				}
				if((AFStatus == AF_CONT_FOCUSING) || (AFStatus == AF_CONT_FOCUSED))
				{
					return AF_CONT_UNFREEZE_MODE;
				}
				else
					return FAILURE;
			}
			else
			{
				return FAILURE;
			}
		}
		break;
		case MANUAL_FOCUS_MODE:
		{
			if(auto_focus_control(MANUAL_FOCUS_SELECT) == SUCCESS)
			{
				usleep(DELAY);
				ret = GetAFStatus(&AFStatus);
				if(ret == FAILURE)
				{
					debug_message("Auto Focus Toggle Fails at Get AF status \n");
					return FAILURE;
				}
				if(AFStatus == MANUAL_FOCUS)
				{
					return MANUAL_FOCUS_MODE;
				}
				else
					return FAILURE;
			}
			else
			{
				return FAILURE;
			}
		}
		break;
		case AUTO_FOCUS_MODE:
		{
			if(auto_focus_control(AUTO_FOCUS_SELECT) == SUCCESS)
			{
				usleep(DELAY);
				ret = GetAFStatus(&AFStatus);
				if(ret == FAILURE)
				{
					debug_message("Auto Focus Toggle Fails at Get AF status \n");
					return FAILURE;
				}
				if((AFStatus == AF_CONT_FOCUSING) || (AFStatus == AF_CONT_FOCUSED))
				{
					return AUTO_FOCUS_MODE;
				}
				else
					return FAILURE;
			}
			else
			{
				return FAILURE;
			}
		}
		break;
		case SINGLE_TRIG_AF_MODE:
		{
			if(auto_focus_control(SINGLE_TRIGGER_AUTO_FOCUS_SELECT) == SUCCESS)
			{
				usleep(DELAY);
				ret = GetAFStatus(&AFStatus);
				if(ret == FAILURE)
				{
					debug_message("Auto Focus Toggle Fails at Get AF status \n");
					return FAILURE;
				}
				if((AFStatus == AF_SINGLE_TRIGGER_FOCUSED) || (AFStatus == AF_SINGLE_TRIGGER_FOCUSING))
				{
					return SINGLE_TRIG_AF_MODE;
				}
				else
				{
					return FAILURE;
				}
			}
			else
			{
				return FAILURE;
			}
		}
		break;
	}
	return FAILURE;
}


/************************************************************************************************************
 *  
 *  MODULE TYPE	:	LIBRARY/CONTROL API 					
 *  Name	:	GetAFStatus
 *  Parameter1	:	int *
 *  Parameter2	:	
 *  Returns	:	int
 *  Description	:       to read the AutoFocus Freeze status	
 ************************************************************************************************************/
int GetAFStatus(int *AFStatus)
{
	int status;

	status = auto_focus_control(GET_FOCUS_STATUS);
	sprintf(g_buff,"focus status register value:%x\n",status);
	debug_message(g_buff);

	if(g_current_device == ecam_3M)
	{
		if((status & 0x80) == 0x80)
		{
			//Step State Failed
			*AFStatus = FAILURE;
			return FAILURE;
		}
		if((status & 0x0C) == 0x08)
		{
			//AUTO FOCUS MODE
			if((status & 0x03) == 0x01)
			{
				debug_message("AFStatus:In Auto focus mode, Focusing  \n");
				*AFStatus = AF_CONT_FOCUSING;
			}
			if((status & 0x03) == 0x02)
			{
				debug_message("AFStatus:In Auto focus mode, Focused \n");
				*AFStatus = AF_CONT_FOCUSED;
			}
			if((status & 0x03) == 0x03)
			{
				debug_message("AFStatus:In Auto focus mode, Freezed \n");
				*AFStatus = AF_CONT_FROZEN;
			}
		}	
		else if((status & 0x0C) == 0x04)
		{
			//SINGLE TRIGGER AUTO FOCUS
			if((status & 0x03) == 0x01)
			{
				debug_message("AFStatus:In Single Trigger Auto focus mode, Focusing  \n");
				*AFStatus = AF_SINGLE_TRIGGER_FOCUSING;
			}
			if((status & 0x03) == 0x02)
			{
				debug_message("AFStatus:In Single Trigger Auto focus mode, Focused \n");
				*AFStatus = AF_SINGLE_TRIGGER_FOCUSED;
			}
			if((status & 0x03) == 0x03)
			{	
				debug_message("AFStatus:In Single Trigger Auto focus mode, Freezed \n");
				*AFStatus = AF_SINGLE_TRIGGER_FROZEN;
			}	
		}	
		else if((status & 0x0C) == 0x0C)
		{
			//MANUAL FOCUS MODE
			debug_message("AFStatus:In Manual Mode \n");
			*AFStatus = MANUAL_FOCUS;
		}	
		else
		{
			*AFStatus = FAILURE;
			return FAILURE;
		}	
	}
	else if(g_current_device == ecam_5M)
	{
		if((status & 0x70) == 0x70)
		{
			//MANUAL FOCUS MODE
			debug_message("AFStatus:In Manual Mode \n");
			*AFStatus = MANUAL_FOCUS;
		}
		else if((status & 0x10) == 0x10)
		{
			//AUTO FOCUS MODE
			debug_message("AFStatus:In Auto focus mode, Focused \n");
			*AFStatus = AF_SINGLE_TRIGGER_FOCUSED;//AF_CONT_FOCUSED;
		}
		else if(status == 0x00)
		{
			//AUTO FOCUS MODE
			debug_message("AFStatus:In Auto focus mode, Focusing  \n");
			*AFStatus = AF_CONT_FOCUSING;			
		}
	}
	return SUCCESS;
}


/************************************************************************************************************
 *  
 *  MODULE TYPE	:	LIBRAY/CONTROL API 					
 *  Name	:	SetManualFocusPosition
 *  Parameter1	:	int
 *  Parameter2	:	
 *  Returns	:	int
 *  Description	:       to set the Manual Focus Position	
 ************************************************************************************************************/
int SetManualFocusPosition(int FocusPosition)
{
	unsigned char reg_value;

	if(g_current_device == ecam_3M) 
	{
		if(write_sensor_reg(SENSOR_MANUAL_FOCUS_REG_ADDR_3M, FocusPosition) == SUCCESS)
			return SUCCESS;
		else
			return FAILURE;
	}
	else if (g_current_device == ecam_5M)
	{
		write_sensor_reg(SENSOR_CMD_PARA3_REG_ADDR_5M, DATA_FOR_INITIAL);//0x3027
		write_sensor_reg(SENSOR_MANUAL_FOCUS_REG_ADDR_5M, FocusPosition); //0x3028				
		write_sensor_reg(SENSOR_CMD_ACK_REG_ADDR_5M, DATA_FOR_SETTING_CMD_ACK_5M);//0x3023
		write_sensor_reg(SENSOR_CMD_MAIN_REG_ADDR_5M, DATA_FOR_SETTING_MANUAL_FOCUS_POSITION); //0x3022
		read_sensor_reg(SENSOR_CMD_ACK_REG_ADDR_5M, &reg_value);

		/*
		while(reg_value != 0)
		{
			read_sensor_reg(SENSOR_CMD_ACK_REG_ADDR_5M, &reg_value);
		}
		*/
		if(reg_value == 0)
			return SUCCESS;
		else
			return FAILURE;
	}	
	else
		return FAILURE;
}

/************************************************************************************************************
 *  
 *  MODULE TYPE	:	LIBRAY/CONTROL API 				
 *  Name	:	GetManualFocusPosition
 *  Parameter1	:	int *	
 *  Parameter2	:	
 *  Returns	:	int
 *  Description	:       to get the Manual Focus Position
 ************************************************************************************************************/
int GetFocusPosition(int *FocusPosition)
{
	unsigned char reg_value;

	if(g_current_device == ecam_3M) 
	{
		if(read_sensor_reg(SENSOR_MANUAL_FOCUS_REG_ADDR_3M, &reg_value) == SUCCESS)
		{
			*FocusPosition = (unsigned short int)reg_value;
			return SUCCESS;
		}
		else
			return FAILURE;
	}
	else if (g_current_device == ecam_5M)
	{
		write_sensor_reg(SENSOR_CMD_ACK_REG_ADDR_5M, DATA_FOR_SETTING_CMD_ACK_5M);//0x3023
		write_sensor_reg(SENSOR_CMD_MAIN_REG_ADDR_5M, DATA_FOR_GETTING_MANUAL_FOCUS_POSITION); //0x3022
		read_sensor_reg(SENSOR_CMD_ACK_REG_ADDR_5M, &reg_value);
		
		while(reg_value != 0)
		{
			read_sensor_reg(SENSOR_CMD_ACK_REG_ADDR_5M, &reg_value);
		}
		if(reg_value == 0)
		{
			if(read_sensor_reg(SENSOR_MANUAL_FOCUS_REG_ADDR_5M, &reg_value) == SUCCESS)
			{
				*FocusPosition = (unsigned short int)reg_value;
				return SUCCESS;
			}
			else
				return FAILURE;
		}
		else
			return FAILURE;
	}
	else
		return FAILURE;
}


/************************************************************************************************************
 *  
 *  MODULE TYPE	:	LIBRAY/CONTROL API 					
 *  Name	:	ControlGPO
 *  Parameter1	:	int
 *  Parameter2	:	int
 *  Returns	:	int
 *  Description	:       to set or clear the GPO specified by the input parameters	 	
 ************************************************************************************************************/
int ControlGPO(int GPIONumber, int Value)
{
	unsigned int reg_addr;
	unsigned char reg_data;
	int ret = 0;

	if(GPIONumber == GPO8_STATE)
	{
		reg_addr = 0xB03C;
		if(Value == SET)
		{
			reg_data = 0x22;
		}
		else
		{
			reg_data = 0x20;
		}
		ret = write_gpo_register(reg_addr, reg_data);
		if(ret == FAILURE)
		{
			return FAILURE;
		}
	}
	else
	{
		return FAILURE;
	}
	return SUCCESS;
}

/************************************************************************************************************
 *  
 *  MODULE TYPE	:	LIBRAY/CONTROL API 					
 *  Name	:	GetGPIStatus
 *  Parameter1	:	int*
 *  Parameter2	:	
 *  Returns	:	int
 *  Description	:       to read the sensor id	 	
 ************************************************************************************************************/
int GetGPIStatus(int *GPIStatus)
{
	int gpi_status;
	
	// Read KeyStatus Register 0x8107
	gpi_status = read_key_status_register();
	if(gpi_status != FAILURE)
	{
		if(gpi_status & GPI7_STATE)
		{
			*GPIStatus |= 0x00000001;
		}
		if(gpi_status & GPI6_STATE)
		{
			*GPIStatus |= 0x00000002;
		}
		if(gpi_status & GPI3_STATE)
		{
			*GPIStatus |= 0x00000004;
		}

		//manually clear the GPI6 status in the GPI status register
		clear_key_status_register(gpi_status & ~(GPI6_STATE));

		return SUCCESS;
	}
	else
		return FAILURE;
}


/************************************************************************************************************
 *  
 *  MODULE TYPE	:	LIBRAY/CONTROL API 					
 *  Name	:	GetSensorId
 *  Parameter1	:	int
 *  Parameter2	:	
 *  Returns	:	int
 *  Description	:       to read the sensor id	 	
 ************************************************************************************************************/
int GetSensorId(int *SensorId)
{
	int sensor_id1,sensor_id2;

	sensor_id1 = diagnostic_control(DIAGNOSTIC_CNT_READ_SENSOR_ID1);
	sensor_id2 = diagnostic_control(DIAGNOSTIC_CNT_READ_SENSOR_ID2);

	if((sensor_id1 != FAILURE) && (sensor_id2 != FAILURE))
	{
		*SensorId = (sensor_id1 << 8) | sensor_id2;
		return SUCCESS;
	}
	else
	{
		return FAILURE;
	}
}


/************************************************************************************************************
 *  
 *  MODULE TYPE	:	LIBRAY/CONTROL API 				
 *  Name	:	GetFirmwareVersionInfo
 *  Parameter1	:	UINT8*
 *  Parameter2	:	
 *  Returns	:	int
 *  Description	:       to get the firmware version in the UVC chip
 ************************************************************************************************************/
int GetFirmwareVersionInfo(char *FirmwareVersion)
{
	int value1 = 0, value2 = 0, value3 = 0, value4 = 0;
	
	if((value1 = reg_read_eeprom(EEPROM_FWV_REG_ADDRESS1)) == FAILURE)
	{
		sprintf(g_buff, "Error in reading EEPROM Firmware Version register1: %x", EEPROM_FWV_REG_ADDRESS1);
		debug_message(g_buff);
		return FAILURE;
	}
	if((value2 = reg_read_eeprom(EEPROM_FWV_REG_ADDRESS2)) == FAILURE)
	{
		sprintf(g_buff, "Error in reading EEPROM Firmware Version register2: %x", EEPROM_FWV_REG_ADDRESS2);
		debug_message(g_buff);
		return FAILURE;
	}
	if((value3 = reg_read_eeprom(EEPROM_FWV_REG_ADDRESS3)) == FAILURE)
	{
		sprintf(g_buff, "Error in reading EEPROM Firmware Version register3: %x", EEPROM_FWV_REG_ADDRESS3);
		debug_message(g_buff);
		return FAILURE;
	}
	if((value4 = reg_read_eeprom(EEPROM_FWV_REG_ADDRESS4)) == FAILURE)
	{
		sprintf(g_buff, "Error in reading EEPROM Firmware Version register4: %x", EEPROM_FWV_REG_ADDRESS4);
		debug_message(g_buff);
		return FAILURE;
	}
	sprintf((char *)FirmwareVersion,"%s%d.%d.%d %s %d",YEAR_FIRST_TWO_DIGITS, value1, value2, value3, "Ver", value4);
	return SUCCESS;	
}


//List of Reserved API's
/************************************************************************************************************
 *  
 *  MODULE TYPE	:	LIBRAY/CONTROL API				
 *  Name	:	GetUVCChipId
 *  Parameter1	:	UINT8 *
 *  Parameter2	:
 *  Returns	:	int
 *  Description	:       Function for reading the UVC chip id from the device
 ************************************************************************************************************/
int GetUVCChipId(int *UVCChipId)
{
	int ret = 0;
	unsigned char data[]={0x00, 0x00, 0x00, 0x00};
	static struct  uvc_xu_control xu_ctrl_data = {
		/* DATA FOR GET FIRMWARE VERSION CONTROL */
		.unit		= 4,
		.selector	= XU_HW_CONTROL_GET_FIRMWARE,
		.size 		= 4,
		.data		= NULL
	};

	xu_ctrl_data.data = data;
	xu_ctrl_data.size = (sizeof(data));

	xu_ctrl_data.data[0] = 0x00;		//Date (1~31)
	xu_ctrl_data.data[1] = 0x00;		//Month (1~12)
	xu_ctrl_data.data[2] = 0x00;		//Year low (00 ~ 99)
	xu_ctrl_data.data[3] = 0x00;		//Year high/Chip Id

	ret = ioctl(uvc_fd,UVCIOC_CTRL_GET,&xu_ctrl_data);
	if(ret<0)
	{
		sprintf(g_buff, "FUNC %s(): LINE %d: failed to set new control value.errno=%d (retval=%d) %s \n",__FUNCTION__,__LINE__,errno, ret,strerror(errno));
		debug_message(g_buff);
		return FAILURE;
	}
	else			
	{
		sprintf(g_buff, "FUNC %s(): LINE %d: success in setting new control value.ret = %d \n",__FUNCTION__,__LINE__,ret);
		debug_message(g_buff);
	}
	sprintf(g_buff,"Date value %d\n",xu_ctrl_data.data[0]);
	debug_message(g_buff);
	sprintf(g_buff,"Month value %d\n",xu_ctrl_data.data[1]);
	debug_message(g_buff);
	sprintf(g_buff,"Year low value %d\n",xu_ctrl_data.data[2]);
	debug_message(g_buff);
	sprintf(g_buff,"Chip id value 0x%02x\n",xu_ctrl_data.data[3]);
	debug_message(g_buff);
	*UVCChipId = xu_ctrl_data.data[3];
	return SUCCESS;
}

//List of Internal Apis

/************************************************************************************************************
 *  
 *  MODULE TYPE	:	FUNCTION			
 *  Name	:	initUvcCtrls
 *  Parameter1	:	void
 *  Parameter2	:	
 *  Returns	:	int
 *  Description	: 	to add and map the uvc ctrls to the driver	
 ************************************************************************************************************/
int initUvcCtrls()
{
	int ret = 0, i = 0;
// Add the UVC XU Control for REGISTER, EEPROM and GET FIRMWARE VERSION
	for ( i=0; i<(LENGTH_OF(e_cam_xu_ctrls)-1); i++ )
	{
  		sprintf(g_buff, "Adding control for %s\n", e_cam_xu_mappings[i].name);
		debug_message(g_buff);
	    	if ((ret=ioctl(uvc_fd, UVCIOC_CTRL_ADD, &e_cam_xu_ctrls[i])) < 0 )
		{
      			if ((errno != EEXIST) && (errno != EACCES))
			{
        			sprintf(g_buff,"uvcioc ctrl add error: errno=%d (retval=%d) %s \n", errno, ret,strerror(errno));
				debug_message(g_buff);
				return FAILURE;
      			}
			else if (errno == EACCES)
			{
				printf("need admin previledges for adding extension controls\n");
				printf("please run 'ecamguvcview' as root (or with sudo)\n");
				return FAILURE;
			}
			else
			{
				ret = 0;
	       			sprintf(g_buff,"Control for %s already exists\n", e_cam_xu_mappings[i].name);
				debug_message(g_buff);
      			}
    		}
		else
		{
			sprintf(g_buff," FUNC %s(): LINE %d: Success in Adding the UVC Control for %s \n",__FUNCTION__,__LINE__,e_cam_xu_mappings[i].name);
			debug_message(g_buff);
		}
  	}
 

// Map the added UVC XU Control for REGISTER, EEPROM and GET FIRMWARE VERSION
  	for ( i=0; i<(LENGTH_OF(e_cam_xu_mappings)-1); i++ ) 
	{
    		sprintf(g_buff, "Mapping controls for %s\n", e_cam_xu_mappings[i].name);
		debug_message(g_buff);
    		if ((ret=ioctl(uvc_fd, UVCIOC_CTRL_MAP, &e_cam_xu_mappings[i])) < 0)
		{
      			if ((errno != EEXIST) && (errno != EACCES))
			{
        			sprintf(g_buff,"uvcioc ctrl map error: errno=%d (retval=%d) %s \n", errno, ret,strerror(errno));
				debug_message(g_buff);
				return FAILURE;
      			}
			else if (errno == EACCES)
			{
				printf("need admin previledges for adding extension controls\n");
				printf("please run 'ecamguvcview' as root (or with sudo)\n");
				return FAILURE;
			}
			else
			{
				ret = 0;
			       	sprintf(g_buff, "Mapping for %s already exists\n", e_cam_xu_mappings[i].name);
				debug_message(g_buff);
      			}
    		}
		else
		{
			sprintf(g_buff, " FUNC %s(): LINE %d: Success in Mapping the UVC Control for %s \n",__FUNCTION__,__LINE__,e_cam_xu_mappings[i].name);
			debug_message(g_buff);
		}
  	}
	return SUCCESS;
}


/************************************************************************************************************
 *  
 *  MODULE TYPE	:	FUNCTION			
 *  Name	:	sensor_mode_selection
 *  Parameter1	:	void
 *  Parameter2	:	
 *  Returns	:	int
 *  Description	: 	to select the sensor control as 9 byte mode	
 ************************************************************************************************************/
int sensor_mode_selection(void)
{
	unsigned char data[]={0x00,(SENSOR_MODE_SELECT_REG_ADDR & 0xFF), (SENSOR_MODE_SELECT_REG_ADDR>>8), 0x09};
	static struct  uvc_xu_control xu_ctrl_data ={
		// DATA FOR SENSOR MODE SEELCT ( 9 Bytes Data for SENSOR selector control) 
		.unit		= 4,
		.selector	= XU_HW_CONTROL_REGISTER,
		.size 		= 4,
		.data		= NULL
	};
	int ret=0;
	xu_ctrl_data.data = data;
	xu_ctrl_data.size = (sizeof(data));
	xu_ctrl_data.data[0] |= REG_CONTROL_WRITE_REGISTER;

	ret = ioctl(uvc_fd,UVCIOC_CTRL_SET,&xu_ctrl_data);
	if(ret<0)
	{
        	sprintf(g_buff, "FUNC %s(): LINE %d: failed to get control value.errno=%d (retval=%d) %s \n",__FUNCTION__,__LINE__,errno, ret,strerror(errno));
		debug_message(g_buff);
		return FAILURE;
	}
	else
	{
		//printf("FUNC %s(): LINE %d: current control value is %d (return value %d) \n",__FUNCTION__,__LINE__,(unsigned int)xu_ctrl_data.data[3],ret);
	}
	xu_ctrl_data.data[0] = 0x00;
	xu_ctrl_data.data[3] = 0x00;
	ret = ioctl(uvc_fd,UVCIOC_CTRL_GET,&xu_ctrl_data);
	if(ret<0)
	{
        	sprintf(g_buff, "FUNC %s(): LINE %d: failed to get control value.errno=%d (retval=%d) %s \n",__FUNCTION__,__LINE__,errno, ret,strerror(errno));
		debug_message(g_buff);
		return FAILURE;
	}
	else
	{
		sprintf(g_buff, "FUNC %s(): LINE %d: current control value is %d (return value %d) \n",__FUNCTION__,__LINE__,(unsigned int)xu_ctrl_data.data[3],ret);
		debug_message(g_buff);
	}
	return SUCCESS;
	
}


/************************************************************************************************************
 *  
 *  MODULE TYPE	:	FUCNTION 				
 *  Name	:	diagnostic_control
 *  Parameter1	:	int
 *  Parameter2	:	
 *  Returns	:	int
 *  Description	:       Control function for reading the Sensor ID
 ************************************************************************************************************/
int diagnostic_control(int diagnostic_option)
{
	unsigned char sensor_id1_value, sensor_id2_value;
	switch(diagnostic_option)
	{
		case DIAGNOSTIC_CNT_READ_SENSOR_ID1:
		{

			if((read_sensor_reg(SENSOR_ID1_REG_ADDR, &sensor_id1_value)) != FAILURE) 
			{
				return (int)sensor_id1_value;
			}
		}
		break;
		case DIAGNOSTIC_CNT_READ_SENSOR_ID2:
		{
			if((read_sensor_reg(SENSOR_ID2_REG_ADDR, &sensor_id2_value)) != FAILURE)
			{
				return (int)sensor_id2_value;
			}
		}
		break;
		default:
		{
			sprintf(g_buff, "FUNC %s(): LINE %d: %d is invalid option \n",__FUNCTION__,__LINE__,diagnostic_option);
			debug_message(g_buff);						
		}
		break;	
	} // end of switch 
	return FAILURE;
}


/************************************************************************************************************
 *  
 *  MODULE TYPE	:	FUCNTION 				
 *  Name	:	reg_read_eeprom
 *  Parameter1	:	int
 *  Parameter2	:	
 *  Returns	:	unsigned char
 *  Description	:       Function for reading the address location specified in the EEPROM
 ************************************************************************************************************/
unsigned char reg_read_eeprom(int reg_address)
{
	int ret = 0;
	unsigned char data[]={0x00, 0x00, 0x00, 0x00};
	static struct  uvc_xu_control xu_ctrl_data ={
		/* DATA FOR EEPROM CONTROL */
		.unit		= 4,
		.selector	= XU_HW_CONTROL_I2C_DEVICE,
		.size 		= 4,
		.data		= NULL
	};

	xu_ctrl_data.data = data;
	xu_ctrl_data.size = (sizeof(data));

	xu_ctrl_data.data[0] = EEPROM_DEVICE_ADDRESS_FOR_READ_OPERATION;
	xu_ctrl_data.data[1] = (reg_address & 0xFF);	// LSB of 16 bit EEPROM Register Address
	xu_ctrl_data.data[2] = (reg_address >> 8);	// MSB of 16 bit EEPROM Register Address
	xu_ctrl_data.data[3] = 0x00;
	ret = ioctl(uvc_fd,UVCIOC_CTRL_SET,&xu_ctrl_data);
	if(ret<0)
	{
		sprintf(g_buff, "FUNC %s(): LINE %d: failed to set new control value.errno=%d (retval=%d) %s \n",__FUNCTION__,__LINE__,errno, ret,strerror(errno));
		debug_message(g_buff);
		return FAILURE;
	}
	else
	{
		//sprintf(g_buff, "FUNC %s(): LINE %d: success in setting new control value.ret = %d \n",__FUNCTION__,__LINE__,ret);
		//debug_message(g_buff);
	}

	//usleep(2000);
		
	xu_ctrl_data.data[0] = EEPROM_DEVICE_ADDRESS_FOR_READ_OPERATION;
	xu_ctrl_data.data[1] = (reg_address & 0xFF);	// LSB of 16 bit EEPROM Register Address
	xu_ctrl_data.data[2] = (reg_address >> 8);	// MSB of 16 bit EEPROM Register Address
	xu_ctrl_data.data[3] = 0x00;
	ret = ioctl(uvc_fd,UVCIOC_CTRL_GET,&xu_ctrl_data);
	if(ret<0)
	{
		sprintf(g_buff, "FUNC %s(): LINE %d: failed to set new control value.errno=%d (retval=%d) %s \n",__FUNCTION__,__LINE__,errno, ret,strerror(errno));
		debug_message(g_buff);
		return FAILURE;
	}
	else			
	{
		sprintf(g_buff, "FUNC %s(): LINE %d: success in setting new control value.ret = %d \n",__FUNCTION__,__LINE__,ret);
		debug_message(g_buff);
		return (unsigned char)xu_ctrl_data.data[3];
	}
}


/************************************************************************************************************
 *  
 *  MODULE TYPE	:	FUNCTION					
 *  Name	:	read_sensor_reg
 *  Parameter1	:	unsigned int
 *  Parameter2	:	unsigned char *
 *  Returns	:	int
 *  Description	: 	to read the sensor register address(argument 1)
 ************************************************************************************************************/
int read_sensor_reg(unsigned int reg_address, unsigned char *reg_data)
{
	unsigned char data[]={0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00};
	static struct  uvc_xu_control xu_ctrl_data ={
		/* DATA FOR SENSOR CONTROL */
		.unit		= 4,
		.selector	= XU_HW_CONTROL_SENSOR,
		.size 		= 9,
		.data		= NULL
	};
	int ret=0;
	xu_ctrl_data.data = data;
	xu_ctrl_data.size = (sizeof(data));
	//sprintf(g_buff,"FUNC %s(): LINE %d: xu_ctrl_data.size is %d \n",__FUNCTION__,__LINE__,(unsigned int)xu_ctrl_data.size);
	//debug_message(g_buff);
	
	// Control Type
	xu_ctrl_data.data[0] = SENSOR_DEVICE_ADDRESS_FOR_READ_OPERATION;
	// Sensor Sub Address Byte Number
	xu_ctrl_data.data[1] = 0x02;	// Sensor Sub Address (or Register Address within the Sensor) is 16bit
	// Sensor value by number
	xu_ctrl_data.data[2] = 0x01;	// Sensor Data is 8bit
	// Sensor Address
	xu_ctrl_data.data[3] = (reg_address & 0xFF); // LSB of 16 bit Sensor Sub Address
	xu_ctrl_data.data[4] = (reg_address >> 8); // MSB of 16 bit Sensor Sub Address
	xu_ctrl_data.data[5] = 0x00; // Always 0x00 for 16bit Sensor Sub Address
	// Sensor Data
	xu_ctrl_data.data[6] = 0x00; 
	xu_ctrl_data.data[7] = 0x0; // Always 0x00 for 8bit Sensor Data
	xu_ctrl_data.data[8] = 0x0; // Always 0x00 for 8bit Sensor Data
	ret = ioctl(uvc_fd,UVCIOC_CTRL_SET,&xu_ctrl_data);
	if(ret<0)
	{
		sprintf(g_buff, "FUNC %s(): LINE %d: failed to get control value.errno=%d (retval=%d) %s \n",__FUNCTION__,__LINE__,errno, ret,strerror(errno));
		debug_message(g_buff);
		return FAILURE;
	}
	else
	{
		//printf("FUNC %s(): LINE %d: current control value is %d (return value %d) \n",__FUNCTION__,__LINE__,(unsigned int)xu_ctrl_data.data[3],ret);
	}

	// Control Type
	xu_ctrl_data.data[0] = SENSOR_DEVICE_ADDRESS_FOR_READ_OPERATION;
	// Sensor Sub Address Byte Number
	xu_ctrl_data.data[1] = 0x02;	// Sensor Sub Address (or Register Address within the Sensor) is 16bit
	// Sensor value by number
	xu_ctrl_data.data[2] = 0x01;	// Sensor Data is 8bit
	// Sensor Address
	xu_ctrl_data.data[3] = (reg_address & 0xFF); // LSB of 16 bit Sensor Sub Address
	xu_ctrl_data.data[4] = (reg_address >> 8); // MSB of 16 bit Sensor Sub Address
	xu_ctrl_data.data[5] = 0x00; // Always 0x00 for 16bit Sensor Sub Address
	// Sensor Data
	xu_ctrl_data.data[6] = 0x00; 
	xu_ctrl_data.data[7] = 0x0; // Always 0x00 for 8bit Sensor Data
	xu_ctrl_data.data[8] = 0x0; // Always 0x00 for 8bit Sensor Data
	ret = ioctl(uvc_fd,UVCIOC_CTRL_GET,&xu_ctrl_data);
	if(ret<0)
	{
		sprintf(g_buff, "FUNC %s(): LINE %d: failed to get control value.errno=%d (retval=%d) %s \n",__FUNCTION__,__LINE__,errno, ret,strerror(errno));
		debug_message(g_buff);
		return FAILURE;
	}
	else
	{
		sprintf(g_buff, "FUNC %s(): LINE %d: 0x%X Reg value is 0x%02X \n",__FUNCTION__,__LINE__,reg_address,(unsigned int)xu_ctrl_data.data[6]);
		debug_message(g_buff);
		*reg_data = xu_ctrl_data.data[6];
	}
	return SUCCESS;
}


/************************************************************************************************************
 *  
 *  MODULE TYPE	:	FUNCTION					
 *  Name	:	write_sensor_reg
 *  Parameter1	:	unsigned int
 *  Parameter2	:	unsigned char
 *  Returns	:	int
 *  Description	: 	to write the data(argument 2) in the sensor register address(argument 1)
 ************************************************************************************************************/
int write_sensor_reg(unsigned int reg_address, unsigned char reg_data)
{
	unsigned char data[]={0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00};
	static struct  uvc_xu_control xu_ctrl_data ={
		/* DATA FOR SENSOR CONTROL */
		.unit		= 4,
		.selector	= XU_HW_CONTROL_SENSOR,
		.size 		= 9,
		.data		= NULL
	};
	int ret=0;
	xu_ctrl_data.data = data;
	xu_ctrl_data.size = (sizeof(data));
	//sprintf(g_buff,"FUNC %s(): LINE %d: xu_ctrl_data.size is %d \n",__FUNCTION__,__LINE__,(unsigned int)xu_ctrl_data.size);
	//debug_message(g_buff);

	// Control Type
	xu_ctrl_data.data[0] = SENSOR_DEVICE_ADDRESS_FOR_WRITE_OPERATION;
	// Sensor Sub Address Byte Number
	xu_ctrl_data.data[1] = 0x02;	// Sensor Sub Address (or Register Address within the Sensor) is 16bit
	// Sensor value by number
	xu_ctrl_data.data[2] = 0x01;	// Sensor Data is 8bit
	// Sensor Address
	xu_ctrl_data.data[3] = (reg_address & 0xFF); // LSB of 16 bit Sensor Sub Address
	xu_ctrl_data.data[4] = (reg_address >> 8); // MSB of 16 bit Sensor Sub Address
	xu_ctrl_data.data[5] = 0x00; // Always 0x00 for 16bit Sensor Sub Address
	// Sensor Data
	xu_ctrl_data.data[6] = reg_data; // Data for flip bit
	xu_ctrl_data.data[7] = 0x0; // Always 0x00 for 8bit Sensor Data
	xu_ctrl_data.data[8] = 0x0; // Always 0x00 for 8bit Sensor Data
	ret = ioctl(uvc_fd,UVCIOC_CTRL_SET,&xu_ctrl_data);
	if(ret<0)
	{
   		sprintf(g_buff,"FUNC %s(): LINE %d: failed to get control value.errno=%d (retval=%d) %s \n",__FUNCTION__,__LINE__,errno, ret,strerror(errno));
		debug_message(g_buff);
		return FAILURE;
	}
	else
	{
		sprintf(g_buff,"FUNC %s(): LINE %d: current control value is %x (return value %d) \n",__FUNCTION__,__LINE__,(unsigned int)xu_ctrl_data.data[6],ret);
		debug_message(g_buff);
	}
	return SUCCESS;
}


/************************************************************************************************************
 *  
 *  MODULE TYPE	:	FUNCTION					
 *  Name	:	write_gpo_register
 *  Parameter1	:	unsigned int
 *  Parameter2	:	unsigned char
 *  Returns	:	int
 *  Description	: 	to write the data(argument 2) in the gpo register address(argument 1)
 ************************************************************************************************************/
int write_gpo_register(unsigned int reg_addr, unsigned char reg_data)
{

	unsigned char data[]={0x00,0x00,0x00,0x00};
	static struct  uvc_xu_control xu_ctrl_data ={
		/* DATA FOR LED CONTROL */
		.unit		= 4,
		.selector	= XU_HW_CONTROL_REGISTER,
		.size 		= 4,
		.data		= NULL
	};

	int ret=0;
	xu_ctrl_data.data = data;
	xu_ctrl_data.size = (sizeof(data));

	xu_ctrl_data.data[0] = REG_CONTROL_WRITE_REGISTER;
	xu_ctrl_data.data[1] = (reg_addr & 0xFF);
	xu_ctrl_data.data[2] = (reg_addr >> 8),
	xu_ctrl_data.data[3] = reg_data;
	ret = ioctl(uvc_fd,UVCIOC_CTRL_SET,&xu_ctrl_data);
	if(ret<0)
	{
		sprintf(g_buff, "FUNC %s(): LINE %d: failed to set new control value.errno=%d (retval=%d) %s \n",__FUNCTION__,__LINE__,errno, ret,strerror(errno));
		return FAILURE;
	}
	else
	{
		sprintf(g_buff, "FUNC %s(): LINE %d: success in setting new control value.ret = %d \n",__FUNCTION__,__LINE__,ret);
		debug_message(g_buff);
	}
	return SUCCESS;
}


/************************************************************************************************************
 *  
 *  MODULE TYPE	:	FUCNTION 				
 *  Name	:	read_key_status_register
 *  Parameter1	:	void
 *  Parameter2	:	
 *  Returns	:	int
 *  Description	:       Function for Reading the Status Register for External Keys Pressed Events	
 ************************************************************************************************************/
int read_key_status_register()
{
	unsigned char data[]={0x00,(EXT_BUTTONS_REG_ADDR & 0xFF), (EXT_BUTTONS_REG_ADDR>>8), 0x00};
	static struct  uvc_xu_control xu_ctrl_data ={
		/* DATA FOR FLIP CONTROL */
		.unit		= 4,
		.selector	= XU_HW_CONTROL_REGISTER,
		.size 		= 4,
		.data		= NULL
	};
	int ret=0;
	int value=0;
	
	xu_ctrl_data.data = data;
	xu_ctrl_data.size = (sizeof(data));
	
	/* Need to send a dummy write */
	xu_ctrl_data.data[0]=0x00;
	xu_ctrl_data.data[3]=0x00;
	ret = ioctl(uvc_fd,UVCIOC_CTRL_SET,&xu_ctrl_data);
	if(ret<0)
	{
		sprintf(g_buff, "FUNC %s(): LINE %d: failed to get control value.errno=%d (retval=%d) %s \n",__FUNCTION__,__LINE__,errno, ret,strerror(errno));
		debug_message(g_buff);
		return FAILURE;
	}
	else
	{
		//printf("FUNC %s(): LINE %d: current control value is %d (return value %d) \n",__FUNCTION__,__LINE__,(unsigned int)xu_ctrl_data.data[3],ret);
	}
	xu_ctrl_data.data[0]=0x00;
	xu_ctrl_data.data[3]=0x00;
	ret = ioctl(uvc_fd,UVCIOC_CTRL_GET,&xu_ctrl_data);
	if(ret<0)
	{
		sprintf(g_buff, "FUNC %s(): LINE %d: failed to get control value.errno=%d (retval=%d) %s \n",__FUNCTION__,__LINE__,errno, ret,strerror(errno));
		debug_message(g_buff);
		return FAILURE;
	}
	else
	{
		//sprintf(g_buff, "FUNC %s(): LINE %d: Status is 0x%02X \n",__FUNCTION__,__LINE__,(unsigned int) xu_ctrl_data.data[3]);
		//debug_message(g_buff);
	}
	value = (int)xu_ctrl_data.data[3];
	
	return value;
}


/************************************************************************************************************
 *  
 *  MODULE TYPE	:	FUCNTION 				
 *  Name	:	clear_key_status_register
 *  Parameter1	:	unsigned char
 *  Parameter2	:	
 *  Returns	:	int
 *  Description	:       Function for Clearing the AF Key Pressed Status in the Status Register	
 ************************************************************************************************************/
int clear_key_status_register(unsigned char value)
{
	unsigned char data[]={0x00,(EXT_BUTTONS_REG_ADDR & 0xFF), (EXT_BUTTONS_REG_ADDR>>8), 0x00};
	static struct  uvc_xu_control xu_ctrl_data ={
		/* DATA FOR FLIP CONTROL */
		.unit		= 4,
		.selector	= XU_HW_CONTROL_REGISTER,
		.size 		= 4,
		.data		= NULL
	};
	int ret=0;
	
	xu_ctrl_data.data = data;
	xu_ctrl_data.size = (sizeof(data));
	
	sprintf(g_buff, "FUNC %s(): LINE %d: Clear the AF Key Status \n",__FUNCTION__,__LINE__);
	debug_message(g_buff);
	xu_ctrl_data.data[0] |= REG_CONTROL_WRITE_REGISTER;
	xu_ctrl_data.data[3] = value;
	ret = ioctl(uvc_fd,UVCIOC_CTRL_SET,&xu_ctrl_data);
	if(ret<0)
	{
        	sprintf(g_buff, "FUNC %s(): LINE %d: failed to get control value.errno=%d (retval=%d) %s \n",__FUNCTION__,__LINE__,errno, ret,strerror(errno));
		debug_message(g_buff);
		return FAILURE;
	}
	else
	{
		//printf("FUNC %s(): LINE %d: current control value is %d (return value %d) \n",__FUNCTION__,__LINE__,(unsigned int)xu_ctrl_data.data[3],ret);
	}

	// Read Back the Status after clearing the AF Key Status
	sprintf(g_buff, "FUNC %s(): LINE %d: Read Back the Status after clearing the AF key Status \n",__FUNCTION__,__LINE__);
	debug_message(g_buff);
	/* Need to send a dummy write */
	xu_ctrl_data.data[0]=0x00;
	xu_ctrl_data.data[3]=0x00;
	ret = ioctl(uvc_fd,UVCIOC_CTRL_SET,&xu_ctrl_data);
	if(ret<0)
	{
		sprintf(g_buff, "FUNC %s(): LINE %d: failed to get control value.errno=%d (retval=%d) %s \n",__FUNCTION__,__LINE__,errno, ret,strerror(errno));
		debug_message(g_buff);
		return FAILURE;
	}
	else
	{
		//printf("FUNC %s(): LINE %d: current control value is %d (return value %d) \n",__FUNCTION__,__LINE__,(unsigned int)xu_ctrl_data.data[3],ret);
	}
	xu_ctrl_data.data[0]=0x00;
	xu_ctrl_data.data[3]=0x00;
	ret = ioctl(uvc_fd,UVCIOC_CTRL_GET,&xu_ctrl_data);
	if(ret<0)
	{
		sprintf(g_buff, "FUNC %s(): LINE %d: failed to get control value.errno=%d (retval=%d) %s \n",__FUNCTION__,__LINE__,errno, ret,strerror(errno));
		debug_message(g_buff);
		return FAILURE;
	}
	else
	{
		sprintf(g_buff, "FUNC %s(): LINE %d: Status is 0x%02X \n",__FUNCTION__,__LINE__,(unsigned int) xu_ctrl_data.data[3]);
		debug_message(g_buff);
	}
	return SUCCESS;
}


/************************************************************************************************************
 *  
 *  MODULE TYPE	:	FUCNTION 				
 *  Name	:	auto_focus_control
 *  Parameter1	:	int
 *  Parameter2	:	
 *  Returns	:	int
 *  Description	:       Control function for autofocus mode control
	
 ************************************************************************************************************/
int auto_focus_control(int focus_option)
{
	unsigned char focus_status, reg_value;
	unsigned int sensor_addr = 0;

	switch(focus_option)
	{
		case CNT_AUTO_FOCUS_FREEZE_SELECT:
		{
			if(g_current_device == ecam_3M)
			{
				if((write_sensor_reg(SENSOR_FOCUS_SELECTION_REG_ADDR_3M, DATA_FOR_FREEZE_IN_AUTO_FOCUS_MODE_3M)) != FAILURE)
				{
					return SUCCESS;
				}
				else
				{
					sprintf(g_buff,"Error in writing the sensor register:%x\n",SENSOR_FOCUS_SELECTION_REG_ADDR_3M);
					debug_message(g_buff);
					return FAILURE;
				}
			}
		}
		break;
		case CNT_AUTO_FOCUS_UNFREEZE_SELECT:
		{
			if(g_current_device == ecam_3M)
			{
				if((write_sensor_reg(SENSOR_FOCUS_SELECTION_REG_ADDR_3M, DATA_FOR_UNFREEZE_IN_AUTO_FOCUS_MODE_3M)) != FAILURE)
				{
					return SUCCESS;
				}
				else
				{
					sprintf(g_buff,"Error in writing the sensor register:%x\n",SENSOR_FOCUS_SELECTION_REG_ADDR_3M);
					debug_message(g_buff);
					return FAILURE;
				}
			}
		}
		break;
		case MANUAL_FOCUS_SELECT:
		{
			if(g_current_device == ecam_3M)
			{
				if((write_sensor_reg(SENSOR_FOCUS_SELECTION_REG_ADDR_3M, DATA_FOR_RELEASING_FOCUS_MODE)) != FAILURE)
				{
					if((write_sensor_reg(SENSOR_FOCUS_SELECTION_REG_ADDR_3M, DATA_FOR_SETTING_MANUAL_FOCUS_MODE_3M)) != FAILURE)
					{
						return SUCCESS;
					}
					else
					{
						sprintf(g_buff,"Error in writing the sensor register:%x\n",SENSOR_FOCUS_SELECTION_REG_ADDR_3M);
						debug_message(g_buff);
						return FAILURE;
					}
				}
				else
				{
					sprintf(g_buff,"Error in writing the sensor register:%x\n",SENSOR_FOCUS_SELECTION_REG_ADDR_3M);
					debug_message(g_buff);
					return FAILURE;
				}
			}
			else if (g_current_device == ecam_5M)
			{
				if((write_sensor_reg(SENSOR_CMD_ACK_REG_ADDR_5M, DATA_FOR_SETTING_CMD_ACK_5M)) != FAILURE)
				{
					if((write_sensor_reg(SENSOR_CMD_MAIN_REG_ADDR_5M, DATA_FOR_RELEASING_FOCUS_MODE)) != FAILURE)
					{
						read_sensor_reg(SENSOR_CMD_ACK_REG_ADDR_5M, &reg_value);
						while(reg_value != 0)
						{
							read_sensor_reg(SENSOR_CMD_ACK_REG_ADDR_5M, &reg_value);
						}
						if(reg_value == 0)
							return SUCCESS;
						else
							return FAILURE;
					}
					else
					{
						sprintf(g_buff,"Error in writing the sensor register:%x\n",SENSOR_CMD_MAIN_REG_ADDR_5M);
						debug_message(g_buff);
						return FAILURE;
					}
				}
				else
				{
					sprintf(g_buff,"Error in writing the sensor register:%x\n",SENSOR_CMD_ACK_REG_ADDR_5M);
					debug_message(g_buff);
					return FAILURE;
				}
			}
		}
		break;
		case AUTO_FOCUS_SELECT:
		{
			if(g_current_device == ecam_3M)
			{
				if((write_sensor_reg(SENSOR_FOCUS_SELECTION_REG_ADDR_3M, DATA_FOR_RELEASING_FOCUS_MODE)) != FAILURE)
				{
					if((write_sensor_reg(SENSOR_FOCUS_SELECTION_REG_ADDR_3M, DATA_FOR_SETTING_CNT_AUTO_FOCUS_MODE)) != FAILURE)
					{
						return SUCCESS;
					}
					else
					{
						sprintf(g_buff,"Error in writing the sensor register:%x\n",SENSOR_FOCUS_SELECTION_REG_ADDR_3M);
						debug_message(g_buff);
						return FAILURE;
					}
				}
				else
				{
					sprintf(g_buff,"Error in writing the sensor register:%x\n",SENSOR_FOCUS_SELECTION_REG_ADDR_3M);
					debug_message(g_buff);
					return FAILURE;
				}
			}
			else if (g_current_device == ecam_5M)
			{
				if((write_sensor_reg(SENSOR_CMD_ACK_REG_ADDR_5M, DATA_FOR_SETTING_CMD_ACK_5M)) != FAILURE)
				{
					if((write_sensor_reg(SENSOR_CMD_MAIN_REG_ADDR_5M, DATA_FOR_SETTING_CNT_AUTO_FOCUS_MODE)) != FAILURE)
					{
						read_sensor_reg(SENSOR_CMD_ACK_REG_ADDR_5M, &reg_value);
						while(reg_value != 0)
						{
							read_sensor_reg(SENSOR_CMD_ACK_REG_ADDR_5M, &reg_value);
						}
						if(reg_value == 0)
							return SUCCESS;
						else
							return FAILURE;
					}
					else
					{
						sprintf(g_buff,"Error in writing the sensor register:%x\n",SENSOR_CMD_MAIN_REG_ADDR_5M);
						debug_message(g_buff);
						return FAILURE;
					}
				}
				else
				{
					sprintf(g_buff,"Error in writing the sensor register:%x\n",SENSOR_CMD_ACK_REG_ADDR_5M);
					debug_message(g_buff);
					return FAILURE;
				}
			}

		}
		break;
		case SINGLE_TRIGGER_AUTO_FOCUS_SELECT:
		{
			if(g_current_device == ecam_3M)
			{
				if((write_sensor_reg(SENSOR_FOCUS_SELECTION_REG_ADDR_3M, DATA_FOR_RELEASING_FOCUS_MODE)) != FAILURE)
				{
					if((write_sensor_reg(SENSOR_FOCUS_SELECTION_REG_ADDR_3M, DATA_FOR_SETTING_SINGLE_TRIGGER_MODE)) != FAILURE)
					{
						return SUCCESS;
					}
					else
					{
						sprintf(g_buff,"Error in writing the sensor register:%x\n",SENSOR_FOCUS_SELECTION_REG_ADDR_3M);
						debug_message(g_buff);
						return FAILURE;
					}
				}
				else
				{
					sprintf(g_buff,"Error in writing the sensor register:%x\n",SENSOR_FOCUS_SELECTION_REG_ADDR_3M);
					debug_message(g_buff);
					return FAILURE;
				}
			}
			else if (g_current_device == ecam_5M)
			{
				if((write_sensor_reg(SENSOR_CMD_ACK_REG_ADDR_5M, DATA_FOR_SETTING_CMD_ACK_5M)) != FAILURE)
				{
					if((write_sensor_reg(SENSOR_CMD_MAIN_REG_ADDR_5M, DATA_FOR_SETTING_SINGLE_TRIGGER_MODE)) != FAILURE)
					{
						read_sensor_reg(SENSOR_CMD_ACK_REG_ADDR_5M, &reg_value);
						while(reg_value != 0)
						{
							read_sensor_reg(SENSOR_CMD_ACK_REG_ADDR_5M, &reg_value);						
						}
						if(reg_value == 0)
							return SUCCESS;
						else
							return FAILURE;
					}
					else
					{
						sprintf(g_buff,"Error in writing the sensor register:%x\n",SENSOR_CMD_MAIN_REG_ADDR_5M);
						debug_message(g_buff);
						return FAILURE;
					}
				}
				else
				{
					sprintf(g_buff,"Error in writing the sensor register:%x\n",SENSOR_CMD_ACK_REG_ADDR_5M);
					debug_message(g_buff);
					return FAILURE;
				}
			}

		}
		break;
		case GET_FOCUS_STATUS:
		{
			if(g_current_device == ecam_3M)
			{
				if((read_sensor_reg(SENSOR_FOCUS_STATUS_REG_ADDR_3M, &focus_status)) != FAILURE)
				{
					return (int)focus_status;
				}
				else
				{
					sprintf(g_buff,"Error in reading the sensor register:%x\n",sensor_addr);
					debug_message(g_buff);
					return FAILURE;
				}
			}
			else if (g_current_device == ecam_5M)
			{
				if((read_sensor_reg(SENSOR_FW_STATUS_REG_ADDR_5M, &focus_status)) != FAILURE)
				{
					return (int)focus_status;
				}
				else
				{
					sprintf(g_buff,"Error in reading the sensor register:%x\n",sensor_addr);
					debug_message(g_buff);
					return FAILURE;
				}
			}
		}
		break;
		default:
		{
			sprintf(g_buff,"FUNC %s(): LINE %d: %d is invalid option \n",__FUNCTION__,__LINE__,focus_option);
			debug_message(g_buff);
			return FAILURE;
		}
		break;		
	}// End of switch case
	return SUCCESS;
}


/************************************************************************************************************
 *  
 *  MODULE TYPE	:	FUNCTION					
 *  Name	:	open_device
 *  Parameter1	:	char *
 *  Parameter2	:	
 *  Returns	:	int
 *  Description	: 	to open the v4l2 camera driver
 ************************************************************************************************************/
int open_device(char *dev_name)
{
	struct stat st;							//to get the file system and file information
	if (-1 == stat (dev_name, &st)) 	
	{					                
		sprintf(g_buff, "%s Cannot identify %d, %s\n", dev_name, errno, strerror (errno));
		debug_message(g_buff);
		return FAILURE;
	}	 
	if (!S_ISCHR (st.st_mode)) 
	 {			
		sprintf(g_buff, "%s is no device\n", dev_name);			
		debug_message(g_buff);
		return FAILURE;
	 }
	 uvc_fd = open(dev_name,O_RDWR,0);			 //open the device as file in read only mode
	 if (-1 == uvc_fd) 
	 {
		debug_message("cann't open the driver\n");
		sprintf (g_buff, "Cannot open %s%d%s\n", dev_name, errno, strerror (errno));
		debug_message(g_buff);
		return FAILURE;
	 }
	 return SUCCESS;
}


/************************************************************************************************************
 *  
 *  MODULE TYPE	:	FUNCTION			
 *  Name	:	close_device
 *  Parameter1	:	void
 *  Parameter2	:	
 *  Returns	:	void
 *  Description	: 	to perform the close operation of v4l2 driver
 ************************************************************************************************************/
int close_device(char *dev_name)
{
	struct stat st;

	if (-1 == stat (dev_name, &st)) 	
	{					                
		sprintf(g_buff, "%s Cannot identify %d, %s\n", dev_name, errno, strerror (errno));
		debug_message(g_buff);
		return FAILURE;
	}
	if(uvc_fd > 0)
	{
		if(close(uvc_fd) < 0)
		{
			sprintf(g_buff, "Unable to close %s and return error no is %d, %s\n", dev_name, errno, strerror (errno));
			debug_message(g_buff);
			return FAILURE;
		}
		uvc_fd = -1;
	}
	return SUCCESS;
}


/************************************************************************************************************
 *  
 *  MODULE TYPE	:	FUNCTION					
 *  Name	:	open_logfile
 *  Parameter1	:	void
 *  Parameter2	:	
 *  Returns	:	void
 *  Description	: 	to open the log file for saving the debug message	
 ************************************************************************************************************/
void open_logfile()
{
	return;

	fstream = fopen("./e_CAM51_USB_log.txt", "a+");
	if(fstream == NULL)
	{
		printf("Unable to create the log file\n");
	}
	return;
}


/************************************************************************************************************
 *  
 *  MODULE TYPE	:	FUNCTION					
 *  Name	:	close_logfile
 *  Parameter1	:	void
 *  Parameter2	:	
 *  Returns	:	void
 *  Description	: 	to close the log file
 ************************************************************************************************************/
void close_logfile()
{
	return;
	
	if(fstream != NULL)
	{
		if(fclose(fstream) == EOF)
		{
			printf("The file e_cam33_log.txt was not closed\n");
		}
		else
		{
			fstream = NULL;
		}
	}
	return;
}


/************************************************************************************************************
 *  
 *  MODULE TYPE	:	FUNCTION					
 *  Name	:	debug_message
 *  Parameter1	:	char*
 *  Parameter2	:	
 *  Returns	:	void
 *  Description	: 	to write the log message in to the log file
 ************************************************************************************************************/
void debug_message(char *msg)
{
	if(g_debugmode)
	{
		__android_log_print(ANDROID_LOG_INFO,"TEST",msg);
	}
	else
	{
		if(fstream != NULL)
		{
			fprintf(fstream, "%s", msg);
		}		
	}	
	return;
}

