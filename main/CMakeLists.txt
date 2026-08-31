idf_component_register(
    SRCS "main.c"
         "ui_pixel.c"
         "ui_pixel_math.c"
         "pdkpass_data.c"
         "pdkpass_model.c"
         "pdkpass_ui.c"
    INCLUDE_DIRS "."
    REQUIRES bsp esp_hw_support
)
