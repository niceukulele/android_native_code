﻿#ifndef FP_ALGO_H
#define FP_ALGO_H
#include "typedef.h"

#ifdef __cplusplus
extern "C" {
#endif

#define  TEST     0                     //Control the stitch_im image
#define  DESCRIPTOR_SIMPLIFY    1
#define  COASE_MATCH_ACCE       1

#define  V68_DEF  1                 // 68 x 118
#define  V64_DEF  0                 // 64 x 128
#define  V96_DEF  0                 // 96 x 96
	
#if V68_DEF || V64_DEF
#define MAX_FP_TEMPLATE_NUM    40  //最大注册图片数
#else
#define   MAX_FP_TEMPLATE_NUM  30  //最大注册图片数
#endif

#define MAX_FP_KEYPOINT_NUM             160
#define MAX_FP_KEYPOINT_NUM_TEMPORARY   300

#define  _REG_SORT_             1   //Sort the templates according recognition times
	
#if V64_DEF
#define SENSOR_ROW           64
#define SENSOR_COL           128
#define ZOOM_OUT_IM          1
#define  imgWidth            176
#define  imgHight            88
#endif

#if V68_DEF
#define SENSOR_ROW           68
#define SENSOR_COL           118
#define ZOOM_OUT_IM          0
#if ZOOM_OUT_IM
#define  imgWidth            144
#define  imgHight            83
#else
#define  imgWidth            120
#define  imgHight            68
#endif
#endif

#if V96_DEF
#define SENSOR_ROW           96
#define SENSOR_COL           96
#define   imgWidth           96
#define   imgHight           96
#endif

#define   VERSION_INFO_LEN    64
#define   MODIFIED_INFO_LEN   1024

#define LSH_BYTE  4
#if DESCRIPTOR_SIMPLIFY
#define MEDIAN_BYTE 2
#else
#define MEDIAN_BYTE 4
#endif
/****************************************************/

typedef struct{
    int32_t   cols;
    int32_t   rows;
    int32_t   step;
    int32_t   dataSize;
    int32_t   dataType;
    void      *data;
}fpMat_t;

typedef struct {
    int16_t flag;
    uint16_t dx;
    uint16_t dy;
    int16_t ori;
    int32_t lsh_des[MEDIAN_BYTE + LSH_BYTE];
}fpKeyPoint_t;

typedef struct {
#if TEST
    fpMat_t *image;
#endif
    fpMat_t *bw_ims;
	fpMat_t *mask;
    int32_t keyPointNum;
    fpKeyPoint_t keyPoint[MAX_FP_KEYPOINT_NUM];
    int32_t is_stitch; //Is stitched or not;
    int32_t idx;
    int32_t zeroIdx;
    int32_t img_quality;     //Image quality score(0~100)
	int32_t valid_area;      //Image valid area ratio(0~100)
    int32_t type;            //Image source(register/studied/replaced)
    int32_t add_pixels;      //Addtional pixel numbers
    int32_t relative_id;     //Relative location in template;
    int32_t replace_freq;    //Replace frequency;
    int32_t recognize_freq;  //Recogniation frequency;
    int32_t* feat_ptr;       //Reserved pointer;
    /*************************************************************/
}fpTemplate_t;

typedef struct{
    int32_t inlinersnum;
    fpMat_t *tranm;
}fpTpH;

typedef struct{
    fpMat_t* stitch_im;
    int32_t start_id;
    int32_t xoff;
    int32_t yoff;
    BOOL is_start;
}Stitch_INFO;

/********************************************************/
typedef struct {
#if _REG_SORT_
    int32_t    regSort[MAX_FP_TEMPLATE_NUM];  //Sort the templates according recognition times
#endif 
    int32_t    fp_sign;                       //Is v1.04.28 or not;
    int8_t     version[VERSION_INFO_LEN];          //Algorithm version;
    int8_t     modify[MODIFIED_INFO_LEN];          //Modify record;
    int32_t    recognize_freq;                //Reconition times for current finger;
    int32_t    replace_num;                   //Replace times for current finger;
    int32_t    study_num;                     //Study times for current finger;
    int32_t    finger_area;                   //Total area for current finger template(Including primary template and secondary template);
    int32_t*   fp_ptr;                        //Reserved pointer; 
}finger_Info;

/**********************************************************************************/

typedef struct {
    uint32_t         g_fpTemplateNum;

	uint32_t         g_fpMaxTemplateNum;

    uint32_t         g_fpTphNum;

    fpTemplate_t     *g_fpTemplate[MAX_FP_TEMPLATE_NUM];

    fpTpH            *g_fpTph[MAX_FP_TEMPLATE_NUM*(MAX_FP_TEMPLATE_NUM + 1) / 2];

    Stitch_INFO      sticth_info;

    finger_Info      fg_info;

}fingerTempInfo;
typedef fingerTempInfo finger_template;
/**********************************************************************************/
typedef struct{
	uint32_t frameNum;
	int16_t  Kr[SENSOR_COL * SENSOR_ROW];        //Auto calibration parameters
	int16_t  B[SENSOR_COL * SENSOR_ROW];         //Baseline no touch
	uint16_t caliRes[SENSOR_COL * SENSOR_ROW];   //calibration data
	uint8_t dataBmp[SENSOR_COL * SENSOR_ROW];    //Bmp after calibration and quantization
	uint8_t sitoBmp[SENSOR_COL * SENSOR_ROW];    //Bmp after sito, calibration and quantization
	uint32_t seleteIndex;                        //Index 0: no sito; 1: sito
}calibrationPara;

/**********************************************************************************/

//Declaration functions which maybe used externally

fpMat_t *fp_loadImage(uint8_t *img, int32_t cols, int32_t rows);


/*
Create new finger template.
Input Parameter:
MaxTemplateNum: initial max template number
This function return a finger template data structure pointer.
*/
fingerTempInfo* newTemp(uint32_t MaxTemplateNum);


/*
Delete last finger in the finger template;
*/
int32_t  eraseLastFinger(fingerTempInfo* ftemp);

void deleteTemp(fingerTempInfo** ftemp);

void freeFeature(fpTemplate_t** fpt);

void matFree(fpMat_t** fpMat_t);


/* 
Get image quality:
Input Parmeters:
gray: finger image;

Output Parameters:
imgQuality: image quality;
valid_area_ratio: image valid area ratio;
*/
void img_quality(fpMat_t* gray, int32_t *imgQuality, int32_t* valid_area_ratio);


/*
This function return image feature template;
Example:
fpTemplate_t *imgtemp = NULL;
imgtemp = getFeature(src,imgQuality,validArea);
*/
fpTemplate_t* getFeature(fpMat_t* src, int32_t imgQuality, int32_t validArea);


/* Register:
Input Parameters:
imgtemp: image feature template;
ftemp: finger template;

Output Parameters:
ftemp: finger template;

This function return 32 bits score which indicates the overlay with last frame and the total add pixels;
*/
int32_t fingerFeatureRegister(fpTemplate_t* imgtemp, fingerTempInfo* ftemp);


/* Recognition:
Input Parameters:
inputTmp: image feature template;
ftemp: finger template;
recogScoreThr: Score threshold for recognition;
flagStudyEnable: 1:Enable Study; 0: Disable Study;

Output Parameters:
update: flag for first study/second study/replacement;

This function return
match score: for recognition;
0: not recognition;
-1: not recognition, valid area is small;
-2: not recognition, require reprocess again (TBD)
*/
int32_t fingerFeatureRecognition(fpTemplate_t* inputTmp, fingerTempInfo* ftemp, int32_t recogScoreThr, int32_t* update, int32_t flagStudyEnable);

int8_t* getVersion(fingerTempInfo* ftemp);

int8_t* getModifyInfo(fingerTempInfo* ftemp);

fpMat_t* getStitchIm(fingerTempInfo* ftemp);


/*
Update new stitch painting when replacement happens or firstly load the templates;
ftemp is input and output;

Example:
int32_t update = 0;
fingerFeatureRecognition(inputTmp, ftemp, recogScore, &update, 1);
if(2 == update)
{
updateStitchPainting(ftemp);
}
*/
void updateStitchPainting(fingerTempInfo* ftemp);


/* 
Delete redundant transfer matrix after finish registration procedure;
ftemp is input and output;
*/
void DeleteTranM(fingerTempInfo* ftemp);


/* Preprocess
Input Parameters:
buffer: rawdata buffer;
bufferLen: rawdata buffer length;
caliPara: autocalibration parameters;
flagChipInfo: flag for chip information, autocalibration parameters
thrSelectBmp: threshold for select bmp(sito or no sito);

Output Parameters:
imgQuality: image quality;
validArea:  valid area ratio;

Example:
#define IS_FLOTING       0x00000000
#define IS_PIXEL_CANCEL  0x00000000
fpMat_t *mat = NULL;
int32_t imgquality[2];
int32_t thrSelectBmp = 400;
calibrationPara *caliPara = (calibrationPara *)MALLOC(sizeof(calibrationPara));
mat = preProcess(buffer, bufferLen, caliPara, IS_FLOTING|IS_PIXEL_CANCEL, thrSelectBmp, &imgquality[0], &imgquality[1]);
*/
fpMat_t *preProcess(uint8_t *buffer, int32_t bufferLen, calibrationPara *caliPara, int32_t flagChipInfo, int32_t *calType, int32_t thrSelectBmp, int32_t *imgQuality, int32_t* validArea);


#ifdef __cplusplus
}
#endif

#endif

