#ifndef ___TEST_H
#define ___TEST_H

#ifdef TEST
	int assertEquals(int value, int expectedValue, char *testId);
	int assertNotEquals(int value, int notExpectedValue, char *testId);
#endif

#endif
