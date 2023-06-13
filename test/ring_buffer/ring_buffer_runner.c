#include "unity_fixture.h"

TEST_GROUP_RUNNER(ring_buffer_test){
	RUN_TEST_CASE(ring_buffer_test, ring_buffer_init_test);
	RUN_TEST_CASE(ring_buffer_test, ring_buffer_get_when_is_empty_test);
	RUN_TEST_CASE(ring_buffer_test, ring_buffer_put_get_test);
	RUN_TEST_CASE(ring_buffer_test, ring_buffer_put_get_return_values_in_order_test);
	RUN_TEST_CASE(ring_buffer_test, ring_buffer_is_full_test);

}
