# name anything you want
add_executable(term)
add_executable(test_ps2)
add_executable(test_vga)
add_executable(test_vga_pins)

# must match with pio filename and executable name from above
pico_generate_pio_header(term ${CMAKE_CURRENT_LIST_DIR}/hsync.pio)
pico_generate_pio_header(term ${CMAKE_CURRENT_LIST_DIR}/vsync.pio)
pico_generate_pio_header(term ${CMAKE_CURRENT_LIST_DIR}/rgb.pio)
pico_generate_pio_header(term ${CMAKE_CURRENT_LIST_DIR}/ps2.pio)

# must match with executable name and source file names
target_sources(term PRIVATE 
	term.c
	vga16_graphics.c
	ps2.c
	scancodes.c
	host.c
	proto.c
)

# must match with executable name
target_link_libraries(term PRIVATE 
	pico_stdlib 
	pico_bootsel_via_double_reset 
	hardware_pio 
	hardware_dma 
	hardware_adc 
	hardware_sync
	hardware_irq
	pico_multicore
)
# enable usb output, disable uart output
pico_enable_stdio_usb(term 1)
pico_enable_stdio_uart(term 0)
pico_add_extra_outputs(term)


# PS2 test program

target_sources(test_ps2 PRIVATE 
	test_ps2.c
	ps2.c
)

target_link_libraries(test_ps2 PRIVATE 
	pico_stdlib 
	pico_bootsel_via_double_reset 
	hardware_irq
)

pico_enable_stdio_usb(test_ps2 1)
pico_enable_stdio_uart(test_ps2 0)
pico_add_extra_outputs(test_ps2)

# VGA pin test program

target_sources(test_vga_pins PRIVATE 
	test_vga_pins.c
)

target_link_libraries(test_vga_pins PRIVATE 
	pico_stdlib 
	pico_bootsel_via_double_reset 
)

pico_enable_stdio_usb(test_vga_pins 1)
pico_enable_stdio_uart(test_vga_pins 0)
pico_add_extra_outputs(test_vga_pins)



# VGA test program

pico_generate_pio_header(test_vga ${CMAKE_CURRENT_LIST_DIR}/hsync.pio)
pico_generate_pio_header(test_vga ${CMAKE_CURRENT_LIST_DIR}/vsync.pio)
pico_generate_pio_header(test_vga ${CMAKE_CURRENT_LIST_DIR}/rgb.pio)
pico_generate_pio_header(test_vga ${CMAKE_CURRENT_LIST_DIR}/ps2.pio)


target_sources(test_vga PRIVATE 
	test_vga.c
	vga16_graphics.c
)

target_link_libraries(test_vga PRIVATE 
	pico_stdlib 
	pico_bootsel_via_double_reset 
	hardware_pio 
	hardware_dma 
	hardware_adc 
	hardware_sync
	hardware_irq
)

pico_enable_stdio_usb(test_vga 1)
pico_enable_stdio_uart(test_vga 0)
pico_add_extra_outputs(test_vga)
pico_add_extra_outputs(test_vga)


add_compile_options(-Ofast)
add_compile_options(-Ofast)
