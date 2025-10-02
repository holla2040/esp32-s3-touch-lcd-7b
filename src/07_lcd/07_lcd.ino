/*
* The LCD parameters and GPIO number used by this example can be changed in 
* [rgb_lcd_port.h](components/rgb_lcd_port.h). Especially, please pay attention 
* to the **vendor specific initialization**, it can be different between 
* manufacturers and should consult the LCD supplier for initialization sequence 
* code.
*/

#include "rgb_lcd_port.h"    // Header for Waveshare RGB LCD driver
#include "gui_paint.h"       // Header for graphical drawing functions
#include "image.h"           // Header for image resources

#define ROTATE ROTATE_0//rotate = 0, 90, 180, 270

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  delay(100);
  Serial.println("Starting LCD demo...");

  Serial.println("Initializing I2C...");
  DEV_I2C_Init();
  delay(100);  // Allow I2C to stabilize

  Serial.println("Initializing IO Extension...");
  IO_EXTENSION_Init();
  delay(100);

  // Turn on the LCD backlight BEFORE initializing the LCD panel
  Serial.println("Turning on backlight...");
  wavesahre_rgb_lcd_bl_on();
  delay(200);  // Give more time for backlight to turn on
  Serial.println("Backlight should be ON");

  // Initialize the Waveshare ESP32-S3 RGB LCD
  Serial.println("Initializing LCD panel...");
  waveshare_esp32_s3_rgb_lcd_init();  
  UDOUBLE Imagesize = EXAMPLE_LCD_H_RES * EXAMPLE_LCD_V_RES * 2; // Each pixel takes 2 bytes in RGB565
  UBYTE *BlackImage;
  if ((BlackImage = (UBYTE *)malloc(Imagesize)) == NULL) // Allocate memory
  {
      printf("Failed to apply for black memory...\r\n");
      exit(0); // Exit the program if memory allocation fails
  }
  // Create a new image canvas and set its background color to white
    Paint_NewImage(BlackImage, EXAMPLE_LCD_H_RES, EXAMPLE_LCD_V_RES, 0, WHITE);

    // Set the canvas scale
    Paint_SetScale(65);
    Paint_SetRotate(ROTATE);
    // Clear the canvas and fill it with a white background
    Paint_Clear(WHITE);
    if (ROTATE == ROTATE_0 || ROTATE == ROTATE_180)
    {
        // Draw gradient color stripes using RGB565 format
        // From red to blue
        Paint_DrawRectangle(1, 1, 64, 600, 0xF800, DOT_PIXEL_1X1, DRAW_FILL_FULL);  
        Paint_DrawRectangle(65, 1, 128, 600, 0x7800, DOT_PIXEL_1X1, DRAW_FILL_FULL); 
        Paint_DrawRectangle(129, 1, 192, 600, 0x3800, DOT_PIXEL_1X1, DRAW_FILL_FULL); 
        Paint_DrawRectangle(192, 1, 256, 600, 0x1800, DOT_PIXEL_1X1, DRAW_FILL_FULL); 
        Paint_DrawRectangle(257, 1, 320, 600, 0x0800, DOT_PIXEL_1X1, DRAW_FILL_FULL); 


        Paint_DrawRectangle(321, 1, 384, 600, 0x07E0, DOT_PIXEL_1X1, DRAW_FILL_FULL); 
        Paint_DrawRectangle(385, 1, 448, 600, 0x03E0, DOT_PIXEL_1X1, DRAW_FILL_FULL); 
        Paint_DrawRectangle(449, 1, 512, 600, 0x01E0, DOT_PIXEL_1X1, DRAW_FILL_FULL); 
        Paint_DrawRectangle(513, 1, 576, 600, 0x00E0, DOT_PIXEL_1X1, DRAW_FILL_FULL); 
        Paint_DrawRectangle(577, 1, 640, 600, 0x0060, DOT_PIXEL_1X1, DRAW_FILL_FULL); 
        Paint_DrawRectangle(641, 1, 705, 600, 0x0020, DOT_PIXEL_1X1, DRAW_FILL_FULL); 

        Paint_DrawRectangle(706, 1, 768, 600, 0x001F, DOT_PIXEL_1X1, DRAW_FILL_FULL); 
        Paint_DrawRectangle(769, 1, 832, 600, 0x000F, DOT_PIXEL_1X1, DRAW_FILL_FULL); 
        Paint_DrawRectangle(833, 1, 896, 600, 0x0007, DOT_PIXEL_1X1, DRAW_FILL_FULL); 
        Paint_DrawRectangle(897, 1, 960, 600, 0x0003, DOT_PIXEL_1X1, DRAW_FILL_FULL); 
        Paint_DrawRectangle(961, 1, 1024, 600, 0x0001, DOT_PIXEL_1X1, DRAW_FILL_FULL); 

        // Display the gradient on the screen
        wavesahre_rgb_lcd_display(BlackImage);
        vTaskDelay(1000);

        Paint_Clear(WHITE);
        // Draw points with increasing sizes
        Paint_DrawPoint(2, 18, BLACK, DOT_PIXEL_1X1, DOT_FILL_RIGHTUP);
        Paint_DrawPoint(2, 20, BLACK, DOT_PIXEL_2X2, DOT_FILL_RIGHTUP);
        Paint_DrawPoint(2, 23, BLACK, DOT_PIXEL_3X3, DOT_FILL_RIGHTUP);
        Paint_DrawPoint(2, 28, BLACK, DOT_PIXEL_4X4, DOT_FILL_RIGHTUP);
        Paint_DrawPoint(2, 33, BLACK, DOT_PIXEL_5X5, DOT_FILL_RIGHTUP);

        // Draw solid and dotted lines
        Paint_DrawLine(20, 5, 80, 65, MAGENTA, DOT_PIXEL_2X2, LINE_STYLE_SOLID);
        Paint_DrawLine(20, 65, 80, 5, MAGENTA, DOT_PIXEL_2X2, LINE_STYLE_SOLID);
        Paint_DrawLine(148, 35, 208, 35, CYAN, DOT_PIXEL_1X1, LINE_STYLE_DOTTED);
        Paint_DrawLine(178, 5, 178, 65, CYAN, DOT_PIXEL_1X1, LINE_STYLE_DOTTED);

        // Draw rectangles (empty and filled)
        Paint_DrawRectangle(20, 5, 80, 65, RED, DOT_PIXEL_2X2, DRAW_FILL_EMPTY);
        Paint_DrawRectangle(85, 5, 145, 65, BLUE, DOT_PIXEL_2X2, DRAW_FILL_FULL);

        // Draw circles (empty and filled)
        Paint_DrawCircle(178, 35, 30, GREEN, DOT_PIXEL_1X1, DRAW_FILL_EMPTY);
        Paint_DrawCircle(240, 35, 30, GREEN, DOT_PIXEL_1X1, DRAW_FILL_FULL);

        // Draw text and numbers
        Paint_DrawString_EN(1, 70, "AaBbCc123", &Font16, RED, WHITE);
        Paint_DrawNum(1, 100, 9.87654321, &Font20, 7, WHITE, BLACK);
        Paint_DrawString_EN(1, 130, "AaBbCc123", &Font20, 0x000f, 0xfff0);
        Paint_DrawString_EN(1, 160, "AaBbCc123", &Font24, RED, WHITE);
        Paint_DrawString_CN(1, 190, "你好Abc", &Font24CN, WHITE, BLUE);

        // Update the display with the newly drawn elements (currently displaying BlackImage)
        wavesahre_rgb_lcd_display(BlackImage);
        vTaskDelay(1000); // Delay for 1000ms to allow the screen to update

//         // Draw a bitmap at coordinates (0,0) with size 800x480 using the provided gImage_Bitmap
//         Paint_BmpWindows(0, 0, gImage_Bitmap, 1024, 600);
// 
//         // Update the screen with the updated image (BlackImage is the framebuffer being drawn to)
//         wavesahre_rgb_lcd_display(BlackImage); // Refresh the display with the new content
//         vTaskDelay(1000); // Delay for 1000ms to allow the screen to update
// 
//         // Draw an image resource gImage_picture at coordinates (0,0) with size 800x480
//         Paint_DrawImage(gImage_picture, 0, 0, 1024, 600);
// 
//         // Optionally, you can also call this to draw the bitmap, though it��s commented out here:
//         // Paint_DrawBitMap(gImage_picture);
// 
//         // Update the screen with the new image (BlackImage is the framebuffer being drawn to)
//         wavesahre_rgb_lcd_display(BlackImage); // Refresh the display to show the updated image

    }
    else
    {
        // Draw gradient color stripes using RGB565 format
        // From red to blue
        Paint_DrawRectangle(1, 1, 600, 64, 0xF800, DOT_PIXEL_1X1, DRAW_FILL_FULL);  
        Paint_DrawRectangle(1, 65,  600, 128, 0x7800, DOT_PIXEL_1X1, DRAW_FILL_FULL); 
        Paint_DrawRectangle(1, 129, 600, 192, 0x3800, DOT_PIXEL_1X1, DRAW_FILL_FULL); 
        Paint_DrawRectangle(1, 193, 600, 256, 0x1800, DOT_PIXEL_1X1, DRAW_FILL_FULL); 
        Paint_DrawRectangle(1, 257, 600, 320, 0x0800, DOT_PIXEL_1X1, DRAW_FILL_FULL); 


        Paint_DrawRectangle(1, 321, 600, 384, 0x07E0, DOT_PIXEL_1X1, DRAW_FILL_FULL); 
        Paint_DrawRectangle(1, 385, 600, 448, 0x03E0, DOT_PIXEL_1X1, DRAW_FILL_FULL); 
        Paint_DrawRectangle(1, 449, 600, 512, 0x01E0, DOT_PIXEL_1X1, DRAW_FILL_FULL); 
        Paint_DrawRectangle(1, 513, 600, 576, 0x00E0, DOT_PIXEL_1X1, DRAW_FILL_FULL); 
        Paint_DrawRectangle(1, 577, 600, 640, 0x0060, DOT_PIXEL_1X1, DRAW_FILL_FULL); 
        Paint_DrawRectangle(1, 641, 600, 704, 0x0020, DOT_PIXEL_1X1, DRAW_FILL_FULL); 

        Paint_DrawRectangle(1, 705, 600, 768, 0x001F, DOT_PIXEL_1X1, DRAW_FILL_FULL); 
        Paint_DrawRectangle(1, 769, 600, 832, 0x000F, DOT_PIXEL_1X1, DRAW_FILL_FULL); 
        Paint_DrawRectangle(1, 833, 600, 896, 0x0007, DOT_PIXEL_1X1, DRAW_FILL_FULL); 
        Paint_DrawRectangle(1, 897, 600, 960, 0x0003, DOT_PIXEL_1X1, DRAW_FILL_FULL); 
        Paint_DrawRectangle(1, 961, 600, 1024, 0x0001, DOT_PIXEL_1X1, DRAW_FILL_FULL); 

        // Display the gradient on the screen
        wavesahre_rgb_lcd_display(BlackImage);
        vTaskDelay(1000);

        Paint_Clear(WHITE);
        // Draw points with increasing sizes
        Paint_DrawPoint(2, 18, BLACK, DOT_PIXEL_1X1, DOT_FILL_RIGHTUP);
        Paint_DrawPoint(2, 20, BLACK, DOT_PIXEL_2X2, DOT_FILL_RIGHTUP);
        Paint_DrawPoint(2, 23, BLACK, DOT_PIXEL_3X3, DOT_FILL_RIGHTUP);
        Paint_DrawPoint(2, 28, BLACK, DOT_PIXEL_4X4, DOT_FILL_RIGHTUP);
        Paint_DrawPoint(2, 33, BLACK, DOT_PIXEL_5X5, DOT_FILL_RIGHTUP);

        // Draw solid and dotted lines
        Paint_DrawLine(20, 5, 80, 65, MAGENTA, DOT_PIXEL_2X2, LINE_STYLE_SOLID);
        Paint_DrawLine(20, 65, 80, 5, MAGENTA, DOT_PIXEL_2X2, LINE_STYLE_SOLID);
        Paint_DrawLine(148, 35, 208, 35, CYAN, DOT_PIXEL_1X1, LINE_STYLE_DOTTED);
        Paint_DrawLine(178, 5, 178, 65, CYAN, DOT_PIXEL_1X1, LINE_STYLE_DOTTED);

        // Draw rectangles (empty and filled)
        Paint_DrawRectangle(20, 5, 80, 65, RED, DOT_PIXEL_2X2, DRAW_FILL_EMPTY);
        Paint_DrawRectangle(85, 5, 145, 65, BLUE, DOT_PIXEL_2X2, DRAW_FILL_FULL);

        // Draw circles (empty and filled)
        Paint_DrawCircle(178, 35, 30, GREEN, DOT_PIXEL_1X1, DRAW_FILL_EMPTY);
        Paint_DrawCircle(240, 35, 30, GREEN, DOT_PIXEL_1X1, DRAW_FILL_FULL);

        // Draw text and numbers
        Paint_DrawString_EN(1, 70, "AaBbCc123", &Font16, RED, WHITE);
        Paint_DrawNum(1, 100, 9.87654321, &Font20, 7, WHITE, BLACK);
        Paint_DrawString_EN(1, 130, "AaBbCc123", &Font20, 0x000f, 0xfff0);
        Paint_DrawString_EN(1, 160, "AaBbCc123", &Font24, RED, WHITE);
        Paint_DrawString_CN(1, 190, "你好Abc", &Font24CN, WHITE, BLUE);

        // Update the display with the newly drawn elements (currently displaying BlackImage)
        wavesahre_rgb_lcd_display(BlackImage);
        vTaskDelay(1000); // Delay for 1000ms to allow the screen to update

        // NOTE: Image arrays removed to save flash space
        // To display images, load them from SD card or SPIFFS instead
        // Example: Paint_BmpWindows(0, 0, your_image, 600, 1024);

    
    }
}

void loop() {
  // Keep backlight on - add a blink pattern to verify I2C is working
  static unsigned long lastToggle = 0;
  static bool backlightState = true;

  if (millis() - lastToggle > 5000) {  // Toggle every 5 seconds as a heartbeat
    lastToggle = millis();
    backlightState = !backlightState;

    if (backlightState) {
      Serial.println("Backlight ON");
      wavesahre_rgb_lcd_bl_on();
    } else {
      Serial.println("Backlight OFF");
      wavesahre_rgb_lcd_bl_off();
    }
  }
}
