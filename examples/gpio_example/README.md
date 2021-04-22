# GPIO Example

## Developer boards with onboard buttons

Press button A and the user LED will blink for 1 second

## Developer boards without onboard buttons

The user LED will blink every 500ms

## Enums

```c
typedef enum
{
	DX_DIRECTION_UNKNOWN,
	DX_INPUT,
	DX_OUTPUT
} DX_GPIO_DIRECTION;
```

```c
typedef enum {
	DX_GPIO_DETECT_LOW,
	DX_GPIO_DETECT_HIGH,
	DX_GPIO_DETECT_BOTH
} DX_GPIO_INPUT_DETECT;
```

## Functions

```c
bool dx_gpioOpen(DX_GPIO* gpio);
bool dx_gpioStateGet(DX_GPIO* gpio, GPIO_Value_Type* oldState);
void dx_gpioClose(DX_GPIO* gpio);
void dx_gpioOff(DX_GPIO* gpio);
void dx_gpioOn(DX_GPIO* gpio);
void dx_gpioSetClose(DX_GPIO** gpioSet, size_t gpioSetCount);
void dx_gpioSetOpen(DX_GPIO** gpioSet, size_t gpioSetCount);
void dx_gpioStateSet(DX_GPIO* gpio, bool state);
```

## Structures

GPIO context structure.

```c
struct _dx_gpio
{
	int fd;
	int pin;
	GPIO_Value initialState;
	bool invertPin;
	DX_GPIO_INPUT_DETECT detect;
	char* name;
	DX_GPIO_DIRECTION direction;
	bool opened;
};
```

