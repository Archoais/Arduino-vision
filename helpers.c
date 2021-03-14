#include "helpers.h"
#include <stdlib.h>
#include <math.h>
#include <stdio.h>

// Convert image to grayscale
void grayscale(int height, int width, RGBTRIPLE image[height][width])
{
    for (int h = 0; h < height; h++)
    {
        for (int w = 0; w < width; w++)
        {
            LONG brightness = image[h][w].rgbtRed + image[h][w].rgbtGreen + image[h][w].rgbtBlue;
            
            //Calculating and rounding average of brighness and equalizing all channels to average
            image[h][w].rgbtRed     = round(brightness / 3.0);
            image[h][w].rgbtGreen   = round(brightness / 3.0);
            image[h][w].rgbtBlue    = round(brightness / 3.0);
        }
    }
    return;

}

// Convert image to sepia
void sepia(int height, int width, RGBTRIPLE image[height][width])
{
    //Allocating memory for a copy of image[height][width] and transfering data
    RGBTRIPLE(*copy)[width] = calloc(height, width * sizeof(RGBTRIPLE));
    for (int h = 0; h < height; h++)
    {
        for (int w = 0; w < width; w++)
        {
            copy[h][w] = image[h][w];
        }
    }
    for (int h = 0; h < height; h++)
    {
        for (int w = 0; w < width; w++)
        {   
            //Transforming channels to sepia values
            image[h][w].rgbtRed     = 0.393 * copy[h][w].rgbtRed + 0.769 * copy[h][w].rgbtGreen + 0.189 * copy[h][w].rgbtBlue;
            image[h][w].rgbtGreen   = 0.349 * copy[h][w].rgbtRed + 0.686 * copy[h][w].rgbtGreen + 0.168 * copy[h][w].rgbtBlue;
            image[h][w].rgbtBlue    = 0.272 * copy[h][w].rgbtRed + 0.534 * copy[h][w].rgbtGreen + 0.131 * copy[h][w].rgbtBlue;
            
            //Capping max value of pixel to 255 to avoid overflow
            if (0.393 * copy[h][w].rgbtRed + 0.769 * copy[h][w].rgbtGreen + 0.189 * copy[h][w].rgbtBlue > 255)
            {
                image[h][w].rgbtRed = 255;
            }
            else
            {
                image[h][w].rgbtRed = round(0.393 * copy[h][w].rgbtRed + 0.769 * copy[h][w].rgbtGreen + 0.189 * copy[h][w].rgbtBlue);
            }

            if (0.349 * copy[h][w].rgbtRed + 0.686 * copy[h][w].rgbtGreen + 0.168 * copy[h][w].rgbtBlue > 255)
            {
                image[h][w].rgbtGreen = 255;
            }
            else
            {
                image[h][w].rgbtGreen = round(0.349 * copy[h][w].rgbtRed + 0.686 * copy[h][w].rgbtGreen + 0.168 * copy[h][w].rgbtBlue);
            }

            if (0.272 * copy[h][w].rgbtRed + 0.534 * copy[h][w].rgbtGreen + 0.131 * copy[h][w].rgbtBlue > 255)
            {
                image[h][w].rgbtBlue = 255;
            }
            else
            {
                image[h][w].rgbtBlue = round(0.272 * copy[h][w].rgbtRed + 0.534 * copy[h][w].rgbtGreen + 0.131 * copy[h][w].rgbtBlue);
            }
        }
    }
    free(copy);
    return;
}

// Reflect image horizontally
void reflect(int height, int width, RGBTRIPLE image[height][width])
{
    //Allocating memory for a copy of image[height][width] and transfering data
    RGBTRIPLE(*copy)[width] = calloc(height, width * sizeof(RGBTRIPLE));
    for (int h = 0; h < height; h++)
    {
        for (int w = 0; w < width; w++)
        {
            copy[h][w] = image[h][w];
        }
    }

    for (int h = 0; h < height; h++)
    {
        for (int w = 0; w < width; w++)
        {
            //performing inversion
            image[h][w].rgbtBlue    = copy[h][width - 1 - w].rgbtBlue;
            image[h][w].rgbtGreen   = copy[h][width - 1 - w].rgbtGreen;
            image[h][w].rgbtRed     = copy[h][width - 1 - w].rgbtRed;
        }
    }

    free(copy);
    return;
}

// Blur image
void blur(int height, int width, RGBTRIPLE image[height][width])
{
    //Allocating memory for a copy of image[height][width] and transfering data
    RGBTRIPLE(*copy)[width] = calloc(height, (width) * sizeof(RGBTRIPLE));
    for (int h = 0; h < height; h++)
    {
        for (int w = 0; w < width; w++)
        {
            copy[h][w] = image[h][w];
        }
    }

    for (int h = 0; h < height; h++)
    {
        for (int w = 0; w < width; w++)
        {
            //Left top corner (start of first row)
            if (h == 0 && w == 0) 
            {
                //Averaging each channel separately using the backup of image data.
                image[h][w].rgbtRed  =  round((copy[h][w].rgbtRed + copy[h][w + 1].rgbtRed + copy[h + 1][w + 1].rgbtRed +
                                               copy[h + 1][w].rgbtRed) / 4.0);

                image[h][w].rgbtGreen = round((copy[h][w].rgbtGreen + copy[h][w + 1].rgbtGreen + copy[h + 1][w + 1].rgbtGreen +
                                               copy[h + 1][w].rgbtGreen) / 4.0);

                image[h][w].rgbtBlue =  round((copy[h][w].rgbtBlue + copy[h][w + 1].rgbtBlue + copy[h + 1][w + 1].rgbtBlue +
                                               copy[h + 1][w].rgbtBlue) / 4.0);
            }
            
            //first row sweep
            else if (h == 0 && w > 0 && w < width - 1) 
            {
                //Averaging each channel separately using the backup of image data.
                image[h][w].rgbtRed  =  round((copy[h + 0][w - 1].rgbtRed + copy[h + 0][w + 0].rgbtRed + copy[h + 0][w + 1].rgbtRed +
                                               copy[h + 1][w - 1].rgbtRed + copy[h + 1][w + 0].rgbtRed + copy[h + 1][w + 1].rgbtRed) / 6.0);

                image[h][w].rgbtGreen = round((copy[h + 0][w - 1].rgbtGreen + copy[h + 0][w + 0].rgbtGreen + copy[h + 0][w + 1].rgbtGreen +
                                               copy[h + 1][w - 1].rgbtGreen + copy[h + 1][w + 0].rgbtGreen + copy[h + 1][w + 1].rgbtGreen) / 6.0);

                image[h][w].rgbtBlue =  round((copy[h + 0][w - 1].rgbtBlue + copy[h + 0][w + 0].rgbtBlue + copy[h + 0][w + 1].rgbtBlue +
                                               copy[h + 1][w - 1].rgbtBlue + copy[h + 1][w + 0].rgbtBlue + copy[h + 1][w + 1].rgbtBlue) / 6.0);
            }
            
            //right top corner (end of first row)
            else if (h == 0 && w == width - 1) 
            {
                //Averaging each channel separately using the backup of image data.
                image[h][w].rgbtRed  =  round((copy[h][w].rgbtRed + copy[h][w - 1].rgbtRed + copy[h + 1][w - 1].rgbtRed +
                                               copy[h + 1][w].rgbtRed) / 4.0);

                image[h][w].rgbtGreen = round((copy[h][w].rgbtGreen + copy[h][w - 1].rgbtGreen + copy[h + 1][w - 1].rgbtGreen +
                                               copy[h + 1][w].rgbtGreen) / 4.0);

                image[h][w].rgbtBlue =  round((copy[h][w].rgbtBlue + copy[h][w - 1].rgbtBlue + copy[h + 1][w - 1].rgbtBlue +
                                               copy[h + 1][w].rgbtBlue) / 4.0);
            }
            
            // Middle row first element
            else if (h > 0 && h < height - 1 && w == 0) 
            {
                //Averaging each channel separately using the backup of image data.
                image[h][w].rgbtRed  =  round((copy[h - 1][w].rgbtRed + copy[h - 1][w + 1].rgbtRed +
                                               copy[h + 0][w].rgbtRed + copy[h + 0][w + 1].rgbtRed +
                                               copy[h + 1][w].rgbtRed + copy[h + 1][w + 1].rgbtRed) / 6.0);

                image[h][w].rgbtGreen = round((copy[h - 1][w].rgbtGreen + copy[h - 1][w + 1].rgbtGreen +
                                               copy[h + 0][w].rgbtGreen + copy[h + 0][w + 1].rgbtGreen +
                                               copy[h + 1][w].rgbtGreen + copy[h + 1][w + 1].rgbtGreen) / 6.0);

                image[h][w].rgbtBlue =  round((copy[h - 1][w].rgbtBlue + copy[h - 1][w + 1].rgbtBlue +
                                               copy[h + 0][w].rgbtBlue + copy[h + 0][w + 1].rgbtBlue +
                                               copy[h + 1][w].rgbtBlue + copy[h + 1][w + 1].rgbtBlue) / 6.0);
            }

            // middle row sweep
            else if (h > 0 && h < height - 1 && w > 0 && w < width - 1) 
            {
                //Averaging each channel separately using the backup of image data.
                image[h][w].rgbtRed  =  round((copy[h - 1][w - 1].rgbtRed + copy[h - 1][w].rgbtRed + copy[h - 1][w + 1].rgbtRed +
                                               copy[h + 0][w - 1].rgbtRed + copy[h + 0][w].rgbtRed + copy[h + 0][w + 1].rgbtRed +
                                               copy[h + 1][w - 1].rgbtRed + copy[h + 1][w].rgbtRed + copy[h + 1][w + 1].rgbtRed) / 9.0);

                image[h][w].rgbtGreen = round((copy[h - 1][w - 1].rgbtGreen + copy[h - 1][w].rgbtGreen + copy[h - 1][w + 1].rgbtGreen +
                                               copy[h + 0][w - 1].rgbtGreen + copy[h + 0][w].rgbtGreen + copy[h + 0][w + 1].rgbtGreen +
                                               copy[h + 1][w - 1].rgbtGreen + copy[h + 1][w].rgbtGreen + copy[h + 1][w + 1].rgbtGreen) / 9.0);

                image[h][w].rgbtBlue =  round((copy[h - 1][w - 1].rgbtBlue + copy[h - 1][w].rgbtBlue + copy[h - 1][w + 1].rgbtBlue +
                                               copy[h + 0][w - 1].rgbtBlue + copy[h + 0][w].rgbtBlue + copy[h + 0][w + 1].rgbtBlue +
                                               copy[h + 1][w - 1].rgbtBlue + copy[h + 1][w].rgbtBlue + copy[h + 1][w + 1].rgbtBlue) / 9.0);
            }
            
            //middle row last pixel
            else if (h > 0 && h < height - 1 && w == width - 1) 
            {
                //Averaging each channel separately using the backup of image data.
                image[h][w].rgbtRed  =  round((copy[h - 1][w - 1].rgbtRed + copy[h - 1][w].rgbtRed +
                                               copy[h + 0][w - 1].rgbtRed + copy[h + 0][w].rgbtRed +
                                               copy[h + 1][w - 1].rgbtRed + copy[h + 1][w].rgbtRed) / 6.0);

                image[h][w].rgbtGreen = round((copy[h - 1][w - 1].rgbtGreen + copy[h - 1][w].rgbtGreen +
                                               copy[h + 0][w - 1].rgbtGreen + copy[h + 0][w].rgbtGreen +
                                               copy[h + 1][w - 1].rgbtGreen + copy[h + 1][w].rgbtGreen) / 6.0);

                image[h][w].rgbtBlue =  round((copy[h - 1][w - 1].rgbtBlue + copy[h - 1][w].rgbtBlue +
                                               copy[h + 0][w - 1].rgbtBlue + copy[h + 0][w].rgbtBlue +
                                               copy[h + 1][w - 1].rgbtBlue + copy[h + 1][w].rgbtBlue) / 6.0);
            }

            //bottom left pixel
            else if (h == height - 1 && w == 0) 
            {
                //Averaging each channel separately using the backup of image data.
                image[h][w].rgbtRed  =  round((copy[h][w].rgbtRed + copy[h - 1][w].rgbtRed + copy[h - 1][w + 1].rgbtRed +
                                               copy[h][w + 1].rgbtRed) / 4.0);

                image[h][w].rgbtGreen = round((copy[h][w].rgbtGreen + copy[h - 1][w].rgbtGreen + copy[h - 1][w + 1].rgbtGreen +
                                               copy[h][w + 1].rgbtGreen) / 4.0);

                image[h][w].rgbtBlue =  round((copy[h][w].rgbtBlue + copy[h - 1][w].rgbtBlue + copy[h - 1][w + 1].rgbtBlue +
                                               copy[h][w + 1].rgbtBlue) / 4.0);
            }
            
            //last row sweep
            else if (h == height - 1 && w > 0 && w < width - 1) 
            {
                //Averaging each channel separately using the backup of image data.
                image[h][w].rgbtRed  =  round((copy[h - 1][w - 1].rgbtRed + copy[h - 1][w + 0].rgbtRed + copy[h - 1][w + 1].rgbtRed +
                                               copy[h + 0][w - 1].rgbtRed + copy[h + 0][w + 0].rgbtRed + copy[h + 0][w + 1].rgbtRed) / 6.0);

                image[h][w].rgbtGreen = round((copy[h - 1][w - 1].rgbtGreen + copy[h - 1][w + 0].rgbtGreen + copy[h - 1][w + 1].rgbtGreen +
                                               copy[h + 0][w - 1].rgbtGreen + copy[h + 0][w + 0].rgbtGreen + copy[h + 0][w + 1].rgbtGreen) / 6.0);

                image[h][w].rgbtBlue =  round((copy[h - 1][w - 1].rgbtBlue + copy[h - 1][w + 0].rgbtBlue + copy[h - 1][w + 1].rgbtBlue +
                                               copy[h + 0][w - 1].rgbtBlue + copy[h + 0][w + 0].rgbtBlue + copy[h + 0][w + 1].rgbtBlue) / 6.0);
            }

            //bottom right pixel
            else if (h == height - 1 && w == width - 1) 
            {
                //Averaging each channel separately using the backup of image data.
                image[h][w].rgbtRed  =  round((copy[h][w].rgbtRed + copy[h - 1][w].rgbtRed + copy[h - 1][w - 1].rgbtRed +
                                               copy[h][w - 1].rgbtRed) / 4.0);

                image[h][w].rgbtGreen = round((copy[h][w].rgbtGreen + copy[h - 1][w].rgbtGreen + copy[h - 1][w - 1].rgbtGreen +
                                               copy[h][w - 1].rgbtGreen) / 4.0);

                image[h][w].rgbtBlue =  round((copy[h][w].rgbtBlue + copy[h - 1][w].rgbtBlue + copy[h - 1][w - 1].rgbtBlue +
                                               copy[h][w - 1].rgbtBlue) / 4.0);
            }
        }
    }

    free(copy);
    return;
}



// Edge  detection
void edges(int height, int width, RGBTRIPLE image[height][width])
{
    for (int h = 0; h < height; h++)
    {
        for (int w = 0; w < width; w++)
        {
            LONG brightness = image[h][w].rgbtRed + image[h][w].rgbtGreen + image[h][w].rgbtBlue;
            
            //Calculating and rounding average of brighness and equalizing all channels to average
            image[h][w].rgbtRed     = round(brightness / 3.0);
            image[h][w].rgbtGreen   = round(brightness / 3.0);
            image[h][w].rgbtBlue    = round(brightness / 3.0);
        }
    }
    
    
    //Allocating memory for a copy of image[height][width] and transfering data
    RGBTRIPLE(*copy)[width] = calloc(height, (width) * sizeof(RGBTRIPLE));
    for (int h = 0; h < height; h++)
    {
        for (int w = 0; w < width; w++)
        {
            copy[h][w] = image[h][w];
        }
    }
    
    double Gx_red;
    double Gx_green;
    double Gx_blue;
    
    double Gy_red;
    double Gy_green;
    double Gy_blue;

    for (int h = 0; h < height; h++)
    {
        for (int w = 0; w < width; w++)
        {
            //Left top corner (start of first row)
            if (h == 0 && w == 0) 
            {
                //Averaging each channel separately using the backup of image data.
                Gx_red  =   (float)(copy[h + 0][w].rgbtRed*(0) + copy[h + 0][w + 1].rgbtRed*(2) + 
                                    copy[h + 1][w].rgbtRed*(0) + copy[h + 1][w + 1].rgbtRed*(1));

                Gx_green =  (float)(copy[h + 0][w].rgbtGreen*(0) + copy[h + 0][w + 1].rgbtGreen*(2) + 
                                    copy[h + 1][w].rgbtGreen*(0) + copy[h + 1][w + 1].rgbtGreen*(1));

                Gx_blue  =  (float)(copy[h + 0][w].rgbtBlue*(0) + copy[h + 0][w + 1].rgbtBlue*(2) + 
                                    copy[h + 1][w].rgbtBlue*(0) + copy[h + 1][w + 1].rgbtBlue*(1));
                            
                Gy_red  =   (float)(copy[h + 0][w].rgbtRed*(0) + copy[h + 0][w + 1].rgbtRed*(0) + 
                                    copy[h + 1][w].rgbtRed*(2) + copy[h + 1][w + 1].rgbtRed*(1));

                Gy_green =  (float)(copy[h + 0][w].rgbtGreen*(0) + copy[h + 0][w + 1].rgbtGreen*(0) + 
                                    copy[h + 1][w].rgbtGreen*(2) + copy[h + 1][w + 1].rgbtGreen*(1));

                Gy_blue  =  (float)(copy[h + 0][w].rgbtBlue*(0) + copy[h + 0][w + 1].rgbtBlue*(0) + 
                                    copy[h + 1][w].rgbtBlue*(2) + copy[h + 1][w + 1].rgbtBlue*(1));
            }
            
            //first row sweep
            else if (h == 0 && w > 0 && w < width - 1) 
            {
                //Averaging each channel separately using the backup of image data.
                Gx_red  =   (float)(copy[h + 0][w - 1].rgbtRed*(-2) + copy[h + 0][w + 0].rgbtRed*(0) + copy[h + 0][w + 1].rgbtRed*(2) +
                                    copy[h + 1][w - 1].rgbtRed*(-1) + copy[h + 1][w + 0].rgbtRed*(0) + copy[h + 1][w + 1].rgbtRed*(1));

                Gx_green =  (float)(copy[h + 0][w - 1].rgbtGreen*(-2) + copy[h + 0][w + 0].rgbtGreen*(0) + copy[h + 0][w + 1].rgbtGreen*(2) +
                                    copy[h + 1][w - 1].rgbtGreen*(-1) + copy[h + 1][w + 0].rgbtGreen*(0) + copy[h + 1][w + 1].rgbtGreen*(1));

                Gx_blue  =  (float)(copy[h + 0][w - 1].rgbtBlue*(-2) + copy[h + 0][w + 0].rgbtBlue*(0) + copy[h + 0][w + 1].rgbtBlue*(2) +
                                    copy[h + 1][w - 1].rgbtBlue*(-1) + copy[h + 1][w + 0].rgbtBlue*(0) + copy[h + 1][w + 1].rgbtBlue*(1));
                            
                Gy_red  =   (float)(copy[h + 0][w - 1].rgbtRed*(0) + copy[h + 0][w + 0].rgbtRed*(0) + copy[h + 0][w + 1].rgbtRed*(0) +
                                    copy[h + 1][w - 1].rgbtRed*(1) + copy[h + 1][w + 0].rgbtRed*(2) + copy[h + 1][w + 1].rgbtRed*(1));

                Gy_green =  (float)(copy[h + 0][w - 1].rgbtGreen*(0) + copy[h + 0][w + 0].rgbtGreen*(0) + copy[h + 0][w + 1].rgbtGreen*(0) +
                                    copy[h + 1][w - 1].rgbtGreen*(1) + copy[h + 1][w + 0].rgbtGreen*(2) + copy[h + 1][w + 1].rgbtGreen*(1));

                Gy_blue  =  (float)(copy[h + 0][w - 1].rgbtBlue*(0) + copy[h + 0][w + 0].rgbtBlue*(0) + copy[h + 0][w + 1].rgbtBlue*(0) +
                                    copy[h + 1][w - 1].rgbtBlue*(1) + copy[h + 1][w + 0].rgbtBlue*(2) + copy[h + 1][w + 1].rgbtBlue*(1));
            }
            
            //right top corner (end of first row)
            else if (h == 0 && w == width - 1) 
            {
                //Averaging each channel separately using the backup of image data.
                Gx_red  =   (float)(copy[h + 0][w - 1].rgbtRed*(-2) + copy[h + 0][w].rgbtRed*(0) +
                                    copy[h + 1][w - 1].rgbtRed*(-1) + copy[h + 1][w].rgbtRed*(0));

                Gx_green =  (float)(copy[h + 0][w - 1].rgbtGreen*(-2) + copy[h + 0][w].rgbtGreen*(0) +
                                    copy[h + 1][w - 1].rgbtGreen*(-1) + copy[h + 1][w].rgbtGreen*(0));

                Gx_blue  =  (float)(copy[h + 0][w - 1].rgbtBlue*(-2) + copy[h + 0][w].rgbtBlue*(0) +
                                    copy[h + 1][w - 1].rgbtBlue*(-1) + copy[h + 1][w].rgbtBlue*(0));
                            
                Gy_red  =   (float)(copy[h + 0][w - 1].rgbtRed*(0) + copy[h + 0][w].rgbtRed*(0) +
                                    copy[h + 1][w - 1].rgbtRed*(1) + copy[h + 1][w].rgbtRed*(2));

                Gy_green =  (float)(copy[h + 0][w - 1].rgbtGreen*(0) + copy[h + 0][w].rgbtGreen*(0) +
                                    copy[h + 1][w - 1].rgbtGreen*(1) + copy[h + 1][w].rgbtGreen*(2));

                Gy_blue  =  (float)(copy[h + 0][w - 1].rgbtBlue*(0) + copy[h + 0][w].rgbtBlue*(0) +
                                    copy[h + 1][w - 1].rgbtBlue*(1) + copy[h + 1][w].rgbtBlue*(2));
            }
            
            // Middle row first element
            else if (h > 0 && h < height - 1 && w == 0) 
            {
                //Averaging each channel separately using the backup of image data.
                Gx_red  =   (float)(copy[h - 1][w].rgbtRed*(0) + copy[h - 1][w + 1].rgbtRed*(1) +
                                    copy[h + 0][w].rgbtRed*(0) + copy[h + 0][w + 1].rgbtRed*(2) +
                                    copy[h + 1][w].rgbtRed*(0) + copy[h + 1][w + 1].rgbtRed*(1));

                Gx_green =  (float)(copy[h - 1][w].rgbtGreen*(0) + copy[h - 1][w + 1].rgbtGreen*(1) +
                                    copy[h + 0][w].rgbtGreen*(0) + copy[h + 0][w + 1].rgbtGreen*(2) +
                                    copy[h + 1][w].rgbtGreen*(0) + copy[h + 1][w + 1].rgbtGreen*(1));

                Gx_blue =   (float)(copy[h - 1][w].rgbtBlue*(0) + copy[h - 1][w + 1].rgbtBlue*(1) +
                                    copy[h + 0][w].rgbtBlue*(0) + copy[h + 0][w + 1].rgbtBlue*(2) +
                                    copy[h + 1][w].rgbtBlue*(0) + copy[h + 1][w + 1].rgbtBlue*(1));
                            
                Gy_red  =   (float)(copy[h - 1][w].rgbtRed*(-2) + copy[h - 1][w + 1].rgbtRed*(-1) +
                                    copy[h + 0][w].rgbtRed*( 0) + copy[h + 0][w + 1].rgbtRed*( 0) +
                                    copy[h + 1][w].rgbtRed*( 2) + copy[h + 1][w + 1].rgbtRed*( 1));

                Gy_green =  (float)(copy[h - 1][w].rgbtGreen*(-2) + copy[h - 1][w + 1].rgbtGreen*(-1) +
                                    copy[h + 0][w].rgbtGreen*( 0) + copy[h + 0][w + 1].rgbtGreen*( 0) +
                                    copy[h + 1][w].rgbtGreen*( 2) + copy[h + 1][w + 1].rgbtGreen*( 1));

                Gy_blue  =  (float)(copy[h - 1][w].rgbtBlue*(-2) + copy[h - 1][w + 1].rgbtBlue*(-1) +
                                    copy[h + 0][w].rgbtBlue*( 0) + copy[h + 0][w + 1].rgbtBlue*( 0) +
                                    copy[h + 1][w].rgbtBlue*( 2) + copy[h + 1][w + 1].rgbtBlue*( 1));
            }

            // middle row sweep
            else if (h > 0 && h < height - 1 && w > 0 && w < width - 1) 
            {
                //Averaging each channel separately using the backup of image data.
                Gx_red   =  (float)(copy[h - 1][w - 1].rgbtRed*(-1) + copy[h - 1][w].rgbtRed*(0) + copy[h - 1][w + 1].rgbtRed*(1) +
                                    copy[h + 0][w - 1].rgbtRed*(-2) + copy[h + 0][w].rgbtRed*(0) + copy[h + 0][w + 1].rgbtRed*(2) +
                                    copy[h + 1][w - 1].rgbtRed*(-1) + copy[h + 1][w].rgbtRed*(0) + copy[h + 1][w + 1].rgbtRed*(1));

                Gx_green =  (float)(copy[h - 1][w - 1].rgbtGreen*(-1) + copy[h - 1][w].rgbtGreen*(0) + copy[h - 1][w + 1].rgbtGreen*(1) +
                                    copy[h + 0][w - 1].rgbtGreen*(-2) + copy[h + 0][w].rgbtGreen*(0) + copy[h + 0][w + 1].rgbtGreen*(2) +
                                    copy[h + 1][w - 1].rgbtGreen*(-1) + copy[h + 1][w].rgbtGreen*(0) + copy[h + 1][w + 1].rgbtGreen*(1));

                Gx_blue  =  (float)(copy[h - 1][w - 1].rgbtBlue*(-1) + copy[h - 1][w].rgbtBlue*(0) + copy[h - 1][w + 1].rgbtBlue*(1) +
                                    copy[h + 0][w - 1].rgbtBlue*(-2) + copy[h + 0][w].rgbtBlue*(0) + copy[h + 0][w + 1].rgbtBlue*(2) +
                                    copy[h + 1][w - 1].rgbtBlue*(-1) + copy[h + 1][w].rgbtBlue*(0) + copy[h + 1][w + 1].rgbtBlue*(1));
                            
                Gy_red   =  (float)(copy[h - 1][w - 1].rgbtRed*(-1) + copy[h - 1][w].rgbtRed*(-2) + copy[h - 1][w + 1].rgbtRed*(-1) +
                                    copy[h + 0][w - 1].rgbtRed*( 0) + copy[h + 0][w].rgbtRed*( 0) + copy[h + 0][w + 1].rgbtRed*( 0) +
                                    copy[h + 1][w - 1].rgbtRed*( 1) + copy[h + 1][w].rgbtRed*( 2) + copy[h + 1][w + 1].rgbtRed*( 1));

                Gy_green =  (float)(copy[h - 1][w - 1].rgbtGreen*(-1) + copy[h - 1][w].rgbtGreen*(-2) + copy[h - 1][w + 1].rgbtGreen*(-1) +
                                    copy[h + 0][w - 1].rgbtGreen*( 0) + copy[h + 0][w].rgbtGreen*( 0) + copy[h + 0][w + 1].rgbtGreen*( 0) +
                                    copy[h + 1][w - 1].rgbtGreen*( 1) + copy[h + 1][w].rgbtGreen*( 2) + copy[h + 1][w + 1].rgbtGreen*( 1));

                Gy_blue  =  (float)(copy[h - 1][w - 1].rgbtBlue*(-1) + copy[h - 1][w].rgbtBlue*(-2) + copy[h - 1][w + 1].rgbtBlue*(-1) +
                                    copy[h + 0][w - 1].rgbtBlue*( 0) + copy[h + 0][w].rgbtBlue*( 0) + copy[h + 0][w + 1].rgbtBlue*( 0) +
                                    copy[h + 1][w - 1].rgbtBlue*( 1) + copy[h + 1][w].rgbtBlue*( 2) + copy[h + 1][w + 1].rgbtBlue*( 1));
            }
            
            //middle row last pixel
            else if (h > 0 && h < height - 1 && w == width - 1) 
            {
                //Averaging each channel separately using the backup of image data.
                Gx_red   =  (float)(copy[h - 1][w - 1].rgbtRed*(-1) + copy[h - 1][w].rgbtRed*(0) +
                                    copy[h + 0][w - 1].rgbtRed*(-2) + copy[h + 0][w].rgbtRed*(0) +
                                    copy[h + 1][w - 1].rgbtRed*(-1) + copy[h + 1][w].rgbtRed*(0));

                Gx_green =  (float)(copy[h - 1][w - 1].rgbtGreen*(-1) + copy[h - 1][w].rgbtGreen*(0) +
                                    copy[h + 0][w - 1].rgbtGreen*(-2) + copy[h + 0][w].rgbtGreen*(0) +
                                    copy[h + 1][w - 1].rgbtGreen*(-1) + copy[h + 1][w].rgbtGreen*(0));

                Gx_blue  =  (float)(copy[h - 1][w - 1].rgbtBlue*(-1) + copy[h - 1][w].rgbtBlue*(0) +
                                    copy[h + 0][w - 1].rgbtBlue*(-2) + copy[h + 0][w].rgbtBlue*(0) +
                                    copy[h + 1][w - 1].rgbtBlue*(-1) + copy[h + 1][w].rgbtBlue*(0));
                            
                Gy_red   =  (float)(copy[h - 1][w - 1].rgbtRed*(-1) + copy[h - 1][w].rgbtRed*(-2) +
                                    copy[h + 0][w - 1].rgbtRed*( 0) + copy[h + 0][w].rgbtRed*( 0) +
                                    copy[h + 1][w - 1].rgbtRed*( 1) + copy[h + 1][w].rgbtRed*( 2));

                Gy_green =  (float)(copy[h - 1][w - 1].rgbtGreen*(-1) + copy[h - 1][w].rgbtGreen*(-2) +
                                    copy[h + 0][w - 1].rgbtGreen*( 0) + copy[h + 0][w].rgbtGreen* (0) +
                                    copy[h + 1][w - 1].rgbtGreen*( 1) + copy[h + 1][w].rgbtGreen*( 2));

                Gy_blue  =  (float)(copy[h - 1][w - 1].rgbtBlue*(-1) + copy[h - 1][w].rgbtBlue*(-2) +
                                    copy[h + 0][w - 1].rgbtBlue*( 0) + copy[h + 0][w].rgbtBlue*( 0) +
                                    copy[h + 1][w - 1].rgbtBlue*( 1) + copy[h + 1][w].rgbtBlue*( 2));
            }

            //bottom left pixel
            else if (h == height - 1 && w == 0) 
            {
                //Averaging each channel separately using the backup of image data.
                Gx_red  =   (float)(copy[h - 1][w].rgbtRed*(0) + copy[h - 1][w + 1].rgbtRed*(1) +
                                    copy[h + 0][w].rgbtRed*(0) + copy[h + 0][w + 1].rgbtRed*(2));

                Gx_green =  (float)(copy[h - 1][w].rgbtGreen*(0) + copy[h - 1][w + 1].rgbtGreen*(1) +
                                    copy[h + 0][w].rgbtGreen*(0) + copy[h + 0][w + 1].rgbtGreen*(2));

                Gx_blue =   (float)(copy[h - 1][w].rgbtBlue*(0) + copy[h - 1][w + 1].rgbtBlue*(1) +
                                    copy[h + 0][w].rgbtBlue*(0) + copy[h + 0][w + 1].rgbtBlue*(2));
                            
                Gy_red  =   (float)(copy[h - 1][w].rgbtRed*(-2) + copy[h - 1][w + 1].rgbtRed*(-1) +
                                    copy[h + 0][w].rgbtRed*( 0) + copy[h + 0][w + 1].rgbtRed*( 0));

                Gy_green =  (float)(copy[h - 1][w].rgbtGreen*(-2) + copy[h - 1][w + 1].rgbtGreen*(-1) +
                                    copy[h + 0][w].rgbtGreen*( 0) + copy[h + 0][w + 1].rgbtGreen*( 0));

                Gy_blue =   (float)(copy[h - 1][w].rgbtBlue*(-2) + copy[h - 1][w + 1].rgbtBlue*(-1) +
                                    copy[h + 0][w].rgbtBlue*( 0) + copy[h + 0][w + 1].rgbtBlue*( 0));
            }
            
            //last row sweep
            else if (h == height - 1 && w > 0 && w < width - 1) 
            {
                //Averaging each channel separately using the backup of image data.
                Gx_red  =   (float)(copy[h - 1][w - 1].rgbtRed*(-1) + copy[h - 1][w + 0].rgbtRed*(0) + copy[h - 1][w + 1].rgbtRed*(1) +
                                    copy[h + 0][w - 1].rgbtRed*(-2) + copy[h + 0][w + 0].rgbtRed*(0) + copy[h + 0][w + 1].rgbtRed*(2));

                Gx_green =  (float)(copy[h - 1][w - 1].rgbtGreen*(-1) + copy[h - 1][w + 0].rgbtGreen*(0) + copy[h - 1][w + 1].rgbtGreen*(1) +
                                    copy[h + 0][w - 1].rgbtGreen*(-2) + copy[h + 0][w + 0].rgbtGreen*(0) + copy[h + 0][w + 1].rgbtGreen*(2));

                Gx_blue  =  (float)(copy[h - 1][w - 1].rgbtBlue*(-1) + copy[h - 1][w + 0].rgbtBlue*(0) + copy[h - 1][w + 1].rgbtBlue*(1) +
                                    copy[h + 0][w - 1].rgbtBlue*(-2) + copy[h + 0][w + 0].rgbtBlue*(0) + copy[h + 0][w + 1].rgbtBlue*(2));
                            
                Gy_red  =   (float)(copy[h - 1][w - 1].rgbtRed*(-1) + copy[h - 1][w + 0].rgbtRed*(-2) + copy[h - 1][w + 1].rgbtRed*(-1) +
                                    copy[h + 0][w - 1].rgbtRed*( 0) + copy[h + 0][w + 0].rgbtRed*( 0) + copy[h + 0][w + 1].rgbtRed*( 0));

                Gy_green =  (float)(copy[h - 1][w - 1].rgbtGreen*(-1) + copy[h - 1][w + 0].rgbtGreen*(-2) + copy[h - 1][w + 1].rgbtGreen*(-1) +
                                    copy[h + 0][w - 1].rgbtGreen*( 0) + copy[h + 0][w + 0].rgbtGreen*( 0) + copy[h + 0][w + 1].rgbtGreen*( 0));

                Gy_blue  =  (float)(copy[h - 1][w - 1].rgbtBlue*(-1) + copy[h - 1][w + 0].rgbtBlue*(-2) + copy[h - 1][w + 1].rgbtBlue*(-1) +
                                    copy[h + 0][w - 1].rgbtBlue*( 0) + copy[h + 0][w + 0].rgbtBlue*( 0) + copy[h + 0][w + 1].rgbtBlue*( 0));
            }

            //bottom right pixel
            else if (h == height - 1 && w == width - 1) 
            {
                //Averaging each channel separately using the backup of image data.
                Gx_red   =  (float)(copy[h - 1][w - 1].rgbtRed*(-1) + copy[h - 1][w + 0].rgbtRed*(0) + 
                                    copy[h + 0][w - 1].rgbtRed*(-2) + copy[h + 0][w + 0].rgbtRed*(0));

                Gx_green =  (float)(copy[h - 1][w - 1].rgbtGreen*(-1)  + copy[h - 1][w + 0].rgbtGreen*(0) + 
                                    copy[h + 0][w - 1].rgbtGreen*(-2)  + copy[h + 0][w + 0].rgbtGreen*(0));

                Gx_blue  =  (float)(copy[h - 1][w - 1].rgbtBlue*(-1) + copy[h - 1][w + 0].rgbtBlue*(0) + 
                                    copy[h + 0][w - 1].rgbtBlue*(-2) + copy[h + 0][w + 0].rgbtBlue*(0));
                            
                Gy_red   =  (float)(copy[h - 1][w - 1].rgbtRed*(-1) + copy[h - 1][w + 0].rgbtRed*(-2) + 
                                    copy[h + 0][w - 1].rgbtRed*( 0) + copy[h + 0][w + 0].rgbtRed*( 0));

                Gy_green =  (float)(copy[h - 1][w - 1].rgbtGreen*(-1)  + copy[h - 1][w + 0].rgbtGreen*(-2) + 
                                    copy[h + 0][w - 1].rgbtGreen*( 0)  + copy[h + 0][w + 0].rgbtGreen*( 0));

                Gy_blue  =  (float)(copy[h - 1][w - 1].rgbtBlue*(-1) + copy[h - 1][w + 0].rgbtBlue*(-2) + 
                                    copy[h + 0][w - 1].rgbtBlue*( 0) + copy[h + 0][w + 0].rgbtBlue*( 0));
            }
            
            image[h][w].rgbtRed     = round (sqrt ((float)(Gx_red*Gx_red      + Gy_red*Gy_red)));
            image[h][w].rgbtGreen   = round (sqrt ((float)(Gx_green*Gx_green  + Gy_green*Gy_green)));
            image[h][w].rgbtBlue    = round (sqrt ((float)(Gx_blue*Gx_blue    + Gy_blue*Gy_blue)));
            
            if (image[h][w].rgbtRed > 255)
            {
                image[h][w].rgbtRed = 255;
            }
            if (image[h][w].rgbtGreen > 255)
            {
                image[h][w].rgbtGreen = 255;
            }
            if (image[h][w].rgbtBlue > 255)
            {
                image[h][w].rgbtBlue = 255;
            }
        }
    }

    
    RGBTRIPLE(*avrg)[width] = calloc(height, (width) * sizeof(RGBTRIPLE));
    for (int h = 0; h < height; h++)
    {
        for (int w = 0; w < width; w++)
        {
            avrg[h][w] = image[h][w];
        }
    }
    
    //Deleting garbage grey pixels
    for (int h = 0; h < height; h++)
    {
        for (int w = 0; w < width; w++)
        {
            //Capping lower grey levels less tha 150 to zero
            
            int cap = 150;
            
            if(avrg[h][w].rgbtRed < cap)
            {
                avrg[h][w].rgbtRed     = 0; 
            }
            
            if(avrg[h][w].rgbtRed >= cap)
            {
                avrg[h][w].rgbtRed     = 255;
            }
            
            if(avrg[h][w].rgbtGreen < cap)
            {
                avrg[h][w].rgbtGreen     = 0; 
            }
        
            if(avrg[h][w].rgbtGreen >= cap)
            {
                avrg[h][w].rgbtGreen     = 255; 
            }
            
            if(avrg[h][w].rgbtBlue < cap)
            {
                avrg[h][w].rgbtBlue     = 0; 
            }
            
            if(avrg[h][w].rgbtBlue >= cap)
            {
                avrg[h][w].rgbtBlue     = 255; 
            }
        }
    }
    
    
    
    
    //stray pixel cleaner
    for (int h = 2; h < height-2; h++)
    {
        for (int w = 2; w < width-2; w++)
        {
            int n = 3;
            int h_sample[1 + (2 * n)];
            int w_sample[1 + (2 * n)];
            
            for(int i = 0; i < 1 + 2*n; i++)
            {
                h_sample[i] = h - n + i;
                w_sample[i] = w - n + i;
            }
            
            double pixel_sum    = 0;
            
            for (int hi = 0; hi < 1 + 2*n; hi++)
            {
                for (int wj = 0; wj < 1 + 2*n; wj++)
                {
                    if (h_sample[hi] >= 0 && h_sample[hi] < height && w_sample[wj] >= 0 && w_sample[wj] < width)
                    {
                        pixel_sum += avrg[hi][wj].rgbtRed + avrg[hi][wj].rgbtGreen + avrg[hi][wj].rgbtBlue;
                    }
                }
            }
            
            if (pixel_sum < 255*6)
            {
                avrg[h][w].rgbtRed = 0;
                avrg[h][w].rgbtGreen = 0;
                avrg[h][w].rgbtBlue = 0;
            }
            
            if(avrg[h][w].rgbtRed == 255)
            {
                image[h][w].rgbtRed     = 255;
                image[h][w].rgbtGreen   = 0;
                image[h][w].rgbtBlue    = 0;
            }
            
        }
    }
    
    
    int high_limit = 0;
    for (int h = 2; h < height-2; h++)
    {
        double scan = 0;
        for (int w = 2; w < width-2; w++)
        {
            scan += avrg[h][w].rgbtGreen;
            
            if (scan == 255*5)
            {
                high_limit = h - 2;
                //h = height - 3;
            }
        }

    }
    
    int low_limit = 0;
    for (int h = (height - 2); h > 0; h--)
    {
        double scan = 0;
        for (int w = 2; w < width-2; w++)
        {
            scan += avrg[h][w].rgbtGreen;
            
            if (scan == 255*5)
            {
                low_limit = h - 2;
            }
        }

    }
    
    int left_limit = 0;
    for (int w = 2; w < width-2; w++)
    {
        double scan = 0;
        for (int h = 2; h < height-2; h++)
        {
            scan += avrg[h][w].rgbtGreen;
            
            if (scan == 255*5)
            {
                left_limit = w - 2;
                //h = height - 3;
            }
        }

    }
    
    int right_limit = 0;
    for (int w = (width - 2); w > 0; w--)
    {
        double scan = 0;
        for (int h = (height - 2); h > 0; h--)
        {
            scan += avrg[h][w].rgbtGreen;
            
            if (scan == 255*5)
            {
                right_limit = w - 2;
            }
        }

    }
    
 
    for (int h = 0; h < height; h++)
    {
        for (int w = 0; w < width; w++)
        {
            if ((h == high_limit || h == low_limit) && w <= left_limit && w >= right_limit)
            {
                image[h][w].rgbtRed   = 0;
                image[h][w].rgbtGreen = 255;
                image[h][w].rgbtBlue  = 0;
            }
            if ((w == left_limit || w == right_limit) && h <= high_limit && h >= low_limit)
            {
                image[h][w].rgbtRed   = 0;
                image[h][w].rgbtGreen = 255;
                image[h][w].rgbtBlue  = 0;
            }
        }
    }
    
    int center_height = low_limit + (int)((high_limit - low_limit)/2.0);
    int center_width  = right_limit + (int)((left_limit - right_limit)/2.0);
    
    printf("            %i\n", center_height);
    //printf("%i                        ", left_limit);
    //printf("%i\n", right_limit);
    printf("            %i\n", center_width);
    
    int dot = (int)round((center_height + center_width)/(2.0*15));
    for (int h = 0; h < dot; h++)
    {
        for (int w = 0; w < dot; w++)
        {
            image[center_height-h][center_width-w].rgbtRed   = 0;
            image[center_height-h][center_width-w].rgbtGreen = 0;
            image[center_height-h][center_width-w].rgbtBlue  = 255;
        }
    }
            
  

    free(copy);
    free(avrg);
    return;
}
