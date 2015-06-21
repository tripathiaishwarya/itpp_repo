//Script to generate the help page for functions
mode(-1)
lines(0)

TOOLBOX_TITLE = "ITPP_toolbox"

tbx_build_help(TOOLBOX_TITLE,get_absolute_file_path("builder_help.sce"));
add_help_chapter('itpp_psk_demod',get_absolute_file_path("builder_help.sce"));


clear TOOLBOX_TITLE;
