#include <unistd.h>
#include <libubox/blobmsg_json.h>
#include <libubox/uloop.h>
#include <libubus.h>

static struct ubus_context *ctx;
static struct blob_buf b;

int main(int argc, char **argv)
{
    const char *ubus_socket = NULL;
    int ch;
    int ret;
    uint32_t id;

    while ((ch = getopt(argc, argv, "cs:")) != -1) {
        switch (ch) {
            case 's':
                ubus_socket = optarg;
                break;
            default:
                break;
        }
    }

    argc -= optind;
    argv += optind;

    ctx = ubus_connect(ubus_socket);
    if (!ctx) {
        fprintf(stderr, "Failed to connect to ubus\n");
        return -1;
    }

    if (ubus_lookup_id(ctx, "key", &id)) {
        fprintf(stderr, "Failed to look up key object\n");
        return -1;
    }

    ubus_invoke(ctx, id, "status", NULL, NULL, 0, 3000);

    blob_buf_init(&b, 0);
    blobmsg_add_u32(&b, "id", 1);
    ubus_invoke(ctx, id, "get", b.head, NULL, 0, 3000);
    blob_buf_free(&b);

    ubus_free(ctx);

    return 0;
}
