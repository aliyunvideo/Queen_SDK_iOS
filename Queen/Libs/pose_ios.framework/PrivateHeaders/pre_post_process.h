//
//  pre_post_process.h
//  bokeh_ios
//
//  Created by Jiayong Deng on 2021/3/31.
//

#ifndef pre_post_process_h
#define pre_post_process_h

#include <stdio.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <time.h>
#include <iostream>
#include <fstream>
#include <iostream>
#include <iomanip>
using std::ofstream;
using namespace std;

#define AIR_NEON_OPTED

//#if defined | __ANDROID__ || defined __linux__
//#define AIR_NEON_OPTED
//#else
//#define AIR_AVX_OPTED
//#endif

#ifdef AIR_AVX_OPTED
#include <immintrin.h>
#include <xmmintrin.h>
#include <emmintrin.h>
#include <smmintrin.h>
#endif

#ifdef AIR_NEON_OPTED
#include <arm_neon.h>
#endif

namespace air
{
//    void uint8_to_float32(uint8_t *src, float *dst, int num, float frac)
//    {
//        uint8_t *p_src = src;
//        float *p_dst = dst;
//    #ifdef AIR_NEON_OPTED
//        float32x4_t mm_frac = vdupq_n_f32(frac); //set a float value
//        int i = 0;
//        uint8x16_t src_vec;
//        uint16x8_t dst_u16_1, dst_u16_2;
//        uint32x4_t dst_u32_1, dst_u32_2, dst_u32_3, dst_u32_4;
//        for (; i + 16 <= num; i += 16)
//        {
//            int nBlockWidth = 16;
//
//            //uint8x16_t
//            src_vec = vld1q_u8(p_src);
//
//            //uint8x16_t -> uint8x8_t -> uint16x8_t
//            dst_u16_1 = vmovl_u8(vget_low_u8(src_vec));//[a, b, c, d, e, f, g, h]
//            dst_u16_2 = vmovl_u8(vget_high_u8(src_vec)); //[i, j, k, l, m, n, o, p]
//
//            //uint16x8_t -> uint16x4_t -> uint32x4_t -> float32x4_t
//            dst_u32_1 = vmovl_u16(vget_low_u16(dst_u16_1)); //[a, b, c, d]
//            vst1q_f32(p_dst + 0, vmulq_f32(vcvtq_f32_u32(dst_u32_1), mm_frac));
//
//            dst_u32_2 = vmovl_u16(vget_high_u16(dst_u16_1)); //[e, f, g, h]
//            vst1q_f32(p_dst + 4 , vmulq_f32(vcvtq_f32_u32(dst_u32_2), mm_frac));
//
//            dst_u32_3 = vmovl_u16(vget_low_u16(dst_u16_2));//[i, j, k, l]
//            vst1q_f32(p_dst + 8 ,vmulq_f32(vcvtq_f32_u32(dst_u32_3), mm_frac));
//
//            dst_u32_4 = vmovl_u16(vget_high_u16(dst_u16_2));//[m, n, o, p]
//            vst1q_f32(p_dst + 12,vmulq_f32(vcvtq_f32_u32(dst_u32_4), mm_frac));
//
//            p_src += nBlockWidth;
//            p_dst += nBlockWidth;
//        }
//        uint8x8_t u8x8src;
//        uint16x8_t u16x8src;
//        uint32x4_t u32x4srch, u32x4srcl;
//        float32x4_t f32x4srch, f32x4srcl;
//        float32x4x2_t f32x4x2dst;
//        for (; i + 8 <= num; i += 8)
//        {
//            int nBlockWidth = 8;
//
//            //uint8x8_t [a, b, c, d, e, f, g, h]
//            u8x8src = vld1_u8(p_src);
//
//            //1 u8x8->u16x8
//            u16x8src = vmovl_u8(u8x8src);
//
//            //2 u16x8 -> u32x4high, u32x4low
//            u32x4srcl = vmovl_u16(vget_low_u16(u16x8src));  //[a, b, c, d]
//            u32x4srch = vmovl_u16(vget_high_u16(u16x8src)); //[e, f, g, h]
//
//            //3 u32x4high, u32x4low -> f32x4high, f32x4low
//            f32x4srcl = vcvtq_f32_u32(u32x4srcl); //[a, b, c, d]
//            f32x4srch = vcvtq_f32_u32(u32x4srch); //[e, f, g, h]
//
//            //4 mul of f32x4high, f32x4low
//            f32x4srcl = vmulq_f32(f32x4srcl, mm_frac); //[a, b, c, d]
//            f32x4srch = vmulq_f32(f32x4srch, mm_frac); //[e, f, g, h]
//
//            //5 zip [a, b, c, d] and [e, f, g, h] to [a, e, b, f] and [ c, g, d, h ]
//            //float32x4x2_t tmp = vzipq_f32(f32x4srch, f32x4srcl);
//
//            //5 uzip [a, b, c, d] and [e, f, g, h] to [a, c, e, g] and [b, d, f, h]
//            f32x4x2dst = vuzpq_f32(f32x4srcl, f32x4srch);
//
//            //6 store [a, c, e, g] and [b, d, f, h] to [a, b, c, d, e, f, g, h]
//            vst2q_f32(p_dst, f32x4x2dst);
//
//            p_src += nBlockWidth;
//            p_dst += nBlockWidth;
//        }
//        for (; i < num; i++)
//        {
//            p_dst[i] = p_src[i] * frac;
//        }
//    #endif
//
//    #ifdef AIR_SSE_OPTED
//        __m128i mask1 = _mm_set_epi8(3,2,1,0,15,14,13,12,11,10,9,8,7,6,5,4);
//        __m128i mask2 = _mm_set_epi8(7,6,5,4,3,2,1,0,15,14,13,12,11,10,9,8);
//        __m128i mask3 = _mm_set_epi8(11,10,9,8,7,6,5,4,3,2,1,0,15,14,13,12);
//        __m128i src_vec, src_vec1, src_vec2, src_vec3, dst_i32_1, dst_i32_2;
//        __m128 dst, dst1, dst2, dst3;
//        __m128 mm_frac = _mm_set_ps1(frac);
//        int i = 0;
//        for(; i <= num; i += 16)
//        {
//            //[a, b, c, d, e, f, g, h, i, j, k, l, m, n, o, p]
//            src_vec = _mm_loadu_si128((__m128i_u *)src_row);
//            //uint8x4 -> int32x4 [a, b, c, d]
//            dst_i32_1 = _mm_cvtepu8_epi32(src_vec);
//            //uint32x4 -> float32x4 [a, b, c, d]
//            dst = _mm_cvtepi32_ps(dst_i32_1);
//            _mm_storeu_ps(dst_row + 0, _mm_mul_ps(dst, mm_frac));
//
//            //[e, f, g, h]
//            src_vec1 = _mm_shuffle_epi8(src_vec, mask1);
//            //uint8x4 -> int32x4 [e, f, g, h]
//            dst_i32_2 = _mm_cvtepu8_epi32(src_vec1);
//            //uint32x4 -> float32x4 [e, f, g, h]
//            dst1 = _mm_cvtepi32_ps(dst_i32_2);
//            _mm_storeu_ps(dst_row + 4, _mm_mul_ps(dst1, mm_frac));
//
//            //[i, j, k, l]
//            src_vec2 = _mm_shuffle_epi8(src_vec, mask2);
//            dst_i32_1 = _mm_cvtepu8_epi32(src_vec2);
//            dst2 = _mm_cvtepi32_ps (dst_i32_1);
//            _mm_storeu_ps(dst_row + 8, _mm_mul_ps(dst2, mm_frac));
//
//            //[m, n, o, p]
//            src_vec3 = _mm_shuffle_epi8(src_vec, mask3);
//            dst_i32_2 = _mm_cvtepu8_epi32(src_vec3);
//            dst3 = _mm_cvtepi32_ps (dst_i32_2);
//            _mm_storeu_ps(dst_row + 12, _mm_mul_ps(dst3, mm_frac));
//        }
//        for (; i < num; i++)
//        {
//            p_dst[i] = p_src[i] * frac;
//        }
//    #endif
//    }
//
//
//    void float32_to_uint8(float *src, uint8_t *dst, int num, float frac)
//    {
//    float *p_src = src;
//    uint8_t *p_dst = dst;
//    #ifdef AIR_NEON_OPTED
//    float32x4_t mm_frac = vdupq_n_f32(frac); //set a float value
//    int i = 0;
//    for (; i + 8 <= num; i += 8)
//    {
//        int nBlockWidth = 8;
//
//        float32x4_t f32_4_src1 = vld1q_f32(p_src + 0); //[a, b, c, d]
//        float32x4_t f32_4_src2 = vld1q_f32(p_src + 4); //[e, f, g, h]
//
//        //mul of float32x4 [a, b, c, d]
//        f32_4_src1 = vmulq_f32(f32_4_src1, mm_frac); //[a, b, c, d]
//        f32_4_src2 = vmulq_f32(f32_4_src2, mm_frac); //[e, f, g, h]
//
//        //3 f32x4 -> u32x4
//        uint32x4_t u32x4src1 = vcvtq_u32_f32(f32_4_src1); //[a, b, c, d]
//        uint32x4_t u32x4src2 = vcvtq_u32_f32(f32_4_src2); //[e, f, g, h]
//
//        //u32x4 -> u16x4
//        uint16x4_t  u16x4src1 = vmovn_u32(u32x4src1); //[a, b, c, d]
//        uint16x4_t  u16x4src2 = vmovn_u32(u32x4src2); //[e, f, g, h]
//
//        //u16x4 and u16x4 to u16x8
//        //combine [a, b, c, d] and [e, f, g, h] to [a, b, c, d, e, f, g, h]
//        uint16x8_t  u16x8src = vcombine_u16(u16x4src1, u16x4src2);
//
//        //u16x8 -> u8x8
//        uint8x8_t  u8x8src = vmovn_u16(u16x8src); //[a, b, c, d, e, f, g, h]
//
//        //store [a, b, c, d, e, f, g, h]
//        vst1_u8(p_dst, u8x8src);
//
//        p_src += nBlockWidth;
//        p_dst += nBlockWidth;
//    }
//    for (; i < num; i++)
//    {
//        p_dst[i] = (uint8_t)(p_src[i] * frac);
//    }
//    #endif
//
//    #ifdef AIR_SSE_OPTED
//
//
//    #endif
//    }
//
//
//
//    void float32_to_uint8_1(float *src, uint8_t *dst, int num, float frac)
//    {
//        float *p_src = src;
//        uint8_t *p_dst = dst;
//    #ifdef AIR_NEON_OPTED
//        float32x4_t mm_frac = vdupq_n_f32(frac); //set a float value
//        int i = 0;
//        for (; i + 8 <= num; i += 8)
//        {
//            int nBlockWidth = 8;
//
//            //[a, c, e, g] and [b, d, f, h]
//            float32x4x2_t f32_4x2_src = vld2q_f32(p_src);
//
//            //mul of float32x4 [a, c, e, g] and [b, d, f, h]
//            float32x4_t f32x4src1 = vmulq_f32(f32_4x2_src.val[0], mm_frac);
//            float32x4_t f32x4src2 = vmulq_f32(f32_4x2_src.val[1], mm_frac);
//
//            //3 f32x4 -> u32x4
//            uint32x4_t u32x4src1 = vcvtq_u32_f32(f32x4src1); //[a, c, e, g]
//            uint32x4_t u32x4src2 = vcvtq_u32_f32(f32x4src2); //[b, d, f, h]
//
//            //u32x4 -> u16x4
//            uint16x4_t  u16x4src1 = vmovn_u32(u32x4src1); //[a, c, e, g]
//            uint16x4_t  u16x4src2 = vmovn_u32(u32x4src2); //[b, d, f, h]
//
//            //uint16x4_t -> uint16x4_t
//            u16x4src2 = vqshl_n_u16(u16x4src2, 8); //[b, d, f, h] bigger value shift to left
//
//            //uint16x4_t -> u8x8srch
//            uint8x8_t u8x8src1  = vreinterpret_u8_u16(u16x4src1); //[a, c, e, g]
//            uint8x8_t u8x8src2  = vreinterpret_u8_u16(u16x4src2); //[b, d, f, h]
//
//            //max of  //[a, 0, c, 0, e, 0, g, 0] and [0, b, 0, d, 0, f, 0, h], dst = [a, b, c, d, e, f, g, h]
//            uint8x8_t  u8x8max = vmax_u8(u8x8src1, u8x8src2);
//
//            //6 store [a, c, e, g] and [b, d, f, h] to [a, b, c, d, e, f, g, h]
//            vst1_u8(p_dst, u8x8max);
//
//            p_src += nBlockWidth;
//            p_dst += nBlockWidth;
//        }
//        for (; i < num; i++)
//        {
//            p_dst[i] = (uint8_t)(p_src[i] * frac);
//        }
//    #endif
//
//    #ifdef AIR_SSE_OPTED
//
//
//    #endif
//    }



    //static const float map_tbl[256] =
    //{
    //0.0000000f, 0.0039216f, 0.0078431f, 0.0117647f, 0.0156863f, 0.0196078f, 0.0235294f, 0.0274510f, 0.0313725f, 0.0352941f, 0.0392157f, 0.0431373f, 0.0470588f, 0.0509804f, 0.0549020f, 0.0588235f,
    //0.0627451f, 0.0666667f, 0.0705882f, 0.0745098f, 0.0784314f, 0.0823529f, 0.0862745f, 0.0901961f, 0.0941176f, 0.0980392f, 0.1019608f, 0.1058824f, 0.1098039f, 0.1137255f, 0.1176471f, 0.1215686f,
    //0.1254902f, 0.1294118f, 0.1333333f, 0.1372549f, 0.1411765f, 0.1450980f, 0.1490196f, 0.1529412f, 0.1568627f, 0.1607843f, 0.1647059f, 0.1686275f, 0.1725490f, 0.1764706f, 0.1803922f, 0.1843137f,
    //0.1882353f, 0.1921569f, 0.1960784f, 0.2000000f, 0.2039216f, 0.2078431f, 0.2117647f, 0.2156863f, 0.2196078f, 0.2235294f, 0.2274510f, 0.2313725f, 0.2352941f, 0.2392157f, 0.2431373f, 0.2470588f,
    //0.2509804f, 0.2549020f, 0.2588235f, 0.2627451f, 0.2666667f, 0.2705882f, 0.2745098f, 0.2784314f, 0.2823529f, 0.2862745f, 0.2901961f, 0.2941176f, 0.2980392f, 0.3019608f, 0.3058824f, 0.3098039f,
    //0.3137255f, 0.3176471f, 0.3215686f, 0.3254902f, 0.3294118f, 0.3333333f, 0.3372549f, 0.3411765f, 0.3450980f, 0.3490196f, 0.3529412f, 0.3568627f, 0.3607843f, 0.3647059f, 0.3686275f, 0.3725490f,
    //0.3764706f, 0.3803922f, 0.3843137f, 0.3882353f, 0.3921569f, 0.3960784f, 0.4000000f, 0.4039216f, 0.4078431f, 0.4117647f, 0.4156863f, 0.4196078f, 0.4235294f, 0.4274510f, 0.4313725f, 0.4352941f,
    //0.4392157f, 0.4431373f, 0.4470588f, 0.4509804f, 0.4549020f, 0.4588235f, 0.4627451f, 0.4666667f, 0.4705882f, 0.4745098f, 0.4784314f, 0.4823529f, 0.4862745f, 0.4901961f, 0.4941176f, 0.4980392f,
    //0.5019608f, 0.5058824f, 0.5098039f, 0.5137255f, 0.5176471f, 0.5215686f, 0.5254902f, 0.5294118f, 0.5333333f, 0.5372549f, 0.5411765f, 0.5450980f, 0.5490196f, 0.5529412f, 0.5568627f, 0.5607843f,
    //0.5647059f, 0.5686275f, 0.5725490f, 0.5764706f, 0.5803922f, 0.5843137f, 0.5882353f, 0.5921569f, 0.5960784f, 0.6000000f, 0.6039216f, 0.6078431f, 0.6117647f, 0.6156863f, 0.6196078f, 0.6235294f,
    //0.6274510f, 0.6313725f, 0.6352941f, 0.6392157f, 0.6431373f, 0.6470588f, 0.6509804f, 0.6549020f, 0.6588235f, 0.6627451f, 0.6666667f, 0.6705882f, 0.6745098f, 0.6784314f, 0.6823529f, 0.6862745f,
    //0.6901961f, 0.6941176f, 0.6980392f, 0.7019608f, 0.7058824f, 0.7098039f, 0.7137255f, 0.7176471f, 0.7215686f, 0.7254902f, 0.7294118f, 0.7333333f, 0.7372549f, 0.7411765f, 0.7450980f, 0.7490196f,
    //0.7529412f, 0.7568627f, 0.7607843f, 0.7647059f, 0.7686275f, 0.7725490f, 0.7764706f, 0.7803922f, 0.7843137f, 0.7882353f, 0.7921569f, 0.7960784f, 0.8000000f, 0.8039216f, 0.8078431f, 0.8117647f,
    //0.8156863f, 0.8196078f, 0.8235294f, 0.8274510f, 0.8313725f, 0.8352941f, 0.8392157f, 0.8431373f, 0.8470588f, 0.8509804f, 0.8549020f, 0.8588235f, 0.8627451f, 0.8666667f, 0.8705882f, 0.8745098f,
    //0.8784314f, 0.8823529f, 0.8862745f, 0.8901961f, 0.8941176f, 0.8980392f, 0.9019608f, 0.9058824f, 0.9098039f, 0.9137255f, 0.9176471f, 0.9215686f, 0.9254902f, 0.9294118f, 0.9333333f, 0.9372549f,
    //0.9411765f, 0.9450980f, 0.9490196f, 0.9529412f, 0.9568627f, 0.9607843f, 0.9647059f, 0.9686275f, 0.9725490f, 0.9764706f, 0.9803922f, 0.9843137f, 0.9882353f, 0.9921569f, 0.9960784f, 1.0000000f,
    //};
    //
    //static const float imean[3] = {0.485f, 0.456f, 0.406f};
    //static const float istd[3] = {0.229f, 0.224f, 0.225f};
    //static const float istd_mul[3] = {4.3668122f, 4.4642857f, 4.4444444f};

template<typename T1, typename T2>
AIR_RET air_resize_nearst_avx(
                              T1 *src, int src_width, int src_height, int src_channel,
                              T2 *dst, int dst_width, int dst_height, int dst_channel)
{
#if defined(TIME_DEBUG)
    airf64 timer = air_get_current_system_time();
#endif
    
    AIR_RET ret = AIR_RET_OK;
    
    if (!src || !dst)
    {
        ret = AIR_RET_ERR_MEM;
        return ret;
    }
    if (src_channel != dst_channel)
    {
        ret = AIR_RET_ERR_PARA;
        return ret;
    }
    
    int src_stride = src_width * src_channel;
    int dst_stride = dst_width * dst_channel;
    
    int i, j;
    airf32 fy = (airf32)(src_height - 1) / (dst_height - 1); //y系数
    airf32 fx = (airf32)(src_width - 1) / (dst_width - 1); //x系数
    
    air16s *x_nearst = new air16s[dst_width]; //x轴最近邻下标
    x_nearst[0] = 0;
    x_nearst[dst_width - 1] = src_width - 1;
    for (j = 1; j < dst_width - 1; j++)
    {
        x_nearst[j] = floorf(j * fx + 0.5f);
    }
    
    for (i = 0; i < dst_height; i++)
    {
        int y_nearst = 0;
        if (i == 0)
        {
            y_nearst = 0;
        }
        else if (i == dst_height - 1)
        {
            y_nearst = src_height - 1;
        }
        else
        {
            y_nearst = floorf(i * fy + 0.5f); //差值上侧值下标
        }
        
        T1 *src_line = src + y_nearst * src_stride;  //输入图第一行
        T2 *dst_line = dst + i * dst_stride; //输出图当前行
        
        for (int j = 0; j < dst_width; j++)
        {
            T1 *input_line_offset  = src_line + x_nearst[j] * src_channel; //左上角值指针
            T2 *p_dst_line = dst_line + j * src_channel;
            
            for (int k = 0; k < src_channel; ++k)
            {
                p_dst_line[k] = (input_line_offset[k]);
            }
        }
    }
    
    if (x_nearst)
    {
        free(x_nearst);
        x_nearst = NULL;
    }
#if defined(TIME_DEBUG)
    timer = air_get_current_system_time() - timer;
    printf("resize_bilinear_norm time_cost: %lf\n", timer);
    t_post_resize += timer;
#endif
    
    return ret;
}


template <typename T1, typename T2>
AIR_RET air_resize_nearst_gray_avx(
                                   T1 *src, int src_width, int src_height, int src_channel,
                                   T2 *dst, int dst_width, int dst_height, int dst_channel)
{
#if defined(TIME_DEBUG)
    airf64 timer = air_get_current_system_time();
#endif
    
    AIR_RET ret = AIR_RET_OK;
    
    if (!src || !dst)
    {
        ret = AIR_RET_ERR_MEM;
        return ret;
    }
    if (src_channel != 3 * dst_channel)
    {
        ret = AIR_RET_ERR_PARA;
        return ret;
    }
    
    int src_stride = src_width * src_channel;
    int dst_stride = dst_width * dst_channel;
    
    int i, j;
    airf32 fy = (airf32)(src_height - 1) / (dst_height - 1); //y系数
    airf32 fx = (airf32)(src_width - 1) / (dst_width - 1);   //x系数
    
    air16s *x_nearst = new air16s[dst_width]; //x轴最近邻下标
    x_nearst[0] = 0;
    x_nearst[dst_width - 1] = src_width - 1;
    for (j = 1; j < dst_width - 1; j++)
    {
        x_nearst[j] = floorf(j * fx + 0.5f);
    }
    
    for (i = 0; i < dst_height; i++)
    {
        int y_nearst = 0;
        if (i == 0)
        {
            y_nearst = 0;
        }
        else if (i == dst_height - 1)
        {
            y_nearst = src_height - 1;
        }
        else
        {
            y_nearst = floorf(i * fy + 0.5f); //差值上侧值下标
        }
        
        T1 *src_line = src + y_nearst * src_stride; //输入图第一行
        T2 *dst_line = dst + i * dst_stride;        //输出图当前行
        
        for (int j = 0; j < dst_width; j++)
        {
            T1 *input_line_offset = src_line + x_nearst[j] * src_channel; //左上角值指针
            T2 *p_dst_line = dst_line + j * dst_channel;
            
            //0.299 0.587 0.114
            //Gray = (R*30 + G*59 + B*11 + 50) / 100
            //Gray = (R*38 + G*75 + B*15) >> 7
            
            p_dst_line[0] = (T2)(input_line_offset[0] * 0.299f + input_line_offset[1] * 0.587f + input_line_offset[2] * 0.114f);
        }
    }
    
    if (x_nearst)
    {
        delete[] x_nearst;
        x_nearst = NULL;
    }
#if defined(TIME_DEBUG)
    timer = air_get_current_system_time() - timer;
    printf("resize_bilinear_norm time_cost: %lf\n", timer);
    t_post_resize += timer;
#endif
    
    return ret;
}

//rgbrgb->rrggbb
template <typename T>
AIR_RET air_rotate_rgbrgb(const T *src, int src_width, int src_height, int src_channel, int rotate_degree, T *dst, int &dst_width, int &dst_height)
{
    AIR_RET ret = AIR_RET_OK;
    
    if (!src)
    {
        ret = AIR_RET_ERR_MEM;
        return ret;
    }
    if (src_width <= 0 || src_height <= 0)
    {
        ret = AIR_RET_ERR_PARA;
        return ret;
    }
    
    
    switch(rotate_degree)
    {
        case 0 :
        {
            dst_width = src_width;
            dst_height = src_height;
            
            memcpy(dst, src, src_height * src_width * src_channel * sizeof(T));
           
            break;
        }
        case 90 :
        {
            dst_width = src_height;
            dst_height = src_width;
            
            int dst_idx = 0;
            for (int x = 0; x < src_width; x++) //src x
            {
                for (int y = src_height - 1; y >= 0; y--) //src y
                {
                    for (int c = 0; c < src_channel; c++) //dst channel
                    {
                        dst[dst_idx++] = src[y * src_width * src_channel + x * src_channel + c];
                    }
                }
            }
            
            break;
        }
        case 180 :
        {
            dst_width = src_width;
            dst_height = src_height;
            
            int dst_idx = 0;
            for (int y = src_height - 1; y >= 0; y--) //dst y
            {
                for (int x = src_width - 1; x >= 0; x--) //dst x
                {
                    for (int c = 0; c < src_channel; c++) //dst channel
                    {
                        dst[dst_idx++] = src[y * src_width * src_channel + x * src_channel + c];
                    }
                }
            }
            
            break;
        }
        case 270 :
        {
            dst_width = src_height;
            dst_height = src_width;
            
            int dst_idx = 0;
            for (int x = src_width - 1; x >= 0; x--) //dst y
            {
                for (int y =  0; y < src_height; y++) //dst x
                {
                    for (int c = 0; c < src_channel; c++) //dst channel
                    {
                        dst[dst_idx++] = src[y * src_width * src_channel + x * src_channel + c];
                    }
                }
            }
            
            break;
        }
    }
    
    return ret;
}

//rgbrgb->rrggbb
template <typename T1, typename T2>
AIR_RET air_resize_nearst_rgbrgb2rrggbb_c3(
   T1 *src, int src_width, int src_height, int src_channel,
   T2 *dst, int dst_width, int dst_height, int dst_channel, int dst_spatial_ = 0)
{
    AIR_RET ret = AIR_RET_OK;
    
    if (!src || !dst)
    {
        ret = AIR_RET_ERR_MEM;
        return ret;
    }
    if (src_channel != dst_channel)
    {
        ret = AIR_RET_ERR_PARA;
        return ret;
    }
    
    
    //int src_stride = src_width * src_channel;
    //int dst_stride = dst_width * dst_channel;
    
    int i, j;
    float fy = (float)(src_height - 1) / (dst_height - 1); //y系数
    float fx = (float)(src_width - 1) / (dst_width - 1);   //x系数
    
    air16s *x_nearst = new air16s[dst_width]; //x轴最近邻下标
    x_nearst[0] = 0;
    x_nearst[dst_width - 1] = src_width - 1;
    for (j = 1; j < dst_width - 1; j++)
    {
        x_nearst[j] = floor(j * fx + 0.5f);
    }
    
    int dst_spatial = dst_height * dst_width;
    if (dst_spatial_ > 0)
        dst_spatial = dst_spatial_;
    T1 *input_line_offset;
    T2 *p_dst_line[3];
    
    for (i = 0; i < dst_height; i++)
    {
        int y_nearst = 0;
        if (i == 0)
        {
            y_nearst = 0;
        }
        else if (i == dst_height - 1)
        {
            y_nearst = src_height - 1;
        }
        else
        {
            y_nearst = floor(i * fy + 0.5f); //差值上侧值下标
        }
        
        T1 *src_line = src + (y_nearst * src_width) * 3; //输入图第一行

        //output:rrrgggbbb
        p_dst_line[0] = dst + i * dst_width;
        p_dst_line[1] = p_dst_line[0] + dst_spatial;
        p_dst_line[2] = p_dst_line[1] + dst_spatial;

        for (int j = 0; j < dst_width; j++)
        {        
            //input: rgbrgbrgb
            input_line_offset = src_line + x_nearst[j] * 3;

            //output:rrrgggbbb
            p_dst_line[0][j] = static_cast<T2>(input_line_offset[0]);
            p_dst_line[1][j] = static_cast<T2>(input_line_offset[1]);
            p_dst_line[2][j] = static_cast<T2>(input_line_offset[2]);
        }
    }
    
    if (x_nearst)
    {
        free(x_nearst);
        x_nearst = NULL;
    }
    
    return ret;
}

    
    
//rgbrgb->rrggbb
template <typename T1, typename T2>
AIR_RET air_resize_nearst_bgrbgr2rrggbb_c3(
                                           T1 *src, int src_width, int src_height, int src_channel,
                                           T2 *dst, int dst_width, int dst_height, int dst_channel, int dst_spatial_ = 0)
{
    AIR_RET ret = AIR_RET_OK;
    
    if (!src || !dst)
    {
        ret = AIR_RET_ERR_MEM;
        return ret;
    }
    if (src_channel != dst_channel)
    {
        ret = AIR_RET_ERR_PARA;
        return ret;
    }
    
    
    //int src_stride = src_width * src_channel;
    //int dst_stride = dst_width * dst_channel;
    
    int i, j;
    float fy = (float)(src_height - 1) / (dst_height - 1); //y系数
    float fx = (float)(src_width - 1) / (dst_width - 1);   //x系数
    
    air16s *x_nearst = new air16s[dst_width]; //x轴最近邻下标
    x_nearst[0] = 0;
    x_nearst[dst_width - 1] = src_width - 1;
    for (j = 1; j < dst_width - 1; j++)
    {
        x_nearst[j] = floor(j * fx + 0.5f);
    }
    
    int dst_spatial = dst_height * dst_width;
    if (dst_spatial_ > 0)
        dst_spatial = dst_spatial_;
    T1 *input_line_offset;
    T2 *p_dst_line[3];
    
    for (i = 0; i < dst_height; i++)
    {
        int y_nearst = 0;
        if (i == 0)
        {
            y_nearst = 0;
        }
        else if (i == dst_height - 1)
        {
            y_nearst = src_height - 1;
        }
        else
        {
            y_nearst = floor(i * fy + 0.5f); //差值上侧值下标
        }
        
        T1 *src_line = src + (y_nearst * src_width) * 3; //输入图第一行
        T2 *dst_line = dst + i * dst_width;        //输出图当前行
        
        for (int j = 0; j < dst_width; j++)
        {
            int xx_nearst = x_nearst[j];
            
            //input: rgbrgbrgb
            input_line_offset = src_line + xx_nearst * 3;
            
            //output:rrrgggbbb
            p_dst_line[0] = dst_line + j;
            p_dst_line[1] = dst_line + j + dst_spatial;
            p_dst_line[2] = dst_line + j + 2 * dst_spatial;
            
            *(p_dst_line[0]) = static_cast<T2>(input_line_offset[2]);
            *(p_dst_line[1]) = static_cast<T2>(input_line_offset[1]);
            *(p_dst_line[2]) = static_cast<T2>(input_line_offset[0]);
        }
    }
    
    if (x_nearst)
    {
        free(x_nearst);
        x_nearst = NULL;
    }
    
    return ret;
}
    
    
    
    
//rrggbb->rrggbb
template <typename T1, typename T2>
AIR_RET air_resize_nearst_rrggbb2rrggbb_c3(
   T1 *src, int src_width, int src_height, int src_channel,
   T2 *dst, int dst_width, int dst_height, int dst_channel, int dst_spatial_ = 0)
{
    AIR_RET ret = AIR_RET_OK;

    if (!src || !dst)
    {
        ret = AIR_RET_ERR_MEM;
        return ret;
    }
    if (src_channel != dst_channel)
    {
        ret = AIR_RET_ERR_PARA;
        return ret;
    }


    //int src_stride = src_width * src_channel;
    //int dst_stride = dst_width * dst_channel;

    int i, j;
    float fy = (float)(src_height - 1) / (dst_height - 1); //y系数
    float fx = (float)(src_width - 1) / (dst_width - 1);   //x系数

    air16s *x_nearst = new air16s[dst_width]; //x轴最近邻下标
    x_nearst[0] = 0;
    x_nearst[dst_width - 1] = src_width - 1;
    for (j = 1; j < dst_width - 1; j++)
    {
        x_nearst[j] = floor(j * fx + 0.5f);
    }

    int src_spatial = src_height * src_width;
    int dst_spatial = dst_height * dst_width;
    if (dst_spatial_ > 0)
        dst_spatial =dst_spatial_;
    T1 *input_line_offset[3];
    T2 *p_dst_line[3];

    for (i = 0; i < dst_height; i++)
    {
        int y_nearst = 0;
        if (i == 0)
        {
            y_nearst = 0;
        }
        else if (i == dst_height - 1)
        {
            y_nearst = src_height - 1;
        }
        else
        {
            y_nearst = floor(i * fy + 0.5f); //差值上侧值下标
        }

        T1 *src_line = src + y_nearst * src_width; //输入图第一行
        T2 *dst_line = dst + i * dst_width;        //输出图当前行

        for (int j = 0; j < dst_width; j++)
        {
            input_line_offset[0] = src_line + x_nearst[j];
            input_line_offset[1] = src_line + x_nearst[j] + src_spatial;
            input_line_offset[2] = src_line + x_nearst[j] + 2 * src_spatial;

            p_dst_line[0] = dst_line + j;
            p_dst_line[1] = dst_line + j + dst_spatial;
            p_dst_line[2] = dst_line + j + 2 * dst_spatial;

            *p_dst_line[0] = static_cast<T2>(*input_line_offset[0]);
            *p_dst_line[1] = static_cast<T2>(*input_line_offset[1]);
            *p_dst_line[2] = static_cast<T2>(*input_line_offset[2]);
        }
    }

    if (x_nearst)
    {
        free(x_nearst);
        x_nearst = NULL;
    }

    return ret;
}
    
//rgbrgb->rgbrgb
template <typename T1, typename T2>
AIR_RET air_resize_nearst_rgbrgb2rgbrgb_c3(
  T1 *src, int src_width, int src_height, int src_channel,
  T2 *dst, int dst_width, int dst_height, int dst_channel)
{
    AIR_RET ret = AIR_RET_OK;
    
    if (!src || !dst)
    {
        ret = AIR_RET_ERR_MEM;
        return ret;
    }
    if (src_channel != dst_channel)
    {
        ret = AIR_RET_ERR_PARA;
        return ret;
    }
    
    int src_stride = src_width * src_channel;
    int dst_stride = dst_width * dst_channel;
    
    int i, j;
    float fy = (float)(src_height - 1) / (dst_height - 1); //y系数
    float fx = (float)(src_width - 1) / (dst_width - 1);   //x系数
    
    air16s *x_nearst = new air16s[dst_width]; //x轴最近邻下标
    x_nearst[0] = 0;
    x_nearst[dst_width - 1] = src_width - 1;
    for (j = 1; j < dst_width - 1; j++)
    {
        x_nearst[j] = floor(j * fx + 0.5f);
    }
    
    for (i = 0; i < dst_height; i++)
    {
        int y_nearst = 0;
        if (i == 0)
        {
            y_nearst = 0;
        }
        else if (i == dst_height - 1)
        {
            y_nearst = src_height - 1;
        }
        else
        {
            y_nearst = floor(i * fy + 0.5f); //差值上侧值下标
        }
        
        T1 *src_line = src + y_nearst * src_stride; //输入图第一行
        T2 *dst_line = dst + i * dst_stride;        //输出图当前行
        
        for (int j = 0; j < dst_width; j++)
        {
            T1 *input_line_offset = src_line + x_nearst[j] * src_channel; //左上角值指针
            T2 *p_dst_line = dst_line + j * src_channel;
            
            for (int k = 0; k < src_channel; ++k)
            {
                p_dst_line[k] = (input_line_offset[k]);
            }
        }
    }
    
    if (x_nearst)
    {
        free(x_nearst);
        x_nearst = NULL;
    }
    
    return ret;
}


template <typename T>
void hwc_resize_biliner_hwc(T *src_image,
                            T *dst_image,
                            //float *y_scale,
                            //float *x_scale,
                            //int *x_coord,
                            //int *y_coord,
                            const int src_width,
                            const int src_height,
                            const int dst_width,
                            const int dst_height,
                            const int channel,
                            int inverse_channel)
{
    float scale_x = (float)(src_width - 1) / (dst_width - 1);
    float scale_y = (float)(src_height - 1) / (dst_height - 1);

    float *x_scale = new float[dst_width];
    float *y_scale = new float[dst_height];
    uint16_t *x_coord = new uint16_t[dst_width];
    uint16_t *y_coord = new uint16_t[dst_height];

    for(int w = 0; w < dst_width; w++)
    {
        if(w == 0)
        {
            x_coord[w] = 0;
            x_scale[w] = 0;
        } else if(w == dst_width - 1)
        {
            x_coord[w] = src_width - 2;
            x_scale[w] = 1;
        } else
        {
            x_coord[w] = floorf(w * scale_x);
            x_scale[w] = (w * scale_x - x_coord[w]);
        }
    }
    for(int h = 0; h<dst_height; h++)
    {
        if(h == 0)
        {
            y_coord[h] = 0;
            y_scale[h] = 0;
        }  else if(h == dst_height - 1)
        {
            y_coord[h] = src_height - 2;
            y_scale[h] = 1;
        } else
        {
            y_coord[h] = floorf(h * scale_y);
            y_scale[h] = (h * scale_y - y_coord[h]);
        }
    }

    for (int global_y = 0; global_y < dst_height; ++global_y)
    {
        for (int global_x = 0; global_x < dst_width; ++global_x)
        {

            float data_left;
            float data_right;
            float data_up;
            float data_down;
            if(global_x < dst_width && global_y < dst_height)
            {
                int line_stride_src = src_width * channel;
                int y_src_u = y_coord[global_y];
                int x_src_l = x_coord[global_x];
                float x_ratio = x_scale[global_x];
                float y_ratio = y_scale[global_y];
                int idx_src = y_src_u * line_stride_src + x_src_l * channel;
                int index_src = 0;
                int idx_dst = global_y * dst_width * channel + global_x * channel;
                int c = 0;
                for (; c < channel; ++c)
                {
                    index_src = idx_src + c;
                    data_left = src_image[index_src];
                    data_right = src_image[index_src + channel];
                    float up_data = x_ratio * (data_right - data_left) + data_left;
                    data_left = src_image[index_src + line_stride_src];
                    data_right = src_image[index_src + line_stride_src + channel];
                    float down_data = x_ratio * (data_right - data_left) + data_left;
                    float result_val = y_ratio * (down_data - up_data) + up_data;
                    if (inverse_channel)
                        dst_image[idx_dst + channel - 1 - c] = (T)result_val;
                    else
                        dst_image[idx_dst + c] = (T)result_val;
                }
            }
        }
    }

    if (x_scale)
    {
        delete[] x_scale;
        x_scale = NULL;
    }
    if (y_scale)
    {
        delete[] y_scale;
        y_scale = NULL;
    }
    if (x_coord)
    {
        delete[] x_coord;
        x_coord = NULL;
    }
    if (y_coord)
    {
        delete[] y_coord;
        y_coord = NULL;
    }
}

template <typename T>
inline int intRound(const T a)
{
    return int(a + 0.5f);
}

template <typename T>
inline T fastMin(const T a, const T b)
{
    return (a < b ? a : b);
}

void render_pose_keypoints1(
        //cv::Mat &frame,
        const vector<float> &keypoints,
        vector<int> keyshape,
        const float threshold,
        float scale,
        vector<int> &kpts,
        vector<int> &lines)
{
    kpts.clear();
    lines.clear();
    int pairs[] =
            {
                    1, 2, 1, 5, 2, 3, 3, 4, 5, 6, 6, 7, 1, 8, 8, 9, 9, 10,
                    1, 11, 11, 12, 12, 13, 1, 0, 0, 14, 14, 16, 0, 15, 15, 17};
    float colors[] =
            {
                    255.f, 0.f, 85.f, 255.f, 0.f, 0.f, 255.f, 85.f, 0.f, 255.f, 170.f, 0.f,
                    255.f, 255.f, 0.f, 170.f, 255.f, 0.f, 85.f, 255.f, 0.f, 0.f, 255.f, 0.f,
                    0.f, 255.f, 85.f, 0.f, 255.f, 170.f, 0.f, 255.f, 255.f, 0.f, 170.f, 255.f,
                    0.f, 85.f, 255.f, 0.f, 0.f, 255.f, 255.f, 0.f, 170.f, 170.f, 0.f, 255.f,
                    255.f, 0.f, 255.f, 85.f, 0.f, 255.f};
    const int pairs_size = sizeof(pairs) / sizeof(int);
    const int number_colors = sizeof(colors) / sizeof(float);

    const int num_person = keyshape[0];
    const int num_keypoints = keyshape[1]; //=18
    const int num_val_per_keypoint = keyshape[2]; //=3
    for (int person = 0; person < num_person; ++person)
    {
        // Draw lines
        for (int pairid = 0; pairid < pairs_size; pairid += 2)
        {
            const int index1 = (person * num_keypoints + pairs[pairid]) * num_val_per_keypoint;
            const int index2 = (person * num_keypoints + pairs[pairid + 1]) * num_val_per_keypoint;
            float score1 = keypoints[index1 + 2];
            float score2 = keypoints[index2 + 2];
            if (score1 > threshold && score2 > threshold)
            {
                //const int color_index = pairs[pairid + 1] * 3;
                const int colorB_index = (pairs[pairid + 1] * 3 + 2) % number_colors;
                const int colorG_index = (pairs[pairid + 1] * 3 + 1) % number_colors;
                const int colorR_index = (pairs[pairid + 1] * 3 + 0) % number_colors;
                int x1 = intRound(keypoints[index1 + 0] * scale);
                int y1 = intRound(keypoints[index1 + 1] * scale);
                int x2 = intRound(keypoints[index2 + 0] * scale);
                int y2 = intRound(keypoints[index2 + 1] * scale);

                lines.push_back(x1);
                lines.push_back(y1);
                lines.push_back(x2);
                lines.push_back(y2);

                //cv::Scalar color{colors[colorB_index], colors[colorG_index], colors[colorR_index]};
                //cv::Point keypoint1{x1, y1};
                //cv::Point keypoint2{x2, y2};
                //cv::line(frame, keypoint1, keypoint2, color, 2);
            }
        }
        // Draw circles
        for (int ptid = 0; ptid < num_keypoints; ++ptid)
        {
            const int index = (person * num_keypoints + ptid) * num_val_per_keypoint;
            if (keypoints[index + 2] > threshold)
            {
                //const int color_index = ptid * 3;
                const int colorB_index = (ptid * 3 + 2) % number_colors;
                const int colorG_index = (ptid * 3 + 1) % number_colors;
                const int colorR_index = (ptid * 3 + 0) % number_colors;
                int x = intRound(keypoints[index + 0] * scale);
                int y = intRound(keypoints[index + 1] * scale);

                kpts.push_back(x);
                kpts.push_back(y);

                //cv::Scalar color{colors[colorB_index], colors[colorG_index], colors[colorR_index]};
                //cv::Point center{x, y};
                //cv::circle(frame, center, 3, color, -1);
            }
        }
    }
}


void find_heatmap_peaks(
        int POSE_MAX_PEOPLE,
        const float *src,
        float *dst,
        const int SRCW,
        const int SRCH,
        const int SRC_CH,
        const float TH)
{
    // find peaks (8-connected neighbor), weights with 7 by 7 area to get sub-pixel location and response
    const int SRC_PLANE_OFFSET = SRCW * SRCH;
    // add 1 for saving total people count, 3 for x, y, score
    const int DST_PLANE_OFFSET = (POSE_MAX_PEOPLE + 1) * 3;
    float *dstptr = dst;
    int c = 0;
    int x = 0;
    int y = 0;
    // int i = 0;
    // int j = 0;
    // TODO: reduce multiplication by using pointer
    for (c = 0; c < SRC_CH - 1; ++c)
    {
        int num_people = 0;
        for (y = 1; y < SRCH - 1 && num_people != POSE_MAX_PEOPLE; ++y)
        {
            for (x = 1; x < SRCW - 1 && num_people != POSE_MAX_PEOPLE; ++x)
            {
                int idx = y * SRCW + x;
                float value = src[idx];
                if (value > TH)
                {
                    const float TOPLEFT = src[idx - SRCW - 1];
                    const float TOP = src[idx - SRCW];
                    const float TOPRIGHT = src[idx - SRCW + 1];
                    const float LEFT = src[idx - 1];
                    const float RIGHT = src[idx + 1];
                    const float BUTTOMLEFT = src[idx + SRCW - 1];
                    const float BUTTOM = src[idx + SRCW];
                    const float BUTTOMRIGHT = src[idx + SRCW + 1];
                    if (value >= TOPLEFT && value >= TOP && value >= TOPRIGHT && value >= LEFT &&
                        value > RIGHT && value > BUTTOMLEFT && value > BUTTOM && value > BUTTOMRIGHT)
                    {
                        float x_acc = 0;
                        float y_acc = 0;
                        float score_acc = 0;
                        int refine_w = 2;
                        for (int i = -refine_w; i <= refine_w; ++i)
                        {
                            int ux = x + i;
                            if (ux >= 0 && ux < SRCW)
                            {
                                for (int j = -refine_w; j <= refine_w; ++j)
                                {
                                    int uy = y + j;
                                    if (uy >= 0 && uy < SRCH)
                                    {
                                        float score = src[uy * SRCW + ux];
                                        if (score > 0.1)
                                        {
                                            x_acc += ux * score;
                                            y_acc += uy * score;
                                            score_acc += score;
                                        }
                                    }
                                }
                            }
                        }
                        x_acc /= score_acc;
                        y_acc /= score_acc;
                        score_acc = value;
                        dstptr[(num_people + 1) * 3 + 0] = x_acc;
                        dstptr[(num_people + 1) * 3 + 1] = y_acc;
                        dstptr[(num_people + 1) * 3 + 2] = score_acc;

                        ++num_people;
                    }
                }
            }
        }
        dstptr[0] = num_people;
        src += SRC_PLANE_OFFSET;
        dstptr += DST_PLANE_OFFSET;
    }
}


void connect_bodyparts(
        int POSE_MAX_PEOPLE,
        vector<float> &pose_keypoints,
        const float *const map,
        const float *const peaks,
        int mapw,
        int maph,
        const int inter_min_above_th, // each part min count, 10
        const float inter_th,         // each part min score
        const int min_subset_cnt,     // each person min keypoint count
        const float min_subset_score, // each person min score
        vector<int> &keypoint_shape)
{
    keypoint_shape.resize(3);
    const int body_part_pairs[] =
            {
                    1, 2, 1, 5, 2, 3, 3, 4, 5, 6, 6, 7, 1, 8, 8, 9, 9, 10, 1, 11, 11,
                    12, 12, 13, 1, 0, 0, 14, 14, 16, 0, 15, 15, 17, 2, 16, 5, 17};
    const int limb_idx[] =
            {
                    31, 32, 39, 40, 33, 34, 35, 36, 41, 42, 43, 44, 19, 20, 21, 22, 23, 24, 25,
                    26, 27, 28, 29, 30, 47, 48, 49, 50, 53, 54, 51, 52, 55, 56, 37, 38, 45, 46};

    const int num_body_parts = 18; // COCO part number
    const int num_body_part_pairs = num_body_parts + 1;
    std::vector<std::pair<std::vector<int>, double>> subset;
    const int subset_counter_index = num_body_parts;
    const int subset_size = num_body_parts + 1;
    const int peaks_offset = 3 * (POSE_MAX_PEOPLE + 1);
    const int map_offset = mapw * maph;

    for (unsigned int pair_index = 0u; pair_index < num_body_part_pairs; ++pair_index)
    {
        const int body_partA = body_part_pairs[2 * pair_index];
        const int body_partB = body_part_pairs[2 * pair_index + 1];
        const float *candidateA = peaks + body_partA * peaks_offset;
        const float *candidateB = peaks + body_partB * peaks_offset;
        const int nA = (int)(candidateA[0]); // number of part A candidates
        const int nB = (int)(candidateB[0]); // number of part B candidates

        // add parts into the subset in special case
        if (nA == 0 || nB == 0)
        {
            // Change w.r.t. other
            if (nA == 0) // nB == 0 or not
            {
                for (int i = 1; i <= nB; ++i)
                {
                    bool num = false;
                    for (unsigned int j = 0u; j < subset.size(); ++j)
                    {
                        const int off = body_partB * peaks_offset + i * 3 + 2;
                        if (subset[j].first[body_partB] == off)
                        {
                            num = true;
                            break;
                        }
                    }
                    if (!num)
                    {
                        std::vector<int> row_vector(subset_size, 0);
                        // store the index
                        row_vector[body_partB] = body_partB * peaks_offset + i * 3 + 2;
                        // the parts number of that person
                        row_vector[subset_counter_index] = 1;
                        // total score
                        const float subsetScore = candidateB[i * 3 + 2];
                        subset.emplace_back(std::make_pair(row_vector, subsetScore));
                    }
                }
            }
            else // if (nA != 0 && nB == 0)
            {
                for (int i = 1; i <= nA; i++)
                {
                    bool num = false;
                    for (unsigned int j = 0u; j < subset.size(); ++j)
                    {
                        const int off = body_partA * peaks_offset + i * 3 + 2;
                        if (subset[j].first[body_partA] == off)
                        {
                            num = true;
                            break;
                        }
                    }
                    if (!num)
                    {
                        std::vector<int> row_vector(subset_size, 0);
                        // store the index
                        row_vector[body_partA] = body_partA * peaks_offset + i * 3 + 2;
                        // parts number of that person
                        row_vector[subset_counter_index] = 1;
                        // total score
                        const float subsetScore = candidateA[i * 3 + 2];
                        subset.emplace_back(std::make_pair(row_vector, subsetScore));
                    }
                }
            }
        }
        else // if (nA != 0 && nB != 0)
        {
            std::vector<std::tuple<double, int, int>> temp;
            const int num_inter = 5;
            // limb PAF x-direction heatmap
            const float *const mapX = map + limb_idx[2 * pair_index] * map_offset;
            // limb PAF y-direction heatmap
            const float *const mapY = map + limb_idx[2 * pair_index + 1] * map_offset;
            // start greedy algorithm

            for (int i = 1; i <= nA; i++)
            {
                for (int j = 1; j <= nB; j++)
                {
                    const float dX = candidateB[j * 3] - candidateA[i * 3];
                    const float dY = candidateB[j * 3 + 1] - candidateA[i * 3 + 1];
                    const float norm_vec = float(std::sqrt(dX * dX + dY * dY));
                    // if (body_partA == 14 || body_partA == 15 || body_partB == 14 || body_partB == 15)
                    // {
                    //     cout << norm_vec << endl;
                    //     if (norm_vec > 3)
                    //     {
                    //         continue;
                    //     }
                    // }
                    // If the peaksPtr are coincident. Don't connect them.
                    if (norm_vec > 1e-6)
                    {
                        const float sX = candidateA[i * 3];
                        const float sY = candidateA[i * 3 + 1];
                        const float vecX = dX / norm_vec;
                        const float vecY = dY / norm_vec;
                        float sum = 0.;
                        int count = 0;
                        for (int lm = 0; lm < num_inter; lm++)
                        {
                            const int mX = fastMin(mapw - 1, intRound(sX + lm * dX / num_inter));
                            const int mY = fastMin(maph - 1, intRound(sY + lm * dY / num_inter));
                            const int idx = mY * mapw + mX;
                            const float score = (abs(vecX * mapX[idx]) + abs(vecY * mapY[idx]));
                            if (score > inter_th)
                            {
                                sum += score;
                                ++count;
                            }
                        }

                        // parts score + connection score
                        if (count > inter_min_above_th)
                        {
                            // temp.emplace_back(std::make_tuple(sum / count, i, j));
                            temp.emplace_back(std::make_tuple(sum / num_inter, i, j));
                        }
                    }
                }
            }
            // select the top minAB connection, assuming that each part occur only once
            // sort rows in descending order based on parts + connection score
            if (!temp.empty())
            {
                std::sort(temp.begin(), temp.end(), std::greater<std::tuple<float, int, int>>());
            }
            std::vector<std::tuple<int, int, double>> connectionK;

            const int minAB = fastMin(nA, nB);
            // assuming that each part occur only once, filter out same part1 to different part2
            std::vector<int> occurA(nA, 0);
            std::vector<int> occurB(nB, 0);
            int counter = 0;
            for (auto & row : temp)
            {
                const float score = std::get<0>(row);
                const int aidx = std::get<1>(row);
                const int bidx = std::get<2>(row);
                if (!occurA[aidx - 1] && !occurB[bidx - 1])
                {
                    // save two part score "position" and limb mean PAF score
                    connectionK.emplace_back(std::make_tuple(body_partA * peaks_offset + aidx * 3 + 2,
                                                             body_partB * peaks_offset + bidx * 3 + 2, score));
                    ++counter;
                    if (counter == minAB)
                    {
                        break;
                    }
                    occurA[aidx - 1] = 1;
                    occurB[bidx - 1] = 1;
                }
            }
            // Cluster all the body part candidates into subset based on the part connection
            // initialize first body part connection
            if (pair_index == 0)
            {
                for (const auto &connectionKI : connectionK)
                {
                    std::vector<int> row_vector(num_body_parts + 3, 0);
                    const int indexA = std::get<0>(connectionKI);
                    const int indexB = std::get<1>(connectionKI);
                    const double score = std::get<2>(connectionKI);
                    row_vector[body_part_pairs[0]] = indexA;
                    row_vector[body_part_pairs[1]] = indexB;
                    row_vector[subset_counter_index] = 2;
                    // add the score of parts and the connection
                    const double subset_score = peaks[indexA] + peaks[indexB] + score;
                    subset.emplace_back(std::make_pair(row_vector, subset_score));
                }
            }
                // Add ears connections (in case person is looking to opposite direction to camera)
            else if (pair_index == 17 || pair_index == 18)
            {
                for (const auto &connectionKI : connectionK)
                {
                    const int indexA = std::get<0>(connectionKI);
                    const int indexB = std::get<1>(connectionKI);
                    for (auto &subsetJ : subset)
                    {
                        auto &subsetJ_first = subsetJ.first[body_partA];
                        auto &subsetJ_first_plus1 = subsetJ.first[body_partB];
                        if (subsetJ_first == indexA && subsetJ_first_plus1 == 0)
                        {
                            subsetJ_first_plus1 = indexB;
                        }
                        else if (subsetJ_first_plus1 == indexB && subsetJ_first == 0)
                        {
                            subsetJ_first = indexA;
                        }
                    }
                }
            }
            else
            {
                if (!connectionK.empty())
                {
                    for (auto & i : connectionK)
                    {
                        const int indexA = std::get<0>(i);
                        const int indexB = std::get<1>(i);
                        const double score = std::get<2>(i);
                        int num = 0;
                        // if A is already in the subset, add B
                        for (auto & j : subset)
                        {
                            if (j.first[body_partA] == indexA)
                            {
                                j.first[body_partB] = indexB;
                                ++num;
                                j.first[subset_counter_index] = j.first[subset_counter_index] + 1;
                                j.second = j.second + peaks[indexB] + score;
                            }
                        }
                        // if A is not found in the subset, create new one and add both
                        if (num == 0)
                        {
                            std::vector<int> row_vector(subset_size, 0);
                            row_vector[body_partA] = indexA;
                            row_vector[body_partB] = indexB;
                            row_vector[subset_counter_index] = 2;
                            const float subsetScore = peaks[indexA] + peaks[indexB] + score;
                            subset.emplace_back(std::make_pair(row_vector, subsetScore));
                        }
                    }
                }
            }
        }
    }

    // Delete people below thresholds, and save to output
    int number_people = 0;
    std::vector<int> valid_subset_indexes;
    valid_subset_indexes.reserve(fastMin((size_t)POSE_MAX_PEOPLE, subset.size()));
    for (unsigned int index = 0; index < subset.size(); ++index)
    {
        const int subset_counter = subset[index].first[subset_counter_index];
        const double subset_score = subset[index].second;
        if (subset_counter >= min_subset_cnt && (subset_score / subset_counter) > min_subset_score)
        {
            ++number_people;
            valid_subset_indexes.emplace_back(index);
            if (number_people == POSE_MAX_PEOPLE)
            {
                break;
            }
        }
    }

    // Fill and return pose_keypoints
    keypoint_shape = {number_people, (int)num_body_parts, 3};
    if (number_people > 0)
    {
        pose_keypoints.resize(number_people * (int)num_body_parts * 3);
    }
    else
    {
        pose_keypoints.clear();
    }
    for (int person = 0; person < valid_subset_indexes.size(); ++person)
    {
        const auto &subsetI = subset[valid_subset_indexes[person]].first;
        // const float right_shoulder;
        for (int bodyPart = 0u; bodyPart < num_body_parts; bodyPart++)
        {
            const int base_offset = (person * num_body_parts + bodyPart) * 3;
            const int body_part_index = subsetI[bodyPart];
            if (body_part_index > 0)
            {
                pose_keypoints[base_offset] = peaks[body_part_index - 2];
                pose_keypoints[base_offset + 1] = peaks[body_part_index - 1];

                // pose_keypoints[base_offset] = (peaks[body_part_index - 2] + 0.5) * 8;
                // pose_keypoints[base_offset + 1] = (peaks[body_part_index - 1] + 0.5) * 8;
                pose_keypoints[base_offset + 2] = peaks[body_part_index];
            }
            else
            {
                pose_keypoints[base_offset] = 0.f;
                pose_keypoints[base_offset + 1] = 0.f;
                pose_keypoints[base_offset + 2] = 0.f;
            }
        }
    }
}


//input [0,255], output [0,255]
template <typename T1, typename T2>
AIR_RET air_resize_bilinear_0_255(
    T1 *src, int src_width, int src_height, int src_channel,
    T2 *dst, int dst_width, int dst_height, int dst_channel)
{
    AIR_RET ret = AIR_RET_OK;
    
    if (!src || !dst)
    {
        ret = AIR_RET_ERR_MEM;
        return ret;
    }
    if (src_channel != dst_channel)
    {
        ret = AIR_RET_ERR_PARA;
        return ret;
    }
    int src_stride = src_width * src_channel;
    int dst_stride = dst_width * dst_channel;
    
    int i, j;
    float fy = (float)(src_height - 1) / (dst_height - 1); //y系数
    float fx = (float)(src_width - 1) / (dst_width - 1);   //x系数
    
    air16s *x_idx_l = new air16s[dst_width];  //差值左侧值下标
    air16s *x_idx_r = new air16s[dst_width];  //差值右侧值下标
    float *x_frac_r = new float[dst_width]; //差值右侧值系数

    x_idx_l[0] = 0;
    x_idx_r[0] = 0;
    x_frac_r[0] = 0;
    x_idx_l[dst_width - 1] = src_width - 1;
    x_idx_r[dst_width - 1] = src_width - 1;
    x_frac_r[dst_width - 1] = 0;

    for (j = 1; j < dst_width - 1; j++)
    {
        float xcoord = j * fx;
        int idx_l = floor(xcoord);
        float frac_r = xcoord - idx_l;

        x_idx_l[j] = idx_l;
        x_idx_r[j] = (idx_l + 1) > (src_width - 1) ? (src_width - 1) : (idx_l + 1);
        x_frac_r[j] = frac_r;
    }

    for (i = 0; i < dst_height; i++)
    {
        float ycoord, frac_bot;
        T1 *src_line_1, *src_line_2;
        T2 *dst_line;
        int idx_top, idx_bot;

        if (i == 0)
        {
            idx_top = 0;
            idx_bot = 0;
            frac_bot = 0;
        }
        else if (i == dst_height - 1)
        {
            idx_top = src_height - 1;
            idx_bot = src_height - 1;
            frac_bot = 0;
        }
        else
        {
            ycoord = i * fy;
            idx_top = floor(ycoord); //差值上侧值下标
            idx_bot = (idx_top + 1 > src_height - 1) ? (src_height - 1) : idx_top + 1;
            frac_bot = ycoord - idx_top; //差值下侧值系数
        }

        dst_line = dst + i * dst_stride;    //输出图当前行
        src_line_1 = src + idx_top * src_stride; //输入图第一行
        src_line_2 = src + idx_bot * src_stride; //输入图第二行

        for (int j = 0; j < dst_width; j++)
        {

            float frac_r = x_frac_r[j]; //差值右侧值系数
            int idx1 = x_idx_l[j];
            int idx2 = x_idx_r[j];

            T1 x1 = src_line_1[idx1];  //左上角值指针
            T1 x2 = src_line_1[idx2];  //右上角值指针
            T1 x11 = src_line_2[idx1]; //左下角值指针
            T1 x22 = src_line_2[idx2]; //右下角值指针

            {
                float y1 = frac_r * (x2 - x1) + x1; //top
                float y2 = frac_r * (x22 - x11) + x11; //bot
                float val = frac_bot * (y2 - y1) + y1; //mid
                dst_line[j] = static_cast<T2>(val);
            }
        }
    }

    if (x_idx_l)
    {
        free(x_idx_l);
        x_idx_l = NULL;
    }
    if (x_idx_r)
    {
        free(x_idx_r);
        x_idx_r = NULL;
    }
    if (x_frac_r)
    {
        free(x_frac_r);
        x_frac_r = NULL;
    }
    
    
    return ret;
}
    
    
//input [0,255], output [0,1]
template <typename T1, typename T2>
AIR_RET air_resize_bilinear_1channel_0_1(T1 *src, int src_width, int src_height, int src_channel,
                                             T2 *dst, int dst_width, int dst_height, int dst_channel)
{
    
    AIR_RET ret = AIR_RET_OK;
    
    if (!src || !dst)
    {
        ret = AIR_RET_ERR_MEM;
        return ret;
    }
    if (src_channel != dst_channel)
    {
        ret = AIR_RET_ERR_PARA;
        return ret;
    }
    int src_stride = src_width;
    int dst_stride = dst_width;
    
    int i, j;
    float fy = (float)(src_height - 1) / (dst_height - 1); //y系数
    float fx = (float)(src_width - 1) / (dst_width - 1);   //x系数
    
    air16s *x_idx_l = new air16s[dst_width];  //差值左侧值下标
    air16s *x_idx_r = new air16s[dst_width];  //差值右侧值下标
    float *x_frac_r = new float[dst_width]; //差值右侧值系数
    
    x_idx_l[0] = 0;
    x_idx_r[0] = 0;
    x_frac_r[0] = 0;
    x_idx_l[dst_width - 1] = src_width - 1;
    x_idx_r[dst_width - 1] = src_width - 1;
    x_frac_r[dst_width - 1] = 0;
    
    for (j = 1; j < dst_width - 1; j++)
    {
        float xcoord = j * fx;
        int idx_l = floor(xcoord);
        float frac_r = xcoord - idx_l;
        
        x_idx_l[j] = idx_l;
        x_idx_r[j] = (idx_l + 1) > (src_width - 1) ? (src_width - 1) : (idx_l + 1);
        x_frac_r[j] = frac_r;
    }
    
    for (i = 0; i < dst_height; i++)
    {
        float ycoord, frac_bot;
        T1 *src_line_1, *src_line_2;
        T2 *dst_line;
        int idx_top, idx_bot;
        
        if (i == 0)
        {
            idx_top = 0;
            idx_bot = 0;
            frac_bot = 0;
        }
        else if (i == dst_height - 1)
        {
            idx_top = src_height - 1;
            idx_bot = src_height - 1;
            frac_bot = 0;
        }
        else
        {
            ycoord = i * fy;
            idx_top = floor(ycoord); //差值上侧值下标
            idx_bot = (idx_top + 1 > src_height - 1) ? (src_height - 1) : idx_top + 1;
            frac_bot = ycoord - idx_top; //差值下侧值系数
        }
        
        dst_line = dst + i * dst_stride;    //输出图当前行
        src_line_1 = src + idx_top * src_stride; //输入图第一行
        src_line_2 = src + idx_bot * src_stride; //输入图第二行
        
        for (int j = 0; j < dst_width; j++)
        {
            
            float frac_r = x_frac_r[j]; //差值右侧值系数
            int idx1 = x_idx_l[j];
            int idx2 = x_idx_r[j];
            
            T1 x1 = src_line_1[idx1];  //左上角值指针
            T1 x2 = src_line_1[idx2];  //右上角值指针
            T1 x11 = src_line_2[idx1]; //左下角值指针
            T1 x22 = src_line_2[idx2]; //右下角值指针
            
            {
                float y1 = frac_r * (x2 - x1) + x1; //top
                float y2 = frac_r * (x22 - x11) + x11; //bot
                float val = frac_bot * (y2 - y1) + y1; //mid
                dst_line[j] = static_cast<T2>(val / 255.f);
            }
        }
    }
    
    if (x_idx_l)
    {
        free(x_idx_l);
        x_idx_l = NULL;
    }
    if (x_idx_r)
    {
        free(x_idx_r);
        x_idx_r = NULL;
    }
    if (x_frac_r)
    {
        free(x_frac_r);
        x_frac_r = NULL;
    }
    
    return ret;
}

    
//input [0,255], output [0,255]
template <typename T1, typename T2>
AIR_RET air_resize_bilinear_1channel_0_255(T1 *src, int src_width, int src_height, int src_channel,
                                             T2 *dst, int dst_width, int dst_height, int dst_channel)
{

    AIR_RET ret = AIR_RET_OK;

    if (!src || !dst)
    {
        ret = AIR_RET_ERR_MEM;
        return ret;
    }
    if (src_channel != dst_channel)
    {
        ret = AIR_RET_ERR_PARA;
        return ret;
    }
    int src_stride = src_width;
    int dst_stride = dst_width;

    int i, j;
    float fy = (float)(src_height - 1) / (dst_height - 1); //y系数
    float fx = (float)(src_width - 1) / (dst_width - 1);   //x系数

    air16s *x_tab_l = new air16s[dst_width];  //差值左侧值下标
    air16s *x_tab_r = new air16s[dst_width];  //差值右侧值下标
    float *tab_frac = new float[dst_width]; //差值右侧值系数

    x_tab_l[0] = 0;
    x_tab_r[0] = 0;
    tab_frac[0] = 0;
    x_tab_l[dst_width - 1] = src_width - 1;
    x_tab_r[dst_width - 1] = src_width - 1;
    tab_frac[dst_width - 1] = 0;

    for (j = 1; j < dst_width - 1; j++)
    {
        int jm;
        float x_frac;
        x_frac = j * fx;
        jm = floor(x_frac);
        x_frac = x_frac - jm;

        x_tab_l[j] = jm;
        x_tab_r[j] = (jm + 1) > (src_width - 1) ? (src_width - 1) : (jm + 1);
        tab_frac[j] = x_frac;
    }

    for (i = 0; i < dst_height; i++)
    {
        float y_frac;
        T1 *src_line_1, *src_line_2;
        T2 *dst_line;
        int im, im_1;

        if (i == 0)
        {
            im = 0;
            im_1 = 0;
            y_frac = 0;
        }
        else if (i == dst_height - 1)
        {
            im = src_height - 1;
            im_1 = src_height - 1;
            y_frac = 0;
        }
        else
        {
            y_frac = i * fy;
            im = floor(y_frac); //差值上侧值下标
            im_1 = (im + 1 > src_height - 1) ? (src_height - 1) : im + 1;
            y_frac = y_frac - im; //差值下侧值系数
        }

        dst_line = dst + i * dst_stride;    //输出图当前行
        src_line_1 = src + im * src_stride; //输入图第一行
        src_line_2 = src + im_1 * src_stride; //输入图第二行

        for (int j = 0; j < dst_width; j++)
        {
            float x_frac = tab_frac[j]; //差值右侧值系数
            int idx1 = x_tab_l[j];
            int idx2 = x_tab_r[j];

            T1 x1 = src_line_1[idx1];  //左上角值指针
            T1 x2 = src_line_1[idx2];  //右上角值指针
            T1 x11 = src_line_2[idx1]; //左下角值指针
            T1 x22 = src_line_2[idx2]; //右下角值指针

            {
                float y1 = x_frac * (x2 - x1) + x1;
                float y2 = x_frac * (x22 - x11) + x11;
                float val = y_frac * (y2 - y1) + y1;
                dst_line[j] = static_cast<T2>(val);
            }
        }
    }

    if (x_tab_l)
    {
        free(x_tab_l);
        x_tab_l = NULL;
    }
    if (x_tab_r)
    {
        free(x_tab_r);
        x_tab_r = NULL;
    }
    if (tab_frac)
    {
        free(tab_frac);
        tab_frac = NULL;
    }

    return ret;
}
    
    
    

//template <typename T>
//void air_sigmoid_static_avx(T *input, int num, T frac, T thre)
//{
//    //x = 30（x-0.6)
//    //return = 1 / (1 + exp(-input));
//    T *p_input = input;
//    int size = num;
//    if (size > 0)
//    {
//        for (int k = 0; k < size; ++k)
//        {
//            p_input[k] = (T)1 / ((T)1 + exp(-p_input[k]));
//        }
//    }
//}
    
    
    
}//air

#endif /* pre_post_process_h */



#define SHIFT_YUV2RGB 20

#define COEF_RV   (1673527)
#define COEF_BU   (2116026)
#define COEF_GV   (-852492)
#define COEF_GU   (-409993)
#define COEF_RGBY (1220542)

#define NEON_LANES_UINT16 8

//static inline uint8_t saturate_cast_8U(int v) {
//    return (uint8_t) ((unsigned) v <= 255 ? v : v > 0 ? 255 : 0);
//} //{ return (uint8_t)(((unsigned int)(v)) <= 255 ? v : v > 0 ? 255 : 0); }
//
//#define CVT_R(__Y, __V) (saturate_cast_8U((std::max(0, (__Y)-16)*COEF_RGBY + ((__V)-128)*COEF_RV + (1<<(SHIFT_YUV2RGB-1)))>>SHIFT_YUV2RGB))
//#define CVT_G(__Y, __U, __V) (saturate_cast_8U((std::max(0, (__Y)-16)*COEF_RGBY + ((__U)-128)*(COEF_GU) + ((__V)-128)*(COEF_GV) + (1<<(SHIFT_YUV2RGB-1))) >> SHIFT_YUV2RGB))
//#define CVT_B(__Y, __U) (saturate_cast_8U((std::max(0, (__Y)-16)*COEF_RGBY + ((__U)-128)*COEF_BU + (1<<(SHIFT_YUV2RGB-1))) >> SHIFT_YUV2RGB))
//void CvtColorYUV420spToRGBPlanar(unsigned char *pucYUV420SPData, unsigned char *pucRGBData,
//                                 int iWidth, int iHeight)
//{
//    int w = iWidth;
//    int h = iHeight;
//    int vData, uData;
//    int leftW = iWidth % NEON_LANES_UINT16;
//    //int leftH = iHeight % NEON_LANES_UINT16;
//    unsigned char *pucUVData = pucYUV420SPData + w * h;
//    unsigned char *pucSrcYData1 = pucYUV420SPData;
//    unsigned char *pucSrcYData2 = pucYUV420SPData + w;
//    //unsigned char *pr1 = pucRGBData;
//    //unsigned char *pr2 = pucRGBData + 3 * w;
//
//    unsigned char *R1 = pucRGBData;
//    unsigned char *R2 = R1 + w;
//    unsigned char *G1 = R1 + w * h;
//    unsigned char *G2 = G1 + w;
//    unsigned char *B1 = G1 + w * h;
//    unsigned char *B2 = B1 + w;
//
//    int16x8_t cmp16 = vdupq_n_s16(16);
//    int8x8_t const128 = vdup_n_u8(128);
//    int32x4_t constBU = vdupq_n_s32(COEF_BU);
//    int32x4_t constGU = vdupq_n_s32(COEF_GU);
//    int32x4_t constGV = vdupq_n_s32(COEF_GV);
//    int32x4_t constRV = vdupq_n_s32(COEF_RV);
//    int32x4_t constShift = vdupq_n_s32(1 << (SHIFT_YUV2RGB - 1));
//
//    for (int i = 0; i < h; i += 2)
//    {
//        for (int j = 0; j <= w - NEON_LANES_UINT16; j += NEON_LANES_UINT16)
//        {
//            uint8x8x2_t VU = vld2_u8(pucUVData);
//            uint8x8x2_t v = vzip_u8(VU.val[0], VU.val[0]);
//            uint8x8x2_t u = vzip_u8(VU.val[1], VU.val[1]);
//            int16x8x4_t vu_;
//            vu_.val[0] = vmovl_s8(vsub_s8(v.val[0], const128));
//            vu_.val[1] = vmovl_s8(vsub_s8(v.val[1], const128));
//            vu_.val[2] = vmovl_s8(vsub_s8(u.val[0], const128));
//            vu_.val[3] = vmovl_s8(vsub_s8(u.val[1], const128));
//
//            uint8x8_t y1 = vld1_u8(pucSrcYData1);
//            int16x8_t y1_ = vmaxq_s16(vsubq_s16(vreinterpretq_s16_u16((vmovl_u8(y1))), cmp16),
//                                      vdupq_n_s16(0));
//
//            uint8x8x3_t rgb1;
//            int32x4_t y1Hight = vmulq_n_s32(vmovl_s16(vget_low_s16(y1_)), COEF_RGBY);
//            int32x4_t y1Low = vmulq_n_s32(vmovl_s16(vget_high_s16(y1_)), COEF_RGBY);
//            //y1
//            //R
//            int32x4_t rvHight = vmulq_s32(constRV, vmovl_s16(vget_low_s16(vu_.val[0])));
//            int32x4_t rvLow = vmulq_s32(constRV, vmovl_s16(vget_high_s16(vu_.val[0])));
//            int16x4_t tempRHigh1 = vshr_n_s16(
//                                              vshrn_n_s32(vaddq_s32(vaddq_s32(y1Hight, rvHight), constShift), 16), 4);
//            int16x4_t tempRLow1 = vshr_n_s16(
//                                             vshrn_n_s32(vaddq_s32(vaddq_s32(y1Low, rvLow), constShift), 16), 4);
//            rgb1.val[0] = vmovn_u16(vreinterpretq_u16_s16(vminq_s16(vmaxq_s16(vcombine_s16(tempRHigh1, tempRLow1), vdupq_n_s16(0)), vdupq_n_s16(255))));
//
//            //G
//            int32x4_t guHight = vmulq_s32(constGU, vmovl_s16(vget_low_s16(vu_.val[2])));
//            int32x4_t guLow = vmulq_s32(constGU, vmovl_s16(vget_high_s16(vu_.val[2])));
//            int32x4_t gvHight = vmulq_s32(constGV, vmovl_s16(vget_low_s16(vu_.val[0])));
//            int32x4_t gvLow = vmulq_s32(constGV, vmovl_s16(vget_high_s16(vu_.val[0])));
//            int16x4_t tempGHigh1 = vshr_n_s16(vshrn_n_s32(vaddq_s32(vaddq_s32(vaddq_s32(y1Hight, guHight), gvHight), constShift), 16), 4);
//            int16x4_t tempGLow1 = vshr_n_s16(vshrn_n_s32(vaddq_s32(vaddq_s32(vaddq_s32(y1Low, guLow), gvLow), constShift), 16), 4);
//            rgb1.val[1] = vmovn_u16(vreinterpretq_u16_s16(vminq_s16(vmaxq_s16(vcombine_s16(tempGHigh1, tempGLow1), vdupq_n_s16(0)), vdupq_n_s16(255))));
//
//            //B
//            int32x4_t buHight = vmulq_s32(constBU, vmovl_s16(vget_low_s16(vu_.val[2])));
//            int32x4_t buLow = vmulq_s32(constBU, vmovl_s16(vget_high_s16(vu_.val[2])));
//            int16x4_t tempBHigh1 = vshr_n_s16(vshrn_n_s32(vaddq_s32(vaddq_s32(y1Hight, buHight), constShift), 16), 4);
//            int16x4_t tempBLow1 = vshr_n_s16(vshrn_n_s32(vaddq_s32(vaddq_s32(y1Low, buLow), constShift), 16), 4);
//            rgb1.val[2] = vmovn_u16(vreinterpretq_u16_s16(vminq_s16(vmaxq_s16(vcombine_s16(tempBHigh1, tempBLow1), vdupq_n_s16(0)), vdupq_n_s16(255))));
//            //vst3_u8(pr1, rgb1);
//            vst1_u8(R1, rgb1.val[0]);
//            vst1_u8(G1, rgb1.val[1]);
//            vst1_u8(B1, rgb1.val[2]);
//
//            uint8x8_t y2 = vld1_u8(pucSrcYData2);
//            int16x8_t y2_ = vminq_s16(vmaxq_s16(vsubq_s16(vreinterpretq_s16_u16((vmovl_u8(y2))), cmp16),
//                                                vdupq_n_s16(0)), vdupq_n_s16(255));
//            int32x4_t y2Hight = vmulq_n_s32(vmovl_s16(vget_low_s16(y2_)), COEF_RGBY);
//            int32x4_t y2Low = vmulq_n_s32(vmovl_s16(vget_high_s16(y2_)), COEF_RGBY);
//            uint8x8x3_t rgb2;
//            //y2
//            //R
//            int16x4_t tempRHigh2 = vshr_n_s16(vshrn_n_s32(vaddq_s32(vaddq_s32(y2Hight, rvHight), constShift), 16), 4);
//            int16x4_t tempRLow2 = vshr_n_s16(vshrn_n_s32(vaddq_s32(vaddq_s32(y2Low, rvLow), constShift), 16), 4);
//            rgb2.val[0] = vmovn_u16(vreinterpretq_u16_s16(vminq_s16(vmaxq_s16(vcombine_s16(tempRHigh2, tempRLow2), vdupq_n_s16(0)),
//                                                                    vdupq_n_s16(255))));
//
//            //G
//            int16x4_t tempGHigh2 = vshr_n_s16(vshrn_n_s32(vaddq_s32(vaddq_s32(vaddq_s32(y2Hight, guHight), gvHight), constShift), 16), 4);
//            int16x4_t tempGLow2 = vshr_n_s16(vshrn_n_s32(vaddq_s32(vaddq_s32(vaddq_s32(y2Low, guLow), gvLow), constShift),
//                                                         16), 4);
//            rgb2.val[1] = vmovn_u16(vreinterpretq_u16_s16(vminq_s16(vmaxq_s16(vcombine_s16(tempGHigh2, tempGLow2), vdupq_n_s16(0)),
//                                                                    vdupq_n_s16(255))));
//
//            //B
//            int16x4_t tempBHigh2 = vshr_n_s16(vshrn_n_s32(vaddq_s32(vaddq_s32(y2Hight, buHight), constShift), 16), 4);
//            int16x4_t tempBLow2 = vshr_n_s16(vshrn_n_s32(vaddq_s32(vaddq_s32(y2Low, buLow), constShift), 16), 4);
//            rgb2.val[2] = vmovn_u16(vreinterpretq_u16_s16(vminq_s16(vmaxq_s16(vcombine_s16(tempBHigh2, tempBLow2), vdupq_n_s16(0)),
//                                                                    vdupq_n_s16(255))));
//            //vst3_u8(pr2, rgb2);
//            vst1_u8(R2, rgb2.val[0]);
//            vst1_u8(G2, rgb2.val[1]);
//            vst1_u8(B2, rgb2.val[2]);
//
//            //pr1 += 3 * NEON_LANES_UINT16;
//            //pr2 += 3 * NEON_LANES_UINT16;
//            R1 += NEON_LANES_UINT16;
//            R2 += NEON_LANES_UINT16;
//            G1 += NEON_LANES_UINT16;
//            G2 += NEON_LANES_UINT16;
//            B1 += NEON_LANES_UINT16;
//            B2 += NEON_LANES_UINT16;
//            pucSrcYData1 += NEON_LANES_UINT16;
//            pucSrcYData2 += NEON_LANES_UINT16;
//
//            pucUVData += NEON_LANES_UINT16;
//        }
//
//        for (int k = 0; k < leftW; k += 2)
//        {
//            vData = *pucUVData++;
//            uData = *pucUVData++;
//
//            //up left
//            *R1++ = CVT_R(*pucSrcYData1, vData);
//            *G1++ = CVT_G(*pucSrcYData1, uData, vData);
//            *B1++ = CVT_B(*pucSrcYData1, uData);
//            pucSrcYData1++;
//
//            //up-right
//            *R1++ = CVT_R(*pucSrcYData1, vData);
//            *G1++ = CVT_G(*pucSrcYData1, uData, vData);
//            *B1++ = CVT_B(*pucSrcYData1, uData);
//            pucSrcYData1++;
//
//            //down left
//            *R2++ = CVT_R(*pucSrcYData2, vData);
//            *G2++ = CVT_G(*pucSrcYData2, uData, vData);
//            *B2++ = CVT_B(*pucSrcYData2, uData);
//            pucSrcYData2++;
//
//            //down-right
//            *R2++ = CVT_R(*pucSrcYData2, vData);
//            *G2++ = CVT_G(*pucSrcYData2, uData, vData);
//            *B2++ = CVT_B(*pucSrcYData2, uData);
//            pucSrcYData2++;
//        }
//
//        //pr1 += 3 * w;
//        //pr2 += 3 * w;
//        R1 += w;
//        R2 += w;
//        G1 += w;
//        G2 += w;
//        B1 += w;
//        B2 += w;
//        pucSrcYData1 += w;
//        pucSrcYData2 += w;
//    }
//}
