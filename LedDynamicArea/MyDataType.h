#pragma once

#include "../jsoncpp/json.h"

const int PROGRAM_FRAME_SINGLE_COLOR_COUNT = 23; //��ɫ�߿�ͼƬ����
const int PROGRAM_FRAME_MULI_COLOR_COUNT = 47; //��ɫ�߿�ͼƬ����

void CreateFrameArray(int singleColorCount, int multiColorCount);
void GetControllerList(Json::Value &ControllerList_Obj, Json::Value &Controller_Supary);