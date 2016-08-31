#include <unistd.h>
#include <libubox/blobmsg_json.h>
#include <libubox/uloop.h>
#include <libubus.h>

static struct ubus_context *ctx;
static struct blob_buf b;

static void caller_subscribe_cb(struct ubus_context *ctx, struct ubus_object *obj)
{
    fprintf(stderr, "Subscribers active: %d\n", obj->has_subscribers);
}

static struct ubus_object caller_object = {
    .subscribe_cb = caller_subscribe_cb,
};


static void client_main(void)
{
   int ret;
   uint32_t id;

    ret = ubus_add_object(ctx, &caller_object);
    if (ret) {
        fprintf(stderr, "Failed to add_object object: %s\n", ubus_strerror(ret));
        return;
    }

    if (ubus_lookup_id(ctx, "key", &id)) {
        fprintf(stderr, "Failed to look up key object\n");
        return;
    }

    ubus_invoke(ctx, id, "status", NULL, NULL, 0, 3000);

    blob_buf_init(&b, 0);
    blobmsg_add_u32(&b, "id", 1);
    ubus_invoke(ctx, id, "get", b.head, NULL, 0, 3000);
    blob_buf_free(&b);
}

int main(int argc, char **argv)
{
    const char *ubus_socket = NULL;
    int ch;

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

    uloop_init();

    ctx = ubus_connect(ubus_socket);
    if (!ctx) {
        fprintf(stderr, "Failed to connect to ubus\n");
        return -1;
    }

    ubus_add_uloop(ctx);

    client_main();

    uloop_run();

    ubus_free(ctx);
    uloop_done();

    return 0;
}
