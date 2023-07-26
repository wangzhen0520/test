//
// Created by WANGZHEN on 2019/3/31.
//
#include "test_others.h"

#include <iostream>
#include <algorithm>
#include <string.h>

#include "action.h"

using namespace std;

void test_for_duplicate(std::string &str)
{
    // int i, j, val, temp;
    // j = temp = 0;

    // for (i = 0; s[i]; i++) {
    //     val = s[i] - 'a';
    //     if ((temp & (1 << val)) == 0) {
    //         s[j++] = s[i];
    //         temp |= 1 << val;
    //     }
    // }
    // s[j] = '\0';

    sort(str.begin(), str.end());
    str.erase(unique(str.begin(), str.end()), str.end());

    // set<char> ss;
    // for (int i = 0; i < str.length(); i++) {
    //     ss.insert(str[i]);
    // }

    // str.clear();
    // for (const auto &it : ss) {
    //     if (it == '\0')
    //         break;
    //     str.push_back(it);
    // }
}

int parse_http_url(const char *url, char *host, size_t maxhost_len, char *path, size_t max_path_len, int *port)
{
    char scheme[8];
    size_t max_scheme_len = sizeof(scheme);

    char *scheme_ptr = (char *)url;
    char *host_ptr = (char *)strstr(url, "://");
    size_t host_len = 0;
    size_t path_len;
    char *port_ptr;
    char *path_ptr;
    char *fragment_ptr;

    if (host_ptr == NULL) {
        printf("Could not find http char\n");
        memcpy(scheme, "http", 4);
        scheme[4] = '\0';
        host_ptr = scheme_ptr;
    } else {
        if (max_scheme_len < host_ptr - scheme_ptr + 1) { /* including NULL-terminating char */
            printf("Scheme str is too small (%d >= %d)", max_scheme_len, host_ptr - scheme_ptr + 1);
            return -1;
        }
        memcpy(scheme, scheme_ptr, host_ptr - scheme_ptr);
        scheme[host_ptr - scheme_ptr] = '\0';

        host_ptr += 3;
    }

    port_ptr = strchr(host_ptr, ':');
    if (port_ptr != NULL) {
        uint16_t tport;
        host_len = port_ptr - scheme_ptr;
        port_ptr++;
        if (sscanf(port_ptr, "%hu", &tport) != 1) {
            printf("Could not find port");
            return -1;
        }
        *port = (int)tport;
    }

    path_ptr = strchr(host_ptr, '/');
    if (host_len == 0) {
        host_len = path_ptr - scheme_ptr;
    }

    if (maxhost_len < host_len + 1) { /* including NULL-terminating char */
        printf("Host str is too small (%d >= %d)", maxhost_len, host_len + 1);
        return -1;
    }
    memcpy(host, scheme_ptr, host_len);
    host[host_len] = '\0';

    fragment_ptr = strchr(host_ptr, '#');
    if (fragment_ptr != NULL) {
        path_len = fragment_ptr - path_ptr;
    } else {
        if (path_ptr != NULL) {
            path_len = strlen(path_ptr);
        } else {
            path_len = 0;
        }
    }

    if (max_path_len < path_len + 1) { /* including NULL-terminating char */
        printf("Path str is too small (%d >= %d)", max_path_len, path_len + 1);
        return -1;
    }

    if (path_ptr != NULL && path_len > 0) {
        memcpy(path, path_ptr, path_len);
        path[path_len] = '\0';
    }

    return 0;
}

int test_others(int argc, char *argv[])
{
    std::cout << "test_others" << std::endl;

    for (const auto &func : Singleton<LtcContainer>::instance()->GetAction(ACTION_OTHERS)) {
        func(argc, argv);
    }

#if 0
    vector<int> a;
    int i = 0;
    do {
        cin >> i;
        a.push_back(i);
    } while (getchar() != '\n');

    vector<int>::iterator it = a.begin();
    int n = a.size() - 1;
    int* N = new int[n];
    int H = 0;
    for (int i = 0; it != a.end(); it++) {
        // cout << "%d ", *it);

        if (it < a.end() - 1) {
            N[i++] = *it;
        } else {
            H = *it;
        }
    }

    int alltime = 0;
    for (int k = 1;; ++k) {
        alltime = 0;
        for (int j = 0; j < n; ++j) {
            int m = N[j] / k;
            int t = N[j] % k;
            if (m == 0 || t != 0 || N[j] < k) {
                alltime += 1;
            }
            alltime += m;

            // cout << "k: %d, N[%d]: %d,m : %d, alltime: %d, h: %d\n", k, j, N[j], m,
            // alltime, H);
        }

        if (alltime <= H) {
            cout << "kkkk: %d\n", k);
            alltime = 0;
            break;
        }
    }
#endif

#if 0
    extern int optind, opterr, optopt;
    extern char* optarg;

    int i = 0;
    for (int j = 0; j < argc; ++j) {
        cout << "the %d para is %s\n", j, argv[j]);
    }

    int c = 0;
    while ((c = getopt(argc, argv, "han:")) != EOF) {
        cout << "start to process %d para\n", optind);
        switch (c) {
        case 'h':
            cout << "we get option -h\n");
            break;
        case 'a':
            cout << "we get option -a\n");
            break;
        case 'n':
            cout << "we get option -n, para is %s\n", optarg);
            break;
        case '?':
            cout << "unknown option: %c\n", optopt);
            break;
        default:
            break;
        }
    }
#endif

#if 0
    std::string str = "abeaacda";
    cout << str << endl;
    test_for_duplicate(str);
    cout << str << endl;
#endif

#if 0
    char str[] = "*ICCID: \"898604D3102280671673\"";

    if (0 == strncmp(str, "*ICCID", 6)) {
        char *result = strtok(str, ":\"");
        while (result != NULL) {
            printf("222 %s\n", result);
            result = strtok(NULL, ":\"");
        }
    }
#endif

#if 0
    // unsigned char buf[] = {0xf4, 0xf5, 0x4d, 0x02, 0x08, 0x09, 0x10, 0x00, 0x00, 0x00, 0x83, 0x00, 0x08, 0x14, 0x00,
    //     0x00, 0x00, 0x00, 0x00, 0x50, 0x00, 0x4f, 0x53, 0x00, 0x3f, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    //     0x01, 0x0e, 0x00, 0x0c, 0x00, 0x00, 0x00, 0x0c, 0x00, 0x00, 0x00, 0x0c, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    //     0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x02,
    //     0x01, 0x02, 0x00, 0x00, 0x00, 0x00, 0x00, 0x63};
    unsigned char buf[] = {0xf4, 0x00, 0x00, 0x63, 0x00};
    hexdump(buf, sizeof(buf));
#endif

#if 0
    char src[] = "FIKS-CAT1-CA305";
    unsigned char buf[4] = {};
    const char *delim = "-";
    uint32_t item = 0;
    char *result = strtok(src, delim);
    while (result != NULL) {
        if (item == 2) {
            printf("ver: %s\n", result);
            // char *ptr = strstr(result, "CA");
            // printf("ptr: %s\n", ptr + 2);
            uint16_t ver = atoi(result + 2);
            buf[0] = (ver >> 8) & 0xff;
            buf[1] = ver & 0xff;
        }
        item++;
        result = strtok(NULL, delim);
    }

    hexdump(buf, sizeof(buf));

#endif


#if 0
    char *url = "http://ota-test.fotile.com/fotileAdminSystem/normalizationUpgrade.do?package=fotile.fiks.cat1.firmware&productId=&version=FIKS-CAT1-CA305&mac=864606062753370&isCustom=1&isEncryption=0";
    char host[256] = {0};
    char path[256] = {0};
    int port = 0;
    int ret = parse_http_url(url, host, sizeof(host), path, sizeof(path), &port);

    printf("host: %s\n", host);
    printf("%d path: %s\n", strlen(path), path);
    printf("port: %d\n", port);
#endif

    return 0;
}