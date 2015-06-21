// Builder gateway function for Sequence Interleaver

function builder_gw_cpp()
    WITHOUT_AUTO_PUTLHSVAR = %t;
    tbx_build_gateway("skeleton_cpp", ..
    ["itpp_seq_interleave","itpp_seq_interleave"], ..
    ["itpp_seq_interleave.cpp"], ..
    get_absolute_file_path("builder_gateway_cpp.sce"), [], "-litpp");

endfunction

builder_gw_cpp();
clear builder_gw_cpp; // remove builder_gw_cpp on stack
