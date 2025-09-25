#ifndef __CM_AGENT_WEBCONFIG_API__
#define __CM_AGENT_WEBCONFIG_API__

#include <stdint.h>
#include <stdlib.h>
#include <msgpack.h>
#include "cmagent_helpers.h"
#include "cmagent_param.h"
#define SUBDOC_COUNT           1

bool WebConfig_blob_handler(char *Encoded_data);
int  get_base64_decodedbuffer(char *pString, char **buffer, int *size);
void cmagentdocDestroy( cmagentdoc_t *gd );
void webConfigFrameworkInit();
                
#endif //__CM_AGENT_WEBCONFIG_API__