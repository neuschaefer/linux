/*----------------------------------------------------------------------------*
 * No Warranty                                                                *
 * Except as may be otherwise agreed to in writing, no warranties of any      *
 * kind, whether express or implied, are given by MTK with respect to any MTK *
 * Deliverables or any use thereof, and MTK Deliverables are provided on an   *
 * "AS IS" basis.  MTK hereby expressly disclaims all such warranties,        *
 * including any implied warranties of merchantability, non-infringement and  *
 * fitness for a particular purpose and any warranties arising out of course  *
 * of performance, course of dealing or usage of trade.  Parties further      *
 * acknowledge that Company may, either presently and/or in the future,       *
 * instruct MTK to assist it in the development and the implementation, in    *
 * accordance with Company's designs, of certain softwares relating to        *
 * Company's product(s) (the "Services").  Except as may be otherwise agreed  *
 * to in writing, no warranties of any kind, whether express or implied, are  *
 * given by MTK with respect to the Services provided, and the Services are   *
 * provided on an "AS IS" basis.  Company further acknowledges that the       *
 * Services may contain errors, that testing is important and Company is      *
 * solely responsible for fully testing the Services and/or derivatives       *
 * thereof before they are used, sublicensed or distributed.  Should there be *
 * any third party action brought against MTK, arising out of or relating to  *
 * the Services, Company agree to fully indemnify and hold MTK harmless.      *
 * If the parties mutually agree to enter into or continue a business         *
 * relationship or other arrangement, the terms and conditions set forth      *
 * hereunder shall remain effective and, unless explicitly stated otherwise,  *
 * shall prevail in the event of a conflict in the terms in any agreements    *
 * entered into between the parties.                                          *
 *----------------------------------------------------------------------------*/
/*-----------------------------------------------------------------------------
 * Copyright (c) 2005, MediaTek, Inc.
 * All rights reserved.
 *
 * Unauthorized use, practice, perform, copy, distribution, reproduction,
 * or disclosure of this information in whole or in part is prohibited.
 *-----------------------------------------------------------------------------
 *
 * $Author: dtvbm11 $
 * $Date: 2012/04/27 $
 * $RCSfile: drv_ttd.c $
 * $Revision: #1 $
 *
 *----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*
 * Header Files
 *----------------------------------------------------------------------------*/
#include "general.h"
#include "x_assert.h"
#include "hw_vdoin.h"
#include "hw_tddc.h"
#include "hw_sw.h"
#include "nptv_debug.h"
#include "drv_ttd.h"
#include "drv_video.h"
#include "drv_tdtv_feature.h"
#include "drv_tdtv_drvif.h"
#include "drv_scaler_drvif.h"
#include "drv_meter.h"
#include "fbm_drvif.h"

typedef unsigned char PEL;
typedef struct 
{
    signed y:6;
    signed x:10;
} DVEC;

#define TDDC_OCC_ERROR      8
#define TDDC_DS             8
#define TDDC_MB_SIZE        4
#define TDDC_DIBR_GAIN      32

static UINT8 u1TddcInitFlag = SV_FALSE;
static INT32 shift_avg_H;
static INT32 shift_avg_V;
static INT32 depth_plus=0;
static INT32 depth_buf_width, depth_buf_height ;
static INT32 occ_count_L, occ_count_R;
static INT32 m_depth_width, m_depth_height;
static INT32 m_mbSize;

static PEL *m_pDownY32_L = NULL;   	//Dram2
static PEL *m_pDownY32_R = NULL;   	//Dram2
static INT32 *check_map = NULL;
static DVEC * DE_buf_L = NULL; 		//Dram3
static DVEC * DE_buf_R = NULL; 		//Dram3
static INT32 * depth_buf_L = NULL;          
static INT32 * depth_buf_R = NULL;            
static INT32 * filter_buf_L = NULL;         
static INT32 * filter_buf_R = NULL;     
static INT32 * final_depth_buf_L = NULL;     
static INT32 * final_depth_buf_R = NULL;        
static INT32 * render_depth_buf_L = NULL;   //Dram4
static INT32 * render_depth_buf_R = NULL;   //Dram4
static INT32 * occ_map_L = NULL;             
static INT32 * occ_map_R = NULL;    
static INT32 * non_occ_L = NULL;              
static INT32 * non_occ_R = NULL; 

INT32  expf_Lut[96]={
1024,1023,1020,1016,1010,1002,992,981,969,954,939,922,904,884,864,842, 
 820, 797, 773, 749, 724, 698,673,647,621,595,569,544,518,493,469,445,
 421, 398, 375, 354, 332, 312,292,273,255,238,221,206,191,177,163,150,
 139, 127, 117, 107,  98,  89, 81, 74, 67, 61, 55, 50, 45, 41, 36, 33, 
  29,  26,  23,  21,  18,  16, 15, 13, 11, 10,  9,  8,  7,  6,  5,  5, 
   4,   3,   3,   3,   2,   2,  2,  1,  1,  1,  1,  1,  1,  1,  0,  0}; 


#define ClipInt(p)  ((p > 255) ? (255) : ((p < 0) ? 0 : p))

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

/* =========================================================================================== */
static void OccProcess(DVEC *level_L, DVEC *level_R, INT32 *occ_map_L, INT32 *occ_map_R,
                        INT32 *non_occ_L, INT32 *non_occ_R, INT32 shift_H, INT32 shift_V)
{
    INT32 i, x, y;
	INT32 L_dp, R_dp, L_disp, R_disp;
    INT32 diff = TDDC_DS*m_mbSize/2;
    INT32 mod = TDDC_DS*m_mbSize;
    
    occ_count_L=0; 
    occ_count_R=0;

    i = 0;
    
	for (y=0; y<m_depth_height; y++)	// LRC L map
	{
    	for (x=0; x<m_depth_width; x++)
        {		
    		L_dp = (level_L[i].x>=0) ? (level_L[i].x+diff) : (level_L[i].x-diff);
    		L_disp = L_dp/mod;
    		
    		if((x+L_disp)>=0 && (x+L_disp)<=m_depth_width-1)
    		{   //has correspondence
    			if ((ABS(level_L[i].x+level_R[i+L_disp].x) <= TDDC_OCC_ERROR   
                    && level_R[i+L_disp].x!=-512 && level_L[i].x!=-512)||
    			  	(ABS(level_L[i].x+level_R[i+L_disp-1].x) <= TDDC_OCC_ERROR/2 
    			  	&& level_R[i+L_disp-1].x!=-512 && level_L[i].x!=-512)||
    			  	(ABS(level_L[i].x+level_R[i+L_disp+1].x) <= TDDC_OCC_ERROR/2 
    			  	&& level_R[i+L_disp+1].x!=-512 && level_L[i].x!=-512))
    			{   //LR perfact match
    				occ_map_L[i] = 255;		non_occ_L[i] =  level_L[i].x;
    			}
    			else
    			{   //unknown
    				occ_count_L++;
    				occ_map_L[i] =0;	non_occ_L[i] = -128;
    			}
    		}
    		else
    		{   //out of boundary
    			occ_map_L[i] =0;	non_occ_L[i] = -128;
    		}
            i++;
    	}
	}
    
    i = 0;
    
	for (y=0; y<m_depth_height; y++)	// LRC R map
	{
    	for (x=0; x<m_depth_width; x++)
        {		
    		R_dp = (level_R[i].x>=0) ? (level_R[i].x+diff) : (level_R[i].x-diff);
    		R_disp = R_dp/mod;

    		if ((x+R_disp)>=0 && (x+R_disp)<=m_depth_width-1)
    		{   //has correspondence
    			if ((ABS(level_R[i].x+level_L[i+R_disp-1].x) <= TDDC_OCC_ERROR   
                    && level_R[i].x!=-512&&level_L[i+R_disp-1].x!=-512)||
    				(ABS(level_R[i].x+level_L[i+R_disp].x) <= TDDC_OCC_ERROR/2 
    				&& level_R[i].x!=-512&&level_L[i+R_disp].x!=-512)||
    				(ABS(level_R[i].x+level_L[i+R_disp+1].x) <= TDDC_OCC_ERROR/2 
    				&& level_R[i].x!=-512&&level_L[i+R_disp+1].x!=-512))
    			{   //LR perfact match
    				occ_map_R[i] = 255; 	non_occ_R[i] =  level_R[i].x;
    			}
    			else
    			{   //unknown
    				occ_count_R++;
    				occ_map_R[i] =0;		non_occ_R[i] = -128;
    			}
    		}
    		else
    		{   //out of boundary
    		    occ_map_R[i] =0;		non_occ_R[i] = -128;
    		}
            i++;
    	}
	}
}
/* =========================================================================================== */
static void HoleFill_L(INT32 *fi_im, DVEC *level, INT32 *occ_map)
{
	INT32 width = m_depth_width;
	INT32 height = m_depth_height;
	INT32 i, x, y;
	INT32 count=0;
	INT32 sum_neighbor=0;	
	
	for(i=0;i<depth_buf_width*depth_buf_height;i++)
	{
        check_map[i] = (occ_map[i]!=255) ? 0 : 1;
	}
    
	//UL filling
	for (y=0;y<height;y++)
	{
    	for (x=0;x<width;x++)
    	{	
    		if (check_map[x+y*width]==0)
    		{
    			if (check_map[x+(y-1)*width]==1||check_map[(x-1)+y*width]==1)
    			{
    				count=0;
    				sum_neighbor=0;
                    
    				if (x-1>=0	 &&check_map[x-1+ y   *width]!=0)	
                        {   sum_neighbor+=fi_im[x-1+ y   *width];   count++;    }//L
    				if (x+1<width &&check_map[x+1+ y   *width]!=0)	
                        {   sum_neighbor+=level[x+1+ y   *width].x; count++;    }//R
    				if (y-1>=0	 &&check_map[x+  (y-1)*width]!=0)	
                        {   sum_neighbor+=fi_im[x+  (y-1)*width];   count++;    }//U
    				if (y+1<height&&check_map[x+  (y+1)*width]!=0)	
                        {   sum_neighbor+=level[x+  (y+1)*width].x; count++;    }//D
    				
    				if(count==0)	
                    {
                        check_map[x+y*width]=0;
    				}
    				else
                    {   
                        fi_im[x+y*width]=sum_neighbor/count;    
                        check_map[x+y*width]=1;
                    }
    			}
    		}
    		else
    		{
    			fi_im[x+y*width]=level[x  +y*width].x;	check_map[x+y*width]=1;
    		}	
    	}
	}
    
	//DR filling
	for (y=height-1;y>=0;y--)
	{
    	for (x=width-1;x>=0;x--)
    	{	
    		if (check_map[x+y*width]==0)
    		{
    			if (check_map[x+(y+1)*width]==1||check_map[(x+1)+y*width]==1)
    			{
    				count=0;
    				sum_neighbor=0;
                    
    				if (x-1>=0	 &&check_map[x-1+ y   *width]!=0)	
                        {   sum_neighbor+=fi_im[x-1+ y   *width];   count++;    }//L
    				if (x+1<width &&check_map[x+1+ y   *width]!=0)	
                        {   sum_neighbor+=fi_im[x+1+ y   *width];   count++;    }//R
    				if (y-1>=0	 &&check_map[x+  (y-1)*width]!=0)	
                        {   sum_neighbor+=fi_im[x+  (y-1)*width];   count++;    }//U
    				if (y+1<height&&check_map[x+  (y+1)*width]!=0)	
                        {   sum_neighbor+=fi_im[x+  (y+1)*width];   count++;    }//D
    				
    				if (count==0)	
    				{
                        check_map[x+y*width]=0;
    				}
                    else
                    {   
                        fi_im[x+y*width]=sum_neighbor/count;    
                        check_map[x+y*width]=1;
                    }
    			}
    		}
    	}
	}
}

/* =========================================================================================== */
static void HoleFill_R(INT32 *fi_im, DVEC *level, INT32 *occ_map)
{
	INT32 width  = m_depth_width;
	INT32 height = m_depth_height;	
	INT32 i, x, y;
	INT32 count=0;
	INT32 sum_neighbor=0;	
	
	for (i=0; i<depth_buf_width*depth_buf_height; i++)
	{
        check_map[i]= (occ_map[i]!=255)	? 0 : 1;
	}
    
	//UL filling
	for (y=0;y<height;y++)
	{
    	for (x=width-1;x>=0;x--)
    	{	
    		if (check_map[x+y*width]==0)
    		{
    			if (check_map[x+(y-1)*width]==1||check_map[(x+1)+y*width]==1)
    			{	
    				count=0;
    				sum_neighbor=0;
                    
    				if (x-1>=0	 &&check_map[x-1+ y   *width]!=0)	
                        {   sum_neighbor+=level[x-1+ y   *width].x; count++;    }//L
    				if (x+1<width &&check_map[x+1+ y   *width]!=0)	
                        {   sum_neighbor+=fi_im[x+1+ y   *width];   count++;    }//R
    				if (y-1>=0	 &&check_map[x+  (y-1)*width]!=0)	
                        {   sum_neighbor+=fi_im[x+  (y-1)*width];   count++;    }//U
    				if (y+1<height&&check_map[x+  (y+1)*width]!=0)	
                        {   sum_neighbor+=level[x+  (y+1)*width].x; count++;    }//D
    				
    				if (count==0)
                    {
                        check_map[x+y*width]=0;
    				}
    				else
                    {   
                        fi_im[x+y*width]=sum_neighbor/count;    
                        check_map[x+y*width]=1;
                    }
    			}
    		}
    		else
    		{
    			fi_im[x+y*width]=level[x  +y*width].x;	
                check_map[x+y*width]=1;
    		}	
    	}
	}
    
	//DR filling
	for (y=height-1;y>=0;y--)
	{
    	for (x=0;x<width;x++)
    	{	
    		if (check_map[x+y*width]==0)
    		{
    			if (check_map[x+(y+1)*width]==1||check_map[(x-1)+y*width]==1)
    			{	
    				count=0;
    				sum_neighbor=0;
                    
    				if (x-1>=0	 &&check_map[x-1+ y   *width]!=0)	
                        {   sum_neighbor+=fi_im[x-1+ y   *width];   count++;    }//L
    				if (x+1<width &&check_map[x+1+ y   *width]!=0)	
                        {   sum_neighbor+=fi_im[x+1+ y   *width];   count++;    }//R
    				if (y-1>=0	 &&check_map[x+  (y-1)*width]!=0)	
                        {   sum_neighbor+=fi_im[x+  (y-1)*width];   count++;    }//U
    				if (y+1<height&&check_map[x+  (y+1)*width]!=0)	
                        {   sum_neighbor+=fi_im[x+  (y+1)*width];   count++;    }//D
    				
    				if (count==0)	
                    {
                        check_map[x+y*width]=0;
    				}
    				else
                    {   
                        fi_im[x+y*width]=sum_neighbor/count;    
                        check_map[x+y*width]=1;
                    }
    			}
    		}
    	}
	}
}


/* =========================================================================================== */
static void DepthBlend(INT32 *render_depth_buf_L, INT32 *final_depth_buf_L,INT32 *cur_depth_buf_L, 
						INT32 *render_depth_buf_R, INT32 *final_depth_buf_R,INT32 *cur_depth_buf_R)
{
	INT32 total_shift_L=0,total_shift_R=0;
    INT32 i;
	INT32 final_shift_H=(total_shift_L-total_shift_R)/m_depth_height/m_depth_width/2;
	//INT32 occ_avg=(occ_count_L+occ_count_R)/2;

    for(i=0;i<m_depth_width*m_depth_height;i++)
    {
        final_depth_buf_L[i]=(cur_depth_buf_L[i]+3*final_depth_buf_L[i]+2)/4;
        final_depth_buf_R[i]=(cur_depth_buf_R[i]+3*final_depth_buf_R[i]+2)/4;
        total_shift_L+=final_depth_buf_L[i];
        total_shift_R+=final_depth_buf_R[i];
    }

    for(i=0; i<m_depth_width*m_depth_height; i++)
	{
		if(depth_plus==1)
		{
			render_depth_buf_L[i]=-(final_depth_buf_L[i]-final_shift_H)/2*TDDC_DIBR_GAIN/32;
			render_depth_buf_R[i]=-(final_depth_buf_R[i]+final_shift_H)/2*TDDC_DIBR_GAIN/32;
		}
		else
		{
			render_depth_buf_L[i]=ClipInt((final_depth_buf_L[i]-final_shift_H)/2*TDDC_DIBR_GAIN/32+final_shift_H/2+128);
			render_depth_buf_R[i]=ClipInt((final_depth_buf_R[i]+final_shift_H)/2*TDDC_DIBR_GAIN/32-final_shift_H/2+128);
		}
	}
}

/* =========================================================================================== */
static void DilterBiltInt(INT32 *Dout, PEL *Cin, INT32 *Din)
{
	INT32 width  =m_depth_width;
	INT32 height =m_depth_height;
    INT32 x, y;
	INT32 xmin, ymin, xmid, ymid, xmax, ymax;
	INT32 d0;
	INT32 weightTotal;
	INT32 weight00,weight01,weight02;
	INT32 weight10,weight11,weight12;
	INT32 weight20,weight21,weight22;

    for(y = 0; y < height; y++)
    for(x = 0; x < width; x++)
    {
		ymin = MAX(0, y-1); ymid=y;	ymax = MIN(height-1, y+1);
		xmin = MAX(0, x-1);	xmid=x;	xmax = MIN( width-1, x+1);
		d0 = 0;//reset value
		weightTotal = 0;

		weight00 = expf_Lut[MIN(ABS(Cin[xmin+ymin*width]-Cin[x+y*width]),95)];
		weight01 = expf_Lut[MIN(ABS(Cin[xmin+ymid*width]-Cin[x+y*width]),95)];
		weight02 = expf_Lut[MIN(ABS(Cin[xmin+ymax*width]-Cin[x+y*width]),95)];
		weight10 = expf_Lut[MIN(ABS(Cin[xmid+ymin*width]-Cin[x+y*width]),95)];
		weight11 = expf_Lut[MIN(ABS(Cin[xmid+ymid*width]-Cin[x+y*width]),95)];
		weight12 = expf_Lut[MIN(ABS(Cin[xmid+ymax*width]-Cin[x+y*width]),95)];
		weight20 = expf_Lut[MIN(ABS(Cin[xmax+ymin*width]-Cin[x+y*width]),95)];
		weight21 = expf_Lut[MIN(ABS(Cin[xmax+ymid*width]-Cin[x+y*width]),95)];
		weight22 = expf_Lut[MIN(ABS(Cin[xmax+ymax*width]-Cin[x+y*width]),95)];
		d0 += (Din[xmin+ymin*width]*weight00);	weightTotal +=weight00;
		d0 += (Din[xmin+ymid*width]*weight01);	weightTotal +=weight01;
		d0 += (Din[xmin+ymax*width]*weight02);	weightTotal +=weight02;
		d0 += (Din[xmid+ymin*width]*weight10);	weightTotal +=weight10;
		d0 += (Din[xmid+ymid*width]*weight11);	weightTotal +=weight11;
		d0 += (Din[xmid+ymax*width]*weight12);	weightTotal +=weight12;
		d0 += (Din[xmax+ymin*width]*weight20);	weightTotal +=weight20;
		d0 += (Din[xmax+ymid*width]*weight21);	weightTotal +=weight21;
		d0 += (Din[xmax+ymax*width]*weight22);	weightTotal +=weight22;
		
		if(weightTotal==0)
			Dout[x+y*width] = Din[x+y*m_depth_width];
		else
			Dout[x+y*width] = MIN(d0/weightTotal,255);
	}	
}

void DepthDown(INT32 *down_depth_buf_L, INT32 *cur_depth_buf_L, 
		INT32 *down_depth_buf_R, INT32 *cur_depth_buf_R, INT32 c_h_down, INT32 c_v_down)
{
	INT32 x, y;
	INT32 x1, x2, x3;
	int y1, y2, y3;

	if((c_h_down==1)&&(c_v_down==0))
	{
		for(y=0;y<m_depth_height;y++)
		for( x=0;x<(m_depth_width+1)/2;x++)
		{
			x1 = MAX(0, 2*x-1);	x2=2*x; x3 = MIN(m_depth_width-1, 2*x+1);
			//int addr=x+y*m_depth_width;
			down_depth_buf_L[x+y*m_depth_width]=(cur_depth_buf_L[x1+y*m_depth_width]+2*cur_depth_buf_L[x2+y*m_depth_width]+cur_depth_buf_L[x3+y*m_depth_width]+2)/4;
			down_depth_buf_R[x+y*m_depth_width]=(cur_depth_buf_R[x1+y*m_depth_width]+2*cur_depth_buf_R[x2+y*m_depth_width]+cur_depth_buf_R[x3+y*m_depth_width]+2)/4;
		}
	}
	else if((c_h_down==0)&&(c_v_down==1))
	{
		for(y=0;y<(m_depth_height+1)/2;y++)
		for(x=0;x<m_depth_width;x++)
		{
			y1 = MAX(0, 2*y-1);	y2=2*y; y3 = MIN(m_depth_height-1, 2*y+1);
			//int addr=x+y*m_depth_width;
			down_depth_buf_L[x+y*m_depth_width]=(cur_depth_buf_L[x+y1*m_depth_width]+2*cur_depth_buf_L[x+y2*m_depth_width]+cur_depth_buf_L[x+y3*m_depth_width]+2)/4;
			down_depth_buf_R[x+y*m_depth_width]=(cur_depth_buf_R[x+y1*m_depth_width]+2*cur_depth_buf_R[x+y2*m_depth_width]+cur_depth_buf_R[x+y3*m_depth_width]+2)/4;
		}
	}
	else if((c_h_down==1)&&(c_v_down==1))
	{
		for(y=0;y<(m_depth_height+1)/2;y++)
		for(x=0;x<m_depth_width;x++)
		{
			y1 = MAX(0, 2*y-1);	y2=2*y; y3 = MIN(m_depth_height-1, 2*y+1);
			//int addr=x+y*m_depth_width;
			down_depth_buf_L[x+y*m_depth_width]=(cur_depth_buf_L[x+y1*m_depth_width]+2*cur_depth_buf_L[x+y2*m_depth_width]+cur_depth_buf_L[x+y3*m_depth_width]+2)/4;
			down_depth_buf_R[x+y*m_depth_width]=(cur_depth_buf_R[x+y1*m_depth_width]+2*cur_depth_buf_R[x+y2*m_depth_width]+cur_depth_buf_R[x+y3*m_depth_width]+2)/4;
		}
		
		for(y=0;y<(m_depth_height+1)/2;y++)
		for(x=0;x<(m_depth_width+1)/2;x++)
		{
				x1 = MAX(0, 2*x-1);	x2=2*x; x3 = MIN(m_depth_width-1, 2*x+1);
			//int addr=x+y*m_depth_width;
			down_depth_buf_L[x+y*m_depth_width]=(down_depth_buf_L[x1+y*m_depth_width]+2*down_depth_buf_L[x2+y*m_depth_width]+down_depth_buf_L[x3+y*m_depth_width]+2)/4;
			down_depth_buf_R[x+y*m_depth_width]=(down_depth_buf_R[x1+y*m_depth_width]+2*down_depth_buf_R[x2+y*m_depth_width]+down_depth_buf_R[x3+y*m_depth_width]+2)/4;
		}
	}
}

void vDrvTddcUpdateParam(void)
{    	
	UINT32 img_width = IO32ReadFldAlign(TDDC_00, C_TDDC_IMG_WIDTH);
	UINT32 img_height = IO32ReadFldAlign(TDDC_00, C_TDDC_IMG_HEIGHT);

    depth_buf_width = ((img_width+7)/8+m_mbSize-1)/m_mbSize;
    depth_buf_height = ((img_height+7)/8+m_mbSize-1)/m_mbSize;    
    m_depth_width = depth_buf_width;
    m_depth_height = depth_buf_height;
}


void vDrvTddcInit(INT32 img_width, INT32 img_height, INT32 ratio_x, INT32 ratio_y, INT32 block_size)
{    
    INT32 i;    

    if (u1TddcInitFlag != SV_FALSE)
    {        
        LOG(0, "TDDC already init\n");
        return;
    }
    
    depth_buf_width = (img_width/4+block_size-1)/block_size;
    depth_buf_height = (img_height/4+block_size-1)/block_size;    
    m_depth_width = depth_buf_width;
    m_depth_height = depth_buf_height;
    m_mbSize = block_size;

	check_map = (INT32*)x_mem_alloc(sizeof(INT32)*m_depth_width*m_depth_height);
    m_pDownY32_L = (PEL*)x_mem_alloc(m_depth_width*m_depth_height); 
    m_pDownY32_R = (PEL*)x_mem_alloc(m_depth_width*m_depth_height);	
	
	DE_buf_L = (DVEC*)x_mem_alloc(sizeof(DVEC)*depth_buf_width*depth_buf_height);	
	DE_buf_R = (DVEC*)x_mem_alloc(sizeof(DVEC)*depth_buf_width*depth_buf_height);	
	depth_buf_L = (INT32*)x_mem_alloc(sizeof(INT32)*depth_buf_width*depth_buf_height);	        
	depth_buf_R = (INT32*)x_mem_alloc(sizeof(INT32)*depth_buf_width*depth_buf_height);	        
	filter_buf_L = (INT32*)x_mem_alloc(sizeof(INT32)*depth_buf_width*depth_buf_height);	        
	filter_buf_R = (INT32*)x_mem_alloc(sizeof(INT32)*depth_buf_width*depth_buf_height);	        
	final_depth_buf_L = (INT32*)x_mem_alloc(sizeof(INT32)*depth_buf_width*depth_buf_height);	    
	final_depth_buf_R = (INT32*)x_mem_alloc(sizeof(INT32)*depth_buf_width*depth_buf_height);	    
	render_depth_buf_L = (INT32*)x_mem_alloc(sizeof(INT32)*depth_buf_width*depth_buf_height);	    
	render_depth_buf_R = (INT32*)x_mem_alloc(sizeof(INT32)*depth_buf_width*depth_buf_height);	    
	occ_map_L = (INT32*)x_mem_alloc(sizeof(INT32)*depth_buf_width*depth_buf_height);	            
	occ_map_R = (INT32*)x_mem_alloc(sizeof(INT32)*depth_buf_width*depth_buf_height);	            
	non_occ_L = (INT32*)x_mem_alloc(sizeof(INT32)*depth_buf_width*depth_buf_height);	            
	non_occ_R = (INT32*)x_mem_alloc(sizeof(INT32)*depth_buf_width*depth_buf_height);	            

    if ((m_pDownY32_L == NULL) || (m_pDownY32_R == NULL) || 
        (DE_buf_L == NULL) || (DE_buf_R == NULL) ||
        (depth_buf_L == NULL) || (depth_buf_R == NULL) || 
        (filter_buf_L == NULL) || (filter_buf_R == NULL) ||
        (final_depth_buf_L == NULL) || (final_depth_buf_R == NULL) || 
        (render_depth_buf_L == NULL) || (render_depth_buf_R == NULL) ||
        (occ_map_L == NULL) || (occ_map_R == NULL) || 
        (non_occ_L == NULL) || (non_occ_R == NULL) || (check_map == NULL))
    {
        LOG(0, "TDDC Alloc memory fail !!!");
        return;
    }
    else
    {        
        LOG(0, "TDDC Alloc memory OK !!!");
    }
    
	for(i=0;i<depth_buf_width*depth_buf_height;i++)
	{
		DE_buf_L[i].x = (i%512)-256;//-512;
        DE_buf_L[i].y = (i%48)-24;//-32;
		DE_buf_R[i].x = (i%512)-256;//-512;
        DE_buf_R[i].y = (i%48)-24;//-32;        
	}
    
    x_memset(m_pDownY32_L, 0, sizeof(PEL)*m_depth_width*m_depth_height);
    x_memset(m_pDownY32_R, 0, sizeof(PEL)*m_depth_width*m_depth_height);
    x_memset(depth_buf_L,   0, sizeof(INT32)*depth_buf_width*depth_buf_height);
    x_memset(depth_buf_R,   0, sizeof(INT32)*depth_buf_width*depth_buf_height);
    x_memset(filter_buf_L,  0, sizeof(INT32)*depth_buf_width*depth_buf_height);
    x_memset(filter_buf_R,  0, sizeof(INT32)*depth_buf_width*depth_buf_height);
    x_memset(final_depth_buf_L,	0, sizeof(INT32)*depth_buf_width*depth_buf_height);
    x_memset(final_depth_buf_R,	0, sizeof(INT32)*depth_buf_width*depth_buf_height);
    x_memset(render_depth_buf_L, 128, sizeof(INT32)*depth_buf_width*depth_buf_height);
    x_memset(render_depth_buf_R, 128, sizeof(INT32)*depth_buf_width*depth_buf_height);
    x_memset(occ_map_L, 0, sizeof(INT32)*depth_buf_width*depth_buf_height);
    x_memset(occ_map_R, 0, sizeof(INT32)*depth_buf_width*depth_buf_height);
    x_memset(non_occ_L, 0, sizeof(INT32)*depth_buf_width*depth_buf_height);
    x_memset(non_occ_R, 0, sizeof(INT32)*depth_buf_width*depth_buf_height);

    u1TddcInitFlag = SV_TRUE;
}


void vDrvTddcProc(void)
{   
    if (u1TddcInitFlag != SV_FALSE)
    {
        OccProcess(DE_buf_L, DE_buf_R, occ_map_L, occ_map_R, non_occ_L, non_occ_R, shift_avg_H, shift_avg_V);

        HoleFill_L(depth_buf_L, DE_buf_L, occ_map_L);
        HoleFill_R(depth_buf_R, DE_buf_R, occ_map_R);	
        DilterBiltInt(filter_buf_L, m_pDownY32_L, depth_buf_L);
        DilterBiltInt(filter_buf_R, m_pDownY32_R, depth_buf_R);
        DepthBlend(render_depth_buf_L, final_depth_buf_L, filter_buf_L, 
			render_depth_buf_R, final_depth_buf_R, filter_buf_R);
	}

    //printf("\t Avg_H = %3d, Avg_V = %3d, Convg=%3d\n", shift_avg_H, shift_avg_V, hw_convg);				
}



