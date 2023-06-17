#include "unity_fixture.h"
#include "ring_buffer.h"
#include <stdio.h>

typedef struct{
	uint8_t var1;
	uint16_t var2;
	uint32_t var3;
	char str[10];
}test_t;

size_t item_size = sizeof(test_t);	


TEST_GROUP(ring_buffer_test);

TEST_SETUP(ring_buffer_test){
}

TEST_TEAR_DOWN(ring_buffer_test){
}


TEST(ring_buffer_test, ring_buffer_init_test){
	ring_buffer_t* ring_buffer = ring_buffer_init(item_size);

	TEST_ASSERT_NOT_NULL(ring_buffer);
	TEST_ASSERT_FALSE(ring_buffer_is_full(ring_buffer));
	TEST_ASSERT_TRUE(ring_buffer_is_empty(ring_buffer));

	ring_buffer_deinit(ring_buffer);
}

TEST(ring_buffer_test, ring_buffer_get_when_is_empty_test){
	ring_buffer_t* ring_buffer = ring_buffer_init(item_size);

	test_t data;
	TEST_ASSERT_FALSE(ring_buffer_get(ring_buffer, &data));

	ring_buffer_deinit(ring_buffer);
}

TEST(ring_buffer_test, ring_buffer_put_get_test){
	ring_buffer_t* ring_buffer = ring_buffer_init(item_size);

	test_t data_put = {.var1 = 1, .var2 = 2, .var3 = 3, .str = "string"};
	test_t data_get;

	ring_buffer_put(ring_buffer, &data_put);

	TEST_ASSERT_TRUE(ring_buffer_get(ring_buffer, &data_get));
	
	TEST_ASSERT_EQUAL_INT8(data_put.var1, data_get.var1);
	TEST_ASSERT_EQUAL_INT16(data_put.var2, data_get.var2);
	TEST_ASSERT_EQUAL_INT32(data_put.var3, data_get.var3);
	TEST_ASSERT_EQUAL_STRING(data_put.str, data_get.str);

	ring_buffer_deinit(ring_buffer);
}

TEST(ring_buffer_test, ring_buffer_put_get_return_values_in_order_test){
	ring_buffer_t* ring_buffer = ring_buffer_init(item_size);

	test_t data_put = {.var1 = 0, .var2 = 0, .var3 = 0, .str = "string"};
	test_t data_get = {.str = "string"};

	for(uint8_t i = 0; i < 10; i++){
		for(uint8_t j = 0; j < RING_BUFFER_SIZE - 1; j++){
			ring_buffer_put(ring_buffer, &data_put);
			data_put.var1++;
			data_put.var2++;
			data_put.var3++;
		}

		TEST_ASSERT_TRUE(ring_buffer_is_full(ring_buffer));

		for(uint8_t j = 0; j < RING_BUFFER_SIZE - 1; j++){
			ring_buffer_get(ring_buffer, &data_get);
		
			TEST_ASSERT_EQUAL_INT8(j, data_get.var1);
			TEST_ASSERT_EQUAL_INT16(j, data_get.var2);
			TEST_ASSERT_EQUAL_INT32(j, data_get.var3);
			TEST_ASSERT_EQUAL_STRING(data_put.str, data_get.str);
		}

		TEST_ASSERT_TRUE(ring_buffer_is_empty(ring_buffer));
		data_put.var1 = 0;
		data_put.var2 = 0;
		data_put.var3 = 0;
	}
	ring_buffer_deinit(ring_buffer);
}


TEST(ring_buffer_test, ring_buffer_is_full_test){
	ring_buffer_t* ring_buffer = ring_buffer_init(item_size);

	test_t data = {.var1 = 1, .var2 = 2, .var3 = 3, .str = "string"};

	for(uint16_t i = 0; i < RING_BUFFER_SIZE + 100; i++){
		ring_buffer_put(ring_buffer, &data);	
	}

	TEST_ASSERT_TRUE(ring_buffer_is_full(ring_buffer));
	TEST_ASSERT_FALSE(ring_buffer_is_empty(ring_buffer));

	ring_buffer_deinit(ring_buffer);
}
