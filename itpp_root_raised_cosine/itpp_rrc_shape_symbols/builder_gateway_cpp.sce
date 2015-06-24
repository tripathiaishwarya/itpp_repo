// Builder gateway function for Root Raised Cosine shape_symbols

function builder_gw_cpp()
    WITHOUT_AUTO_PUTLHSVAR = %t;
    tbx_build_gateway("skeleton_cpp", ..
    ["rrc_shape_symbols","itpp_rrc_shape_symbols"], ..
    ["itpp_rrc_shape_symbols.cpp"], ..
    get_absolute_file_path("builder_gateway_cpp.sce"), [], "-litpp");

endfunction

builder_gw_cpp();
clear builder_gw_cpp; // remove builder_gw_cpp on stack
