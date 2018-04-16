#include <stdio.h>
#include <string.h>
#include <string.h>
#include <stdlib.h>
#include "jpeglib.h"

int get_Y_U_V(unsigned char*rData,unsigned char* in_Y,unsigned char* in_U,unsigned char* in_V,int nStride,int height)
{
	int i = 0;
	int y_n =0;
	int u_n =0;
	int v_n =0;
	int u = 0;
	int v = 2;
	long size = nStride*height*3/2;
	//使用yuv420格式读取yu不连续放置
	#if 1
	while(i < 1280*720)
	{
		in_Y[y_n] = rData[i];
		i++;
		y_n++;
	}
	while(i < 1280*720*5/4)
	{
		in_U[u_n] = rData[i];
		i++;
		u_n++;
	}
	while(i < 1280*720*3/2)
	{
		in_V[v_n] = rData[i];
		i++;
		v_n++;
	
	}
	#endif

	return 0;
}


    int yuv420p_to_yuv420sp(unsigned char * yuv420p,unsigned char* yuv420sp,int width,int height)  
    {  
        if(yuv420p==NULL)  
            return 0;  
        int i=0,j=0;  
        //Y  
        for(i=0;i<width*height;i++)  
        {  
            yuv420sp[i]=yuv420p[i];  
        }  
      
        int m=0,n=0;  
        for(j=0;j<width*height/2;j++)  
        {  
            if(j%2==0)  
	    {
               yuv420sp[j+width*height]=yuv420p[width*height+m];  
		m++;
	    }
            else 
		{
                   	 yuv420sp[j+width*height]=yuv420p[width*height*5/4+n];  
			n++;
		} 
        }  
    }  

    int yuv420p_to_jpeg(const char * filename, const char* pdata,int image_width,int image_height, int quality)  
    {     
        struct jpeg_compress_struct cinfo;    
        struct jpeg_error_mgr jerr;    
        cinfo.err = jpeg_std_error(&jerr);    
        jpeg_create_compress(&cinfo);    
      
        FILE * outfile;    // target file    
        if ((outfile = fopen(filename, "wb")) == NULL) {    
            fprintf(stderr, "can't open %s\n", filename);    
            exit(1);    
        }    
        jpeg_stdio_dest(&cinfo, outfile);    
      
        cinfo.image_width = image_width;  // image width and height, in pixels    
        cinfo.image_height = image_height;    
        cinfo.input_components = 3;    // # of color components per pixel    
        cinfo.in_color_space = JCS_YCbCr;  //colorspace of input image    
        jpeg_set_defaults(&cinfo);    
        jpeg_set_quality(&cinfo, quality, TRUE );    
      
        //////////////////////////////    
        //  cinfo.raw_data_in = TRUE;    
        cinfo.jpeg_color_space = JCS_YCbCr;    
        cinfo.comp_info[0].h_samp_factor = 2;    
        cinfo.comp_info[0].v_samp_factor = 2;    
        /////////////////////////    
      
        jpeg_start_compress(&cinfo, TRUE);    
      
        JSAMPROW row_pointer[1];  
      
        unsigned char *yuvbuf;  
        if((yuvbuf=(unsigned char *)malloc(image_width*3))!=NULL)  
            memset(yuvbuf,0,image_width*3);  
      
        unsigned char *ybase,*ubase;  
        ybase=pdata;  
        ubase=pdata+image_width*image_height;    
        int j=0;  
        while (cinfo.next_scanline < cinfo.image_height)   
        {  
            int idx=0;  
	    int i;
            for(i=0;i<image_width;i++)  
            {   
                yuvbuf[idx++]=ybase[i + j * image_width];  
                yuvbuf[idx++]=ubase[j/2 * image_width+(i/2)*2];  
                yuvbuf[idx++]=ubase[j/2 * image_width+(i/2)*2+1];      
            }  
            row_pointer[0] = yuvbuf;  
            jpeg_write_scanlines(&cinfo, row_pointer, 1);  
            j++;  
        }  
        jpeg_finish_compress(&cinfo);    
        jpeg_destroy_compress(&cinfo);    
        fclose(outfile);    
	free(yuvbuf);
        return 0;    
    }  

int main()
{
	unsigned char* pData = (unsigned char*)malloc(1280 * 720*3/2);//存放yuv420sp数据
	unsigned char* rData = (unsigned char*)malloc(1280*720*3/2);//存放yuv420p数据
	
	unsigned long dwSize = 0;
	FILE *rfp = fopen("./00001.yuv","rb");
	if(NULL == rfp)
		printf("fopen file error!\n");

	fread(rData,1280*720*3/2,1,rfp);	//读取yuv文件数据到rData里面
	yuv420p_to_yuv420sp(rData,pData,1280,720);

	yuv420p_to_jpeg("00001.jpg",pData,1280,720,100);

	free(pData);
	free(rData);

	return 0;
}























