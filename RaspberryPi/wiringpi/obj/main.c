#include <stdio.h>
#include <stdlib.h>     //exit()
#include <signal.h>     //signal()
#include <time.h>
#include "GUI_Paint.h"
#include "GUI_BMPfile.h"
#include "ImageData.h"
#include "EPD_2in13.h"

void  Handler(int signo)
{
    //System Exit
    printf("\r\nHandler:Goto Sleep mode\r\n");
    EPD_Sleep();
    DEV_ModuleExit();

    exit(0);
}

int main(void)
{
    printf("2.13inch e-Paper demo\r\n");
    DEV_ModuleInit();

    // Exception handling:ctrl + c
    signal(SIGINT, Handler);

    if(EPD_Init(FULL_UPDATE) != 0) {
        printf("e-Paper init failed\r\n");
    }
    EPD_Clear();
    DEV_Delay_ms(500);

    //Create a new image cache
    UBYTE *ImageBuffer;
    UWORD Imagesize = ((EPD_WIDTH % 8 == 0)? (EPD_WIDTH / 8 ): (EPD_WIDTH / 8 + 1)) * EPD_HEIGHT;
    if((ImageBuffer = (UBYTE *)malloc(Imagesize)) == NULL) {
        printf("Failed to apply for memory...\r\n");
        exit(0);
    }
    printf("Paint_NewImage\r\n");
    Paint_NewImage(ImageBuffer, EPD_WIDTH, EPD_HEIGHT, 270, WHITE);
    Paint_SetMirroring(MIRROR_HORIZONTAL); //

   // show bmp    
    printf("show bmp\r\n");
    for (int i = 0; i < 1; i++) {
	    Paint_SelectImage(ImageBuffer);

	    /* Get the LATEST quote and date */
	    char lastQuote[255], lastDate[255];
	    int  lastQuote_len = 0, lastDate_len = 0;

	    FILE *quoteFp = fopen("/home/pi/lastQuote.txt", "r"); if(quoteFp) { lastQuote_len = fread(&lastQuote[0], 1, 255, quoteFp); fclose(quoteFp);}
	    FILE *dateFp = fopen("/home/pi/lastUpdate.txt", "r"); if (dateFp) { lastDate_len  = fread(&lastDate[0], 1, 255, dateFp);   fclose(dateFp);}

	    /* safely close c-strings */
	    lastQuote[lastQuote_len-1] = 0;
	    lastDate[lastDate_len-1]   = 0;

	    printf("Printing quote : %s time : %s\n", lastQuote, lastDate);

	    Paint_Clear(WHITE);
	    GUI_ReadBmp("/home/pi/eInkRPiCryptoTicker/RaspberryPi/wiringpi/pic/BitcoinLogo.BMP", 0, 0);

	    Paint_DrawString_EN(100, 10, "EUR", &Font20, WHITE, BLACK);
	    Paint_DrawString_EN(100, 30, lastQuote, &Font20, WHITE, BLACK);
	    Paint_DrawString_EN(40, 110, lastDate, &Font12, WHITE, BLACK);
	    EPD_Display(ImageBuffer);
	    DEV_Delay_ms(5000);
    }




    printf("Goto Sleep mode...\r\n");
    EPD_Sleep();
    
    if(NULL != ImageBuffer){
        free(ImageBuffer);
        ImageBuffer = NULL;        
    }
    return 0;
}
