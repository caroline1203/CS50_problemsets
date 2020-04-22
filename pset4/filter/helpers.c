#include "helpers.h"
#include <math.h>

// Convert image to grayscale
void grayscale(int height, int width, RGBTRIPLE image[height][width])
{
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            //average of each color
            BYTE newByte = round((image[i][j].rgbtBlue + image[i][j].rgbtGreen + image[i][j].rgbtRed) / 3.0);
            image[i][j].rgbtBlue = newByte;
            image[i][j].rgbtGreen = newByte;
            image[i][j].rgbtRed = newByte;
        }
    }
    return;
}

// Reflect image horizontally
void reflect(int height, int width, RGBTRIPLE image[height][width])
{
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width / 2; j++)
        {
            //swap of each pixel horizontal
            BYTE blue = image[i][j].rgbtBlue;
            BYTE green = image[i][j].rgbtGreen;
            BYTE red = image[i][j].rgbtRed;
            image[i][j].rgbtBlue = image[i][width - 1 - j].rgbtBlue;
            image[i][j].rgbtGreen = image[i][width - 1 - j].rgbtGreen;
            image[i][j].rgbtRed = image[i][width - 1 - j].rgbtRed;
            image[i][width - 1 - j].rgbtBlue = blue;
            image[i][width - 1 - j].rgbtGreen = green;
            image[i][width - 1 - j].rgbtRed = red;
        }
    }
    return;
}

// Blur image
void blur(int height, int width, RGBTRIPLE image[height][width])
{
    RGBTRIPLE copyImage[height][width];

    //keep the new color
    BYTE color[height][width][3];

    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            float count = 0;
            int blue = 0;
            int gree n = 0;
            int red = 0;
            
            //add surround pixels
            for (int m = -1; m < 2; m++)
            {
                for (int n = -1; n < 2; n++)
                {
                    if (i + m >= 0 && i + m < height && j + n >= 0 && j + n < width)
                    {
                        blue += image[i + m][j + n].rgbtBlue;
                        green += image[i + m][j + n].rgbtGreen;
                        red += image[i + m][j + n].rgbtRed;
                        count++;
                    }

                }
            }
            
            //get the average
            color[i][j][0] = round(blue / count);
            color[i][j][1] = round(green / count);
            color[i][j][2] = round(red / count);
        }
    }

    //set the image with new pixel value
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {

            image[i][j].rgbtBlue = color[i][j][0];
            image[i][j].rgbtGreen = color[i][j][1];
            image[i][j].rgbtRed = color[i][j][2];
        }
    }
    return;
}

// Detect edges
void edges(int height, int width, RGBTRIPLE image[height][width])
{
    //the reference number
    int kernel[3][3] = {{-1, 0, 1}, {-2, 0, 2}, {-1, 0, 1}};

    //store the new color pixel value
    int color[height][width][3];
    
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            int bluex = 0;
            int greenx = 0;
            int redx = 0;
            int bluey = 0;
            int greeny = 0;
            int redy = 0;

            for (int m = 0; m < 3; m++)
            {
                for (int n = 0; n < 3; n++)
                {
                    //get GX and GY
                    if (i + m - 1 >= 0 && i + m - 1 < height && j + n - 1 >= 0 && j + n - 1 < width)
                    {
                        bluex += image[i + m - 1][j + n - 1].rgbtBlue * kernel[m][n];
                        greenx += image[i + m - 1][j + n - 1].rgbtGreen * kernel[m][n];
                        redx += image[i + m - 1][j + n - 1].rgbtRed * kernel[m][n];
                    
                        bluey += image[i + m - 1][j + n - 1].rgbtBlue * kernel[n][m];
                        greeny += image[i + m - 1][j + n - 1].rgbtGreen * kernel[n][m];
                        redy += image[i + m - 1][j + n - 1].rgbtRed * kernel[n][m];
                    }
                }
            }


            //sobel operator
            int b = round(sqrt(bluex * bluex + bluey * bluey));
            int g = round(sqrt(greenx * greenx + greeny * greeny));
            int r = round(sqrt(redx * redx + redy * redy));

            //cap 255
            if (b > 255)
            {
                b = 255;
            }
            if (g > 255)
            {
                g = 255;
            }
            if (r > 255)
            {
                r = 255;
            }

            color[i][j][0] = b;
            color[i][j][1] = g;
            color[i][j][2] = r;

        }
    }
    
    //apply to the image
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {

            image[i][j].rgbtBlue = color[i][j][0];
            image[i][j].rgbtGreen = color[i][j][1];
            image[i][j].rgbtRed = color[i][j][2];
        }
    }

    return;
}
