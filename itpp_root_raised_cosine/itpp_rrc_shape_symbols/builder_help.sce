//Script to generate the help page for functions
mode(-1)
lines(0)

TOOLBOX_TITLE = "ITPP_toolbox"


help_lang_dir = get_absolute_file_path('builder_help.sce');

tbx_build_help(TOOLBOX_TITLE,help_lang_dir);
add_help_chapter('itpp_shape_symbols',help_lang_dir);
