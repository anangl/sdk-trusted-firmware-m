/*
 * Copyright (c) 2019-2022 Arm Limited
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include "Driver_PPC.h"

#include "Driver_Common.h"
#include "cmsis.h"
#include "cmsis_driver_config.h"
#include "RTE_Device.h"
#include "ppc_corstone310_drv.h"

/* Driver version */
#define ARM_PPC_DRV_VERSION ARM_DRIVER_VERSION_MAJOR_MINOR(1,0)

#if (defined (RTE_PPC_CORSTONE310_MAIN0) && (RTE_PPC_CORSTONE310_MAIN0 == 1)) || \
    (defined (RTE_PPC_CORSTONE310_MAIN_EXP0) && (RTE_PPC_CORSTONE310_MAIN_EXP0 == 1)) || \
    (defined (RTE_PPC_CORSTONE310_MAIN_EXP1) && (RTE_PPC_CORSTONE310_MAIN_EXP1 == 1)) || \
    (defined (RTE_PPC_CORSTONE310_MAIN_EXP2) && (RTE_PPC_CORSTONE310_MAIN_EXP2 == 1)) || \
    (defined (RTE_PPC_CORSTONE310_MAIN_EXP3) && (RTE_PPC_CORSTONE310_MAIN_EXP3 == 1)) || \
    (defined (RTE_PPC_CORSTONE310_PERIPH0) && (RTE_PPC_CORSTONE310_PERIPH0 == 1)) || \
    (defined (RTE_PPC_CORSTONE310_PERIPH1) && (RTE_PPC_CORSTONE310_PERIPH1 == 1)) || \
    (defined (RTE_PPC_CORSTONE310_PERIPH_EXP0) && (RTE_PPC_CORSTONE310_PERIPH_EXP0 == 1)) || \
    (defined (RTE_PPC_CORSTONE310_PERIPH_EXP1) && (RTE_PPC_CORSTONE310_PERIPH_EXP1 == 1)) || \
    (defined (RTE_PPC_CORSTONE310_PERIPH_EXP2) && (RTE_PPC_CORSTONE310_PERIPH_EXP2 == 1)) || \
    (defined (RTE_PPC_CORSTONE310_PERIPH_EXP3) && (RTE_PPC_CORSTONE310_PERIPH_EXP3 == 1))

/* Driver Version */
static const ARM_DRIVER_VERSION DriverVersion = {
    ARM_PPC_API_VERSION,
    ARM_PPC_DRV_VERSION
};

static ARM_DRIVER_VERSION PPC_CORSTONE310_GetVersion(void)
{
    return DriverVersion;
}

typedef struct _CORSTONE310_PPC_Resources {
    struct ppc_corstone310_dev_t* dev;       /* PPC device structure */
} CORSTONE310_PPC_Resources;

#if (defined (RTE_PPC_CORSTONE310_MAIN0) && (RTE_PPC_CORSTONE310_MAIN0 == 1))

static CORSTONE310_PPC_Resources MAIN0_PPC_DEV = {
    .dev = &PPC_CORSTONE310_MAIN0_DEV,
};

/* MAIN0 PPC Driver wrapper functions */
static int32_t PPC_CORSTONE310_MAIN0_Initialize(void)
{
    ppc_corstone310_init(MAIN0_PPC_DEV.dev);

    return ARM_DRIVER_OK;
}

static int32_t PPC_CORSTONE310_MAIN0_Uninitialize(void)
{
    /* Nothing to do */
    return ARM_DRIVER_OK;
}

static int32_t
PPC_CORSTONE310_MAIN0_ConfigPrivilege(uint32_t periph,
                                 PPC_CORSTONE310_SecAttr sec_attr,
                                 PPC_CORSTONE310_PrivAttr priv_attr)
{
    enum ppc_corstone310_error_t ret;

    ret = ppc_corstone310_config_privilege(MAIN0_PPC_DEV.dev, periph,
                                      (enum ppc_corstone310_sec_attr_t)sec_attr,
                                      (enum ppc_corstone310_priv_attr_t)priv_attr);

    if( ret != PPC_CORSTONE310_ERR_NONE) {
        return ARM_DRIVER_ERROR;
    }

    return ARM_DRIVER_OK;
}

static bool PPC_CORSTONE310_MAIN0_IsPeriphPrivOnly(uint32_t periph)
{
    return ppc_corstone310_is_periph_priv_only(MAIN0_PPC_DEV.dev, periph);
}

/* Secure only functions */
#if (defined (__ARM_FEATURE_CMSE) && (__ARM_FEATURE_CMSE == 3U))

static int32_t
PPC_CORSTONE310_MAIN0_ConfigSecurity(uint32_t periph, PPC_CORSTONE310_SecAttr sec_attr)
{
    enum ppc_corstone310_error_t ret;

    ret = ppc_corstone310_config_security(MAIN0_PPC_DEV.dev, periph,
                                     (enum ppc_corstone310_sec_attr_t)sec_attr);

    if( ret != PPC_CORSTONE310_ERR_NONE) {
        return ARM_DRIVER_ERROR;
    }

    return ARM_DRIVER_OK;
}

static bool PPC_CORSTONE310_MAIN0_IsPeriphSecure(uint32_t periph)
{
    return ppc_corstone310_is_periph_secure(MAIN0_PPC_DEV.dev, periph);
}

static int32_t PPC_CORSTONE310_MAIN0_EnableInterrupt(void)
{
    enum ppc_corstone310_error_t ret;

    ret = ppc_corstone310_irq_enable(MAIN0_PPC_DEV.dev);

    if( ret != PPC_CORSTONE310_ERR_NONE) {
        return ARM_DRIVER_ERROR;
    }

    return ARM_DRIVER_OK;
}

static void PPC_CORSTONE310_MAIN0_DisableInterrupt(void)
{
    ppc_corstone310_irq_disable(MAIN0_PPC_DEV.dev);
}

static void PPC_CORSTONE310_MAIN0_ClearInterrupt(void)
{
    ppc_corstone310_clear_irq(MAIN0_PPC_DEV.dev);
}

static bool PPC_CORSTONE310_MAIN0_InterruptState(void)
{
    return ppc_corstone310_irq_state(MAIN0_PPC_DEV.dev);
}

#endif /* (defined (__ARM_FEATURE_CMSE) && (__ARM_FEATURE_CMSE == 3U)) */

/* PPC Corstone310 MAIN0 Driver CMSIS access structure */
DRIVER_PPC_CORSTONE310 Driver_PPC_CORSTONE310_MAIN0 = {
    .GetVersion        = PPC_CORSTONE310_GetVersion,
    .Initialize        = PPC_CORSTONE310_MAIN0_Initialize,
    .Uninitialize      = PPC_CORSTONE310_MAIN0_Uninitialize,
    .ConfigPrivilege   = PPC_CORSTONE310_MAIN0_ConfigPrivilege,
    .IsPeriphPrivOnly  = PPC_CORSTONE310_MAIN0_IsPeriphPrivOnly,
#if (defined (__ARM_FEATURE_CMSE) && (__ARM_FEATURE_CMSE == 3U))
    .ConfigSecurity    = PPC_CORSTONE310_MAIN0_ConfigSecurity,
    .IsPeriphSecure    = PPC_CORSTONE310_MAIN0_IsPeriphSecure,
    .EnableInterrupt   = PPC_CORSTONE310_MAIN0_EnableInterrupt,
    .DisableInterrupt  = PPC_CORSTONE310_MAIN0_DisableInterrupt,
    .ClearInterrupt    = PPC_CORSTONE310_MAIN0_ClearInterrupt,
    .InterruptState    = PPC_CORSTONE310_MAIN0_InterruptState
#endif /* (defined (__ARM_FEATURE_CMSE) && (__ARM_FEATURE_CMSE == 3U)) */
};
#endif /* RTE_PPC_CORSTONE310_MAIN0 */

#if (defined (RTE_PPC_CORSTONE310_MAIN_EXP0) && (RTE_PPC_CORSTONE310_MAIN_EXP0 == 1))

static CORSTONE310_PPC_Resources MAIN_EXP0_PPC_DEV = {
    .dev = &PPC_CORSTONE310_MAIN_EXP0_DEV,
};

/* MAIN PPCEXP0 Driver wrapper functions */
static int32_t PPC_CORSTONE310_MAIN_EXP0_Initialize(void)
{
    ppc_corstone310_init(MAIN_EXP0_PPC_DEV.dev);

    return ARM_DRIVER_OK;
}

static int32_t PPC_CORSTONE310_MAIN_EXP0_Uninitialize(void)
{
    /* Nothing to do */
    return ARM_DRIVER_OK;
}

static int32_t
PPC_CORSTONE310_MAIN_EXP0_ConfigPrivilege(uint32_t periph,
                                     PPC_CORSTONE310_SecAttr sec_attr,
                                     PPC_CORSTONE310_PrivAttr priv_attr)
{
    enum ppc_corstone310_error_t ret;

    ret = ppc_corstone310_config_privilege(MAIN_EXP0_PPC_DEV.dev, periph,
                                      (enum ppc_corstone310_sec_attr_t)sec_attr,
                                      (enum ppc_corstone310_priv_attr_t)priv_attr);

    if( ret != PPC_CORSTONE310_ERR_NONE) {
        return ARM_DRIVER_ERROR;
    }

    return ARM_DRIVER_OK;
}

static bool PPC_CORSTONE310_MAIN_EXP0_IsPeriphPrivOnly(uint32_t periph)
{
    return ppc_corstone310_is_periph_priv_only(MAIN_EXP0_PPC_DEV.dev, periph);
}

/* Secure only functions */
#if (defined (__ARM_FEATURE_CMSE) && (__ARM_FEATURE_CMSE == 3U))

static int32_t
PPC_CORSTONE310_MAIN_EXP0_ConfigSecurity(uint32_t periph,
                                    PPC_CORSTONE310_SecAttr sec_attr)
{
    enum ppc_corstone310_error_t ret;

    ret = ppc_corstone310_config_security(MAIN_EXP0_PPC_DEV.dev, periph,
                                     (enum ppc_corstone310_sec_attr_t)sec_attr);

    if( ret != PPC_CORSTONE310_ERR_NONE) {
        return ARM_DRIVER_ERROR;
    }

    return ARM_DRIVER_OK;
}

static bool PPC_CORSTONE310_MAIN_EXP0_IsPeriphSecure(uint32_t periph)
{
    return ppc_corstone310_is_periph_secure(MAIN_EXP0_PPC_DEV.dev, periph);
}

static int32_t PPC_CORSTONE310_MAIN_EXP0_EnableInterrupt(void)
{
    enum ppc_corstone310_error_t ret;

    ret = ppc_corstone310_irq_enable(MAIN_EXP0_PPC_DEV.dev);

    if( ret != PPC_CORSTONE310_ERR_NONE) {
        return ARM_DRIVER_ERROR;
    }

    return ARM_DRIVER_OK;
}

static void PPC_CORSTONE310_MAIN_EXP0_DisableInterrupt(void)
{
    ppc_corstone310_irq_disable(MAIN_EXP0_PPC_DEV.dev);
}

static void PPC_CORSTONE310_MAIN_EXP0_ClearInterrupt(void)
{
    ppc_corstone310_clear_irq(MAIN_EXP0_PPC_DEV.dev);
}

static bool PPC_CORSTONE310_MAIN_EXP0_InterruptState(void)
{
    return ppc_corstone310_irq_state(MAIN_EXP0_PPC_DEV.dev);
}

#endif /* (defined (__ARM_FEATURE_CMSE) && (__ARM_FEATURE_CMSE == 3U)) */

/* PPC Corstone-310 MAIN EXP0 Driver CMSIS access structure */
DRIVER_PPC_CORSTONE310 Driver_PPC_CORSTONE310_MAIN_EXP0 = {
    .GetVersion        = PPC_CORSTONE310_GetVersion,
    .Initialize        = PPC_CORSTONE310_MAIN_EXP0_Initialize,
    .Uninitialize      = PPC_CORSTONE310_MAIN_EXP0_Uninitialize,
    .ConfigPrivilege   = PPC_CORSTONE310_MAIN_EXP0_ConfigPrivilege,
    .IsPeriphPrivOnly  = PPC_CORSTONE310_MAIN_EXP0_IsPeriphPrivOnly,
#if (defined (__ARM_FEATURE_CMSE) && (__ARM_FEATURE_CMSE == 3U))
    .ConfigSecurity    = PPC_CORSTONE310_MAIN_EXP0_ConfigSecurity,
    .IsPeriphSecure    = PPC_CORSTONE310_MAIN_EXP0_IsPeriphSecure,
    .EnableInterrupt   = PPC_CORSTONE310_MAIN_EXP0_EnableInterrupt,
    .DisableInterrupt  = PPC_CORSTONE310_MAIN_EXP0_DisableInterrupt,
    .ClearInterrupt    = PPC_CORSTONE310_MAIN_EXP0_ClearInterrupt,
    .InterruptState    = PPC_CORSTONE310_MAIN_EXP0_InterruptState
#endif /* (defined (__ARM_FEATURE_CMSE) && (__ARM_FEATURE_CMSE == 3U)) */
};
#endif /* RTE_PPC_CORSTONE310_MAIN_EXP0 */

#if (defined (RTE_PPC_CORSTONE310_MAIN_EXP1) && (RTE_PPC_CORSTONE310_MAIN_EXP1 == 1))

static CORSTONE310_PPC_Resources MAIN_EXP1_PPC_DEV = {
    .dev = &PPC_CORSTONE310_MAIN_EXP1_DEV,
};

/* MAIN PPCEXP1 Driver wrapper functions */
static int32_t PPC_CORSTONE310_MAIN_EXP1_Initialize(void)
{
    ppc_corstone310_init(MAIN_EXP1_PPC_DEV.dev);

    return ARM_DRIVER_OK;
}

static int32_t PPC_CORSTONE310_MAIN_EXP1_Uninitialize(void)
{
    /* Nothing to do */
    return ARM_DRIVER_OK;
}

static int32_t
PPC_CORSTONE310_MAIN_EXP1_ConfigPrivilege(uint32_t periph,
                                     PPC_CORSTONE310_SecAttr sec_attr,
                                     PPC_CORSTONE310_PrivAttr priv_attr)
{
    enum ppc_corstone310_error_t ret;

    ret = ppc_corstone310_config_privilege(MAIN_EXP1_PPC_DEV.dev, periph,
                                      (enum ppc_corstone310_sec_attr_t)sec_attr,
                                      (enum ppc_corstone310_priv_attr_t)priv_attr);

    if( ret != PPC_CORSTONE310_ERR_NONE) {
        return ARM_DRIVER_ERROR;
    }

    return ARM_DRIVER_OK;
}

static bool PPC_CORSTONE310_MAIN_EXP1_IsPeriphPrivOnly(uint32_t periph)
{
    return ppc_corstone310_is_periph_priv_only(MAIN_EXP1_PPC_DEV.dev, periph);
}

/* Secure only functions */
#if (defined (__ARM_FEATURE_CMSE) && (__ARM_FEATURE_CMSE == 3U))

static int32_t
PPC_CORSTONE310_MAIN_EXP1_ConfigSecurity(uint32_t periph,
                                    PPC_CORSTONE310_SecAttr sec_attr)
{
    enum ppc_corstone310_error_t ret;

    ret = ppc_corstone310_config_security(MAIN_EXP1_PPC_DEV.dev, periph,
                                     (enum ppc_corstone310_sec_attr_t)sec_attr);

    if( ret != PPC_CORSTONE310_ERR_NONE) {
        return ARM_DRIVER_ERROR;
    }

    return ARM_DRIVER_OK;
}

static bool PPC_CORSTONE310_MAIN_EXP1_IsPeriphSecure(uint32_t periph)
{
    return ppc_corstone310_is_periph_secure(MAIN_EXP1_PPC_DEV.dev, periph);
}

static int32_t PPC_CORSTONE310_MAIN_EXP1_EnableInterrupt(void)
{
    enum ppc_corstone310_error_t ret;

    ret = ppc_corstone310_irq_enable(MAIN_EXP1_PPC_DEV.dev);

    if( ret != PPC_CORSTONE310_ERR_NONE) {
        return ARM_DRIVER_ERROR;
    }

    return ARM_DRIVER_OK;
}

static void PPC_CORSTONE310_MAIN_EXP1_DisableInterrupt(void)
{
    ppc_corstone310_irq_disable(MAIN_EXP1_PPC_DEV.dev);
}

static void PPC_CORSTONE310_MAIN_EXP1_ClearInterrupt(void)
{
    ppc_corstone310_clear_irq(MAIN_EXP1_PPC_DEV.dev);
}

static bool PPC_CORSTONE310_MAIN_EXP1_InterruptState(void)
{
    return ppc_corstone310_irq_state(MAIN_EXP1_PPC_DEV.dev);
}

#endif /* (defined (__ARM_FEATURE_CMSE) && (__ARM_FEATURE_CMSE == 3U)) */

/* PPC Corstone-310 MAIN EXP1 Driver CMSIS access structure */
DRIVER_PPC_CORSTONE310 Driver_PPC_CORSTONE310_MAIN_EXP1 = {
    .GetVersion        = PPC_CORSTONE310_GetVersion,
    .Initialize        = PPC_CORSTONE310_MAIN_EXP1_Initialize,
    .Uninitialize      = PPC_CORSTONE310_MAIN_EXP1_Uninitialize,
    .ConfigPrivilege   = PPC_CORSTONE310_MAIN_EXP1_ConfigPrivilege,
    .IsPeriphPrivOnly  = PPC_CORSTONE310_MAIN_EXP1_IsPeriphPrivOnly,
#if (defined (__ARM_FEATURE_CMSE) && (__ARM_FEATURE_CMSE == 3U))
    .ConfigSecurity    = PPC_CORSTONE310_MAIN_EXP1_ConfigSecurity,
    .IsPeriphSecure    = PPC_CORSTONE310_MAIN_EXP1_IsPeriphSecure,
    .EnableInterrupt   = PPC_CORSTONE310_MAIN_EXP1_EnableInterrupt,
    .DisableInterrupt  = PPC_CORSTONE310_MAIN_EXP1_DisableInterrupt,
    .ClearInterrupt    = PPC_CORSTONE310_MAIN_EXP1_ClearInterrupt,
    .InterruptState    = PPC_CORSTONE310_MAIN_EXP1_InterruptState
#endif /* (defined (__ARM_FEATURE_CMSE) && (__ARM_FEATURE_CMSE == 3U)) */
};
#endif /* RTE_PPC_CORSTONE310_MAIN_EXP1 */

#if (defined (RTE_PPC_CORSTONE310_MAIN_EXP2) && (RTE_PPC_CORSTONE310_MAIN_EXP2 == 1))

static CORSTONE310_PPC_Resources MAIN_EXP2_PPC_DEV = {
    .dev = &PPC_CORSTONE310_MAIN_EXP2_DEV,
};

/* MAIN PPCEXP2 Driver wrapper functions */
static int32_t PPC_CORSTONE310_MAIN_EXP2_Initialize(void)
{
    ppc_corstone310_init(MAIN_EXP2_PPC_DEV.dev);

    return ARM_DRIVER_OK;
}

static int32_t PPC_CORSTONE310_MAIN_EXP2_Uninitialize(void)
{
    /* Nothing to do */
    return ARM_DRIVER_OK;
}

static int32_t
PPC_CORSTONE310_MAIN_EXP2_ConfigPrivilege(uint32_t periph,
                                     PPC_CORSTONE310_SecAttr sec_attr,
                                     PPC_CORSTONE310_PrivAttr priv_attr)
{
    enum ppc_corstone310_error_t ret;

    ret = ppc_corstone310_config_privilege(MAIN_EXP2_PPC_DEV.dev, periph,
                                      (enum ppc_corstone310_sec_attr_t)sec_attr,
                                      (enum ppc_corstone310_priv_attr_t)priv_attr);

    if( ret != PPC_CORSTONE310_ERR_NONE) {
        return ARM_DRIVER_ERROR;
    }

    return ARM_DRIVER_OK;
}

static bool PPC_CORSTONE310_MAIN_EXP2_IsPeriphPrivOnly(uint32_t periph)
{
    return ppc_corstone310_is_periph_priv_only(MAIN_EXP2_PPC_DEV.dev, periph);
}

/* Secure only functions */
#if (defined (__ARM_FEATURE_CMSE) && (__ARM_FEATURE_CMSE == 3U))

static int32_t
PPC_CORSTONE310_MAIN_EXP2_ConfigSecurity(uint32_t periph,
                                    PPC_CORSTONE310_SecAttr sec_attr)
{
    enum ppc_corstone310_error_t ret;

    ret = ppc_corstone310_config_security(MAIN_EXP2_PPC_DEV.dev, periph,
                                     (enum ppc_corstone310_sec_attr_t)sec_attr);

    if( ret != PPC_CORSTONE310_ERR_NONE) {
        return ARM_DRIVER_ERROR;
    }

    return ARM_DRIVER_OK;
}

static bool PPC_CORSTONE310_MAIN_EXP2_IsPeriphSecure(uint32_t periph)
{
    return ppc_corstone310_is_periph_secure(MAIN_EXP2_PPC_DEV.dev, periph);
}

static int32_t PPC_CORSTONE310_MAIN_EXP2_EnableInterrupt(void)
{
    enum ppc_corstone310_error_t ret;

    ret = ppc_corstone310_irq_enable(MAIN_EXP2_PPC_DEV.dev);

    if( ret != PPC_CORSTONE310_ERR_NONE) {
        return ARM_DRIVER_ERROR;
    }

    return ARM_DRIVER_OK;
}

static void PPC_CORSTONE310_MAIN_EXP2_DisableInterrupt(void)
{
    ppc_corstone310_irq_disable(MAIN_EXP2_PPC_DEV.dev);
}

static void PPC_CORSTONE310_MAIN_EXP2_ClearInterrupt(void)
{
    ppc_corstone310_clear_irq(MAIN_EXP2_PPC_DEV.dev);
}

static bool PPC_CORSTONE310_MAIN_EXP2_InterruptState(void)
{
    return ppc_corstone310_irq_state(MAIN_EXP2_PPC_DEV.dev);
}

#endif /* (defined (__ARM_FEATURE_CMSE) && (__ARM_FEATURE_CMSE == 3U)) */

/* PPC Corstone-310 MAIN EXP2 Driver CMSIS access structure */
DRIVER_PPC_CORSTONE310 Driver_PPC_CORSTONE310_MAIN_EXP2 = {
    .GetVersion        = PPC_CORSTONE310_GetVersion,
    .Initialize        = PPC_CORSTONE310_MAIN_EXP2_Initialize,
    .Uninitialize      = PPC_CORSTONE310_MAIN_EXP2_Uninitialize,
    .ConfigPrivilege   = PPC_CORSTONE310_MAIN_EXP2_ConfigPrivilege,
    .IsPeriphPrivOnly  = PPC_CORSTONE310_MAIN_EXP2_IsPeriphPrivOnly,
#if (defined (__ARM_FEATURE_CMSE) && (__ARM_FEATURE_CMSE == 3U))
    .ConfigSecurity    = PPC_CORSTONE310_MAIN_EXP2_ConfigSecurity,
    .IsPeriphSecure    = PPC_CORSTONE310_MAIN_EXP2_IsPeriphSecure,
    .EnableInterrupt   = PPC_CORSTONE310_MAIN_EXP2_EnableInterrupt,
    .DisableInterrupt  = PPC_CORSTONE310_MAIN_EXP2_DisableInterrupt,
    .ClearInterrupt    = PPC_CORSTONE310_MAIN_EXP2_ClearInterrupt,
    .InterruptState    = PPC_CORSTONE310_MAIN_EXP2_InterruptState
#endif /* (defined (__ARM_FEATURE_CMSE) && (__ARM_FEATURE_CMSE == 3U)) */
};
#endif /* RTE_PPC_CORSTONE310_MAIN_EXP2 */

#if (defined (RTE_PPC_CORSTONE310_MAIN_EXP3) && (RTE_PPC_CORSTONE310_MAIN_EXP3 == 1))

static CORSTONE310_PPC_Resources MAIN_EXP3_PPC_DEV = {
    .dev = &PPC_CORSTONE310_MAIN_EXP3_DEV,
};

/* MAIN PPCEXP3 Driver wrapper functions */
static int32_t PPC_CORSTONE310_MAIN_EXP3_Initialize(void)
{
    ppc_corstone310_init(MAIN_EXP3_PPC_DEV.dev);

    return ARM_DRIVER_OK;
}

static int32_t PPC_CORSTONE310_MAIN_EXP3_Uninitialize(void)
{
    /* Nothing to do */
    return ARM_DRIVER_OK;
}

static int32_t
PPC_CORSTONE310_MAIN_EXP3_ConfigPrivilege(uint32_t periph,
                                     PPC_CORSTONE310_SecAttr sec_attr,
                                     PPC_CORSTONE310_PrivAttr priv_attr)
{
    enum ppc_corstone310_error_t ret;

    ret = ppc_corstone310_config_privilege(MAIN_EXP3_PPC_DEV.dev, periph,
                                      (enum ppc_corstone310_sec_attr_t)sec_attr,
                                      (enum ppc_corstone310_priv_attr_t)priv_attr);

    if( ret != PPC_CORSTONE310_ERR_NONE) {
        return ARM_DRIVER_ERROR;
    }

    return ARM_DRIVER_OK;
}

static bool PPC_CORSTONE310_MAIN_EXP3_IsPeriphPrivOnly(uint32_t periph)
{
    return ppc_corstone310_is_periph_priv_only(MAIN_EXP3_PPC_DEV.dev, periph);
}

/* Secure only functions */
#if (defined (__ARM_FEATURE_CMSE) && (__ARM_FEATURE_CMSE == 3U))

static int32_t
PPC_CORSTONE310_MAIN_EXP3_ConfigSecurity(uint32_t periph,
                                    PPC_CORSTONE310_SecAttr sec_attr)
{
    enum ppc_corstone310_error_t ret;

    ret = ppc_corstone310_config_security(MAIN_EXP3_PPC_DEV.dev, periph,
                                     (enum ppc_corstone310_sec_attr_t)sec_attr);

    if( ret != PPC_CORSTONE310_ERR_NONE) {
        return ARM_DRIVER_ERROR;
    }

    return ARM_DRIVER_OK;
}

static bool PPC_CORSTONE310_MAIN_EXP3_IsPeriphSecure(uint32_t periph)
{
    return ppc_corstone310_is_periph_secure(MAIN_EXP3_PPC_DEV.dev, periph);
}

static int32_t PPC_CORSTONE310_MAIN_EXP3_EnableInterrupt(void)
{
    enum ppc_corstone310_error_t ret;

    ret = ppc_corstone310_irq_enable(MAIN_EXP3_PPC_DEV.dev);

    if( ret != PPC_CORSTONE310_ERR_NONE) {
        return ARM_DRIVER_ERROR;
    }

    return ARM_DRIVER_OK;
}

static void PPC_CORSTONE310_MAIN_EXP3_DisableInterrupt(void)
{
    ppc_corstone310_irq_disable(MAIN_EXP3_PPC_DEV.dev);
}

static void PPC_CORSTONE310_MAIN_EXP3_ClearInterrupt(void)
{
    ppc_corstone310_clear_irq(MAIN_EXP3_PPC_DEV.dev);
}

static bool PPC_CORSTONE310_MAIN_EXP3_InterruptState(void)
{
    return ppc_corstone310_irq_state(MAIN_EXP3_PPC_DEV.dev);
}

#endif /* (defined (__ARM_FEATURE_CMSE) && (__ARM_FEATURE_CMSE == 3U)) */

/* PPC Corstone-310 MAIN EXP3 Driver CMSIS access structure */
DRIVER_PPC_CORSTONE310 Driver_PPC_CORSTONE310_MAIN_EXP3 = {
    .GetVersion        = PPC_CORSTONE310_GetVersion,
    .Initialize        = PPC_CORSTONE310_MAIN_EXP3_Initialize,
    .Uninitialize      = PPC_CORSTONE310_MAIN_EXP3_Uninitialize,
    .ConfigPrivilege   = PPC_CORSTONE310_MAIN_EXP3_ConfigPrivilege,
    .IsPeriphPrivOnly  = PPC_CORSTONE310_MAIN_EXP3_IsPeriphPrivOnly,
#if (defined (__ARM_FEATURE_CMSE) && (__ARM_FEATURE_CMSE == 3U))
    .ConfigSecurity    = PPC_CORSTONE310_MAIN_EXP3_ConfigSecurity,
    .IsPeriphSecure    = PPC_CORSTONE310_MAIN_EXP3_IsPeriphSecure,
    .EnableInterrupt   = PPC_CORSTONE310_MAIN_EXP3_EnableInterrupt,
    .DisableInterrupt  = PPC_CORSTONE310_MAIN_EXP3_DisableInterrupt,
    .ClearInterrupt    = PPC_CORSTONE310_MAIN_EXP3_ClearInterrupt,
    .InterruptState    = PPC_CORSTONE310_MAIN_EXP3_InterruptState
#endif /* (defined (__ARM_FEATURE_CMSE) && (__ARM_FEATURE_CMSE == 3U)) */
};
#endif /* RTE_PPC_CORSTONE310_MAIN_EXP3 */

#if (defined (RTE_PPC_CORSTONE310_PERIPH0) && (RTE_PPC_CORSTONE310_PERIPH0 == 1))

static CORSTONE310_PPC_Resources PERIPH0_PPC_DEV = {
    .dev = &PPC_CORSTONE310_PERIPH0_DEV,
};

/* PERIPH0 Driver wrapper functions */
static int32_t PPC_CORSTONE310_PERIPH0_Initialize(void)
{
    ppc_corstone310_init(PERIPH0_PPC_DEV.dev);

    return ARM_DRIVER_OK;
}

static int32_t PPC_CORSTONE310_PERIPH0_Uninitialize(void)
{
    /* Nothing to do */
    return ARM_DRIVER_OK;
}

static int32_t
PPC_CORSTONE310_PERIPH0_ConfigPrivilege(uint32_t periph,
                                   PPC_CORSTONE310_SecAttr sec_attr,
                                   PPC_CORSTONE310_PrivAttr priv_attr)
{
    enum ppc_corstone310_error_t ret;

    ret = ppc_corstone310_config_privilege(PERIPH0_PPC_DEV.dev, periph,
                                      (enum ppc_corstone310_sec_attr_t)sec_attr,
                                      (enum ppc_corstone310_priv_attr_t)priv_attr);

    if( ret != PPC_CORSTONE310_ERR_NONE) {
        return ARM_DRIVER_ERROR;
    }

    return ARM_DRIVER_OK;
}

static bool PPC_CORSTONE310_PERIPH0_IsPeriphPrivOnly(uint32_t periph)
{
    return ppc_corstone310_is_periph_priv_only(PERIPH0_PPC_DEV.dev, periph);
}

/* Secure only functions */
#if (defined (__ARM_FEATURE_CMSE) && (__ARM_FEATURE_CMSE == 3U))

static int32_t
PPC_CORSTONE310_PERIPH0_ConfigSecurity(uint32_t periph,
                                  PPC_CORSTONE310_SecAttr sec_attr)
{
    enum ppc_corstone310_error_t ret;

    ret = ppc_corstone310_config_security(PERIPH0_PPC_DEV.dev, periph,
                                     (enum ppc_corstone310_sec_attr_t)sec_attr);

    if( ret != PPC_CORSTONE310_ERR_NONE) {
        return ARM_DRIVER_ERROR;
    }

    return ARM_DRIVER_OK;
}

static bool PPC_CORSTONE310_PERIPH0_IsPeriphSecure(uint32_t periph)
{
    return ppc_corstone310_is_periph_secure(PERIPH0_PPC_DEV.dev, periph);
}

static int32_t PPC_CORSTONE310_PERIPH0_EnableInterrupt(void)
{
    enum ppc_corstone310_error_t ret;

    ret = ppc_corstone310_irq_enable(PERIPH0_PPC_DEV.dev);

    if( ret != PPC_CORSTONE310_ERR_NONE) {
        return ARM_DRIVER_ERROR;
    }

    return ARM_DRIVER_OK;
}

static void PPC_CORSTONE310_PERIPH0_DisableInterrupt(void)
{
    ppc_corstone310_irq_disable(PERIPH0_PPC_DEV.dev);
}

static void PPC_CORSTONE310_PERIPH0_ClearInterrupt(void)
{
    ppc_corstone310_clear_irq(PERIPH0_PPC_DEV.dev);
}

static bool PPC_CORSTONE310_PERIPH0_InterruptState(void)
{
    return ppc_corstone310_irq_state(PERIPH0_PPC_DEV.dev);
}

#endif /* (defined (__ARM_FEATURE_CMSE) && (__ARM_FEATURE_CMSE == 3U)) */

/* PPC Corstone-310 PERIPH0 Driver CMSIS access structure */
DRIVER_PPC_CORSTONE310 Driver_PPC_CORSTONE310_PERIPH0 = {
    .GetVersion        = PPC_CORSTONE310_GetVersion,
    .Initialize        = PPC_CORSTONE310_PERIPH0_Initialize,
    .Uninitialize      = PPC_CORSTONE310_PERIPH0_Uninitialize,
    .ConfigPrivilege   = PPC_CORSTONE310_PERIPH0_ConfigPrivilege,
    .IsPeriphPrivOnly  = PPC_CORSTONE310_PERIPH0_IsPeriphPrivOnly,
#if (defined (__ARM_FEATURE_CMSE) && (__ARM_FEATURE_CMSE == 3U))
    .ConfigSecurity    = PPC_CORSTONE310_PERIPH0_ConfigSecurity,
    .IsPeriphSecure    = PPC_CORSTONE310_PERIPH0_IsPeriphSecure,
    .EnableInterrupt   = PPC_CORSTONE310_PERIPH0_EnableInterrupt,
    .DisableInterrupt  = PPC_CORSTONE310_PERIPH0_DisableInterrupt,
    .ClearInterrupt    = PPC_CORSTONE310_PERIPH0_ClearInterrupt,
    .InterruptState    = PPC_CORSTONE310_PERIPH0_InterruptState
#endif /* (defined (__ARM_FEATURE_CMSE) && (__ARM_FEATURE_CMSE == 3U)) */
};
#endif /* RTE_PPC_CORSTONE310_PERIPH0 */

#if (defined (RTE_PPC_CORSTONE310_PERIPH1) && (RTE_PPC_CORSTONE310_PERIPH1 == 1))

static CORSTONE310_PPC_Resources PERIPH1_PPC_DEV = {
    .dev = &PPC_CORSTONE310_PERIPH1_DEV,
};

/* PERIPH1 Driver wrapper functions */
static int32_t PPC_CORSTONE310_PERIPH1_Initialize(void)
{
    ppc_corstone310_init(PERIPH1_PPC_DEV.dev);

    return ARM_DRIVER_OK;
}

static int32_t PPC_CORSTONE310_PERIPH1_Uninitialize(void)
{
    /* Nothing to do */
    return ARM_DRIVER_OK;
}

static int32_t
PPC_CORSTONE310_PERIPH1_ConfigPrivilege(uint32_t periph,
                                   PPC_CORSTONE310_SecAttr sec_attr,
                                   PPC_CORSTONE310_PrivAttr priv_attr)
{
    enum ppc_corstone310_error_t ret;

    ret = ppc_corstone310_config_privilege(PERIPH1_PPC_DEV.dev, periph,
                                      (enum ppc_corstone310_sec_attr_t)sec_attr,
                                      (enum ppc_corstone310_priv_attr_t)priv_attr);

    if( ret != PPC_CORSTONE310_ERR_NONE) {
        return ARM_DRIVER_ERROR;
    }

    return ARM_DRIVER_OK;
}

static bool PPC_CORSTONE310_PERIPH1_IsPeriphPrivOnly(uint32_t periph)
{
    return ppc_corstone310_is_periph_priv_only(PERIPH1_PPC_DEV.dev, periph);
}

/* Secure only functions */
#if (defined (__ARM_FEATURE_CMSE) && (__ARM_FEATURE_CMSE == 3U))

static int32_t
PPC_CORSTONE310_PERIPH1_ConfigSecurity(uint32_t periph,
                                  PPC_CORSTONE310_SecAttr sec_attr)
{
    enum ppc_corstone310_error_t ret;

    ret = ppc_corstone310_config_security(PERIPH1_PPC_DEV.dev, periph,
                                     (enum ppc_corstone310_sec_attr_t)sec_attr);

    if( ret != PPC_CORSTONE310_ERR_NONE) {
        return ARM_DRIVER_ERROR;
    }

    return ARM_DRIVER_OK;
}

static bool PPC_CORSTONE310_PERIPH1_IsPeriphSecure(uint32_t periph)
{
    return ppc_corstone310_is_periph_secure(PERIPH1_PPC_DEV.dev, periph);
}

static int32_t PPC_CORSTONE310_PERIPH1_EnableInterrupt(void)
{
    enum ppc_corstone310_error_t ret;

    ret = ppc_corstone310_irq_enable(PERIPH1_PPC_DEV.dev);

    if( ret != PPC_CORSTONE310_ERR_NONE) {
        return ARM_DRIVER_ERROR;
    }

    return ARM_DRIVER_OK;
}

static void PPC_CORSTONE310_PERIPH1_DisableInterrupt(void)
{
    ppc_corstone310_irq_disable(PERIPH1_PPC_DEV.dev);
}

static void PPC_CORSTONE310_PERIPH1_ClearInterrupt(void)
{
    ppc_corstone310_clear_irq(PERIPH1_PPC_DEV.dev);
}

static bool PPC_CORSTONE310_PERIPH1_InterruptState(void)
{
    return ppc_corstone310_irq_state(PERIPH1_PPC_DEV.dev);
}

#endif /* (defined (__ARM_FEATURE_CMSE) && (__ARM_FEATURE_CMSE == 3U)) */

/* PPC Corstone-310 PERIPH1 Driver CMSIS access structure */
DRIVER_PPC_CORSTONE310 Driver_PPC_CORSTONE310_PERIPH1 = {
    .GetVersion        = PPC_CORSTONE310_GetVersion,
    .Initialize        = PPC_CORSTONE310_PERIPH1_Initialize,
    .Uninitialize      = PPC_CORSTONE310_PERIPH1_Uninitialize,
    .ConfigPrivilege   = PPC_CORSTONE310_PERIPH1_ConfigPrivilege,
    .IsPeriphPrivOnly  = PPC_CORSTONE310_PERIPH1_IsPeriphPrivOnly,
#if (defined (__ARM_FEATURE_CMSE) && (__ARM_FEATURE_CMSE == 3U))
    .ConfigSecurity    = PPC_CORSTONE310_PERIPH1_ConfigSecurity,
    .IsPeriphSecure    = PPC_CORSTONE310_PERIPH1_IsPeriphSecure,
    .EnableInterrupt   = PPC_CORSTONE310_PERIPH1_EnableInterrupt,
    .DisableInterrupt  = PPC_CORSTONE310_PERIPH1_DisableInterrupt,
    .ClearInterrupt    = PPC_CORSTONE310_PERIPH1_ClearInterrupt,
    .InterruptState    = PPC_CORSTONE310_PERIPH1_InterruptState
#endif /* (defined (__ARM_FEATURE_CMSE) && (__ARM_FEATURE_CMSE == 3U)) */
};
#endif /* RTE_PPC_CORSTONE310_PERIPH1 */

#if (defined (RTE_PPC_CORSTONE310_PERIPH_EXP0) && (RTE_PPC_CORSTONE310_PERIPH_EXP0 == 1))

static CORSTONE310_PPC_Resources PERIPH_EXP0_PPC_DEV = {
    .dev = &PPC_CORSTONE310_PERIPH_EXP0_DEV,
};

/* PERIPH PPCEXP0 Driver wrapper functions */
static int32_t PPC_CORSTONE310_PERIPH_EXP0_Initialize(void)
{
    ppc_corstone310_init(PERIPH_EXP0_PPC_DEV.dev);

    return ARM_DRIVER_OK;
}

static int32_t PPC_CORSTONE310_PERIPH_EXP0_Uninitialize(void)
{
    /* Nothing to do */
    return ARM_DRIVER_OK;
}

static int32_t
PPC_CORSTONE310_PERIPH_EXP0_ConfigPrivilege(uint32_t periph,
                                       PPC_CORSTONE310_SecAttr sec_attr,
                                       PPC_CORSTONE310_PrivAttr priv_attr)
{
    enum ppc_corstone310_error_t ret;

    ret = ppc_corstone310_config_privilege(PERIPH_EXP0_PPC_DEV.dev, periph,
                                      (enum ppc_corstone310_sec_attr_t)sec_attr,
                                      (enum ppc_corstone310_priv_attr_t)priv_attr);

    if( ret != PPC_CORSTONE310_ERR_NONE) {
        return ARM_DRIVER_ERROR;
    }

    return ARM_DRIVER_OK;
}

static bool PPC_CORSTONE310_PERIPH_EXP0_IsPeriphPrivOnly(uint32_t periph)
{
    return ppc_corstone310_is_periph_priv_only(PERIPH_EXP0_PPC_DEV.dev, periph);
}

/* Secure only functions */
#if (defined (__ARM_FEATURE_CMSE) && (__ARM_FEATURE_CMSE == 3U))

static int32_t
PPC_CORSTONE310_PERIPH_EXP0_ConfigSecurity(uint32_t periph,
                                      PPC_CORSTONE310_SecAttr sec_attr)
{
    enum ppc_corstone310_error_t ret;

    ret = ppc_corstone310_config_security(PERIPH_EXP0_PPC_DEV.dev, periph,
                                     (enum ppc_corstone310_sec_attr_t)sec_attr);

    if( ret != PPC_CORSTONE310_ERR_NONE) {
        return ARM_DRIVER_ERROR;
    }

    return ARM_DRIVER_OK;
}

static bool PPC_CORSTONE310_PERIPH_EXP0_IsPeriphSecure(uint32_t periph)
{
    return ppc_corstone310_is_periph_secure(PERIPH_EXP0_PPC_DEV.dev, periph);
}

static int32_t PPC_CORSTONE310_PERIPH_EXP0_EnableInterrupt(void)
{
    enum ppc_corstone310_error_t ret;

    ret = ppc_corstone310_irq_enable(PERIPH_EXP0_PPC_DEV.dev);

    if( ret != PPC_CORSTONE310_ERR_NONE) {
        return ARM_DRIVER_ERROR;
    }

    return ARM_DRIVER_OK;
}

static void PPC_CORSTONE310_PERIPH_EXP0_DisableInterrupt(void)
{
    ppc_corstone310_irq_disable(PERIPH_EXP0_PPC_DEV.dev);
}

static void PPC_CORSTONE310_PERIPH_EXP0_ClearInterrupt(void)
{
    ppc_corstone310_clear_irq(PERIPH_EXP0_PPC_DEV.dev);
}

static bool PPC_CORSTONE310_PERIPH_EXP0_InterruptState(void)
{
    return ppc_corstone310_irq_state(PERIPH_EXP0_PPC_DEV.dev);
}

#endif /* (defined (__ARM_FEATURE_CMSE) && (__ARM_FEATURE_CMSE == 3U)) */

/* PPC Corstone-310 PERIPH EXP0 Driver CMSIS access structure */
DRIVER_PPC_CORSTONE310 Driver_PPC_CORSTONE310_PERIPH_EXP0 = {
    .GetVersion        = PPC_CORSTONE310_GetVersion,
    .Initialize        = PPC_CORSTONE310_PERIPH_EXP0_Initialize,
    .Uninitialize      = PPC_CORSTONE310_PERIPH_EXP0_Uninitialize,
    .ConfigPrivilege   = PPC_CORSTONE310_PERIPH_EXP0_ConfigPrivilege,
    .IsPeriphPrivOnly  = PPC_CORSTONE310_PERIPH_EXP0_IsPeriphPrivOnly,
#if (defined (__ARM_FEATURE_CMSE) && (__ARM_FEATURE_CMSE == 3U))
    .ConfigSecurity    = PPC_CORSTONE310_PERIPH_EXP0_ConfigSecurity,
    .IsPeriphSecure    = PPC_CORSTONE310_PERIPH_EXP0_IsPeriphSecure,
    .EnableInterrupt   = PPC_CORSTONE310_PERIPH_EXP0_EnableInterrupt,
    .DisableInterrupt  = PPC_CORSTONE310_PERIPH_EXP0_DisableInterrupt,
    .ClearInterrupt    = PPC_CORSTONE310_PERIPH_EXP0_ClearInterrupt,
    .InterruptState    = PPC_CORSTONE310_PERIPH_EXP0_InterruptState
#endif /* (defined (__ARM_FEATURE_CMSE) && (__ARM_FEATURE_CMSE == 3U)) */
};
#endif /* RTE_PPC_CORSTONE310_PERIPH_EXP0 */

#if (defined (RTE_PPC_CORSTONE310_PERIPH_EXP1) && (RTE_PPC_CORSTONE310_PERIPH_EXP1 == 1))

static CORSTONE310_PPC_Resources PERIPH_EXP1_PPC_DEV = {
    .dev = &PPC_CORSTONE310_PERIPH_EXP1_DEV,
};

/* PERIPH PPCEXP1 Driver wrapper functions */
static int32_t PPC_CORSTONE310_PERIPH_EXP1_Initialize(void)
{
    ppc_corstone310_init(PERIPH_EXP1_PPC_DEV.dev);

    return ARM_DRIVER_OK;
}

static int32_t PPC_CORSTONE310_PERIPH_EXP1_Uninitialize(void)
{
    /* Nothing to do */
    return ARM_DRIVER_OK;
}

static int32_t
PPC_CORSTONE310_PERIPH_EXP1_ConfigPrivilege(uint32_t periph,
                                       PPC_CORSTONE310_SecAttr sec_attr,
                                       PPC_CORSTONE310_PrivAttr priv_attr)
{
    enum ppc_corstone310_error_t ret;

    ret = ppc_corstone310_config_privilege(PERIPH_EXP1_PPC_DEV.dev, periph,
                                      (enum ppc_corstone310_sec_attr_t)sec_attr,
                                      (enum ppc_corstone310_priv_attr_t)priv_attr);

    if( ret != PPC_CORSTONE310_ERR_NONE) {
        return ARM_DRIVER_ERROR;
    }

    return ARM_DRIVER_OK;
}

static bool PPC_CORSTONE310_PERIPH_EXP1_IsPeriphPrivOnly(uint32_t periph)
{
    return ppc_corstone310_is_periph_priv_only(PERIPH_EXP1_PPC_DEV.dev, periph);
}

/* Secure only functions */
#if (defined (__ARM_FEATURE_CMSE) && (__ARM_FEATURE_CMSE == 3U))

static int32_t
PPC_CORSTONE310_PERIPH_EXP1_ConfigSecurity(uint32_t periph,
                                      PPC_CORSTONE310_SecAttr sec_attr)
{
    enum ppc_corstone310_error_t ret;

    ret = ppc_corstone310_config_security(PERIPH_EXP1_PPC_DEV.dev, periph,
                                     (enum ppc_corstone310_sec_attr_t)sec_attr);

    if( ret != PPC_CORSTONE310_ERR_NONE) {
        return ARM_DRIVER_ERROR;
    }

    return ARM_DRIVER_OK;
}

static bool PPC_CORSTONE310_PERIPH_EXP1_IsPeriphSecure(uint32_t periph)
{
    return ppc_corstone310_is_periph_secure(PERIPH_EXP1_PPC_DEV.dev, periph);
}

static int32_t PPC_CORSTONE310_PERIPH_EXP1_EnableInterrupt(void)
{
    enum ppc_corstone310_error_t ret;

    ret = ppc_corstone310_irq_enable(PERIPH_EXP1_PPC_DEV.dev);

    if( ret != PPC_CORSTONE310_ERR_NONE) {
        return ARM_DRIVER_ERROR;
    }

    return ARM_DRIVER_OK;
}

static void PPC_CORSTONE310_PERIPH_EXP1_DisableInterrupt(void)
{
    ppc_corstone310_irq_disable(PERIPH_EXP1_PPC_DEV.dev);
}

static void PPC_CORSTONE310_PERIPH_EXP1_ClearInterrupt(void)
{
    ppc_corstone310_clear_irq(PERIPH_EXP1_PPC_DEV.dev);
}

static bool PPC_CORSTONE310_PERIPH_EXP1_InterruptState(void)
{
    return ppc_corstone310_irq_state(PERIPH_EXP1_PPC_DEV.dev);
}

#endif /* (defined (__ARM_FEATURE_CMSE) && (__ARM_FEATURE_CMSE == 3U)) */

/* PPC Corstone-310 PERIPH EXP1 Driver CMSIS access structure */
DRIVER_PPC_CORSTONE310 Driver_PPC_CORSTONE310_PERIPH_EXP1 = {
    .GetVersion        = PPC_CORSTONE310_GetVersion,
    .Initialize        = PPC_CORSTONE310_PERIPH_EXP1_Initialize,
    .Uninitialize      = PPC_CORSTONE310_PERIPH_EXP1_Uninitialize,
    .ConfigPrivilege   = PPC_CORSTONE310_PERIPH_EXP1_ConfigPrivilege,
    .IsPeriphPrivOnly  = PPC_CORSTONE310_PERIPH_EXP1_IsPeriphPrivOnly,
#if (defined (__ARM_FEATURE_CMSE) && (__ARM_FEATURE_CMSE == 3U))
    .ConfigSecurity    = PPC_CORSTONE310_PERIPH_EXP1_ConfigSecurity,
    .IsPeriphSecure    = PPC_CORSTONE310_PERIPH_EXP1_IsPeriphSecure,
    .EnableInterrupt   = PPC_CORSTONE310_PERIPH_EXP1_EnableInterrupt,
    .DisableInterrupt  = PPC_CORSTONE310_PERIPH_EXP1_DisableInterrupt,
    .ClearInterrupt    = PPC_CORSTONE310_PERIPH_EXP1_ClearInterrupt,
    .InterruptState    = PPC_CORSTONE310_PERIPH_EXP1_InterruptState
#endif /* (defined (__ARM_FEATURE_CMSE) && (__ARM_FEATURE_CMSE == 3U)) */
};
#endif /* RTE_PPC_CORSTONE310_PERIPH_EXP1 */

#if (defined (RTE_PPC_CORSTONE310_PERIPH_EXP2) && (RTE_PPC_CORSTONE310_PERIPH_EXP2 == 1))

static CORSTONE310_PPC_Resources PERIPH_EXP2_PPC_DEV = {
    .dev = &PPC_CORSTONE310_PERIPH_EXP2_DEV,
};

/* PERIPH PPCEXP2 Driver wrapper functions */
static int32_t PPC_CORSTONE310_PERIPH_EXP2_Initialize(void)
{
    ppc_corstone310_init(PERIPH_EXP2_PPC_DEV.dev);

    return ARM_DRIVER_OK;
}

static int32_t PPC_CORSTONE310_PERIPH_EXP2_Uninitialize(void)
{
    /* Nothing to do */
    return ARM_DRIVER_OK;
}

static int32_t
PPC_CORSTONE310_PERIPH_EXP2_ConfigPrivilege(uint32_t periph,
                                       PPC_CORSTONE310_SecAttr sec_attr,
                                       PPC_CORSTONE310_PrivAttr priv_attr)
{
    enum ppc_corstone310_error_t ret;

    ret = ppc_corstone310_config_privilege(PERIPH_EXP2_PPC_DEV.dev, periph,
                                      (enum ppc_corstone310_sec_attr_t)sec_attr,
                                      (enum ppc_corstone310_priv_attr_t)priv_attr);

    if( ret != PPC_CORSTONE310_ERR_NONE) {
        return ARM_DRIVER_ERROR;
    }

    return ARM_DRIVER_OK;
}

static bool PPC_CORSTONE310_PERIPH_EXP2_IsPeriphPrivOnly(uint32_t periph)
{
    return ppc_corstone310_is_periph_priv_only(PERIPH_EXP2_PPC_DEV.dev, periph);
}

/* Secure only functions */
#if (defined (__ARM_FEATURE_CMSE) && (__ARM_FEATURE_CMSE == 3U))

static int32_t
PPC_CORSTONE310_PERIPH_EXP2_ConfigSecurity(uint32_t periph,
                                      PPC_CORSTONE310_SecAttr sec_attr)
{
    enum ppc_corstone310_error_t ret;

    ret = ppc_corstone310_config_security(PERIPH_EXP2_PPC_DEV.dev, periph,
                                     (enum ppc_corstone310_sec_attr_t)sec_attr);

    if( ret != PPC_CORSTONE310_ERR_NONE) {
        return ARM_DRIVER_ERROR;
    }

    return ARM_DRIVER_OK;
}

static bool PPC_CORSTONE310_PERIPH_EXP2_IsPeriphSecure(uint32_t periph)
{
    return ppc_corstone310_is_periph_secure(PERIPH_EXP2_PPC_DEV.dev, periph);
}

static int32_t PPC_CORSTONE310_PERIPH_EXP2_EnableInterrupt(void)
{
    enum ppc_corstone310_error_t ret;

    ret = ppc_corstone310_irq_enable(PERIPH_EXP2_PPC_DEV.dev);

    if( ret != PPC_CORSTONE310_ERR_NONE) {
        return ARM_DRIVER_ERROR;
    }

    return ARM_DRIVER_OK;
}

static void PPC_CORSTONE310_PERIPH_EXP2_DisableInterrupt(void)
{
    ppc_corstone310_irq_disable(PERIPH_EXP2_PPC_DEV.dev);
}

static void PPC_CORSTONE310_PERIPH_EXP2_ClearInterrupt(void)
{
    ppc_corstone310_clear_irq(PERIPH_EXP2_PPC_DEV.dev);
}

static bool PPC_CORSTONE310_PERIPH_EXP2_InterruptState(void)
{
    return ppc_corstone310_irq_state(PERIPH_EXP2_PPC_DEV.dev);
}

#endif /* (defined (__ARM_FEATURE_CMSE) && (__ARM_FEATURE_CMSE == 3U)) */

/* PPC Corstone-310 PERIPH EXP2 Driver CMSIS access structure */
DRIVER_PPC_CORSTONE310 Driver_PPC_CORSTONE310_PERIPH_EXP2 = {
    .GetVersion        = PPC_CORSTONE310_GetVersion,
    .Initialize        = PPC_CORSTONE310_PERIPH_EXP2_Initialize,
    .Uninitialize      = PPC_CORSTONE310_PERIPH_EXP2_Uninitialize,
    .ConfigPrivilege   = PPC_CORSTONE310_PERIPH_EXP2_ConfigPrivilege,
    .IsPeriphPrivOnly  = PPC_CORSTONE310_PERIPH_EXP2_IsPeriphPrivOnly,
#if (defined (__ARM_FEATURE_CMSE) && (__ARM_FEATURE_CMSE == 3U))
    .ConfigSecurity    = PPC_CORSTONE310_PERIPH_EXP2_ConfigSecurity,
    .IsPeriphSecure    = PPC_CORSTONE310_PERIPH_EXP2_IsPeriphSecure,
    .EnableInterrupt   = PPC_CORSTONE310_PERIPH_EXP2_EnableInterrupt,
    .DisableInterrupt  = PPC_CORSTONE310_PERIPH_EXP2_DisableInterrupt,
    .ClearInterrupt    = PPC_CORSTONE310_PERIPH_EXP2_ClearInterrupt,
    .InterruptState    = PPC_CORSTONE310_PERIPH_EXP2_InterruptState
#endif /* (defined (__ARM_FEATURE_CMSE) && (__ARM_FEATURE_CMSE == 3U)) */
};
#endif /* RTE_PPC_CORSTONE310_PERIPH_EXP2 */

#if (defined (RTE_PPC_CORSTONE310_PERIPH_EXP3) && (RTE_PPC_CORSTONE310_PERIPH_EXP3 == 1))

static CORSTONE310_PPC_Resources PERIPH_EXP3_PPC_DEV = {
    .dev = &PPC_CORSTONE310_PERIPH_EXP3_DEV,
};

/* PERIPH PPCEXP3 Driver wrapper functions */
static int32_t PPC_CORSTONE310_PERIPH_EXP3_Initialize(void)
{
    ppc_corstone310_init(PERIPH_EXP3_PPC_DEV.dev);

    return ARM_DRIVER_OK;
}

static int32_t PPC_CORSTONE310_PERIPH_EXP3_Uninitialize(void)
{
    /* Nothing to do */
    return ARM_DRIVER_OK;
}

static int32_t
PPC_CORSTONE310_PERIPH_EXP3_ConfigPrivilege(uint32_t periph,
                                       PPC_CORSTONE310_SecAttr sec_attr,
                                       PPC_CORSTONE310_PrivAttr priv_attr)
{
    enum ppc_corstone310_error_t ret;

    ret = ppc_corstone310_config_privilege(PERIPH_EXP3_PPC_DEV.dev, periph,
                                      (enum ppc_corstone310_sec_attr_t)sec_attr,
                                      (enum ppc_corstone310_priv_attr_t)priv_attr);

    if( ret != PPC_CORSTONE310_ERR_NONE) {
        return ARM_DRIVER_ERROR;
    }

    return ARM_DRIVER_OK;
}

static bool PPC_CORSTONE310_PERIPH_EXP3_IsPeriphPrivOnly(uint32_t periph)
{
    return ppc_corstone310_is_periph_priv_only(PERIPH_EXP3_PPC_DEV.dev, periph);
}

/* Secure only functions */
#if (defined (__ARM_FEATURE_CMSE) && (__ARM_FEATURE_CMSE == 3U))

static int32_t
PPC_CORSTONE310_PERIPH_EXP3_ConfigSecurity(uint32_t periph,
                                      PPC_CORSTONE310_SecAttr sec_attr)
{
    enum ppc_corstone310_error_t ret;

    ret = ppc_corstone310_config_security(PERIPH_EXP3_PPC_DEV.dev, periph,
                                     (enum ppc_corstone310_sec_attr_t)sec_attr);

    if( ret != PPC_CORSTONE310_ERR_NONE) {
        return ARM_DRIVER_ERROR;
    }

    return ARM_DRIVER_OK;
}

static bool PPC_CORSTONE310_PERIPH_EXP3_IsPeriphSecure(uint32_t periph)
{
    return ppc_corstone310_is_periph_secure(PERIPH_EXP3_PPC_DEV.dev, periph);
}

static int32_t PPC_CORSTONE310_PERIPH_EXP3_EnableInterrupt(void)
{
    enum ppc_corstone310_error_t ret;

    ret = ppc_corstone310_irq_enable(PERIPH_EXP3_PPC_DEV.dev);

    if( ret != PPC_CORSTONE310_ERR_NONE) {
        return ARM_DRIVER_ERROR;
    }

    return ARM_DRIVER_OK;
}

static void PPC_CORSTONE310_PERIPH_EXP3_DisableInterrupt(void)
{
    ppc_corstone310_irq_disable(PERIPH_EXP3_PPC_DEV.dev);
}

static void PPC_CORSTONE310_PERIPH_EXP3_ClearInterrupt(void)
{
    ppc_corstone310_clear_irq(PERIPH_EXP3_PPC_DEV.dev);
}

static bool PPC_CORSTONE310_PERIPH_EXP3_InterruptState(void)
{
    return ppc_corstone310_irq_state(PERIPH_EXP3_PPC_DEV.dev);
}

#endif /* (defined (__ARM_FEATURE_CMSE) && (__ARM_FEATURE_CMSE == 3U)) */

/* PPC Corstone-310 PERIPH EXP3 Driver CMSIS access structure */
DRIVER_PPC_CORSTONE310 Driver_PPC_CORSTONE310_PERIPH_EXP3 = {
    .GetVersion        = PPC_CORSTONE310_GetVersion,
    .Initialize        = PPC_CORSTONE310_PERIPH_EXP3_Initialize,
    .Uninitialize      = PPC_CORSTONE310_PERIPH_EXP3_Uninitialize,
    .ConfigPrivilege   = PPC_CORSTONE310_PERIPH_EXP3_ConfigPrivilege,
    .IsPeriphPrivOnly  = PPC_CORSTONE310_PERIPH_EXP3_IsPeriphPrivOnly,
#if (defined (__ARM_FEATURE_CMSE) && (__ARM_FEATURE_CMSE == 3U))
    .ConfigSecurity    = PPC_CORSTONE310_PERIPH_EXP3_ConfigSecurity,
    .IsPeriphSecure    = PPC_CORSTONE310_PERIPH_EXP3_IsPeriphSecure,
    .EnableInterrupt   = PPC_CORSTONE310_PERIPH_EXP3_EnableInterrupt,
    .DisableInterrupt  = PPC_CORSTONE310_PERIPH_EXP3_DisableInterrupt,
    .ClearInterrupt    = PPC_CORSTONE310_PERIPH_EXP3_ClearInterrupt,
    .InterruptState    = PPC_CORSTONE310_PERIPH_EXP3_InterruptState
#endif /* (defined (__ARM_FEATURE_CMSE) && (__ARM_FEATURE_CMSE == 3U)) */
};
#endif /* RTE_PPC_CORSTONE310_PERIPH_EXP3 */
#endif
