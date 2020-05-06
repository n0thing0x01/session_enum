// ConsoleApplication28.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//

#include <iostream>
// ConsoleApplication25.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//

#include <iostream>

#ifndef UNICODE
#define UNICODE
#endif
#pragma comment(lib, "Netapi32.lib")
#pragma warning(disable:4996)
#include <stdio.h>
#include <assert.h>
#include <windows.h> 
#include <lm.h>
#include <ctime>

int session_enum(LPTSTR pszServerName) {
    NET_API_STATUS nStatus;
    LPSESSION_INFO_10 pBuf = NULL;
    LPSESSION_INFO_10 pTmpBuf;
    DWORD dwLevel = 10;
    DWORD dwPrefMaxLen = MAX_PREFERRED_LENGTH;
    DWORD dwEntriesRead = 0;
    DWORD dwTotalEntries = 0;
    DWORD dwResumeHandle = 0;
    DWORD i;
    DWORD dwTotalCount = 0;
    //LPTSTR pszServerName = NULL;
    do // begin do
    {
        nStatus = NetSessionEnum(pszServerName,
            NULL,
            NULL,
            dwLevel,
            (LPBYTE*)&pBuf,
            dwPrefMaxLen,
            &dwEntriesRead,
            &dwTotalEntries,
            &dwResumeHandle);
        //
        // If the call succeeds,
        //
        if ((nStatus == NERR_Success) || (nStatus == ERROR_MORE_DATA))
        {
            if ((pTmpBuf = pBuf) != NULL)
            {
                //
                // Loop through the entries.
                //
                for (i = 0; (i < dwEntriesRead); i++)
                {
                    assert(pTmpBuf != NULL);

                    if (pTmpBuf == NULL)
                    {
                        fprintf(stderr, "An access violation has occurred\n");
                        break;
                    }
                    //
                    // Print the retrieved data. 
                    //

                    //sprintf(url, "%ws -> %ws\n", pTmpBuf->sesi10_cname,pTmpBuf->sesi10_username);


                    SYSTEMTIME sys;
                    GetLocalTime(&sys);
                    char current_time[64] = { NULL };
                    sprintf(current_time, "%4d-%02d-%02d %02d:%02d:%02d ", sys.wYear, sys.wMonth, sys.wDay, sys.wHour, sys.wMinute, sys.wSecond);




                    printf("[%s]  [%ws]  [%ws]  [%ws]\n", current_time, pszServerName, pTmpBuf->sesi10_cname, pTmpBuf->sesi10_username);
                    /*
                    wprintf(L"\n\tClient: %s\n", pTmpBuf->sesi10_cname);
                    wprintf(L"\tUser:   %s\n", pTmpBuf->sesi10_username);
                    printf("\tActive: %d\n", pTmpBuf->sesi10_time);
                    printf("\tIdle:   %d\n", pTmpBuf->sesi10_idle_time);
                    */
                    pTmpBuf++;
                    dwTotalCount++;
                }
            }
        }
        //
        // Otherwise, indicate a system error.
        //
        else
            fprintf(stderr, "A system error has occurred: %d\n", nStatus);
        //
        // Free the allocated memory.
        //
        if (pBuf != NULL)
        {
            NetApiBufferFree(pBuf);
            pBuf = NULL;
        }
    }
    // 
    // Continue to call NetSessionEnum while 
    //  there are more entries. 
    // 
    while (nStatus == ERROR_MORE_DATA); // end do

    // Check again for an allocated buffer.
    //
    if (pBuf != NULL)
        NetApiBufferFree(pBuf);
    return 0;
}
int wmain(int argc, wchar_t* argv[])
{



    //
    // Check command line arguments.
    //

    if (argc == 1) {
        printf("\nUsing:\n\t session_enum.exe \\\\dc1 \\\\dc2 \n");
        return 0;
    }

    while (true)
    {
        for (size_t i = 0; i < argc; i++)
        {
            if (i == 0) {
                continue;
            }
            //printf("%ws\n",argv[i]);
            session_enum(argv[i]);
        }
        Sleep(5000);
    }




    return 0;
}
