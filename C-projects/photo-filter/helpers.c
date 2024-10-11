#include "helpers.h"
#include <stdlib.h>
#include <math.h>

// Convert image to grayscale
void grayscale(int height, int width, RGBTRIPLE image[height][width])
{
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            float grey = (image[i][j].rgbtBlue + image[i][j].rgbtRed + image[i][j].rgbtGreen) / 3.0;
            int down = grey;
            int up  = grey + .5;
        if (down != up)
        {
            down++;
        }
            image[i][j].rgbtBlue = down;
            image[i][j].rgbtRed = down;
            image[i][j].rgbtGreen = down;
        }
    }
    return;
}

// Reflect image horizontally
void reflect(int height, int width, RGBTRIPLE image[height][width])
{
    // read the pixel, swap it with the other side, then read the next pixel, and swap with the next one in order
    for (int i = 0; i < height; i++)
    {
        if (width == 2)
        {
            for (int j = 0; j < width - 1; j++) // swaps for only a width of 2
            {
                RGBTRIPLE swap;
                swap = image[i][j];
                image[i][j] = image[i][(width - 1) - j];
                image[i][(width - 1) - j] = swap;
            }
        }
        for (int j = 0; j < width - 2; j++) // swaps for any width greater than 2
        {
            RGBTRIPLE swap;
            swap = image[i][j];
            image[i][j] = image[i][(width - 1) - j];
            image[i][(width - 1) - j] = swap;
        }
    }
    return;
}

// Blur image
void blur(int height, int width, RGBTRIPLE image[height][width])
{
    RGBTRIPLE(*new)[width] = calloc(height, width * sizeof(RGBTRIPLE));
    // runs through collums
    for (int i = 0; i < height; i++)
    {
        //runs through rows
        for (int j = 0; j < width; j++)
        {
            //creates array for the rgb colors to be calculated for average
            float color[] = {0, 0, 0};
            // how many they will be divided by
            float divider = 0.0;
            // runs through upper, middle, and bottom row for averaging
            for (int h = -1; h < 2; h++)
            {
                // runs through collums for averaging
                for (int w = -1; w < 2; w++)
                {
                    // if it is less than the border then it doesnt check for pixels
                    if (((i + h) >= 0 && (i + h) < height) && ((j + w) >= 0 && (j + w) < width))
                    {
                        color[0] += image[i + h][j + w].rgbtRed;
                        color[1] += image[i + h][j + w].rgbtBlue;
                        color[2] += image[i + h][j + w].rgbtGreen;
                        divider ++;
                    }
                }
            }
            // rounds up or rounds down depending on decimal value
            int average[] = {0, 0, 0};
            for (int c = 0; c < 3; c++)
            {
                color[c] /= divider;
                average[c] = color[c];
                int check = color[c] + .5;
                if (average[c] != check)
                {
                    average[c]++;
                }
            }
            // changes the pixels to the average
            new[i][j].rgbtRed = average[0];
            new[i][j].rgbtBlue = average[1];
            new[i][j].rgbtGreen = average[2];
        }
    }
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            image[i][j].rgbtRed = new[i][j].rgbtRed;
            image[i][j].rgbtBlue = new[i][j].rgbtBlue;
            image[i][j].rgbtGreen = new[i][j].rgbtGreen;
        }
    }
    free(new);
    return;
}

// Detect edges
void edges(int height, int width, RGBTRIPLE image[height][width])
{
    //what the new image will be
    RGBTRIPLE(*new)[width] = calloc(height, width * sizeof(RGBTRIPLE));;

    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            float gx[] = {0, 0, 0};
            float gy[] = {0, 0, 0};
            // loops to scan over the 3x3 box in x direction
            for (int h = -1; h < 2; h++)
            {
                for (int w = -1; w < 2; w++)
                {
                    if (((i + h) < 0 || (i + h) >= height) || ((j + w) < 0 || (j + w) >= width))
                    {
                        gx[0] += 0;
                        gx[1] += 0;
                        gx[2] += 0;
                    }
                    else
                    {
                        if (h == 0)
                        {
                            if (w == -1)
                            {
                                gx[0] += image[i + h][j + w].rgbtRed * -2;
                                gx[1] += image[i + h][j + w].rgbtBlue * -2;
                                gx[2] += image[i + h][j + w].rgbtGreen * -2;
                            }
                            if (w == 1)
                            {
                                gx[0] += image[i + h][j + w].rgbtRed * 2;
                                gx[1] += image[i + h][j + w].rgbtBlue * 2;
                                gx[2] += image[i + h][j + w].rgbtGreen * 2;
                            }
                        }
                        // other two rows
                        else
                        {
                            gx[0] += image[i + h][j + w].rgbtRed * w;
                            gx[1] += image[i + h][j + w].rgbtBlue * w;
                            gx[2] += image[i + h][j + w].rgbtGreen * w;
                        }
                    }

                }
            }
            // for the y direction
            for (int w = -1; w < 2; w++)
            {
                for (int h = -1; h < 2; h++)
                {
                    if (((i + h) < 0 || (i + h) >= height) || ((j + w) < 0 || (j + w) >= width))
                    {
                        gy[0] += 0;
                        gy[1] += 0;
                        gy[2] += 0;
                    }
                    else
                    {
                        if (w == 0)
                        {
                            if (h == -1)
                            {
                                gy[0] += image[i + h][j + w].rgbtRed * -2;
                                gy[1] += image[i + h][j + w].rgbtBlue * -2;
                                gy[2] += image[i + h][j + w].rgbtGreen * -2;
                            }
                            if (h == 1)
                            {
                                gy[0] += image[i + h][j + w].rgbtRed * 2;
                                gy[1] += image[i + h][j + w].rgbtBlue * 2;
                                gy[2] += image[i + h][j + w].rgbtGreen * 2;
                            }
                        }
                        else
                        {
                            gy[0] += image[i + h][j + w].rgbtRed * h;
                            gy[1] += image[i + h][j + w].rgbtBlue * h;
                            gy[2] += image[i + h][j + w].rgbtGreen * h;
                        }
                    }
                }
            }
            // square root? for r b g
            float colors[] = {0, 0, 0};
            int edges[] = {0, 0, 0};
            for (int c = 0; c < 3; c++)
            {
                colors[c] = sqrt((gx[c] * gx[c]) + (gy[c] * gy[c]));
                // cap at 255
                if (colors[c] > 255)
                {
                    colors[c] = 255;
                }
                edges[c] = colors[c];
                int check = colors[c] + .5;
                if ( edges[c] != check)
                {
                    edges[c]++;
                }
            }
            // store new color to new picture
            new[i][j].rgbtRed = edges[0];
            new[i][j].rgbtBlue = edges[1];
            new[i][j].rgbtGreen = edges[2];
        }
    }
    // updates original picture with edges
    for ( int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            image[i][j].rgbtRed = new[i][j].rgbtRed;
            image[i][j].rgbtBlue = new[i][j].rgbtBlue;
            image[i][j].rgbtGreen = new[i][j].rgbtGreen;
        }
    }
    free(new);
    return;
}
