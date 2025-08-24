/*********************************************************************
 * @file        APP_ACT.h
 * @brief       Template_BriefDescription.
 * @note        TemplateDetailsDescription.\n
 *
 * @author      xxxxxx
 * @date        jj/mm/yyyy
 * @version     1.0
 */
  
#ifndef APP_ACT_H_INCLUDED
#define APP_ACT_H_INCLUDED





    // ********************************************************************
    // *                      Includes
    // ********************************************************************
    #include "TypeCommon.h"
    #include "APP_CFG/ConfigFiles/APPACT_ConfigPublic.h"
    // ********************************************************************
    // *                      Defines
    // ********************************************************************

    // ********************************************************************
    // *                      Types
    // ********************************************************************
	/* CAUTION : Automatic generated code section for Enum: Start */

	/* CAUTION : Automatic generated code section for Enum: End */
	//-----------------------------ENUM TYPES-----------------------------//


	/* CAUTION : Automatic generated code section for Structure: Start */

	/* CAUTION : Automatic generated code section for Structure: End */
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
    /**
    *
    *	@brief      Perform all Init action for this module.\n
    *
    */
    t_eReturnCode APPACT_Init(void);
    /**
    *
    *	@brief      Perform all Cyclic action for this module.\n
    *   @note       In preOpe mode -> If one of the configuration is not set the Module Cyclic 
    *               retry indefinitely and the module state doesn't change until all 
    *               actuator configuration are set
    *               In Ope mode -> call driver cyclic
    *
    */
    t_eReturnCode APPACT_Cyclic(void);
    /**
    *
    *	@brief Function to know the module state.\n 
    *
    *	@param[in]  f_State_pe : store the value, value from @ref t_eCyclicModState
    *
    *   @retval RC_OK                             @ref RC_OK
    *   @retval RC_ERROR_PTR_NULL                 @ref RC_ERROR_PTR_NUL
    */
    t_eReturnCode APPACT_GetState(t_eCyclicModState *f_State_pe);
    /**
    *
    *	@brief Function to update the module state.\n
    *
    *	@param[in]  f_State_e : the new value, value from @ref t_eCyclicModState
    *
    *   @retval RC_OK                             @ref RC_OK
    */
    t_eReturnCode APPACT_SetState(t_eCyclicModState f_State_e);
    /**
    *
    *	@brief  Get actuator value 
    *
    *	@param[in]  f_actuator_e   : actuator enum
    *	@param[in]  f_value_ps16   : storage for the value 
    * 
    */
    t_eReturnCode APPACT_GetActValue(t_eAPPACT_ActInterface f_actuator_e, t_float32 * f_actValue_pf32);
    /**
    *
    *	@brief      Set value for an actuator
    *
    *	@param[in]  f_actuator_e   : actuator enum
    *	@param[in] f_value_s16 : value to set
    *
    */
    t_eReturnCode APPACT_SetActValue(t_eAPPACT_ActInterface f_actuator_e, t_float32  f_actValue_f32);

#endif // APP_ACT_H_INCLUDED           
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
