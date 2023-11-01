#include <sys/wait.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "../lib/shm.h"
#include "Unity/unity.h"

struct test_data {
	int num1;
	double num2;
	char *str;
};

struct test_data *test;
shm *test_shm;

#define SHM_CUSTOM_KEY 0x1fff
#define SIZE (sizeof(*test))

void child_writes_shm()
{
	if(fork() == 0) {
		shm_write(test_shm, (void *) test, SIZE);
		exit(0);
	}
	wait(NULL);
}


void setUp()
{
	test = malloc(sizeof(*test));
	test->num1 = 5;
	test->num2 = 9.99;
	test->str = "Test";
}

void tearDown()
{
	free(test);
	shm_delete(test_shm);
}

void test_shm_create_size_zero(void)
{
	test_shm = shm_create(0, 0);
	TEST_ASSERT_NULL(test_shm);
}

void test_shm_create_no_key(void)
{
	test_shm = shm_create(0, SIZE);
	child_writes_shm();
	struct test_data *res = (struct test_data *) shm_read(test_shm);
	TEST_ASSERT_EQUAL_INT(5, res->num1);
	TEST_ASSERT_EQUAL_DOUBLE(9.99, res->num2);
	TEST_ASSERT_EQUAL_STRING("Test", res->str);
}

void test_shm_create_key(void)
{
	test_shm = shm_create(SHM_CUSTOM_KEY, SIZE);
	child_writes_shm();
	struct test_data *res = (struct test_data *) shm_read(test_shm);
	TEST_ASSERT_EQUAL_INT(5, res->num1);
	TEST_ASSERT_EQUAL_DOUBLE(9.99, res->num2);
	TEST_ASSERT_EQUAL_STRING("Test", res->str);
}

void test_shm_attach_size_zero(void)
{
	test_shm = shm_create(0, 0);
	shm_detach(test_shm);
	shm_attach(shm_get_id(test_shm), 0);
	TEST_ASSERT_NULL(test_shm);
}

void test_shm_attach(void)
{
	test_shm = shm_create(0, SIZE);
	child_writes_shm();
	shm_id_t id = shm_get_id(test_shm);
	shm_detach(test_shm);
	test_shm = shm_attach(id, SIZE);
	struct test_data *res = (struct test_data *) shm_read(test_shm);
	TEST_ASSERT_EQUAL_INT(5, res->num1);
	TEST_ASSERT_EQUAL_DOUBLE(9.99, res->num2);
	TEST_ASSERT_EQUAL_STRING("Test", res->str);
}

int main(void)
{
	UNITY_BEGIN();
	RUN_TEST(test_shm_create_size_zero);
	RUN_TEST(test_shm_create_no_key);
	RUN_TEST(test_shm_create_key);
	RUN_TEST(test_shm_attach_size_zero);
	RUN_TEST(test_shm_attach);
	UNITY_END();
}
