// Builder gateway function for CRC Code decoding

function builder_gw_cpp()
    WITHOUT_AUTO_PUTLHSVAR = %t;
    tbx_build_gateway("skeleton_cpp", ..
    ["itpp_crc_decode","itpp_crc_decode"], ..
    ["itpp_crc_decode.cpp"], ..
    get_absolute_file_path("builder_gateway_cpp.sce"), [], "-litpp");

endfunction

builder_gw_cpp();
clear builder_gw_cpp; // remove builder_gw_cpp on stack
