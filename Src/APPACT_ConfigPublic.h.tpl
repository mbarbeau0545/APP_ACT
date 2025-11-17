/*********************************************************************
 * @file        APPACT_ConfigPublic.h
 * @brief       Template_BriefDescription.
 * @note        TemplateDetailsDescription.\n
 *
 * @author      xxxxxx
 * @date        jj/mm/yyyy
 * @version     1.0
 */
  
#ifndef APPACT_CONFIGPUBLIC_H_INCLUDED
#define APPACT_CONFIGPUBLIC_H_INCLUDED





    // ********************************************************************
    // *                      Includes
    // ********************************************************************
    #include "TypeCommon.h"
    // ********************************************************************
    // *                      Defines
    // ********************************************************************
    ///@brief Define for normalize return function of spec files
    #define APPACT_MOTOR_STS_OFF            ((t_float32)0.0f)
    #define APPACT_MOTOR_STS_ON             ((t_float32)1.0f)
    #define APPACT_MOTOR_STS_ENDSTOP_CW     ((t_float32)2.0f)
    #define APPACT_MOTOR_STS_ENDSTOP_CCW    ((t_float32)3.0f)
    
    #define APPACT_SOFT_STOP                ((t_float32)0.0f)
    #define APPACT_HARD_STOP                ((t_float32)-1.0f)
    #define APPACT_ENABLE_MOTOR             ((t_float32)-2.0f)

    ///@brief define normalize direction of spec files 
    #define APPACT_DIRECTION_CW ((t_float32)0.0f)
    #define APPACT_DIRECTION_CCW ((t_float32)1.0f)
    // ********************************************************************
    // *                      Types
    // ********************************************************************
    /* CAUTION : Automatic generated code section for Enum: Start */
    /**
    * @brief Enum for Actuators list.
    */
    typedef enum
    {
        APPACT_ACTITF_MTR_XL_PULSE = 0,       /**< Actuator Device MTR_XL, Interface PULSE, Moteur XL Left Pulses */
        APPACT_ACTITF_MTR_XL_SPEED,             /**< Actuator Device MTR_XL, Interface SPEED, Moteur XL Left Speed */
        APPACT_ACTITF_MTR_XL_TRIGTIME,          /**< Actuator Device MTR_XL, Interface TRIGTIME, Moteur XL Left Trigger Time to plan sending pulse */
        APPACT_ACTITF_MTR_XR_PULSE,             /**< Actuator Device MTR_XR, Interface PULSE, Moteur XR Left Pulses */
        APPACT_ACTITF_MTR_XR_SPEED,             /**< Actuator Device MTR_XR, Interface SPEED, Moteur XR Left Speed */
        APPACT_ACTITF_MTR_XR_TRIGTIME,          /**< Actuator Device MTR_XR, Interface TRIGTIME, Moteur XR RightTrigger Time to plan sending pulse */
        APPACT_ACTITF_MTR_Y_PULSE,              /**< Actuator Device MTR_Y, Interface PULSE, Moteur Y Pulses */
        APPACT_ACTITF_MTR_Y_SPEED,              /**< Actuator Device MTR_Y, Interface SPEED, Moteur Y Speed */
        APPACT_ACTITF_MTR_Y_TRIGTIME,           /**< Actuator Device MTR_Y, Interface TRIGTIME, Moteur Y Trigger Time to plan sending pulse */
        APPACT_ACTITF_MTR_Z_PULSE,              /**< Actuator Device MTR_Z, Interface PULSE, Moteur Z Pulses */
        APPACT_ACTITF_MTR_Z_SPEED,              /**< Actuator Device MTR_Z, Interface SPEED, Moteur Z Speed */
        APPACT_ACTITF_MTR_Z_TRIGTIME,           /**< Actuator Device MTR_Z, Interface TRIGTIME, Moteur Z Left Trigger Time to plan sending pulse */
    
        APPACT_ACTITF_NB,
    } t_eAPPACT_ActInterface;

    /**
    * @brief Enum for Actuators drivers list.
    */
    typedef enum
    {
        APPACT_DRV_CL42T = 0,              /**< CL42T Driver  */
    
        APPACT_DRV_NB,
    } t_eAPPACT_ActDriverList;

    /**
    * @brief Enumeration of all sensors device list.
    */
    typedef enum
    {
        APPACT_ACTDVC_MTR_XL = 0,
        APPACT_ACTDVC_MTR_XR,
        APPACT_ACTDVC_MTR_Y,
        APPACT_ACTDVC_MTR_Z,
    
        APPACT_ACTDVC_NB,
    } t_eAPPACT_ActDeviceList;

    /* CAUTION : Automatic generated code section for Enum: End */
	
    //-----------------------------ENUM TYPES-----------------------------//
	/* CAUTION : Automatic generated code section for Structure: Start */

	/* CAUTION : Automatic generated code section for Structure: End */
    /**< Union to store the idx for convert management */
   //-----------------------------STRUCT TYPES---------------------------//

	/* CAUTION : Automatic generated code section : Start */

	/* CAUTION : Automatic generated code section : End */
	//-----------------------------TYPEDEF TYPES---------------------------//
    // ********************************************************************
    // *                      Prototypes
    // ********************************************************************
        
    // ********************************************************************
    // *                      Variables
    // ********************************************************************
    


    //********************************************************************************
    //                      Public functions - Prototyupes
    //********************************************************************************

#endif // APPACT_CONFIGPUBLIC_H_INCLUDED           
//************************************************************************************
// End of File
//************************************************************************************

/**
 *
 *	@brief
 *	@note   
 *
 *
 *	@params[in] 
 *	@params[out]
 *	 
 *
 *
 */
