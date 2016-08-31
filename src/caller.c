#include <unistd.h>
#include <libubox/blobmsg_json.h>
#include <libubox/uloop.h>
#include <libubus.h>

static struct ubus_context *ctx;
static struct blob_buf b;

static void caller_status_cb(struct ubus_request *req, int type, struct blob_attr *msg)
{
    // Parse reply message and action accordingly
    // TODO

#if 0
    static const struct blobmsg_policy policy = { "interface", BLOBMSG_TYPE_ARRAY };
    struct blob_attr *cur;

    blobmsg_parse(&policy, 1, &cur, blob_data(msg), blob_len(msg));
    if (cur)
        interfaces = blob_memdup(cur);
#endif

    fprintf(stderr, "Caller CB\n");
}

static void caller_get_cb(struct ubus_request *req, int type, struct blob_attr *msg)
{
    fprintf(stderr, "Get CB\n");
}

int main(int argc, char **argv)
{
    const char *ubus_socket = NULL;
    int ch;
    int ret;
    int i;
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

    for (i = 0; i < 10; i++) {
        ubus_invoke(ctx, id, "status", NULL, caller_status_cb, 0, 3000);

        blob_buf_init(&b, 0);
        blobmsg_add_u32(&b, "id", 1);
        ubus_invoke(ctx, id, "get", b.head, caller_get_cb, 0, 3000);
        blob_buf_free(&b);
        sleep(1);
    }

    ubus_free(ctx);

    return 0;
}
