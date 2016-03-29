/****************************************************
* Toolbox for ytl's opencv project use
* Tianlong Yang, yangtianlong111@gmail.com
* Computer Science & Technology, Tsinghua University
* Copyright (c) 2016.3.29 All rights reserved.
*****************************************************/


#ifndef TOOLBOX_H
#define TOOLBOX_H

#define USE_ALIAS
#include "basic.hpp"
#include "tlDebug\tlDebug.hpp"
#include "tlGeometry\tlGeometry.hpp"
#include "tlArray\tlArray.hpp"
#include "tlMath\tlMath.hpp"
#include "tlGeometry\tlGeometry.hpp"
#include "tlTranslate\tlTranslate.hpp"


/**************************************
由于opencv库中存在一些问题和还不太完善的功能，使用此ToolBox进行补充
ytl的Toolbox，由{
	一般类模块
		tlDebug
		tlGeometry
		tlArray
	opencv函数模块
		tlMath
		tlTranslate
		tlGrating
	基础函数
		basic
}
组成

tlDebug为类Qt的调试类，可以使用tlDebug()<<xxx来调用，详见tlDebug.hpp
tlArray为矩阵类，用来生成多维矩阵和矩阵相关操作
tlMath为opencv相关计算函数
tlGeometry为几何相关函数,包括坐点类tlPoint和多边形类tlPoly
tlGrating为光栅几何相关函数
tlTranslate为opencv图形转换函数


使用`using namespace tl;`来使用ToolBox里的函数

使用`#define USE_ALIAS`来使用别名:
Debug <= tlDebug
Point <= tlPoint

************************************/



/********************************************
OpenCV中的注意事项
1.Scalar的 / 重载是有问题的, 要用自己重写的div(),
2.Mat的 * / 为矩阵操作,要用Mat::mul(),Mat::div()
********************************************/




#endif