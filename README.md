=======
nuiCellCAM
==========

Android/Java code for USB Camera 

Eclipse project for android to use USB Webcam.

To run this application, the following conditions should be satisfied.

1) The kernel is V4L2 enabled, e.g.,

 CONFIG_VIDEO_DEV=y

 CONFIG_VIDEO_V4L2_COMMON=y

 CONFIG_VIDEO_MEDIA=y

 CONFIG_USB_VIDEO_CLASS=y

 CONFIG_V4L_USB_DRIVERS=y

 CONFIG_USB_VIDEO_CLASS_INPUT_EVDEV=y

2) The permission of /dev/video0 is set 0666 in /ueventd.xxxx.rc

3) USB WebCam is UVC camera, and it supports YUYV format.

Supported platform : Iconia Tab A500.

 This application will also work on V4L2-enabled pandaboard and beagleboard.

Galaxy SII
----------
To get it working on a SGS2 you have to do the following:

1) /dev/video0-3 are used the WebCam would be found on /dev/video4 (change it in the code)

2) May be this problem occours not with all OTG's but I have to disable autosuspend for USB:

	echo -1 > /sys/module/usbcore/parameters/autosuspend

3) I didn't get it to work till now. For further debugging set uvcvideo trace to maximum:

	echo 0xffff > /sys/module/uvcvideo/parameters/trace

4) Currently I stuck on the following logcat:

	I/TEST    ( 4677): *****************************************
	I/TEST    ( 4677):    ***UVC WEBCAM APPLICATION STARTS***
	I/TEST    ( 4677):        AT Mon Jan 24 22:54:00 2000
	I/TEST    ( 4677): *****************************************
	I/TEST    ( 4677): Adding control for ORIENTATION CTRL
	I/TEST    ( 4677): Control for ORIENTATION CTRL already exists
	I/TEST    ( 4677): Adding control for GPIO CTRL
	I/TEST    ( 4677): Control for GPIO CTRL already exists
	I/TEST    ( 4677): Adding control for EEPROM ACCESS CTRL
	I/TEST    ( 4677): Control for EEPROM ACCESS CTRL already exists
	I/TEST    ( 4677): Adding control for e-CAM FW VERSION CTRL
	I/TEST    ( 4677): Control for e-CAM FW VERSION CTRL already exists
	I/TEST    ( 4677): Mapping controls for ORIENTATION CTRL
	I/TEST    ( 4677):  FUNC initUvcCtrls(): LINE 789: Success in Mapping the UVC Control for ORIENTATION CTRL
	I/TEST    ( 4677): Mapping controls for GPIO CTRL
	I/dalvikvm-heap( 2309): Grow heap (frag case) to 13.559MB for 2828816-byte allocation
	I/TEST    ( 4677):  FUNC initUvcCtrls(): LINE 789: Success in Mapping the UVC Control for GPIO CTRL
	I/TEST    ( 4677): Mapping controls for EEPROM ACCESS CTRL
	I/TEST    ( 4677):  FUNC initUvcCtrls(): LINE 789: Success in Mapping the UVC Control for EEPROM ACCESS CTRL
	I/TEST    ( 4677): Mapping controls for e-CAM FW VERSION CTRL
	I/TEST    ( 4677):  FUNC initUvcCtrls(): LINE 789: Success in Mapping the UVC Control for e-CAM FW VERSION CTRL
	I/TEST    ( 4677): FUNC sensor_mode_selection(): LINE 843: current control value is 9 (return value 0)
	I/TEST    ( 4677): Adding control for SENSOR CTRL
	I/TEST    ( 4677): Control for SENSOR CTRL already exists
	I/TEST    ( 4677): FUNC InitLibrary(): LINE 142: Success in Mapping the UVC Control for SENSOR CTRL
	D/dalvikvm( 2309): GC_FOR_ALLOC freed 0K, 26% free 13704K/18408K, paused 16ms, total 16ms
	D/dalvikvm( 2309): GC_CONCURRENT freed 3000K, 42% free 10705K/18408K, paused 3ms+4ms, total 58ms
	I/QuickSettingsController( 2309): Default tiles being loaded
	I/QuickSettingsController( 2309): Tiles list: toggleUser|toggleBrightness|toggleSettings|toggleWifi|toggleMobileData|toggleBattery|toggleAirplane|toggleBluetooth
	E/TEST    ( 4677): VIDIOC_STREAMON error 1, Operation not permitted
	E/TEST    ( 4677): device resetted
	D/WebCam  ( 4677): surfaceChanged
	D/BluetoothAdapter( 2309): 1093816496: getState() :  mService = null. Returning STATE_OFF
	D/BluetoothAdapter( 2309): 1093816496: getState() :  mService = null. Returning STATE_OFF
	D/BluetoothAdapter( 2309): 1093816496: getState() :  mService = null. Returning STATE_OFF
	D/BluetoothAdapter( 2309): 1093816496: getState() :  mService = null. Returning STATE_OFF
	D/BluetoothAdapter( 2309): 1093816496: getState() :  mService = null. Returning STATE_OFF
	D/BluetoothAdapter( 2309): 1093816496: getState() :  mService = null. Returning STATE_OFF
	I/ActivityManager( 2196): Displayed com.camera.simplewebcam/ch.nuicell.ecam.Main: +444ms
	W/IInputConnectionWrapper( 2488): showStatusIcon on inactive InputConnection
	D/BluetoothAdapter( 2309): 1093816496: getState() :  mService = null. Returning STATE_OFF
	V/PhoneStatusBar( 2309): setLightsOn(true)
	D/UsbTetherTile( 2309): ACTION_USB_STATE
	D/dalvikvm( 2196): GC_EXPLICIT freed 352K, 26% free 7181K/9672K, paused 17ms+10ms, total 205ms
	D/dalvikvm( 2196): GC_FOR_ALLOC freed 268K, 27% free 7133K/9672K, paused 47ms, total 47ms
	D/dalvikvm( 2196): GC_FOR_ALLOC freed 215K, 27% free 7134K/9672K, paused 138ms, total 139ms
	I/ActivityManager( 2196): START u0 {act=com.android.systemui.recent.action.TOGGLE_RECENTS flg=0x10800000 cmp=com.android.systemui/.recent.RecentsActivity (has extras)
	D/dalvikvm( 2196): GC_FOR_ALLOC freed 213K, 27% free 7139K/9672K, paused 54ms, total 54ms
	D/dalvikvm( 2196): GC_FOR_ALLOC freed 2K, 25% free 7350K/9672K, paused 47ms, total 47ms
	I/dalvikvm-heap( 2196): Grow heap (frag case) to 7.558MB for 214528-byte allocation
	D/dalvikvm( 2196): GC_FOR_ALLOC freed 0K, 24% free 7560K/9884K, paused 53ms, total 53ms
	I/ActivityManager( 2196): Config changes=1480 {1.0 ?mcc?mnc en_US ldltr sw320dp w320dp h508dp 240dpi nrml long port finger -keyb/v/h dpad/v s.8 themeResource=null}
	I/InputReader( 2196): Reconfiguring input devices.  changes=0x00000004
	I/InputReader( 2196): Device reconfigured: id=6, name='sec_touchscreen', size 480x800, orientation 0, mode 1, display id 0
	I/InputReader( 2196): Reconfiguring input devices.  changes=0x00000004
	I/InputReader( 2196): Device reconfigured: id=6, name='sec_touchscreen', size 480x800, orientation 0, mode 1, display id 0
	D/WebCam  ( 4677): surfaceDestroyed
	E/TEST    ( 4677): VIDIOC_STREAMOFF error 9, Bad file number
	E/TEST    ( 4677): munmap error 22, Invalid argument
	E/TEST    ( 4677): close error 9, Bad file number

And on the following dmesg:

	<7>[18179.273760] c0 uvcvideo: uvc_v4l2_ioctl(unknown ioctl 'U', dir=rw, #2 (0xc0585502))
	<7>[18179.274335] c0 uvcvideo: XU control a917c75d-4119-11da-ae0e-000d56ac7b4c/5 queried: len 4, flags { get 1 set 1 auto 0 }.
	<7>[18179.274431] c0 uvcvideo: Added control a917c75d-4119-11da-ae0e-000d56ac7b4c/5 to device 1 entity 4
	<7>[18179.274521] c0 uvcvideo: Adding mapping 'EEPROM ACCESS CTRL' to control a917c75d-4119-11da-ae0e-000d56ac7b4c/5.
	<7>[18179.274632] c0 uvcvideo: uvc_v4l2_ioctl(unknown ioctl 'U', dir=rw, #2 (0xc0585502))
	<7>[18179.275193] c0 uvcvideo: XU control a917c75d-4119-11da-ae0e-000d56ac7b4c/7 queried: len 4, flags { get 1 set 0 auto 0 }.
	<7>[18179.275291] c0 uvcvideo: Added control a917c75d-4119-11da-ae0e-000d56ac7b4c/7 to device 1 entity 4
	<7>[18179.275383] c0 uvcvideo: Adding mapping 'e-CAM FW VERSION CTRL' to control a917c75d-4119-11da-ae0e-000d56ac7b4c/7.
	<7>[18179.275499] c0 uvcvideo: uvc_v4l2_ioctl(unknown ioctl 'U', dir=-w, #4 (0x40085504))
	<6>[18179.275687] c0 OTG compare_ed(384): update ed 8 (0x8)
	<7>[18179.276028] c0 uvcvideo: uvc_v4l2_ioctl(unknown ioctl 'U', dir=rw, #3 (0xc0085503))
	<6>[18179.276211] c0 OTG compare_ed(384): update ed 8 (0x8)
	<7>[18179.276494] c0 uvcvideo: uvc_v4l2_ioctl(unknown ioctl 'U', dir=-w, #1 (0x40185501))
	<7>[18179.276677] c0 uvcvideo: uvc_v4l2_ioctl(unknown ioctl 'U', dir=rw, #2 (0xc0585502))
	<7>[18179.277272] c0 uvcvideo: XU control a917c75d-4119-11da-ae0e-000d56ac7b4c/16 queried: len 9, flags { get 1 set 1 auto 0 }.
	<7>[18179.277371] c0 uvcvideo: Added control a917c75d-4119-11da-ae0e-000d56ac7b4c/16 to device 1 entity 4
	<7>[18179.277419] c0 uvcvideo: Adding mapping 'SENSOR CTRL' to control a917c75d-4119-11da-ae0e-000d56ac7b4c/16.
	<7>[18179.277531] c0 uvcvideo: uvc_v4l2_ioctl(VIDIOC_QUERYCAP)
	<7>[18179.277702] c0 uvcvideo: uvc_v4l2_ioctl(VIDIOC_CROPCAP)
	<7>[18179.277834] c0 uvcvideo: uvc_v4l2_ioctl(VIDIOC_S_CROP)
	<7>[18179.278118] c0 uvcvideo: uvc_v4l2_ioctl(VIDIOC_S_FMT)
	<7>[18179.278261] c0 uvcvideo: Trying format 0x56595559 (YUYV): 2592x1944.
	<7>[18179.278353] c0 uvcvideo: Using default frame interval 400000.0 us (2.5 fps).
	<6>[18179.278410] c0 OTG compare_ed(384): update ed 8 (0x8)
	<6>[18179.279144] c0 OTG compare_ed(384): update ed 8 (0x8)
	<6>[18179.279935] c0 OTG compare_ed(384): update ed 8 (0x8)
	<6>[18179.280664] c0 OTG compare_ed(384): update ed 8 (0x8)
	<7>[18179.281062] c0 uvcvideo: uvc_v4l2_ioctl(VIDIOC_REQBUFS)
	<7>[18179.286807] c0 uvcvideo: uvc_v4l2_ioctl(VIDIOC_QUERYBUF)
	<7>[18179.287033] c0 uvcvideo: uvc_v4l2_mmap
	<7>[18179.328178] c0 uvcvideo: uvc_v4l2_ioctl(VIDIOC_QUERYBUF)
	<7>[18179.328418] c0 uvcvideo: uvc_v4l2_mmap
	<7>[18179.357857] c1 uvcvideo: uvc_v4l2_ioctl(VIDIOC_QUERYBUF)
	<7>[18179.358032] c1 uvcvideo: uvc_v4l2_mmap
	<7>[18179.382703] c1 uvcvideo: uvc_v4l2_ioctl(VIDIOC_QUERYBUF)
	<7>[18179.382844] c1 uvcvideo: uvc_v4l2_mmap
	<7>[18179.397128] c1 uvcvideo: uvc_v4l2_ioctl(VIDIOC_QBUF)
	<7>[18179.397264] c1 uvcvideo: Queuing buffer 0.
	<7>[18179.397308] c1 uvcvideo: uvc_v4l2_ioctl(VIDIOC_QBUF)
	<7>[18179.397458] c1 uvcvideo: Queuing buffer 1.
	<7>[18179.397502] c1 uvcvideo: uvc_v4l2_ioctl(VIDIOC_QBUF)
	<7>[18179.397648] c1 uvcvideo: Queuing buffer 2.
	<7>[18179.397691] c1 uvcvideo: uvc_v4l2_ioctl(VIDIOC_QBUF)
	<7>[18179.397806] c1 uvcvideo: Queuing buffer 3.
	<7>[18179.397880] c1 uvcvideo: uvc_v4l2_ioctl(VIDIOC_STREAMON)
	<6>[18179.398020] c1 OTG compare_ed(384): update ed 8 (0x8)
	<7>[18179.398714] c1 uvcvideo: Device requested 3072 B/frame bandwidth.
	<7>[18179.398767] c1 uvcvideo: Selecting alternate setting 7 (3072 B/frame bandwidth).
	<7>[18179.402255] c1 uvcvideo: Allocated 5 URB buffers of 32x3072 bytes each.
	<3>[18179.402357] c1 uvcvideo: Failed to submit URB 0 (-1).
	<7>[18179.402519] c1 uvcvideo: uvc_v4l2_ioctl(VIDIOC_STREAMOFF)
	<7>[18179.407315] c1 uvcvideo: uvc_v4l2_release
