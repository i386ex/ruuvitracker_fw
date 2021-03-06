// eLua platform configuration

#ifndef __PLATFORM_CONF_H__
#define __PLATFORM_CONF_H__

#include "auxmods.h"
#include "type.h"
#include "stacks.h"
#include "stm32f4xx_conf.h"
#include "elua_int.h"
#include "sermux.h"

// *****************************************************************************
// Define here what components you want for this platform

#define BUILD_XMODEM
#define BUILD_SHELL
#define BUILD_ROMFS
#define BUILD_MMCFS
#define BUILD_TERM
//#define BUILD_UIP
//#define BUILD_DHCPC
//#define BUILD_DNS
#define BUILD_CON_GENERIC
//#define BUILD_ADC
//#define BUILD_RPC
//#define BUILD_RFS
//#define BUILD_CON_TCP
#define BUILD_LINENOISE
#define BUILD_C_INT_HANDLERS
#define BUILD_LUA_INT_HANDLERS
//#define ENABLE_ENC
#define BUILD_GSM
#define BUILD_GPS

#define PLATFORM_HAS_SYSTIMER

//#define ENABLE_TRACE       // Enable the ETM TRACE interface (TRACECLK, TRACE D0-3)
#define ENABLE_JTAG_SWD    // Do NOT remove this unless you really not need JTAG/SWD

// *****************************************************************************
// UART/Timer IDs configuration data (used in main.c)

#define CON_UART_ID           0
#define CON_UART_SPEED        115200
#define TERM_LINES            25
#define TERM_COLS             80

// *****************************************************************************
// Auxiliary libraries that will be compiled for this platform

// Number of resources (0 if not available/not implemented)
#define NUM_PIO               5
#define NUM_SPI               1
#define NUM_UART              3
#define NUM_TIMER             12
#define NUM_PHYS_TIMER        12
#define NUM_PWM               0
#define NUM_ADC               16
#define NUM_CAN               0
#define NUM_I2C               1

//#ifdef FORSTM3210E_EVAL
//#define AUXLIB_LCD      "stm3210lcd"
//LUALIB_API int ( luaopen_lcd )( lua_State* L );
//#define LCDLINE  _ROM( AUXLIB_LCD, luaopen_lcd, lcd_map )
//#else
#define LCDLINE
//#endif

#ifdef ENABLE_ENC
#define PS_LIB_TABLE_NAME "stm32"
#endif


#ifdef BUILD_ADC
#define ADCLINE _ROM( AUXLIB_ADC, luaopen_adc, adc_map )
#else
#define ADCLINE
#endif

#if defined( ELUA_BOOT_RPC ) && !defined( BUILD_RPC )
#define BUILD_RPC
#endif

#if defined( BUILD_RPC )
#define RPCLINE _ROM( AUXLIB_RPC, luaopen_rpc, rpc_map )
#else
#define RPCLINE
#endif

#ifdef PS_LIB_TABLE_NAME
#define PLATLINE _ROM( PS_LIB_TABLE_NAME, luaopen_platform, platform_map )
#else
#define PLATLINE
#endif

#ifdef BUILD_GSM
extern int luaopen_gsm( lua_State *L );
#define GSMLINE _ROM( "gsm", luaopen_gsm, gsm_map ) _ROM("http", luaopen_gsm, http_map)
#else
#define GSMLINE
#endif

#ifdef BUILD_GPS
extern int luaopen_gps( lua_State *L );
#define GPSLINE _ROM( "gps", luaopen_gps, gps_map )
#else
#define GPSLINE
#endif

/* Ruuvi-Lib */
extern int luaopen_ruuvi( lua_State *L );

#define LUA_PLATFORM_LIBS_ROM\
  _ROM( AUXLIB_PIO, luaopen_pio, pio_map )\
  _ROM( AUXLIB_SPI, luaopen_spi, spi_map )\
  _ROM( AUXLIB_I2C, luaopen_i2c, i2c_map) \
  _ROM( AUXLIB_PD, luaopen_pd, pd_map )\
  _ROM( AUXLIB_UART, luaopen_uart, uart_map )\
  _ROM( AUXLIB_TERM, luaopen_term, term_map )\
  _ROM( AUXLIB_PACK, luaopen_pack, pack_map )\
  _ROM( AUXLIB_BIT, luaopen_bit, bit_map )\
  _ROM( AUXLIB_CPU, luaopen_cpu, cpu_map )\
  _ROM( AUXLIB_ELUA, luaopen_elua, elua_map )\
  _ROM( AUXLIB_TMR, luaopen_tmr, tmr_map )\
  ADCLINE\
/*  _ROM( AUXLIB_CAN, luaopen_can, can_map )*/ \
  _ROM( AUXLIB_PWM, luaopen_pwm, pwm_map )\
  RPCLINE\
  LCDLINE\
  _ROM( AUXLIB_ELUA, luaopen_elua, elua_map )\
  _ROM( LUA_MATHLIBNAME, luaopen_math, math_map )\
  _ROM( "ruuvi", luaopen_ruuvi, ruuvi_map ) \
  GSMLINE \
  GPSLINE \
  PLATLINE

// *****************************************************************************
// Configuration data

#define EGC_INITIAL_MODE      1

// Virtual timers (0 if not used)
#define VTMR_NUM_TIMERS       4
#define VTMR_FREQ_HZ          10

// GSM+GPS serial ports
#if defined( ELUA_BOARD_RUUVIB1 )
#define GSM_UART_ID           1
#define GPS_UART_ID           2
#elif defined (ELUA_BOARD_RUUVIC1) || defined (ELUA_BOARD_RUUVIC2)
#define GSM_UART_ID           2
#define GPS_UART_ID           1
#endif

// Enable RX buffering on UART
//#define BUF_ENABLE_UART
//#define CON_BUF_SIZE          BUF_SIZE_128

// ADC Configuration Params
#define ADC_BIT_RESOLUTION    12
#define BUF_ENABLE_ADC
#define ADC_BUF_SIZE          BUF_SIZE_128

// These should be adjusted to support multiple ADC devices
#define ADC_TIMER_FIRST_ID    0
#define ADC_NUM_TIMERS        2

// RPC boot options
#define RPC_UART_ID           CON_UART_ID
#define RPC_UART_SPEED        CON_UART_SPEED


#if defined(BUILD_MMCFS)
// MMCFS Support (FatFs on SD/MMC)
// For STM32F407VGT6 - PA5 = CLK, PA6 = MISO, PA7 = MOSI, PA4 = CS
#define MMCFS_TICK_HZ                10
#define MMCFS_TICK_MS                ( 1000 / MMCFS_TICK_HZ )
#if defined (ELUA_BOARD_RUUVIC1) || defined (ELUA_BOARD_RUUVIC2)
#define MMCFS_CS_PORT                1 // PB, see platform.c, keys start from 0
#define MMCFS_CS_PIN                 15
#define MMCFS_SPI_NUM                0 // SPI1, see platform.c, keys start from zero
#else
#error "Define SDCard MMCFS_XXX constants in platform_conf.h"
#endif
#endif // defined(BUILD_MMCFS)

// CPU frequency (needed by the CPU module, 0 if not used)
u32 platform_s_cpu_get_frequency();
#define CPU_FREQUENCY         platform_s_cpu_get_frequency()

// PIO prefix ('0' for P0, P1, ... or 'A' for PA, PB, ...)
#define PIO_PREFIX            'A'
// Pins per port configuration:
// #define PIO_PINS_PER_PORT (n) if each port has the same number of pins, or
// #define PIO_PIN_ARRAY { n1, n2, ... } to define pins per port in an array
// Use #define PIO_PINS_PER_PORT 0 if this isn't needed
#define PIO_PINS_PER_PORT     16

// Remote file system data
#define RFS_BUFFER_SIZE       BUF_SIZE_512
#define RFS_UART_ID           0
#define RFS_TIMEOUT           100000
#define RFS_UART_SPEED        115200

// Linenoise buffer sizes
#define LINENOISE_HISTORY_SIZE_LUA    50
#define LINENOISE_HISTORY_SIZE_SHELL  10

// Allocator data: define your free memory zones here in two arrays
// (start address and end address)
// The F4 has 196K in total, but it is split into 3 sections:
// SRAM (xrw) : ORIGIN = 0x20000000, SIZE = 128K      // Major SRAM
// CCMDATARAM (xrw) : ORIGIN = 0x10000000, SIZE = 64K    // Core Coupled Data SRAM
// BKPSRAM, SIZE = 4K // Backup SRAM
//
#define CCMDATARAM_SIZE         ( 64 * 1024 )
#define SRAM_SIZE               ( 128 * 1024 )
#define MEM_START_ADDRESS       { ( void* )end, (void* )( CCMDATARAM_BASE ) }
#define MEM_END_ADDRESS         { ( void* )( SRAM_BASE + SRAM_SIZE - STACK_SIZE_TOTAL - 1 ), (void*)( CCMDATARAM_BASE + CCMDATARAM_SIZE - 1 ) }

// Interrupt queue size
#define PLATFORM_INT_QUEUE_LOG_SIZE 5

// Interrupt list
#define INT_GPIO_POSEDGE      ELUA_INT_FIRST_ID
#define INT_GPIO_NEGEDGE      ( ELUA_INT_FIRST_ID + 1 )
#define INT_TMR_MATCH         ( ELUA_INT_FIRST_ID + 2 )
#define INT_UART_RX           ( ELUA_INT_FIRST_ID + 3 )
#define INT_GSM_CALL          ( ELUA_INT_FIRST_ID + 4 )
#define INT_GSM_SMS           ( ELUA_INT_FIRST_ID + 5 )
#define INT_ELUA_LAST         INT_GSM_SMS

#define PLATFORM_CPU_CONSTANTS\
  _C( INT_GPIO_POSEDGE ),     \
  _C( INT_GPIO_NEGEDGE ),     \
  _C( INT_TMR_MATCH ),        \
       _C( INT_UART_RX ),     \
       _C( INT_GSM_CALL ),    \
       _C( INT_GSM_SMS )

// Conver from GPIO_PinSourceXX to GPIO_Pin_XX
#define GPIO_SOURCE2PIN(n)    (1 << (n))

#endif // #ifndef __PLATFORM_CONF_H__

