#include "pdkpass_wifi_form.h"

#include <string.h>

static int hex_value(char c)
{
    if (c >= '0' && c <= '9') return c - '0';
    if (c >= 'a' && c <= 'f') return c - 'a' + 10;
    if (c >= 'A' && c <= 'F') return c - 'A' + 10;
    return -1;
}

static bool decode_value(const char *src, size_t length,
                         char *dst, size_t dst_size)
{
    size_t used = 0;
    if (!dst || dst_size == 0) return false;

    for (size_t i = 0; i < length; i++) {
        unsigned char value = (unsigned char)src[i];
        if (value == '+') {
            value = ' ';
        } else if (value == '%') {
            if (i + 2 >= length) return false;
            int high = hex_value(src[i + 1]);
            int low = hex_value(src[i + 2]);
            if (high < 0 || low < 0) return false;
            value = (unsigned char)((high << 4) | low);
            i += 2;
        }
        if (value == '\0' || used + 1 >= dst_size) return false;
        dst[used++] = (char)value;
    }
    dst[used] = '\0';
    return true;
}

bool pdkpass_wifi_form_parse(const char *body, size_t body_len,
                             char *ssid, size_t ssid_size,
                             char *password, size_t password_size)
{
    bool have_ssid = false;
    bool have_password = false;
    size_t offset = 0;

    if (!body || !ssid || !password || body_len == 0) return false;
    ssid[0] = '\0';
    password[0] = '\0';

    while (offset < body_len) {
        size_t end = offset;
        while (end < body_len && body[end] != '&') end++;
        size_t equals = offset;
        while (equals < end && body[equals] != '=') equals++;
        if (equals == end) return false;

        size_t key_length = equals - offset;
        const char *value = body + equals + 1;
        size_t value_length = end - equals - 1;
        if (key_length == 4 && memcmp(body + offset, "ssid", 4) == 0) {
            if (have_ssid || !decode_value(value, value_length,
                                            ssid, ssid_size)) return false;
            have_ssid = true;
        } else if (key_length == 8 &&
                   memcmp(body + offset, "password", 8) == 0) {
            if (have_password || !decode_value(value, value_length,
                                                password, password_size)) return false;
            have_password = true;
        }
        offset = end + 1;
    }

    size_t ssid_length = strlen(ssid);
    size_t password_length = strlen(password);
    return have_ssid && have_password && ssid_length >= 1 && ssid_length <= 32 &&
           (password_length == 0 || (password_length >= 8 && password_length <= 63));
}
