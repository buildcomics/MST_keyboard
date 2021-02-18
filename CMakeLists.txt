add_executable(main
        main.c
        )

# Pull in our pico_stdlib which aggregates commonly used features
target_link_libraries(main pico_stdlib)

# create map/bin/hex/uf2 file etc.
pico_add_extra_outputs(main)

# add url via pico_set_program_url
example_auto_set_url(main)
