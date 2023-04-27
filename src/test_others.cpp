//
// Created by WANGZHEN on 2019/3/31.
//
#include "test_others.h"

#include <iostream>
#include <algorithm>

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
    return 0;
}