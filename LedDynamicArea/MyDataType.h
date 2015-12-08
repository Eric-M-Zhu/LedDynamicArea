#pragma once

#include "../jsoncpp/json.h"

const int PROGRAM_FRAME_SINGLE_COLOR_COUNT = 23; //纯色边框图片个数
const int PROGRAM_FRAME_MULI_COLOR_COUNT = 47; //花色边框图片个数

void CreateFrameArray(int singleColorCount, int multiColorCount);
void GetControllerList(Json::Value &ControllerList_Obj, Json::Value &Controller_Supary);