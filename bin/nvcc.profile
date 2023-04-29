
TOP              = $(_HERE_)/..

PATH            += $(TOP)/extools/bin;$(TOP)/open64/bin;$(_HERE_);$(TOP)/lib;

INCLUDES        +=  "-I$(TOP)/include" "-I$(TOP)/include/cudart" $(_SPACE_)

LIBRARIES        =+ $(_SPACE_) "/LIBPATH:$(TOP)/lib/$(_WIN_PLATFORM_)" cudart.lib

CUDAFE_FLAGS    +=
OPENCC_FLAGS    +=
PTXAS_FLAGS     +=
