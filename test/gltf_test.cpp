#include "test.hpp"
#include "gltf_test.hpp"
#include "glTF.hpp"

namespace Sol::Test {

void glTF_test1(bool skip) {
    glTF gltf;
    bool ok = glTF::get("test/test_1.gltf", &gltf);
    ASSERT(ok == true, "FAILED to read gltf file");
    TEST_EQ("nodes[3]_translation", gltf.nodes.nodes[3].translation[0], -17.7082f, skip);
    TEST_EQ("bufferviews[1]", gltf.buffer_views.views[1].byte_length, static_cast<uint32_t>(76768), skip);
    TEST_EQ("accessors[0]", gltf.accessors.accessors[0].type, glTF::Accessor::VEC3, skip);
    TEST_EQ("meshes[0].primitives[0]", gltf.meshes.meshes[0].primitives[0].indices, 21, skip);
    TEST_STR_EQ("images[0].uri", gltf.images.images[0].uri.cstr(), "duckCM.png", skip);
}

void glTFTest::run() {
    TEST_MODULE_BEGIN("glTF_1", true, false);
    glTF_test1(false);
    TEST_MODULE_END();
}
} // namespace Sol::Test
