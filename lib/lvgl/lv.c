#include "lvgl.h"
#include "ILI9341_STM32_Driver.h"
// #include "ILI9341_GFX.h"
#include "main.h"
#include "gpio.h"
#include "usart.h"

#define BASE_TEMPO 5 //ms

uint32_t tempo;

void ili9341_flush(lv_disp_drv_t *drv, const lv_area_t *area, const lv_color_t *color_p)
{
    if (area->x2 < 0 || area->y2 < 0 || area->x1 > (LV_HOR_RES_MAX - 1) || area->y1 > (LV_VER_RES_MAX - 1))
    {
        lv_disp_flush_ready(drv);
        return;
    }

    /* Truncate the area to the screen */
    int32_t act_x1 = area->x1 < 0 ? 0 : area->x1;
    int32_t act_y1 = area->y1 < 0 ? 0 : area->y1;
    int32_t act_x2 = area->x2 > LV_HOR_RES_MAX - 1 ? LV_HOR_RES_MAX - 1 : area->x2;
    int32_t act_y2 = area->y2 > LV_VER_RES_MAX - 1 ? LV_VER_RES_MAX - 1 : area->y2;

    int32_t y;
    uint8_t data[4];
    int32_t len = len = (act_x2 - act_x1 + 1) * 2;
    lv_coord_t w = (area->x2 - area->x1) + 1;

    /* window horizontal */
    ILI9341_Write_Command(ILI9341_CASET);
    // ili9341_write(ILI9341_CMD_MODE, ILI9341_CASET);
    data[0] = act_x1 >> 8;
    data[1] = act_x1;
    data[2] = act_x2 >> 8;
    data[3] = act_x2;
    // ili9341_write_array(ILI9341_DATA_MODE, data, 4);
    ILI9341_Write_Array(data, 4);

    /* window vertical */
    ILI9341_Write_Command(ILI9341_PASET);
    // ili9341_write(ILI9341_CMD_MODE, ILI9341_PASET);
    data[0] = act_y1 >> 8;
    data[1] = act_y1;
    data[2] = act_y2 >> 8;
    data[3] = act_y2;
    // ili9341_write_array(ILI9341_DATA_MODE, data, 4);
    ILI9341_Write_Array(data, 4);

    ILI9341_Write_Command(ILI9341_RAMWR);
    // ili9341_write(ILI9341_CMD_MODE, ILI9341_RAMWR);
    for (y = act_y1; y <= act_y2; y++)
    {
        ILI9341_Write_Array((uint8_t *)color_p, len);
        // ili9341_write_array(ILI9341_DATA_MODE, (uint8_t *)color_p, len);
        color_p += w;
    }

    lv_disp_flush_ready(drv);
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
    // disp_drv.flush_cb = my_flush_cb;
    disp_drv.flush_cb = ili9341_flush;

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