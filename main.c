#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <malloc.h>
#include <dma.h>
#include <framebuffer.h>
#include <gpio-f1c100s.h>
#include <pwm-f1c100s.h>

#define TEST_GPIO   0
#define TEST_PWM    0
#define TEST_FB     1

extern void sys_uart_putc(char c);

int boot_main(int argc, char **argv) {
    
    /* Do initial mem pool */
	do_init_mem_pool();
    do_init_dma_pool();
    
#if TEST_GPIO
    gpio_f1c100s_set_dir(&GPIO_PE, 2, GPIO_DIRECTION_OUTPUT);
    while(1)
    {
        gpio_f1c100s_set_value(&GPIO_PE, 2, 0);
        gpio_f1c100s_set_value(&GPIO_PE, 2, 1);
    }
#endif

#if TEST_FB
    {
        framebuffer_t fb_f1c100s;
        fb_f1c100s_init(&fb_f1c100s);
        render_t* render = fb_f1c100s_create(&fb_f1c100s);
        for (int i = 0; i < render->pixlen/4;i++)
        {
            ((uint32_t*)render->pixels)[i] = 0xFFFF0000;
        }
        fb_f1c100s_present(&fb_f1c100s, render);
        fb_f1c100s_setbl(&fb_f1c100s, 100);
    }
#endif

#if TEST_PWM
    pwm_t led_pwm_bl =
        {
            .virt = 0x01c21000,
            .duty = 50,
            .period = 1000000,
            .channel = 1,
            .polarity = true,
            .pwm_port = &GPIO_PE,
            .pwm_pin = 6,
            .pwm_pin_cfg = 3,
        };
    pwm_f1c100s_init(&led_pwm_bl);
#endif

    while(1)
    {
        sys_uart_putc('A');
    }
    return 0;
}

void __fatal_error(const char *msg) {
    while (1);
}

#ifndef NDEBUG
void __assert_func(const char *file, int line, const char *func, const char *expr) {
    //printf("Assertion '%s' failed, at file %s:%d\n", expr, file, line);
    __fatal_error("Assertion failed");
}
#endif

