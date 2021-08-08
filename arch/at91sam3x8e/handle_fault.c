/* See the end of this file for copyright, license, and warranty information. */

#include <arch-generic/hardware.h>

#include <arch/hardware.h>
#include <arch/interrupt.h>

#include <toolchain.h>

static void uart_write_sync(const char *s)
{
	char c;
	while ((c = *s++) != '\0') {
		mom_are_we_there_yet(UART->UART_SR & UART_SR_TXRDY);
		UART->UART_THR = c;
	}
}

/** Setup UART to manual byte-by-byte control */
static inline void uart_emergency_setup(void)
{
	UART->UART_IDR = 0xffffffff;

	mom_are_we_there_yet(UART->UART_SR & UART_SR_TXRDY);
	UART->UART_PTCR = UART_PTCR_RXTDIS | UART_PTCR_TXTDIS;

	UART->UART_CR = UART_CR_RXDIS | UART_CR_RSTRX
		      | UART_CR_TXDIS | UART_CR_RSTTX;

	UART->UART_CR = UART_CR_TXEN;

	UART->UART_THR = '\0';
}

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wswitch"
static inline void print_err_msg(enum irqno irqno)
{
	uart_write_sync("\n\n########## SERIOUS BRUH MOMENT! ##########\n");

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

	uart_write_sync(" Fault encountered, cannot continue\nRegister dump:\n");
}
#pragma GCC diagnostic pop /* -Wswitch */

static void reg_to_str(char *dest, word_t val)
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

static void print_reg(const char *name, word_t val)
{
	/* static saves stack space, which might be limited */
	static char line[] = "???? = 0x????????\n";
	char c;
	char *name_pos = line;
	while ((c = *name++) != '\0')
		*name_pos++ = c;
	while (name_pos < &line[4])
		*name_pos++ = ' ';
	reg_to_str(&line[9], val);
	uart_write_sync(line);
}

static void print_regs(struct exc_context *context)
{
	print_reg("R0", context->sp->r0);
	print_reg("R1", context->sp->r1);
	print_reg("R2", context->sp->r2);
	print_reg("R3", context->sp->r3);
	print_reg("R4", context->r4);
	print_reg("R5", context->r5);
	print_reg("R6", context->r6);
	print_reg("R7", context->r7);
	print_reg("R8", context->r8);
	print_reg("R9", context->r9);
	print_reg("R10", context->r10);
	print_reg("R11", context->r11);
	print_reg("R12", context->sp->r12);
	print_reg("SP", *(word_t *)&context->sp);
	print_reg("LR", *(word_t *)&context->sp->lr);
	print_reg("PC", *(word_t *)&context->sp->pc);
	print_reg("xPSR", context->sp->psr);
}

#include <arch/debug.h>

__naked __noreturn void handle_fault(struct exc_context *context, enum irqno irqno)
{
	uart_emergency_setup();

	print_err_msg(irqno);
	print_regs(context);

	if (SCB->HFSR & SCB_HFSR_FORCED_Msk)
		print_reg("CFSR", SCB->CFSR);

	uart_write_sync("\nSystem halted, goodbye\n\n");

	__breakpoint;
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
