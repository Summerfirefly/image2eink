project('image2eink', 'c', license: 'GPL-3.0', version: '1.0.0',
    default_options: [
        'c_std=gnu11',
        'warning_level=3'
    ]
)

add_project_link_arguments(
    '-lm',
    language: ['c']
)

srcfiles = files(
    'main.c',
    'src/i2e_image_write.c',
    'src/i2e_image_handler.c',
    'src/i2e_pallette.c',
    'src/i2e_utils.c'
)

executable('image2eink', srcfiles, include_directories: ['include'])
