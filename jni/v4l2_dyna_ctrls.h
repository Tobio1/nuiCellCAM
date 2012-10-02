/*******************************************************************************#
#           guvcview              http://guvcview.berlios.de                    #
#                                                                               #
#           Paulo Assis <pj.assis@gmail.com>                                    #
#                                                                               #
# This program is free software; you can redistribute it and/or modify          #
# it under the terms of the GNU General Public License as published by          #
# the Free Software Foundation; either version 2 of the License, or             #
# (at your option) any later version.                                           #
#                                                                               #
# This program is distributed in the hope that it will be useful,               #
# but WITHOUT ANY WARRANTY; without even the implied warranty of                #
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the                 #
# GNU General Public License for more details.                                  #
#                                                                               #
# You should have received a copy of the GNU General Public License             #
# along with this program; if not, write to the Free Software                   #
# Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA     #
#                                                                               #
********************************************************************************/

#ifndef V4L2_DYNA_CTRLS_H
#define V4L2_DYNA_CTRLS_H

/*
 * Dynamic controls
 */

#define UVC_CTRL_DATA_TYPE_RAW		0
#define UVC_CTRL_DATA_TYPE_SIGNED	1
#define UVC_CTRL_DATA_TYPE_UNSIGNED	2
#define UVC_CTRL_DATA_TYPE_BOOLEAN	3
#define UVC_CTRL_DATA_TYPE_ENUM		4
#define UVC_CTRL_DATA_TYPE_BITMASK	5

#define XU_HW_CONTROL_LED1			1
#define XU_MOTORCONTROL_PANTILT_RELATIVE	1
#define XU_MOTORCONTROL_PANTILT_RESET		2
#define XU_MOTORCONTROL_FOCUS			3
#define XU_COLOR_PROCESSING_DISABLE		5
#define XU_RAW_DATA_BITS_PER_PIXEL		8

#define UVC_CONTROL_SET_CUR	(1 << 0)
#define UVC_CONTROL_GET_CUR	(1 << 1)
#define UVC_CONTROL_GET_MIN	(1 << 2)
#define UVC_CONTROL_GET_MAX	(1 << 3)
#define UVC_CONTROL_GET_RES	(1 << 4)
#define UVC_CONTROL_GET_DEF	(1 << 5)
/* Control should be saved at suspend and restored at resume. */
#define UVC_CONTROL_RESTORE	(1 << 6)
/* Control can be updated by the camera. */
#define UVC_CONTROL_AUTO_UPDATE	(1 << 7)

#define UVC_CONTROL_GET_RANGE   (UVC_CONTROL_GET_CUR | UVC_CONTROL_GET_MIN | \
                                 UVC_CONTROL_GET_MAX | UVC_CONTROL_GET_RES | \
                                 UVC_CONTROL_GET_DEF)

struct uvc_xu_control_info {
	__u8 entity[16];
	__u8 index;
	__u8 selector;
	__u16 size;
	__u32 flags;
};

struct uvc_xu_control_mapping {
         __u32 id;
         __u8 name[32];
         __u8 entity[16];
         __u8 selector;
 
         __u8 size;
         __u8 offset;
         __u32 v4l2_type;
         __u32 data_type;
 
         struct uvc_menu_info __user *menu_info;
         __u32 menu_count;
 
         __u32 reserved[4];
 };

struct uvc_xu_control {
	__u8 unit;
	__u8 selector;
	__u16 size;
	__u8 *data;
};

struct uvc_xu_control_query {
         __u8 unit;
         __u8 selector;
         __u8 query;
         __u16 size;
         __u8 __user *data;
};

#define UVCIOC_CTRL_ADD		_IOW('U', 1, struct uvc_xu_control_info)
#define UVCIOC_CTRL_MAP		_IOWR('U', 2, struct uvc_xu_control_mapping)
#define UVCIOC_CTRL_GET		_IOWR('U', 3, struct uvc_xu_control)
#define UVCIOC_CTRL_SET		_IOW('U', 4, struct uvc_xu_control)

// for newer kernels 2.6.40+
//#define UVCIOC_CTRL_MAP   _IOWR('u', 0x20, struct uvc_xu_control_mapping)
//#define UVCIOC_CTRL_QUERY   _IOWR('u', 0x21, struct uvc_xu_control_query)

#define GPI6_STATE		(1<<0)			//AF_KEY
#define GPI7_STATE		(1<<1)			//LED_KEY
#define GPI3_STATE		(1<<2)			//FLIP_KEY
#define GPO8_STATE		(1<<3)			//GPO KEY

#define SUCCESS  1
#define FAILURE -1
#define SET	 1
#define CLEAR	 0

enum e_con_device
{
	ecam_3M=1,
	ecam_5M
};

enum focus_status_options
{
	AF_CONT_FROZEN = 1,
	AF_CONT_FOCUSED,
	MANUAL_FOCUS,
	AF_CONT_FOCUSING,
	AF_SINGLE_TRIGGER_FOCUSING,
	AF_SINGLE_TRIGGER_FOCUSED,
	AF_SINGLE_TRIGGER_FROZEN
};

enum focus_mode_options
{
	AF_CONT_FREEZE_MODE = 1,
	AF_CONT_UNFREEZE_MODE,
	MANUAL_FOCUS_MODE,
	AUTO_FOCUS_MODE,
	SINGLE_TRIG_AF_MODE
};


#define UVC_RC_UNDEFINED                                0x00
#define UVC_SET_CUR                                     0x01
#define UVC_GET_CUR                                     0x81
#define UVC_GET_MIN                                     0x82
#define UVC_GET_MAX                                     0x83
#define UVC_GET_RES                                     0x84
#define UVC_GET_LEN                                     0x85
#define UVC_GET_INFO                                    0x86
#define UVC_GET_DEF                                     0x87


#endif
