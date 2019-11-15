/*
 * Copyright (c) 2019, The Linux Foundation. All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are
 * met:
 *    * Redistributions of source code must retain the above copyright
 *      notice, this list of conditions and the following disclaimer.
 *    * Redistributions in binary form must reproduce the above
 *      copyright notice, this list of conditions and the following
 *      disclaimer in the documentation and/or other materials provided
 *      with the distribution.
 *    * Neither the name of The Linux Foundation nor the names of its
 *      contributors may be used to endorse or promote products derived
 *      from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED "AS IS" AND ANY EXPRESS OR IMPLIED
 * WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NON-INFRINGEMENT
 * ARE DISCLAIMED.  IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS
 * BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR
 * BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
 * WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE
 * OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN
 * IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#include "adspmsgd_apps.h"
#include "remote.h"

#include <stdio.h>
#include <android/log.h>

#define LOG_NODE_SIZE           258
#define LOG_FILENAME_SIZE       30
#define LOG_MSG_SIZE            LOG_NODE_SIZE - LOG_FILENAME_SIZE - \
    sizeof(enum adspmsgd_apps_Level) - (3*sizeof(unsigned short))

typedef struct __attribute__((packed))
{
    enum adspmsgd_apps_Level level;
    unsigned short line;
    unsigned short thread_id;
    unsigned short asid;
    char str[LOG_MSG_SIZE];
    char file [LOG_FILENAME_SIZE];
} LogNode;

static inline android_LogPriority convert_level_to_android_priority(
    enum adspmsgd_apps_Level level)
{
    switch (level) {
        case LOW:
            return ANDROID_LOG_DEBUG;
        case MEDIUM:
            return ANDROID_LOG_INFO;
        case HIGH:
            return ANDROID_LOG_WARN;
        case ERROR:
            return ANDROID_LOG_ERROR;
        case FATAL:
            return ANDROID_LOG_FATAL;
        default:
            return ANDROID_LOG_DEFAULT;
        }
}

int adspmsgd_apps_log(const unsigned char *log_message_buffer,
                      int log_message_bufferLen)
{
    LogNode *logMessage = (LogNode *)log_message_buffer;
    while (  (log_message_bufferLen  > 0) && (logMessage != NULL)) {
        __android_log_print(convert_level_to_android_priority(logMessage->level),
            "adsprpc", "%s:%d:0x%x:%d:%s", logMessage->file, logMessage->line,
            logMessage->thread_id, logMessage->asid, logMessage->str);
        logMessage++;
        log_message_bufferLen -= sizeof(LogNode);
    };

    return 0;
}
