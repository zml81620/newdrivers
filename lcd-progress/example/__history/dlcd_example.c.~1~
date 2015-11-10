/*
 * dlcd_example.c
 *
 * 
 *
 * Copyright (C) 2009 Texas Instruments Incorporated - http://www.ti.com/ 
 * 
 * 
 *  Redistribution and use in source and binary forms, with or without 
 *  modification, are permitted provided that the following conditions 
 *  are met:
 *
 *    Redistributions of source code must retain the above copyright 
 *    notice, this list of conditions and the following disclaimer.
 *
 *    Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the 
 *    documentation and/or other materials provided with the   
 *    distribution.
 *
 *    Neither the name of Texas Instruments Incorporated nor the names of
 *    its contributors may be used to endorse or promote products derived
 *    from this software without specific prior written permission.
 *
 *  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS 
 *  "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT 
 *  LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
 *  A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT 
 *  OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, 
 *  SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT 
 *  LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
 *  DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
 *  THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT 
 *  (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE 
 *  OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
*/
/*******************************************************************************
 *	HEADER FILES
 */
#include <stdio.h>
#include <fcntl.h>
#include <string.h>
#include <getopt.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/time.h>
#include <sys/mman.h>
#include <sys/ioctl.h>
#include <fcntl.h>
#include <linux/fb.h>
#include <asm/types.h>
#include <time.h>

/* Kernel header file, prefix path comes from makefile */
#include <media/davinci/davinci_vpfe.h>
#include <video/davincifb_ioctl.h>
#include "osd16.h"
#include "test_8.h"

/*******************************************************************************
 *	LOCAL DEFINES
 */
#define CAPTURE_DEVICE		"/dev/video0"

#define WIDTH			720
#define HEIGHT			576

#define MIN_BUFFERS		2

#define UYVY_BLACK		0x10801080

/* Device parameters */
#define OSD0_DEVICE		"/dev/fb/0"
#define OSD1_DEVICE		"/dev/fb/2"
#define FBVID0_DEVICE		"/dev/fb/1"
#define FBVID1_DEVICE		"/dev/fb/3"

/* Function error codes */
#define SUCCESS			0
#define FAILURE			-1

/* Bits per pixel for video window */
#define YUV_422_BPP		16
#define BITMAP_BPP_8		8

#define DISPLAY_INTERFACE	"COMPOSITE"
#define DISPLAY_MODE		"NTSC"

#define round_32(width)		((((width) + 31) / 32) * 32 )

/* D1 screen dimensions */
#define VID0_WIDTH		720
#define VID0_HEIGHT		480
#define VID0_BPP		16
#define VID0_FRAME_SIZE		(VID0_WIDTH*VID0_HEIGHT)
#define VID0_VMODE		FB_VMODE_INTERLACED

#define VID1_WIDTH 		720
#define VID1_HEIGHT		480
#define VID1_BPP		16
#define VID1_FRAME_SIZE		(VID1_WIDTH*VID1_HEIGHT)
#define VID1_VMODE		FB_VMODE_INTERLACED

#define OSD0_BPP		4
#define	OSD0_WIDTH		(round_32(240*OSD0_BPP/8) * 8/OSD0_BPP)
#define	OSD0_HEIGHT		120
#define OSD0_FRAME_SIZE		(OSD0_WIDTH*OSD0_HEIGHT)
#define OSD0_VMODE		FB_VMODE_INTERLACED

#define OSD1_BPP		8
#define	OSD1_WIDTH		(round_32(240*OSD1_BPP/8) * 8/OSD1_BPP)
#define	OSD1_HEIGHT		120
#define OSD1_FRAME_SIZE		(OSD1_WIDTH*OSD1_HEIGHT)
#define OSD1_VMODE		FB_VMODE_INTERLACED

/* position */
#define	OSD0_XPOS		0
#define	OSD0_YPOS		0
#define	OSD1_XPOS		300
#define	OSD1_YPOS		250
#define	VID0_XPOS		0
#define	VID0_YPOS		0
#define	VID1_XPOS		0
#define	VID1_YPOS		0

/* Zoom Params */
#define	OSD0_HZOOM		0
#define	OSD0_VZOOM		0
#define	OSD1_HZOOM		0
#define	OSD1_VZOOM		0
#define	VID0_HZOOM		0
#define	VID0_VZOOM		0
#define	VID1_HZOOM		0
#define	VID1_VZOOM		0

/* OSD window blend factor */
#define OSD1_WINDOW_BF		0
#define OSD1_WINDOW_CK		0
#define OSD1_CK			0
#define OSD0_WINDOW_BF		3
#define OSD0_WINDOW_CK		0
#define OSD0_CK			0

#define VIDEO_NUM_BUFS		3
#define OSD_NUM_BUFS		2
#define RED_COLOR 		249
#define BLUE_COLOR 		140	//blue color
#define RAM_CLUT_IDX 		0xFF
#define BITMAP_COLOR		0x11

#define CURSOR_XPOS		100
#define CURSOR_YPOS		100
#define CURSOR_XRES		50
#define CURSOR_YRES		50
#define CURSOR_THICKNESS	1
#define CURSOR_COLOR		0xF9

#define ATTR_BLINK_INTERVAL	1
#define ATTR_BLEND_VAL 		0xaa

#define ATTRIB_MODE		"mode"
#define ATTRIB_OUTPUT		"output"
#define LCD_TEST_STD		LCD_STD_640x480

#define LOOP_COUNT		500

#define DEBUG
#ifdef DEBUG
#define DBGENTER  	printf("%s : Enter\n", __FUNCTION__);
#define DBGEXIT		printf("%s : Leave\n", __FUNCTION__);
#define PREV_DEBUG(x)	printf("DEBUG:%s:%s:%s\n",__FUNCTION__,__LINE__,x);
#else
#define DBGENTER
#define DBGEXIT
#define PREV_DEBUG(x)
#endif

#define CLEAR(x)	memset (&(x), 0, sizeof (x))

/*******************************************************************************
 *	ENUMERATED DATATYPE DEFINITIONS
 */
enum lcd_standard {
	LCD_STD_640x480,
	LCD_STD_640x400,
	LCD_STD_640x350,
	LCD_STD_MAX
};

/*******************************************************************************
 *	STRUCTURE DEFINITIONS
 */
struct buffer {
	void *start;
	size_t length;
};

struct vpbe_test_info {
	int vid0_width;
	int vid0_height;
	int vid0_bpp;
	int vid0_frame_size;
	int vid0_vmode;

	int vid1_width;
	int vid1_height;
	int vid1_bpp;
	int vid1_frame_size;
	int vid1_vmode;

	int osd0_bpp;
	int osd0_width;
	int osd0_height;
	int osd0_frame_size;
	int osd0_vmode;

	int osd1_bpp;
	int osd1_width;
	int osd1_height;
	int osd1_frame_size;
	int osd1_vmode;

	int osd0_xpos;
	int osd0_ypos;
	int osd1_xpos;
	int osd1_ypos;
	int vid0_xpos;
	int vid0_ypos;
	int vid1_xpos;
	int vid1_ypos;

	int osd0_hzoom;
	int osd0_vzoom;
	int osd1_hzoom;
	int osd1_vzoom;
	int vid0_hzoom;
	int vid0_vzoom;
	int vid1_hzoom;
	int vid1_vzoom;

	int osd1_window_bf;
	int osd1_window_ck;
	int osd1_ck;
	int osd0_window_bf;
	int osd0_window_ck;
	int osd0_ck;

	char display_interface[32];
	char display_mode[32];

	int osd0_coloridx;
	int osd1_coloridx;
	int ram_clut_idx;
	int bitmap_color;

	int cursor_xpos;
	int cursor_ypos;
	int cursor_xres;
	int cursor_yres;
	int cursor_thickness;
	int cursor_color;

	int attr_blink_interval;
	int attr_blend_val;
};

/*******************************************************************************
 *	FILE GLOBALS
 */
static struct vpbe_test_info test_data = {
	VID0_WIDTH,
	VID0_HEIGHT,
	VID0_BPP,
	VID0_FRAME_SIZE,
	VID0_VMODE,

	VID1_WIDTH,
	VID1_HEIGHT,
	VID1_BPP,
	VID1_FRAME_SIZE,
	VID1_VMODE,

	OSD0_BPP,
	OSD0_WIDTH,
	OSD0_HEIGHT,
	OSD0_FRAME_SIZE,
	OSD0_VMODE,

	OSD1_BPP,
	OSD1_WIDTH,
	OSD1_HEIGHT,
	OSD1_FRAME_SIZE,
	OSD1_VMODE,

	OSD0_XPOS,
	OSD0_YPOS,
	OSD1_XPOS,
	OSD1_YPOS,
	VID0_XPOS,
	VID0_YPOS,
	VID1_XPOS,
	VID1_YPOS,

	OSD0_HZOOM,
	OSD0_VZOOM,
	OSD1_HZOOM,
	OSD1_VZOOM,
	VID0_HZOOM,
	VID0_VZOOM,
	VID1_HZOOM,
	VID1_VZOOM,

	OSD1_WINDOW_BF,
	OSD1_WINDOW_CK,
	OSD1_CK,
	OSD0_WINDOW_BF,
	OSD0_WINDOW_CK,
	OSD0_CK,

	DISPLAY_INTERFACE,
	DISPLAY_MODE,

	BLUE_COLOR,
	RED_COLOR,
	RAM_CLUT_IDX,
	BITMAP_COLOR,

	CURSOR_XPOS,
	CURSOR_YPOS,
	CURSOR_XRES,
	CURSOR_YRES,
	CURSOR_THICKNESS,
	CURSOR_COLOR,

	ATTR_BLINK_INTERVAL,
	ATTR_BLEND_VAL,
};

static int fdCapture = -1;
static struct buffer *buffers = NULL;
static int nBuffers;
static int nWidthFinal;
static int nHeightFinal;
static int quit;
static char *vid0_display[VIDEO_NUM_BUFS];
static char *vid1_display[VIDEO_NUM_BUFS];
static char *osd0_display[OSD_NUM_BUFS];
static char *osd1_display[OSD_NUM_BUFS];
static int fd_vid0, fd_vid1, fd_osd0, fd_osd1;
static struct fb_var_screeninfo vid0_varInfo, vid1_varInfo, osd0_varInfo,
    osd1_varInfo, prev_vid0_var, prev_vid1_var, prev_osd0_var, prev_osd1_var;
static struct fb_fix_screeninfo vid0_fixInfo, vid1_fixInfo, osd0_fixInfo,
    osd1_fixInfo;
static int vid0_size, vid1_size, osd0_size, osd1_size;
static int stress_test = 1;
static int start_loopCnt = LOOP_COUNT;
static int rgb565_enable;
static int rgb565_enable_osd1;
static struct v4l2_cropcap cropcap;

/*******************************************************************************
 *	EXTERN VARIABLES
 */
extern int errno;

/*******************************************************************************
 *	LOCAL FUNCTION PROTOTYPES
 */
static void restore_config(void);
static void initialize_capture(void);
static int start_loop(void);
static int init_capture_device(void);
static int set_data_format(void);
static int init_capture_buffers(void);
static int start_streaming(void);
static int display_frame(char, void *);
static int flip_bitmap_buffers(int, int);
static int flip_video_buffers(int, int);
static int mmap_vid1(void);
static int mmap_vid0(void);
static int mmap_osd0(void);
static int mmap_osd1(void);
static int display_bitmap_osd0(void);
static int display_bitmap_osd1(void);
static int unmap_and_disable(char);
static int init_osd1_device(int, struct fb_var_screeninfo *);
static int init_osd0_device(int, struct fb_var_screeninfo *);
static int init_vid0_device(int, struct fb_var_screeninfo *);
static int init_vid1_device(int, struct fb_var_screeninfo *);
static int vpbe_UE_3(enum lcd_standard lcd_std);
static void close_all_windows(void);
static int open_all_windows(void);
static int change_sysfs_attrib(char *, char *);

/*******************************************************************************
 *	FUNCTION DEFINITIONS
 */
static void restore_config(void)
{
	DBGENTER;

	/* Set MODE */
	change_sysfs_attrib(ATTRIB_MODE, DISPLAY_MODE);
	change_sysfs_attrib(ATTRIB_OUTPUT, DISPLAY_INTERFACE);

	/* Restore the VID1 variable info */
	if (ioctl(fd_vid1, FBIOGET_VSCREENINFO, &vid1_varInfo) < 0) {
		printf("\nFailed FBIOGET_VSCREENINFO");
		return;
	}
	vid1_varInfo.bits_per_pixel = VID1_BPP;
	vid1_varInfo.vmode = FB_VMODE_INTERLACED;
	if (ioctl(fd_vid1, FBIOPUT_VSCREENINFO, &vid1_varInfo) < 0) {
		printf("\nFailed FBIOPUT_VSCREENINFO");
		return;
	}

	osd0_display[0] =
	    (char *)mmap(NULL,
			 osd0_fixInfo.line_length * osd0_varInfo.yres_virtual,
			 PROT_READ | PROT_WRITE, MAP_SHARED, fd_osd0, 0);
	osd1_display[0] =
	    (char *)mmap(NULL,
			 osd1_fixInfo.line_length * osd1_varInfo.yres_virtual,
			 PROT_READ | PROT_WRITE, MAP_SHARED, fd_osd1, 0);
	vid0_display[0] =
	    (char *)mmap(NULL,
			 vid0_fixInfo.line_length * vid0_varInfo.yres_virtual,
			 PROT_READ | PROT_WRITE, MAP_SHARED, fd_vid0, 0);
	vid1_display[0] =
	    (char *)mmap(NULL,
			 vid1_fixInfo.line_length * vid1_varInfo.yres_virtual,
			 PROT_READ | PROT_WRITE, MAP_SHARED, fd_vid1, 0);

	memset(osd0_display[0], 0x00,
	       osd0_fixInfo.line_length * osd0_varInfo.yres_virtual);
	memset(osd1_display[0], 0x66,
	       osd1_fixInfo.line_length * osd1_varInfo.yres_virtual);
	memset(vid0_display[0], 0x80,
	       vid0_fixInfo.line_length * vid0_varInfo.yres_virtual);
	memset(vid1_display[0], 0x80,
	       vid1_fixInfo.line_length * vid1_varInfo.yres_virtual);

	munmap(osd0_display[0],
	       osd0_fixInfo.line_length * osd0_varInfo.yres_virtual);
	munmap(osd1_display[0],
	       osd1_fixInfo.line_length * osd1_varInfo.yres_virtual);
	munmap(vid0_display[0],
	       vid0_fixInfo.line_length * vid0_varInfo.yres_virtual);
	munmap(vid1_display[0],
	       vid1_fixInfo.line_length * vid1_varInfo.yres_virtual);

	ioctl(fd_vid0, FBIOBLANK, 1);
	ioctl(fd_vid1, FBIOBLANK, 0);
	ioctl(fd_osd0, FBIOBLANK, 1);
	ioctl(fd_osd1, FBIOBLANK, 1);
	system("cat /dev/zero > /dev/fb/2 2> /dev/null");
	system("./putimage davinci.yuv");

	DBGEXIT;
}

/******************************************************************************/
static void initialize_capture(void)
{
	printf("initializing capture device\n");
	init_capture_device();
	printf("setting data format\n");
	set_data_format();
	printf("initializing capture buffers\n");
	init_capture_buffers();
	printf("initializing display device\n");
	start_streaming();
}

/******************************************************************************/
static int start_loop(void)
{
	struct v4l2_buffer buf;
	static int captFrmCnt = 0;
	char *ptrPlanar = NULL;

	ptrPlanar = (char *)calloc(1, nWidthFinal * nHeightFinal * 2);

	while (!quit) {
		fd_set fds;
		struct timeval tv;
		int r;


		FD_ZERO(&fds);
		FD_SET(fdCapture, &fds);

		/* Timeout */
		tv.tv_sec = 2;
		tv.tv_usec = 0;
		r = select(fdCapture + 1, &fds, NULL, NULL, &tv);
		if (-1 == r) {
			if (EINTR == errno)
				continue;
			printf("StartCameraCapture:select\n");
			return -1;
		}
		if (0 == r)
			continue;

		CLEAR(buf);
		buf.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
		buf.memory = V4L2_MEMORY_MMAP;

		/* determine ready buffer */
		if (-1 == ioctl(fdCapture, VIDIOC_DQBUF, &buf)) {
			if (EAGAIN == errno)
				continue;
			printf("StartCameraCaputre:ioctl:VIDIOC_DQBUF\n");
			return -1;
		}

		display_frame(VID0, buffers[buf.index].start);
		display_frame(VID1, buffers[buf.index].start);
		display_bitmap_osd0();
		display_bitmap_osd1();

		printf("time:%d    frame:%u\n", time(NULL), captFrmCnt++);

		/* requeue the buffer */
		if (-1 == ioctl(fdCapture, VIDIOC_QBUF, &buf)) {
			printf("StartCameraCaputre:ioctl:VIDIOC_QBUF\n");
		}
		if (stress_test) {
			start_loopCnt--;
			if (start_loopCnt == 0) {
				start_loopCnt = 50;
				break;
			}
		}
	}
	return SUCCESS;
}

/******************************************************************************/
static int init_capture_device(void)
{
	struct v4l2_capability cap;

	/* input-0 is selected by default, so no need to set it */
	if ((fdCapture = open(CAPTURE_DEVICE, O_RDWR | O_NONBLOCK, 0)) <= -1) {
		printf("InitDevice:open::\n");
		return -1;
	}

	/* is capture supported? */
	if (-1 == ioctl(fdCapture, VIDIOC_QUERYCAP, &cap)) {
		printf("InitDevice:ioctl:VIDIOC_QUERYCAP:\n");
		return -1;
	}

	if (!(cap.capabilities & V4L2_CAP_VIDEO_CAPTURE)) {
		printf("InitDevice:capture is not supported on:%s\n",
		       CAPTURE_DEVICE);
		return -1;
	}

	/* is MMAP-IO supported? */
	if (!(cap.capabilities & V4L2_CAP_STREAMING)) {
		printf("InitDevice:IO method MMAP is not supported on:%s\n",
		       CAPTURE_DEVICE);
		return -1;
	}

	/* select cropping as deault rectangle */
	cropcap.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;

	if (-1 == ioctl(fdCapture, VIDIOC_CROPCAP, &cropcap)) {
		printf("InitDevice:ioctl:VIDIOC_CROPCAP\n");
		/* ignore error */
	}

	printf("Default crop capbility bounds - %d %d %d %d"
	       " ; default - %d %d %d %d \n",
	       cropcap.bounds.left, cropcap.bounds.top,
	       cropcap.bounds.width, cropcap.bounds.height,
	       cropcap.defrect.left, cropcap.defrect.top,
	       cropcap.defrect.width, cropcap.defrect.height);
	return 0;
}

/******************************************************************************/
static int set_data_format(void)
{
	v4l2_std_id prev_std, cur_std;
	struct v4l2_format fmt;
	unsigned int min;

	cur_std = prev_std = VPFE_STD_AUTO;
	printf("set_data_format:setting std to auto select\n");
	if (-1 == ioctl(fdCapture, VIDIOC_S_STD, &cur_std)) {
		printf
		    ("set_data_format:unable to set standard automatically\n");
	}
	sleep(1);		/* wait until decoder is fully locked */
	if (-1 == ioctl(fdCapture, VIDIOC_QUERYSTD, &cur_std)) {
		printf("set_data_format:ioctl:VIDIOC_QUERYSTD:\n");
	}

	if (cur_std == V4L2_STD_NTSC)
		printf("Input video standard is NTSC.\n");
	else if (cur_std == V4L2_STD_PAL)
		printf("Input video standard is PAL.\n");
	else if (cur_std == V4L2_STD_PAL_M)
		printf("Input video standard is PAL-M.\n");
	else if (cur_std == V4L2_STD_PAL_N)
		printf("Input video standard is PAL-N.\n");
	else if (cur_std == V4L2_STD_SECAM)
		printf("Input video standard is SECAM.\n");
	else if (cur_std == V4L2_STD_PAL_60)
		printf("Input video standard to PAL60.\n");

	printf("set_data_format:setting data format\n");
	printf("set_data_format:requesting width:%d height:%d\n", WIDTH,
	       HEIGHT);
	CLEAR(fmt);
	fmt.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
	fmt.fmt.pix.width = WIDTH;
	fmt.fmt.pix.height = HEIGHT;
	fmt.fmt.pix.pixelformat = V4L2_PIX_FMT_UYVY;
	fmt.fmt.pix.field = V4L2_FIELD_INTERLACED;

	if (-1 == ioctl(fdCapture, VIDIOC_S_FMT, &fmt))
		printf("set_data_format:ioctl:VIDIOC_S_FMT\n");

	if (-1 == ioctl(fdCapture, VIDIOC_G_FMT, &fmt))
		printf("set_data_format:ioctl:VIDIOC_QUERYSTD:\n");

	nWidthFinal = fmt.fmt.pix.width;
	nHeightFinal = fmt.fmt.pix.height;

	printf("set_data_format:finally negotiated width:%d height:%d\n",
	       nWidthFinal, nHeightFinal);

	/* checking what is finally negotiated */
	min = fmt.fmt.pix.width * 2;
	if (fmt.fmt.pix.bytesperline < min) {
		printf
		    ("set_data_format:driver reports bytes_per_line:%d(bug)\n",
		     fmt.fmt.pix.bytesperline);
		/*correct it */
		fmt.fmt.pix.bytesperline = min;
	}

	min = fmt.fmt.pix.bytesperline * fmt.fmt.pix.height;
	if (fmt.fmt.pix.sizeimage < min) {
		printf("set_data_format:driver reports size:%d(bug)\n",
		       fmt.fmt.pix.sizeimage);

		/*correct it */
		fmt.fmt.pix.sizeimage = min;
	}

	printf("set_data_format:Finally negitaited width:%d height:%d\n",
	       nWidthFinal, nHeightFinal);

	return 0;
}

/******************************************************************************/
static int init_capture_buffers(void)
{
	struct v4l2_requestbuffers req;
	int nIndex = 0;

	CLEAR(req);
	req.count = MIN_BUFFERS;
	req.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
	req.memory = V4L2_MEMORY_MMAP;

	if (-1 == ioctl(fdCapture, VIDIOC_REQBUFS, &req)) {
		printf("init_capture_buffers:ioctl:VIDIOC_REQBUFS\n");
		return -1;
	}

	if (req.count < MIN_BUFFERS) {
		printf("init_capture_buffers only:"
		       "%d buffers avilable, can't proceed\n", req.count);
		return -1;
	}

	nBuffers = req.count;
	printf("device buffers:%d\n", req.count);
	buffers = (struct buffer *)calloc(req.count, sizeof(struct buffer));
	if (!buffers) {
		printf("init_capture_buffers:calloc:\n");
		return -1;
	}

	for (nIndex = 0; nIndex < req.count; ++nIndex) {
		struct v4l2_buffer buf;
		CLEAR(buf);
		buf.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
		buf.memory = V4L2_MEMORY_MMAP;
		buf.index = nIndex;

		if (-1 == ioctl(fdCapture, VIDIOC_QUERYBUF, &buf)) {
			printf("init_capture_buffers:VIDIOC_QUERYBUF:\n\n");
			return -1;
		}

		buffers[nIndex].length = buf.length;
		buffers[nIndex].start =
		    mmap(NULL, buf.length, PROT_READ | PROT_WRITE,
			 MAP_SHARED, fdCapture, buf.m.offset);

		printf("buffer:%d phy:%x mmap:%p length:%d\n", buf.index,
		       buf.m.offset, buffers[nIndex].start, buf.length);

		if (MAP_FAILED == buffers[nIndex].start) {
			printf("init_capture_buffers:mmap:\n");
			return -1;
		}
	}

	return 0;
}

/******************************************************************************/
static int start_streaming(void)
{
	int i = 0;
	enum v4l2_buf_type type;

	for (i = 0; i < nBuffers; i++) {
		struct v4l2_buffer buf;
		CLEAR(buf);
		buf.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
		buf.memory = V4L2_MEMORY_MMAP;
		buf.index = i;
		printf("Queing buffer:%d\n", i);

		if (-1 == ioctl(fdCapture, VIDIOC_QBUF, &buf))
			printf("start_streaming:ioctl:VIDIOC_QBUF:\n");
	}
	/* all done , get set go */
	type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
	if (-1 == ioctl(fdCapture, VIDIOC_STREAMON, &type))
		printf("start_streaming:ioctl:VIDIOC_STREAMON:\n");

	return 0;
}

/******************************************************************************/
static int display_bitmap_osd0(void)
{
	static unsigned int nDisplayIdx = 1;
	static unsigned int nWorkingIndex = 0;
	int y;
	char *dst;
	char *src;
	int fd;

	dst = osd0_display[nWorkingIndex];
	if (dst == NULL)
		return -1;
	fd = fd_osd0;
	if (rgb565_enable == 1) {	/* RGB565 */
		src = (char *)rgb16;
		for (y = 0; y < test_data.osd0_height; y++) {
			memcpy(dst, src, (test_data.osd0_width * 2));
			dst += osd0_fixInfo.line_length;
			src += (704 * 2);
		}
	} else if (rgb565_enable == 2) {	/* 8 bit bitmap */
		src = (char *)test_8;
		for (y = 0; y < test_data.osd0_height; y++) {
			memcpy(dst, src, (test_data.osd0_width));
			dst += osd0_fixInfo.line_length;
			src += (704);
		}
	} else			/* 1/2/4 bit bitmap and attribute */
		memset(dst, test_data.osd0_coloridx, osd0_size);

	nWorkingIndex = (nWorkingIndex + 1) % OSD_NUM_BUFS;
	nDisplayIdx = (nDisplayIdx + 1) % OSD_NUM_BUFS;

	if ((flip_bitmap_buffers(fd, nDisplayIdx)) < 0)
		return -1;
	return 0;
}

/******************************************************************************/
static int display_bitmap_osd1()
{
	static unsigned int nDisplayIdx = 1;
	static unsigned int nWorkingIndex = 0;
	int y;
	char *dst;
	char *src;
	int fd;

	dst = osd1_display[nWorkingIndex];
	if (dst == NULL)
		return -1;
	fd = fd_osd1;
	if (rgb565_enable_osd1 == 1) {	/* RGB565 */
		src = (char *)rgb16;
		for (y = 0; y < test_data.osd1_height; y++) {
			memcpy(dst, src, (test_data.osd1_width * 2));
			dst += osd1_fixInfo.line_length;
			src += (704 * 2);
		}
	} else if (rgb565_enable_osd1 == 2) {	/* 8 bit bitmap */
		src = (char *)test_8;
		for (y = 0; y < test_data.osd1_height; y++) {
			memcpy(dst, src, (test_data.osd1_width));
			dst += osd1_fixInfo.line_length;
			src += (704);
		}
	} else			/* 1/2/4 bit bitmap and attribute */
		memset(dst, test_data.osd1_coloridx, osd1_size);

	nWorkingIndex = (nWorkingIndex + 1) % OSD_NUM_BUFS;
	nDisplayIdx = (nDisplayIdx + 1) % OSD_NUM_BUFS;

	if ((flip_bitmap_buffers(fd, nDisplayIdx)) < 0)
		return -1;
	return 0;
}

/******************************************************************************/
static int display_frame(char id, void *ptr_buffer)
{
	static unsigned int nDisplayIdx = 0;
	static unsigned int nWorkingIndex = 1;
	int y;
	int xres, yres;
	char *dst;
	char *src;
	int fd;
	unsigned int line_length;

	if (id == VID0) {
		xres = test_data.vid0_width;
		yres = test_data.vid0_height;
		dst = vid0_display[nWorkingIndex];
		if (dst == NULL)
			return -1;
		fd = fd_vid0;
		line_length = vid0_fixInfo.line_length;
	}
	if (id == VID1) {
		xres = test_data.vid1_width;
		yres = test_data.vid1_height;
		dst = vid1_display[nWorkingIndex];
		if (dst == NULL)
			return -1;
		fd = fd_vid1;
		line_length = vid1_fixInfo.line_length;
	}
	src = ptr_buffer;
	for (y = 0; y < yres; y++) {
		memcpy(dst, src, (640 * 2));
		dst += line_length;
		src += (720 * 2);
	}
	nWorkingIndex = (nWorkingIndex + 1) % VIDEO_NUM_BUFS;
	nDisplayIdx = (nDisplayIdx + 1) % VIDEO_NUM_BUFS;
	if ((flip_video_buffers(fd, nDisplayIdx)) < 0)
		return -1;
	return 0;
}

/******************************************************************************/
static int flip_bitmap_buffers(int fd, int buf_index)
{
	struct fb_var_screeninfo var_info;

	if (ioctl(fd, FBIOGET_VSCREENINFO, &var_info) < -1) {
		printf("flip_bitmap_buffers:FBIOGET_VSCREENINFO\n\n");
		return -1;
	}

	var_info.yoffset = var_info.yres * buf_index;

	/* Swap the working buffer for the displayed buffer */
	if (ioctl(fd, FBIOPAN_DISPLAY, &var_info) < -1) {
		printf("flip_bitmap_buffers:FBIOPAN_DISPLAY\n\n");
		return -1;
	}

	return 0;
}

/******************************************************************************/
static int flip_video_buffers(int fd, int buf_index)
{
	int dummy;
	struct fb_var_screeninfo var_info;

	if (ioctl(fd, FBIOGET_VSCREENINFO, &var_info) < -1) {
		printf("flip_video_buffers:FBIOGET_VSCREENINFO\n\n");
		return -1;
	}

	var_info.yoffset = var_info.yres * buf_index;

	/* Wait for vertical sync */
	if (ioctl(fd_vid0, FBIO_WAITFORVSYNC, &dummy) < -1) {
		printf("Failed FBIO_WAITFORVSYNC\n");
		return -1;
	}

	/* Swap the working buffer for the displayed buffer */
	if (ioctl(fd, FBIOPAN_DISPLAY, &var_info) < -1) {
		printf("flip_video_buffers:FBIOPAN_DISPLAY\n");
		printf("\n");
		return -1;
	}

	return 0;
}

/******************************************************************************/
static int mmap_vid1(void)
{
	int i;
	vid1_size = vid1_fixInfo.line_length * vid1_varInfo.yres;
	/* Map the video0 buffers to user space */
	vid1_display[0] = (char *)mmap(NULL, vid1_size * VIDEO_NUM_BUFS,
				       PROT_READ | PROT_WRITE, MAP_SHARED,
				       fd_vid1, 0);

	if (vid1_display[0] == MAP_FAILED) {
		printf("\nFailed mmap on %s", FBVID1_DEVICE);
		return FAILURE;
	}

	for (i = 0; i < VIDEO_NUM_BUFS - 1; i++) {
		vid1_display[i + 1] = vid1_display[i] + vid1_size;
		printf("Display buffer %d mapped to address %#lx\n", i + 1,
		       (unsigned long)vid1_display[i + 1]);
	}
	return SUCCESS;
}

/******************************************************************************/
static int mmap_vid0(void)
{
	int i;
	vid0_size = vid0_fixInfo.line_length * vid0_varInfo.yres;
	/* Map the video0 buffers to user space */
	vid0_display[0] = (char *)mmap(NULL, vid0_size * VIDEO_NUM_BUFS,
				       PROT_READ | PROT_WRITE, MAP_SHARED,
				       fd_vid0, 0);

	if (vid0_display[0] == MAP_FAILED) {
		printf("\nFailed mmap on %s", FBVID0_DEVICE);
		return FAILURE;
	}

	for (i = 0; i < VIDEO_NUM_BUFS - 1; i++) {
		vid0_display[i + 1] = vid0_display[i] + vid0_size;
		printf("Display buffer %d mapped to address %#lx\n", i + 1,
		       (unsigned long)vid0_display[i + 1]);
	}
	return SUCCESS;
}

/******************************************************************************/
static int mmap_osd0(void)
{
	int i;
	osd0_size = osd0_fixInfo.line_length * osd0_varInfo.yres;
	/* Map the osd0 buffers to user space */
	osd0_display[0] = (char *)mmap(NULL, osd0_size * OSD_NUM_BUFS,
				       PROT_READ | PROT_WRITE, MAP_SHARED,
				       fd_osd0, 0);

	if (osd0_display[0] == MAP_FAILED) {
		printf("\nFailed mmap on %s", OSD0_DEVICE);
		return FAILURE;
	}

	for (i = 0; i < OSD_NUM_BUFS - 1; i++) {
		osd0_display[i + 1] = osd0_display[i] + osd0_size;
		printf("Display buffer %d mapped to address %#lx\n", i + 1,
		       (unsigned long)osd0_display[i + 1]);
	}
	return SUCCESS;
}

/******************************************************************************/
static int mmap_osd1(void)
{
	int i;
	osd1_size = osd1_fixInfo.line_length * osd1_varInfo.yres;
	/* Map the osd1 buffers to user space */
	osd1_display[0] = (char *)mmap(NULL,
				       osd1_size * OSD_NUM_BUFS,
				       PROT_READ | PROT_WRITE,
				       MAP_SHARED, fd_osd1, 0);

	if (osd1_display[0] == MAP_FAILED) {
		printf("\nFailed mmap on %s", OSD0_DEVICE);
		return FAILURE;
	}

	for (i = 0; i < OSD_NUM_BUFS - 1; i++) {
		osd1_display[i + 1] = osd1_display[i] + osd1_size;
		printf("Display buffer %d mapped to address %#lx\n", i + 1,
		       (unsigned long)osd1_display[i + 1]);
	}
	return SUCCESS;
}

/******************************************************************************/
static int unmap_and_disable(char id)
{

	switch (id) {
	case VID0:
		if (munmap(vid0_display[0], vid0_size * VIDEO_NUM_BUFS) == -1) {
			printf("\nFailed munmap on %s", FBVID1_DEVICE);
			return FAILURE;
		}
		break;
	case VID1:
		if (munmap(vid1_display[0], vid1_size * VIDEO_NUM_BUFS) == -1) {
			printf("\nFailed munmap on %s", FBVID1_DEVICE);
			return FAILURE;
		}
		break;
	case OSD0:
		if (munmap(osd0_display[0], osd0_size * OSD_NUM_BUFS) == -1) {
			printf("\nFailed munmap on %s", OSD0_DEVICE);
			return FAILURE;
		}
		break;
	case OSD1:
		if (munmap(osd1_display[0], osd1_size * OSD_NUM_BUFS) == -1) {
			printf("\nFailed munmap on %s", OSD1_DEVICE);
			return FAILURE;
		}
		break;
	default:
		break;
	}
	return SUCCESS;
}

/******************************************************************************/
static int init_osd1_device(int fd_osd1, struct fb_var_screeninfo *var_info)
{
	vpbe_window_position_t pos;

	if (ioctl(fd_osd1, FBIOGET_FSCREENINFO, &osd1_fixInfo) < 0) {
		printf("\nFailed FBIOGET_FSCREENINFO osd1");
		return FAILURE;
	}

	/* Get Existing var_screeninfo for osd1 window */
	if (ioctl(fd_osd1, FBIOGET_VSCREENINFO, var_info) < 0) {
		printf("\nFailed FBIOGET_VSCREENINFO");
		return FAILURE;
	}
	prev_osd1_var = *var_info;

	/* Modify the resolution and bpp as required */
	var_info->xres = test_data.osd1_width;
	var_info->yres = test_data.osd1_height;
	var_info->bits_per_pixel = test_data.osd1_bpp;
	var_info->vmode = test_data.osd1_vmode;
	var_info->xres_virtual = 720;
	var_info->yres_virtual = var_info->yres * OSD_NUM_BUFS;

	/* Set osd1 window format */
	if (ioctl(fd_osd1, FBIOPUT_VSCREENINFO, var_info) < 0) {
		printf("\nFailed FBIOPUT_VSCREENINFO");
		return FAILURE;
	}

	/* Set window position */
	pos.xpos = test_data.osd1_xpos;
	pos.ypos = test_data.osd1_ypos;

	if (ioctl(fd_osd1, FBIO_SETPOS, &pos) < 0) {
		printf("\nFailed  FBIO_SETPOS");
		return FAILURE;
	}

	if (ioctl(fd_osd1, FBIOGET_FSCREENINFO, &osd1_fixInfo) < 0) {
		printf("\nFailed FBIOGET_FSCREENINFO osd1");
		return FAILURE;
	}

	return SUCCESS;
}

/******************************************************************************/
static int init_osd0_device(int fd_osd0, struct fb_var_screeninfo *var_info)
{
	vpbe_window_position_t pos;

	if (ioctl(fd_osd0, FBIOGET_FSCREENINFO, &osd0_fixInfo) < 0) {
		printf("\nFailed FBIOGET_FSCREENINFO osd0");
		return FAILURE;
	}

	/* Get Existing var_screeninfo for osd0 window */
	if (ioctl(fd_osd0, FBIOGET_VSCREENINFO, var_info) < 0) {
		printf("\nFailed FBIOGET_VSCREENINFO");
		return FAILURE;
	}
	prev_osd0_var = *var_info;

	/* Modify the resolution and bpp as required */
	var_info->xres = test_data.osd0_width;
	var_info->yres = test_data.osd0_height;
	var_info->bits_per_pixel = test_data.osd0_bpp;
	var_info->vmode = test_data.osd0_vmode;
	var_info->xres_virtual = 720;
	var_info->yres_virtual = var_info->yres * OSD_NUM_BUFS;

	/* Set osd0 window format */
	if (ioctl(fd_osd0, FBIOPUT_VSCREENINFO, var_info) < 0) {
		printf("\nFailed FBIOPUT_VSCREENINFO");
		return FAILURE;
	}

	/* Set window position */
	pos.xpos = test_data.osd0_xpos;
	pos.ypos = test_data.osd0_ypos;

	if (ioctl(fd_osd0, FBIO_SETPOS, &pos) < 0) {
		printf("\nFailed  FBIO_SETPOS");
		return FAILURE;
	}

	return SUCCESS;
}

/******************************************************************************/
static int init_vid0_device(int fd, struct fb_var_screeninfo *var_info)
{
	vpbe_window_position_t pos;
	if (ioctl(fd, FBIOGET_FSCREENINFO, &vid0_fixInfo) < 0) {
		printf("\nFailed FBIOGET_FSCREENINFO vid0");
		return FAILURE;
	}

	/* Get Existing var_screeninfo for vid0 window */
	if (ioctl(fd, FBIOGET_VSCREENINFO, var_info) < 0) {
		printf("\nFailed FBIOGET_VSCREENINFO");
		return FAILURE;
	}

	prev_vid0_var = *var_info;

	/* Modify the resolution and bpp as required */
	var_info->xres = test_data.vid0_width;
	var_info->yres = test_data.vid0_height;
	var_info->bits_per_pixel = test_data.vid0_bpp;
	var_info->vmode = test_data.vid0_vmode;
	var_info->xres_virtual = 720;
	var_info->yres_virtual = var_info->yres * VIDEO_NUM_BUFS;

	/* Set vid0 window format */
	if (ioctl(fd, FBIOPUT_VSCREENINFO, var_info) < 0) {
		printf("\nFailed FBIOPUT_VSCREENINFO");
		return FAILURE;
	}

	/* Set window position */
	pos.xpos = test_data.vid0_xpos;
	pos.ypos = test_data.vid0_ypos;

	if (ioctl(fd_vid0, FBIO_SETPOS, &pos) < 0) {
		printf("\nFailed  FBIO_SETPOS");
		return FAILURE;
	}
	return SUCCESS;
}

/******************************************************************************/
static int init_vid1_device(int fd, struct fb_var_screeninfo *var_info)
{
	vpbe_window_position_t pos;

	if (ioctl(fd, FBIOGET_FSCREENINFO, &vid1_fixInfo) < 0) {
		printf("\nFailed FBIOGET_FSCREENINFO vid1");
		return FAILURE;
	}

	/* Get Existing var_screeninfo for vid1 window */
	if (ioctl(fd, FBIOGET_VSCREENINFO, var_info) < 0) {
		printf("\nFailed FBIOGET_VSCREENINFO");
		return FAILURE;
	}

	prev_vid1_var = *var_info;

	/* Modify the resolution and bpp as required */
	var_info->xres = test_data.vid1_width;
	var_info->yres = test_data.vid1_height;
	var_info->bits_per_pixel = test_data.vid1_bpp;
	var_info->vmode = test_data.vid1_vmode;
	var_info->xres_virtual = 720;
	var_info->yres_virtual = var_info->yres * VIDEO_NUM_BUFS;

	/* Set vid1 window format */
	if (ioctl(fd, FBIOPUT_VSCREENINFO, var_info) < 0) {
		printf("\nFailed FBIOPUT_VSCREENINFO");
		return FAILURE;
	}

	/* Set window position */
	pos.xpos = test_data.vid1_xpos;
	pos.ypos = test_data.vid1_ypos;

	if (ioctl(fd_vid1, FBIO_SETPOS, &pos) < 0) {
		printf("\nFailed  FBIO_SETPOS");
		return FAILURE;
	}

	return SUCCESS;
}

/******************************************************************************
 * Test all windows simultaneously on DLCD
 ******************************************************************************/
static int vpbe_UE_3(enum lcd_standard lcd_std)
{
	struct fb_cursor cursor_info;
	rgb565_enable = 1;	/* To display 16 bit image on osd0 */
	rgb565_enable_osd1 = 2;	/* To display 8 bit image on osd1 */
	DBGENTER;

	initialize_capture();

	test_data.vid0_width = 640;
	test_data.vid0_bpp = 16;
	test_data.vid0_xpos = 0;
	test_data.vid0_ypos = 0;
	test_data.vid0_vmode = FB_VMODE_NONINTERLACED;

	test_data.vid1_width = 352;
	test_data.vid1_height = 240;
	test_data.vid1_bpp = 16;
	test_data.vid1_vmode = FB_VMODE_NONINTERLACED;
	test_data.vid1_xpos = 32;
	test_data.vid1_ypos = 32;

	test_data.osd0_bpp = 16;
	test_data.osd0_xpos = 0;
	test_data.osd0_ypos = 0;
	test_data.osd0_width = 150;
	test_data.osd0_height = 150;
	test_data.osd0_vmode = FB_VMODE_NONINTERLACED;
	test_data.osd0_coloridx = BLUE_COLOR;

	test_data.osd1_bpp = 8;
	test_data.osd1_xpos = 550;
	test_data.osd1_width = 75;
	test_data.osd1_height = 75;
	test_data.osd1_vmode = FB_VMODE_NONINTERLACED;

	strcpy(test_data.display_interface, "LCD");

	switch (lcd_std) {
	case LCD_STD_640x480:
		test_data.vid0_height = 480;
		test_data.osd1_ypos = 380;
		strcpy(test_data.display_mode, "640x480");
		break;
	case LCD_STD_640x400:
		test_data.vid0_height = 400;
		test_data.osd1_ypos = 300;
		strcpy(test_data.display_mode, "640x400");
		break;
	case LCD_STD_640x350:
		test_data.vid0_height = 350;
		test_data.osd1_ypos = 250;
		strcpy(test_data.display_mode, "640x350");
		break;
	default:
		break;
	}

	change_sysfs_attrib(ATTRIB_OUTPUT, test_data.display_interface);
	change_sysfs_attrib(ATTRIB_MODE, test_data.display_mode);

	// Initialize all the windows
	if ((init_vid0_device(fd_vid0, &vid0_varInfo)) < 0) {
		printf("\nFailed to init vid0 window ");
		return FAILURE;
	}
	if ((init_vid1_device(fd_vid1, &vid1_varInfo)) < 0) {
		printf("\nFailed to init vid1 window ");
		return FAILURE;
	}
	if ((init_osd0_device(fd_osd0, &osd0_varInfo)) < 0) {
		printf("\nFailed to init osd0 window ");
		return FAILURE;
	}
	if ((init_osd1_device(fd_osd1, &osd1_varInfo)) < 0) {
		printf("\nFailed to init osd1 window ");
		return FAILURE;
	}
	// Disable unwanted windows and enable needed windows only
	if (ioctl(fd_vid0, FBIOBLANK, 0) < 0) {
		printf("\nFailed to enable VID0");
		close(fd_vid0);
		return FAILURE;
	}
	if (ioctl(fd_vid1, FBIOBLANK, 1) < 0) {
		printf("\nFailed to disable VID1");
		close(fd_vid1);
		return FAILURE;
	}
	if (ioctl(fd_osd0, FBIOBLANK, 0) < 0) {
		printf("\nFailed to enable OSD0");
		close(fd_osd0);
		return FAILURE;
	}
	if (ioctl(fd_osd1, FBIOBLANK, 0) < 0) {
		printf("\nFailed to enable OSD1");
		close(fd_osd1);
		return FAILURE;
	}

	if (FAILURE == mmap_vid0())
		return FAILURE;
	if (FAILURE == mmap_vid1())
		return FAILURE;
	if (FAILURE == mmap_osd0())
		return FAILURE;
	if (FAILURE == mmap_osd1())
		return FAILURE;

	cursor_info.enable = VPBE_ENABLE;
	cursor_info.image.dx = CURSOR_XPOS;
	cursor_info.image.dy = CURSOR_YPOS;
	cursor_info.image.width = CURSOR_XRES;
	cursor_info.image.height = CURSOR_YRES;
	cursor_info.image.depth = CURSOR_THICKNESS;
	cursor_info.image.fg_color = CURSOR_COLOR;

	if (ioctl(fd_vid1, FBIO_SET_CURSOR, &cursor_info) < 0) {
		printf("\nFailed  FBIO_SET_CURSOR");
		return FAILURE;
	}

	start_loop();

	/* unmap video buffers */
	if (unmap_and_disable(VID0) < 0)
		return FAILURE;
	if (unmap_and_disable(VID1) < 0)
		return FAILURE;
	if (unmap_and_disable(OSD0) < 0)
		return FAILURE;
	if (unmap_and_disable(OSD1) < 0)
		return FAILURE;

	DBGEXIT;
	return SUCCESS;
}

/******************************************************************************/
static void close_all_windows(void)
{
	if (fd_vid0)
		close(fd_vid0);
	if (fd_vid1)
		close(fd_vid1);
	if (fd_osd0)
		close(fd_osd0);
	if (fd_osd1)
		close(fd_osd1);
}

/******************************************************************************/
static int open_all_windows(void)
{
	if ((fd_vid0 = open(FBVID0_DEVICE, O_RDWR)) < 0)
		goto open_all_exit;
	if ((fd_vid1 = open(FBVID1_DEVICE, O_RDWR)) < 0)
		goto open_all_exit;
	if ((fd_osd0 = open(OSD0_DEVICE, O_RDWR)) < 0)
		goto open_all_exit;
	if ((fd_osd1 = open(OSD1_DEVICE, O_RDWR)) < 0)
		goto open_all_exit;
	return SUCCESS;
      open_all_exit:
	close_all_windows();
	return FAILURE;
}

/*******************************************************************************
 *	Function will use the SysFS interface to change the output and mode
 */
static int change_sysfs_attrib(char *attribute, char *value)
{
	int sysfd = -1;
	char init_val[32];
	char attrib_tag[128];

	strcpy(attrib_tag, "/sys/class/davinci_display/ch0/");
	strcat(attrib_tag, attribute);

	sysfd = open(attrib_tag, O_RDWR);
	if (!sysfd) {
		printf("Error: cannot open %d\n", sysfd);
		return FAILURE;
	}
	printf("%s was opened successfully\n", attrib_tag);

	read(sysfd, init_val, 32);
	lseek(sysfd, 0, SEEK_SET);
	printf("Current %s value is %s\n", attribute, init_val);

	write(sysfd, value, 1 + strlen(value));
	lseek(sysfd, 0, SEEK_SET);

	memset(init_val, '\0', 32);
	read(sysfd, init_val, 32);
	lseek(sysfd, 0, SEEK_SET);
	printf("Changed %s to %s\n", attribute, init_val);

	close(sysfd);
	return SUCCESS;
}

/******************************************************************************/
/* main function */
int main(int argc, char *argv[])
{
	int ret = 0;

	DBGENTER;
	if (open_all_windows() != 0) {
		printf("Test fail\n");
		return 0;
	}

	ret = vpbe_UE_3(LCD_TEST_STD);
	restore_config();
	close_all_windows();
	DBGEXIT;
	return ret;
}
