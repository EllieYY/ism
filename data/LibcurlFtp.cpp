#include "LibcurlFtp.h"
#include <iostream>
#include <sys/stat.h>
#include <sys/types.h>
#include <fstream>
#include <QDebug>
#include <QFile>
using namespace std;



struct FtpFile {
    const char *filename;
    FILE *stream;
};

LibcurlFtp::LibcurlFtp(QObject *parent) : QObject(parent)
{
    // 设置协议
    m_pUrl.setScheme("ftp");
}


// QHash<int, int> fileFilterInfo : 文件类型，文件版本
bool LibcurlFtp::Getlist(QString &remotePath, QString &listFileName, QString &localPath, QHash<int, int> fileFilterInfo)
{
    // 格式转换
    QByteArray remoteArray = remotePath.toUtf8();
    const char* remotePathStr = remoteArray.constData();

    QByteArray pathFileArray = (localPath + listFileName).toUtf8();
    const char* pathFile = pathFileArray.constData();

    QByteArray localArray = localPath.toUtf8();
    const char* localPathStr = localArray.constData();

    // curl 初始化 + 参数设置
    CURL *curl;
    CURLcode res;
    FILE *ftpfile;

    /* local file name to store the file as */
    ftpfile = fopen(pathFile, "wb"); /* b is binary, needed on win32 */
    curl = curl_easy_init();

    if(curl) {
        /* Get a file listing from sunet */
        // 路径包含用户名和密码
        curl_easy_setopt(curl, CURLOPT_URL, remotePathStr);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, ftpfile);
        res = curl_easy_perform(curl);
        /* Check for errors */

        if(res != CURLE_OK)
          fprintf(stderr, "curl_easy_perform() failed: %s\n",
                  curl_easy_strerror(res));

        /* always cleanup */
        curl_easy_cleanup(curl);
    }
    fclose(ftpfile);

    // 文件列表解析
    QString listFilePath = localPath + listFileName;
    QFile file(listFilePath);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        qDebug() << "[getFileList] cannot open file : " << listFilePath;
        return false;
    }

    QList<QString> fileList;  fileList.clear();
    QTextStream in(&file);
    while (!in.atEnd()) {
        QString line = in.readLine();

        QString fileName;
        QChar type;
        processLine(line, fileName, type);

        if (type == 'd') {
            continue;
        }
        fileList.append(fileName);
    }
    file.close();


    // 文件筛选 | 文件下载
    for (QString name : fileList) {
        if (!paramFileFilter(name, fileFilterInfo)) {
            continue;
        }

        // 文件下载
        if(ftpDownload(remotePath, name, localPath)) {
           if (m_callBack != NULL)
           m_callBack(localPath, name);
        } else {
           qDebug() << name << "download error.";
//           return false;
        }
    }

    return true;
}

// 文件目录行处理
void LibcurlFtp::processLine(QString line, QString& fileName, QChar& type)
{
    type = line[0];

    int index = line.lastIndexOf(" ");
    fileName = line.mid(index + 1);

    QString info = QString("[%1] %2").arg(type).arg(fileName);
    qDebug() << info;
}

// 文件筛选
// QHash<int, int> fileFilterInfo : 文件类型，文件版本
bool LibcurlFtp::paramFileFilter(QString fileName, QHash<int, int> fileFilterInfo)
{
    // TODO:文件过滤
    // 文件名：“PRM.”+参数类型（4位）+“.”+节点编码（4位）+“.”+文件序列号（6位）
    bool ok;
    int type = fileName.mid(4, 4).toInt(&ok, 16);
    int code = fileName.mid(9, 4).toInt(&ok, 16);




    return true;
}


bool LibcurlFtp::Download(fileDownloadedCallBack callBack, QString &remotpath, QString &myfloderlist, QString &localpath,
                          QHash<int, int> fileFilterInfo)
{
    m_callBack = callBack;
    if(Getlist(remotpath, myfloderlist, localpath, fileFilterInfo))
    {
        return false;
    }
    return true;
}



/* 文件下载 */
static size_t write_callback(void *buffer, size_t size, size_t nmemb, void *stream)
{
    struct FtpFile *out = (struct FtpFile *)stream;
    if(out && !out->stream) {
        /* open file for writing */
        out->stream = fopen(out->filename, "wb");
        if(!out->stream) {
            qDebug() << "write_callback failed.";
            return -1; /* failure, can't open file to write */
        }
    }

    qDebug() << "filename:" << out->filename;
    return fwrite(buffer, size, nmemb, out->stream);
}

int LibcurlFtp::ftpDownload(QString remotePath, QString fileName, QString localPath)
{
    QByteArray localArray = (localPath + fileName).toUtf8();
    const char* localFile = localArray.constData();

    QByteArray serverArray = (remotePath + fileName).toUtf8();
    const char* targetUrl = serverArray.constData();

    CURL *curl;
    CURLcode res;
    struct FtpFile ftpfile = {
        localFile, /* name to store the file as if succesful */
        NULL
    };

    curl_global_init(CURL_GLOBAL_DEFAULT);
    curl = curl_easy_init();
    if(curl) {
        /*
        * You better replace the URL with one that works! Note that we use an
        * FTP:// URL with standard explicit FTPS. You can also do FTPS:// URLs if
        * you want to do the rarer kind of transfers: implicit.
        */
        curl_easy_setopt(curl, CURLOPT_URL, targetUrl);
        /* Define our callback to get called when there's data to be written */
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_callback);
        /* Set a pointer to our struct to pass to the callback */
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &ftpfile);
        curl_easy_setopt(curl, CURLOPT_FTP_FILEMETHOD, CURLFTPMETHOD_NOCWD);
        /* We activate SSL and we require it for both control and data */
        curl_easy_setopt(curl, CURLOPT_USE_SSL, CURLUSESSL_ALL);

        /* Switch on full protocol/debug output */
        curl_easy_setopt(curl, CURLOPT_VERBOSE, 1L);

        res = curl_easy_perform(curl);
        if(CURLE_OK != res) {
             /* failed */
             logger()->error("curl_easy_perform() failed: %1, %2", res, curl_easy_strerror(res));
        }

        /* always cleanup */
        curl_easy_cleanup(curl);
    }

    /* close the local file */
    if(ftpfile.stream)
        fclose(ftpfile.stream);

    curl_global_cleanup();

//    emit downloadOk(localPath);

    return 0;
}


/* 文件上传 */
static size_t read_callback(void *ptr, size_t size, size_t nmemb, void *stream)
{
  curl_off_t nread;
  /* in real-world cases, this would probably get this data differently
     as this fread() stuff is exactly what the library already would do
     by default internally */
  size_t retcode = fread(ptr, size, nmemb, (FILE*)stream);

  nread = (curl_off_t)retcode;

  fprintf(stderr, "*** We read %" CURL_FORMAT_CURL_OFF_T
          " bytes from file\n", nread);
  return retcode;
}

int LibcurlFtp::ftpUpload(QString remotePath, QString fileName, QString localPath)
{    
    QByteArray localArray = (localPath + fileName).toUtf8();
    const char* localFile = localArray.constData();

    QByteArray serverArray = (remotePath + fileName).toUtf8();
    const char* targetUrl = serverArray.constData();

    CURL *curl;
    CURLcode res;
    FILE *hd_src;
    struct stat file_info;
    curl_off_t fsize;

    /* get the file size of the local file */
    if(stat(localFile, &file_info)) {
        logger()->error("Couldnt open '%s': %s\n", localFile, strerror(errno));
        return -1;
    }
    fsize = (curl_off_t)file_info.st_size;

    logger()->info("Local file size: %" CURL_FORMAT_CURL_OFF_T " bytes.\n", fsize);

    /* get a FILE * of the same file */
    hd_src = fopen(localFile, "rb");

    /* In windows, this will init the winsock stuff */
    curl_global_init(CURL_GLOBAL_ALL);
    /* get a curl handle */
    curl = curl_easy_init();
    if(curl) {
        /* build a list of commands to pass to libcurl */
        //        headerlist = curl_slist_append(headerlist, buf_1);
        //        headerlist = curl_slist_append(headerlist, buf_2);

        /* we want to use our own read function */
        curl_easy_setopt(curl, CURLOPT_READFUNCTION, read_callback);

        /* enable uploading */
        curl_easy_setopt(curl, CURLOPT_UPLOAD, 1L);

        /* specify target */
        curl_easy_setopt(curl, CURLOPT_URL, targetUrl);

        /* pass in that last of FTP commands to run after the transfer */
        //        curl_easy_setopt(curl, CURLOPT_POSTQUOTE, headerlist);

        /* now specify which file to upload */
        curl_easy_setopt(curl, CURLOPT_READDATA, hd_src);

        /* Set the size of the file to upload (optional).  If you give a *_LARGE
           option you MUST make sure that the type of the passed-in argument is a
           curl_off_t. If you use CURLOPT_INFILESIZE (without _LARGE) you must
           make sure that to pass in a type 'long' argument. */
        curl_easy_setopt(curl, CURLOPT_INFILESIZE_LARGE, (curl_off_t)fsize);

        /* Now run off and do what you've been told! */
        res = curl_easy_perform(curl);
        /* Check for errors */
        if(res != CURLE_OK) {
            logger()->error("curl_easy_perform() failed: %s\n", curl_easy_strerror(res));
        }

        /* always cleanup */
        curl_easy_cleanup(curl);
    }
    fclose(hd_src); /* close the local file */
    curl_global_cleanup();

//    emit uploadOk(url);
    return 0;
}


// 设置地址和端口
void LibcurlFtp::setHostPort(const QString &host, int port)
{
    m_pUrl.setHost(host);
    m_pUrl.setPort(port);
}

// 设置登录 FTP 服务器的用户名和密码
void LibcurlFtp::setUserInfo(const QString &userName, const QString &password)
{
    m_pUrl.setUserName(userName);
    m_pUrl.setPassword(password);
}



//#include "download.h"
//#include <iostream>
//using namespace std;
//int main()
//{
//    FileDownloadedCallBack callBack;
//    FtpDownloadder *dl=new FtpDownloadder("192.168.1.1","21","zheng","yun");
//    if(dl->Download(callBack))
//    {
//        cout<<"download over";
//    }
//    delete dl;
//    return 0;
//}








