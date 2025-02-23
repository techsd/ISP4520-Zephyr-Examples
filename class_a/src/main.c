/*
 * Class A LoRaWAN sample application
 *
 * Copyright (c) 2020 Manivannan Sadhasivam <mani@kernel.org>
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#include <zephyr/device.h>
#include <zephyr/lorawan/lorawan.h>
#include <zephyr/kernel.h>
#include <zephyr/drivers/uart.h>

/* Customize based on network configuration */
#define LORAWAN_DEV_EUI			{ 0xDD, 0xEE, 0xAA, 0xDD, 0xBB, 0xEE,\
					  0xEE, 0xFF }
#define LORAWAN_JOIN_EUI		{ 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,\
					  0x00, 0x00 }
#define LORAWAN_APP_KEY			{ 0x2B, 0x7E, 0x15, 0x16, 0x28, 0xAE,\
					  0xD2, 0xA6, 0xAB, 0xF7, 0x15, 0x88,\
					  0x09, 0xCF, 0x4F, 0x3C }

#define DELAY K_MSEC(10000)

#define LOG_LEVEL CONFIG_LOG_DEFAULT_LEVEL
#include <zephyr/logging/log.h>
LOG_MODULE_REGISTER(lorawan_class_a);

char data[] = {'h', 'e', 'l', 'l', 'o', 'w', 'o', 'r', 'l', 'd'};

static void dl_callback(uint8_t port, bool data_pending,
			int16_t rssi, int8_t snr,
			uint8_t len, const uint8_t *data)
{
	LOG_INF("Port %d, Pending %d, RSSI %ddB, SNR %ddBm", port, data_pending, rssi, snr);
	if (data) {
		LOG_HEXDUMP_INF(data, len, "Payload: ");
	}
}

static void lorwan_datarate_changed(enum lorawan_datarate dr)
{
	uint8_t unused, max_size;

	lorawan_get_payload_sizes(&unused, &max_size);
	LOG_INF("New Datarate: DR_%d, Max Payload %d", dr, max_size);
}

static const struct device *uart_dev;
static uint8_t uart_buffer[256];
static volatile size_t uart_buffer_len;

static void uart_cb(const struct device *dev, struct uart_event *evt, void *user_data)
{
	switch (evt->type) {
	case UART_TX_DONE:
		LOG_INF("UART TX done");
		break;
	case UART_RX_RDY:
		memcpy(uart_buffer + uart_buffer_len, evt->data.rx.buf, evt->data.rx.len);
		uart_buffer_len += evt->data.rx.len;
		break;
	case UART_RX_DISABLED:
		LOG_INF("UART RX disabled");
		break;
	case UART_RX_BUF_REQUEST:
		uart_rx_buf_rsp(dev, uart_buffer, sizeof(uart_buffer));
		break;
	case UART_RX_BUF_RELEASED:
		LOG_INF("UART RX buffer released");
		break;
	case UART_RX_STOPPED:
		LOG_ERR("UART RX stopped");
		break;
	case UART_RX_ABORTED:
		LOG_ERR("UART RX aborted");
		break;
	default:
		break;
	}
}

void main(void)
{
	const struct device *lora_dev;
	struct lorawan_join_config join_cfg;
	uint8_t dev_eui[] = LORAWAN_DEV_EUI;
	uint8_t join_eui[] = LORAWAN_JOIN_EUI;
	uint8_t app_key[] = LORAWAN_APP_KEY;
	int ret;

	struct lorawan_downlink_cb downlink_cb = {
		.port = LW_RECV_PORT_ANY,
		.cb = dl_callback
	};

	lora_dev = DEVICE_DT_GET(DT_ALIAS(lora0));
	if (!device_is_ready(lora_dev)) {
		LOG_ERR("%s: device not ready.", lora_dev->name);
		return;
	}

	ret = lorawan_start();
	if (ret < 0) {
		LOG_ERR("lorawan_start failed: %d", ret);
		return;
	}

	lorawan_register_downlink_callback(&downlink_cb);
	lorawan_register_dr_changed_callback(lorwan_datarate_changed);

	join_cfg.mode = LORAWAN_ACT_OTAA;
	join_cfg.dev_eui = dev_eui;
	join_cfg.otaa.join_eui = join_eui;
	join_cfg.otaa.app_key = app_key;
	join_cfg.otaa.nwk_key = app_key;

	LOG_INF("Joining network over OTAA");
	ret = lorawan_join(&join_cfg);
	if (ret < 0) {
		LOG_ERR("lorawan_join_network failed: %d", ret);
		return;
	}

	uart_dev = device_get_binding(DT_LABEL(DT_NODELABEL(uart0)));
	if (!uart_dev) {
		LOG_ERR("UART device not found");
		return;
	}

	uart_callback_set(uart_dev, uart_cb, NULL);
	uart_rx_enable(uart_dev, uart_buffer, sizeof(uart_buffer), 100);

	LOG_INF("Sending data...");
	while (1) {
		if (uart_buffer_len > 0) {
			ret = lorawan_send(2, uart_buffer, uart_buffer_len, LORAWAN_MSG_CONFIRMED);
			uart_buffer_len = 0;

			if (ret == -EAGAIN) {
				LOG_ERR("lorawan_send failed: %d. Continuing...", ret);
				k_sleep(DELAY);
				continue;
			}

			if (ret < 0) {
				LOG_ERR("lorawan_send failed: %d", ret);
				return;
			}

			LOG_INF("Data sent!");
		}
		k_sleep(DELAY);
	}
}
