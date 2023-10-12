/*
 * SPDX-License-Identifier: BSD-3-Clause
 * Copyright (C) IBM Corporation 2014.
 */

#ifndef _RTE_CPUFLAGS_PPC_64_H_
#define _RTE_CPUFLAGS_PPC_64_H_

#ifdef __cplusplus
extern "C" {
#endif

/**
 * Enumeration of all CPU features supported
 */
enum rte_cpu_flag_t {
	RTE_CPUFLAG_PPC_LE = 0,
	RTE_CPUFLAG_TRUE_LE,
	RTE_CPUFLAG_PSERIES_PERFMON_COMPAT,
	RTE_CPUFLAG_VSX,
	RTE_CPUFLAG_ARCH_2_06,
	RTE_CPUFLAG_POWER6_EXT,
	RTE_CPUFLAG_DFP,
	RTE_CPUFLAG_PA6T,
	RTE_CPUFLAG_ARCH_2_05,
	RTE_CPUFLAG_ICACHE_SNOOP,
	RTE_CPUFLAG_SMT,
	RTE_CPUFLAG_BOOKE,
	RTE_CPUFLAG_CELLBE,
	RTE_CPUFLAG_POWER5_PLUS,
	RTE_CPUFLAG_POWER5,
	RTE_CPUFLAG_POWER4,
	RTE_CPUFLAG_NOTB,
	RTE_CPUFLAG_EFP_DOUBLE,
	RTE_CPUFLAG_EFP_SINGLE,
	RTE_CPUFLAG_SPE,
	RTE_CPUFLAG_UNIFIED_CACHE,
	RTE_CPUFLAG_4xxMAC,
	RTE_CPUFLAG_MMU,
	RTE_CPUFLAG_FPU,
	RTE_CPUFLAG_ALTIVEC,
	RTE_CPUFLAG_PPC601,
	RTE_CPUFLAG_PPC64,
	RTE_CPUFLAG_PPC32,
	RTE_CPUFLAG_TAR,
	RTE_CPUFLAG_LSEL,
	RTE_CPUFLAG_EBB,
	RTE_CPUFLAG_DSCR,
	RTE_CPUFLAG_HTM,
	RTE_CPUFLAG_ARCH_2_07,
};

#include "generic/rte_cpuflags.h"

#ifdef __cplusplus
}
#endif

#endif /* _RTE_CPUFLAGS_PPC_64_H_ */
