/**
 * @file   LED2.c
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
MODULE_DESCRIPTION("Button in GPIO 19 and 13 lights on/off an LED in GPIO 20");
MODULE_VERSION("0.1");


#define DEBOUNCE_TIME 200
#define LED_ON 1
#define LED_OFF 0

//All variables needs to be defines as static 
static unsigned int gpioLED = 20;
static unsigned int gpioButtonOn = 19;
static unsigned int gpioButtonOff = 13;
static unsigned int irqNumberOn;
static unsigned int irqNumberOff;
static unsigned int numberPressesOn = 0;
static unsigned int numberPressesOff = 0;
static bool ledOn;

/**
 * @brief  Interrupt handler which mamages which button has been pressed
*/
static irq_handler_t LED2_irq_handler(unsigned int irq, void *dev_id, struct pt_regs *regs){
static char* envp[] = { "HOME=/", NULL };
   if (irqNumberOn == irq){
      char* argv[] = {"/usr/lib/lkm/pushbutton3.sh", NULL};
      ledOn = LED_ON;
      gpio_set_value(gpioLED, ledOn);
      numberPressesOn++;
      printk(KERN_INFO "LED2: Interrupt on LED2 (ON) from the LKM\n");
      call_usermodehelper(argv[0], argv, envp, UMH_NO_WAIT);
   } else if (irqNumberOff == irq) {
      char* argv[] = {"/usr/lib/lkm/pushbutton4.sh", NULL};
      ledOn = LED_OFF;
      gpio_set_value(gpioLED, ledOn);
      numberPressesOff++;
      printk(KERN_INFO "LED2: Interrupt on LED2 (OFF) from the LKM\n");
      call_usermodehelper(argv[0], argv, envp, UMH_NO_WAIT);
   } else {
      printk(KERN_ERR "LED2: Interrupt on LED2 (NOT DEFINED) from the LKM\n");
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
static int __init LED2_init(void){
   int result = 0;
   printk(KERN_INFO "LED2: Initializing the LED2 LKM\n");

   if (!gpio_is_valid(gpioLED)){
      //Return ENODEV: No such device
      printk(KERN_INFO "LED2: invalid LED GPIO\n");
      return ENODEV;
   }

   //Initialize LED
   gpio_request(gpioLED, "sysfs");
   gpio_direction_output(gpioLED, LED_OFF);
   gpio_set_value(gpioLED, LED_OFF);
   gpio_export(gpioLED, false);

   //Initialize Pushbutton which switch On LED
   gpio_request(gpioButtonOn, "sysfs");
   gpio_direction_input(gpioButtonOn);
   gpio_set_debounce(gpioButtonOn, DEBOUNCE_TIME);
   gpio_export(gpioButtonOn, false); 

   //Initialize Pushbutton which switch Off LED
   gpio_request(gpioButtonOff, "sysfs");
   gpio_direction_input(gpioButtonOff);
   gpio_set_debounce(gpioButtonOff, DEBOUNCE_TIME);
   gpio_export(gpioButtonOff, false);      

   //Setting the GPIO Interrupt number related to GPIO I/O
   irqNumberOn = gpio_to_irq(gpioButtonOn);
   irqNumberOff = gpio_to_irq(gpioButtonOff);
 
   //Setting the GPIO Interrupt Handler Button On
   result = request_irq(irqNumberOn, (irq_handler_t) LED2_irq_handler, IRQF_TRIGGER_FALLING, "LED2_irq_handler", NULL);
   if(result) {
      printk(KERN_ERR "LED2: Failed to register button On LED1\n");
      freeAll();
      return result;
   }

   //Setting the GPIO Interrupt Handler Button Off
   result = request_irq(irqNumberOff, (irq_handler_t) LED2_irq_handler, IRQF_TRIGGER_FALLING, "LED2_irq_handler", NULL);
   if(result) {
      printk(KERN_ERR "LED2: Failed to register button Off LED2\n");
      freeAll();
      return result;
   }
 
   return 0;
}

/**
 * @brief  Ending of the function, uninitilize evrything
*/ 
static void __exit LED2_exit(void){
   printk(KERN_INFO "LED2: The button C was pressed %d times\n", numberPressesOn);
   printk(KERN_INFO "LED2: The button D was pressed %d times\n", numberPressesOff);
   freeAll();
   printk(KERN_INFO "LED2: Uninitializing the LED2 from the LKM\n");
}
 
module_init(LED2_init);
module_exit(LED2_exit);