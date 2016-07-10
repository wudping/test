//#pragma once
#ifndef _OCR_READ_DATA_H
#define _OCR_READ_DATA_H


/*
	�ɮפ��� head model
	head�榡
	nr_class
	nr_feature
	O_label (�}�C�Φ�)

	�Ҧp

	33
	129
	1 2 3 4 5 6 7 8 ...

	model�OOmega�����
*/



#include <stdio.h>
#include "OCR_image_process.h"
#include "OCR_shared.h"


#define		_SVM_FILE_CNY_20_HEAD_		"..\\DATA\\CNY20_head.bin"		// CNY20 head
#define		_SVM_FILE_CNY_20_MODEL_		"..\\DATA\\CNY20_model.bin"		// CNY20 model��m

#define		_SVM_FILE_CNY_100_HEAD_		"..\\DATA\\CNY100_head.bin"		// CNY100 head
#define		_SVM_FILE_CNY_100_MODEL_	"..\\DATA\\CNY100_model.bin"	// CNY100 model��m



void		OCR_read_SVM_model( OCR_INIT_TYPE init_type );					// Ū�� svm ���

void		OCR_pre_read_SVM_model( OCR_INIT_TYPE init_type );				// �w��Ū�� svm ���

void		OCR_read_SCALE_TABLE();									        // �w��Ū�� scale table ���						

//void		OCR_model_to_binary( OCR_INIT_TYPE init_type );					// �N model�ɮ��ন binary
//void		OCR_test();					

#endif