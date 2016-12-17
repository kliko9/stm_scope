#pragma once

#include <functional>
#include <vector>
#include <string>

#include "stm32f4xx.h"

#define DEVICE_NAME "STM_SCOPE"
#define DEVICE_PIN "0000"

#define UART_CMD_STATE "AT"
#define UART_CMD_GET_ADDRESS "AT+ADDR"
#define UART_CMD_SET_BAUD_9600 "AT+BAUD4"
#define UART_CMD_SET_BAUD_115200 "AT+BAUD8"
#define UART_CMD_SET_NAME "AT+NAME" DEVICE_NAME
#define UART_CMD_SET_PIN "AT+PIN" DEVICE_PIN

#define RECV_MAX_LENGTH 256

namespace sys {

class UART {
public:
	virtual ~UART();
	static UART &Instance();
	UART(const UART &) = delete;
	UART operator=(const UART &) = delete;

	void RegisterDataSetter(const std::function<char()> &fn);
	void RegisterDataReceiver(const std::function<void(const char *)> &fn);
	std::vector<std::function<void(const char *)>> &Receivers() { return receivers_; };
	std::function<char()> &Setter() {return setter_; };
	const char *Buffer() { return buffer_; };

	void BeginTransmission();
	void StopTransmission();

	void SendCmd(const char *cmd, unsigned length);
	void AppendBuffer(char s);

private:
	UART();

	const char *cmd_ = nullptr;
	unsigned cmd_len_ = 0;

	unsigned buffer_idx_ = 0;
	char buffer_[RECV_MAX_LENGTH] = {0, };

	std::function<char()> setter_;
	std::vector<std::function<void(const char *)>> receivers_;

	void GPIOInit();
	void UARTInit();
	char WriteCmd();
};

} // system
