#include "dialog.h"
#include "log.hpp"
#include <QMutex>
#include <QDateTime>
#include <QFile>

#include <QApplication>
void MessageTypePut(QtMsgType type, const QMessageLogContext &context, const QString &msg);
int main(int argc, char *argv[])
{
#ifdef OUTPUT_LOG
    qInstallMessageHandler(MessageTypePut);
#endif
    QApplication a(argc, argv);
    Dialog w;
    w.show();
    return a.exec();
}

void MessageTypePut(QtMsgType type, const QMessageLogContext &context, const QString &msg)
{
//    getCurrFilePath();
    static QMutex mutex;
    mutex.lock();
    QString text;
   switch(type)
   {
   case QtDebugMsg:
       text = QString("Debug:");
       break;

   case QtWarningMsg:
       text = QString("Warning:");
       break;

   case QtCriticalMsg:
       text = QString("Critical:");
       break;

   case QtFatalMsg:
       text = QString("Fatal:");
       break;
   default:
       break;
   }
   //日志写到文件
   QString current_date_time = QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss");
   QString message = QString("%1 %2%3").arg(current_date_time).arg(text).arg(msg);
   QFile file(LOG_FILE);
   file.open(QIODevice::WriteOnly | QIODevice::Append);
   QTextStream text_stream(&file);
   text_stream << message << "\r\n";
   file.flush();//将缓冲的数据刷新到文件
   file.close();
   mutex.unlock();
}
