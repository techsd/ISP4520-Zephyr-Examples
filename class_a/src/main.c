/*
 * Class A LoRaWAN sample application
 *
 * Copyright (c) 2020 Manivannan Sadhasivam <mani@kernel.org>
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#include <zephyr/device.h>
#include <zephyr/kernel.h>
#include <zephyr/drivers/uart.h>
#include <zephyr/drivers/lora.h>
#include <zephyr/drivers/gpio.h>

#define LOG_LEVEL CONFIG_LOG_DEFAULT_LEVEL
#include <zephyr/logging/log.h>
LOG_MODULE_REGISTER(lora_class_a);

#define DELAY K_MSEC(10000)
#define TIME_FOR_POLL 9
#define SOS_TIME_TO_SEND 3
#define TIME_TO_FIRST_SEARCH_SATELITS 90

#define _RX_PIN_NUMBER 6
#define _TX_PIN_NUMBER 8
#define _RTS_PIN_NUMBER 14
#define _CTS_PIN_NUMBER 15

#define PIN_LED 30
#define PIN_3V3ON 5
#define GNSS_EXTINT 12
#define GNSS_3D_FIX 17
#define GNSS_JAM_IND 7
#define GNSS_FORCE_ON 22
#define GNSS_RESET 13
#define ON_MEAS_CIRC 4
#define BUTTON_IN 2
#define ADC_IN 31

static const struct device *lora_dev;
static const struct device *uart_dev;
static const struct device *gpio_dev;
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
	struct lora_modem_config config;
	int ret;

	lora_dev = DEVICE_DT_GET(DT_ALIAS(lora0));
	if (!device_is_ready(lora_dev)) {
		LOG_ERR("%s: device not ready.", lora_dev->name);
		return;
	}

	uart_dev = device_get_binding(DT_LABEL(DT_NODELABEL(uart0)));
	if (!uart_dev) {
		LOG_ERR("UART device not found");
		return;
	}

	gpio_dev = device_get_binding(DT_LABEL(DT_NODELABEL(gpio0)));
	if (!gpio_dev) {
		LOG_ERR("GPIO device not found");
		return;
	}

	gpio_pin_configure(gpio_dev, PIN_LED, GPIO_OUTPUT);
	gpio_pin_configure(gpio_dev, PIN_3V3ON, GPIO_OUTPUT);
	gpio_pin_configure(gpio_dev, GNSS_EXTINT, GPIO_OUTPUT);
	gpio_pin_configure(gpio_dev, GNSS_3D_FIX, GPIO_INPUT);
	gpio_pin_configure(gpio_dev, GNSS_JAM_IND, GPIO_INPUT);
	gpio_pin_configure(gpio_dev, GNSS_FORCE_ON, GPIO_OUTPUT);
	gpio_pin_configure(gpio_dev, GNSS_RESET, GPIO_OUTPUT);
	gpio_pin_configure(gpio_dev, ON_MEAS_CIRC, GPIO_OUTPUT);
	gpio_pin_configure(gpio_dev, BUTTON_IN, GPIO_INPUT);
	gpio_pin_configure(gpio_dev, ADC_IN, GPIO_INPUT);

	uart_callback_set(uart_dev, uart_cb, NULL);
	uart_rx_enable(uart_dev, uart_buffer, sizeof(uart_buffer), 100);

	config.frequency = 868100000;
	config.bandwidth = BW_125_KHZ;
	config.datarate = SF_7;
	config.preamble_len = 8;
	config.coding_rate = CR_4_5;
	config.tx_power = 14;
	config.tx = true;

	ret = lora_config(lora_dev, &config);
	if (ret < 0) {
		LOG_ERR("LoRa config failed: %d", ret);
		return;
	}

	LOG_INF("Sending data...");
	while (1) {
		if (uart_buffer_len > 0) {
			ret = lora_send(lora_dev, uart_buffer, uart_buffer_len);
			uart_buffer_len = 0;

			if (ret < 0) {
				LOG_ERR("LoRa send failed: %d", ret);
				return;
			}

			LOG_INF("Data sent!");

			// Update frequency for next transmission
			config.frequency += config.bandwidth * 1000;
			if (config.frequency > 868500000) {
				config.frequency = 868100000;
			}

			ret = lora_config(lora_dev, &config);
			if (ret < 0) {
				LOG_ERR("LoRa config update failed: %d", ret);
				return;
			}
		}
		k_sleep(DELAY);
	}
}
