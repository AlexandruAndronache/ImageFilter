#include "helpers.h"
#include <math.h>
#include <stdio.h>

#pragma GCC diagnostic ignored "-Wunused-but-set-variable"

// Convert image to grayscale
void grayscale(int height, int width, RGBTRIPLE image[height][width])
{
    float average = 0;

    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            average =
                round((image[i][j].rgbtBlue + image[i][j].rgbtGreen + image[i][j].rgbtRed) / 3.0);
            image[i][j].rgbtBlue = (BYTE) average;
            image[i][j].rgbtGreen = (BYTE) average;
            image[i][j].rgbtRed = (BYTE) average;
        }
    }

    return;
}

// Convert image to sepia
void sepia(int height, int width, RGBTRIPLE image[height][width])
{
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {

            int sepiaRed = round(.393 * image[i][j].rgbtRed + .769 * image[i][j].rgbtGreen +
                                 .189 * image[i][j].rgbtBlue);
            int sepiaGreen = round(.349 * image[i][j].rgbtRed + .686 * image[i][j].rgbtGreen +
                                   .168 * image[i][j].rgbtBlue);
            int sepiaBlue = round(.272 * image[i][j].rgbtRed + .534 * image[i][j].rgbtGreen +
                                  .131 * image[i][j].rgbtBlue);

            if (sepiaRed > 255)
                sepiaRed = 255;
            if (sepiaGreen > 255)
                sepiaGreen = 255;
            if (sepiaBlue > 255)
                sepiaBlue = 255;

            image[i][j].rgbtRed = sepiaRed;
            image[i][j].rgbtGreen = sepiaGreen;
            image[i][j].rgbtBlue = sepiaBlue;
        }
    }
    return;
}

// Reflect image horizontally
void reflect(int height, int width, RGBTRIPLE image[height][width])
{
    RGBTRIPLE temp;

    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width / 2; j++)
        {
            temp = image[i][j];
            image[i][j] = image[i][width - j - 1];
            image[i][width - j - 1] = temp;
        }
    }

    return;
}

// Blur image
void blur(int height, int width, RGBTRIPLE image[height][width])
{

    double avg_red;
    double avg_green;
    double avg_blue;
    // a pixel can be: corner, edge or normal. Based on this I want to make a switch statement
    // that changes the pixel type based on tests.
    /* enum pixel {CORNER_TOP_LEFT, CORNER_TOP_RIGHT, CORNER_BOTTOM_RIGHT , CORNER_BOTTOM_LEFT
               EDGE_UP, EDGE_RIGHT, EDGE_BOTTOM, EDGE_LEFT};

        int pixel_type;

        if (x = 0 && y == 0 ) pixel_type = CORNER_TOP_LEFT;
        else
        if (x == 0 && y == width - 1) pixel_type = CORNER_TOP_RIGHT;
        else
        if (x == height - 1 && y = 0) pixel_type = CORNER_BOTTOM_LEFT;
        else
        if (x == height - 1 && y == width - 1) pixel_type = CORNER_BOTTOM_RIGHT;
        else
        if (x == 0) pixel_type = EDGE_UP
        else
        if (y == width - 1) pixel_type = EDGE_RIGHT;
        else
        if (x == height - 1) pixel_type = EDGE_BOTTOM;
        else
        if (y == 0) pixel_type = EDGE_LEFT;

        switch(pixel_type)
        {
            case CORNER_TOP_LEFT:
            {
                avg = copy[x][y] + copy[x][y + 1] +
                      copy[x + 1][y] + copy[x + 1][y + 1];
                break;
            }

            case CORNER_TOP_RIGHT:
            {
                avg = copy[x][y] + copy[x][y-1] +
                    copy[x + 1][y - 1] + copy[x + 1][y];

                break;
            }

            case CORNER_BOTTOM_RIGHT:
            {
                avg = copy[x][y] + copy[x][y-1] +
                      copy[x + 1][y - 1] + copy[x + 1][y];
                break;
            }

            case CORNER_BOTTOM_LEFT:
            {
                avg = copy[x][y] + copy[x][y - 1] +
                      copy[x - 1][y - 1] + copy[x - 1][y];
                break;
            }

            case EDGE_UP:
            {
                avg =    [x][y - 1] + [x][y] + [x][y + 1] +
                         [x + 1][y - 1] + [x + 1][y] + [x + 1][y + 1];
                break;
            }

            case EDGE_RIGHT:
            {
                avg = [x - 1][y - 1] + [x - 1][y] +
                      [x][y - 1] + [x][y] +
                      [x + 1][y - 1] + [x + 1][y];
                break;
            }

            case EDGE_BOTTOM:
            {
                avg = [x - 1][y - 1] + [x - 1][y] + [x - 1][y + 1] +
                      [x][y - 1] + [x][y] + x[x][y + 1];
                break;
            }

            case EDGE_LEFT:
            {
                avg = [x - 1][y] + [x - 1][y + 1] +
                      [x][y] + [x][y + 1] +
                      [x + 1][y] + [x + 1][y + 1];
                break;
            }

            default:
            {

                "You should not be here!\n";
                break;
            }



        }

    */

    // apply filter to edges
    /* iterate through copy image
        is it an edge? ( x = 0 || x = height - 1 && y = 0 || y = width - 1)
        if yes:
            is it a corner? (x == 0 && y == 0) || (x == 0 && y == width - 1)
                         || (x == height - 1 && y == 0) || (x == height - 1 && y == width - 1)
            if yes:
            calculate average within 1 row & 1 column (4 numbers):
                // top left
                if (x == 0 && y == 0) : avg = copy[x][y] + copy[x][y + 1] +
                                              copy[x + 1][y] + copy[x + 1][y + 1];
                // top right
                else
                if (x == 0 && y == width - 1): avg = copy[x][y] + copy[x][y-1] +
                                                     copy[x + 1][y - 1] + copy[x + 1][y]
                // bottom left
                else
                if (x == height - 1 && y == 0): avg = copy[x][y] + copy[x - 1][y] +
                                                      copy[x - 1][y + 1] + copy[x][y + 1]
                // bottom right
                else: (x == height - 1 && y == width - 1) avg = copy[x][y] + copy[x][y - 1] +
                                                                copy[x - 1][y - 1] + copy[x - 1][y]
            if no:
                // Top edge
                if x == 0: avg =    [x][y - 1] + [x][y] + [x][y + 1] +
                                    [x + 1][y - 1] + [x + 1][y] + [x + 1][y + 1]
                // Left Edge
                else if y = 0: avg = [x - 1][y] + [x - 1][y + 1] +
                                     [x][y] + [x][y + 1] +
                                     [x + 1][y] + [x + 1][y + 1]
                // Bottom edge
                else if x = height - 1: avg = [x - 1][y - 1] + [x - 1][y] + [x - 1][y + 1] +
                                              [x][y - 1] + [x][y] + x[x][y + 1]
                // Right edge
                else if y == width - 1: avg = [x - 1][y - 1] + [x - 1][y] +
                                              [x][y - 1] + [x][y] +
                                              [x + 1][y - 1] + [x + 1][y]
        if no:
            avg = [x - 1][y - 1] + [x - 1][y] + [x + 1][y + 1] +
                  [x][y - 1] + [x][y] + [x][y + 1]
                  [x + 1][y - 1] + [x + 1][y] + [x + 1][y + 1]

            apply 3x3 filter
    */
    // apply filter to corners
    // calculate average within 1 row & 1 column (4 numbers)
    // apply filter to rest of the image

    // Create a copy of image
    RGBTRIPLE copy[height][width];

    // declare i and j to use in typedef
    int i, j;
    // typedef to make it easier to understand

    for (i = 0; i < height; i++)
    {
        for (j = 0; j < width; j++)
        {
            copy[i][j] = image[i][j];
        }
    }

    enum pixel {
        CORNER_TOP_LEFT,
        CORNER_TOP_RIGHT,
        CORNER_BOTTOM_LEFT,
        CORNER_BOTTOM_RIGHT,
        EDGE_UP,
        EDGE_RIGHT,
        EDGE_BOTTOM,
        EDGE_LEFT,
        NORMAL
    };

    int pixel_type = NORMAL;

    for (int x = 0; x < height; x++)
    {
        for (int y = 0; y < width; y++)
        {

            // top left corner has coordinates x = 0 y = 0
            if ((x == 0) && y == 0)
                pixel_type = CORNER_TOP_LEFT;
            else
                // top right corner has x = 0 y = n
                if ((x == 0) && y == width - 1)
                    pixel_type = CORNER_TOP_RIGHT;
                else
                    // bottom left has x = n y = 0
                    if ((x == height - 1) && y == 0)
                        pixel_type = CORNER_BOTTOM_LEFT;
                    else
                        // bottom right has x = n y = n
                        if ((x == height - 1) && y == width - 1)
                            pixel_type = CORNER_BOTTOM_RIGHT;
                        else
                            //
                            if (x == 0)
                                pixel_type = EDGE_UP;
                            else if (y == (width - 1))
                                pixel_type = EDGE_RIGHT;
                            else if (x == (height - 1))
                                pixel_type = EDGE_BOTTOM;
                            else if (y == 0)
                                pixel_type = EDGE_LEFT;
                            else
                                pixel_type = NORMAL;

            switch (pixel_type)
            {

                case CORNER_TOP_LEFT:
                {
                    avg_red = round((copy[x][y].rgbtRed + copy[x][y + 1].rgbtRed +
                                     copy[x + 1][y].rgbtRed + copy[x + 1][y + 1].rgbtRed) /
                                    4.0);
                    avg_green = round((copy[x][y].rgbtGreen + copy[x][y + 1].rgbtGreen +
                                       copy[x + 1][y].rgbtGreen + copy[x + 1][y + 1].rgbtGreen) /
                                      4.0);
                    avg_blue = round((copy[x][y].rgbtBlue + copy[x][y + 1].rgbtBlue +
                                      copy[x + 1][y].rgbtBlue + copy[x + 1][y + 1].rgbtBlue) /
                                     4.0);
                    break;
                }

                case CORNER_TOP_RIGHT:
                {
                    avg_red = round((copy[x][y - 1].rgbtRed + copy[x][y].rgbtRed +
                                     copy[x + 1][y - 1].rgbtRed + copy[x + 1][y].rgbtRed) /
                                    4.0);
                    avg_green = round((copy[x][y - 1].rgbtGreen + copy[x][y].rgbtGreen +
                                       copy[x + 1][y - 1].rgbtGreen + copy[x + 1][y].rgbtGreen) /
                                      4.0);
                    avg_blue = round((copy[x][y - 1].rgbtBlue + copy[x][y].rgbtBlue +
                                      copy[x + 1][y - 1].rgbtBlue + copy[x + 1][y].rgbtBlue) /
                                     4.0);
                    break;
                }

                case CORNER_BOTTOM_RIGHT:
                {
                    avg_red = round((copy[x - 1][y - 1].rgbtRed + copy[x - 1][y].rgbtRed +
                                     copy[x][y - 1].rgbtRed + copy[x][y].rgbtRed) /
                                    4.0);
                    avg_green = round((copy[x - 1][y - 1].rgbtGreen + copy[x - 1][y].rgbtGreen +
                                       copy[x][y - 1].rgbtGreen + copy[x][y].rgbtGreen) /
                                      4.0);
                    avg_blue = round((copy[x - 1][y - 1].rgbtBlue + copy[x - 1][y].rgbtBlue +
                                      copy[x][y - 1].rgbtBlue + copy[x][y].rgbtBlue) /
                                     4.0);
                    break;
                }

                case CORNER_BOTTOM_LEFT:
                {
                    avg_red = round((copy[x - 1][y].rgbtRed + copy[x - 1][y + 1].rgbtRed +
                                     copy[x][y].rgbtRed + copy[x][y + 1].rgbtRed) /
                                    4.0);
                    avg_green = round((copy[x - 1][y].rgbtGreen + copy[x - 1][y + 1].rgbtGreen +
                                       copy[x][y].rgbtGreen + copy[x][y + 1].rgbtGreen) /
                                      4.0);
                    avg_blue = round((copy[x - 1][y].rgbtBlue + copy[x - 1][y + 1].rgbtBlue +
                                      copy[x][y].rgbtBlue + copy[x][y + 1].rgbtBlue) /
                                     4.0);
                    break;
                }

                case EDGE_UP:
                {
                    avg_red = round((copy[x][y - 1].rgbtRed + copy[x][y].rgbtRed +
                                     copy[x][y + 1].rgbtRed + copy[x + 1][y - 1].rgbtRed +
                                     copy[x + 1][y].rgbtRed + copy[x + 1][y + 1].rgbtRed) /
                                    6.0);
                    avg_green = round((copy[x][y - 1].rgbtGreen + copy[x][y].rgbtGreen +
                                       copy[x][y + 1].rgbtGreen + copy[x + 1][y - 1].rgbtGreen +
                                       copy[x + 1][y].rgbtGreen + copy[x + 1][y + 1].rgbtGreen) /
                                      6.0);
                    avg_blue = round((copy[x][y - 1].rgbtBlue + copy[x][y].rgbtBlue +
                                      copy[x][y + 1].rgbtBlue + copy[x + 1][y - 1].rgbtBlue +
                                      copy[x + 1][y].rgbtBlue + copy[x + 1][y + 1].rgbtBlue) /
                                     6.0);
                    break;
                }

                case EDGE_RIGHT:
                {
                    avg_red = round(copy[x - 1][y - 1].rgbtRed + copy[x - 1][y].rgbtRed +
                                    copy[x][y - 1].rgbtRed + copy[x][y].rgbtRed +
                                    copy[x + 1][y - 1].rgbtRed + copy[x + 1][y].rgbtRed) /
                              6.0;
                    avg_green = round(copy[x - 1][y - 1].rgbtGreen + copy[x - 1][y].rgbtGreen +
                                      copy[x][y - 1].rgbtGreen + copy[x][y].rgbtGreen +
                                      copy[x + 1][y - 1].rgbtGreen + copy[x + 1][y].rgbtGreen) /
                                6.0;
                    avg_blue = round(copy[x - 1][y - 1].rgbtBlue + copy[x - 1][y].rgbtBlue +
                                     copy[x][y - 1].rgbtBlue + copy[x][y].rgbtBlue +
                                     copy[x + 1][y - 1].rgbtBlue + copy[x + 1][y].rgbtBlue) /
                               6.0;

                    break;
                }

                case EDGE_BOTTOM:
                {
                    avg_red = round(copy[x - 1][y - 1].rgbtRed + copy[x - 1][y].rgbtRed +
                                    copy[x - 1][y + 1].rgbtRed + copy[x][y - 1].rgbtRed +
                                    copy[x][y].rgbtRed + copy[x][y + 1].rgbtRed) /
                              6.0;
                    avg_green = round(copy[x - 1][y - 1].rgbtGreen + copy[x - 1][y].rgbtGreen +
                                      copy[x - 1][y + 1].rgbtGreen + copy[x][y - 1].rgbtGreen +
                                      copy[x][y].rgbtGreen + copy[x][y + 1].rgbtGreen) /
                                6.0;
                    avg_blue = round(copy[x - 1][y - 1].rgbtBlue + copy[x - 1][y].rgbtBlue +
                                     copy[x - 1][y + 1].rgbtBlue + copy[x][y - 1].rgbtBlue +
                                     copy[x][y].rgbtBlue + copy[x][y + 1].rgbtBlue) /
                               6.0;
                    break;
                }

                case EDGE_LEFT:
                {
                    avg_red = round((copy[x - 1][y].rgbtRed + copy[x - 1][y + 1].rgbtRed +
                                     copy[x][y].rgbtRed + copy[x][y + 1].rgbtRed +
                                     copy[x + 1][y].rgbtRed + copy[x + 1][y + 1].rgbtRed) /
                                    6.0);
                    avg_green = round((copy[x - 1][y].rgbtGreen + copy[x - 1][y + 1].rgbtGreen +
                                       copy[x][y].rgbtGreen + copy[x][y + 1].rgbtGreen +
                                       copy[x + 1][y].rgbtGreen + copy[x + 1][y + 1].rgbtGreen) /
                                      6.0);
                    avg_blue = round((copy[x - 1][y].rgbtBlue + copy[x - 1][y + 1].rgbtBlue +
                                      copy[x][y].rgbtBlue + copy[x][y + 1].rgbtBlue +
                                      copy[x + 1][y].rgbtBlue + copy[x + 1][y + 1].rgbtBlue) /
                                     6.0);
                    break;
                }

                case NORMAL:
                {
                    avg_red = round((copy[x - 1][y - 1].rgbtRed + copy[x - 1][y].rgbtRed +
                                     copy[x - 1][y + 1].rgbtRed + copy[x][y - 1].rgbtRed +
                                     copy[x][y].rgbtRed + copy[x][y + 1].rgbtRed +
                                     copy[x + 1][y - 1].rgbtRed + copy[x + 1][y].rgbtRed +
                                     copy[x + 1][y + 1].rgbtRed) /
                                    9.0);
                    avg_green = round(copy[x - 1][y - 1].rgbtGreen + copy[x - 1][y].rgbtGreen +
                                      copy[x - 1][y + 1].rgbtGreen + copy[x][y - 1].rgbtGreen +
                                      copy[x][y].rgbtGreen + copy[x][y + 1].rgbtGreen +
                                      copy[x + 1][y - 1].rgbtGreen + copy[x + 1][y].rgbtGreen +
                                      copy[x + 1][y + 1].rgbtGreen) /
                                9.0;
                    avg_blue = round(copy[x - 1][y - 1].rgbtBlue + copy[x - 1][y].rgbtBlue +
                                     copy[x - 1][y + 1].rgbtBlue + copy[x][y - 1].rgbtBlue +
                                     copy[x][y].rgbtBlue + copy[x][y + 1].rgbtBlue +
                                     copy[x + 1][y - 1].rgbtBlue + copy[x + 1][y].rgbtBlue +
                                     copy[x + 1][y + 1].rgbtBlue) /
                               9.0;
                    break;
                }

                default:
                {
                    // avg_red = (copy[x - 1][y - 1].rgbtRed + copy[x - 1][y].rgbtRed + copy[x -
                    // 1][y + 1].rgbtRed +
                    //            copy[x][y - 1].rgbtRed + copy[x][y].rgbtRed + copy[x][y +
                    //            1].rgbtRed + copy[x + 1][y - 1].rgbtRed + copy[x + 1][y].rgbtRed +
                    //            copy[x + 1][y + 1].rgbtRed) / 9.0;
                    // avg_green = (copy[x - 1][y - 1].rgbtGreen + copy[x - 1][y].rgbtGreen + copy[x
                    // - 1][y + 1].rgbtGreen +
                    //       copy[x][y - 1].rgbtGreen + copy[x][y].rgbtGreen + copy[x][y +
                    //       1].rgbtGreen + copy[x + 1][y - 1].rgbtGreen + copy[x + 1][y].rgbtGreen
                    //       + copy[x + 1][y + 1].rgbtGreen) / 9.0;
                    // avg_blue = (copy[x - 1][y - 1].rgbtBlue + copy[x - 1][y].rgbtBlue + copy[x -
                    // 1][y + 1].rgbtBlue +
                    //       copy[x][y - 1].rgbtBlue + copy[x][y].rgbtBlue + copy[x][y + 1].rgbtBlue
                    //       + copy[x + 1][y - 1].rgbtBlue + copy[x + 1][y].rgbtBlue + copy[x + 1][y
                    //       + 1].rgbtBlue) / 9.0;

                    printf("You should not be here!\n");
                    printf("pixel_type: %d\n", pixel_type);
                    break;
                }
            }

            // check for overflow
            if (avg_red > 255)
                avg_red = 255;
            if (avg_green > 255)
                avg_green = 255;
            if (avg_blue > 255)
                avg_blue = 255;

            // set values
            image[x][y].rgbtRed = (int) round(avg_red);
            image[x][y].rgbtGreen = (int) round(avg_green);
            image[x][y].rgbtBlue = (int) round(avg_blue);
        }
    }

    return;
}
