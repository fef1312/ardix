/* See the end of this file for copyright, license, and warranty information. */

#include <ardix/io.h>
#include <ardix/kent.h>
#include <ardix/kevent.h>
#include <ardix/sched.h>

#include <config.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <unistd.h>

int child_test(void)
{
	printf("[child ] i'm so sleempy,, calling sleep(),,\n");
	sleep(1000);
	printf("[child ] sleep() returned, i'm gonna kill myself now uwu\n");
	return 69;
}

/**
 * @brief init daemon entry point.
 */
int init_main(void)
{
	printf("[parent] calling exec()\n");
	pid_t pid = exec(child_test);
	printf("[parent] exec() returned, child pid = %d\n", pid);

	int status;
	printf("[parent] calling waitpid()\n");
	waitpid(-1, &status, 0);
	printf("[parent] waitpid() returned, child exit code = %d\n", status);
	printf("[parent] my child has died, goodbye cruel world qwq\n");

	return 0;
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
