//
// Created  on 3/19/2020.
//

#include "test_utilities.h"
#include "map.h"
#include <stdlib.h>

#define NUMBER_TESTS 8

bool testMapCreateDestroy()
{
    Map map = mapCreate();
    ASSERT_TEST(map != NULL);
    ASSERT_TEST(mapGetSize(map) == 0);
    ASSERT_TEST(mapGetFirst(map) == NULL);
    mapDestroy(map);
    return true;
}

bool testMapClear()
{
    Map map = mapCreate();
    ASSERT_TEST(map != NULL);
    ASSERT_TEST(mapPut(map, "key1", "value1") == MAP_SUCCESS);
    ASSERT_TEST(mapPut(map, "key2", "value2") == MAP_SUCCESS);
    ASSERT_TEST(mapContains(map,"key1") == true);
    ASSERT_TEST(mapGetSize(map) == 2);
    ASSERT_TEST(mapClear(NULL) == MAP_NULL_ARGUMENT);
    ASSERT_TEST(mapClear(map) == MAP_SUCCESS);
    ASSERT_TEST(mapGetSize(map) == 0);
    ASSERT_TEST(mapContains(map,"key1") == false);
    mapDestroy(map);
    return true;
}

bool testMapGetFirstAndGetNext()
{
    Map map = mapCreate();

    ASSERT_TEST(mapPut(map, "key1", "value1") == MAP_SUCCESS);
    ASSERT_TEST(mapPut(map, "key2", "value2") == MAP_SUCCESS);
     ASSERT_TEST(mapPut(map, "key3", "value3") == MAP_SUCCESS);
    char *data[] = {"key3", "key2", "key1"};
    int i = 0;
    MAP_FOREACH(iter, map)
    {
        if (i != 3)
        {
            ASSERT_TEST(strcmp(data[i], iter) == 0);
        }
        else
        {
            ASSERT_TEST(iter == NULL);
        }
        i++;
    }
    mapDestroy(map);
    return true;
}

bool testMapContainsAndRemove()
{
    Map map = mapCreate();
    ASSERT_TEST(map != NULL);
    ASSERT_TEST(mapGetSize(map) == 0);
    ASSERT_TEST(mapPut(map, "key1", "value1") == MAP_SUCCESS);
    ASSERT_TEST(mapPut(map, "key2", "value2") == MAP_SUCCESS);
    ASSERT_TEST(mapContains(map, "key1"));
    ASSERT_TEST(mapContains(map, "key2"));
    ASSERT_TEST(!mapContains(map, "key3"));
    ASSERT_TEST(mapRemove(map, "key1") == MAP_SUCCESS);
    ASSERT_TEST(!mapContains(map, "key1"));
    ASSERT_TEST(mapRemove(map, "key3") == MAP_ITEM_DOES_NOT_EXIST);
    mapDestroy(map);
    return true;
}

bool testMapCopy()
{
    Map map = mapCreate();
    ASSERT_TEST(map != NULL);
    ASSERT_TEST(mapGetSize(map) == 0);
    ASSERT_TEST(mapGetFirst(map) == NULL);
    ASSERT_TEST(mapPut(map, "key1", "value1") == MAP_SUCCESS);
    ASSERT_TEST(mapPut(map, "key2", "value2") == MAP_SUCCESS);
    Map map2 = mapCopy(map);
    ASSERT_TEST(map2 != NULL);
    char *getVal = mapGet(map, "key1");
    ASSERT_TEST(strcmp(getVal, "value1") == 0);
    char *getVal2 = mapGet(map, "key2");
    ASSERT_TEST(strcmp(getVal2, "value2") == 0);
    ASSERT_TEST(mapGetSize(map) == mapGetSize(map2));
    mapDestroy(map2);
    mapDestroy(map);
    return true;
}

bool testMapAddAndSize()
{
    Map map = mapCreate();
    ASSERT_TEST(mapGetSize(map) == 0);
    ASSERT_TEST(mapPut(map, "key", "value") == MAP_SUCCESS);
    ASSERT_TEST(mapGetSize(map) == 1);
    ASSERT_TEST(mapPut(map, "key2", "value2") == MAP_SUCCESS);
    ASSERT_TEST(mapGetSize(map) == 2);
    ASSERT_TEST(mapClear(map) == MAP_SUCCESS);
    ASSERT_TEST(mapGetSize(map) == 0);
    mapDestroy(map);
    return true;
}

bool testMapGet()
{
    Map map = mapCreate();
    ASSERT_TEST(mapPut(map, "key", "value") == MAP_SUCCESS);
    char *getVal = mapGet(map, "key");
    ASSERT_TEST(strcmp(getVal, "value") == 0);
    ASSERT_TEST(mapPut(map, "key", "value2") == MAP_SUCCESS);
    char *getVal2 = mapGet(map, "key");
    ASSERT_TEST(strcmp(getVal2, "value2") == 0);
    mapDestroy(map);
    return true;
}

bool testIterator()
{
    Map map = mapCreate();

    ASSERT_TEST(mapPut(map, "key1", "value1") == MAP_SUCCESS);
    ASSERT_TEST(mapPut(map, "key2", "value2") == MAP_SUCCESS);

    int i = 0;
    MAP_FOREACH(iter, map)
    {
        if (i != 2)
        {
            ASSERT_TEST(iter != NULL);
        }
        else
        {
            ASSERT_TEST(iter == NULL);
        }
        i++;
    }
    mapDestroy(map);
    return true;
}

bool (*tests[])(void) = {
    testMapCreateDestroy,
    testMapAddAndSize,
    testMapGet,
    testIterator,
    testMapCopy,
    testMapContainsAndRemove,
    testMapGetFirstAndGetNext,
    testMapClear};

const char *testNames[] = {
    "testMapCreateDestroy",
    "testMapAddAndSize",
    "testMapGet",
    "testIterator",
    "testMapCopy",
    "testMapContainsAndRemove",
    "testMapGetFirstAndGetNext",
    "testMapClear"};

int main(int argc, char *argv[])
{
    if (argc == 1)
    {
        for (int test_idx = 0; test_idx < NUMBER_TESTS; test_idx++)
        {
            RUN_TEST(tests[test_idx], testNames[test_idx]);
        }
        return 0;
    }
    if (argc != 2)
    {
        fprintf(stdout, "Usage: map <test index>\n");
        return 0;
    }

    int test_idx = strtol(argv[1], NULL, 10);
    if (test_idx < 1 || test_idx > NUMBER_TESTS)
    {
        fprintf(stderr, "Invalid test index %d\n", test_idx);
        return 0;
    }

    RUN_TEST(tests[test_idx - 1], testNames[test_idx - 1]);
    return 0;
}
