# TODO SlugVk-lib-common

1. Complete Spv::Serialized class, Serialized::write() function. In its current state it is 
cumbersome to extract information from. And its ratio of memory footprint to useful info 
is poor. It will be fine after serialization tho, as all the memory is just in temporary 
storage (SCRATCH allocation).
