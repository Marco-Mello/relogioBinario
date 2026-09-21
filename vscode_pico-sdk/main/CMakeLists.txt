add_executable(pico_emb
                main.c
                ssd1306.c
                wifi_ntp.c)

set_target_properties(pico_emb PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})

# pull in common dependencies
target_link_libraries(pico_emb pico_stdlib hardware_i2c pico_cyw43_arch_lwip_threadsafe_background pico_lwip_sntp)

# create map/bin/hex/uf2 file etc.
pico_add_extra_outputs(pico_emb)
