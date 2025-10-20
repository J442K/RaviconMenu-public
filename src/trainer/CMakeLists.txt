# Simple Trainer ASI - Basic trainer that can be loaded by our ASI Loader
add_library(SimpleTrainer SHARED
    SimpleTrainer.cpp
)

# Set output name to .asi
set_target_properties(SimpleTrainer PROPERTIES
    OUTPUT_NAME "SimpleTrainer"
    SUFFIX ".asi"
)

# Link with Windows libraries
target_link_libraries(SimpleTrainer
    kernel32
    user32
)

# Set as DLL
set_target_properties(SimpleTrainer PROPERTIES
    PREFIX ""
    SUFFIX ".asi"
)














