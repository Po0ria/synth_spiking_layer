#include<stdio.h>    
#include<stdlib.h>
#include <float.h>


void dense(float weights[11][14400],float bias[11],float inputs[30][30][16],float outputs[11],int n_strides,int input_flat_sz){
  int output_sz = 11;
  int l = input_flat_sz;
  int i, j, f, s, t;

  for(i=0;i<11;i++){
    outputs[i] += bias[i];
    for(j=0;j<input_flat_sz;j++){
      f = j%30;
      s = (j/30)%30;
      t = j/(30*30);
      outputs[i] += weights[i][j]*inputs[f][s][t];
    }
  }

}

void denseSpike(float membrane_potential[],float inputs[],float spikes[]){
  float thresh = 1.0;
  float spike = 1.;
  float reset = 0.;
  int i;

  for(i=0;i<11;i++){
      membrane_potential[i] = membrane_potential[i] + inputs[i];
      if(membrane_potential[i] >= thresh){
        spikes[i] = spike;
        membrane_potential[i] = reset;
      }
      else {
        spikes[i] = 0.;
      }
  }
}

void conv(float kernel[3][3][16][2], float bias[30][30][16], float input[32][32][2], float output[30][30][16], int width, int height, int filters, int channels, int n_strides){

  int i, j, k, l, w, h, jh, iw;

  for(i=0;i<n_strides;i++){
    for(j=0;j<n_strides;j++){
      for(k=0;k<filters;k++){
        output[i][j][k] += bias[i][j][k];
        for(l=0;l<channels;l++){
          for(w=0;w<width;w++){
            iw = i+w;
            for(h=0;h<height;h++){
              jh = j+h;
              output[i][j][k] += kernel[w][h][k][l]*input[iw][jh][l];
            }
          }
        }
      }
    }
  }
}

void convSpike(float membrane_potential[30][30][16],float inputs[30][30][16],float spikes[30][30][16],int n_strides,int filters) {
  float thresh = 1.0;
  float spike = 1.;
  float reset = 0.;

  int i, j, k;

  for(i=0;i<n_strides;i++){
    for(j=0;j<n_strides;j++){
      for(k=0;k<filters;k++){
        membrane_potential[i][j][k] = membrane_potential[i][j][k] + inputs[i][j][k];
        if(membrane_potential[i][j][k] >= thresh){
          spikes[i][j][k] = spike;
          membrane_potential[i][j][k] = reset;
        }
        else {
          spikes[i][j][k] = 0.;
        }
      }
    }
  }
}


int main(){

int kernel_width = 3;
int kernel_height = 3;
int kernel_filters = 16;
int kernel_channels = 2;
int n_strides = 30;

int input_width = 30;
int input_height = 30;
int bias_filters = 16;




float kernel[kernel_width][kernel_height][kernel_filters][kernel_channels];
float bias[input_width][input_height][bias_filters];
float input_image[input_width][input_height][bias_filters];
float output_image[input_width][input_height][kernel_filters];
float spikes[input_width][input_height][kernel_filters];
float membrane_potential[input_width][input_height][kernel_filters];


for(int s=0; s<kernel_width; s++)
  for (int r=0; r<kernel_height; r++)
    for (int m=0; m<kernel_filters; m++)
      for (int c=0; c<kernel_channels; c++)
          kernel[s][r][m][c] = (rand() % (100) - 50)/100.0;
          
for(int p=0; p<input_width; p++)
  for (int q=0; q<input_height; q++)
    for (int c=0; c<bias_filters; c++)
      input_image[p][q][c] = rand() % (1 + 1 - 0) + 0; 

for(int p=0; p<input_width; p++)
  for (int q=0; q<input_height; q++)
    for (int c=0; c<bias_filters; c++)
      bias[p][q][c] = rand() % (1 + 1 - 0) + 0;
conv(kernel,
       bias,
       input_image,
       output_image,
       kernel_width,
       kernel_height,
       kernel_filters,
       kernel_channels,
       n_strides);    

convSpike(membrane_potential, output_image, spikes,n_strides,kernel_filters);

return 0;
}
