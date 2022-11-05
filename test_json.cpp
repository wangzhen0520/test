#include <iostream>
#include <string>

#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <errno.h>
#include <time.h>

#include "test_json.h"

#include "json.h"
#include "json_visit.h"

using namespace std;

void parseJsonString(struct json_object *obj)
{
    if (!obj)
        return;

    cout << "------------------" << endl;
    json_object_object_foreach(obj, key, val)
    {
        cout << "key: " << key << endl;
        json_type type = json_object_get_type(val);
        switch (type) {
            case json_type_null:
                cout << "json type: json_type_null" << endl;
                cout << "val: " << json_object_get_string(val) << endl;
                cout << endl;
                break;

            case json_type_boolean:
                cout << "json type: json_type_boolean" << endl;
                cout << "val: " << json_object_get_boolean(val) << endl;
                cout << endl;
                break;

            case json_type_double:
                cout << "json type: json_type_double" << endl;
                cout << "val: " << json_object_get_double(val) << endl;
                cout << endl;
                break;

            case json_type_int:
                cout << "json type: json_type_int" << endl;
                cout << "val: " << json_object_get_int(val) << endl;
                cout << endl;
                break;

            case json_type_object:
                cout << "json type: json_type_object" << endl;
                cout << "val: " << json_object_get_string(val) << endl;
                cout << endl;
                parseJsonString(val);
                break;

            case json_type_array: {
                cout << "json type: json_type_array" << endl;
                cout << "val: " << json_object_get_string(val) << endl;
                cout << endl;

                int length = json_object_array_length(val);
                cout << "array length: " << length << endl;
                for (int i = 0; i < length; i++) {
                    struct json_object *arrayObj = json_object_array_get_idx(val, i);
                    parseJsonString(arrayObj);
                }
                break;
            }
            case json_type_string:
                cout << "json type: json_type_string" << endl;
                cout << "val: " << json_object_get_string(val) << endl;
                cout << endl;
                break;

            default:
                cout << "not support json type: " << type << endl;
                break;
        }
    }
}

int test_json_c()
{
    // const char *json_string = "{\n\
    //     \"errNum\": 0,\n\
    //     \"errMsg\": \"success\",\n\
    //     \"shanghai\": {\n\
    //         \"name\": \"szzs\",\n\
    //         \"curdot\": 2323.554,\n\
    //         \"curprice\": -5.897,\n\
    //         \"rate\": 0.25,\n\
    //         \"dealnumber\": 11586,\n\
    //         \"turnover\": 98322,\n\
    //     },\n\
    //     \"shengzhen\": {\n\
    //         \"name\": \"scz\",\n\
    //         \"curdot\": 2323.554,\n\
    //         \"curprice\": -5.897,\n\
    //         \"rate\": 0.25,\n\
    //         \"dealnumber\": 11586,\n\
    //         \"turnover\": 98322,\n\
    //     }\n\
    // }";

    // printf("%s\n", json_string);
    // struct json_object *obj = json_tokener_parse(json_string);

    struct json_object *obj = json_object_from_file("../json.txt");
    struct json_object *obj2 = json_object_from_file("../jsonCT.txt");

    parseJsonString(obj);

    cout << "======================================" << endl;

    parseJsonString(obj2);

    if (obj) {
        json_object_put(obj);
        obj = nullptr;
    }

    if (obj2) {
        json_object_put(obj2);
        obj2 = nullptr;
    }

    return 0;
}

int test_json_c1()
{
    json_object *my_array;
	my_array = json_object_new_array();
	// json_object_array_add(my_array, json_object_new_int(1));
	// json_object_array_add(my_array, json_object_new_int(2));
	// json_object_array_add(my_array, json_object_new_int(3));
	// json_object_array_put_idx(my_array, 4, json_object_new_int(5));
	printf("my_array=\n");
	for (size_t i = 0; i < json_object_array_length(my_array); i++)
	{
		json_object *obj = json_object_array_get_idx(my_array, i);
		printf("\t[%d]=%s\n", (int)i, json_object_to_json_string(obj));
	}
	printf("my_array.to_string()=%s\n", json_object_to_json_string(my_array));

	json_object_put(my_array);
    return 0;
}

static void do_benchmark(json_object *src1);

static const char *json_str1 =
    "{"
    "    \"glossary\": {"
    "        \"title\": \"example glossary\","
    "        \"GlossDiv\": {"
    "            \"number\": 16446744073709551615,"
    "            \"title\": \"S\","
    "            \"null_obj\": null, "
    "            \"exixt\": false,"
    "            \"quantity\":20,"
    "            \"univalent\":19.8,"
    "            \"GlossList\": {"
    "                \"GlossEntry\": {"
    "                    \"ID\": \"SGML\","
    "                    \"SortAs\": \"SGML\","
    "                    \"GlossTerm\": \"Standard Generalized Markup Language\","
    "                    \"Acronym\": \"SGML\","
    "                    \"Abbrev\": \"ISO 8879:1986\","
    "                    \"GlossDef\": {"
    "                        \"para\": \"A meta-markup language, used to create markup languages "
    "such as DocBook.\","
    "                        \"GlossSeeAlso\": [\"GML\", \"XML\"]"
    "                    },"
    "                    \"GlossSee\": \"markup\""
    "                }"
    "            }"
    "        }"
    "    }"
    "}";

static const char *json_str2 = "{\"menu\": {"
                               "    \"header\": \"SVG Viewer\","
                               "    \"items\": ["
                               "        {\"id\": \"Open\"},"
                               "        {\"id\": \"OpenNew\", \"label\": \"Open New\"},"
                               "        null,"
                               "        {\"id\": \"ZoomIn\", \"label\": \"Zoom In\"},"
                               "        {\"id\": \"ZoomOut\", \"label\": \"Zoom Out\"},"
                               "        {\"id\": \"OriginalView\", \"label\": \"Original View\"},"
                               "        null,"
                               "        {\"id\": \"Quality\", \"another_null\": null},"
                               "        {\"id\": \"Pause\"},"
                               "        {\"id\": \"Mute\"},"
                               "        null,"
                               "        {\"id\": \"Find\", \"label\": \"Find...\"},"
                               "        {\"id\": \"FindAgain\", \"label\": \"Find Again\"},"
                               "        {\"id\": \"Copy\"},"
                               "        {\"id\": \"CopyAgain\", \"label\": \"Copy Again\"},"
                               "        {\"id\": \"CopySVG\", \"label\": \"Copy SVG\"},"
                               "        {\"id\": \"ViewSVG\", \"label\": \"View SVG\"},"
                               "        {\"id\": \"ViewSource\", \"label\": \"View Source\"},"
                               "        {\"id\": \"SaveAs\", \"label\": \"Save As\"},"
                               "        null,"
                               "        {\"id\": \"Help\"},"
                               "        {\"id\": \"About\", \"label\": \"About Adobe CVG Viewer...\"}"
                               "    ]"
                               "}}";

static const char *json_str3 = "{\"menu\": {"
                               "  \"id\": \"file\","
                               "  \"value\": \"File\","
                               "  \"popup\": {"
                               "    \"menuitem\": ["
                               "      {\"value\": \"New\", \"onclick\": \"CreateNewDoc()\"},"
                               "      {\"value\": \"Open\", \"onclick\": \"OpenDoc()\"},"
                               "      {\"value\": \"Close\", \"onclick\": \"CloseDoc()\"}"
                               "    ]"
                               "  }"
                               "}}";

json_object_to_json_string_fn my_custom_serializer;
int my_custom_serializer(struct json_object *jso, struct printbuf *pb, int level, int flags)
{
    sprintbuf(pb, "OTHER");
    return 0;
}

json_c_shallow_copy_fn my_shallow_copy;
int my_shallow_copy(json_object *src, json_object *parent, const char *key, size_t index, json_object **dst)
{
    int rc;
    rc = json_c_shallow_copy_default(src, parent, key, index, dst);
    printf("CALLED: my_shallow_copy key: %s index: %u\n", key, index);

    if (rc < 0)
        return rc;
    if (key != NULL && strcmp(key, "with_serializer") == 0) {
        printf("CALLED: my_shallow_copy on with_serializer object\n");
        void *userdata = json_object_get_userdata(src);
        json_object_set_serializer(*dst, my_custom_serializer, userdata, NULL);
        return 2;
    }
    return rc;
}

int test_deep_copy(int argc, char **argv)
{
    struct json_object *src1, *src2, *src3;
    struct json_object *dst1 = NULL, *dst2 = NULL, *dst3 = NULL;
    int benchmark = 0;

    if (argc > 1 && strcmp(argv[1], "--benchmark") == 0) {
        benchmark = 1;
    }

    src1 = json_tokener_parse(json_str1);
    src2 = json_tokener_parse(json_str2);
    src3 = json_tokener_parse(json_str3);

    assert(src1 != NULL);
    assert(src2 != NULL);
    assert(src3 != NULL);

    printf("PASSED - loaded input data\n");

    /* do this 3 times to make sure overwriting it works */
    assert(0 == json_object_deep_copy(src1, &dst1, NULL));
    assert(0 == json_object_deep_copy(src2, &dst2, NULL));
    assert(0 == json_object_deep_copy(src3, &dst3, NULL));

    printf("PASSED - all json_object_deep_copy() returned succesful\n");

    assert(-1 == json_object_deep_copy(src1, &dst1, NULL));
    assert(errno == EINVAL);
    assert(-1 == json_object_deep_copy(src2, &dst2, NULL));
    assert(errno == EINVAL);
    assert(-1 == json_object_deep_copy(src3, &dst3, NULL));
    assert(errno == EINVAL);

    printf("PASSED - all json_object_deep_copy() returned EINVAL for non-null pointer\n");

    assert(1 == json_object_equal(src1, dst1));
    assert(1 == json_object_equal(src2, dst2));
    assert(1 == json_object_equal(src3, dst3));

    printf("PASSED - all json_object_equal() tests returned succesful\n");

    assert(0 == strcmp(json_object_to_json_string_ext(src1, JSON_C_TO_STRING_PRETTY),
                    json_object_to_json_string_ext(dst1, JSON_C_TO_STRING_PRETTY)));
    assert(0 == strcmp(json_object_to_json_string_ext(src2, JSON_C_TO_STRING_PRETTY),
                    json_object_to_json_string_ext(dst2, JSON_C_TO_STRING_PRETTY)));
    assert(0 == strcmp(json_object_to_json_string_ext(src3, JSON_C_TO_STRING_PRETTY),
                    json_object_to_json_string_ext(dst3, JSON_C_TO_STRING_PRETTY)));

    printf("PASSED - comparison of string output\n");

    json_object_get(dst1);
    assert(-1 == json_object_deep_copy(src1, &dst1, NULL));
    assert(errno == EINVAL);
    json_object_put(dst1);

    printf("PASSED - trying to overrwrite an object that has refcount > 1");

    printf("\nPrinting JSON objects for visual inspection\n");
    printf("------------------------------------------------\n");
    printf(" JSON1\n");
    printf("%s\n", json_object_to_json_string_ext(dst1, JSON_C_TO_STRING_PRETTY));
    printf("------------------------------------------------\n");

    printf("------------------------------------------------\n");
    printf(" JSON2\n");
    printf("%s\n", json_object_to_json_string_ext(dst2, JSON_C_TO_STRING_PRETTY));
    printf("------------------------------------------------\n");

    printf("------------------------------------------------\n");
    printf(" JSON3\n");
    printf("------------------------------------------------\n");
    printf("%s\n", json_object_to_json_string_ext(dst3, JSON_C_TO_STRING_PRETTY));
    printf("------------------------------------------------\n");

    json_object_put(dst1);
    json_object_put(dst2);
    json_object_put(dst3);

    printf("\nTesting deep_copy with a custom serializer set\n");
    json_object *with_serializer = json_object_new_string("notemitted");

    char udata[] = "dummy userdata";
    json_object_set_serializer(with_serializer, my_custom_serializer, udata, NULL);
    json_object_object_add(src1, "with_serializer", with_serializer);
    dst1 = NULL;
    /* With a custom serializer in use, a custom shallow_copy function must also be used */
    assert(-1 == json_object_deep_copy(src1, &dst1, NULL));
    assert(0 == json_object_deep_copy(src1, &dst1, my_shallow_copy));

    printf("------------------------------------------------\n");
    printf(" dst1\n");
    printf("------------------------------------------------\n");
    printf("%s\n", json_object_to_json_string_ext(dst1, JSON_C_TO_STRING_PRETTY));
    printf("------------------------------------------------\n");
	
    // int rv = json_c_visit(dst1, 0, NULL, NULL);

    json_object *dest_with_serializer = json_object_object_get(dst1, "with_serializer");
    assert(dest_with_serializer != NULL);
    char *dst_userdata = (char *)json_object_get_userdata(dest_with_serializer);
    assert(strcmp(dst_userdata, "dummy userdata") == 0);

    const char *special_output = json_object_to_json_string(dest_with_serializer);
    assert(strcmp(special_output, "OTHER") == 0);
    printf("\ndeep_copy with custom serializer worked OK.\n");
    json_object_put(dst1);

    if (benchmark) {
        do_benchmark(src2);
    }

    json_object_put(src1);
    json_object_put(src2);
    json_object_put(src3);

    return 0;
}

static void do_benchmark(json_object *src2)
{
    json_object *dst2 = NULL;

    int ii;
    /**
     * The numbers that I got are:
     * BENCHMARK - 1000000 iterations of 'dst2 = json_tokener_parse(json_object_get_string(src2))' took 71 seconds
     * BENCHMARK - 1000000 iterations of 'json_object_deep_copy(src2, &dst2, NULL)' took 29 seconds
     */

    int iterations = 1000000;
    time_t start = time(NULL);

    start = time(NULL);
    for (ii = 0; ii < iterations; ii++) {
        dst2 = json_tokener_parse(json_object_get_string(src2));
        json_object_put(dst2);
    }
    printf("BENCHMARK - %d iterations of 'dst2 = "
           "json_tokener_parse(json_object_get_string(src2))' took %d seconds\n",
        iterations,
        (int)(time(NULL) - start));

    start = time(NULL);
    dst2 = NULL;
    for (ii = 0; ii < iterations; ii++) {
        json_object_deep_copy(src2, &dst2, NULL);
        json_object_put(dst2);
        dst2 = NULL;
    }
    printf("BENCHMARK - %d iterations of 'json_object_deep_copy(src2, &dst2, NULL)' took %d "
           "seconds\n",
        iterations,
        (int)(time(NULL) - start));
}

void shwo_object_json(json_object *obj)
{
	printf("------------------------------------------------\n");
	printf(" json info\n");
	printf("------------------------------------------------\n");
	printf("%s\n", json_object_to_json_string_ext(obj, JSON_C_TO_STRING_PRETTY));
	printf("------------------------------------------------\n");
}

void test_deep_copy2()
{
    struct json_object *src1, *src2, *src3;
    struct json_object *dst1 = NULL;

    src1 = json_tokener_parse(json_str1);
    src2 = json_tokener_parse(json_str2);

    assert(src1 != NULL);

    shwo_object_json(src1);
    shwo_object_json(src2);

    printf("PASSED - loaded input data\n");

    int ret = json_object_deep_copy(src1, &src2, my_shallow_copy);
    printf("ret - %d\n", ret);

    assert(ret == 0);

    shwo_object_json(src2);

    json_object_put(src1);
    json_object_put(src2);
}

int test_replace_existing(int argc, char **argv)
{
	MC_SET_DEBUG(1);

	/*
	 * Check that replacing an existing object keeps the key valid,
	 * and that it keeps the order the same.
	 */
	json_object *my_object = json_object_new_object();
	json_object_object_add(my_object, "foo1", json_object_new_string("bar1"));
	json_object_object_add(my_object, "foo2", json_object_new_string("bar2"));
	json_object_object_add(my_object, "deleteme", json_object_new_string("bar2"));
	json_object_object_add(my_object, "foo3", json_object_new_string("bar3"));

    shwo_object_json(my_object);

	printf("==== delete-in-loop test starting ====\n");

	int orig_count = 0;
	json_object_object_foreach(my_object, key0, val0)
	{
		printf("Key at index %d is [%s] %d", orig_count, key0, (val0 == NULL));
		if (strcmp(key0, "deleteme") == 0)
		{
			json_object_object_del(my_object, key0);
			printf(" (deleted)\n");
		}
		else
			printf(" (kept)\n");
		orig_count++;
	}

    shwo_object_json(my_object);

	printf("==== replace-value first loop starting ====\n");

	const char *original_key = NULL;
	orig_count = 0;
	json_object_object_foreach(my_object, key, val)
	{
		printf("Key at index %d is [%s] %d\n", orig_count, key, (val == NULL));
		orig_count++;
		if (strcmp(key, "foo2") != 0)
			continue;
		printf("replacing value for key [%s]\n", key);
		original_key = key;
		json_object_object_add(my_object, key, json_object_new_string("zzz"));
	}

    shwo_object_json(my_object);

	printf("==== second loop starting ====\n");

	int new_count = 0;
	int retval = 0;
	json_object_object_foreach(my_object, key2, val2)
	{
		printf("Key at index %d is [%s] %d\n", new_count, key2, (val2 == NULL));
		new_count++;
		if (strcmp(key2, "foo2") != 0)
			continue;
		printf("pointer for key [%s] does %smatch\n", key2,
		       (key2 == original_key) ? "" : "NOT ");
		if (key2 != original_key)
			retval = 1;
	}
	if (new_count != orig_count)
	{
		printf("mismatch between original count (%d) and new count (%d)\n", orig_count,
		       new_count);
		retval = 1;
	}

	json_object_put(my_object);

	return retval;
}

static json_c_visit_userfunc emit_object;
static json_c_visit_userfunc skip_arrays;
static json_c_visit_userfunc pop_and_stop;
static json_c_visit_userfunc err_on_subobj2;
static json_c_visit_userfunc pop_array;
static json_c_visit_userfunc stop_array;
static json_c_visit_userfunc err_return;

int test_visit_c(void)
{
	MC_SET_DEBUG(1);

	const char *input = "{\
		\"obj1\": 123,\
		\"obj2\": {\
			\"subobj1\": \"aaa\",\
			\"subobj2\": \"bbb\",\
			\"subobj3\": [ \"elem1\", \"elem2\", true ],\
		},\
		\"obj3\": 1.234,\
		\"obj4\": [ true, false, null ]\
	}";

	json_object *jso = json_tokener_parse(input);
	printf("jso.to_string()=%s\n", json_object_to_json_string(jso));

	int rv;
	rv = json_c_visit(jso, 0, emit_object, NULL);
	printf("json_c_visit(emit_object)=%d\n", rv);
	printf("================================\n\n");

	rv = json_c_visit(jso, 0, skip_arrays, NULL);
	printf("json_c_visit(skip_arrays)=%d\n", rv);
	printf("================================\n\n");

	rv = json_c_visit(jso, 0, pop_and_stop, NULL);
	printf("json_c_visit(pop_and_stop)=%d\n", rv);
	printf("================================\n\n");

	rv = json_c_visit(jso, 0, err_on_subobj2, NULL);
	printf("json_c_visit(err_on_subobj2)=%d\n", rv);
	printf("================================\n\n");

	rv = json_c_visit(jso, 0, pop_array, NULL);
	printf("json_c_visit(pop_array)=%d\n", rv);
	printf("================================\n\n");

	rv = json_c_visit(jso, 0, stop_array, NULL);
	printf("json_c_visit(stop_array)=%d\n", rv);
	printf("================================\n\n");

	rv = json_c_visit(jso, 0, err_return, NULL);
	printf("json_c_visit(err_return)=%d\n", rv);
	printf("================================\n\n");

	json_object_put(jso);

	return 0;
}

static int emit_object(json_object *jso, int flags, json_object *parent_jso, const char *jso_key,
                       size_t *jso_index, void *userarg)
{
	printf("flags: 0x%x, key: %s, index: %ld, value: %s\n", flags,
	       (jso_key ? jso_key : "(null)"), (jso_index ? (long)*jso_index : -1L),
	       json_object_to_json_string(jso));
	return JSON_C_VISIT_RETURN_CONTINUE;
}

static int skip_arrays(json_object *jso, int flags, json_object *parent_jso, const char *jso_key,
                       size_t *jso_index, void *userarg)
{
	(void)emit_object(jso, flags, parent_jso, jso_key, jso_index, userarg);
	if (json_object_get_type(jso) == json_type_array)
		return JSON_C_VISIT_RETURN_SKIP;
	return JSON_C_VISIT_RETURN_CONTINUE;
}

static int pop_and_stop(json_object *jso, int flags, json_object *parent_jso, const char *jso_key,
                        size_t *jso_index, void *userarg)
{
	(void)emit_object(jso, flags, parent_jso, jso_key, jso_index, userarg);
	if (jso_key != NULL && strcmp(jso_key, "subobj1") == 0)
	{
		printf("POP after handling subobj1\n");
		return JSON_C_VISIT_RETURN_POP;
	}
	if (jso_key != NULL && strcmp(jso_key, "obj3") == 0)
	{
		printf("STOP after handling obj3\n");
		return JSON_C_VISIT_RETURN_STOP;
	}
	return JSON_C_VISIT_RETURN_CONTINUE;
}

static int err_on_subobj2(json_object *jso, int flags, json_object *parent_jso, const char *jso_key,
                          size_t *jso_index, void *userarg)
{
	(void)emit_object(jso, flags, parent_jso, jso_key, jso_index, userarg);
	if (jso_key != NULL && strcmp(jso_key, "subobj2") == 0)
	{
		printf("ERROR after handling subobj1\n");
		return JSON_C_VISIT_RETURN_ERROR;
	}
	return JSON_C_VISIT_RETURN_CONTINUE;
}

static int pop_array(json_object *jso, int flags, json_object *parent_jso, const char *jso_key,
                     size_t *jso_index, void *userarg)
{
	(void)emit_object(jso, flags, parent_jso, jso_key, jso_index, userarg);
	if (jso_index != NULL && (*jso_index == 0))
	{
		printf("POP after handling array[0]\n");
		return JSON_C_VISIT_RETURN_POP;
	}
	return JSON_C_VISIT_RETURN_CONTINUE;
}

static int stop_array(json_object *jso, int flags, json_object *parent_jso, const char *jso_key,
                      size_t *jso_index, void *userarg)
{
	(void)emit_object(jso, flags, parent_jso, jso_key, jso_index, userarg);
	if (jso_index != NULL && (*jso_index == 0))
	{
		printf("STOP after handling array[1]\n");
		return JSON_C_VISIT_RETURN_STOP;
	}
	return JSON_C_VISIT_RETURN_CONTINUE;
}

static int err_return(json_object *jso, int flags, json_object *parent_jso, const char *jso_key,
                      size_t *jso_index, void *userarg)
{
	printf("flags: 0x%x, key: %s, index: %ld, value: %s\n", flags,
	       (jso_key ? jso_key : "(null)"), (jso_index ? (long)*jso_index : -1L),
	       json_object_to_json_string(jso));
	return 100;
}

int test_json(int argc, char *argv[])
{
#define test
#ifdef test
    return 0;
#else
    cout << __FILE__ << ":" << __func__ << endl;
#endif

#if 0
    std::string srcfile("");
    std::string customefile("");
    if (argc == 3) {
        srcfile = argv[1];
        customefile = argv[2];
    } else {
        std::cout << "help: [cmd] src dst" << std::endl;
        return -1;
    }

    jsonMerge(srcfile, customefile);
#endif

#if 0
    struct json_object *new_obj;
    new_obj = json_tokener_parse("[\"abc\",null,\"def\",12]");
    std::cout << "new_obj_.to.string()=" << json_object_to_json_string(new_obj) << std::endl;
    json_object_put(new_obj);
#endif
    
    // test_json_c();
    test_json_c1();

    // test_deep_copy(argc, argv);

    // test_replace_existing(argc, argv);

    // test_deep_copy2();

    // test_visit_c();

    return 0;
}
