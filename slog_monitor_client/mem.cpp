/*** xrkmonitor license ***

   Copyright (c) 2019 by rockdeng

   Licensed under the Apache License, Version 2.0 (the "License");
   you may not use this file except in compliance with the License.
   You may obtain a copy of the License at

       http://www.apache.org/licenses/LICENSE-2.0

   Unless required by applicable law or agreed to in writing, software
   distributed under the License is distributed on an "AS IS" BASIS,
   WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
   See the License for the specific language governing permissions and
   limitations under the License.


   字符云监控(xrkmonitor) 开源版 (c) 2019 by rockdeng
   当前版本：v1.0
   使用授权协议： apache license 2.0

   云版本主页：http://xrkmonitor.com

   云版本为开源版提供永久免费告警通道支持，告警通道支持短信、邮件、
   微信等多种方式，欢迎使用

   模块 slog_monitor_client 功能:
        用于上报监控系统自身相关模块的监控点数据，内部使用易于扩展的
        protobuf 协议

****/

#ifndef __STDC_FORMAT_MACROS
#define __STDC_FORMAT_MACROS 1
#endif

#include <stdlib.h>
#include <inttypes.h>
#include <errno.h>
#include <unistd.h>
#include <string.h>
#include <stdio.h>
#include <sys/shm.h>
#include <supper_log.h>
#include <mt_report.h>
#include "mem.h"

int GetMemInfo(TMemInfo &mem)
{
	FILE *fp = popen("/bin/cat /proc/meminfo | awk \'{if(NF==3) print $0 }\'", "r");
	if(fp == NULL) {
		return SLOG_ERROR_LINE;
	}

	char sMemField[64] = {0}, sMemUnit[16] = {0};
	uint32_t dwValue = 0;
	while( fscanf(fp, "%s%u%s", sMemField, &dwValue, sMemUnit) == 3) {
		if(0 == strcasecmp(sMemField, "MemTotal:"))
		{
			mem.dwMemTotal = dwValue;
			strncpy(mem.szUnit, sMemUnit, sizeof(mem.szUnit));
		}
		else if(0 == strcasecmp(sMemField, "MemFree:"))
			mem.dwMemFree = dwValue;
		else if(0 == strcasecmp(sMemField, "Buffers:"))
			mem.dwBuffers = dwValue;
		else if(0 == strcasecmp(sMemField, "Cached:"))
			mem.dwCached = dwValue;
		else if(0 == strcasecmp(sMemField, "MemAvailable:"))
			mem.dwMemAvailable = dwValue;
		else if(0 == strcasecmp(sMemField, "SwapTotal:"))
			mem.dwSwapTotal = dwValue;
		else if(0 == strcasecmp(sMemField, "SwapFree:"))
			mem.dwSwapFree = dwValue;
		else if(0 == strcasecmp(sMemField, "Dirty:"))
			mem.dwDirty = dwValue;
		else if(0 == strcasecmp(sMemField, "Mapped:"))
			mem.dwMapped = dwValue;
	}
	pclose(fp);
	return 0;
}

