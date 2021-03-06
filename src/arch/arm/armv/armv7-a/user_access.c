/*
 * Copyright 2014, General Dynamics C4 Systems
 *
 * This software may be distributed and modified according to the terms of
 * the GNU General Public License version 2. Note that NO WARRANTY is provided.
 * See "LICENSE_GPLv2.txt" for details.
 *
 * @TAG(GD_GPL)
 */

#include <machine/hardware.h>
#include <arch/user_access.h>

#define PMUSERENR_ENABLE BIT(0)

#define CNTKCTL_PL0PCTEN BIT(0)
#define CNTKCTL_PL0VCTEN BIT(1)
#define CNTKCTL_PL0VTEN  BIT(8)
#define CNTKCTL_PL0PTEN  BIT(9)

#define ID_DFR0_PMU_MASK (0xf << 28)
#define ID_DFR0_PMU_NONE (0xf << 28)

#define ID_PFR1_GENERIC_TIMER BIT(16)

static void
check_export_pmu(void)
{
#ifdef CONFIG_EXPORT_PMU_USER
    /* Export performance counters */
    uint32_t v;
    v = PMUSERENR_ENABLE;
    MCR(PMUSERENR, v);
#endif
}

static void
check_export_arch_timer(void)
{
    uint32_t v;
    MRC(CNTKCTL, v);
#ifdef CONFIG_EXPORT_PCNT_USER
    v |= CNTKCTL_PL0PCTEN;
#endif
#ifdef CONFIG_EXPORT_VCNT_USER
    v |= CNTKCTL_PL0VCTEN;
#endif
    MCR(CNTKCTL, v);
}

void
armv_init_user_access(void)
{
    uint32_t v;
    /* Performance Monitoring Unit */
    MRC(ID_DFR0, v);
    if ((v & ID_DFR0_PMU_MASK) != ID_DFR0_PMU_NONE) {
        check_export_pmu();
    }
    /* Arch timers */
    MRC(ID_PFR1, v);
    if (v & ID_PFR1_GENERIC_TIMER) {
        check_export_arch_timer();
    }
}

