add_subdirectory(tracker)

# Make sure the compiler can find include files from our tracker library. 
include_directories (./tracker) 


# Make sure the linker can find the tracker library once it is built. 
link_directories (${TP_Interface_AR_BINARY_DIR}/lib) 

