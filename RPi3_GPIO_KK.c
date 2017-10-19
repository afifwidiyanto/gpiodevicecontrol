#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/gpio.h>
#include <linux/interrupt.h> // for the IRQ code

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Afif Widiyanto Musthofa");
MODULE_DESCRIPTION("Button test driver");
MODULE_VERSION("0.3.3");

static unsigned int gpioDevice1 = 23; // GPIO23
static unsigned int gpioDevice2 = 24;
static unsigned int gpioDevice3 = 25;
static unsigned int gpioButton1 = 17; // GPIO17
static unsigned int gpioButton2 = 27;
static unsigned int gpioButton3 = 22;
static unsigned int irqNumber1On; // share IRQ num within file
static unsigned int irqNumber2On; // need to check these IRQ numbers with "cat interrupts | grep rpi3" at /proc/interrupts or /proc
static unsigned int irqNumber3On;
static unsigned int buttonCounter1 = 0;
static unsigned int buttonCounter2 = 0;
static unsigned int buttonCounter3 = 0;
static unsigned int totalCounter = 0;
static bool devOn1 = 0; // Initial state of devices
static bool devOn2 = 0;
static bool devOn3 = 0;
int result1On = 0;
int result2On = 0;
int result3On = 0;

/** prototype for the custom IRQ handler function, function below (still borked)
	learn how to use multiple irq_handler_t
	or learn about rpi3_gpio_handler in /proc/interrupts
	or perhaps fiddle around with the dev_id of shared interrupt lines
	try to use IRQF_SHARED in these lines**/

static irq_handler_t rpi3_gpio_irq_handler_1(unsigned int irq, void *dev_id, struct pt_regs *regs);
static irq_handler_t rpi3_gpio_irq_handler_2(unsigned int irq, void *dev_id, struct pt_regs *regs);
static irq_handler_t rpi3_gpio_irq_handler_3(unsigned int irq, void *dev_id, struct pt_regs *regs);

/** LKM initialization function */

static int __init rpi3_gpio_init(void) {
	/** a struct datatype to represent the IRQ numbers  (still not sure about this one)**/

	printk(KERN_INFO "GPIO_TEST: Initializing the GPIO_TEST LKM\n");

	/* GPIO validation on the three devices */
	if (!gpio_is_valid(gpioDevice1) || !gpio_is_valid(gpioDevice2) || !gpio_is_valid(gpioDevice3)) {
		printk(KERN_INFO "GPIO_TEST: invalid GPIO for Devices\n");
		return -ENODEV; //wouldn't using ENXIO is more appropriate than ENODEV?
	}

	/* Configuring GPIO pins for devices */
/* 	devOn1 = false; // These lines can be used set DEFAULT OFF/ON state for the devices
	devOn2 = false;
	devOn3 = false; */

	gpio_request(gpioDevice1, "sysfs"); // request LED GPIO
	gpio_direction_output(gpioDevice1, devOn1); // set in output mode
	//gpio_set_value(gpioDevice1, devOn1); // not reqd - see line above
	gpio_export(gpioDevice1, false); // appears in /sys/class/gpio
	// false prevents in/out change

	gpio_request(gpioDevice2, "sysfs");
	gpio_direction_output(gpioDevice2, devOn2);
	// gpio_set_value(gpioDevice2, devOn2);
	gpio_export(gpioDevice2, false);

	gpio_request(gpioDevice3, "sysfs");
	gpio_direction_output(gpioDevice3, devOn3);
	// gpio_set_value(gpioDevice3, devOn3);
	gpio_export(gpioDevice3, false);

	gpio_request(gpioButton1, "sysfs"); // set up gpioButton1
	gpio_direction_input(gpioButton1); // set up as input
	gpio_set_debounce(gpioButton1, 200); // debounce delay of 200ms to avoid erratic and uncontrolled interrupt
	gpio_export(gpioButton1, false); // appears in /sys/class/gpio

	gpio_request(gpioButton2, "sysfs");
	gpio_direction_input(gpioButton2);
	gpio_set_debounce(gpioButton2, 200);
	gpio_export(gpioButton2, false);

	gpio_request(gpioButton3, "sysfs");
	gpio_direction_input(gpioButton3);
	gpio_set_debounce(gpioButton3, 200);
	gpio_export(gpioButton3, false);

	printk(KERN_INFO "GPIO_TEST: button1 value is currently: %d\n", gpio_get_value(gpioButton1));
	irqNumber1On = gpio_to_irq(gpioButton1); // map GPIO to IRQ number 1
	printk(KERN_INFO "GPIO_TEST: button1 mapped to IRQ: %d\n", irqNumber1On);

	printk(KERN_INFO "GPIO_TEST: button2 value is currently: %d\n", gpio_get_value(gpioButton2));
	irqNumber2On = gpio_to_irq(gpioButton2); // map GPIO to IRQ number 2
	printk(KERN_INFO "GPIO_TEST: button2 mapped to IRQ: %d\n", irqNumber2On);

	printk(KERN_INFO "GPIO_TEST: button3 value is currently: %d\n", gpio_get_value(gpioButton3));
	irqNumber3On = gpio_to_irq(gpioButton3); // map GPIO to IRQ number 3
	printk(KERN_INFO "GPIO_TEST: button3 mapped to IRQ: %d\n", irqNumber3On);

	/* This next call requests interrupt lines */
	/* Interrupt lines when tactile button is pressed */

	result1On = request_irq(irqNumber1On, // interrupt number requested
			(irq_handler_t) rpi3_gpio_irq_handler_1_on, // handler function
			// TO DO: Insert IRQF_SHARED here
			IRQF_TRIGGER_RISING, // on rising edge (press, not release)
			"rpi3_gpio_handler", // used in /proc/interrupts
			NULL); // *dev_id for shared interrupt lines shouldn't be NULL
	printk(KERN_INFO "GPIO_TEST: IRQ request result for device 1 is: %d\n", result1On);
	// return result1On;

	result2On = request_irq(irqNumber2On,
			(irq_handler_t) rpi3_gpio_irq_handler_2_on,
			IRQF_TRIGGER_RISING,
			"rpi3_gpio_handler",
			NULL);
	printk(KERN_INFO "GPIO_TEST: IRQ request result for device 2 is: %d\n", result2On);
	// return result2On;

	result3On = request_irq(irqNumber3On,
			(irq_handler_t) rpi3_gpio_irq_handler_3_on,
			IRQF_TRIGGER_RISING,
			"rpi3_gpio_handler",
			NULL);
	printk(KERN_INFO "GPIO_TEST: IRQ request result for device 3 is: %d\n", result3On);
	// return result3On;

	return 0;
 }

/** LKM cleanup function */

static void __exit rpi3_gpio_exit(void) {

	printk(KERN_INFO "GPIO_TEST: button 1 value is currently: %d\n", gpio_get_value(gpioButton1));
	printk(KERN_INFO "GPIO_TEST: button 1 was pressed %d times\n", buttonCounter1);
	printk(KERN_INFO "GPIO_TEST: button 2 value is currently: %d\n", gpio_get_value(gpioButton2));
	printk(KERN_INFO "GPIO_TEST: button 2 was pressed %d times\n", buttonCounter2);
	printk(KERN_INFO "GPIO_TEST: button 3 value is currently: %d\n", gpio_get_value(gpioButton3));
	printk(KERN_INFO "GPIO_TEST: button 3 was pressed %d times\n", buttonCounter3);
	printk(KERN_INFO "GPIO_TEST: in total the buttons was pressed %d times\n", totalCounter);

	gpio_set_value(gpioDevice1, 0); // turn the LED off
	gpio_unexport(gpioDevice1); // unexport the LED GPIO
	free_irq(irqNumber1On, NULL);// free the IRQ number, no *dev_id
	gpio_unexport(gpioButton1); // unexport the Button GPIO
	gpio_free(gpioDevice1); // free the LED GPIO
	gpio_free(gpioButton1); // free the Button GPIO

	gpio_set_value(gpioDevice2, 0);
	gpio_unexport(gpioDevice2);
	free_irq(irqNumber2On, NULL);
	gpio_unexport(gpioButton2);
	gpio_free(gpioDevice2);
	gpio_free(gpioButton2);

	gpio_set_value(gpioDevice3, 0);
	gpio_unexport(gpioDevice3);
	free_irq(irqNumber3On, NULL);
	gpio_unexport(gpioButton3);
	gpio_free(gpioDevice3);
	gpio_free(gpioButton3);

	printk(KERN_INFO "GPIO_TEST: Goodbye from the LKM!\n");
}

/** GPIO IRQ Handler functions */

static irq_handler_t rpi3_gpio_irq_handler_1(unsigned int irq, void *dev_id, struct pt_regs *regs) {
	devOn1 = !devOn1; // invert the LED state
	gpio_set_value(gpioDevice1, devOn1); // set LED accordingly
	printk(KERN_INFO "GPIO_TEST: Interrupt! (button 1 is %d)\n",
	gpio_get_value(gpioButton1));
	buttonCounter1++;
	totalCounter++; // global counter
	return (irq_handler_t) IRQ_HANDLED; // announce IRQ handled
}

static irq_handler_t rpi3_gpio_irq_handler_2(unsigned int irq, void *dev_id, struct pt_regs *regs) {
	devOn2 = !devOn2;
	gpio_set_value(gpioDevice2, devOn2);
	printk(KERN_INFO "GPIO_TEST: Interrupt! (button 2 is %d)\n",
	gpio_get_value(gpioButton2));
	buttonCounter2++;
	totalCounter++;
	return (irq_handler_t) IRQ_HANDLED;
}

static irq_handler_t rpi3_gpio_irq_handler_3(unsigned int irq, void *dev_id, struct pt_regs *regs) {
	devOn3 = !devOn3;
	gpio_set_value(gpioDevice3, devOn3);
	printk(KERN_INFO "GPIO_TEST: Interrupt! (button 3 is %d)\n",
	gpio_get_value(gpioButton3));
	buttonCounter3++;
	totalCounter++;
	return (irq_handler_t) IRQ_HANDLED;
}

module_init(rpi3_gpio_init);
module_exit(rpi3_gpio_exit);

/** change default state
    added result1On & result1Off
	added IRQ Handler for falling edge*/

/** TO DO:
	get to know how to put multiple interrupt handler in one module, or alternatively create multiple modules
	in accordance to that, learn how to use irq_handler or irq_handler_t
	learn about rpi3_gpio_handler /proc/interrupts
	learn how to use multiple irq_handler_t
	or perhaps fiddle around with the dev_id of shared interrupt lines
	read "Interrupt Handling" by Sarah Diesburg
	add a command for 1000000 blinking LED
	add time measurement to do 1000000 blinking LED
*/

/** check with sudo tail /var/log/kern.log
	check gpio with ls -l gpio* at /sys/class/gpio
	the device states are toggles so that kernel won't need multiple functions. It is intended for non-fuzzy state of On/Off anyway
	check the existing IRQ number with cat interupts | grep rpi3 at /proc
*/

/** untuk cek apakah busy bisa pakai common error -EBUSY ==> denotes given interrupt line is already in use & no sharing
*/
