#include <unistd.h>
#include <libubox/blobmsg_json.h>
#include <libubox/uloop.h>
#include <libubus.h>

static struct ubus_context *ctx;

static int counter = 0;
static uint32_t obj_id;
static struct ubus_subscriber subscriber_event;

static int subscriber_notify(struct ubus_context *ctx, struct ubus_object *obj,
        struct ubus_request_data *req,
        const char *method, struct blob_attr *msg)
{
    char *str = "zzz";

    printf("Notify handler...\n");

    //str = blobmsg_format_json(msg, true);
    //str = blobmsg_get_string(msg);
    fprintf(stderr, "Received notification '%s': blob len - %d\n", method, blobmsg_data_len(msg));

    return 0;
}

static void subscriber_handle_remove(struct ubus_context *ctx,
        struct ubus_subscriber *obj, uint32_t id)
{
    fprintf(stderr, "Object %08x went away\n", id);
}

// Handle incoming notification
static void subscriber_main(void)
{
    int ret;

    subscriber_event.cb = subscriber_notify;
    subscriber_event.remove_cb = subscriber_handle_remove;  // remove_cb called when server initial the the subscribe, eg server exit

    // Register
    ret = ubus_register_subscriber(ctx, &subscriber_event);
    if (ret) {
        fprintf(stderr, "Failed to add watch handler: %s\n", ubus_strerror(ret));
    }

    // Get id of object to be subscribed
    ret = ubus_lookup_id(ctx, "key", &obj_id);

    // Subscribe
    ret = ubus_subscribe(ctx, &subscriber_event, obj_id);
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

    subscriber_main();

    uloop_run();

    ubus_free(ctx);
    uloop_done();

    return 0;
}
