#include "unity_fixture.h"
#include "ring_buffer.h"

TEST_GROUP(ring_buffer_test);

TEST_SETUP(ring_buffer_test){

}

TEST_TEAR_DOWN(ring_buffer_test){

}

TEST(ring_buffer_test, ring_buffer_init_test){
	ring_buffer_t ring_buffer;
	ring_buffer_init(&ring_buffer);

	TEST_ASSERT_TRUE(0);
}
