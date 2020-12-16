#pragma once

#include <cstdio>
#include <cstdint>
#include <cstring>

inline const char* ccBaseFileName(const char* aFullFileName, int aSize)
{
#if true
    // 9, 12 is arbitrary value
    if(aSize < 12)
    {
        return aFullFileName;
    }

    const char* file = aFullFileName + aSize - 9;
    for(; file > aFullFileName; file--)
    {
        if('/' == *file)
        {
            return file + 1;
        }
    }
#endif
    return aFullFileName;
}

#if defined(__FILENAME__)
#undef __FILENAME__
#endif
#define __FILENAME__ ccBaseFileName(__FILE__, sizeof(__FILE__))

#define LOG_HEADER_STR "[%s(%s:%i)] "
#define LOG_HEADER_FMT __FUNCTION__, __FILENAME__, __LINE__

#define ccLog(fmt, args...) printf(LOG_HEADER_STR fmt"\n",LOG_HEADER_FMT,##args)
#define ccTrace(fmt, args...) printf(LOG_HEADER_STR fmt"\n",LOG_HEADER_FMT,##args)
#define ccInfo(fmt, args...) printf(LOG_HEADER_STR fmt"\n",LOG_HEADER_FMT,##args)
#define ccWarning(fmt, args...) printf(LOG_HEADER_STR fmt"\n",LOG_HEADER_FMT,##args)
#define ccError(fmt, args...) printf(LOG_HEADER_STR fmt"\n",LOG_HEADER_FMT,##args)
#define ccFtl(fmt, args...) printf(LOG_HEADER_STR fmt"\n",LOG_HEADER_FMT,##args)

#define ccDebug ccLog

class __CcDebugPosition
{
private:
    const static uint32_t mFileLen = 32;
    const static uint32_t mFuncLen = 32;
public:
    __CcDebugPosition(const char *file, const char *func, uint32_t line)
        : mLine(line)
    {
        strncpy(mFunc, func, mFuncLen-1);
        strncpy(mFile, basename(file), mFileLen-1);
        mFunc[mFuncLen-1]='\0';
        mFile[mFileLen-1]='\0';
    }
    __CcDebugPosition()
    {
        mFile[0]    = '\0';
        mFunc[0]    = '\0';
        mLine       = 0;
    };

    void set(const char *file, const char *func, uint32_t line)
    {
        strncpy(mFunc, func, mFuncLen-1);
        strncpy(mFile, basename(file), mFileLen-1);
        mFunc[mFuncLen-1]='\0';
        mFile[mFileLen-1]='\0';
        mLine = line;
    }
public:
    char        mFile[mFileLen];
    char        mFunc[mFuncLen];
    uint32_t    mLine;
};

class CcTrace
{
private:
    __CcDebugPosition mPosition;
public:
    CcTrace() {};
    CcTrace(const char *func, const char *file, const int line)
    {
        mPosition.set(file, func, line);
    };

    ~CcTrace()
    {
        ccLog(LOG_HEADER_STR"exit.\n", mPosition.mFunc,mPosition.mFile,mPosition.mLine);
    };
};

#define ccFTrace() \
    do{\
        ccLog(LOG_HEADER_STR "entered.\n", LOG_HEADER_FMT); \
    }while(0);\
    CcTrace _trace(__FUNCTION__, __FILENAME__, __LINE__)

#define ccFTraceWithLog(fmt, args...) \
    do{\
        ccLog(LOG_HEADER_STR "entered" ":" fmt"\n",LOG_HEADER_FMT, ##args); \
    }while(0);\
    CcTrace _Trace(__FUNCTION__, __FILENAME__, __LINE__)

namespace mav
{
    enum Status
    {
        OK,
        PENDING,
        ERROR
    };
}
