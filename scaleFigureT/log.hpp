#ifndef LOG_H
#define LOG_H

#include <QCoreApplication>
#include <QDebug>


//#define OUTPUT_LOG //输出信息输出到输出控制台还是日志

#ifdef  OUTPUT_LOG
#define  outPut  qOut//release版
#else
#define  outPut  qDebug()//debug版
#endif

//日志文件名称
#define LOG_FILE     QCoreApplication::applicationDirPath()/*strFilePath*/ + "/logger.txt"
#define qOut qDebug()<<__FUNCTION__<<"["<<__LINE__<<"]"
#define xErrPrint  qCritical()<<__FUNCTION__<<"["<<__LINE__<<"]"

//QString strFilePath;
//void getCurrFilePath()
//{
//    strFilePath = QCoreApplication::applicationDirPath();//应用程序可执行文件的路径
//}

#endif // LOG_H


