#include <unistd.h>
#include <signal.h>

#include <libubox/blobmsg_json.h>
#include "libubus.h"

static struct ubus_context *ctx;
static struct ubus_subscriber mkeyd_event;
static struct blob_buf b;

enum {
    STATUS_ID,
    __STATUS_MAX,
};

static const struct blobmsg_policy mkeyd_status_policy[__STATUS_MAX] = {
    [STATUS_ID] = { .name = "id", .type = BLOBMSG_TYPE_INT32 },
};

static int
mkeyd_status(struct ubus_context *ctx, struct ubus_object *obj,
        struct ubus_request_data *req, const char *method,
        struct blob_attr *msg)
{
    struct device *dev = NULL;
    struct blob_attr *tb[__STATUS_MAX];

    blobmsg_parse(mkeyd_status_policy, __STATUS_MAX, tb, blob_data(msg), blob_len(msg));

    if (tb[STATUS_ID]) {
        // Check if id valid
        // TODO
    }

    blob_buf_init(&b, 0);
    blobmsg_add_u8(&b, "gpio", 6);
    blobmsg_add_u8(&b, "value", 1);
    ubus_send_reply(ctx, req, b.head);

    return 0;
}

static const struct ubus_method mkeyd_methods[] = {
    UBUS_METHOD("status" , mkeyd_status , mkeyd_status_policy) ,
#if 0
	UBUS_METHOD("get"    , mkeyd_get    , mkeyd_get_policy)    ,
	UBUS_METHOD("set"    , mkeyd_set    , mkeyd_set_policy)    ,
#endif
};

static struct ubus_object_type mkeyd_object_type =
	UBUS_OBJECT_TYPE("key", mkeyd_methods);

static struct ubus_object mkeyd_object = {
	.name = "key",
	.type = &mkeyd_object_type,
	.methods = mkeyd_methods,
	.n_methods = ARRAY_SIZE(mkeyd_methods),
};

static void server_main(void)
{
	int ret;

	ret = ubus_add_object(ctx, &mkeyd_object);
	if (ret)
		fprintf(stderr, "Failed to add object: %s\n", ubus_strerror(ret));

	ret = ubus_register_subscriber(ctx, &mkeyd_event);
	if (ret)
		fprintf(stderr, "Failed to add watch handler: %s\n", ubus_strerror(ret));

	uloop_run();
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
	signal(SIGPIPE, SIG_IGN);

	ctx = ubus_connect(ubus_socket);
	if (!ctx) {
		fprintf(stderr, "Failed to connect to ubus\n");
		return -1;
	}

	ubus_add_uloop(ctx);

	server_main();

	ubus_free(ctx);
	uloop_done();

	return 0;
}
