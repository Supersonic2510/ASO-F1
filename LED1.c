/**
 * @file   LED1.c
 * @author Pol Navarro Solà
 * @date   09/10/2021
 * @brief  A kernel module for controlling a GPIO LED and a GPIO button
*/
 
#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/gpio.h>
#include <linux/interrupt.h>
#include <asm/irq.h>
#include <asm/io.h>
 
MODULE_LICENSE("GPL");
MODULE_AUTHOR("Pol Navarro Solà");
MODULE_DESCRIPTION("Button in GPIO 16 and 26 lights on/off an LED in GPIO 21");
MODULE_VERSION("0.1");


#define DEBOUNCE_TIME 200
#define LED_ON 1
#define LED_OFF 0
 
//All variables needs to be defines as static 
static unsigned int gpioLED = 21;
static unsigned int gpioButtonOn = 16;
static unsigned int gpioButtonOff = 26;
static unsigned int irqNumberOn;
static unsigned int irqNumberOff;
static unsigned int numberPressesOn = 0;
static unsigned int numberPressesOff = 0;
static bool ledOn;

/**
 * @brief  Interrupt handler which mamages which button has been pressed
*/
static irq_handler_t LED1_irq_handler(unsigned int irq, void *dev_id, struct pt_regs *regs){
static char* envp[] = { "HOME=/", NULL };
   if (irqNumberOn == irq){
      char* argv[] = {"/usr/lib/lkm/pushbutton1.sh", NULL};
      ledOn = LED_ON;
      gpio_set_value(gpioLED, ledOn);
      numberPressesOn++;
      printk(KERN_INFO "LED1: Interrupt on LED1 (ON) from the LKM\n");
      call_usermodehelper(argv[0], argv, envp, UMH_NO_WAIT);
   } else if (irqNumberOff == irq) {
      char* argv[] = {"/usr/lib/lkm/pushbutton2.sh", NULL};
      ledOn = LED_OFF;
      gpio_set_value(gpioLED, ledOn);
      numberPressesOff++;
      printk(KERN_INFO "LED1: Interrupt on LED1 (OFF) from the LKM\n");
      call_usermodehelper(argv[0], argv, envp, UMH_NO_WAIT);
   } else {
      printk(KERN_ERR "LED1: Interrupt on LED1 (NOT DEFINED) from the LKM\n");
   }

   return (irq_handler_t) IRQ_HANDLED;
}

/**
 * @brief  Simple auxiliar function to free all variables related to GPIO/Handler
*/ 
static void freeAll(void) {
   gpio_set_value(gpioLED, LED_OFF);
   gpio_unexport(gpioLED);
   free_irq(irqNumberOn, NULL);
   free_irq(irqNumberOff, NULL);
   gpio_unexport(gpioButtonOn);
   gpio_unexport(gpioButtonOff);
   gpio_free(gpioLED);
   gpio_free(gpioButtonOn);
   gpio_free(gpioButtonOff);
}

/**
 * @brief  Beggining of the function, initilize evrything
*/ 
static int __init LED1_init(void){
   int result = 0;
   printk(KERN_INFO "LED1: Initializing the LED1 LKM\n");

   if (!gpio_is_valid(gpioLED)){
      //Return ENODEV: No such device
      printk(KERN_INFO "LED1: invalid LED GPIO\n");
      return ENODEV;
   }

   //Initialize LED
   gpio_request(gpioLED, "sysfs");
   gpio_direction_output(gpioLED, LED_OFF);
   gpio_set_value(gpioLED, LED_OFF);
   gpio_export(gpioLED, false);

   //Initialize Pushbutton which switch on LED
   gpio_request(gpioButtonOn, "sysfs");
   gpio_direction_input(gpioButtonOn);
   gpio_set_debounce(gpioButtonOn, DEBOUNCE_TIME);
   gpio_export(gpioButtonOn, false); 

   //Initialize Pushbutton which switch off LED
   gpio_request(gpioButtonOff, "sysfs");
   gpio_direction_input(gpioButtonOff);
   gpio_set_debounce(gpioButtonOff, DEBOUNCE_TIME);
   gpio_export(gpioButtonOff, false);      

   //Setting the GPIO Interrupt number related to GPIO I/O
   irqNumberOn = gpio_to_irq(gpioButtonOn);
   irqNumberOff = gpio_to_irq(gpioButtonOff);
 
   //Setting the GPIO Interrupt Handler Button On
   result = request_irq(irqNumberOn, (irq_handler_t) LED1_irq_handler, IRQF_TRIGGER_FALLING, "LED1_irq_handler", NULL);
   if(result) {
      printk(KERN_ERR "LED1: Failed to register button On LED1\n");
      freeAll();
      return result;
   }

   //Setting the GPIO Interrupt Handler Button Off
   result = request_irq(irqNumberOff, (irq_handler_t) LED1_irq_handler, IRQF_TRIGGER_FALLING, "LED1_irq_handler", NULL);
   if(result) {
      printk(KERN_ERR "LED1: Failed to register button Off LED1\n");
      freeAll();
      return result;
   }
 
   return 0;
}

/**
 * @brief  Ending of the function, uninitilize evrything
*/ 
static void __exit LED1_exit(void){
   printk(KERN_INFO "LED1: The button A was pressed %d times\n", numberPressesOn);
   printk(KERN_INFO "LED1: The button B was pressed %d times\n", numberPressesOff);
   freeAll();
   printk(KERN_INFO "LED1: Uninitializing the LED1 from the LKM\n");
}
 
module_init(LED1_init);
module_exit(LED1_exit);