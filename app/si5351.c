/*
 * si5351-avr-tiny-minimal.c - Minimal Si5351 library for avr-gcc and
 * ATtiny microprocessors with 8 kB of flash memory and a USI module.
 *
 * Copyright (C) 2015 Jason Milldrum <milldrum@gmail.com>
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include <stdint.h>
#include "si5351.h"

uint32_t xtal_freq = SI5351_XTAL_FREQ;
int32_t ref_correction;

struct Si5351Status dev_status;
struct Si5351IntStatus dev_int_status;

/*
 * si5351_init(uint8_t xtal_load_c, uint32_t ref_osc_freq)
 *
 * Setup communications to the Si5351 and set the crystal
 * load capacitance.
 *
 * xtal_load_c - Crystal load capacitance. Use the SI5351_CRYSTAL_LOAD_*PF
 * defines in the header file
 * ref_osc_freq - Crystal/reference oscillator frequency in 1 Hz increments.
 * Defaults to 25000000 if a 0 is used here.
 *
 */
void si5351_init(uint8_t xtal_load_c, uint32_t ref_osc_freq)
{
	//USI_TWI_Master_Initialise();
	//HAL_I2C_Master_Transmit_IT();
	//HAL_I2C_Master_Transmit_DMA();
	//HAL_I2C_Master_Receive_DMA();
	//HAL_I2C_Init();

	/* Set crystal load capacitance */
//	si5351_write(SI5351_CRYSTAL_LOAD, xtal_load_c);

	/*

	// Change the ref osc freq if different from default
	if (ref_osc_freq != 0)
	{
		xtal_freq = ref_osc_freq;
	}

	// Initialize the CLK outputs according to flowchart in datasheet
	// First, turn them off
	si5351_write(16, 0x80);
	si5351_write(17, 0x80);
	si5351_write(18, 0x80);

	// Turn the clocks back on...
	si5351_write(16, 0x0c);
	si5351_write(17, 0x0c);
	si5351_write(18, 0x0c);

	// Then reset the PLLs
	si5351_pll_reset(SI5351_PLLA);
	si5351_pll_reset(SI5351_PLLB);
*/
}

/*
void si5351_set_freq(uint64_t freq, enum si5351_clock clk)
{
	struct Si5351Frac pll_frac, ms_frac;

	// Lower bounds check
	if(freq < SI5351_MULTISYNTH_MIN_FREQ)
	{
		freq = SI5351_MULTISYNTH_MIN_FREQ;
	}

	// Upper bounds check
	if(freq > SI5351_MULTISYNTH_DIVBY4_FREQ)
	{
		freq = SI5351_MULTISYNTH_DIVBY4_FREQ;
	}


	// Set the PLL
	pll_frac.a = (uint16_t)(SI5351_PLL_FIXED / xtal_freq);
	if(ref_correction < 0)
	{
		pll_frac.b = (uint32_t)((pll_frac.a * (uint32_t)(ref_correction * -1)) / 10);
	}
	else
	{
		pll_frac.b = 1000000UL - (uint32_t)((pll_frac.a * (uint32_t)(ref_correction)) / 10);
		pll_frac.a--;
	}
	pll_frac.c = 1000000UL;
	si5351_set_pll(pll_frac, SI5351_PLLA);

	// Set the MS
	ms_frac.a = (uint16_t)(SI5351_PLL_FIXED / freq);
	ms_frac.b = (uint32_t)(((SI5351_PLL_FIXED % freq) * 1000000UL) / freq);
	ms_frac.c = 1000000UL;
	si5351_set_ms(clk, ms_frac, 0, SI5351_OUTPUT_CLK_DIV_1, 0);
}

void si5351_set_pll(struct Si5351Frac frac, enum si5351_pll target_pll)
{
	struct Si5351RegSet pll_reg;

	// Calculate parameters
  pll_reg.p1 = 128 * frac.a + ((128 * frac.b) / frac.c) - 512;
  pll_reg.p2 = 128 * frac.b - frac.c * ((128 * frac.b) / frac.c);
  pll_reg.p3 = frac.c;

  // Derive the register values to write
  // Prepare an array for parameters to be written to
  uint8_t params[20];
  uint8_t i = 0;
  uint8_t temp;

  // Registers 26-27 for PLLA
  temp = ((pll_reg.p3 >> 8) & 0xFF);
  params[i++] = temp;

  temp = (uint8_t)(pll_reg.p3 & 0xFF);
  params[i++] = temp;

  // Register 28 for PLLA
  temp = (uint8_t)((pll_reg.p1 >> 16) & 0x03);
  params[i++] = temp;

  // Registers 29-30 for PLLA
  temp = (uint8_t)((pll_reg.p1 >> 8) & 0xFF);
  params[i++] = temp;

  temp = (uint8_t)(pll_reg.p1 & 0xFF);
  params[i++] = temp;

  // Register 31 for PLLA
  temp = (uint8_t)((pll_reg.p3 >> 12) & 0xF0);
  temp += (uint8_t)((pll_reg.p2 >> 16) & 0x0F);
  params[i++] = temp;

  // Registers 32-33 for PLLA
  temp = (uint8_t)((pll_reg.p2 >> 8) & 0xFF);
  params[i++] = temp;

  temp = (uint8_t)(pll_reg.p2 & 0xFF);
  params[i++] = temp;

  // Write the parameters
  if(target_pll == SI5351_PLLA)
  {
    si5351_write_bulk(SI5351_PLLA_PARAMETERS, i, params);
  }
  else if(target_pll == SI5351_PLLB)
  {
    si5351_write_bulk(SI5351_PLLB_PARAMETERS, i, params);
  }
}

void si5351_set_ms(enum si5351_clock clk, struct Si5351Frac frac, uint8_t int_mode, uint8_t r_div, uint8_t div_by_4)
{
	struct Si5351RegSet ms_reg;
	uint8_t params[20];
	uint8_t i = 0;
 	uint8_t temp;
 	uint8_t reg_val;

	// Calculate parameters
	if (div_by_4 == 1)
	{
		ms_reg.p3 = 1;
		ms_reg.p2 = 0;
		ms_reg.p1 = 0;
	}
	else
	{
		ms_reg.p1 = 128 * frac.a + ((128 * frac.b) / frac.c) - 512;
		ms_reg.p2 = 128 * frac.b - frac.c * ((128 * frac.b) / frac.c);
		ms_reg.p3 = frac.c;
	}

	// Registers 42-43 for CLK0
	temp = (uint8_t)((ms_reg.p3 >> 8) & 0xFF);
	params[i++] = temp;

	temp = (uint8_t)(ms_reg.p3 & 0xFF);
	params[i++] = temp;

	// Register 44 for CLK0
	si5351_read((SI5351_CLK0_PARAMETERS + 2) + (clk * 8), &reg_val);
	reg_val &= ~(0x03);
	temp = reg_val | ((uint8_t)((ms_reg.p1 >> 16) & 0x03));
	params[i++] = temp;

	// Registers 45-46 for CLK0
	temp = (uint8_t)((ms_reg.p1 >> 8) & 0xFF);
	params[i++] = temp;

	temp = (uint8_t)(ms_reg.p1 & 0xFF);
	params[i++] = temp;

	// Register 47 for CLK0
	temp = (uint8_t)((ms_reg.p3 >> 12) & 0xF0);
	temp += (uint8_t)((ms_reg.p2 >> 16) & 0x0F);
	params[i++] = temp;

	// Registers 48-49 for CLK0
	temp = (uint8_t)((ms_reg.p2 >> 8) & 0xFF);
	params[i++] = temp;

	temp = (uint8_t)(ms_reg.p2 & 0xFF);
	params[i++] = temp;

	// Write the parameters
	switch(clk)
	{
		case SI5351_CLK0:
			si5351_write_bulk(SI5351_CLK0_PARAMETERS, i, params);
			break;
		case SI5351_CLK1:
			si5351_write_bulk(SI5351_CLK1_PARAMETERS, i, params);
			break;
		case SI5351_CLK2:
			si5351_write_bulk(SI5351_CLK2_PARAMETERS, i, params);
			break;
		case SI5351_CLK3:
			si5351_write_bulk(SI5351_CLK3_PARAMETERS, i, params);
			break;
		case SI5351_CLK4:
			si5351_write_bulk(SI5351_CLK4_PARAMETERS, i, params);
			break;
		case SI5351_CLK5:
			si5351_write_bulk(SI5351_CLK5_PARAMETERS, i, params);
			break;
		case SI5351_CLK6:
			si5351_write_bulk(SI5351_CLK6_PARAMETERS, i, params);
			break;
		case SI5351_CLK7:
			si5351_write_bulk(SI5351_CLK7_PARAMETERS, i, params);
			break;
	}

	si5351_set_int(clk, int_mode);
	si5351_set_ms_div(clk, r_div, div_by_4);
}

void si5351_output_enable(enum si5351_clock clk, uint8_t enable)
{
	uint8_t reg_val;

	if(si5351_read(SI5351_OUTPUT_ENABLE_CTRL, &reg_val) != 0)
	{
		return;
	}

	if(enable == 1)
	{
		reg_val &= ~(1<<(uint8_t)clk);
	}
	else
	{
		reg_val |= (1<<(uint8_t)clk);
	}

	si5351_write(SI5351_OUTPUT_ENABLE_CTRL, reg_val);
}

void si5351_drive_strength(enum si5351_clock clk, enum si5351_drive drive)
{
	uint8_t reg_val;
	const uint8_t mask = 0x03;

	if(si5351_read(SI5351_CLK0_CTRL + (uint8_t)clk, &reg_val) != 0)
	{
		return;
	}

	switch(drive)
	{
	case SI5351_DRIVE_2MA:
		reg_val &= ~(mask);
		reg_val |= 0x00;
		break;
	case SI5351_DRIVE_4MA:
		reg_val &= ~(mask);
		reg_val |= 0x01;
		break;
	case SI5351_DRIVE_6MA:
		reg_val &= ~(mask);
		reg_val |= 0x02;
		break;
	case SI5351_DRIVE_8MA:
		reg_val &= ~(mask);
		reg_val |= 0x03;
		break;
	default:
		break;
	}

	si5351_write(SI5351_CLK0_CTRL + (uint8_t)clk, reg_val);
}

void si5351_update_status(void)
{
	si5351_update_sys_status(&dev_status);
	si5351_update_int_status(&dev_int_status);
}

void si5351_set_correction(int32_t corr)
{
	//xtal_freq = (uint32_t)(xtal_freq + (corr * (xtal_freq / 10000000UL)));
	ref_correction = corr;
}

void si5351_set_phase(enum si5351_clock clk, uint8_t phase)
{
	// Mask off the upper bit since it is reserved
	phase = phase & 0b01111111;

	si5351_write(SI5351_CLK0_PHASE_OFFSET + (uint8_t)clk, phase);
}

void si5351_pll_reset(enum si5351_pll target_pll)
{
	if(target_pll == SI5351_PLLA)
 	{
    	si5351_write(SI5351_PLL_RESET, SI5351_PLL_RESET_A);
	}
	else if(target_pll == SI5351_PLLB)
	{
	    si5351_write(SI5351_PLL_RESET, SI5351_PLL_RESET_B);
	}
}

void si5351_set_ms_source(enum si5351_clock clk, enum si5351_pll pll)
{
	uint8_t reg_val;

	si5351_read(SI5351_CLK0_CTRL + (uint8_t)clk, &reg_val);

	if(pll == SI5351_PLLA)
	{
		reg_val &= ~(SI5351_CLK_PLL_SELECT);
	}
	else if(pll == SI5351_PLLB)
	{
		reg_val |= SI5351_CLK_PLL_SELECT;
	}

	si5351_write(SI5351_CLK0_CTRL + (uint8_t)clk, reg_val);
}

void si5351_set_int(enum si5351_clock clk, uint8_t enable)
{
	uint8_t reg_val;
	si5351_read(SI5351_CLK0_CTRL + (uint8_t)clk, &reg_val);

	if(enable == 1)
	{
		reg_val |= (SI5351_CLK_INTEGER_MODE);
	}
	else
	{
		reg_val &= ~(SI5351_CLK_INTEGER_MODE);
	}

	si5351_write(SI5351_CLK0_CTRL + (uint8_t)clk, reg_val);
}

void si5351_set_clock_pwr(enum si5351_clock clk, uint8_t pwr)
{
	uint8_t reg_val;
	si5351_read(SI5351_CLK0_CTRL + (uint8_t)clk, &reg_val);

	if(pwr == 1)
	{
		reg_val &= 0b01111111;
	}
	else
	{
		reg_val |= 0b10000000;
	}

	si5351_write(SI5351_CLK0_CTRL + (uint8_t)clk, reg_val);
}

void si5351_set_clock_invert(enum si5351_clock clk, uint8_t inv)
{
	uint8_t reg_val;
	si5351_read(SI5351_CLK0_CTRL + (uint8_t)clk, &reg_val);

	if(inv == 1)
	{
		reg_val |= (SI5351_CLK_INVERT);
	}
	else
	{
		reg_val &= ~(SI5351_CLK_INVERT);
	}

	si5351_write(SI5351_CLK0_CTRL + (uint8_t)clk, reg_val);
}

void si5351_set_clock_source(enum si5351_clock clk, enum si5351_clock_source src)
{
	uint8_t reg_val;
	si5351_read(SI5351_CLK0_CTRL + (uint8_t)clk, &reg_val);

	// Clear the bits first
	reg_val &= ~(SI5351_CLK_INPUT_MASK);

	switch(src)
	{
	case SI5351_CLK_SRC_XTAL:
		reg_val |= (SI5351_CLK_INPUT_XTAL);
		break;
	case SI5351_CLK_SRC_CLKIN:
		reg_val |= (SI5351_CLK_INPUT_CLKIN);
		break;
	case SI5351_CLK_SRC_MS0:
		if(clk == SI5351_CLK0)
		{
			return;
		}

		reg_val |= (SI5351_CLK_INPUT_MULTISYNTH_0_4);
		break;
	case SI5351_CLK_SRC_MS:
		reg_val |= (SI5351_CLK_INPUT_MULTISYNTH_N);
		break;
	default:
		return;
	}

	si5351_write(SI5351_CLK0_CTRL + (uint8_t)clk, reg_val);
}

void si5351_set_clock_disable(enum si5351_clock clk, enum si5351_clock_disable dis_state)
{
	uint8_t reg_val, reg;

	if (clk >= SI5351_CLK0 && clk <= SI5351_CLK3)
	{
		reg = SI5351_CLK3_0_DISABLE_STATE;
	}
	else if (clk >= SI5351_CLK0 && clk <= SI5351_CLK3)
	{
		reg = SI5351_CLK7_4_DISABLE_STATE;
	}

	si5351_read(reg, &reg_val);

	if (clk >= SI5351_CLK0 && clk <= SI5351_CLK3)
	{
		reg_val &= ~(0b11 << (clk * 2));
		reg_val |= dis_state << (clk * 2);
	}
	else if (clk >= SI5351_CLK0 && clk <= SI5351_CLK3)
	{
		reg_val &= ~(0b11 << ((clk - 4) * 2));
		reg_val |= dis_state << ((clk - 4) * 2);
	}

	si5351_write(reg, reg_val);
}

void si5351_set_clock_fanout(enum si5351_clock_fanout fanout, uint8_t enable)
{
	uint8_t reg_val;
	si5351_read(SI5351_FANOUT_ENABLE, &reg_val);

	switch(fanout)
	{
	case SI5351_FANOUT_CLKIN:
		if(enable)
		{
			reg_val |= SI5351_CLKIN_ENABLE;
		}
		else
		{
			reg_val &= ~(SI5351_CLKIN_ENABLE);
		}
		break;
	case SI5351_FANOUT_XO:
		if(enable)
		{
			reg_val |= SI5351_XTAL_ENABLE;
		}
		else
		{
			reg_val &= ~(SI5351_XTAL_ENABLE);
		}
		break;
	case SI5351_FANOUT_MS:
		if(enable)
		{
			reg_val |= SI5351_MULTISYNTH_ENABLE;
		}
		else
		{
			reg_val &= ~(SI5351_MULTISYNTH_ENABLE);
		}
		break;
	}

	si5351_write(SI5351_FANOUT_ENABLE, reg_val);
}

void si5351_update_sys_status(struct Si5351Status *status)
{
	uint8_t reg_val = 0;

	if(si5351_read(SI5351_DEVICE_STATUS, &reg_val) != 0)
	{
		return;
	}

	status->SYS_INIT = (reg_val >> 7) & 0x01;
	status->LOL_B = (reg_val >> 6) & 0x01;
	status->LOL_A = (reg_val >> 5) & 0x01;
	status->LOS = (reg_val >> 4) & 0x01;
	status->REVID = reg_val & 0x03;
}

void si5351_update_int_status(struct Si5351IntStatus *int_status)
{
	uint8_t reg_val = 0;

	if(si5351_read(SI5351_DEVICE_STATUS, &reg_val) != 0)
	{
		return;
	}

	int_status->SYS_INIT_STKY = (reg_val >> 7) & 0x01;
	int_status->LOL_B_STKY = (reg_val >> 6) & 0x01;
	int_status->LOL_A_STKY = (reg_val >> 5) & 0x01;
	int_status->LOS_STKY = (reg_val >> 4) & 0x01;
}

void si5351_set_ms_div(enum si5351_clock clk, uint8_t r_div, uint8_t div_by_4)
{
	uint8_t reg_val, reg_addr;

	switch(clk)
	{
		case SI5351_CLK0:
			reg_addr = SI5351_CLK0_PARAMETERS + 2;
			break;
		case SI5351_CLK1:
			reg_addr = SI5351_CLK1_PARAMETERS + 2;
			break;
		case SI5351_CLK2:
			reg_addr = SI5351_CLK2_PARAMETERS + 2;
			break;
		case SI5351_CLK3:
			reg_addr = SI5351_CLK3_PARAMETERS + 2;
			break;
		case SI5351_CLK4:
			reg_addr = SI5351_CLK4_PARAMETERS + 2;
			break;
		case SI5351_CLK5:
			reg_addr = SI5351_CLK5_PARAMETERS + 2;
			break;
		case SI5351_CLK6:
			return;
		case SI5351_CLK7:
			return;
	}

	si5351_read(reg_addr, &reg_val);

	// Clear the relevant bits
	reg_val &= ~(0x7c);

	if(div_by_4 == 0)
	{
		reg_val &= ~(SI5351_OUTPUT_CLK_DIVBY4);
	}
	else
	{
		reg_val |= (SI5351_OUTPUT_CLK_DIVBY4);
	}

	reg_val |= (r_div << SI5351_OUTPUT_CLK_DIV_SHIFT);

	si5351_write(reg_addr, reg_val);
}


uint8_t si5351_write_bulk(uint8_t addr, uint8_t bytes, uint8_t *data)
{
	int i;
	uint8_t msg_buffer[bytes + 2];
	uint8_t temp;

	msg_buffer[0] = SI5351_BUS_BASE_ADDR;
	msg_buffer[1] = addr;

	for(i = 0; i < bytes; i++)
	{
		msg_buffer[i + 2] = data[i];
	}

	temp = USI_TWI_Start_Transceiver_With_Data(msg_buffer, bytes + 2);

	return 0;
}

uint8_t si5351_write(uint8_t addr, uint8_t data)
{
	uint8_t msg_buffer[3];
	uint8_t temp;

	msg_buffer[0] = SI5351_BUS_BASE_ADDR;
	msg_buffer[1] = addr;
	msg_buffer[2] = data;

	temp = USI_TWI_Start_Transceiver_With_Data(msg_buffer, 3);

	return 0;
}

uint8_t si5351_read(uint8_t addr, uint8_t *data)
{
	uint8_t msg_buffer[2];
	uint8_t temp;

	msg_buffer[0] = SI5351_BUS_BASE_ADDR;
	msg_buffer[1] = addr;

	temp = USI_TWI_Start_Transceiver_With_Data(msg_buffer, 2);

	msg_buffer[0] = (SI5351_BUS_BASE_ADDR) | (TRUE << TWI_READ_BIT);

	temp = USI_TWI_Start_Transceiver_With_Data(msg_buffer, 2);

	*data = msg_buffer[1];

	return 0;
}

*/
