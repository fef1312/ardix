/* See the end of this file for copyright, license, and warranty information. */

#include <arch-generic/hardware.h>

#include <arch/hardware.h>
#include <arch/interrupt.h>

#include <toolchain.h>

/** Setup UART to manual byte-by-byte control */
static inline void uart_emergency_setup(void)
{
	UART->UART_PTCR = UART_PTCR_RXTDIS | UART_PTCR_TXTDIS;

	UART->UART_CR = UART_CR_RXDIS | UART_CR_RSTRX
		      | UART_CR_TXDIS | UART_CR_RSTTX;

	UART->UART_IDR = 0xffffffff;

	UART->UART_CR = UART_CR_RXEN | UART_CR_TXEN;
}

static void uart_write_sync(const char *s)
{
	char c;
	while ((c = *s++) != '\0') {
		mom_are_we_there_yet(UART->UART_SR & UART_SR_TXRDY);
		UART->UART_THR = c;
	}
}

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wswitch"
static inline void print_err_msg(enum irqno irqno)
{
	switch (irqno) {
	case IRQNO_HARD_FAULT:
		uart_write_sync("Hard");
		break;
	case IRQNO_MM_FAULT:
		uart_write_sync("Memory Management");
		break;
	case IRQNO_BUS_FAULT:
		uart_write_sync("Bus");
		break;
	case IRQNO_USAGE_FAULT:
		uart_write_sync("Usage");
		break;
	}

	uart_write_sync(" Fault encountered, system halted.\n\n");
}
#pragma GCC diagnostic pop /* -Wswitch */

static void reg_to_str(char *dest, uint32_t val)
{
	for (int i = 28; i >= 0; i -= 4) {
		uint8_t digit = (val >> i) & 0x0f;

		if (digit < 0x0a)
			digit += '0';
		else
			digit += 'a' - 0x0a;

		*dest++ = digit;
	}
}

static void print_regs(struct reg_snapshot *regs)
{
	static char reg_line[] = "r0  = 0x????????\n";
	char *reg_val = &reg_line[8]; /* first question mark */

	reg_to_str(reg_val, regs->hw.r0);
	uart_write_sync(reg_line);

	reg_line[1] = '1';
	reg_to_str(reg_val, regs->hw.r1);
	uart_write_sync(reg_line);

	reg_line[1] = '2';
	reg_to_str(reg_val, regs->hw.r2);
	uart_write_sync(reg_line);

	reg_line[1] = '3';
	reg_to_str(reg_val, regs->hw.r3);
	uart_write_sync(reg_line);

	reg_line[1] = '4';
	reg_to_str(reg_val, regs->sw.r4);
	uart_write_sync(reg_line);

	reg_line[1] = '5';
	reg_to_str(reg_val, regs->sw.r5);
	uart_write_sync(reg_line);

	reg_line[1] = '6';
	reg_to_str(reg_val, regs->sw.r6);
	uart_write_sync(reg_line);

	reg_line[1] = '7';
	reg_to_str(reg_val, regs->sw.r7);
	uart_write_sync(reg_line);

	reg_line[1] = '8';
	reg_to_str(reg_val, regs->sw.r8);
	uart_write_sync(reg_line);

	reg_line[1] = '9';
	reg_to_str(reg_val, regs->sw.r9);
	uart_write_sync(reg_line);

	reg_line[1] = '1';
	reg_line[2] = '0';
	reg_to_str(reg_val, regs->sw.r10);
	uart_write_sync(reg_line);

	reg_line[2] = '1';
	reg_to_str(reg_val, regs->sw.r11);
	uart_write_sync(reg_line);

	reg_line[2] = '2';
	reg_to_str(reg_val, regs->hw.r12);
	uart_write_sync(reg_line);

	reg_line[0] = 's';
	reg_line[1] = 'p';
	reg_line[2] = ' ';
	reg_to_str(reg_val, (uint32_t)(regs + 1)); /* where SP was before reg save */
	uart_write_sync(reg_line);

	reg_line[0] = 'l';
	reg_line[1] = 'r';
	reg_to_str(reg_val, (uint32_t)regs->hw.lr);
	uart_write_sync(reg_line);

	reg_line[0] = 'p';
	reg_line[1] = 'c';
	reg_to_str(reg_val, (uint32_t)regs->hw.pc);
	uart_write_sync(reg_line);
}

#include <arch/debug.h>

__naked __noreturn void arch_handle_fault(struct reg_snapshot *regs, enum irqno irqno)
{
	uart_emergency_setup();
	print_err_msg(irqno);
	print_regs(regs);

	/* give developers a chance to inspect the system */
	__breakpoint;
	/* but never leave this function */
	while (1);
}

/*
 * This file is part of Ardix.
 * Copyright (c) 2020, 2021 Felix Kopp <owo@fef.moe>.
 *
 * Ardix is non-violent software: you may only use, redistribute,
 * and/or modify it under the terms of the CNPLv6+ as found in
 * the LICENSE file in the source code root directory or at
 * <https://git.pixie.town/thufie/CNPL>.
 *
 * Ardix comes with ABSOLUTELY NO WARRANTY, to the extent
 * permitted by applicable law.  See the CNPLv6+ for details.
 */
