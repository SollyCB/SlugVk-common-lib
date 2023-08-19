#include "test.hpp"
#include "Allocator.hpp"
#include "Spv.hpp"
#include "HashMap.hpp"
#include "glTF.hpp"
#include "Print.hpp"

using namespace Sol;

int main() {
    MemoryConfig config = {};
    MemoryService::instance()->init(&config);
    Test::Suite::instance()->init(false);

	HashMap<int, int>::run_tests();
	Spv::run_tests();
	glTF::run_tests();
    print_tests();

    Test::Suite::instance()->kill();
    MemoryService::instance()->shutdown();
    return 0;
}
