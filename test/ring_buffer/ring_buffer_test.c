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

	TEST_ASSERT_FALSE(ring_buffer_is_full(&ring_buffer));
	TEST_ASSERT_TRUE(ring_buffer_is_empty(&ring_buffer));
}

TEST(ring_buffer_test, ring_buffer_get_when_is_empty_test){
	ring_buffer_t ring_buffer;
        ring_buffer_init(&ring_buffer);
	uint8_t data;

	TEST_ASSERT_FALSE(ring_buffer_get(&ring_buffer, &data));
}

TEST(ring_buffer_test, ring_buffer_put_get_test){
	uint8_t data_put = 0xFF;
	uint8_t data_get = 0x00;
	ring_buffer_t ring_buffer;

	ring_buffer_init(&ring_buffer);
	ring_buffer_put(&ring_buffer, &data_put);

	TEST_ASSERT_TRUE(ring_buffer_get(&ring_buffer, &data_get));
	TEST_ASSERT_EQUAL_INT8(data_put, data_get);
}

TEST(ring_buffer_test, ring_buffer_put_get_return_values_in_order_test){
	uint8_t data_put = 0x00;
	uint8_t data_get = 0x00;

	ring_buffer_t ring_buffer;
	ring_buffer_init(&ring_buffer);

	for(uint8_t i = 0; i < RING_BUFFER_SIZE - 1; i++){
		ring_buffer_put(&ring_buffer, &data_put);
		data_put++;
	}

	TEST_ASSERT_TRUE(ring_buffer_is_full(&ring_buffer));

	for(uint8_t i = 0; i < RING_BUFFER_SIZE - 1; i++){
		ring_buffer_get(&ring_buffer, &data_get);
		TEST_ASSERT_EQUAL_INT8(data_get, i);
	}

	TEST_ASSERT_TRUE(ring_buffer_is_empty(&ring_buffer));
}


TEST(ring_buffer_test, ring_buffer_is_full_test){
	ring_buffer_t ring_buffer;
	uint8_t data = 0xAA;

	for(uint16_t i = 0; i < RING_BUFFER_SIZE + 1; i++){
		ring_buffer_put(&ring_buffer, &data);	
	}

	TEST_ASSERT_TRUE(ring_buffer_is_full(&ring_buffer));
	TEST_ASSERT_FALSE(ring_buffer_is_empty(&ring_buffer));
}
