#ifndef _AIR_IMAGE_GAUSSION_FILTER_HPP_
#define _AIR_IMAGE_GAUSSION_FILTER_HPP_

#include <math.h>
//#include <pthread.h>
#include "common.h"

#if defined(_MSC_VER)
#include <io.h>
#include <direct.h>

#elif defined(__GNUC__)
#include <sys/uio.h>
#include <sys/types.h>
#include <sys/timeb.h>
#include <sys/time.h>
#include <unistd.h>
#include <dirent.h>
#include <pthread.h>
#endif

using namespace std;
namespace air
{

#define AIR_NEON_OPTED
    
///////
#ifdef __GNUC__
#    define GCC_VERSION_AT_LEAST(x,y) (__GNUC__ > (x) || __GNUC__ == (x) && __GNUC_MINOR__ >= (y))
#else
#    define GCC_VERSION_AT_LEAST(x,y) 0
#endif

#if GCC_VERSION_AT_LEAST(3,1)
#    define air_attribute_deprecated __attribute__((deprecated))
#elif defined(_MSC_VER)
#    define air_attribute_deprecated __declspec(deprecated)
#else
#    define air_attribute_deprecated
#endif

//#if defined(__x86_64__) || defined(WIN64) || defined(__aarch64__) || defined(__APPLE__)
//typedef air64s airsize;
//#define AIR_SSE_OPTED
//#define AIR_AVX_OPTED
//#define AIR_STRIDE_ALIGN  32
//
//#elif defined(__i386__) || defined(WIN32) || !defined(__aarch64__)
//typedef air32s airsize;
//#define AIR_SSE_OPTED
//#define AIR_STRIDE_ALIGN  16
//
//#else

typedef air32s airsize;
#define AIR_STRIDE_ALIGN  32

//#endif

#define AIR_USING_LIBJPEG
#define AIR_USING_LIBPNG

#if defined(__arm) || defined(__aarch64__)
#undef AIR_USING_LIBJPEG
#undef AIR_USING_LIBPNG
#endif

#define  AIR_STRIDE_CONST    (AIR_STRIDE_ALIGN - 1)
#define  AIR_PTR_ALIGNED(ptr)    (void* )(((size_t)(ptr) + AIR_STRIDE_CONST) & (-AIR_STRIDE_ALIGN))
#define  AIR_SIZE_ALIGNED(size) ((size + AIR_STRIDE_CONST) & -(AIR_STRIDE_ALIGN))


#define AIR_CLAMP(x, l, u) ((x) < (l) ? (l) : ((x) > (u) ? (u) : (x)))
#define AIR_CLAMP0255(x)  AIR_CLAMP(x, 0, 255)

#define AIR_MIN(i,j)   (((i) > (j)) ? (j) : (i))
#define AIR_MAX(i,j)   (((i) > (j)) ? (i) : (j))
#define AIR_SIGN(x) (((x) < 0) ? (-1) : ((x) > 0 ? (1) : (0)))

#define  AIR_MIN_3_NUM(a,b,c) AIR_MIN(AIR_MIN(a,b),c)
#define  AIR_MAX_3_NUM(a,b,c) AIR_MAX(AIR_MAX(a,b),c)

#define  AIR_INT_MIN(a, b)  ((a)^(((a)^(b)) & (((a) < (b)) - 1)))
#define  AIR_INT_MAX(a, b)  ((a)^(((a)^(b)) & (((a) > (b)) - 1)))


#define air_memset  memset
#define air_memcpy  memcpy


#define AIR_MAX_INT32    ((air32s)0x7FFFFFFF)
#define AIR_MIN_INT32   ((air32s)0x80000000)

#define AIR_MAX_UINT32    ((air32u)0xFFFFFFFFUL)

#define AIR_MAX_INT16    ((air16s)0x7FFF)
#define AIR_MIN_INT16   ((air16s)0x8000)

#define AIR_MAX_UINT16    ((air16u)0xFFFFUL)



#if defined(__GNUC__)
#define AIR_INLINE  inline
#define AIR_ALIGNED(x) __attribute__ ((aligned (x)))

#if defined(AIR_SSE_OPTED)
#include <x86intrin.h>
#include <immintrin.h>
#endif

// #define AIR_OMP_OPTED

#elif defined(_MSC_VER)
#define AIR_INLINE __inline
#define inline __inline
#define AIR_ALIGNED(x) __declspec(align(x))

#if defined(AIR_SSE_OPTED)
#include <intrin.h>
#include <immintrin.h>
#endif

// #define AIR_OMP_OPTED

#endif


#define  AIR_DALIGNED  AIR_ALIGNED(AIR_STRIDE_ALIGN)


#if defined(AIR_NEON_OPTED)
#include <arm_neon.h>
#endif


#if defined(_MSC_VER)
#define snprintf _snprintf
#endif

#if defined(AIR_OMP_OPTED)
#include <omp.h>
#endif
///////////

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

#undef LOOP_Y_NUM
#define LOOP_Y_NUM (3)
#define AIR_MAX_GAUSS_COEF_WIDTH (256)

    typedef struct
    {
    airf32 sigma;
    air32s size;  // if size == -1, the value of size will be determined by sigma
    } air_gaussion_blur_params;
    
    static AIR_RET air_compute_gaussion_filter_coef(airf32 *gauss_coef,
            int *gauss_coef_width,
            airf32 sigma,
            int real_type_flag,
            int size);

    static void air_coln_filter_uint8(air8u **src,
                                      air8u *dst,
                                      int dststep,
                                      int count,
                                      int width,
                                      airf32 *coef,
                                      int coef_num,
                                      air16u *coef_q8);

    static void air_row_filter_uint8(air8u *_dst,
                                     air8u *_src,
                                     int width,
                                     int cn,
                                     air16u *kernel_data,
                                     int kernel_size);


    static void air_row_filter_real32(
        air8u *dst, air8u *src,
        int width, int cn,
        airf32 *coef, int coef_num);

    static void air_coln_filter_real32(
        air8u **src, air8u *dst,
        int dststep, int count, int width,
        airf32 *coef, int coef_num);


    static int border_interpolate(int p, int len);


    void *air_malloc(airsize size)
    {
        air8u *udata, *udata_offset;
        air8u **adata;
        if (size <= 0) return NULL;
        udata = (air8u *)malloc(size + sizeof(void *) + AIR_STRIDE_ALIGN + sizeof(airsize));
        if (udata == NULL) return NULL;
        udata_offset = udata + sizeof(void *) + sizeof(airsize);
        adata = (air8u **)AIR_PTR_ALIGNED(udata_offset);
        ((airsize *)adata)[-1] = size;
        adata[-2] = udata;
        return (void *)adata;
    }

    void *air_calloc(airsize num, airsize size)
    {
        void *ptr = air_malloc(num * size);
        if (ptr)
        {
            memset(ptr, 0, num * size);
        }
        return ptr;
    }

    void air_free(void *p)
    {
        if (p)
        {
            air8u *udata = ((air8u **)p)[-2];
            if (udata)
            {
                free(udata);
            }
        }
    }

    void *air_realloc(void *p, airsize size)
    {
        airsize *ptr_t;
        airsize old_size;
        if (p == NULL) return air_malloc(size);

        ptr_t = (airsize *)p;
        old_size = ptr_t[-1];
        if (old_size < size)
        {
            void *ptr_t2 = air_malloc(size);
            if (ptr_t2 == NULL) return NULL;

            memcpy(ptr_t2, p, old_size);
            air_free(p);

            return ptr_t2;
        }
        else
        {
            return p;
        }
    }

    AIR_RET air_image_gaussian_blur_raw_data(
        air8u *dst,
        int dst_width,
        int dst_height,
        int dst_stride,
        int dst_chnnel,
        air8u *src,
        int src_width,
        int src_height,
        int src_stride,
        int src_channel,
        airf32 sigma,
        int real_data_type_flag,
        int size)
    {
#if defined(TIME_DEBUG)
        airf64 timer = air_get_current_system_time();
#endif

        AIR_RET ret = AIR_RET_OK;
        int max_buffer_rows, max_width, coef_num;
        air8u *rows[AIR_MAX_GAUSS_COEF_WIDTH + LOOP_Y_NUM];
        air8u *src_row = NULL;
        int buffer_step;
        air8u *ring_buffer = NULL;
        int dx1, dx2, i, j, width, height, stride, channels;
        int buffer_element_size, src_element_size;
        int border_tab[AIR_MAX_GAUSS_COEF_WIDTH * LOOP_Y_NUM];
        int *btab = (int *)&border_tab[0];
        air8u *ptr_buffer;
        int total_buffer_size;
        airf32 AIR_DALIGNED coef[AIR_MAX_GAUSS_COEF_WIDTH];
        air16u AIR_DALIGNED coef_q8[AIR_MAX_GAUSS_COEF_WIDTH];
        if (dst == NULL || src == NULL || dst_width != src_width || dst_chnnel != src_channel || dst_height != src_height || dst_stride != src_stride || sigma <= 0.f)
        {

            return AIR_RET_ERR_PARA;
        }

        width = src_width;
        height = src_height;
        stride = src_stride;
        channels = src_channel;
        src_element_size = channels;

        ret = air_compute_gaussion_filter_coef(coef, &coef_num, sigma, real_data_type_flag, size);
        if (ret != AIR_RET_OK)
        {
            return ret;
        }

        dx1 = dx2 = coef_num >> 1;

        if (real_data_type_flag)
        {
            src_element_size = src_element_size * 4;
        }

        max_width = width + coef_num - 1;
        total_buffer_size = max_width * src_element_size;

        buffer_step = src_element_size * AIR_SIZE_ALIGNED(width);

        if (real_data_type_flag == 0)
        {
            airf64 recale = 1.0 / (1 << 16);
            buffer_element_size = channels;
            buffer_step = buffer_element_size * AIR_SIZE_ALIGNED(width);
            for (i = 0; i < coef_num; i++)
            {
                coef_q8[i] = (int)(coef[i] * 256.f + 0.5f);
                coef[i] = (airf32)(coef_q8[i] * recale);
            }
        }

        max_buffer_rows = coef_num + LOOP_Y_NUM;
        total_buffer_size += buffer_step * max_buffer_rows + 2 * AIR_STRIDE_ALIGN;
        ptr_buffer = (air8u *)air_malloc(total_buffer_size);
        if (ptr_buffer == NULL)
        {
            return AIR_RET_ERR_MEM;
        }

        src_row = ptr_buffer;

        ring_buffer = ptr_buffer + max_width * src_element_size;
        ring_buffer = (air8u *)AIR_PTR_ALIGNED(ring_buffer);

        {
            for (i = 0; i < dx1; i++)
            {
                int p0 = (border_interpolate(i - dx1, width)) * channels;
                for (j = 0; j < channels; j++)
                {
                    btab[i * channels + j] = p0 + j;
                }
            }

            for (i = 0; i < dx2; i++)
            {
                int p0 = (border_interpolate(width + i, width)) * channels;
                for (j = 0; j < channels; j++)
                {
                    btab[(i + dx1) * channels + j] = p0 + j;
                }
            }
        }

        {

            air8u **brows = &rows[0];
            int bufRows = max_buffer_rows;
            int kwidth = coef_num;
            int width1 = width + kwidth - 1;
            int dy = 0;
            int count = height;
            int startY0 = 0, startY = 0;
            int ay = coef_num >> 1;

            int rowCount = 0, dstY = 0;
            int bi;

            i = 0;

            for (;; dst += stride * i, dy += i)
            {
                int dcount = bufRows - ay - startY - rowCount;
                dcount = dcount > 0 ? dcount : bufRows - coef_num + 1;
                dcount = AIR_MIN(dcount, count);
                count -= dcount;
                for (; dcount-- > 0; src += stride)
                {
                    air8u *brow, *row;
                    bi = (startY - startY0 + rowCount) % bufRows;
                    brow = (air8u *)AIR_PTR_ALIGNED(&ring_buffer[0]) + bi * buffer_step;
                    row = src_row;

                    if (++rowCount > bufRows)
                    {
                        --rowCount;
                        ++startY;
                    }

                    memcpy(row + dx1 * src_element_size, src, (width1 - dx2 - dx1) * src_element_size);

                    if (channels * (int)sizeof(int) == src_element_size)
                    {
                        int *isrc = (int *)src;
                        int *irow = (int *)row;
                        for (i = 0; i < dx1 * channels; i++)
                        {
                            irow[i] = isrc[btab[i]];
                        }
                        for (i = 0; i < dx2 * channels; i++)
                        {
                            irow[i + (width1 - dx2) * channels] = isrc[btab[i + dx1 * channels]];
                        }
                    }
                    else
                    {
                        for (i = 0; i < dx1 * src_element_size; i++)
                        {
                            row[i] = src[btab[i]];
                        }
                        for (i = 0; i < dx2 * src_element_size; i++)
                        {
                            row[i + (width1 - dx2) * src_element_size] = src[btab[i + dx1 * src_element_size]];
                        }
                    }

                    if (real_data_type_flag)
                    {
                        air_row_filter_real32(brow, row, width, channels, coef, coef_num);
                    }
                    else
                    {
                        //here
                        air_row_filter_uint8(brow, row, width, channels, coef_q8, coef_num);
                    }
                }

                {
                    int max_i = AIR_MIN(bufRows, height - (dstY + dy) + (coef_num - 1));
                    for (i = 0; i < max_i; i++)
                    {
                        int srcY = border_interpolate(dstY + dy + i - ay, height);

                        if (srcY >= startY + rowCount)
                            break;
                        bi = (srcY - startY0) % bufRows;
                        brows[i] = (air8u *)AIR_PTR_ALIGNED(&ring_buffer[0]) + bi * buffer_step;
                    }
                    if (i < coef_num)
                        break;
                    i -= coef_num - 1;

                    if (real_data_type_flag)
                    {
                        air_coln_filter_real32(brows, dst, stride, i, width * channels, coef, coef_num);
                    }
                    else
                    {
                        //here
                        air_coln_filter_uint8(brows, dst, stride, i, width * channels, coef, coef_num, coef_q8);
                    }
                }
            }
        }

        air_free(ptr_buffer);

#if defined(TIME_DEBUG)
        timer = air_get_current_system_time() - timer;
        printf("gaussian_blur time_cost: %lf\n", timer);
        t_gauss_blur += timer;
#endif

        return ret;
    }

    static void get_gaussian_kernel(airf64 *kernel, int n, airf64 sigma)
    {
#define SMALL_GAUSSIAN_SIZE (7)
        static const airf32 small_gaussian_tab[][SMALL_GAUSSIAN_SIZE] =
        {
            { 1.f },
            { 0.25f, 0.5f, 0.25f },
            { 0.0625f, 0.25f, 0.375f, 0.25f, 0.0625f },
            { 0.03125f, 0.109375f, 0.21875f, 0.28125f, 0.21875f, 0.109375f, 0.03125f }
        };

        const airf32 *fixed_kernel = n % 2 == 1 && n <= SMALL_GAUSSIAN_SIZE && sigma <= 0 ?
                                     small_gaussian_tab[n >> 1] : 0;

        airf64 *cd = kernel;

        airf64 sigmaX = sigma > 0 ? sigma : ((n - 1) * 0.5 - 1) * 0.3 + 0.8;
        airf64 scale2X = -0.5 / (sigmaX * sigmaX);
        airf64 sum = 0;
        int i;
        for (i = 0; i < n; i++)
        {
            airf64 x = i - (n - 1) * 0.5;
            airf64 t = fixed_kernel ? (airf64)fixed_kernel[i] : expl(scale2X * x * x);
            cd[i] = t;
            sum += cd[i];
        }

        sum = 1. / sum;
        for (i = 0; i < n; i++)
        {
            cd[i] *= sum;
        }
    }


    static AIR_RET air_compute_gaussion_filter_coef(
        airf32 *gauss_coef, int *gauss_coef_width,
        airf32 sigma, int real_type_flag, int size)
    {
        AIR_RET ret = AIR_RET_OK;
        int i, gauss_width, n, t;
        airf64 g_f_c[AIR_MAX_GAUSS_COEF_WIDTH];
        //airf64 acc = 0;

        if (real_type_flag)
        {
            n = 4;
        }
        else
        {
            n = 3;
        }


        t = (int)ceilf(n * sigma);
        if (size == -2)
        {
            t /= 2;
        }
        gauss_width = AIR_MAX(t, 1);
        if ((2 * gauss_width + 1) > AIR_MAX_GAUSS_COEF_WIDTH)
        {
            return AIR_RET_ERR_NOT_SUPPORT;
        }

        if (size <= 0)
        {
            *gauss_coef_width = 2 * gauss_width + 1;
        }
        else
        {
            if (size & 1)
            {
                *gauss_coef_width = size;
            }
            else
            {
                //*gauss_coef_width = 2 * size + 1;
                *gauss_coef_width = size / 2 * 2 + 1;
            }
        }

        get_gaussian_kernel(g_f_c, *gauss_coef_width, sigma);

        for (i = 0; i < *gauss_coef_width; i++)
        {
            gauss_coef[i] = (airf32)g_f_c[i];
        }

        return ret;
    }



    static void air_row_filter_uint8(air8u *_dst,
                                     air8u *_src,
                                     int width,
                                     int cn,
                                     air16u *kernel_data,
                                     int kernel_size)
    {
        int i = 0, k, _ksize = kernel_size;
        air8u *dst = _dst;
        air16u *_kx = kernel_data;
        width *= cn;
#if defined(AIR_SSE_OPTED)
        __m128i c_128 = _mm_set1_epi16(128);
        for (; i <= width - 16; i += 16)
        {
            air8u *src = _src + i;
            __m128i f, z = _mm_setzero_si128(), s0 = z, s2 = z;
            __m128i x0, x2;
            for (k = 0; k < _ksize; k++, src += cn)
            {
                f = _mm_set1_epi16(_kx[k]);
                x0 = _mm_loadu_si128((const __m128i *)src);
                x2 = _mm_unpackhi_epi8(x0, z);
                x0 = _mm_unpacklo_epi8(x0, z);
                x0 = _mm_mullo_epi16(x0, f);
                x2 = _mm_mullo_epi16(x2, f);
                s0 = _mm_adds_epu16(s0, x0);
                s2 = _mm_adds_epu16(s2, x2);
            }

            s0 = _mm_adds_epu16(s0, c_128);
            s2 = _mm_adds_epu16(s2, c_128);
            s0 = _mm_srli_epi16(s0, 8);
            s2 = _mm_srli_epi16(s2, 8);
            s0 = _mm_packus_epi16(s0, s2);
            _mm_store_si128((__m128i *)(dst + i), s0);
        }

        for (; i <= width - 4; i += 4)
        {
            air8u *src = _src + i;
            __m128i f, z = _mm_setzero_si128(), s0 = z, x0;
            for (k = 0; k < _ksize; k++, src += cn)
            {
                f = _mm_set1_epi16(_kx[k]);
                x0 = _mm_cvtsi32_si128(*(const int *)src);
                x0 = _mm_unpacklo_epi8(x0, z);
                x0 = _mm_mullo_epi16(x0, f);
                s0 = _mm_adds_epu16(s0, x0);
            }
            s0 = _mm_adds_epu16(s0, c_128);
            s0 = _mm_srli_epi16(s0, 8);
            s0 = _mm_packus_epi16(s0, s0);
            *((int *)&dst[i]) = _mm_cvtsi128_si32(s0);
        }
#endif
#if defined(AIR_NEON_OPTED)
        for (; i <= width - 16; i += 16)
        {
            air8u *src = _src + i;
            uint16x8_t s0 = vdupq_n_u16(0);
            uint16x8_t s2 = s0;
            for (k = 0; k < _ksize; k++, src += cn)
            {
                uint8x16_t f = vdupq_n_u8(_kx[k]);
                uint8x16_t x0 = vld1q_u8(src);
                s0 = vmlal_u8(s0, vget_low_u8(x0), vget_low_u8(f));
                s2 = vmlal_u8(s2, vget_high_u8(x0), vget_high_u8(f));
            }
            int16x8_t s0_i16 = vreinterpretq_s16_u16(vrshrq_n_u16(s0, 8));
            int16x8_t s2_i16 = vreinterpretq_s16_u16(vrshrq_n_u16(s2, 8));
            uint8x16_t d = vcombine_u8(vqmovun_s16(s0_i16), vqmovun_s16(s2_i16));
            vst1q_u8(&dst[i], d);
        }

        for (; i <= width - 4; i += 4)
        {
            air8u *src = _src + i;
            uint16x8_t s0 = vdupq_n_u16(0);
            for (k = 0; k < _ksize; k++, src += cn)
            {
                uint32x2_t x0;
                x0 = vld1_lane_u32((const air32u *)src, x0, 0);
                uint8x16_t f = vdupq_n_u8(_kx[k]);
                s0 = vmlal_u8(s0, vreinterpret_u8_u32(x0), vget_low_u8(f));
            }
            s0 = vrshrq_n_u16(s0, 8);

            uint32x2_t dd = vreinterpret_u32_u8(vqmovun_s16(vreinterpretq_s16_u16(s0)));

            vst1_lane_u32((air32u *)&dst[i], dd, 0);
        }
#endif

        for (; i < width; i++)
        {
            air8u *S = _src + i;
            int s0 = _kx[0] * S[0];
            for (k = 1; k < _ksize; k++)
            {
                S += cn;
                s0 += _kx[k] * S[0];
            }
            dst[i] = (s0 + 128) >> 8;
        }
    }


    static void air_row_filter_real32(
        air8u *dst, air8u *src,
        int width, int cn,
        airf32 *coef, int coef_num)
    {
        int _ksize = coef_num;
        airf32 *src0 = (airf32 *)src;
        airf32 *_dst = (airf32 *)dst;
        airf32 *_kx = (airf32 *)coef;
        airf32 *S;

        int i = 0, k;

        width *= cn;
#if defined(AIR_SSE_OPTED)
        for (; i <= width - 8; i += 8)
        {
            const airf32 *src = src0 + i;
            __m128 f, s0 = _mm_setzero_ps(), s1 = s0, x0, x1;
            for (k = 0; k < _ksize; k++, src += cn)
            {
                f = _mm_load_ss(_kx + k);
                f = _mm_shuffle_ps(f, f, 0);

                x0 = _mm_loadu_ps(src);
                x1 = _mm_loadu_ps(src + 4);
                s0 = _mm_add_ps(s0, _mm_mul_ps(x0, f));
                s1 = _mm_add_ps(s1, _mm_mul_ps(x1, f));
            }
            _mm_store_ps(_dst + i, s0);
            _mm_store_ps(_dst + i + 4, s1);
        }
#endif
        {
            airf32 s0, s1, s2, s3;
            airf32 f;
            airf32 *D = _dst;
            for (; i <= width - 4; i += 4)
            {
                S = (airf32 *)src + i;
                f = _kx[0];
                s0 = f * S[0];
                s1 = f * S[1];
                s2 = f * S[2];
                s3 = f * S[3];

                for (k = 1; k < _ksize; k++)
                {
                    S += cn;
                    f = _kx[k];
                    s0 += f * S[0];
                    s1 += f * S[1];
                    s2 += f * S[2];
                    s3 += f * S[3];
                }

                D[i] = s0;
                D[i + 1] = s1;
                D[i + 2] = s2;
                D[i + 3] = s3;
            }

            for (; i < width; i++)
            {
                S = (airf32 *)src + i;
                s0 = _kx[0] * S[0];
                for (k = 1; k < _ksize; k++)
                {
                    S += cn;
                    s0 += _kx[k] * S[0];
                }
                D[i] = s0;
            }
        }
    }


#if defined(AIR_SSE_OPTED)
    static int coln_filter_uint8_sse(air8u **_src, air8u *dst, int width, int ksize2, air16u *ky)
    {
        air8u **src = (air8u **)_src;
        __m128i *S, *S2;
        int i = 0, k;
        for (; i <= width - 16; i += 16)
        {
            __m128i f = _mm_set1_epi16(ky[0]);
            __m128i s0, s1;
            S = (__m128i *)(src[0] + i);
            __m128i t1 = _mm_load_si128(S);
            __m128i t0 = _mm_unpacklo_epi8(t1, _mm_setzero_si128());
            t1 = _mm_unpackhi_epi8(t1, _mm_setzero_si128());

            s0 = _mm_mullo_epi16(t0, f);
            s1 = _mm_mullo_epi16(t1, f);

            for (k = 1; k <= ksize2; k++)
            {
                S = (__m128i *)(src[k] + i);
                S2 = (__m128i *)(src[-k] + i);
                f = _mm_set1_epi16(ky[k]);
                __m128i t_s1 = _mm_load_si128(S);
                __m128i t_s2 = _mm_load_si128(S2);

                __m128i t_s1_lo = _mm_unpacklo_epi8(t_s1, _mm_setzero_si128());
                __m128i t_s1_hi = _mm_unpackhi_epi8(t_s1, _mm_setzero_si128());

                __m128i t_s2_lo = _mm_unpacklo_epi8(t_s2, _mm_setzero_si128());
                __m128i t_s2_hi = _mm_unpackhi_epi8(t_s2, _mm_setzero_si128());

                t_s1_lo = _mm_adds_epu16(t_s1_lo, t_s2_lo);
                t_s1_hi = _mm_adds_epu16(t_s1_hi, t_s2_hi);

                t_s1_lo = _mm_mullo_epi16(t_s1_lo, f);
                t_s1_hi = _mm_mullo_epi16(t_s1_hi, f);

                s0 = _mm_adds_epu16(s0, t_s1_lo);
                s1 = _mm_adds_epu16(s1, t_s1_hi);
            }
            s0 = _mm_adds_epu16(s0, _mm_set1_epi16(128));
            s1 = _mm_adds_epu16(s1, _mm_set1_epi16(128));
            s0 = _mm_srli_epi16(s0, 8);
            s1 = _mm_srli_epi16(s1, 8);
            s0 = _mm_packus_epi16(s0, s1);

            _mm_storeu_si128((__m128i *)(dst + i), s0);
        }

        for (; i <= width - 4; i += 4)
        {
            __m128i s0, f = _mm_set1_epi16(ky[0]);
            s0 = _mm_cvtsi32_si128(*((int *)(src[0] + i)));
            s0 = _mm_cvtepu8_epi16(s0);
            s0 = _mm_mullo_epi16(s0, f);

            for (k = 1; k <= ksize2; k++)
            {
                __m128i t1, t2;
                f = _mm_set1_epi16(ky[k]);
                t1 = _mm_cvtsi32_si128(*((int *)(src[k] + i)));
                t2 = _mm_cvtsi32_si128(*((int *)(src[-k] + i)));
                t1 = _mm_cvtepu8_epi16(t1);
                t2 = _mm_cvtepu8_epi16(t2);
                t1 = _mm_adds_epu16(t1, t2);
                t1 = _mm_mullo_epi16(t1, f);
                s0 = _mm_adds_epu16(s0, t1);
            }
            s0 = _mm_adds_epu16(s0, _mm_set1_epi16(128));
            s0 = _mm_srli_epi16(s0, 8);
            s0 = _mm_packus_epi16(s0, s0);
            *(int *)(dst + i) = _mm_cvtsi128_si32(s0);
        }
        return i;
    }

    static int coln_filter_real32_sse(air8u **_src, air8u *_dst, int width, int ksize2, airf32 *ky)
    {
        int i = 0, k;

        airf32 **src = (airf32 **)_src;
        airf32 *S, *S2;
        airf32 *dst = (airf32 *)_dst;

        for (; i <= width - 16; i += 16)
        {
            __m128 f = _mm_load_ss(ky);
            __m128 s0, s1, s2, s3;
            __m128 x0, x1;
            f = _mm_shuffle_ps(f, f, 0);

            S = src[0] + i;
            s0 = _mm_load_ps(S);
            s1 = _mm_load_ps(S + 4);
            s0 = _mm_mul_ps(s0, f);
            s1 = _mm_mul_ps(s1, f);
            s2 = _mm_load_ps(S + 8);
            s3 = _mm_load_ps(S + 12);
            s2 = _mm_mul_ps(s2, f);
            s3 = _mm_mul_ps(s3, f);

            for (k = 1; k <= ksize2; k++)
            {
                S = src[k] + i;
                S2 = src[-k] + i;
                f = _mm_load_ss(ky + k);
                f = _mm_shuffle_ps(f, f, 0);
                x0 = _mm_add_ps(_mm_load_ps(S), _mm_load_ps(S2));
                x1 = _mm_add_ps(_mm_load_ps(S + 4), _mm_load_ps(S2 + 4));
                s0 = _mm_add_ps(s0, _mm_mul_ps(x0, f));
                s1 = _mm_add_ps(s1, _mm_mul_ps(x1, f));
                x0 = _mm_add_ps(_mm_load_ps(S + 8), _mm_load_ps(S2 + 8));
                x1 = _mm_add_ps(_mm_load_ps(S + 12), _mm_load_ps(S2 + 12));
                s2 = _mm_add_ps(s2, _mm_mul_ps(x0, f));
                s3 = _mm_add_ps(s3, _mm_mul_ps(x1, f));
            }

            _mm_storeu_ps(dst + i, s0);
            _mm_storeu_ps(dst + i + 4, s1);
            _mm_storeu_ps(dst + i + 8, s2);
            _mm_storeu_ps(dst + i + 12, s3);
        }

        for (; i <= width - 4; i += 4)
        {
            __m128 x0, s0;
            __m128 f = _mm_load_ss(ky);

            f = _mm_shuffle_ps(f, f, 0);
            s0 = _mm_load_ps(src[0] + i);
            s0 = _mm_mul_ps(s0, f);

            for (k = 1; k <= ksize2; k++)
            {
                f = _mm_load_ss(ky + k);
                f = _mm_shuffle_ps(f, f, 0);
                S = src[k] + i;
                S2 = src[-k] + i;
                x0 = _mm_add_ps(_mm_load_ps(src[k] + i), _mm_load_ps(src[-k] + i));
                s0 = _mm_add_ps(s0, _mm_mul_ps(x0, f));
            }

            _mm_storeu_ps(dst + i, s0);
        }


        return i;
    }



#endif


    static void air_coln_filter_real32(
        air8u **src, air8u *dst,
        int dststep, int count, int width,
        airf32 *coef, int coef_num)
    {
        int ksize2 = coef_num / 2;
        airf32 *ky = (airf32 *)coef + ksize2;
        int k;

        src += ksize2;

        for (; count--; dst += dststep, src++)
        {
            airf32 *D = (airf32 *)dst;
            int i = 0;
#if defined(AIR_SSE_OPTED)
            i = coln_filter_real32_sse(src, dst, width, ksize2, ky);
#endif
            for (; i <= width - 4; i += 4)
            {
                airf32 f = ky[0];
                airf32 *S = (airf32 *)src[0] + i, *S2;
                airf32 s0 = f * S[0], s1 = f * S[1], s2 = f * S[2], s3 = f * S[3];

                for (k = 1; k <= ksize2; k++)
                {
                    S = (airf32 *)src[k] + i;
                    S2 = (airf32 *)src[-k] + i;
                    f = ky[k];
                    s0 += f * (S[0] + S2[0]);
                    s1 += f * (S[1] + S2[1]);
                    s2 += f * (S[2] + S2[2]);
                    s3 += f * (S[3] + S2[3]);
                }

                D[i] = s0;
                D[i + 1] = s1;
                D[i + 2] = s2;
                D[i + 3] = s3;
            }
            for (; i < width; i++)
            {
                airf32 s0 = ky[0] * ((airf32 *)src[0])[i];
                for (k = 1; k <= ksize2; k++)
                    s0 += ky[k] * (((airf32 *)src[k])[i] + ((airf32 *)src[-k])[i]);
                D[i] = s0;
            }
        }

    }




#if defined(AIR_NEON_OPTED)
    static int coln_filter_uint8_neon(air8u **_src, air8u *dst, int width, int ksize2, air16u *ky)
    {
        air8u **src = (air8u **)_src;
        int i = 0, k;
        for (; i <= width - 16; i += 16)
        {
            uint16x8_t f = vdupq_n_u16(ky[0]);
            uint8x16_t t1 = vld1q_u8(src[0] + i);
            uint16x8_t s0 = vmovl_u8(vget_low_u8(t1));
            uint16x8_t s1 = vmovl_u8(vget_high_u8(t1));
            s0 = vmulq_u16(s0, f);
            s1 = vmulq_u16(s1, f);
            for (k = 1; k <= ksize2; k++)
            {
                f = vdupq_n_u16(ky[k]);
                uint8x16_t t_s1 = vld1q_u8(src[k] + i);
                uint8x16_t t_s2 = vld1q_u8(src[-k] + i);

                uint16x8_t t_s1_lo = vmovl_u8(vget_low_u8(t_s1));
                uint16x8_t t_s1_hi = vmovl_u8(vget_high_u8(t_s1));

                uint16x8_t t_s2_lo = vmovl_u8(vget_low_u8(t_s2));
                uint16x8_t t_s2_hi = vmovl_u8(vget_high_u8(t_s2));

                t_s1_lo = vqaddq_u16(t_s1_lo, t_s2_lo);
                t_s1_hi = vqaddq_u16(t_s1_hi, t_s2_hi);

                t_s1_lo = vmulq_u16(t_s1_lo, f);
                t_s1_hi = vmulq_u16(t_s1_hi, f);

                s0 = vqaddq_u16(s0, t_s1_lo);
                s1 = vqaddq_u16(s1, t_s1_hi);
            }
            s0 = vrshrq_n_u16(s0, 8);
            s1 = vrshrq_n_u16(s1, 8);
            int16x8_t s0_i16 = vreinterpretq_s16_u16(s0);
            int16x8_t s1_i16 = vreinterpretq_s16_u16(s1);
            uint8x16_t d = vcombine_u8(vqmovun_s16(s0_i16), vqmovun_s16(s1_i16));
            vst1q_u8(&dst[i], d);
        }

        for (; i <= width - 4; i += 4)
        {
            uint16x8_t f = vdupq_n_u16(ky[0]);
            uint32x2_t x0;
            x0 = vld1_lane_u32((const air32u *)(src[0] + i), x0, 0);
            uint16x8_t s0;
            s0 = vmovl_u8(vreinterpret_u8_u32(x0));
            s0 = vmulq_u16(s0, f);
            for (k = 1; k <= ksize2; k++)
            {
                uint32x2_t tt1, tt2;
                f = vdupq_n_u16(ky[k]);
                tt1 = vld1_lane_u32((const air32u *)(src[k] + i), tt1, 0);
                tt2 = vld1_lane_u32((const air32u *)(src[-k] + i), tt2, 0);

                uint16x8_t t1 = vmovl_u8(vreinterpret_u8_u32(tt1));
                uint16x8_t t2 = vmovl_u8(vreinterpret_u8_u32(tt2));

                t1 = vqaddq_u16(t1, t2);
                t1 = vmulq_u16(t1, f);
                s0 = vqaddq_u16(s0, t1);

            }
            s0 = vrshrq_n_u16(s0, 8);
            uint32x2_t dd = vreinterpret_u32_u8(vqmovun_s16(vreinterpretq_s16_u16(s0)));
            vst1_lane_u32((air32u *)&dst[i], dd, 0);
        }
        return i;
    }
#endif



    static void air_coln_filter_uint8(air8u **src, air8u *dst, int dststep, int count, int width,
                                      airf32 *coef, int coef_num, air16u *coef_q8)
    {
        int ksize2 = coef_num >> 1;
        int i, k;
        air16u *ky_q8 = coef_q8 + ksize2;

        src += ksize2;

        for (; count--; dst += dststep, src++)
        {
            air8u *D = (air8u *)dst;
#if defined(AIR_SSE_OPTED)
            i = coln_filter_uint8_sse(src, dst, width, ksize2, ky_q8);
#elif defined(AIR_NEON_OPTED)
            i = coln_filter_uint8_neon(src, dst, width, ksize2, ky_q8);
#else
            i = 0;
#endif
            for (; i < width; i++)
            {
                int s0 = ky_q8[0] * ((const air8u *)src[0])[i];
                for (k = 1; k <= ksize2; k++)
                    s0 += ky_q8[k] * (((const air8u *)src[k])[i] + ((const air8u *)src[-k])[i]);
                D[i] = (s0 + (1 << 7)) >> 8;
            }
        }
    }


    static int border_interpolate(int p, int len)
    {
        if ((unsigned)p < (unsigned)len) return p;
        if (len == 1) return 0;
        do
        {
            if (p < 0)
            {
                p = -p;
            }
            else
            {
                p = 2 * len - p - 2;
            }
        }
        while ((unsigned)p >= (unsigned)len);
        return p;
    }


#ifdef __cplusplus
}
#endif /* __cplusplus */

}

#endif
