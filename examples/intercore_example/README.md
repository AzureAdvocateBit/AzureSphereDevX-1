# Intercore Example

## Functions

```c
bool dx_interCoreSendMessage(INTERCORE_CONTEXT* intercore_ctx, void* control_block, size_t message_length);
bool dx_interCoreCommunicationsEnable(INTERCORE_CONTEXT* intercore_ctx);
```

## Structures

GPIO context structure.

```c
typedef struct {
	char* rtAppComponentId;
	int sockFd;
	bool nonblocking_io;
	void (*interCoreCallback)(void*, ssize_t message_size);
	void* intercore_recv_block;
	size_t intercore_recv_block_length;
} INTERCORE_CONTEXT;
```

## Intercore example

Declare intercore message structure/shape

```c
INTERCORE_ENVIRONMENT_DATA_BLOCK_T intercore_send_block;
INTERCORE_ENVIRONMENT_DATA_BLOCK_T intercore_recv_block;
INTERCORE_ENVIRONMENT_T current_environment;
```


Declare intercore context.

```c
INTERCORE_CONTEXT intercore_environment_ctx = {
	.sockFd = -1, .nonblocking_io = true,
	.rtAppComponentId = CORE_ENVIRONMENT_COMPONENT_ID, .interCoreCallback = intercore_environment_receive_msg_handler,
	.intercore_recv_block = &intercore_recv_block, .intercore_recv_block_length = sizeof(intercore_recv_block)
};
```

Declare intercore callback.

```c
/// <summary>
/// Callback handler for Inter-Core Messaging 
/// </summary>
static void intercore_environment_receive_msg_handler(void* data_block, ssize_t message_length) {
	INTERCORE_ENVIRONMENT_DATA_BLOCK_T* block = (INTERCORE_ENVIRONMENT_DATA_BLOCK_T*)data_block;

	switch (block->ic_msg_type) {
	case IC_ENVIRONMENT:
		current_environment.temperature = block->environment.temperature;
		current_environment.pressure = block->environment.pressure;
		break;
	default:
		break;
	}
}

```

