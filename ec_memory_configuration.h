#ifndef __MSI_EC_REGISTERS_CONFIG__
#define __MSI_EC_REGISTERS_CONFIG__

#include <linux/types.h>

#define MSI_EC_DRIVER_NAME "msi-ec"

#define MSI_EC_ADDR_UNKNOWN 0xff01 // unknown address
#define MSI_EC_ADDR_UNSUPP  0xff01 // unsupported parameter

// Firmware info addresses are universal
#define MSI_EC_FW_VERSION_ADDRESS 0xa0
#define MSI_EC_FW_DATE_ADDRESS    0xac
#define MSI_EC_FW_TIME_ADDRESS    0xb4
#define MSI_EC_FW_VERSION_LENGTH  12
#define MSI_EC_FW_DATE_LENGTH     8
#define MSI_EC_FW_TIME_LENGTH     8

struct msi_ec_webcam_conf {
	int address;
	int block_address;
	int bit;
};

struct msi_ec_fn_win_swap_conf {
	int address;
	int bit;
	bool invert;
};

struct msi_ec_cooler_boost_conf {
	int address;
	int bit;
};

#define MSI_EC_MODE_NULL { NULL, 0 }
struct msi_ec_mode {
	const char *name;
	int value;
};

#define MSI_EC_SHIFT_MODE_NAME_LIMIT 20
struct msi_ec_shift_mode_conf {
	int address;
	struct msi_ec_mode modes[5]; // fixed size for easier hard coding
};

struct msi_ec_super_battery_conf {
	int address;
	int mask;
};

struct msi_ec_fan_mode_conf {
	int address;
	struct msi_ec_mode modes[5]; // fixed size for easier hard coding
};

/* Maximum number of (temperature, speed) pairs the EC curve table can hold.
 * Chosen to match the largest known MSI EC fan curve (typically 7 entries,
 * but we reserve extra headroom for future devices).
 */
#define CURVE_MAX_ENTRIES 16

/*
 * CURVE_APPLY_STRATEGY_NORMAL
 *
 * The custom curve is written to EC immediately and stays active in all fan
 * modes, including auto/silent.  Use this when the device handles non-advanced
 * modes correctly even with a modified curve present in EC memory.
 */
#define CURVE_APPLY_STRATEGY_NORMAL 0

/*
 * CURVE_APPLY_STRATEGY_RESET_ON_AUTO
 *
 * On some devices a custom curve left in EC memory corrupts fan behaviour when
 * a non-advanced mode (auto, silent, basic) is selected (see issue #138).
 * With this strategy the driver writes the custom curve only while "advanced"
 * fan_mode is active, and restores the factory defaults whenever another mode
 * is chosen.  The user-supplied values are still remembered in RAM so they are
 * re-applied the next time "advanced" is selected.
 */
#define CURVE_APPLY_STRATEGY_RESET_ON_AUTO 1

/*
 * msi_ec_fan_curve - EC register layout for a single fan's thermal curve.
 *
 * The EC stores the fan speed table starting at @speed_start_address and
 * the temperature thresholds starting at @temperature_start_address.  Both
 * arrays are laid out in consecutive bytes (one byte per entry).
 *
 * There is one more speed entry than temperature entry: the first speed value
 * applies to temperatures below the first threshold, so no lower bound
 * temperature is stored.  With N entries you get N speeds and (N-1) temps.
 */
struct msi_ec_fan_curve {
	int speed_start_address;       /* EC address of the first fan-speed byte */
	int temperature_start_address; /* EC address of the first threshold byte  */
	int entries_count;             /* Total number of speed steps (N)          */

	/* One of CURVE_APPLY_STRATEGY_* above; defaults to _NORMAL (= 0). */
	int apply_strategy;
};

struct msi_ec_cpu_conf {
	int rt_temp_address;      /* EC register: real-time CPU temperature (°C) */
	int rt_fan_speed_address; /* EC register: real-time CPU fan speed (%)    */

	struct msi_ec_fan_curve fan_curve;
};

struct msi_ec_gpu_conf {
	int rt_temp_address;      /* EC register: real-time GPU temperature (°C) */
	int rt_fan_speed_address; /* EC register: real-time GPU fan speed (%)    */

	struct msi_ec_fan_curve fan_curve;
};

struct msi_ec_led_conf {
	int micmute_led_address;
	int mute_led_address;
	int bit;
};

#define MSI_EC_KBD_BL_STATE_MASK 0x3
struct msi_ec_kbd_bl_conf {
	int bl_mode_address;
	int bl_modes[2];
	int max_mode;

	int bl_state_address;
	int state_base_value;
	int max_state;
};

struct msi_ec_conf {
	const char **allowed_fw;

	int charge_control_address;
	struct msi_ec_webcam_conf         webcam;
	struct msi_ec_fn_win_swap_conf    fn_win_swap;
	struct msi_ec_cooler_boost_conf   cooler_boost;
	struct msi_ec_shift_mode_conf     shift_mode;
	struct msi_ec_super_battery_conf  super_battery;
	struct msi_ec_fan_mode_conf       fan_mode;
	struct msi_ec_cpu_conf            cpu;
	struct msi_ec_gpu_conf            gpu;
	struct msi_ec_led_conf            leds;
	struct msi_ec_kbd_bl_conf         kbd_bl;
};

#endif // __MSI_EC_REGISTERS_CONFIG__
