#include "test.hpp"
#include "Allocator.hpp"
//#include "new_spv.hpp"
#include "HashMap.hpp"

using namespace Sol;

int main() {
    MemoryConfig config = {};
    MemoryService::instance()->init(&config);
    Test::Suite::instance()->init(false);

    //Spv spv;
    ///spv.run_tests();
    //glTFTest gltf;
    //gltf.run_tests();
    run_hashmap_tests();


    Test::Suite::instance()->kill();
    MemoryService::instance()->shutdown();
    return 0;
}
