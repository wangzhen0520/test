//
// Created by WANGZHEN on 2019/3/31.
//
#include "test_others.h"
#include <iostream>
#include <vector>

using namespace std;
int test_others(int argc, char* argv[])
{
    std::cout << "test_others" << std::endl;

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
        // printf("%d ", *it);

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

            // printf("k: %d, N[%d]: %d,m : %d, alltime: %d, h: %d\n", k, j, N[j], m,
            // alltime, H);
        }

        if (alltime <= H) {
            printf("kkkk: %d\n", k);
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
        printf("the %d para is %s\n", j, argv[j]);
    }

    int c = 0;
    while ((c = getopt(argc, argv, "han:")) != EOF) {
        printf("start to process %d para\n", optind);
        switch (c) {
        case 'h':
            printf("we get option -h\n");
            break;
        case 'a':
            printf("we get option -a\n");
            break;
        case 'n':
            printf("we get option -n, para is %s\n", optarg);
            break;
        case '?':
            printf("unknown option: %c\n", optopt);
            break;
        default:
            break;
        }
    }
#endif
    return 0;
}