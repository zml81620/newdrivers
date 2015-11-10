#include <linux/module.h>
#include <linux/init.h>
#include <linux/kernel.h>
#include <linux/platform_device.h>
#include <linux/jiffies.h>
#include <linux/param.h>
#include <linux/input.h>
#include <linux/delay.h>
#include <asm-generic/gpio.h>
#include <linux/gpio.h>

#include <mach/hardware.h>


#define ROW_NR 3
#define COL_NR 6
#define DFT_COL_SCAN_UDELAY 15
#define MAX_COL_SCAN_UDELAY 20
#define DEV_DRV_NAME "davinci_matrix_keypad"


#if 0
static const int row_gpio[ROW_NR] = {14,15,16};
static const int col_gpio[COL_NR] = {10,11,12};
#else
static const char row_gpio[ROW_NR] = {10,11,12};
static const char col_gpio[COL_NR] = {14,15,16,17,13,19};
#endif

static const int key_matrix[ROW_NR][COL_NR] =
{
#if 0 
    [0] = {KEY_LEFT, KEY_RIGHT,KEY_STOP},
    [1] = {KEY_UP, KEY_DOWN,KEY_HOME},    
    [2] = {KEY_COPY,KEY_OPEN,KEY_PASTE},
#else
    [0] = {KEY_F,KEY_D,KEY_G,KEY_Z,KEY_X,KEY_S}, 
    [1] = {KEY_H,KEY_J,KEY_K,KEY_L,KEY_RESERVED,KEY_RESERVED},
    [2] = {KEY_ESC,KEY_ENTER,KEY_LEFT,KEY_UP,KEY_RIGHT,KEY_DOWN},
#endif
};
static struct timer_list scan_timer;
static struct input_dev *kp_input_dev;
static unsigned char col_status[ROW_NR] = {0};


struct tagKeycor
{
   unsigned char row;
   unsigned char col;
}lastkey_cor;

static unsigned long col_scan_udelay = DFT_COL_SCAN_UDELAY;
module_param(col_scan_udelay, ulong, S_IRUGO);
MODULE_PARM_DESC(col_scan_udelay, "udelay before scan column status, "
		"default is 5");


static void matrix_kp_scan(unsigned long data);
static void matrix_kp_setup_scan_timer(struct timer_list * timer,
		unsigned long data)
{
	timer->function = matrix_kp_scan;
	timer->expires = jiffies + HZ/20;
	timer->data = data;
}

static void matrix_kp_get_col_status(unsigned char * status, int cnt)
{
	int row;
	int col;
	int val;
	int ret;

	memset(status, 0, sizeof(status[0]) * cnt);
	for (row = 0; row < ROW_NR && row < cnt; row++) 
	{
#if 1
		gpio_request(row_gpio[row],NULL);
		gpio_direction_output(row_gpio[row], 0);
#else
		ret = gpio_direction_output(row_gpio[row], 0);
		printk("\ngpio_direction_output(row_gpio[row], 0) = %d\n",ret);
#endif
		if (col_scan_udelay > 0)
		{
		   udelay(col_scan_udelay);
		}
		for (col = 0; col < COL_NR; col++)
		{
		   val = gpio_get_value(col_gpio[col]);
		   if (val == 0)
		   {
			   status[row] |= (1<<col);
		   }
		}		
#if 1
		ret = gpio_direction_input(row_gpio[row]);
#else		
		udelay(col_scan_udelay);
		gpio_free(row_gpio[row]);
		ret = gpio_direction_input(row_gpio[row]);
	        printk("\ngpio_direction_input(row_gpio[row]) = %d\n",ret);
#endif
	}
}

static unsigned char matrix_kp_changed(const unsigned char* status,int cnt,\
                                        const unsigned char* oldstatus)
{
	int row;
	unsigned char changed = 0;

	for (row = 0; row < cnt; row++)
	{
		changed = oldstatus[row] ^ status[row];
		if(changed)
		{
           return 1;
		}
	}
	return 0;
}

static void matrix_kp_procstatus(const unsigned char* status,int rowindex,\
								int key_status,struct input_dev * pdev)
{
	int nowcol = 0xFF;    

	if(status[rowindex] == 0x0)
	{
		nowcol = lastkey_cor.col;
	}
	else if(status[rowindex] == 0x01)
	{
		nowcol = 0;
	}
	else if(status[rowindex] == 0x02)
	{
		nowcol = 1;
	}
	else if(status[rowindex] == 0x06)
	{
		nowcol = 2;
	}
	else if(status[rowindex] == 0x0A)
	{
		nowcol = 3;
	}
	else if(status[rowindex] == 0x12)
	{
		nowcol = 4;
	}
	else if(status[rowindex] == 0x22)
	{
		nowcol = 5;
	}
	if((nowcol >= 0) && ((nowcol <= 5)))
	{
	  input_report_key(pdev, key_matrix[rowindex][nowcol], key_status);
	  input_sync(pdev);
	  printk(KERN_ERR "\n2key %d is %s\n", key_matrix[rowindex][nowcol],  //KERN_DEBUG
			  (key_status == 1) ? "pressed" : "released");
	  printk(KERN_ERR "\n2key_matrix,row=%d,col=%d\n",rowindex,nowcol);
	  if(key_status == 1)
	  {
		lastkey_cor.row = rowindex;
		lastkey_cor.col = nowcol;
	  }
	  else
	  {
		lastkey_cor.row = 0xFF;
		lastkey_cor.col = 0xFF;
	  }
	}
}

static int matrix_kp_processkey(const unsigned char* status,int cnt,\
								 struct input_dev * pdev)
{
	int row;
	int col;
	unsigned char changed;
	unsigned char pressed = 0;	
	int process_flage = 0;

	for (row = 0; row < (cnt - 1); row++)
	{
		changed = col_status[row] ^ status[row];
		//printk(KERN_ERR "\n1 status[%d]=%d\n",row,status[row]);
		if (!changed)
		{
			 continue;
		}
		for (col = 0; col < COL_NR; col++)
		{			 
	         if (changed & (1<<col))
			 {
				pressed = !!(status[row] & (1<<col)); //转换为bool类型值
				if(pressed == 1)
				{
				   lastkey_cor.row = row;
				   lastkey_cor.col = col;
				}
				else
				{
				   lastkey_cor.row = 0xFF;
				   lastkey_cor.col = 0xFF;
				}
				printk(KERN_ERR "\n1key %d is %s\n", key_matrix[row][col],  
						(pressed == 1) ? "pressed" : "released");
				printk(KERN_ERR "\n1key_matrix,row=%d,col=%d\n",row,col);
				input_report_key(pdev, key_matrix[row][col], pressed);
				process_flage = 1;
				input_sync(pdev);
			 }//if (changed & (1<<col))
		}//for(col = 0; col < COL_NR; col++)
	}//for (row = 0; row < ROW_NR; row++)

	row = cnt - 1;
	changed = col_status[row] ^ status[row];
	//printk(KERN_ERR "\n2 status[%d]=%d\n",row,status[row]);
	if(changed)
	{
		process_flage = 1;
		if(status[row] == 0)
		{
		  matrix_kp_procstatus(status,row,0,pdev);
		}
		else
		{
          matrix_kp_procstatus(status,row,1,pdev);
		}
	}// if(changed)
	if((col_status[lastkey_cor.row] == 0) && (status[lastkey_cor.row] == 0) && (process_flage == 0))
	{
		if( (lastkey_cor.row != 0xFF)&&(lastkey_cor.col != 0xFF) )
		{
		   printk(KERN_ERR "\n key %d will released,row=%x,col=%x \n",\
		   key_matrix[lastkey_cor.row][lastkey_cor.col],lastkey_cor.row,lastkey_cor.col);
		   input_report_key(pdev, key_matrix[lastkey_cor.row][lastkey_cor.col],0);
		   input_sync(pdev);
		   lastkey_cor.row = 0xFF;
		   lastkey_cor.col = 0xFF;
		}
	}
	return process_flage;
}

static void matrix_kp_scan(unsigned long data)
{
	struct input_dev * dev = (struct input_dev*)data;
	unsigned char new_status[ROW_NR] = {0};
	unsigned char temp_status[ROW_NR] = {0};
	unsigned char changed;
	unsigned char can_process_flage = 0;
	unsigned long tdealy = 0;

	matrix_kp_get_col_status(temp_status, ROW_NR);
	changed = matrix_kp_changed(temp_status,ROW_NR,col_status);
#if 1
	if(changed)
	{
	  tdealy = jiffies + (20*HZ/1000);
	  while(jiffies < tdealy)
	  {
         ;
	  }
	  matrix_kp_get_col_status(new_status, ROW_NR);
	  changed = matrix_kp_changed(new_status,ROW_NR,temp_status);
	  can_process_flage = 1;
	}
	if( (!changed) && (!can_process_flage) )
	{
      memcpy(new_status, temp_status, sizeof(new_status));
	}
	if(!changed)
	{	  
	  matrix_kp_processkey(new_status,ROW_NR,dev);
	  if(can_process_flage)
	  {
		memcpy(col_status, new_status, sizeof(col_status));
	  }
	  else
	  {
        memcpy(col_status, temp_status, sizeof(col_status));
	  }
	}
	else
	{
	  printk(KERN_ERR "\n key is not stable\n");
	}
#else
	if(changed)
	{
	  tdealy = jiffies + (20*HZ/1000);
	  while(jiffies < tdealy)
	  {
         ;
	  }
	  matrix_kp_get_col_status(new_status, ROW_NR);
	  changed = matrix_kp_changed(new_status,ROW_NR,temp_status);
	  if(changed == 0)
	  {
	    can_process_flage = 2; //可以处理了
	  }
	  else if(changed == 1)
	  {
	    memcpy(col_status, new_status, sizeof(new_status));
		can_process_flage = 1; //不稳定，不能处理
	  }
	}  //if(changed)
	if( (!changed) && (can_process_flage == 1) ) //震荡时间较长，经过了循环，由不稳定到稳定的
	{
      memcpy(new_status, temp_status, sizeof(new_status));
	  can_process_flage = 2;
	}
	if(can_process_flage == 2)//可以处理了
	{
	    matrix_kp_processkey(new_status,ROW_NR,dev);
		memcpy(col_status, new_status, sizeof(col_status));
        can_process_flage = 0;
	}
#endif	
	matrix_kp_setup_scan_timer(&scan_timer, data);
	add_timer(&scan_timer);
}

static int matrix_kp_open(struct input_dev *dev)
{
    matrix_kp_get_col_status(col_status, ROW_NR);
	matrix_kp_setup_scan_timer(&scan_timer, (unsigned long)dev);
	add_timer(&scan_timer);

	lastkey_cor.row = 0xFF;
    lastkey_cor.col = 0xFF;

	return 0;
}

static void matrix_kp_close(struct input_dev *dev)
{
     del_timer_sync(&scan_timer);
}

static int matrix_kp_probe(struct platform_device *pdev)
{
	int row;
	int col;

	init_timer(&scan_timer);

	kp_input_dev = input_allocate_device();
	if (kp_input_dev == NULL)
	{
		return -ENOMEM;
	}

	set_bit(EV_KEY, kp_input_dev->evbit);
	for (row = 0; row < ROW_NR; row++)
	{
		for (col = 0; col < COL_NR; col++)
		{
			set_bit(key_matrix[row][col], kp_input_dev->keybit);
		}
	}
	kp_input_dev->name = DEV_DRV_NAME;
	kp_input_dev->phys = DEV_DRV_NAME;
	kp_input_dev->dev.parent = &pdev->dev;
	kp_input_dev->id.bustype = BUS_HOST;
	kp_input_dev->open = matrix_kp_open;
	kp_input_dev->close = matrix_kp_close;

	if (input_register_device(kp_input_dev) < 0) 
	{
	   printk("\n: register input device fail\n");
	   printk(KERN_ERR DEV_DRV_NAME": register input device fail\n");
	   return -EINVAL;
	}
	printk("\n: register input device ok\n");
	return 0;
}

static int matrix_kp_remove(struct platform_device *pdev)
{
	input_unregister_device(kp_input_dev);
	input_free_device(kp_input_dev);
	return 0;
}

static void matrix_kp_release_dev(struct device * dev)
{
}

static struct platform_device davinci_matrix_keypad_device = {
	.name = DEV_DRV_NAME,
	.id = -1,
	.dev = {
		.platform_data = NULL,
		.release = matrix_kp_release_dev,
	},
	.num_resources = 0,
	.resource = NULL,
};

static struct platform_driver davinci_matrix_keypad_driver = {
	.driver = {
		.name = DEV_DRV_NAME,
	},
	.probe = matrix_kp_probe,
	.remove = matrix_kp_remove,
};

static void matrix_kp_check_params(void)
{
	if (col_scan_udelay > MAX_COL_SCAN_UDELAY)
	{
		printk(KERN_INFO "invalid col_scan_udelay %lu, "
				"reset it to default delay %d\n",
				col_scan_udelay, DFT_COL_SCAN_UDELAY);
		col_scan_udelay = DFT_COL_SCAN_UDELAY;
	}
}


static int __init matrix_kp_init(void)
{
	int rval = 0;
	int result;

	printk(KERN_INFO "init davinci matrix keypad driver\n");

	matrix_kp_check_params();

	result = platform_device_register(&davinci_matrix_keypad_device);
	if (result == 0)
	{
		result = platform_driver_register(&davinci_matrix_keypad_driver);
		if (result != 0)
		{
			printk(KERN_ERR "register davinci matrix kp driver fail\n");
			platform_device_unregister(&davinci_matrix_keypad_device);
			rval = result;
		}
	}
	else
	{
		printk(KERN_ERR "register davinci matrix kp device fail\n");
		rval = result;
	}

	return rval;
}

static void __exit matrix_kp_exit(void)
{
	printk(KERN_INFO "exit davinci matrix keypad driver\n");
#if 0
	unconfig_gpio();
#endif
	platform_driver_unregister(&davinci_matrix_keypad_driver);
	platform_device_unregister(&davinci_matrix_keypad_device);
}

module_init(matrix_kp_init);
module_exit(matrix_kp_exit);

MODULE_AUTHOR("creesec <creesec@sina.com>");
MODULE_DESCRIPTION("davinci matrix keypad driver");
MODULE_LICENSE("Dual BSD/GPL");

