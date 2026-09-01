#pragma once

#include <stdbool.h>
#include <stddef.h>

// Decode a bounded application/x-www-form-urlencoded Wi-Fi form. SSIDs must
// be 1..32 bytes; passwords must be empty for an open network or 8..63 bytes.
bool pdkpass_wifi_form_parse(const char *body, size_t body_len,
                             char *ssid, size_t ssid_size,
                             char *password, size_t password_size);
