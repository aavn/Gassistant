#ifndef _REST_UTIL_H
#define _REST_UTIL_H

#define GET_CMD 1
#define PUT_CMD 2

RestResponse sendRestService(int httpVerb, const char * url ,const char * data);
RestResponse updateRestService(int httpVerb, const char * url ,const char * data);
void syncTime();
RestResponse getResponseData(String response);
#endif

