/**
 * \file
 *
 * \brief Two-Wire Interface High Speed (TWIHS) driver for SAM.
 *
 * Copyright (c) 2013-2018 Microchip Technology Inc. and its subsidiaries.
 *
 * \asf_license_start
 *
 * \page License
 *
 * Subject to your compliance with these terms, you may use Microchip
 * software and any derivatives exclusively with Microchip products.
 * It is your responsibility to comply with third party license terms applicable
 * to your use of third party software (including open source software) that
 * may accompany Microchip software.
 *
 * THIS SOFTWARE IS SUPPLIED BY MICROCHIP "AS IS". NO WARRANTIES,
 * WHETHER EXPRESS, IMPLIED OR STATUTORY, APPLY TO THIS SOFTWARE,
 * INCLUDING ANY IMPLIED WARRANTIES OF NON-INFRINGEMENT, MERCHANTABILITY,
 * AND FITNESS FOR A PARTICULAR PURPOSE. IN NO EVENT WILL MICROCHIP BE
 * LIABLE FOR ANY INDIRECT, SPECIAL, PUNITIVE, INCIDENTAL OR CONSEQUENTIAL
 * LOSS, DAMAGE, COST OR EXPENSE OF ANY KIND WHATSOEVER RELATED TO THE
 * SOFTWARE, HOWEVER CAUSED, EVEN IF MICROCHIP HAS BEEN ADVISED OF THE
 * POSSIBILITY OR THE DAMAGES ARE FORESEEABLE.  TO THE FULLEST EXTENT
 * ALLOWED BY LAW, MICROCHIP'S TOTAL LIABILITY ON ALL CLAIMS IN ANY WAY
 * RELATED TO THIS SOFTWARE WILL NOT EXCEED THE AMOUNT OF FEES, IF ANY,
 * THAT YOU HAVE PAID DIRECTLY TO MICROCHIP FOR THIS SOFTWARE.
 *
 * \asf_license_stop
 *
 */
/*
 * Support and FAQ: visit <a href="https://www.microchip.com/support/">Microchip Support</a>
 */

#ifndef TWIHS_H_INCLUDED
#define TWIHS_H_INCLUDED

#include "definitions.h"                // SYS function prototypes

/// @cond 0
/**INDENT-OFF**/
#ifdef __cplusplus
extern "C" {
#endif
/**INDENT-ON**/
/// @endcond

/** Time-out value (number of attempts). */
#define TWIHS_TIMEOUT              30000

/**
 * \brief Return codes for TWIHS APIs.
 * @{
 */
#define TWIHS_SUCCESS              0
#define TWIHS_INVALID_ARGUMENT     1
#define TWIHS_ARBITRATION_LOST     2
#define TWIHS_NO_CHIP_FOUND        3
#define TWIHS_RECEIVE_OVERRUN      4
#define TWIHS_RECEIVE_NACK         5
#define TWIHS_SEND_OVERRUN         6
#define TWIHS_SEND_NACK            7
#define TWIHS_BUSY                 8
#define TWIHS_ERROR_TIMEOUT        9
/**
 * @}
 */

#define PASS 1
#define FAIL 0    
    
/**
 * \brief Input parameters when initializing the TWIHS module mode.
 */
typedef struct twihs_options {
	//! MCK for TWIHS.
	uint32_t master_clk;
	//! The baud rate of the TWIHS bus.
	uint32_t speed;
	//! The desired address.
	uint8_t chip;
	//! SMBUS mode (set 1 to use SMBUS quick command, otherwise don't).
	uint8_t smbus;
} twihs_options_t;

/**
 * \brief Information concerning the data transmission.
 */
typedef struct twihs_packet {
	//! TWIHS address/commands to issue to the other chip (node).
	uint8_t addr[3];
	//! Length of the TWIHS data address segment (1-3 bytes).
	uint32_t addr_length;
	//! Where to find the data to be transferred.
	void *buffer;
	//! How many bytes do we want to transfer.
	uint32_t length;
	//! TWIHS chip address to communicate with.
	uint8_t chip;
} twihs_packet_t;

/**
 * \brief If Master mode is enabled, a SMBUS Quick Command is sent.
 *
 * \param p_twihs   Base address of the TWIHS instance.
 */
static inline void twihs_enable_smbus_quick_command(twihs_registers_t *p_twihs)
{
	p_twihs->TWIHS_CR = TWIHS_CR_QUICK_Msk;
}

/**
 * \brief Enable high speed mode.
 *
 * \param p_twihs   Base address of the TWIHS instance.
 */
static inline void twihs_enable_highspeed(twihs_registers_t *p_twihs)
{
	p_twihs->TWIHS_CR = TWIHS_CR_HSEN_Msk;
}

/**
 * \brief Disable high speed mode.
 *
 * \param p_twihs   Base address of the TWIHS instance.
 */
static inline void twihs_disable_highspeed(twihs_registers_t *p_twihs)
{
	p_twihs->TWIHS_CR = TWIHS_CR_HSDIS_Msk;
}

/**
 * \brief Enable SMBus mode.
 *
 * \param p_twihs   Base address of the TWIHS instance.
 */
static inline void twihs_enable_smbus(twihs_registers_t *p_twihs)
{
	p_twihs->TWIHS_CR = TWIHS_CR_SMBEN_Msk;
}

/**
 * \brief Disable SMBus mode.
 *
 * \param p_twihs   Base address of the TWIHS instance.
 */
static inline void twihs_disable_smbus(twihs_registers_t *p_twihs)
{
	p_twihs->TWIHS_CR = TWIHS_CR_SMBDIS_Msk;
}

/**
 * \brief Enable packet error checking.
 *
 * \param p_twihs   Base address of the TWIHS instance.
 */
static inline void twihs_enable_pec(twihs_registers_t *p_twihs)
{
	p_twihs->TWIHS_CR = TWIHS_CR_PECEN_Msk;
}

/**
 * \brief Disable packet error checking.
 *
 * \param p_twihs   Base address of the TWIHS instance.
 */
static inline void twihs_disable_pec(twihs_registers_t *p_twihs)
{
	p_twihs->TWIHS_CR = TWIHS_CR_PECDIS_Msk;
}

/**
 * \brief Request a packet error checking.
 *
 * \param p_twihs   Base address of the TWIHS instance.
 */
static inline void twihs_request_pec(twihs_registers_t *p_twihs)
{
	p_twihs->TWIHS_CR = TWIHS_CR_PECRQ_Msk;
}

/**
 * \brief If master mode is enabled, send a bus clear command.
 *
 * \param p_twihs   Base address of the TWIHS instance.
 */
static inline void twihs_send_clear(twihs_registers_t *p_twihs)
{
	p_twihs->TWIHS_CR = TWIHS_CR_CLEAR_Msk;
}

#if !(SAMV70 || SAMV71 || SAME70 || SAMS70)
/**
 * \brief Enable alternative command mode.
 *
 * \param p_twihs   Base address of the TWIHS instance.
 */
static inline void twihs_enable_alternative_command(twihs_registers_t *p_twihs)
{
	p_twihs->TWIHS_CR = TWIHS_CR_ACMEN_Msk;
}

/**
 * \brief Enable alternative command mode.
 *
 * \param p_twihs   Base address of the TWIHS instance.
 */
static inline void twihs_disable_alternative_command(twihs_registers_t *p_twihs)
{
	p_twihs->TWIHS_CR = TWIHS_CR_ACMDIS_Msk;
}
#endif

/**
 * \brief Normal value to be returned in the ACK cycle of the data phase in slave receiver mode.
 *
 * \param p_twihs   Base address of the TWIHS instance.
 */
static inline void twihs_disable_slave_nack(twihs_registers_t *p_twihs)
{
	p_twihs->TWIHS_SMR &= ~TWIHS_SMR_NACKEN_Msk;
}

/**
 * \brief NACK value to be returned in the ACK cycle of the data phase in slave receiver mode.
 *
 * \param p_twihs   Base address of the TWIHS instance.
 */
static inline void twihs_enable_slave_nack(twihs_registers_t *p_twihs)
{
	p_twihs->TWIHS_SMR |= TWIHS_SMR_NACKEN_Msk;
}

/**
 * \brief Acknowledge of the SMBus Default Address disabled.
 *
 * \param p_twihs   Base address of the TWIHS instance.
 */
static inline void twihs_disable_slave_default_addr(twihs_registers_t *p_twihs)
{
	p_twihs->TWIHS_SMR &= ~TWIHS_SMR_SMDA_Msk;
}

/**
 * \brief Acknowledge of the SMBus Default Address enabled.
 *
 * \param p_twihs   Base address of the TWIHS instance.
 */
static inline void twihs_enable_slave_default_addr(twihs_registers_t *p_twihs)
{
	p_twihs->TWIHS_SMR |= TWIHS_SMR_SMDA_Msk;
}

/**
 * \brief Acknowledge of the SMBus Host Header disabled.
 *
 * \param p_twihs   Base address of the TWIHS instance.
 */
static inline void twihs_disable_smbus_host_header(twihs_registers_t *p_twihs)
{
	p_twihs->TWIHS_SMR &= ~TWIHS_SMR_SMHH_Msk;
}

/**
 * \brief Acknowledge of the SMBus Host Header enabled.
 *
 * \param p_twihs   Base address of the TWIHS instance.
 */
static inline void twihs_enable_smbus_host_header(twihs_registers_t *p_twihs)
{
	p_twihs->TWIHS_SMR |= TWIHS_SMR_SMHH_Msk;
}

/**
 * \brief Clock stretching disabled in slave mode, OVRE and UNRE will indicate overrun and underrun.
 *
 * \param p_twihs   Base address of the TWIHS instance.
 */
static inline void twihs_disable_clock_wait_state(twihs_registers_t *p_twihs)
{
	p_twihs->TWIHS_SMR |= TWIHS_SMR_SCLWSDIS_Msk;
}

/**
 * \brief Clear clock wait state disable mode.
 *
 * \param p_twihs   Base address of the TWIHS instance.
 */
static inline void twihs_clear_disable_clock_wait_state(twihs_registers_t *p_twihs)
{
	p_twihs->TWIHS_SMR &= ~TWIHS_SMR_SCLWSDIS_Msk;
}

#if (SAMG53 || SAMG54 || SAMV70 || SAMV71 || SAME70 || SAMS70)
/**
 * \brief Slave Address 1 matching disabled.
 *
 * \param p_twihs   Base address of the TWIHS instance.
 */
static inline void twihs_disable_slave_addr1_matching(twihs_registers_t *p_twihs)
{
	p_twihs->TWIHS_SMR &= ~TWIHS_SMR_SADR1EN_Msk;
}

/**
 * \brief Slave Address 1 matching enabled.
 *
 * \param p_twihs   Base address of the TWIHS instance.
 */
static inline void twihs_enable_slave_addr1_matching(twihs_registers_t *p_twihs)
{
	p_twihs->TWIHS_SMR |= TWIHS_SMR_SADR1EN_Msk;
}

/**
 * \brief Slave Address 2 matching disabled.
 *
 * \param p_twihs   Base address of the TWIHS instance.
 */
static inline void twihs_disable_slave_addr2_matching(twihs_registers_t *p_twihs)
{
	p_twihs->TWIHS_SMR &= ~TWIHS_SMR_SADR2EN_Msk;
}

/**
 * \brief Slave Address 2 matching enabled.
 *
 * \param p_twihs   Base address of the TWIHS instance.
 */
static inline void twihs_enable_slave_addr2_matching(twihs_registers_t *p_twihs)
{
	p_twihs->TWIHS_SMR |= TWIHS_SMR_SADR2EN_Msk;
}

/**
 * \brief Slave Address 3 matching disabled.
 *
 * \param p_twihs   Base address of the TWIHS instance.
 */
static inline void twihs_disable_slave_addr3_matching(twihs_registers_t *p_twihs)
{
	p_twihs->TWIHS_SMR &= ~TWIHS_SMR_SADR3EN_Msk;
}

/**
 * \brief Slave Address 3 matching enabled.
 *
 * \param p_twihs   Base address of the TWIHS instance.
 */
static inline void twihs_enable_slave_addr3_matching(twihs_registers_t *p_twihs)
{
	p_twihs->TWIHS_SMR |= TWIHS_SMR_SADR3EN_Msk;
}

/**
 * \brief First received data matching disabled.
 *
 * \param p_twihs   Base address of the TWIHS instance.
 */
static inline void twihs_disable_slave_data_matching(twihs_registers_t *p_twihs)
{
	p_twihs->TWIHS_SMR &= ~TWIHS_SMR_DATAMEN_Msk;
}

/**
 * \brief First received data matching enabled.
 *
 * \param p_twihs   Base address of the TWIHS instance.
 */
static inline void twihs_enable_slave_data_matching(twihs_registers_t *p_twihs)
{
	p_twihs->TWIHS_SMR |= TWIHS_SMR_DATAMEN;
}

void twihs_set_sleepwalking(twihs_registers_t *p_twihs,
		uint32_t ul_matching_addr1, bool flag1,
		uint32_t ul_matching_addr2, bool flag2,
		uint32_t ul_matching_addr3, bool flag3,
		uint32_t ul_matching_data, bool flag);
#endif

void twihs_enable_master_mode(twihs_registers_t *p_twihs);
void twihs_disable_master_mode(twihs_registers_t *p_twihs);
uint32_t twihs_master_init(twihs_registers_t *p_twihs, const twihs_options_t *p_opt);
uint32_t twihs_set_speed(twihs_registers_t *p_twihs, uint32_t ul_speed, uint32_t ul_mck);
uint32_t twihs_probe(twihs_registers_t *p_twihs, uint8_t uc_slave_addr);
uint32_t twihs_master_read(twihs_registers_t *p_twihs, twihs_packet_t *p_packet);
uint32_t twihs_master_write(twihs_registers_t *p_twihs, twihs_packet_t *p_packet);
void twihs_enable_interrupt(twihs_registers_t *p_twihs, uint32_t ul_sources);
void twihs_disable_interrupt(twihs_registers_t *p_twihs, uint32_t ul_sources);
uint32_t twihs_get_interrupt_status(twihs_registers_t *p_twihs);
uint32_t twihs_get_interrupt_mask(twihs_registers_t *p_twihs);
uint8_t twihs_read_byte(twihs_registers_t *p_twihs);
void twihs_write_byte(twihs_registers_t *p_twihs, uint8_t uc_byte);
void twihs_enable_slave_mode(twihs_registers_t *p_twihs);
void twihs_disable_slave_mode(twihs_registers_t *p_twihs);
void twihs_slave_init(twihs_registers_t *p_twihs, uint32_t ul_device_addr);
void twihs_set_slave_addr(twihs_registers_t *p_twihs, uint32_t ul_device_addr);
uint32_t twihs_slave_read(twihs_registers_t *p_twihs, uint8_t *p_data);
uint32_t twihs_slave_write(twihs_registers_t *p_twihs, uint8_t *p_data);
void twihs_reset(twihs_registers_t *p_twihs);
void twihs_set_write_protection(twihs_registers_t *p_twihs, bool flag);
void twihs_read_write_protection_status(twihs_registers_t *p_twihs, uint32_t *p_status);
void twihs_smbus_set_timing(twihs_registers_t *p_twihs, uint32_t ul_timing);
void twihs_set_filter(twihs_registers_t *p_twihs, uint32_t ul_filter);
void twihs_mask_slave_addr(twihs_registers_t *p_twihs, uint32_t ul_mask);


/// @cond 0
/**INDENT-OFF**/
#ifdef __cplusplus
}
#endif
/**INDENT-ON**/
/// @endcond

#endif /* TWIHS_H_INCLUDED */
