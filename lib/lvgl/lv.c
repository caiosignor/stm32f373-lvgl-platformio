#include "lvgl.h"
#include "ILI9341_STM32_Driver.h"
// #include "ILI9341_GFX.h"
#include "main.h"
#include "gpio.h"
#include "usart.h"
#define BASE_TEMPO 5 //ms

uint32_t tempo;

//callback para copiar o buffer para uma área específica do display.
void my_flush_cb(lv_disp_drv_t *disp_drv, const lv_area_t *area, lv_color_t *color_p)
{
    /*The most simple case (but also the slowest) to put all pixels to the screen one-by-one*/
    int32_t x, y;
    for (y = area->y1; y <= area->y2; y++)
    {
        for (x = area->x1; x <= area->x2; x++)
        {
            ILI9341_Draw_Pixel(x, y, color_p->ch.blue);
            // ILI9341_Draw_Pixel(x, y, color_p->ch.blue + color_p->ch.red + color_p->ch.green_h + color_p->ch.green_l);
            // ILI9341_Draw_Pixel(x, y, color_p->ch.blue + color_p->ch.red + color_p->ch.green);
            // ILI9341_Draw_Pixel(x, y, color_p->ch.blue + color_p->ch.red + color_p->ch.green);
            // put_px(x, y, *color_p)
            color_p++;
        }
    }

    /* IMPORTANT!!!
     * Inform the graphics library that you are ready with the flushing*/
    lv_disp_flush_ready(disp_drv);
}

void lv_ex_btn_1(void)
{
    lv_obj_t *label;
    lv_obj_t *btn1 = lv_btn_create(lv_scr_act(), NULL);
    lv_obj_align(btn1, NULL, LV_ALIGN_CENTER, 0, -40);
    label = lv_label_create(btn1, NULL);
    lv_label_set_text(label, "Primeiro Botao");
}

void lv_setup()
{
    lv_init();
    ILI9341_Init(); //initial driver setup to drive ili9341

    //buffer interno para do display
    static lv_disp_buf_t disp_buff;

    //inicializa os buffers internos.
    static lv_color_t buf_1[LV_HOR_RES_MAX * 10];
    static lv_color_t buf_2[LV_HOR_RES_MAX * 10];
    lv_disp_buf_init(&disp_buff, buf_1, NULL, LV_HOR_RES * 10);
    // lv_disp_buf_init(&disp_buff, buf_1, buf_2, LV_HOR_RES_MAX * 10);

    // //informações do driver do display; contém as callbacks necessárias para interagir com o display;
    lv_disp_drv_t disp_drv;
    //inicializacao basica
    lv_disp_drv_init(&disp_drv);

    // disp_drv.hor_res = 240;
    // disp_drv.ver_res = 320;

    //configura o driver do display;
    disp_drv.buffer = &disp_buff;
    disp_drv.flush_cb = my_flush_cb;
    // disp_drv.flush_cb = ili9341_flush;

    //registra o driver do display no objeto do display.
    lv_disp_drv_register(&disp_drv);

    lv_obj_t *label = lv_label_create(lv_scr_act(), NULL);
    lv_label_set_text(label, "LVGL Test");
    lv_obj_align(label, NULL, LV_ALIGN_CENTER, 0, 0);

    lv_ex_btn_1(); //inicializa os botões

    tempo = HAL_GetTick();
}

uint32_t tempo2;

void lv_loop()
{
    // if (tempo > HAL_GetTick())
    // {
    //     lv_tick_inc(BASE_TEMPO);
    //     tempo = HAL_GetTick() + BASE_TEMPO;
    //     lv_task_handler();
    // }

    lv_task_handler();
    HAL_Delay(50);

    // ILI9341_Fill_Screen(DARKGREEN);
    // ILI9341_Set_Rotation(SCREEN_HORIZONTAL_2);
    // ILI9341_Draw_Text("FPS TEST, 40 loop 2 screens", 10, 10, BLACK, 5, WHITE);
    // ILI9341_Draw_Filled_Rectangle_Coord(5, 10, 25, 30, WHITE);
    // ILI9341_Fill_Screen(DARKGREEN);

    // HAL_Delay(500);
    // HAL_GPIO_TogglePin(LED_GPIO_Port, LED_Pin);
}