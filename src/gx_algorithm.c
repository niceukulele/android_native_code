/******************** (C) COPYRIGHT 2013 GOODIX ********************************
* File Name          : gf_algorithm.h
* Author               : peng.hu
* Version            	: 1.0
* Date               	: 2015.1.9
* Description        : Defined structs that used by algorithm.And defined all APIs
* 					   as function pointer sets.
*******************************************************************************/
#include "gx_algorithm.h"
#include "fp_algorithm.h"
#include "gx_memory.h"
#include "gx_log.h"

#define GF_MAX_ENROLL_COUNT 8
#define GF_MAX_COUNT 35
#define MIN_VERIFY_THRESHOLD 0

#ifndef inline
#define inline 
#endif

extern fpMat_t *matCreat_new(int32_t column, int32_t row, uint8_t dataType);


/****************************************************************
 * Description : Init for enrolling fingerprint, It will creat
 * 				 a fingerprint template.
 ****************************************************************/
enroll_session_t* enrolStart(void) {
	
	enroll_session_t *pSession = NULL;
	gx_enroll_session_data_t *pSessionData = NULL;
	gx_template_t *pTemplate = NULL;

        //LOGD("enrolStart : 1.06.20 96 x 96");
	
	pSession = (enroll_session_t *)gx_Malloc(sizeof(enroll_session_t));

	pSessionData = (gx_enroll_session_data_t *)gx_Malloc( sizeof(gx_enroll_session_data_t));
	pTemplate = (gx_template_t*)gx_Malloc(sizeof( gx_template_t));

	if (NULL == pSession || NULL == pSessionData || NULL == pTemplate)
	{
		LOGE("enrolStart : gx_Malloc failed");
		gx_Free(pSession);
		gx_Free(pSessionData);
		gx_Free(pTemplate);
	}
	LOGD("enrolStart : gx_Malloc success.");
	pSession->nMaxNum = GF_MAX_ENROLL_COUNT;
	pSession->nUsedNum = 0;
	pSession->pSessionData = pSessionData;

	pTemplate->pFingerTemplate = (finger_template *) newTemp(GF_MAX_COUNT);
	pSessionData->pTemplate = pTemplate;
	//wb add
        LOGD("enrolStart: Algorithm version %s", getVersion(pTemplate->pFingerTemplate));
	return pSession;
}

/****************************************************************
 * Description : enroll one or more image to fingerprint template.
 * ***************************************************************/
#ifndef MIN
#  define MIN(a,b)  ((a) > (b) ? (b) : (a))
#endif

#ifndef MAX
#  define MAX(a,b)  ((a) < (b) ? (b) : (a))
#endif
gx_errcode_e enrolAddImage(enroll_session_t *pSession, gx_image_t* image,
		image_quality_t* image_quality) 
{
	int32_t imgquality[2] = {0};
    fpMat_t finger_im = {0};
    finger_template* pFPTemplate = NULL;
    int32_t update = 0;
    
	LOGE("enrolAddImage");
	if (pSession == NULL || pSession->pSessionData == NULL
			|| ((gx_enroll_session_data_t *)(pSession->pSessionData))->pTemplate == NULL
			|| ((gx_enroll_session_data_t *)(pSession->pSessionData))->pTemplate->pFingerTemplate == NULL
			|| image == NULL
			|| image_quality == NULL) {
		LOGE("BAD PARAM , NULL POINTER.");
		return GX_ALGO_BAD_PARAM;
	}
	if (image->format.bits_per_pixels != 8
			||image->format.channels  != 1
			||image->frame_count == 0)
	{
		LOGD("enrolAddImage : bits = %d  channels = %d frame_count = %d",
			image->format.bits_per_pixels,image->format.channels,image->frame_count);
		LOGE("BAD PARAM ,Image Format is not right.");
		return GX_ALGO_BAD_PARAM;
	}

	LOGD("image->buffer = %x",(int)image->buffer);
	if (image->buffer == NULL)
	{
		LOGE("BAD PARAM ,Image buffer is NULL!");
		return GX_ALGO_BAD_PARAM;
	}

	pFPTemplate =
			(finger_template *)((gx_enroll_session_data_t *)(pSession->pSessionData))->pTemplate->pFingerTemplate;

	finger_im.cols = image->format.width;
	finger_im.rows = image->format.height;
	finger_im.dataType = 1;
	finger_im.dataSize = image->format.width * image->format.height;
	finger_im.data = (char *)gx_Malloc(finger_im.dataSize);
	finger_im.step = finger_im.rows * finger_im.dataType;

	gx_Memcpy(finger_im.data,image->buffer,finger_im.dataSize);

	img_quality(&finger_im,&imgquality[0],&imgquality[1]);

	image_quality->quality = imgquality[0];
	image_quality->coverage = imgquality[1];
        LOGE("enrolAddImage, image quality %d, coverage %d", image_quality->quality, image_quality->coverage);
	if (pSession->nUsedNum < pSession->nMaxNum)
	{
		int32_t result = 0;
		fpTemplate_t* pFeature = getFeature(&finger_im,image_quality->quality,image_quality->coverage);
		result = fingerFeatureRegister(pFeature,pFPTemplate);

		pSession->nPreOverlay = 100-((unsigned)(result & 0xff000000) >> 24);

		pSession->nOverlay = (100-(result & 0x000000ff));

		pSession->nUsedNum++;

        freeFeature(&pFeature);
	}

	gx_Free(finger_im.data);
    pSession->nProgress = MIN(100,pSession->nUsedNum * (99 / pSession->nMaxNum + 1));
	return GX_ALGO_SUCCESS;
}

/****************************************************************
 * Description : Delete last frame frome fingerprint templates
 * ***************************************************************/
gx_errcode_e enrolDeleteImage(enroll_session_t *pSession)
{
	finger_template *pFPTemplate =
			(finger_template *)((gx_enroll_session_data_t *)(pSession->pSessionData))->pTemplate->pFingerTemplate;

	LOGE("enrolDeleteImage");
	if (pSession == NULL || pSession->pSessionData == NULL
			|| ((gx_enroll_session_data_t *)(pSession->pSessionData))->pTemplate == NULL
			|| ((gx_enroll_session_data_t *)(pSession->pSessionData))->pTemplate->pFingerTemplate == NULL)
	{
		LOGE("BAD PARAM , NULL POINTER.");
		return GX_ALGO_BAD_PARAM;
	}

	pFPTemplate = (finger_template *)
			((gx_enroll_session_data_t *)(pSession->pSessionData))->pTemplate->pFingerTemplate;

	eraseLastFinger(pFPTemplate);
	//wb add
        if (pSession->nUsedNum > 0)
        {
	    pSession->nUsedNum--;
	    pSession->nProgress = MIN(100,pSession->nUsedNum * (99 / pSession->nMaxNum + 1));
        }
	return GX_ALGO_SUCCESS;
}

/****************************************************************
 * Description : get the template data pointer.
 * ***************************************************************/
gx_errcode_e enrolGetTemplate(enroll_session_t *session,
		gx_template_t** tpl) {
	LOGE("enrolGetTemplate");
	if (session == NULL || session->pSessionData == NULL)
	{
		LOGE("enrolGetTemplate param failed!");
		return GX_ALGO_BAD_PARAM;
	}
	*tpl = ((gx_enroll_session_data_t *)session->pSessionData)->pTemplate;

	return GX_ALGO_SUCCESS;
}

/****************************************************************
 * Description : end the session
 * ***************************************************************/
gx_errcode_e enrolFinish(enroll_session_t *session) {

    gx_template_t *pTemplate = NULL;

	LOGE("enrolFinish");
	/*gx_Free memory*/
	if (session == NULL)
	{
		LOGE("enrolFinish : session is NULL.");
		return GX_ALGO_BAD_PARAM;
	}
	pTemplate = (gx_template_t *)((gx_enroll_session_data_t *)(session->pSessionData)->pTemplate);
	deleteTemp((fingerTempInfo **)(&(pTemplate->pFingerTemplate)));
        //wb modify
        gx_Free(pTemplate);
	gx_Free(session->pSessionData);
	session->pSessionData = NULL;
	gx_Free(session);
	return GX_ALGO_SUCCESS;
}

/****************************************************************
 * Description : verify fingerprint.
 * ***************************************************************/
gx_errcode_e identifyImage(gx_image_t* image, gx_template_t** candidates,
		uint32_t candidate_count, int32_t* result, int32_t* score,
		image_quality_t* image_quality, int32_t* update) 
{
	int i = 0;
	int32_t quality[2] = {0};
    fpTemplate_t *pTemplate = NULL;
    fpMat_t *finger_im = NULL;

    LOGE("identifyImage");

    
	if (image == NULL || candidates == NULL || result == NULL || score == NULL
			|| image_quality == NULL || update == NULL)
	{
		LOGE("GF_BAD_PARAM");
		return GX_ALGO_BAD_PARAM;
	}

	if (image->format.bits_per_pixels != 8
		||image->format.channels  != 1
		||image->frame_count == 0)
	{
		LOGE("GF_NOSUPPORT_IMAGE : %d %d %d",
			image->format.bits_per_pixels,
			image->format.channels,
			image->frame_count);
		return GX_ALGO_BAD_PARAM;
	}
	if (image->buffer == NULL)
	{
		LOGE("GF_NO_IMAGEDATA");
		return GX_ALGO_BAD_PARAM;
	}

	finger_im = matCreat_new(image->format.width,image->format.height,1);
	gx_Memcpy(finger_im->data,image->buffer,image->format.height * image->format.width);
	
        LOGE("Identify,finger_im : cols = %d rows = %d",finger_im->cols,finger_im->rows);
        
	img_quality(finger_im,&quality[0],&quality[1]);
	image_quality->coverage = quality[1];
	image_quality->quality  = quality[0];
        LOGE("Identify, image quality %d, coverage %d", image_quality->quality, image_quality->coverage);
	pTemplate = getFeature(finger_im,image_quality->quality,image_quality->coverage);

	for (i = 0; i < candidate_count; i++) {
		fingerTempInfo* pFPTemplate = (fingerTempInfo *)(candidates[i]->pFingerTemplate);
		int hasUpdate = 0;

		int ratio =  fingerFeatureRecognition(pTemplate,pFPTemplate, 0, &hasUpdate, 1);
		LOGE("i = %d , ratio = %d  hasUpdate = %d",i,ratio,hasUpdate);
		//wb add
		if (hasUpdate >= 2) {
            updateStitchPainting(pFPTemplate);
        }
		if (ratio >= MIN_VERIFY_THRESHOLD) {
			//*score  = ratio * 100 >> 8;
			*score  = ratio;
			*result = i;
			*update = hasUpdate;
        	freeFeature(&pTemplate);
        	matDestroy(finger_im);
        	LOGD("IDENTIFY : free mat");
			return GX_ALGO_SUCCESS;
		}
	}

	freeFeature(&pTemplate);

	matDestroy(finger_im);
	LOGD("N-IDENTIFY : free mat");
	
	*score = 0;
	*result = -1;
	if (update != NULL)
		*update = 0;
	return GX_ALGO_SUCCESS;
}

#define PACK_DATA(dst,src,length) gx_Memcpy(dst,src, length);dst += length;
#define UNPACK_DATA(dst,src,length) gx_Memcpy(dst,src, length);src += length;

/****************************************************************
 * Description : pack size
 * ***************************************************************/
inline uint32_t getPackMatSize(fpMat_t *mat)
{
	int length = 0;
	if (NULL == mat)
	{
		return sizeof(int);
//		return sizeof(fpMat_t);
	}
	length += sizeof(mat->cols);
	length += sizeof(mat->rows);
	length += sizeof(mat->step);
	length += sizeof(mat->dataSize);
	length += sizeof(mat->dataType);
	length += mat->dataSize;
	return length;
}
/****************************************************************
 * Description : Save mat to buffer
 * ***************************************************************/
inline uint32_t packMat(fpMat_t *mat,uint8_t *dst)
{
	uint8_t *pData = dst;
	if (NULL == dst)
	{
		return 0;
	}
	if (NULL == mat)
	{
		gx_Memset(pData,-1,sizeof(mat->dataSize));
		return sizeof(mat->dataSize);
	}

	PACK_DATA(pData, &(mat->dataSize), sizeof(mat->dataSize));
	PACK_DATA(pData, &(mat->cols), sizeof(mat->cols));
	PACK_DATA(pData, &(mat->rows), sizeof(mat->rows));
	PACK_DATA(pData, &(mat->step), sizeof(mat->step));
	PACK_DATA(pData, &(mat->dataType), sizeof(mat->dataType));

	if (gx_Memcmp(mat,dst,20) == 0)
	{
		LOGE("memcmp = 0");
	}


	PACK_DATA(pData, mat->data, mat->dataSize);

	return pData - dst;
}



/****************************************************************
 * Description : Get mat from buffer
 * ***************************************************************/
inline uint32_t unpackMat(fpMat_t **ppMat,uint8_t *src)
{
	uint8_t *pData = src;
    int size = 0;
    fpMat_t *mat = NULL;
    
	if (src == NULL)
	{
		return 0;
	}
	
	UNPACK_DATA(&size,pData, sizeof(int));
	if (size == -1)
	{
		*ppMat = NULL;
		return pData - src; // 4
	}
	mat = (fpMat_t *)gx_Malloc(sizeof(fpMat_t));
	gx_Memset(mat,0,sizeof(fpMat_t));
	mat->dataSize = size;
	UNPACK_DATA(&(mat->cols),pData, sizeof(mat->cols));
	UNPACK_DATA(&(mat->rows),pData, sizeof(mat->rows));
	UNPACK_DATA(&(mat->step),pData, sizeof(mat->step));
	UNPACK_DATA(&(mat->dataType),pData, sizeof(mat->dataType));
	if (0 != mat->dataSize )
	{
		mat->data = (void *)gx_Malloc((unsigned int)mat->dataSize);
		UNPACK_DATA(mat->data,pData,mat->dataSize);
	}
	else
	{
		mat->data = NULL;
	}
	*ppMat = mat;
	return pData - src;
}

/****************************************************************
 * Description : Save one finger template to buffer
 * ***************************************************************/
//#define PACK_DATA(dst,src,length) gx_Memcpy((dst),(src),(length));src += (length);

inline int32_t packFpTemplate(fpTemplate_t *pFpTemplate,uint8_t *dst)
{
	int i = 0;
    uint8_t *pData = NULL;
	if (NULL == pFpTemplate || NULL == dst)
	{
		return 0;
	}
    pData = dst;
	
//	pData += packMat(pFpTemplate->image,pData);
	pData += packMat(pFpTemplate->bw_ims,pData);
	//pData += packMat(pFpTemplate->mask,pData);
	PACK_DATA(pData,&(pFpTemplate->keyPointNum),4);
	for (i = 0 ; i < pFpTemplate->keyPointNum ; i++)
	{
		PACK_DATA(pData,&(pFpTemplate->keyPoint[i]),sizeof(fpKeyPoint_t));
	}
	PACK_DATA(pData,&(pFpTemplate->is_stitch),4);
//	PACK_DATA(pData,&(pFpTemplate->originImgQuality),4);
//	PACK_DATA(pData,&(pFpTemplate->originImgValidArea),4);
//	PACK_DATA(pData,&(pFpTemplate->zeroIdx),4);
	PACK_DATA(pData,&(pFpTemplate->idx),4);
	PACK_DATA(pData,&(pFpTemplate->zeroIdx),4);
	PACK_DATA(pData,&(pFpTemplate->img_quality),4);
	PACK_DATA(pData,&(pFpTemplate->valid_area),4);
	PACK_DATA(pData,&(pFpTemplate->type),4);
	PACK_DATA(pData,&(pFpTemplate->add_pixels),4);
	PACK_DATA(pData,&(pFpTemplate->relative_id),4);
	PACK_DATA(pData,&(pFpTemplate->replace_freq),4);
	PACK_DATA(pData,&(pFpTemplate->recognize_freq),4);
	return pData - dst;
}

inline int32_t unpackFpTemplate(fpTemplate_t **ppFpTemplate,uint8_t *src)
{
	int i = 0;
    uint8_t *pData = src;
    fpTemplate_t *pFpTemplate = NULL;
    
	if (NULL == ppFpTemplate || NULL == src)
	{
		return 0;
	}
	
	pFpTemplate = (fpTemplate_t *)gx_Malloc(sizeof(fpTemplate_t));

//	pData += unpackMat(&(pFpTemplate->image),pData);
//	pFpTemplate->image = NULL;
	pData += unpackMat(&(pFpTemplate->bw_ims),pData);
//	pData += unpackMat(&(pFpTemplate->mask),pData);
	UNPACK_DATA(&(pFpTemplate->keyPointNum),pData,4);
//	pFpTemplate->keyPoint = (fpKeyPoint_t **)gx_Malloc(sizeof(fpKeyPoint_t * ) * pFpTemplate->keyPointNum);
//  gx_Mallc max number of template.
//	pFpTemplate->keyPoint = (fpKeyPoint_t **)gx_Malloc(sizeof(fpKeyPoint_t * ) * MAX_FP_KEYPOINT_NUM);

	for (i = 0 ; i < pFpTemplate->keyPointNum ; i++)
	{
		//pFpTemplate->keyPoint[i] = (fpKeyPoint_t *)gx_Malloc(sizeof(fpKeyPoint_t));
		UNPACK_DATA(&(pFpTemplate->keyPoint[i]),pData,sizeof(fpKeyPoint_t));
	}
	//for (i = pFpTemplate->keyPointNum ; i < MAX_FP_KEYPOINT_NUM ; i++)
	//{
		//pFpTemplate->keyPoint[i] = (fpKeyPoint_t *)gx_Malloc(sizeof(fpKeyPoint_t));
	//}
	UNPACK_DATA(&(pFpTemplate->is_stitch),pData,4);
//	UNPACK_DATA(&(pFpTemplate->originImgQuality),pData,4);
//	UNPACK_DATA(&(pFpTemplate->originImgValidArea),pData,4);
//	UNPACK_DATA(&(pFpTemplate->zeroIdx),pData,4);
	UNPACK_DATA(&(pFpTemplate->idx),pData,4);
	UNPACK_DATA(&(pFpTemplate->zeroIdx),pData,4);
	UNPACK_DATA(&(pFpTemplate->img_quality),pData,4);
	UNPACK_DATA(&(pFpTemplate->valid_area),pData,4);
	UNPACK_DATA(&(pFpTemplate->type),pData,4);
	UNPACK_DATA(&(pFpTemplate->add_pixels),pData,4);
	UNPACK_DATA(&(pFpTemplate->relative_id),pData,4);
	UNPACK_DATA(&(pFpTemplate->replace_freq),pData,4);
	UNPACK_DATA(&(pFpTemplate->recognize_freq),pData,4);

	*ppFpTemplate = pFpTemplate;
	return pData - src;
}

inline int32_t getPpackFpTemplateSize(fpTemplate_t *pFpTemplate)
{
	int length = 0;
	if (NULL == pFpTemplate)
	{
		LOGE("getPpackFpTemplateSize = null");
		return 0;
	}
//	length += getPackMatSize(pFpTemplate->image);
	length += getPackMatSize(pFpTemplate->bw_ims);

//	LOGD("%d , %d ,%d ",pFpTemplate->mask->dataSize,pFpTemplate->mask->cols,pFpTemplate->mask->rows);
//	length += getPackMatSize(pFpTemplate->mask);

	/* 4 + pFpTemplate->keyPointNum * sizeof(fpKeyPoint_t) + 32*/
	length += 44 + pFpTemplate->keyPointNum * sizeof(fpKeyPoint_t);
	return length;
}

inline int32_t packH(fpTpH *h , uint8_t *dst)
{
	uint8_t *pData = dst;
	if (NULL == h || NULL == dst)
	{
		LOGE("H = NULL");
		return 0;
	}
	PACK_DATA(pData,&(h->inlinersnum),4);
	pData += packMat(h->tranm,pData);
	return pData - dst;
}

inline int32_t getPackHSize(fpTpH *h)
{
	if (h == NULL)
		return 0;
	return 4 + getPackMatSize(h->tranm);
}

inline int32_t unpackH(fpTpH **ppH , uint8_t *src)
{
	uint8_t *pData = src;
    fpTpH *h = NULL;

	if ( NULL == src || ppH == NULL)
	{
		return 0;
	}

	h = (fpTpH *)gx_Malloc(sizeof(fpTpH));
	gx_Memset(h,0,sizeof(fpTpH));
	UNPACK_DATA(&(h->inlinersnum),pData,4);
	pData += unpackMat(&(h->tranm),pData);
	*ppH = h;
	return pData - src;
}

inline int32_t packStitchInfo(Stitch_INFO *info,uint8_t *dst)
{
	uint8_t *pData = dst;
	if (NULL == info || NULL == dst)
	{
		return 0;
	}

	pData += packMat(info->stitch_im,pData);
	PACK_DATA(pData,&(info->start_id),4);
	PACK_DATA(pData,&(info->xoff),4);
	PACK_DATA(pData,&(info->yoff),4);
	//wb add
	//PACK_DATA(pData,&(info->is_start),sizeof(info->is_start));
	PACK_DATA(pData,&(info->is_start),4);
	return pData - dst;
}

inline int32_t unpackStitchInfo(Stitch_INFO *pInfo,uint8_t *src)
{
	uint8_t *pData = src;
	if (NULL == pInfo || NULL == src)
	{
		return 0;
	}
	pData += unpackMat(&(pInfo->stitch_im),pData);
	UNPACK_DATA(&(pInfo->start_id),pData,4);
	UNPACK_DATA(&(pInfo->xoff),pData,4);
	UNPACK_DATA(&(pInfo->yoff),pData,4);
	//wb add
	//UNPACK_DATA(&(pInfo->is_start),pData,sizeof(pInfo->is_start));
	UNPACK_DATA(&(pInfo->is_start),pData,4);
	return pData - src;
}

inline int32_t getPackStitchInfoSize(Stitch_INFO *info)
{
	if (NULL == info)
		return 0;
	//don't save stitch im.

	return sizeof(Stitch_INFO) - sizeof(info->stitch_im)+ getPackMatSize(info->stitch_im);
}

inline int32_t packFingerInfo(finger_Info *info,uint8_t *dst)
{
	uint8_t *pData = dst;
	if (NULL == info || NULL == dst)
	{
		return 0;
	}

	PACK_DATA(pData,info,sizeof(finger_Info));

//	PACK_DATA(pData,&(info->fp_sign),4);
//	PACK_DATA(pData,&(info->version),sizeof(info->version));
//	PACK_DATA(pData,&(info->modify),sizeof(info->modify));
//	PACK_DATA(pData,&(info->recognize_freq),4);
//	PACK_DATA(pData,&(info->replace_num),4);
//	PACK_DATA(pData,&(info->study_num),4);
//	PACK_DATA(pData,&(info->finger_area),4);
	return pData - dst;
}

inline int32_t getPackFingerInfoSize(finger_Info *info)
{
	int length = 0;
	if (NULL == info)
		return 0;
	length += sizeof(finger_Info);
	return length;
}

inline int32_t unpackFingerInfo(finger_Info *info,uint8_t *src)
{
	uint8_t *pData = src;
	if (NULL == info || NULL == src )
	{
		return 0;
	}

	UNPACK_DATA(info,pData,sizeof(finger_Info));
	return pData - src;
}


/****************************************************************
 * Description : Pack the template as data.
 * ***************************************************************/
gx_errcode_e templatePack(gx_template_t* tpl, uint8_t* dst)
{
	int i = 0;
    uint8_t *pData = NULL;
    finger_template *pTemplate = NULL;
    
	LOGE("templatePack");
	if (tpl == NULL || dst == NULL || tpl->pFingerTemplate == NULL)
	{
		LOGE("templatePack : dst = NULL , tpl == NULL");
		return GX_ALGO_BAD_PARAM;
	}
	pTemplate = (finger_template *)tpl->pFingerTemplate;
	pData = dst;


	PACK_DATA(pData,&(pTemplate->g_fpTemplateNum),sizeof(pTemplate->g_fpTemplateNum));
	PACK_DATA(pData,&(pTemplate->g_fpMaxTemplateNum),sizeof(pTemplate->g_fpMaxTemplateNum));
	PACK_DATA(pData,&(pTemplate->g_fpTphNum),sizeof(pTemplate->g_fpTphNum));

	LOGE("templatePack  = %d",pData - dst);

	for (i = 0 ; i < pTemplate->g_fpTemplateNum ; i++)
	{
		pData += packFpTemplate(pTemplate->g_fpTemplate[i],pData);
	}
	LOGE("templatePack  = %d",pData - dst);
	for (i = 0 ; i < pTemplate->g_fpTphNum ; i++)
	{
		pData += packH(pTemplate->g_fpTph[i],pData);
	}

	LOGE("templatePack  = %d",pData - dst);

	pData += packStitchInfo(&(pTemplate->sticth_info),pData);
	LOGE("templatePack  = %d",pData - dst);
	pData += packFingerInfo(&(pTemplate->fg_info),pData);
	LOGE("templatePack  = %d",pData - dst);

	return GX_ALGO_SUCCESS;
}

/****************************************************************
 * Description : Get the template data size.
 * ***************************************************************/
uint32_t templateGetPackedSize(gx_template_t *tpl)
{
	int i = 0; 
	int length = 0;
    finger_template *pTemplate = NULL;
	LOGE("templateGetPackedSize");

	if (NULL == tpl || tpl->pFingerTemplate == NULL || tpl->pFingerTemplate == 0)
	{
		return length;
	}

    pTemplate = (finger_template *)tpl->pFingerTemplate;

	length += sizeof(pTemplate->g_fpTemplateNum) 
	+ sizeof(pTemplate->g_fpTphNum) 
	+ sizeof(pTemplate->g_fpMaxTemplateNum);
	LOGE("templateGetPackedSize  = %d",length);
	for (i = 0 ; i < pTemplate->g_fpTemplateNum ; i++)
	{
		length += getPpackFpTemplateSize(pTemplate->g_fpTemplate[i]);
	}
	LOGE("templateGetPackedSize  = %d",length);
	for (i = 0 ; i < pTemplate->g_fpTphNum ; i++)
	{
		length += getPackHSize(pTemplate->g_fpTph[i]);
	}
	LOGE("templateGetPackedSize  = %d",length);
	length += getPackStitchInfoSize(&(pTemplate->sticth_info));
	LOGE("templateGetPackedSize  = %d",length);
	length += getPackFingerInfoSize(&(pTemplate->fg_info));
	LOGE("templateGetPackedSize  = %d",length);

	return length;
}

/****************************************************************
 * Description : Unpack template data
 * ***************************************************************/

gx_errcode_e templateUnPack(uint8_t* src,uint32_t length,gx_template_t** tpl)
{
	int i = 0; 
	gx_template_t *pHuaweiTemp = NULL;
    finger_template *pTemplate = NULL;
    uint8_t *pData = NULL;
    pHuaweiTemp = (gx_template_t *)gx_Malloc(sizeof(gx_template_t));

	LOGD("templateUnPack : length = %d",length);
	if (src == NULL || tpl == NULL || length == 0)
	{
		gx_Free(pHuaweiTemp);
		return GX_ALGO_BAD_PARAM;
	}
	
	pTemplate = (finger_template *) newTemp(GF_MAX_COUNT);
	pHuaweiTemp->pFingerTemplate = pTemplate;
//
////	fingerTempInfo* pTemplate = (fingerTempInfo *)gx_Malloc(sizeof(fingerTempInfo));
	gx_Memset(pTemplate,0,sizeof(fingerTempInfo));
	if (NULL == pTemplate)
	{
		return GX_ALGO_NO_MEMORY;
	}
	if (src == NULL)
	{
		gx_Free(pTemplate);
		return GX_ALGO_BAD_PARAM;
	}
	pData = src;

//	int memPoolNum = 0;
//	UNPACK_DATA(&memPoolNum,pData,sizeof(memPoolNum));
//	LOGE("memPoolNum = %d ",memPoolNum);
//
//	tMemPool *pool = NULL;
//	for (int i = 0; i < memPoolNum ; i++)
//	{
//		int poolSize = 0;
//		UNPACK_DATA(&poolSize,pData,sizeof(int));
//		LOGE("poolSize = %d ",poolSize);
//		mpAlloc(poolSize,&(pTemplate->memPools[i]));
//	}
//	LOGE("templateUnPack == %d",pData - src);

	UNPACK_DATA(&(pTemplate->g_fpTemplateNum),pData,sizeof(pTemplate->g_fpTemplateNum));
	UNPACK_DATA(&(pTemplate->g_fpMaxTemplateNum),pData,sizeof(pTemplate->g_fpMaxTemplateNum));
	UNPACK_DATA(&(pTemplate->g_fpTphNum),pData,sizeof(pTemplate->g_fpTphNum));
	LOGE("templateUnPack == %d",pData - src);

	for (i = 0 ; i < pTemplate->g_fpTemplateNum ; i++)
	{
		pData += unpackFpTemplate(&(pTemplate->g_fpTemplate[i]),pData);
	}
	LOGE("templateUnPack == %d",pData - src);

	for (i = 0 ; i < pTemplate->g_fpTphNum ; i++)
	{
		pData += unpackH(&(pTemplate->g_fpTph[i]),pData);
	}

	LOGE("templateUnPack == %d",pData - src);//bu dui

	pData += unpackStitchInfo(&(pTemplate->sticth_info),pData);

	LOGE("templateUnPack == %d",pData - src);

//	finger_Info *pFingerInfo = NULL;
	pData += unpackFingerInfo(&pTemplate->fg_info,pData);
//	gx_Memcpy(&(pTemplate->fg_info),pFingerInfo,sizeof(finger_Info));
//	gx_Free(pFingerInfo);
	LOGE("templateUnPack == %d",pData - src);

	/*unpack pTemplate .*/
	//NewGetStitchImPhone(pTemplate);
	//LOGE("NewGetStitchImPhone SUCCESS");

	*tpl = pHuaweiTemp;
	return GX_ALGO_SUCCESS;
}

/****************************************************************
 * Description : Release fingerprint memory
 * ***************************************************************/
void templateDelete(gx_template_t* tpl)
{
    finger_template *pTemplate = NULL;

	LOGE("templateDelete");
	if (tpl == NULL ||tpl->pFingerTemplate == NULL)
	{
		LOGE("templateDelete : template is NULL.");
		return;
	}
	pTemplate = (finger_template *)tpl->pFingerTemplate;
	deleteTemp((fingerTempInfo **)(&pTemplate));
        //wb modify
        gx_Free(tpl);
	tpl = NULL;
}

/****************************************************************
 * Description : Get the version of algorithm
 * ***************************************************************/
int64_t getAlgorithmVersion(void)
{
	return (1<<24 | 6 << 16 | 30 << 8 ) | SENSOR_118_68;
}

/****************************************************************
 * Description : Get Quality of image
 * ***************************************************************/
void getQuality(gx_image_t *image,image_quality_t* image_quality)
{
		fpMat_t finger_im = {0};
		if (image == NULL || image->buffer == NULL)
		{
			return;
		}
		
    finger_im.cols = image->format.width;
    finger_im.rows = image->format.height;
    finger_im.dataType = 1;
    finger_im.dataSize = image->format.width * image->format.height;
    finger_im.data = image->buffer;
    finger_im.step = finger_im.rows * finger_im.dataType;
    
    img_quality(&finger_im,&(image_quality->quality),&(image_quality->coverage));
}


