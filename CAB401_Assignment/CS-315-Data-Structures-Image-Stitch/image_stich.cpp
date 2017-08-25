/*********
Author: Mackenzie Larson
Date: 9/14/14
Class: CS 315 LAB 04
Description: This program will reconstruct an image from a collection of 16 subimages 
To compile:  g++ image_stich.cpp EasyBMP.cpp -o l04.out
             ./l04.out
**********/ 

#include "EasyBMP.h"
#include <math.h>
#include <iostream>
#include <climits>
#include <cstdlib>

using namespace std;


// Filenames for input and output
const string INFILENAMES [16] = {"img2.bmp", "img16.bmp", "img3.bmp",
                                 "img4.bmp", "img5.bmp", "img6.bmp", "img7.bmp", "img8.bmp",
                                 "img9.bmp", "img10.bmp", "img11.bmp", "img12.bmp",
                                 "img13.bmp", "img14.bmp", "img15.bmp", "img1.bmp"};

int rowMatch (BMP& UseTop, BMP& UseBottom )
{ // Compares the top edge of UseTop to the bottom edge of UseBottom.
  // Assumes UseTop and UseBottom are squares of same size
  // score obtained by adding the difference between color components
  
  //Initializes RGBApixel from easyBMP. This is a class that represents a single colored pixel. Using this we will be 
  //able to get the RGB values more easily from the top and the bottom pixels. 
  RGBApixel TopPixel; 
  RGBApixel BottomPixel;
  int sum = 0; 
  int score = 0;
  //tell width returns the number of pixels in the horizontal direction 
  for (int i = 0; i < UseTop.TellWidth() - 1; i++)
    {
      //will find the top pixel of UseTop using GetPixel from easyBMP
      TopPixel = UseTop.GetPixel(i, 0);
      //the bottom pixel is found 
      BottomPixel = UseBottom.GetPixel(i, UseTop.TellHeight() - 1);
      //we use abs so the score won't be negative 
      //compare the top and bottom pixel by each RGB value and add together. This becomes the score. 
      sum = abs(TopPixel.Red - BottomPixel.Red) + abs(TopPixel.Green - BottomPixel.Green) +
        abs(TopPixel.Blue - BottomPixel.Blue);
      score += sum;
    }
  return score;

}

int columnMatch ( BMP& UseRight, BMP& UseLeft )
{
  // Compares the right edge of UseRight to the left edge of UseLeft.
  // Assumes UseRight and UseLeft are squares of same size
  // score obtained by adding the difference between color components
  // similar to the rowMatch

  //same logic as rowmatch 
  RGBApixel RightPixel; 
  RGBApixel LeftPixel;
  int sum = 0; 
  int score = 0;
  for (int i = 0; i < UseRight.TellHeight() - 1; i++)
    {
      RightPixel = UseRight.GetPixel(UseLeft.TellWidth() - 1, i);
      LeftPixel = UseLeft.GetPixel(0, i);
      sum = abs(RightPixel.Red - LeftPixel.Red) + abs(RightPixel.Green - LeftPixel.Green) +
        abs(RightPixel.Blue - LeftPixel.Blue);
      score += sum;
    }
  return score;

}

void finalScore (BMP (& images)[16], int (& score)[2][16][16]) 
{
  // uses the rowMatch and columnMatch to create scores
  // score[0][i][j] is the EAST SCORE and score[1][i][j] is the NORTH SCORE
  //rows
  for (int i = 0; i < 16; i++)
    {//cols
      for (int j = 0; j < 16; j++)
        {
          score[1][i][j] = rowMatch(images[i], images[j]);
          score[0][j][i] = columnMatch(images[i], images[j]);
        }
    }

}

int findNorthWest(int score[2][16][16])
{
  // finds the North West tile by adding the best North and West scores for each tile and
  // choosing the one that maximizes the score
  int tempscore = 0; 
  int max = 0; 
  int NW_tile = 0;
  int min_N = 0;
  int min_W = 0;
  //Will run through each row and col gathering the scores for the min north and min west. 
  //The scores for min north and min west for each image are then compared to the max score
  //If the score for an image is greater than the max,the tile, i , becomes the most northwest tile
  //The process of comparison is continued until all 16 images have been compared to the max
  //the final max is the northwest tile
  for (int i = 0; i < 16; i++)
    {
      min_N = score[1][i][0];
      min_W = score[0][i][0];

      for(int j = 1; j < 16; ++j)
        {
          if(min_N > score[1][i][j])
            {
              min_N = score[1][i][j];
            }

          if(min_W > score[0][i][j])
            {
              min_W = score[0][i][j];
            }
        }
      tempscore = min_N + min_W;

      if (tempscore > max)
        {
          max = tempscore;
          NW_tile = i;
        }


    }
  return NW_tile;
}

int findEastNeighbor(int score[2][16][16], int tile, bool remaining[16])
{
  // for a given tile, find its eastern neighbor among the remaining ones
  // remaining[j] is true for tiles that have not yet been placed in the final image

  int min = INT_MAX; 
  int temp = tile; 
  int E_tile = 17;
  // Will walk through the tiles one at a time. Store the score as temp of each tile. If the temp is less than 
  // the const min, and the remaining tiles, and tile not equal to i, then the tile becomes the east neighbor unless another score is smaller. If no tile fits the qualification for the for loop, then there isn't an east neighbor and s counted as 17 (ouside the image range of 16) 
  for (int i = 0; i < 16; i++)
    {
      temp = score[0][i][tile]; 
      if (temp < min && remaining[i] && tile != i)
        {
          min = temp;
          E_tile = i;
        }
    }
  return E_tile;
}

int findSouthNeighbor(int score[2][16][16], int tile, bool remaining[16])
{
  // for a given tile, find its southern neighbor, among the remaining ones
  // remaining[j] is true for tiles that have not yet been selected for placement
  // similar to findEastNeighbor

  //same logic as east neighbor function! see above
  int min = INT_MAX; 
  int temp = tile; 
  int N_tile = 17;
  for (int i = 0; i < 16; i++)
    {
      temp = score[1][i][tile]; 
      if (temp < min && remaining[i] && tile != i)
        {
          min = temp;
          N_tile = i;
        }

    }
  return N_tile;
}

void copy(BMP & InImg, BMP & OutImg, int c, int r)
{
  // copy image to larger final picture so that the InImg is placed in row i, column j of OutImg
  //TellWidth will return the number fpixles in the horizontal direction (cols)
  //tellheight will return the number of pixles in the vertical direction (rows) 
  for (int i = 0; i < InImg.TellWidth() && i < OutImg.TellWidth(); i++)
    {
      for (int j = 0; j < InImg.TellHeight() && i < OutImg.TellHeight(); j++)
        {
          RGBApixel pixel = InImg.GetPixel(i, j);
	  //outimg another easybmp
          OutImg.SetPixel(c * InImg.TellWidth() + i, r * InImg.TellHeight() + j, pixel);
        }
    }
}

void greedy(int score[2][16][16], BMP InImg[16], BMP & OutImg)
{
  //greedy algorithm to put the image together
  bool remaining[16] = { 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 };
  int ordered[16];
  int index = 0;
  //sectioning the image into quarters, the rest falls into place 
  for (int i = 0; i < 4; i++)
    {
      for (int j = 0; j < 4; j++)
        {
          index = 4 * i + j;
	  //to start, if i,j is 0,0 and there isn't an image ther (top left square) it will find the northwest image and copy it to the index
          if (i == 0 && j == 0)
            {
              //Will find the NorthWest 
              ordered[index] = findNorthWest(score);
              remaining[ordered[index]] = 0;
              copy(InImg[ordered[index]], OutImg, 0, 0);
            }
          else
            {
              if (j > 0)
                {
                  //If the column is bigger than 0, it is either the east neighbor of the north west point or the south neighbor
                  ordered[index] = findEastNeighbor(score, ordered[index - 1], remaining);
                  remaining[ordered[index]] = 0;
                  copy(InImg[ordered[index]], OutImg, j, i);
                }
              else
                {
                  // if the column is 0 then find south neighbor
                  ordered[index] = findSouthNeighbor(score, ordered[index - 4], remaining);
                  remaining[ordered[index]] = 0;
                  copy(InImg[ordered[index]], OutImg, j, i); 
                }
            }
        }

    }
}

int main()
{
  BMP InImg[16], OutImg; // vector of input images and output image
  int score [2][16][16] ;       // holds EAST and NORTH scores
  for( int i=0; i<16; ++i ) // Read in the sub-images
    InImg[i].ReadFromFile( INFILENAMES[i].c_str());
  int subsize = InImg[0].TellWidth();
  OutImg.SetSize( 4*subsize, 4*subsize ); // Set size of output image
  finalScore(InImg, score);
  greedy( score, InImg, OutImg);
  cout << "Writing reconstructed image to FinalImage.bmp!" << endl;
  OutImg.WriteToFile("FinalImage.bmp");

  return 0;
}
