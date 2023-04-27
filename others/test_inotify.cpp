#ifdef __linux__
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/inotify.h>

#include "action.h"

using namespace std;

#define EVENT_SIZE (sizeof(struct inotify_event))
#define BUF_LEN (10 * (EVENT_SIZE + 256))

static const char * filetype[] = {"directory", "file"};
const std::string INOTIFY_DIR("/home/user/nfs");

static void displayInotifyEvent(struct inotify_event *event)
{
    const char *type = (event->mask & IN_ISDIR) ? filetype[0] : filetype[1];
    if (event->len) {
        if (event->mask & IN_CREATE) {
            printf("The %s %s was created.\n", type, event->name);
        } else if (event->mask & IN_DELETE) {
            printf("The %s %s was deleted.\n", type, event->name);
        } else if (event->mask & IN_MODIFY) {
            printf("The %s %s was modified.\n", type, event->name);
        }
    }
}

void test_inotify(int argc, char *argv[])
{
#define test
#ifdef test
    return;
#else
    cout << __FILE__ << ":" << __func__ << endl;
#endif

    int length, i = 0;
    int fd;
    int wd;
    char buffer[BUF_LEN] __attribute__((aligned(8)));

    const char *type;
    char *p;
    struct inotify_event *event;

    printf("%s %s\n", filetype[0], filetype[1]);

    fd = inotify_init();
    if (fd < 0) {
        perror("inotify_init");
    }

    wd = inotify_add_watch(fd, INOTIFY_DIR.c_str(), IN_MODIFY | IN_CREATE | IN_DELETE);
    if (wd == -1) {
        perror("error inotify_add_watch\n");
        return;
    }

    for (;;) {
        length = read(fd, buffer, BUF_LEN);

        if (length < 0) {
            perror("read\n");
            return;
        }
        for (p = buffer; p < buffer + length;) {
            event = (struct inotify_event *)p;
            displayInotifyEvent(event);

            p += sizeof(struct inotify_event) + event->len;
        }
    }

    inotify_rm_watch(fd, wd);
    close(fd);
}

LTC_REGISTER_ACTION(ACTION_OTHERS, test_inotify);
#endif