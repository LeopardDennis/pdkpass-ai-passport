#include <assert.h>
#include <string.h>
#include "pdkpass_wifi_form.h"

int main(void)
{
    char ssid[33];
    char password[65];
    const char valid[] = "ssid=Pit+Lane&password=fast%402026";
    assert(pdkpass_wifi_form_parse(valid, strlen(valid), ssid, sizeof(ssid),
                                    password, sizeof(password)));
    assert(strcmp(ssid, "Pit Lane") == 0);
    assert(strcmp(password, "fast@2026") == 0);

    const char open[] = "ssid=Guest&password=";
    assert(pdkpass_wifi_form_parse(open, strlen(open), ssid, sizeof(ssid),
                                    password, sizeof(password)));
    assert(password[0] == '\0');

    const char short_password[] = "ssid=Home&password=short";
    assert(!pdkpass_wifi_form_parse(short_password, strlen(short_password),
                                     ssid, sizeof(ssid), password,
                                     sizeof(password)));
    const char malformed[] = "ssid=Home%ZZ&password=longenough";
    assert(!pdkpass_wifi_form_parse(malformed, strlen(malformed), ssid,
                                     sizeof(ssid), password,
                                     sizeof(password)));
    const char duplicate[] = "ssid=One&ssid=Two&password=longenough";
    assert(!pdkpass_wifi_form_parse(duplicate, strlen(duplicate), ssid,
                                     sizeof(ssid), password,
                                     sizeof(password)));
    return 0;
}
