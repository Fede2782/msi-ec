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

struct msi_ec_usb_powershare_conf {
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

/*
 * Fan curve
 *
 * A fan curve is made of N fan speeds (s0 .. s(N-1)) and N-1 temperature
 * thresholds (t1 .. t(N-1)). The fan runs at s0 below t1, at s1 between
 * t1 and t2, ..., and at s(N-1) from t(N-1) upwards.
 *
 * In the EC memory the speeds and the temperatures are stored as two
 * separate arrays of consecutive bytes:
 *   speed_start_address       -> s0, s1, ..., s(N-1)  (N bytes)
 *   temperature_start_address -> t1, t2, ..., t(N-1)  (N-1 bytes)
 *
 * The EC most likely also has a t0 byte right before t1, but its meaning
 * is unclear, so the driver never touches it.
 */

/* Upper bound for entries_count, kept above any known device for extensibility. */
#define MSI_EC_FAN_CURVE_MAX_ENTRIES 16

/*
 * The custom curve is kept in the EC in every fan mode.
 */
#define MSI_EC_FAN_CURVE_APPLY_NORMAL 0

/*
 * The custom curve is only written to the EC while the fan mode is
 * "advanced". In every other fan mode the EC gets the default curve back.
 * Required on devices where a custom curve in the EC breaks the fan
 * control of the non-advanced modes.
 */
#define MSI_EC_FAN_CURVE_APPLY_RESET_ON_AUTO 1

struct msi_ec_fan_curve_conf {
	int speed_start_address;       // MSI_EC_ADDR_UNSUPP if unsupported
	int temperature_start_address; // MSI_EC_ADDR_UNSUPP if unsupported
	int entries_count;             // N, number of fan speeds
	int apply_strategy;            // MSI_EC_FAN_CURVE_APPLY_*
};

struct msi_ec_cpu_conf {
	int rt_temp_address;
	int rt_fan_speed_address; // realtime % RPM
	struct msi_ec_fan_curve_conf fan_curve;
};

struct msi_ec_gpu_conf {
	int rt_temp_address;
	int rt_fan_speed_address; // realtime % RPM
	struct msi_ec_fan_curve_conf fan_curve;
};

struct msi_ec_led_conf {
	int micmute_led_address;
	int mute_led_address;
	int bit;
};

#define MSI_EC_KBD_BL_STATE_MASK 0x3
struct msi_ec_kbd_bl_conf {
	int bl_mode_address;
	struct msi_ec_mode bl_modes[5];
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
	struct msi_ec_usb_powershare_conf usb_powershare;
	struct msi_ec_shift_mode_conf     shift_mode;
	struct msi_ec_super_battery_conf  super_battery;
	struct msi_ec_fan_mode_conf       fan_mode;
	struct msi_ec_cpu_conf            cpu;
	struct msi_ec_gpu_conf            gpu;
	struct msi_ec_led_conf            leds;
	struct msi_ec_kbd_bl_conf         kbd_bl;
};

#endif // __MSI_EC_REGISTERS_CONFIG__
