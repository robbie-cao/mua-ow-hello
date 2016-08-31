#include <unistd.h>
#include <signal.h>

#include <libubox/blobmsg_json.h>
#include "libubus.h"
#include "mraa.h"

#define GPIO_PIN    16

static struct ubus_context *ctx;
static struct ubus_subscriber mkeyd_event;
static struct blob_buf b;

static mraa_gpio_context gpio;

static void mkeyd_notify(void*);

static int
mkeyd_status(struct ubus_context *ctx, struct ubus_object *obj,
        struct ubus_request_data *req, const char *method,
        struct blob_attr *msg)
{
    blob_buf_init(&b, 0);
    blobmsg_add_string(&b, "status", "XX");
    blobmsg_add_u16(&b, "gpio", GPIO_PIN);
    blobmsg_add_u8(&b, "value", mraa_gpio_read(gpio));
    ubus_send_reply(ctx, req, b.head);
    // Test notify
    mkeyd_notify(NULL);

    return 0;
}

enum {
    GET_ID,
    __GET_MAX,
};

static const struct blobmsg_policy mkeyd_get_policy[__GET_MAX] = {
    [GET_ID] = { .name = "id", .type = BLOBMSG_TYPE_INT32 },
};

static int
mkeyd_get(struct ubus_context *ctx, struct ubus_object *obj,
        struct ubus_request_data *req, const char *method,
        struct blob_attr *msg)
{
    struct blob_attr *tb[__GET_MAX];

    blobmsg_parse(mkeyd_get_policy, __GET_MAX, tb, blob_data(msg), blob_len(msg));

    if (tb[GET_ID]) {
        // Check if id valid
        // TODO
    }

    blob_buf_init(&b, 0);
    blobmsg_add_string(&b, "get", "XX");
    blobmsg_add_u16(&b, "gpio", GPIO_PIN);
    blobmsg_add_u8(&b, "value", mraa_gpio_read(gpio));
    ubus_send_reply(ctx, req, b.head);
    // Test notify
    mkeyd_notify(NULL);

    return 0;
}

static const struct ubus_method mkeyd_methods[] = {
    { .name = "status" , .handler = mkeyd_status } ,
	UBUS_METHOD("get"    , mkeyd_get    , mkeyd_get_policy)    ,
#if 0
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

static int isr_count = 0;

static void mkeyd_isr(void *param)
{
    uint8_t level = mraa_gpio_read(gpio);
    char str[32];

    sprintf(str, "ISR-%02d - %d - %s",
            isr_count,
            param ? *((int *)param) : 0,
            level ? "KeyPress" : "KeyRelease"
            );
    isr_count += 1;

    blob_buf_init(&b, 0);
    blobmsg_add_string(&b, "isr", "XX");
    blobmsg_add_u16(&b, "key", GPIO_PIN);
    blobmsg_add_u8(&b, "action", mraa_gpio_read(gpio));
    ubus_notify(ctx,  &mkeyd_object, str, b.head, -1);
}

static int notify_count = 0;

static void mkeyd_notify(void* param)
{
    char str[32];

    if (param) {
        sprintf(str, "Notify-%02d - %d", notify_count, *((int *)param));
    } else {
        sprintf(str, "Notify-%02d", notify_count);
    }
    notify_count += 1;
    ubus_notify(ctx,  &mkeyd_object, str, b.head, -1);
}

static int mkeyd_init(void)
{
    int param = 0x55;

    gpio = mraa_gpio_init(GPIO_PIN);
    // Check init reseut
    // TODO
    mraa_gpio_dir(gpio, MRAA_GPIO_IN);
    mraa_gpio_isr(gpio, MRAA_GPIO_EDGE_BOTH, mkeyd_isr, &param);

    return 0;
}

static void mkeyd_main(void)
{
	int ret;

	ret = ubus_add_object(ctx, &mkeyd_object);
	if (ret) {
        fprintf(stderr, "Failed to add object: %s\n", ubus_strerror(ret));
    }

#if 0
	ret = ubus_register_subscriber(ctx, &mkeyd_event);
	if (ret) {
        fprintf(stderr, "Failed to add watch handler: %s\n", ubus_strerror(ret));
    }
#endif
    mkeyd_init();

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

	mkeyd_main();

	ubus_free(ctx);
	uloop_done();

	return 0;
}
