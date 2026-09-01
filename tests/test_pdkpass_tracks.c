#include "pdkpass_tracks.h"

#include <assert.h>
#include <string.h>

static void assert_track(const char *circuit)
{
    pdkpass_track_geometry_t geometry = { 0 };
    assert(pdkpass_track_get(circuit, &geometry));
    assert(geometry.xy != NULL);
    assert(geometry.point_count == 49U);
    assert(geometry.xy[0] == geometry.xy[(geometry.point_count - 1U) * 2U]);
    assert(geometry.xy[1] == geometry.xy[(geometry.point_count - 1U) * 2U + 1U]);
    for (size_t i = 0; i < geometry.point_count; i++) {
        assert(geometry.xy[i * 2U] <= 191U);
        assert(geometry.xy[i * 2U + 1U] <= 60U);
    }
}

int main(void)
{
    static const char *circuits[] = {
        "MELBOURNE", "SHANGHAI", "SUZUKA", "MIAMI", "MONTREAL", "MONACO",
        "BARCELONA", "SPIELBERG", "SILVERSTONE", "SPA-FRANCORCHAMPS",
        "HUNGARORING", "ZANDVOORT", "MONZA", "MADRING", "BAKU", "SEPANG",
        "MARINA BAY", "COTA", "MEXICO CITY", "INTERLAGOS", "LAS VEGAS",
        "LUSAIL", "YAS MARINA",
    };
    for (size_t i = 0; i < sizeof(circuits) / sizeof(circuits[0]); i++) {
        assert_track(circuits[i]);
    }

    pdkpass_track_geometry_t madring;
    pdkpass_track_geometry_t madrid;
    assert(pdkpass_track_get("MADRING", &madring));
    assert(pdkpass_track_get("MADRID", &madrid));
    assert(madring.xy == madrid.xy);

    pdkpass_track_geometry_t unknown = { 0 };
    assert(!pdkpass_track_get("UNKNOWN", &unknown));
    assert(!pdkpass_track_get(NULL, &unknown));
    assert(!pdkpass_track_get("MONZA", NULL));
    return 0;
}
