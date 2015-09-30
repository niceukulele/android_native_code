/******************** (C) COPYRIGHT 2013 GOODIX ********************************
* File Name          : gf_algorithm.h
* Author               : peng.hu
* Version              : 1.0
* Date                  : 2015.1.9
* Description         : Algorithm api
*******************************************************************************/

#ifndef GF_ALGORITHM_H_
#define GF_ALGORITHM_H_

#include "gx_image.h"

#ifdef __cplusplus
extern "C"{
#endif

typedef struct _gx_template_t{
	void *pFingerTemplate;                   /* < Fingerprint template pointer.*/
}gx_template_t;


typedef struct _gx_enroll_session_data_t
{
	gx_template_t *pTemplate;                /* < Goodix fingerprint template pointer.*/
}gx_enroll_session_data_t;        

typedef struct _enroll_session_t{
	gx_enroll_session_data_t* pSessionData;	 /* < Have already enrolled */
    int16_t                   nMaxNum;	     /* < Max template need enroll*/
	int16_t                   nUsedNum;      /* < Have already enrolled */
    int32_t                   nProgress;     /* < [0,100] enroll progress*/   
    int32_t 				  nPreOverlay;   /* < [0,100] overlay ratio with pre frame.*/
    int32_t					  nOverlay;		 /* < [0,100] overlay ratio with fingerprint template(All frame).*/
}enroll_session_t;

typedef enum 
{
    GX_ALGO_SUCCESS = 0,
    GX_ALGO_FAILED = 0X80,
    GX_ALGO_BAD_PARAM,
    GX_ALGO_NO_MEMORY,
}gx_errcode_e;

#define SENSOR_96_96  	0x01
#define SENSOR_118_68 	0x02
#define SENSOR_ALL    	0xFF

/****************************************************************
 * Description : Init for enrolling fingerprint, It will creat a session data include 
 * 				 a fingerprint template.
 * param:
 * 		        no
 * return:
 *			enroll_session_t * , the session data pointer.
 * ***************************************************************/
enroll_session_t* enrolStart(void);

/****************************************************************
 * Description : enroll one or more image to fingerprint template.
 *                   It support only one image now.
 *
 * param:
 * 		 in session , session data pointer .
 * 		 in image ,   image buffer and infomation.
 * 		 out image_quality , image quality.
 * return:
 *		 gx_errcode_e , 0 is success or less than 0 , some error happened.
 * ***************************************************************/
gx_errcode_e enrolAddImage(enroll_session_t *pSession, gx_image_t* image,
		     image_quality_t* image_quality);

/****************************************************************
 * Description : enroll one or more image to fingerprint template.
 *                   It support only one image now.
 *
 * param:
 * 		 in session , session data pointer .
 * 		 in image ,   image buffer and infomation.
 * 		 out image_quality , image quality.
 * return:
 *		 gx_errcode_e , 0 is success or less than 0 , some error happened.
 * ***************************************************************/
gx_errcode_e enrolDeleteImage(enroll_session_t *pSession);

/****************************************************************
 * Description : get the template data pointer.
 *
 * param:
 * 		 in session , session data pointer .
 * 		 out tpl , get the template pointer
 * return:
 *		 int , -1 get failed 0 success
 * ***************************************************************/
gx_errcode_e enrolGetTemplate(enroll_session_t *session, gx_template_t** tpl);

/****************************************************************
 * Description : end the session
 *
 * param:
 * 		 in session , session data pointer .
 * return:
 *		 int , [-n,0)  failed , 0 success
 * ***************************************************************/
gx_errcode_e enrolFinish(enroll_session_t *session);

/****************************************************************
 * Description : verify fingerprint.
 *
 * param:
 * 		 in img , fingerprint data .
 * 		 in candidates, fingerprint templates array.
 * 		 in candidate_count , fingerprint template count.
 * 		 out result, <0 verify failed  ,
 * 		 	         [0 - (candidate_count -1)] access fingerprint template index
 * 		 out score , [0 - 100]matching degree.
 * 		 out image_qulity, qulity.
 * 		 out update , > 0 , the template has changed , need update.
 * return:
 *		 int [errcode], < 0 failed , 0 success
 *
 * ***************************************************************/
gx_errcode_e identifyImage(gx_image_t* img,
		    gx_template_t** candidates,
		    uint32_t candidate_count,
		    int32_t* result,
		    int32_t* score,
	        image_quality_t* image_quality,
		    int32_t* update);

/****************************************************************
 * Description : Get the template data size.
 *
 * param:
 * 		 in tpl , template data pointer.
 * return:
 *		 uint , the size of fingerprint template.
 * ***************************************************************/
uint32_t templateGetPackedSize(gx_template_t *tpl);

/****************************************************************
 * Description : Pack the template as data.
 * param:
 * 		 in tpl , the fingerprint template pointer.
 * 		 out dst , output pack data
 * return:
 *		 gx_errcode_e , 0
 * ***************************************************************/
gx_errcode_e templatePack(gx_template_t* tpl, uint8_t* dst);

/****************************************************************
 * Description : Unpack template data
 * param:
 * 		 in src , data pointer.
 * 		 in length , data length.
 * 		 out tpl , fingerprint template
 * return:
 *		 gx_errcode_e , errcode, < 0 failed  = 0  success
 * ***************************************************************/
gx_errcode_e templateUnPack(uint8_t* src,
                            uint32_t length,
                            gx_template_t** tpl);

/****************************************************************
 * Description : Release fingerprint memory
 * param:
 * 		 in tpl , template pointer
 * return:
 *		 no
 * ***************************************************************/
void templateDelete(gx_template_t* tpl);


/****************************************************************
 * Description : Get the version of algorithm
 * param:
 * 		 N/A
 * return:
 *		 format version of algorithm as int
 *		 EX : 1.06.19
 *		 32 - 64 bits revert
 *		 24 - 32 bits big version
 *		 16 - 24 bits middle version
 *		 8  - 16 bits small version
 *		 0  - 8  bits sensor size
 * ***************************************************************/
int64_t getAlgorithmVersion(void);

/****************************************************************
 * Description : Get the version of algorithm
 * param:
 * 		 [in]image , source image 
 *		[out]image_quality , image quality if image is valid quality and score
 *				 will be zero.
 * return:
 *				N/A
 * ***************************************************************/
void getQuality(gx_image_t *image,image_quality_t* image_quality);

#ifdef __cplusplus
}
#endif

#endif /* GF_ALGORITHM_H_ */

