#include "helper.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "router_globals.h"

esp_err_t downloadStatic(httpd_req_t *req, const char *fileStart, const size_t fileSize)
{
    httpd_resp_set_hdr(req, "Cache-Control", "max-age=31536000");

    setCloseHeader(req);

    return httpd_resp_send(req, fileStart, fileSize);
}

void setCloseHeader(httpd_req_t *req)
{
    const char *field = "Connection";
    const char *value = "close";
    httpd_resp_set_hdr(req, field, value);
}

void fillParamArray(char *buf, char *argv[], char *ssidKey, char *passKey)
{
    char ssidParam[32];
    char passParam[64];

    if (httpd_query_key_value(buf, ssidKey, ssidParam, 32) == ESP_OK)
    {
        preprocess_string(ssidParam);
        ESP_LOGI(TAG, "Found URL query parameter => %s=%s", ssidKey, ssidParam);

        httpd_query_key_value(buf, passKey, passParam, 64);
        preprocess_string(passParam);
        ESP_LOGI(TAG, "Found URL query parameter => %s=%s", passKey, passParam);
        strcpy(argv[1], ssidParam);
        strcpy(argv[2], passParam);
    }
}

void setApByQuery(char *buf, nvs_handle_t nvs)
{
    int argc = 3;
    char *argv[argc];
    argv[0] = "set_ap";
    fillParamArray(buf, argv, "ap_ssid", "ap_password");
    nvs_set_str(nvs, "ap_ssid", argv[1]);
    nvs_set_str(nvs, "ap_passwd", argv[2]);
}

void setStaByQuery(char *buf, nvs_handle_t nvs)
{
    int argc = 3;
    char *argv[argc];
    argv[0] = "set_sta";
    fillParamArray(buf, argv, "ssid", "password");
    nvs_set_str(nvs, "ssid", argv[1]);
    nvs_set_str(nvs, "passwd", argv[2]);
}
