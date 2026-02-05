#ifndef CAN_PAL_MAPPING_H
#define CAN_PAL_MAPPING_H

#include "can_pal_cfg.h"

/* Include PD files */
#if (defined(CAN_OVER_FLEXCAN))
    #include "flexcan_driver.h"
#endif

/*!
 * @brief Enumeration with the types of peripherals supported by CAN PAL
 *
 * This enumeration contains the types of peripherals supported by CAN PAL.
 * Implements : can_inst_type_t_Class
 */
typedef enum {
#if defined(CAN_INSTANCE_COUNT)
    CAN_INST_TYPE_FLEXCAN = 0u,
#endif /* defined(CAN_INSTANCE_COUNT) */
} can_inst_type_t;

/*!
 * @brief Structure storing PAL instance information
 *
 * This structure is used for storing PAL instance information.
 * Implements : can_instance_t_Class
 */
typedef struct {
    can_inst_type_t instType;    /*!< Peripheral over which the PAL is used */
    uint32_t instIdx;            /*!< Instance index of the peripheral over which the PAL is used */
} can_instance_t;

#endif /* CAN_PAL_MAPPING_H */
