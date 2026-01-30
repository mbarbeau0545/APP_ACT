/*********************************************************************
 * @file        APP_SNS.c
 * @brief       Template_BriefDescription.
 * @note        TemplateDetailsDescription.\n
 *
 * @author      xxxxxx
 * @date        jj/mm/yyyy
 * @version     1.0
 */






// ********************************************************************
// *                      Includes
// ********************************************************************

#include "./APP_ACT.h"
#include "APP_CTRL/APP_SYS/Src/APP_SYS.h"
#include "Constant.h"
#include "APP_CFG/ConfigFiles/APPACT_ConfigPrivate.h"
// ********************************************************************
// *                      Defines
// ********************************************************************

// ********************************************************************
// *                      Types
// ********************************************************************
/* CAUTION : Automatic generated code section for Enum: Start */

/* CAUTION : Automatic generated code section for Enum: End */
//-----------------------------ENUM TYPES-----------------------------//
///@brief fsm state to cfg state
typedef enum 
{
    APPACT_FSM_CFGSTS_INIT_DRIVER = 0,      //---- Initialisation of driver ----//
    APPACT_FSM_CFGSTS_GET_CFG,              //---- Get xonfiguration from App Sys ----//
    APPACT_FSM_CFGSTS_APPLY_CFG,            //---- Apply configuration ----//
    APPACT_FSM_CFGSTS_SIG_REGISTER,         //---- Signal registration for Control Actuator Mode ----//
} t_eAPPACT_FsmCfgsts;

///@brief driver state 
typedef enum
{
    APPACT_DRV_STS_DISABLE = 0,
    APPACT_DRV_STS_ENABLE,
} t_eAPPACT_DrvState;
/* CAUTION : Automatic generated code section for Structure: Start */

/* CAUTION : Automatic generated code section for Structure: End */
//-----------------------------STRUCT TYPES---------------------------//
/* CAUTION : Automatic generated code section : Start */

/* CAUTION : Automatic generated code section : End */
//-----------------------------TYPEDEF TYPES---------------------------//
typedef struct 
{
    t_float32 setActValue_f32;            /**< For Debug Purpose */
    t_float32 getActValue_f32;            /**< For Debug Purpose */
    const t_sAPPACT_SysActCfg * cfgInfo_ps;
} t_sAPPACT_ActIfaceInfo;

typedef struct 
{
    t_uint8 dvcCfg_u8;                      //---- the device configuraiton ----//
    t_bool isConfigured_b;                  //---- flag to know if the configuration is done ----//
    const t_sAPPACT_ActDvcOpeCfg * dvcOpeCfg_ps;    //----  pointor to the cfg ----//
} t_sAPPACT_ActDvcInfo;
// ********************************************************************
// *                      Prototypes
// ********************************************************************
	
// ********************************************************************
// *                      Variables
// ********************************************************************
/* CAUTION : Automatic generated code section for Variable: Start */
/* CAUTION : Automatic generated code section for Variable: End */
static t_eCyclicModState g_AppAct_ModState_e = STATE_CYCLIC_CFG;
static t_eAPPACT_FsmCfgsts g_FsmCfgSts_e = APPACT_FSM_CFGSTS_INIT_DRIVER;
/**
 * @brief Sensors Interface Information
 */
static t_sAPPACT_ActIfaceInfo g_ActInterfaceInfo_as[APPACT_ACTITF_NB];

///@brief Device Interface Information 
static t_sAPPACT_ActDvcInfo g_ActDeviceInfo_as[APPACT_ACTDVC_NB];

/**< Variable for Sensors Drivers State*/
static t_eAPPACT_DrvState g_ActDrvState_ae[APPACT_DRV_NB];

///@brief Fast Task Managment 
static t_bool g_enableFastTask_b = FALSE;

///@brief When Acutator Control Mode is ON, Logic cannot set values
///            This happened whenever Ctrl Signal are used
static t_bool g_isCtrlModeON_b = FALSE;
//********************************************************************************
//                      Local functions - Prototypes
//********************************************************************************
/**
*
*	@brief
*	@note   
*
*
*/
static t_eReturnCode s_APPACT_ConfigurationState(void);
/**
*
*	@brief
*	@note   
*
*
*/
static t_eReturnCode s_APPACT_Fsm_CfgSts_GetCfg(void);
/**
*
*	@brief
*	@note   
*
*
*/
static t_eReturnCode s_APPACT_Fsm_CfgSts_ApplyCfg(void); 
/**
*
*	@brief
*	@note   
*
*
*/
static t_eReturnCode s_APPACT_Fsm_CfgSts_SigRegister(void);
/**
*
*	@brief
*	@note   
*
*
*/
static t_eReturnCode s_APPACT_Fsm_CfgSts_InitDriver(void);
/**
*
*	@brief
*	@note   
*
*
*/
static t_eReturnCode s_APPACT_PreOperational(void);
/**
 *
 *	@brief      Perform preOperationnal action.\n
 *  @note       Set the sensor configuration.\n
 *              Call driver init function.\n
 *              If one of the configuration is not set the Module Cyclic 
 *              retry indefinitely.\n
 *              
 * 
 *  @retval RC_OK                             @ref RC_OK
 *  @retval RC_ERROR_PARAM_INVALID            @ref RC_ERROR_PARAM_INVALID
 *  @retval RC_ERROR_WRONG_STATE              @ref RC_ERROR_WRONG_STATE

 */
static t_eReturnCode s_APPACT_Operational(void);
/**
 *
 *	@brief      Perform preOperationnal action.\n
 */
static void s_APPACT_FastTask(void);
/**
 *
 *	@brief      Perform preOperationnal action.\n
 */
static void s_APPACT_DebugRoutine(void);
/**
 * @brief This function handle the reception of signals from APPSIG
 * ----------------------------------------------------------------------------
 * @param[in] f_signal_e : signal ID
 * @param[in] f_sigVal_f32 :signl; value
 * ----------------------------------------------------------------------------
 * @return void
 */
static void s_APPACT_SigReceptionCallback(t_eAPPSIG_Signal f_signal_e, t_float32 f_sigVal_f32);
//****************************************************************************
//                      Public functions - Implementation
//********************************************************************************
/*********************************
 * APPACT_Init
 *********************************/
t_eReturnCode APPACT_Init(void)
{
    t_eReturnCode Ret_e = RC_OK;
    t_uint8 idxSnsIf_u8;
    t_uint8 idxDvcAct_u8;
    t_uint8 idxDrv_u8;

    // check sensors cfg
    for(idxSnsIf_u8 = (t_uint8)0 ; (idxSnsIf_u8 < APPACT_ACTITF_NB) && (Ret_e == RC_OK) ; idxSnsIf_u8++)
    {   
        if(c_AppAct_SysAct_as[idxSnsIf_u8].GetValue_pcb == NULL_FUNCTION
        || c_AppAct_SysAct_as[idxSnsIf_u8].SetValue_pcb == NULL_FUNCTION)
        {
            Ret_e = RC_ERROR_PARAM_INVALID;
            ASSERT((t_uint16)idxSnsIf_u8);
        }

        //---- set default value ----//
        g_ActInterfaceInfo_as[idxSnsIf_u8].setActValue_f32 = (t_float32)0.0f;
        g_ActInterfaceInfo_as[idxSnsIf_u8].cfgInfo_ps = &c_AppAct_SysAct_as[idxSnsIf_u8];
    }
    
    for(idxDvcAct_u8 = (t_uint8)0 ; (idxDvcAct_u8 < APPACT_ACTDVC_NB) && (Ret_e == RC_OK) ; idxDvcAct_u8++)
    {
        if(c_AppAct_ActDvcOpeCfg_as[idxDvcAct_u8].SetCfg_pcb == NULL_FUNCTION)
        {
            Ret_e = RC_ERROR_PARAM_INVALID;
            ASSERT((t_uint16)idxDvcAct_u8);
        }
        else
        {
            g_ActDeviceInfo_as[idxDvcAct_u8].isConfigured_b = (t_bool)TRUE;
            g_ActDeviceInfo_as[idxDvcAct_u8].dvcCfg_u8 = 0xFF;
            g_ActDeviceInfo_as[idxDvcAct_u8].dvcOpeCfg_ps = &c_AppAct_ActDvcOpeCfg_as[idxDvcAct_u8];
        }
    }
    for(idxDrv_u8 = (t_uint8)0 ; idxDrv_u8 < APPACT_DRV_NB ; idxDrv_u8++)
    {
        g_ActDrvState_ae[idxDrv_u8] = APPACT_DRV_STS_DISABLE;
    }
    if(Ret_e == RC_OK)
    {
        Ret_e = APPSYS_AddFastTask(APPSYS_MODULE_APP_ACT, s_APPACT_FastTask);
    }

    return Ret_e;
}
/*********************************
 * APPACT_Init
 *********************************/
t_eReturnCode APPACT_Cyclic(void)
{
    t_eReturnCode Ret_e = RC_OK;

    switch (g_AppAct_ModState_e)
    {
    case STATE_CYCLIC_CFG:
    {
        Ret_e = s_APPACT_ConfigurationState();
        if(Ret_e == RC_OK)
        {
            g_AppAct_ModState_e = STATE_CYCLIC_WAITING;
        }
        break;
    }
    case STATE_CYCLIC_PREOPE:
    {
        Ret_e = s_APPACT_PreOperational();
        if(Ret_e == RC_OK)
        {
            g_AppAct_ModState_e = STATE_CYCLIC_OPE;
        }
        break;
    }
    case STATE_CYCLIC_WAITING:
    {
        // nothing to do, just wait all module are Ope
        break;
    }
    case STATE_CYCLIC_OPE:
    {
        Ret_e = s_APPACT_Operational();
        if(Ret_e < RC_OK)
        {
            g_AppAct_ModState_e = STATE_CYCLIC_ERROR;
        }
        break;
    }
    case STATE_CYCLIC_ERROR:
    {
        break;
    }
    case STATE_CYCLIC_BUSY:
    default:
        Ret_e = RC_OK;
        break;
    }
    return Ret_e;
}

/*********************************
 * APPACT_GetState
 *********************************/
t_eReturnCode APPACT_GetState(t_eCyclicModState *f_State_pe)
{
    t_eReturnCode Ret_e = RC_OK;
    
    if(f_State_pe == (t_eCyclicModState *)NULL)
    {
        Ret_e = RC_ERROR_PTR_NULL;
    }
    if(Ret_e == RC_OK)
    {
        *f_State_pe = g_AppAct_ModState_e;
    }
    return Ret_e;
}
/*********************************
 * APPACT_GetState
 *********************************/
t_eReturnCode APPACT_SetState(t_eCyclicModState f_State_e)
{
    g_AppAct_ModState_e = f_State_e;
    return RC_OK;
}

/*********************************
 * APPACT_GetActValue
 *********************************/
t_eReturnCode APPACT_GetActValue(t_eAPPACT_ActInterface f_actuator_e, t_float32 * f_actValue_pf32)
{
    t_eReturnCode Ret_e = RC_OK;
    t_eAPPACT_ActDeviceList actDeviceLink_e;
    t_sAPPACT_ActIfaceInfo * actItfInfo_ps;
    t_float32 tmpsetActValue_f32 = 0.0f;

    if(g_AppAct_ModState_e != STATE_CYCLIC_OPE)
    {
        Ret_e = RC_WARNING_BUSY;
    }
    if(f_actValue_pf32 == (t_float32 *)NULL)
    {
        Ret_e = RC_ERROR_PTR_NULL;
        ASSERT((t_uint16)0);
    }
    else if(f_actuator_e > APPACT_ACTITF_NB)
    {
        Ret_e = RC_ERROR_PARAM_INVALID;
        ASSERT((t_uint16)f_actuator_e);
    }
    else 
    {
        actItfInfo_ps = (t_sAPPACT_ActIfaceInfo *)(&g_ActInterfaceInfo_as[f_actuator_e]);
        actDeviceLink_e = actItfInfo_ps->cfgInfo_ps->deviceLink_e;

        if(g_ActDeviceInfo_as[actDeviceLink_e].isConfigured_b == (t_bool)FALSE)
        {
            Ret_e = RC_ERROR_MISSING_CONFIG;
            ASSERT((t_uint16)0);
        }
        else 
        {
            // call specific function to get value
            Ret_e = actItfInfo_ps->cfgInfo_ps->GetValue_pcb(&tmpsetActValue_f32);
            
            if(Ret_e == RC_OK)
            {
                *f_actValue_pf32 = tmpsetActValue_f32;
                actItfInfo_ps->getActValue_f32 = tmpsetActValue_f32;
            }
            else 
            {
                *f_actValue_pf32 = 0.0f;
                actItfInfo_ps->getActValue_f32 = 0.0f;
            }
        }
    }

    return Ret_e;
}

/*********************************
 * APPACT_SetActValue
 *********************************/
t_eReturnCode APPACT_SetActValue(t_eAPPACT_ActInterface f_actuator_e, t_float32 f_setActValue_f32)
{
    t_eReturnCode Ret_e = RC_OK;
    t_eAPPACT_ActDeviceList actDeviceLink_e;
    t_sAPPACT_ActIfaceInfo * actItfInfo_ps;

    if(g_AppAct_ModState_e != STATE_CYCLIC_OPE)
    {
        Ret_e = RC_WARNING_BUSY;
    }
    else if(f_actuator_e > APPACT_ACTITF_NB)
    {
        Ret_e = RC_ERROR_PARAM_INVALID;
        ASSERT((t_uint16)f_actuator_e);
    }
    else 
    {
        actItfInfo_ps = (t_sAPPACT_ActIfaceInfo *)(&g_ActInterfaceInfo_as[f_actuator_e]);
        actDeviceLink_e = actItfInfo_ps->cfgInfo_ps->deviceLink_e;

        if(g_ActDeviceInfo_as[actDeviceLink_e].isConfigured_b == (t_bool)FALSE)
        {
            Ret_e = RC_ERROR_MISSING_CONFIG;
            ASSERT((t_uint16)0);
        }
        else if(g_isCtrlModeON_b == TRUE)
        {
            Ret_e = RC_WARNING_NOT_ALLOWED;
        }
        else 
        {
            // call specific function to get value
            Ret_e = actItfInfo_ps->cfgInfo_ps->SetValue_pcb(f_setActValue_f32);
            actItfInfo_ps->setActValue_f32 = (t_float32)(f_setActValue_f32);
        }
    }

    return Ret_e;
}
//********************************************************************************
//                      Local functions - Implementation
//********************************************************************************
/*********************************
 * s_APPACT_ConfigurationState
 *********************************/
static t_eReturnCode s_APPACT_ConfigurationState(void)
{
    t_eReturnCode Ret_e = RC_OK;

    switch(g_FsmCfgSts_e)
    {
        case APPACT_FSM_CFGSTS_INIT_DRIVER:
            Ret_e = s_APPACT_Fsm_CfgSts_InitDriver();
            if(Ret_e == RC_OK)
            {
                Ret_e = RC_WARNING_PENDING;
                g_FsmCfgSts_e = APPACT_FSM_CFGSTS_GET_CFG;
            }
            else if(Ret_e > RC_OK)
            {
                Ret_e = RC_WARNING_PENDING;
            }
        break;
        case APPACT_FSM_CFGSTS_GET_CFG:
            Ret_e = s_APPACT_Fsm_CfgSts_GetCfg();
            if(Ret_e == RC_OK)
            {
                Ret_e = RC_WARNING_PENDING;
                g_FsmCfgSts_e = APPACT_FSM_CFGSTS_APPLY_CFG;
            }
            else if(Ret_e > RC_OK)
            {
                Ret_e = RC_WARNING_PENDING;
            }
        break;
        case APPACT_FSM_CFGSTS_APPLY_CFG:
            Ret_e = s_APPACT_Fsm_CfgSts_ApplyCfg();
            if(Ret_e == RC_OK)
            {
                Ret_e = RC_WARNING_PENDING;
                g_FsmCfgSts_e = APPACT_FSM_CFGSTS_SIG_REGISTER;
            }
            else if(Ret_e > RC_OK)
            {
                Ret_e = RC_WARNING_PENDING;
            }
        break;
        case APPACT_FSM_CFGSTS_SIG_REGISTER:
            Ret_e = s_APPACT_Fsm_CfgSts_SigRegister();
            if(Ret_e == RC_OK)
            {
                // Ret_e = RC_OK;  // out of cfg sts
                g_FsmCfgSts_e = APPACT_FSM_CFGSTS_INIT_DRIVER;
            }
        break;
        default:
            Ret_e = RC_ERROR_NOT_ALLOWED;
        break;
    }

    return Ret_e;
}

/*********************************
 * s_APPACT_Fsm_CfgSts_GetCfg
 *********************************/
static t_eReturnCode s_APPACT_Fsm_CfgSts_GetCfg(void)
{
    t_eReturnCode Ret_e;
    t_uint8 idxActevice_u8;
    t_uint8 snsOptVal_u8 = (t_uint8)0;
    t_sAPPACT_ActDvcInfo * snsDvcInfo_ps;

    Ret_e = RC_OK;
    for(idxActevice_u8 = (t_uint8)0 ; 
    (idxActevice_u8 < (t_uint8)APPACT_ACTDVC_NB) && (Ret_e == RC_OK) ; 
    idxActevice_u8++)
    {
        snsDvcInfo_ps = (t_sAPPACT_ActDvcInfo *)(&g_ActDeviceInfo_as[idxActevice_u8]);
        Ret_e = APPSYS_GetSysOption(snsDvcInfo_ps->dvcOpeCfg_ps->sysOptSnsDvc_e,
                                    &snsOptVal_u8);
        if(Ret_e == RC_OK)
        {
            snsDvcInfo_ps->dvcCfg_u8 = (t_uint8)snsOptVal_u8;
        }
    }

    return Ret_e;
}

/*********************************
 * s_APPACT_Fsm_CfgSts_ApplyCfg
 *********************************/
static t_eReturnCode s_APPACT_Fsm_CfgSts_ApplyCfg(void)
{
    t_eReturnCode Ret_e;
    t_sAPPACT_ActDvcInfo * actDeviceInfo_ps;
    t_eAPPACT_ActDriverList drvUsed_e;
    static t_uint8 s_LLACT_u8 = 0;   

    // actuators configuration call
    Ret_e = RC_OK;
    for(; (s_LLACT_u8 < APPACT_ACTDVC_NB) && (Ret_e == RC_OK) ; s_LLACT_u8++)
    {
        drvUsed_e = APPACT_DRV_NB;
        actDeviceInfo_ps = (t_sAPPACT_ActDvcInfo *)(&g_ActDeviceInfo_as[s_LLACT_u8]);
        if(actDeviceInfo_ps->dvcOpeCfg_ps->SetCfg_pcb != NULL_FUNCTION)
        {
            Ret_e = actDeviceInfo_ps->dvcOpeCfg_ps->SetCfg_pcb( actDeviceInfo_ps->dvcCfg_u8, 
                                                                &drvUsed_e);

            if(Ret_e == RC_OK)
            {
                if(drvUsed_e != APPACT_DRV_NB
                && (drvUsed_e < APPACT_DRV_NB))
                {
                    g_ActDrvState_ae[drvUsed_e] = APPACT_DRV_STS_ENABLE;

                    if((c_AppAct_SysDrvCfg_as[drvUsed_e].isFastTaskCyclic_b == (t_bool)TRUE)
                    && (g_enableFastTask_b == (t_bool)FALSE))
                    {
                        g_enableFastTask_b = (t_bool)TRUE;
                    }
                }

                actDeviceInfo_ps->isConfigured_b = (t_bool)TRUE;                
            }
            //---- ok actuators don't use ----//
            else if(Ret_e == RC_WARNING_NO_OPERATION)
            {
                Ret_e = RC_OK;
            }
        }
        else
        {
            Ret_e = RC_ERROR_PTR_NULL;
            ASSERT((t_uint16)s_LLACT_u8);
        }
    }
    if((s_LLACT_u8 < APPACT_ACTDVC_NB)
    && (Ret_e >= RC_OK)) // only if problem has not been captured yet
    {// problem or waiting on init or sensors config just waiting for next cycle
        Ret_e = RC_WARNING_BUSY;
    }

    return Ret_e;
}

/*********************************
 * s_APPACT_Fsm_CfgSts_SigRegister
 *********************************/
static t_eReturnCode s_APPACT_Fsm_CfgSts_SigRegister(void)
{
    t_eReturnCode Ret_e;
    t_uint8 actIfId_u8;
    t_sAPPACT_ActIfaceInfo * actIfInfo_ps = NULL;

    Ret_e = RC_OK;
    for(actIfId_u8 = 0; (actIfId_u8 < APPACT_ACTITF_NB) && (Ret_e == RC_OK) ; actIfId_u8++)
    {
        actIfInfo_ps = &g_ActInterfaceInfo_as[actIfId_u8];

        if(actIfInfo_ps->cfgInfo_ps->SigCtrlDebug_e != APPSIG_SIGNAL_NB)
        {
            Ret_e = APPSIG_AddRcvMsgCallback(   actIfInfo_ps->cfgInfo_ps->SigCtrlDebug_e,
                                                s_APPACT_SigReceptionCallback);
        }
    }

    return Ret_e;
}   
/*********************************
 * s_APPACT_Fsm_CfgSts_InitDriver
 *********************************/
static t_eReturnCode s_APPACT_Fsm_CfgSts_InitDriver(void)
{
    t_eReturnCode Ret_e;
    t_uint8 LLDRV_u8;

    //---- driver init -----//
    Ret_e = RC_OK;
    for(LLDRV_u8 = (t_uint8)0; (LLDRV_u8 < APPACT_DRV_NB) && (Ret_e == RC_OK) ; LLDRV_u8++)
    {
        if(c_AppAct_SysDrvCfg_as[LLDRV_u8].Init_pcb != NULL_FUNCTION)
        {
            Ret_e = (c_AppAct_SysDrvCfg_as[LLDRV_u8].Init_pcb)();
        }        
    }

    return Ret_e;
}
/*********************************
 * s_APPACT_PreOperational
 *********************************/
static t_eReturnCode s_APPACT_PreOperational(void)
{
    t_eReturnCode Ret_e;
    if(g_enableFastTask_b == (t_bool)TRUE)
    {
        Ret_e = APPSYS_SetFastTaskState(APPSYS_MODULE_APP_ACT, APPSYS_FAST_TASK_ENABLE);
    }
    else 
    {
        Ret_e = RC_OK;
    }

    return Ret_e;
}
/*********************************
 * s_APPACT_Operational
 *********************************/
static t_eReturnCode s_APPACT_Operational(void)
{
    t_eReturnCode Ret_e = RC_OK;
    t_uint8 LLDRV_u8; 

    for(LLDRV_u8 = (t_uint8)0 ; (LLDRV_u8 < APPACT_DRV_NB); LLDRV_u8++)
    {
        if((c_AppAct_SysDrvCfg_as[LLDRV_u8].Cyclic_pcb != NULL_FUNCTION)
        && (c_AppAct_SysDrvCfg_as[LLDRV_u8].isFastTaskCyclic_b == FALSE))
        {
            Ret_e = (c_AppAct_SysDrvCfg_as[LLDRV_u8].Cyclic_pcb)();
        }
        if(Ret_e < RC_OK)
        {
            ASSERT((t_uint16)LLDRV_u8);
            ASSERT((t_uint16)Ret_e);
        }
    }

    s_APPACT_DebugRoutine();

    return Ret_e;
}

/*********************************
 * s_APPACT_Operational
 *********************************/
static void s_APPACT_FastTask(void)
{
    t_eReturnCode Ret_e = RC_OK;
    t_uint8 LLDRV_u8; 

    for(LLDRV_u8 = (t_uint8)0 ; (LLDRV_u8 < APPACT_DRV_NB) ; LLDRV_u8++)
    {
        if((c_AppAct_SysDrvCfg_as[LLDRV_u8].Cyclic_pcb != NULL_FUNCTION)
        && (c_AppAct_SysDrvCfg_as[LLDRV_u8].isFastTaskCyclic_b == TRUE))
        {
            Ret_e = (c_AppAct_SysDrvCfg_as[LLDRV_u8].Cyclic_pcb)();
        }
        if(Ret_e < RC_OK)
        {
            ASSERT((t_uint16)LLDRV_u8);
            ASSERT((t_uint16)Ret_e);
        }
    }

    return;
}

/*********************************
 * s_APPACT_DebugRoutine
 *********************************/
static void s_APPACT_DebugRoutine(void)
{
    t_eReturnCode Ret_e;
    t_sint32 idxActIf_s32;
    t_sAPPACT_ActIfaceInfo * actIfInfo_ps;

    for(idxActIf_s32 = 0 ; idxActIf_s32 < APPACT_ACTITF_NB ; idxActIf_s32 ++)
    {
        actIfInfo_ps = &g_ActInterfaceInfo_as[idxActIf_s32];
        if(actIfInfo_ps->cfgInfo_ps->SigSetDebug_e < APPSIG_SIGNAL_NB)
        {
            Ret_e = APPSIG_SetSignalValue(  actIfInfo_ps->cfgInfo_ps->SigSetDebug_e, 
                                            actIfInfo_ps->setActValue_f32);
            if(Ret_e != RC_OK)
            {
                ASSERT((t_uint16)Ret_e);
            }
        }
        if(actIfInfo_ps->cfgInfo_ps->SigGetDebug_e < APPSIG_SIGNAL_NB)
        {
            Ret_e = APPSIG_SetSignalValue(  actIfInfo_ps->cfgInfo_ps->SigGetDebug_e, 
                                            actIfInfo_ps->getActValue_f32);
            if(Ret_e != RC_OK)
            {
                ASSERT((t_uint16)Ret_e);
            }
        }
    }

    return;
}

/*********************************
 * s_APPACT_SigReceptionCallback
 *********************************/
static void s_APPACT_SigReceptionCallback(t_eAPPSIG_Signal f_signal_e, t_float32 f_sigVal_f32)
{
    t_eReturnCode Ret_e;
    t_uint8 actIfId_u8 = 0;
    t_sint16 tmpValue_s16;
    t_sAPPACT_ActIfaceInfo * actIfInfo_ps = NULL;

    //---- First update control mode ----//
    if(g_isCtrlModeON_b == FALSE)
    {
        g_isCtrlModeON_b = TRUE;
    }

    //---- Find out the actuator interface link to the signal ---//
    for(actIfId_u8 = 0 ; actIfId_u8 < APPACT_ACTITF_NB ; actIfId_u8++)
    {
        actIfInfo_ps = &g_ActInterfaceInfo_as[actIfId_u8];

        if(actIfInfo_ps->cfgInfo_ps->SigCtrlDebug_e == f_signal_e)
        {
            //---- value is a signed 16 bits in SignalViewer ----//
            tmpValue_s16 = (t_sint16)f_sigVal_f32;
            //---- call specifiic function link ----//
            Ret_e = actIfInfo_ps->cfgInfo_ps->SetValue_pcb((t_float32)tmpValue_s16);
            if(Ret_e < RC_OK)
            {
                ASSERT((t_uint16)Ret_e);
            }
            else 
            {
                actIfInfo_ps->setActValue_f32 = f_sigVal_f32;
            }
        }
    }

    return;
}
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

