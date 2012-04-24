/* Include files */

#include "blascompat32.h"
#include "BHclientT_sfun.h"
#include "c2_BHclientT.h"
#include "mwmathutil.h"
#define CHARTINSTANCE_CHARTNUMBER      (chartInstance.chartNumber)
#define CHARTINSTANCE_INSTANCENUMBER   (chartInstance.instanceNumber)
#include "BHclientT_sfun_debug_macros.h"

/* Type Definitions */

/* Named Constants */
#define c2_IN_NO_ACTIVE_CHILD          (0)

/* Variable Declarations */

/* Variable Definitions */
static SFc2_BHclientTInstanceStruct chartInstance;

/* Function Declarations */
static void initialize_c2_BHclientT(void);
static void initialize_params_c2_BHclientT(void);
static void enable_c2_BHclientT(void);
static void disable_c2_BHclientT(void);
static void c2_update_debugger_state_c2_BHclientT(void);
static const mxArray *get_sim_state_c2_BHclientT(void);
static void set_sim_state_c2_BHclientT(const mxArray *c2_st);
static void finalize_c2_BHclientT(void);
static void sf_c2_BHclientT(void);
static void init_script_number_translation(uint32_T c2_machineNumber, uint32_T
  c2_chartNumber);
static const mxArray *c2_sf_marshall(void *c2_chartInstance, void *c2_u);
static void c2_info_helper(c2_ResolvedFunctionInfo c2_info[45]);
static const mxArray *c2_b_sf_marshall(void *c2_chartInstance, void *c2_u);
static void init_dsm_address_info(void);

/* Function Definitions */
static void initialize_c2_BHclientT(void)
{
  _sfTime_ = (real_T)ssGetT(chartInstance.S);
  chartInstance.c2_is_active_c2_BHclientT = 0U;
}

static void initialize_params_c2_BHclientT(void)
{
}

static void enable_c2_BHclientT(void)
{
  _sfTime_ = (real_T)ssGetT(chartInstance.S);
}

static void disable_c2_BHclientT(void)
{
  _sfTime_ = (real_T)ssGetT(chartInstance.S);
}

static void c2_update_debugger_state_c2_BHclientT(void)
{
}

static const mxArray *get_sim_state_c2_BHclientT(void)
{
  const mxArray *c2_st = NULL;
  const mxArray *c2_y = NULL;
  real_T c2_u;
  const mxArray *c2_b_y = NULL;
  real_T c2_b_u;
  const mxArray *c2_c_y = NULL;
  uint8_T c2_c_u;
  const mxArray *c2_d_y = NULL;
  real_T *c2_Fl;
  real_T *c2_tau;
  c2_Fl = (real_T *)ssGetOutputPortSignal(chartInstance.S, 2);
  c2_tau = (real_T *)ssGetOutputPortSignal(chartInstance.S, 1);
  c2_st = NULL;
  c2_y = NULL;
  sf_mex_assign(&c2_y, sf_mex_createcellarray(3));
  c2_u = *c2_Fl;
  c2_b_y = NULL;
  sf_mex_assign(&c2_b_y, sf_mex_create("y", &c2_u, 0, 0U, 0U, 0U, 0));
  sf_mex_setcell(c2_y, 0, c2_b_y);
  c2_b_u = *c2_tau;
  c2_c_y = NULL;
  sf_mex_assign(&c2_c_y, sf_mex_create("y", &c2_b_u, 0, 0U, 0U, 0U, 0));
  sf_mex_setcell(c2_y, 1, c2_c_y);
  c2_c_u = chartInstance.c2_is_active_c2_BHclientT;
  c2_d_y = NULL;
  sf_mex_assign(&c2_d_y, sf_mex_create("y", &c2_c_u, 3, 0U, 0U, 0U, 0));
  sf_mex_setcell(c2_y, 2, c2_d_y);
  sf_mex_assign(&c2_st, c2_y);
  return c2_st;
}

static void set_sim_state_c2_BHclientT(const mxArray *c2_st)
{
  const mxArray *c2_u;
  const mxArray *c2_Fl;
  real_T c2_d0;
  real_T c2_y;
  const mxArray *c2_b_Fl;
  real_T c2_d1;
  real_T c2_b_y;
  const mxArray *c2_b_is_active_c2_BHclientT;
  uint8_T c2_u0;
  uint8_T c2_c_y;
  real_T *c2_c_Fl;
  real_T *c2_tau;
  c2_c_Fl = (real_T *)ssGetOutputPortSignal(chartInstance.S, 2);
  c2_tau = (real_T *)ssGetOutputPortSignal(chartInstance.S, 1);
  chartInstance.c2_doneDoubleBufferReInit = true;
  c2_u = sf_mex_dup(c2_st);
  c2_Fl = sf_mex_dup(sf_mex_getcell(c2_u, 0));
  sf_mex_import("Fl", sf_mex_dup(c2_Fl), &c2_d0, 1, 0, 0U, 0, 0U, 0);
  c2_y = c2_d0;
  sf_mex_destroy(&c2_Fl);
  *c2_c_Fl = c2_y;
  c2_b_Fl = sf_mex_dup(sf_mex_getcell(c2_u, 1));
  sf_mex_import("tau", sf_mex_dup(c2_b_Fl), &c2_d1, 1, 0, 0U, 0, 0U, 0);
  c2_b_y = c2_d1;
  sf_mex_destroy(&c2_b_Fl);
  *c2_tau = c2_b_y;
  c2_b_is_active_c2_BHclientT = sf_mex_dup(sf_mex_getcell(c2_u, 2));
  sf_mex_import("is_active_c2_BHclientT", sf_mex_dup(c2_b_is_active_c2_BHclientT),
                &c2_u0, 1, 3, 0U, 0, 0U, 0);
  c2_c_y = c2_u0;
  sf_mex_destroy(&c2_b_is_active_c2_BHclientT);
  chartInstance.c2_is_active_c2_BHclientT = c2_c_y;
  sf_mex_destroy(&c2_u);
  c2_update_debugger_state_c2_BHclientT();
  sf_mex_destroy(&c2_st);
}

static void finalize_c2_BHclientT(void)
{
}

static void sf_c2_BHclientT(void)
{
  int32_T c2_previousEvent;
  real_T c2_alfa;
  real_T c2_q;
  real_T c2_dq;
  real_T c2_Fe;
  real_T c2_nargout = 2.0;
  real_T c2_nargin = 4.0;
  real_T c2_Kfe;
  real_T c2_g_q;
  real_T c2_F;
  real_T c2_C_q_dq;
  real_T c2_B_q;
  real_T c2_Fl;
  real_T c2_tau;
  real_T c2_b_dq;
  real_T c2_b_alfa;
  real_T c2_b_nargout = 1.0;
  real_T c2_b_nargin = 2.0;
  real_T c2_gauss;
  real_T c2_KFl;
  real_T c2_sig;
  real_T c2_b;
  real_T c2_A;
  real_T c2_x;
  real_T c2_b_x;
  real_T c2_c_x;
  real_T c2_a;
  real_T c2_ak;
  real_T c2_b_b;
  real_T c2_d_x;
  real_T c2_c_b;
  real_T c2_e_x;
  real_T c2_d_b;
  real_T c2_b_a;
  real_T c2_f_x;
  real_T c2_e_b;
  real_T c2_g_x;
  real_T c2_h_x;
  boolean_T c2_f_b;
  real_T c2_g_b;
  real_T c2_y;
  real_T c2_h_b;
  real_T c2_b_y;
  real_T c2_i_b;
  real_T c2_c_y;
  real_T c2_j_b;
  real_T c2_d_y;
  real_T *c2_b_Fl;
  real_T *c2_b_tau;
  real_T *c2_b_Fe;
  real_T *c2_c_dq;
  real_T *c2_b_q;
  real_T *c2_c_alfa;
  c2_c_alfa = (real_T *)ssGetInputPortSignal(chartInstance.S, 0);
  c2_b_Fe = (real_T *)ssGetInputPortSignal(chartInstance.S, 3);
  c2_c_dq = (real_T *)ssGetInputPortSignal(chartInstance.S, 2);
  c2_b_Fl = (real_T *)ssGetOutputPortSignal(chartInstance.S, 2);
  c2_b_q = (real_T *)ssGetInputPortSignal(chartInstance.S, 1);
  c2_b_tau = (real_T *)ssGetOutputPortSignal(chartInstance.S, 1);
  _sfTime_ = (real_T)ssGetT(chartInstance.S);
  _SFD_CC_CALL(CHART_ENTER_SFUNCTION_TAG,0);
  _SFD_DATA_RANGE_CHECK(*c2_c_alfa, 0U);
  _SFD_DATA_RANGE_CHECK(*c2_b_q, 1U);
  _SFD_DATA_RANGE_CHECK(*c2_c_dq, 2U);
  _SFD_DATA_RANGE_CHECK(*c2_b_Fe, 3U);
  _SFD_DATA_RANGE_CHECK(*c2_b_tau, 4U);
  _SFD_DATA_RANGE_CHECK(*c2_b_Fl, 5U);
  c2_previousEvent = _sfEvent_;
  _sfEvent_ = CALL_EVENT;
  _SFD_CC_CALL(CHART_ENTER_DURING_FUNCTION_TAG,0);
  c2_alfa = *c2_c_alfa;
  c2_q = *c2_b_q;
  c2_dq = *c2_c_dq;
  c2_Fe = *c2_b_Fe;
  sf_debug_symbol_scope_push(13U, 0U);
  sf_debug_symbol_scope_add("nargout", &c2_nargout, c2_sf_marshall);
  sf_debug_symbol_scope_add("nargin", &c2_nargin, c2_sf_marshall);
  sf_debug_symbol_scope_add("Kfe", &c2_Kfe, c2_sf_marshall);
  sf_debug_symbol_scope_add("g_q", &c2_g_q, c2_sf_marshall);
  sf_debug_symbol_scope_add("F", &c2_F, c2_sf_marshall);
  sf_debug_symbol_scope_add("C_q_dq", &c2_C_q_dq, c2_sf_marshall);
  sf_debug_symbol_scope_add("B_q", &c2_B_q, c2_sf_marshall);
  sf_debug_symbol_scope_add("Fl", &c2_Fl, c2_sf_marshall);
  sf_debug_symbol_scope_add("tau", &c2_tau, c2_sf_marshall);
  sf_debug_symbol_scope_add("Fe", &c2_Fe, c2_sf_marshall);
  sf_debug_symbol_scope_add("dq", &c2_dq, c2_sf_marshall);
  sf_debug_symbol_scope_add("q", &c2_q, c2_sf_marshall);
  sf_debug_symbol_scope_add("alfa", &c2_alfa, c2_sf_marshall);
  CV_EML_FCN(0, 0);
  _SFD_EML_CALL(0,3);
  c2_B_q = 1.0;
  _SFD_EML_CALL(0,4);
  c2_C_q_dq = 0.0;
  _SFD_EML_CALL(0,5);
  c2_F = 0.1;
  _SFD_EML_CALL(0,6);
  c2_b_dq = c2_dq;
  c2_b_alfa = c2_alfa;
  sf_debug_symbol_scope_push(8U, 0U);
  sf_debug_symbol_scope_add("nargout", &c2_b_nargout, c2_sf_marshall);
  sf_debug_symbol_scope_add("nargin", &c2_b_nargin, c2_sf_marshall);
  sf_debug_symbol_scope_add("gauss", &c2_gauss, c2_sf_marshall);
  sf_debug_symbol_scope_add("KFl", &c2_KFl, c2_sf_marshall);
  sf_debug_symbol_scope_add("sig", &c2_sig, c2_sf_marshall);
  sf_debug_symbol_scope_add("Fl", &c2_b, c2_sf_marshall);
  sf_debug_symbol_scope_add("alfa", &c2_b_alfa, c2_sf_marshall);
  sf_debug_symbol_scope_add("dq", &c2_b_dq, c2_sf_marshall);
  CV_EML_FCN(0, 1);

  /*  environment */
  _SFD_EML_CALL(0,18);
  c2_sig = 0.4;
  _SFD_EML_CALL(0,19);
  c2_KFl = 1.0;
  _SFD_EML_CALL(0,21);
  c2_A = c2_b_dq;
  c2_x = c2_A;
  c2_b_x = c2_x;
  c2_c_x = c2_b_x;
  c2_a = c2_c_x / 0.4;
  c2_ak = c2_a;
  c2_b_b = muDoubleScalarPower(c2_ak, 2.0);
  c2_d_x = -4.0 * c2_b_b;
  c2_c_b = c2_d_x;
  c2_e_x = c2_c_b;
  c2_c_b = c2_e_x;
  c2_c_b = muDoubleScalarExp(c2_c_b);
  c2_gauss = 9.9735570100358173E-001 * c2_c_b;
  _SFD_EML_CALL(0,23);
  c2_d_b = c2_gauss;
  c2_b_a = c2_d_b;
  c2_f_x = c2_b_alfa;
  c2_e_b = c2_f_x;
  c2_g_x = c2_e_b;
  c2_e_b = c2_g_x;
  c2_h_x = c2_e_b;
  c2_f_b = rtIsNaN(c2_h_x);
  if (c2_f_b) {
    c2_e_b = rtNaN;
  } else if (c2_e_b > 0.0) {
    c2_e_b = 1.0;
  } else if (c2_e_b < 0.0) {
    c2_e_b = -1.0;
  } else {
    c2_e_b = 0.0;
  }

  c2_b = c2_b_a * c2_e_b;
  _SFD_EML_CALL(0,-23);
  sf_debug_symbol_scope_pop();
  c2_Fl = 300.0 * c2_b;
  _SFD_EML_CALL(0,7);
  c2_g_q = 0.0;
  _SFD_EML_CALL(0,8);
  c2_Kfe = 0.0;

  /*     % inertia */
  /*    % coriollis */
  /*         % friction */
  /*            % static friction */
  /*             % gravity */
  _SFD_EML_CALL(0,10);
  c2_g_b = c2_Fe;
  c2_y = 0.0 * c2_g_b;
  c2_h_b = c2_dq;
  c2_b_y = 0.1 * c2_h_b;
  c2_i_b = c2_dq;
  c2_c_y = 0.0 * c2_i_b;
  c2_j_b = c2_alfa;
  c2_d_y = c2_j_b;
  c2_tau = ((((c2_d_y + c2_c_y) + c2_b_y) + c2_Fl) + c2_g_q) + c2_y;
  _SFD_EML_CALL(0,-10);
  sf_debug_symbol_scope_pop();
  *c2_b_tau = c2_tau;
  *c2_b_Fl = c2_Fl;
  _SFD_CC_CALL(EXIT_OUT_OF_FUNCTION_TAG,0);
  _sfEvent_ = c2_previousEvent;
  sf_debug_check_for_state_inconsistency(_BHclientTMachineNumber_,
    chartInstance.chartNumber, chartInstance.instanceNumber);
}

static void init_script_number_translation(uint32_T c2_machineNumber, uint32_T
  c2_chartNumber)
{
}

static const mxArray *c2_sf_marshall(void *c2_chartInstance, void *c2_u)
{
  const mxArray *c2_y = NULL;
  real_T c2_b_u;
  const mxArray *c2_b_y = NULL;
  c2_y = NULL;
  c2_b_u = *((real_T *)c2_u);
  c2_b_y = NULL;
  sf_mex_assign(&c2_b_y, sf_mex_create("y", &c2_b_u, 0, 0U, 0U, 0U, 0));
  sf_mex_assign(&c2_y, c2_b_y);
  return c2_y;
}

const mxArray *sf_c2_BHclientT_get_eml_resolved_functions_info(void)
{
  const mxArray *c2_nameCaptureInfo = NULL;
  c2_ResolvedFunctionInfo c2_info[45];
  const mxArray *c2_m0 = NULL;
  int32_T c2_i0;
  c2_ResolvedFunctionInfo *c2_r0;
  c2_nameCaptureInfo = NULL;
  c2_info_helper(c2_info);
  sf_mex_assign(&c2_m0, sf_mex_createstruct("nameCaptureInfo", 1, 45));
  for (c2_i0 = 0; c2_i0 < 45; c2_i0 = c2_i0 + 1) {
    c2_r0 = &c2_info[c2_i0];
    sf_mex_addfield(c2_m0, sf_mex_create("nameCaptureInfo", c2_r0->context, 15,
      0U, 0U, 0U, 2, 1, strlen(c2_r0->context)), "context",
                    "nameCaptureInfo", c2_i0);
    sf_mex_addfield(c2_m0, sf_mex_create("nameCaptureInfo", c2_r0->name, 15, 0U,
      0U, 0U, 2, 1, strlen(c2_r0->name)), "name",
                    "nameCaptureInfo", c2_i0);
    sf_mex_addfield(c2_m0, sf_mex_create("nameCaptureInfo", c2_r0->dominantType,
      15, 0U, 0U, 0U, 2, 1, strlen(c2_r0->dominantType)),
                    "dominantType", "nameCaptureInfo", c2_i0);
    sf_mex_addfield(c2_m0, sf_mex_create("nameCaptureInfo", c2_r0->resolved, 15,
      0U, 0U, 0U, 2, 1, strlen(c2_r0->resolved)), "resolved"
                    , "nameCaptureInfo", c2_i0);
    sf_mex_addfield(c2_m0, sf_mex_create("nameCaptureInfo", &c2_r0->fileLength,
      7, 0U, 0U, 0U, 0), "fileLength", "nameCaptureInfo",
                    c2_i0);
    sf_mex_addfield(c2_m0, sf_mex_create("nameCaptureInfo", &c2_r0->fileTime1, 7,
      0U, 0U, 0U, 0), "fileTime1", "nameCaptureInfo", c2_i0);
    sf_mex_addfield(c2_m0, sf_mex_create("nameCaptureInfo", &c2_r0->fileTime2, 7,
      0U, 0U, 0U, 0), "fileTime2", "nameCaptureInfo", c2_i0);
  }

  sf_mex_assign(&c2_nameCaptureInfo, c2_m0);
  return c2_nameCaptureInfo;
}

static void c2_info_helper(c2_ResolvedFunctionInfo c2_info[45])
{
  c2_info[0].context = "";
  c2_info[0].name = "pi";
  c2_info[0].dominantType = "";
  c2_info[0].resolved = "[B]pi";
  c2_info[0].fileLength = 0U;
  c2_info[0].fileTime1 = 0U;
  c2_info[0].fileTime2 = 0U;
  c2_info[1].context = "";
  c2_info[1].name = "mtimes";
  c2_info[1].dominantType = "double";
  c2_info[1].resolved = "[ILX]$matlabroot$/toolbox/eml/lib/matlab/ops/mtimes.m";
  c2_info[1].fileLength = 3302U;
  c2_info[1].fileTime1 = 1242750894U;
  c2_info[1].fileTime2 = 0U;
  c2_info[2].context = "[ILX]$matlabroot$/toolbox/eml/lib/matlab/ops/mtimes.m";
  c2_info[2].name = "nargin";
  c2_info[2].dominantType = "";
  c2_info[2].resolved = "[B]nargin";
  c2_info[2].fileLength = 0U;
  c2_info[2].fileTime1 = 0U;
  c2_info[2].fileTime2 = 0U;
  c2_info[3].context = "[ILX]$matlabroot$/toolbox/eml/lib/matlab/ops/mtimes.m";
  c2_info[3].name = "gt";
  c2_info[3].dominantType = "double";
  c2_info[3].resolved = "[B]gt";
  c2_info[3].fileLength = 0U;
  c2_info[3].fileTime1 = 0U;
  c2_info[3].fileTime2 = 0U;
  c2_info[4].context = "[ILX]$matlabroot$/toolbox/eml/lib/matlab/ops/mtimes.m";
  c2_info[4].name = "isa";
  c2_info[4].dominantType = "double";
  c2_info[4].resolved = "[B]isa";
  c2_info[4].fileLength = 0U;
  c2_info[4].fileTime1 = 0U;
  c2_info[4].fileTime2 = 0U;
  c2_info[5].context = "[ILX]$matlabroot$/toolbox/eml/lib/matlab/ops/mtimes.m";
  c2_info[5].name = "isinteger";
  c2_info[5].dominantType = "double";
  c2_info[5].resolved = "[B]isinteger";
  c2_info[5].fileLength = 0U;
  c2_info[5].fileTime1 = 0U;
  c2_info[5].fileTime2 = 0U;
  c2_info[6].context = "[ILX]$matlabroot$/toolbox/eml/lib/matlab/ops/mtimes.m";
  c2_info[6].name = "isscalar";
  c2_info[6].dominantType = "double";
  c2_info[6].resolved = "[B]isscalar";
  c2_info[6].fileLength = 0U;
  c2_info[6].fileTime1 = 0U;
  c2_info[6].fileTime2 = 0U;
  c2_info[7].context = "[ILX]$matlabroot$/toolbox/eml/lib/matlab/ops/mtimes.m";
  c2_info[7].name = "strcmp";
  c2_info[7].dominantType = "char";
  c2_info[7].resolved = "[B]strcmp";
  c2_info[7].fileLength = 0U;
  c2_info[7].fileTime1 = 0U;
  c2_info[7].fileTime2 = 0U;
  c2_info[8].context = "[ILX]$matlabroot$/toolbox/eml/lib/matlab/ops/mtimes.m";
  c2_info[8].name = "size";
  c2_info[8].dominantType = "double";
  c2_info[8].resolved = "[B]size";
  c2_info[8].fileLength = 0U;
  c2_info[8].fileTime1 = 0U;
  c2_info[8].fileTime2 = 0U;
  c2_info[9].context = "[ILX]$matlabroot$/toolbox/eml/lib/matlab/ops/mtimes.m";
  c2_info[9].name = "eq";
  c2_info[9].dominantType = "double";
  c2_info[9].resolved = "[B]eq";
  c2_info[9].fileLength = 0U;
  c2_info[9].fileTime1 = 0U;
  c2_info[9].fileTime2 = 0U;
  c2_info[10].context = "[ILX]$matlabroot$/toolbox/eml/lib/matlab/ops/mtimes.m";
  c2_info[10].name = "class";
  c2_info[10].dominantType = "double";
  c2_info[10].resolved = "[B]class";
  c2_info[10].fileLength = 0U;
  c2_info[10].fileTime1 = 0U;
  c2_info[10].fileTime2 = 0U;
  c2_info[11].context = "[ILX]$matlabroot$/toolbox/eml/lib/matlab/ops/mtimes.m";
  c2_info[11].name = "not";
  c2_info[11].dominantType = "logical";
  c2_info[11].resolved = "[B]not";
  c2_info[11].fileLength = 0U;
  c2_info[11].fileTime1 = 0U;
  c2_info[11].fileTime2 = 0U;
  c2_info[12].context = "";
  c2_info[12].name = "sqrt";
  c2_info[12].dominantType = "double";
  c2_info[12].resolved = "[ILX]$matlabroot$/toolbox/eml/lib/matlab/elfun/sqrt.m";
  c2_info[12].fileLength = 572U;
  c2_info[12].fileTime1 = 1203448046U;
  c2_info[12].fileTime2 = 0U;
  c2_info[13].context = "[ILX]$matlabroot$/toolbox/eml/lib/matlab/elfun/sqrt.m";
  c2_info[13].name = "isreal";
  c2_info[13].dominantType = "double";
  c2_info[13].resolved = "[B]isreal";
  c2_info[13].fileLength = 0U;
  c2_info[13].fileTime1 = 0U;
  c2_info[13].fileTime2 = 0U;
  c2_info[14].context = "[ILX]$matlabroot$/toolbox/eml/lib/matlab/elfun/sqrt.m";
  c2_info[14].name = "lt";
  c2_info[14].dominantType = "double";
  c2_info[14].resolved = "[B]lt";
  c2_info[14].fileLength = 0U;
  c2_info[14].fileTime1 = 0U;
  c2_info[14].fileTime2 = 0U;
  c2_info[15].context = "[ILX]$matlabroot$/toolbox/eml/lib/matlab/elfun/sqrt.m";
  c2_info[15].name = "eml_error";
  c2_info[15].dominantType = "char";
  c2_info[15].resolved =
    "[ILX]$matlabroot$/toolbox/eml/lib/matlab/eml/eml_error.m";
  c2_info[15].fileLength = 315U;
  c2_info[15].fileTime1 = 1213926746U;
  c2_info[15].fileTime2 = 0U;
  c2_info[16].context = "[ILX]$matlabroot$/toolbox/eml/lib/matlab/elfun/sqrt.m";
  c2_info[16].name = "eml_scalar_sqrt";
  c2_info[16].dominantType = "double";
  c2_info[16].resolved =
    "[ILX]$matlabroot$/toolbox/eml/lib/matlab/elfun/eml_scalar_sqrt.m";
  c2_info[16].fileLength = 664U;
  c2_info[16].fileTime1 = 1209330794U;
  c2_info[16].fileTime2 = 0U;
  c2_info[17].context = "";
  c2_info[17].name = "mrdivide";
  c2_info[17].dominantType = "double";
  c2_info[17].resolved =
    "[ILX]$matlabroot$/toolbox/eml/lib/matlab/ops/mrdivide.m";
  c2_info[17].fileLength = 800U;
  c2_info[17].fileTime1 = 1238434292U;
  c2_info[17].fileTime2 = 0U;
  c2_info[18].context =
    "[ILX]$matlabroot$/toolbox/eml/lib/matlab/ops/mrdivide.m";
  c2_info[18].name = "ge";
  c2_info[18].dominantType = "double";
  c2_info[18].resolved = "[B]ge";
  c2_info[18].fileLength = 0U;
  c2_info[18].fileTime1 = 0U;
  c2_info[18].fileTime2 = 0U;
  c2_info[19].context =
    "[ILX]$matlabroot$/toolbox/eml/lib/matlab/ops/mrdivide.m";
  c2_info[19].name = "rdivide";
  c2_info[19].dominantType = "double";
  c2_info[19].resolved =
    "[ILX]$matlabroot$/toolbox/eml/lib/matlab/ops/rdivide.m";
  c2_info[19].fileLength = 620U;
  c2_info[19].fileTime1 = 1213926766U;
  c2_info[19].fileTime2 = 0U;
  c2_info[20].context = "[ILX]$matlabroot$/toolbox/eml/lib/matlab/ops/rdivide.m";
  c2_info[20].name = "isempty";
  c2_info[20].dominantType = "double";
  c2_info[20].resolved = "[B]isempty";
  c2_info[20].fileLength = 0U;
  c2_info[20].fileTime1 = 0U;
  c2_info[20].fileTime2 = 0U;
  c2_info[21].context = "[ILX]$matlabroot$/toolbox/eml/lib/matlab/ops/rdivide.m";
  c2_info[21].name = "eml_warning";
  c2_info[21].dominantType = "char";
  c2_info[21].resolved =
    "[ILX]$matlabroot$/toolbox/eml/lib/matlab/eml/eml_warning.m";
  c2_info[21].fileLength = 262U;
  c2_info[21].fileTime1 = 1236257278U;
  c2_info[21].fileTime2 = 0U;
  c2_info[22].context = "[ILX]$matlabroot$/toolbox/eml/lib/matlab/ops/rdivide.m";
  c2_info[22].name = "eml_div";
  c2_info[22].dominantType = "double";
  c2_info[22].resolved =
    "[ILX]$matlabroot$/toolbox/eml/lib/matlab/eml/eml_div.m";
  c2_info[22].fileLength = 4269U;
  c2_info[22].fileTime1 = 1228093826U;
  c2_info[22].fileTime2 = 0U;
  c2_info[23].context = "";
  c2_info[23].name = "uminus";
  c2_info[23].dominantType = "double";
  c2_info[23].resolved = "[B]uminus";
  c2_info[23].fileLength = 0U;
  c2_info[23].fileTime1 = 0U;
  c2_info[23].fileTime2 = 0U;
  c2_info[24].context = "";
  c2_info[24].name = "power";
  c2_info[24].dominantType = "double";
  c2_info[24].resolved = "[ILX]$matlabroot$/toolbox/eml/lib/matlab/ops/power.m";
  c2_info[24].fileLength = 5380U;
  c2_info[24].fileTime1 = 1228093898U;
  c2_info[24].fileTime2 = 0U;
  c2_info[25].context = "[ILX]$matlabroot$/toolbox/eml/lib/matlab/ops/power.m";
  c2_info[25].name = "eml_scalar_eg";
  c2_info[25].dominantType = "double";
  c2_info[25].resolved =
    "[ILX]$matlabroot$/toolbox/eml/lib/matlab/eml/eml_scalar_eg.m";
  c2_info[25].fileLength = 3068U;
  c2_info[25].fileTime1 = 1240262010U;
  c2_info[25].fileTime2 = 0U;
  c2_info[26].context =
    "[ILX]$matlabroot$/toolbox/eml/lib/matlab/eml/eml_scalar_eg.m/any_enums";
  c2_info[26].name = "false";
  c2_info[26].dominantType = "";
  c2_info[26].resolved = "[B]false";
  c2_info[26].fileLength = 0U;
  c2_info[26].fileTime1 = 0U;
  c2_info[26].fileTime2 = 0U;
  c2_info[27].context =
    "[ILX]$matlabroot$/toolbox/eml/lib/matlab/eml/eml_scalar_eg.m";
  c2_info[27].name = "isstruct";
  c2_info[27].dominantType = "double";
  c2_info[27].resolved = "[B]isstruct";
  c2_info[27].fileLength = 0U;
  c2_info[27].fileTime1 = 0U;
  c2_info[27].fileTime2 = 0U;
  c2_info[28].context =
    "[ILX]$matlabroot$/toolbox/eml/lib/matlab/eml/eml_scalar_eg.m/zerosum";
  c2_info[28].name = "cast";
  c2_info[28].dominantType = "double";
  c2_info[28].resolved = "[B]cast";
  c2_info[28].fileLength = 0U;
  c2_info[28].fileTime1 = 0U;
  c2_info[28].fileTime2 = 0U;
  c2_info[29].context =
    "[ILX]$matlabroot$/toolbox/eml/lib/matlab/eml/eml_scalar_eg.m/zerosum";
  c2_info[29].name = "plus";
  c2_info[29].dominantType = "double";
  c2_info[29].resolved = "[B]plus";
  c2_info[29].fileLength = 0U;
  c2_info[29].fileTime1 = 0U;
  c2_info[29].fileTime2 = 0U;
  c2_info[30].context = "[ILX]$matlabroot$/toolbox/eml/lib/matlab/ops/power.m";
  c2_info[30].name = "eml_scalexp_alloc";
  c2_info[30].dominantType = "double";
  c2_info[30].resolved =
    "[ILX]$matlabroot$/toolbox/eml/lib/matlab/eml/eml_scalexp_alloc.m";
  c2_info[30].fileLength = 808U;
  c2_info[30].fileTime1 = 1230494700U;
  c2_info[30].fileTime2 = 0U;
  c2_info[31].context =
    "[ILX]$matlabroot$/toolbox/eml/lib/matlab/eml/eml_scalexp_alloc.m";
  c2_info[31].name = "minus";
  c2_info[31].dominantType = "double";
  c2_info[31].resolved = "[B]minus";
  c2_info[31].fileLength = 0U;
  c2_info[31].fileTime1 = 0U;
  c2_info[31].fileTime2 = 0U;
  c2_info[32].context = "[ILX]$matlabroot$/toolbox/eml/lib/matlab/ops/power.m";
  c2_info[32].name = "eml_scalar_floor";
  c2_info[32].dominantType = "double";
  c2_info[32].resolved =
    "[ILX]$matlabroot$/toolbox/eml/lib/matlab/elfun/eml_scalar_floor.m";
  c2_info[32].fileLength = 260U;
  c2_info[32].fileTime1 = 1209330790U;
  c2_info[32].fileTime2 = 0U;
  c2_info[33].context = "[ILX]$matlabroot$/toolbox/eml/lib/matlab/ops/power.m";
  c2_info[33].name = "ne";
  c2_info[33].dominantType = "double";
  c2_info[33].resolved = "[B]ne";
  c2_info[33].fileLength = 0U;
  c2_info[33].fileTime1 = 0U;
  c2_info[33].fileTime2 = 0U;
  c2_info[34].context = "";
  c2_info[34].name = "exp";
  c2_info[34].dominantType = "double";
  c2_info[34].resolved = "[ILX]$matlabroot$/toolbox/eml/lib/matlab/elfun/exp.m";
  c2_info[34].fileLength = 324U;
  c2_info[34].fileTime1 = 1240261974U;
  c2_info[34].fileTime2 = 0U;
  c2_info[35].context = "[ILX]$matlabroot$/toolbox/eml/lib/matlab/elfun/exp.m";
  c2_info[35].name = "eml_scalar_exp";
  c2_info[35].dominantType = "double";
  c2_info[35].resolved =
    "[ILX]$matlabroot$/toolbox/eml/lib/matlab/elfun/eml_scalar_exp.m";
  c2_info[35].fileLength = 243U;
  c2_info[35].fileTime1 = 1240261972U;
  c2_info[35].fileTime2 = 0U;
  c2_info[36].context = "";
  c2_info[36].name = "sign";
  c2_info[36].dominantType = "double";
  c2_info[36].resolved = "[ILX]$matlabroot$/toolbox/eml/lib/matlab/elfun/sign.m";
  c2_info[36].fileLength = 408U;
  c2_info[36].fileTime1 = 1203448042U;
  c2_info[36].fileTime2 = 0U;
  c2_info[37].context = "[ILX]$matlabroot$/toolbox/eml/lib/matlab/elfun/sign.m";
  c2_info[37].name = "eml_scalar_sign";
  c2_info[37].dominantType = "double";
  c2_info[37].resolved =
    "[ILX]$matlabroot$/toolbox/eml/lib/matlab/elfun/eml_scalar_sign.m";
  c2_info[37].fileLength = 543U;
  c2_info[37].fileTime1 = 1203448012U;
  c2_info[37].fileTime2 = 0U;
  c2_info[38].context =
    "[ILX]$matlabroot$/toolbox/eml/lib/matlab/elfun/eml_scalar_sign.m";
  c2_info[38].name = "isnan";
  c2_info[38].dominantType = "double";
  c2_info[38].resolved =
    "[ILX]$matlabroot$/toolbox/eml/lib/matlab/elmat/isnan.m";
  c2_info[38].fileLength = 506U;
  c2_info[38].fileTime1 = 1228093810U;
  c2_info[38].fileTime2 = 0U;
  c2_info[39].context =
    "[ILX]$matlabroot$/toolbox/eml/lib/matlab/elfun/eml_scalar_sign.m";
  c2_info[39].name = "eml_guarded_nan";
  c2_info[39].dominantType = "char";
  c2_info[39].resolved =
    "[ILX]$matlabroot$/toolbox/eml/lib/matlab/eml/eml_guarded_nan.m";
  c2_info[39].fileLength = 485U;
  c2_info[39].fileTime1 = 1192466780U;
  c2_info[39].fileTime2 = 0U;
  c2_info[40].context =
    "[ILX]$matlabroot$/toolbox/eml/lib/matlab/eml/eml_guarded_nan.m";
  c2_info[40].name = "eml_is_float_class";
  c2_info[40].dominantType = "char";
  c2_info[40].resolved =
    "[ILX]$matlabroot$/toolbox/eml/lib/matlab/eml/eml_is_float_class.m";
  c2_info[40].fileLength = 226U;
  c2_info[40].fileTime1 = 1197850442U;
  c2_info[40].fileTime2 = 0U;
  c2_info[41].context =
    "[ILX]$matlabroot$/toolbox/eml/lib/matlab/eml/eml_is_float_class.m";
  c2_info[41].name = "ischar";
  c2_info[41].dominantType = "char";
  c2_info[41].resolved = "[B]ischar";
  c2_info[41].fileLength = 0U;
  c2_info[41].fileTime1 = 0U;
  c2_info[41].fileTime2 = 0U;
  c2_info[42].context =
    "[ILX]$matlabroot$/toolbox/eml/lib/matlab/eml/eml_guarded_nan.m";
  c2_info[42].name = "nan";
  c2_info[42].dominantType = "char";
  c2_info[42].resolved = "[B]nan";
  c2_info[42].fileLength = 0U;
  c2_info[42].fileTime1 = 0U;
  c2_info[42].fileTime2 = 0U;
  c2_info[43].context =
    "[ILX]$matlabroot$/toolbox/eml/lib/matlab/elfun/eml_scalar_sign.m";
  c2_info[43].name = "ones";
  c2_info[43].dominantType = "char";
  c2_info[43].resolved = "[B]ones";
  c2_info[43].fileLength = 0U;
  c2_info[43].fileTime1 = 0U;
  c2_info[43].fileTime2 = 0U;
  c2_info[44].context =
    "[ILX]$matlabroot$/toolbox/eml/lib/matlab/elfun/eml_scalar_sign.m";
  c2_info[44].name = "zeros";
  c2_info[44].dominantType = "char";
  c2_info[44].resolved = "[B]zeros";
  c2_info[44].fileLength = 0U;
  c2_info[44].fileTime1 = 0U;
  c2_info[44].fileTime2 = 0U;
}

static const mxArray *c2_b_sf_marshall(void *c2_chartInstance, void *c2_u)
{
  const mxArray *c2_y = NULL;
  boolean_T c2_b_u;
  const mxArray *c2_b_y = NULL;
  c2_y = NULL;
  c2_b_u = *((boolean_T *)c2_u);
  c2_b_y = NULL;
  sf_mex_assign(&c2_b_y, sf_mex_create("y", &c2_b_u, 11, 0U, 0U, 0U, 0));
  sf_mex_assign(&c2_y, c2_b_y);
  return c2_y;
}

static void init_dsm_address_info(void)
{
}

/* SFunction Glue Code */
void sf_c2_BHclientT_get_check_sum(mxArray *plhs[])
{
  ((real_T *)mxGetPr((plhs[0])))[0] = (real_T)(1266066U);
  ((real_T *)mxGetPr((plhs[0])))[1] = (real_T)(1377351808U);
  ((real_T *)mxGetPr((plhs[0])))[2] = (real_T)(4275432520U);
  ((real_T *)mxGetPr((plhs[0])))[3] = (real_T)(1583922349U);
}

mxArray *sf_c2_BHclientT_get_autoinheritance_info(void)
{
  const char *autoinheritanceFields[] = { "checksum", "inputs", "parameters",
    "outputs" };

  mxArray *mxAutoinheritanceInfo = mxCreateStructMatrix(1,1,4,
    autoinheritanceFields);

  {
    mxArray *mxChecksum = mxCreateDoubleMatrix(4,1,mxREAL);
    double *pr = mxGetPr(mxChecksum);
    pr[0] = (double)(910849554U);
    pr[1] = (double)(2510568895U);
    pr[2] = (double)(1679371031U);
    pr[3] = (double)(1568498591U);
    mxSetField(mxAutoinheritanceInfo,0,"checksum",mxChecksum);
  }

  {
    const char *dataFields[] = { "size", "type", "complexity" };

    mxArray *mxData = mxCreateStructMatrix(1,4,3,dataFields);

    {
      mxArray *mxSize = mxCreateDoubleMatrix(1,2,mxREAL);
      double *pr = mxGetPr(mxSize);
      pr[0] = (double)(1);
      pr[1] = (double)(1);
      mxSetField(mxData,0,"size",mxSize);
    }

    {
      const char *typeFields[] = { "base", "fixpt" };

      mxArray *mxType = mxCreateStructMatrix(1,1,2,typeFields);
      mxSetField(mxType,0,"base",mxCreateDoubleScalar(10));
      mxSetField(mxType,0,"fixpt",mxCreateDoubleMatrix(0,0,mxREAL));
      mxSetField(mxData,0,"type",mxType);
    }

    mxSetField(mxData,0,"complexity",mxCreateDoubleScalar(0));

    {
      mxArray *mxSize = mxCreateDoubleMatrix(1,2,mxREAL);
      double *pr = mxGetPr(mxSize);
      pr[0] = (double)(1);
      pr[1] = (double)(1);
      mxSetField(mxData,1,"size",mxSize);
    }

    {
      const char *typeFields[] = { "base", "fixpt" };

      mxArray *mxType = mxCreateStructMatrix(1,1,2,typeFields);
      mxSetField(mxType,0,"base",mxCreateDoubleScalar(10));
      mxSetField(mxType,0,"fixpt",mxCreateDoubleMatrix(0,0,mxREAL));
      mxSetField(mxData,1,"type",mxType);
    }

    mxSetField(mxData,1,"complexity",mxCreateDoubleScalar(0));

    {
      mxArray *mxSize = mxCreateDoubleMatrix(1,2,mxREAL);
      double *pr = mxGetPr(mxSize);
      pr[0] = (double)(1);
      pr[1] = (double)(1);
      mxSetField(mxData,2,"size",mxSize);
    }

    {
      const char *typeFields[] = { "base", "fixpt" };

      mxArray *mxType = mxCreateStructMatrix(1,1,2,typeFields);
      mxSetField(mxType,0,"base",mxCreateDoubleScalar(10));
      mxSetField(mxType,0,"fixpt",mxCreateDoubleMatrix(0,0,mxREAL));
      mxSetField(mxData,2,"type",mxType);
    }

    mxSetField(mxData,2,"complexity",mxCreateDoubleScalar(0));

    {
      mxArray *mxSize = mxCreateDoubleMatrix(1,2,mxREAL);
      double *pr = mxGetPr(mxSize);
      pr[0] = (double)(1);
      pr[1] = (double)(1);
      mxSetField(mxData,3,"size",mxSize);
    }

    {
      const char *typeFields[] = { "base", "fixpt" };

      mxArray *mxType = mxCreateStructMatrix(1,1,2,typeFields);
      mxSetField(mxType,0,"base",mxCreateDoubleScalar(10));
      mxSetField(mxType,0,"fixpt",mxCreateDoubleMatrix(0,0,mxREAL));
      mxSetField(mxData,3,"type",mxType);
    }

    mxSetField(mxData,3,"complexity",mxCreateDoubleScalar(0));
    mxSetField(mxAutoinheritanceInfo,0,"inputs",mxData);
  }

  {
    mxSetField(mxAutoinheritanceInfo,0,"parameters",mxCreateDoubleMatrix(0,0,
                mxREAL));
  }

  {
    const char *dataFields[] = { "size", "type", "complexity" };

    mxArray *mxData = mxCreateStructMatrix(1,2,3,dataFields);

    {
      mxArray *mxSize = mxCreateDoubleMatrix(1,2,mxREAL);
      double *pr = mxGetPr(mxSize);
      pr[0] = (double)(1);
      pr[1] = (double)(1);
      mxSetField(mxData,0,"size",mxSize);
    }

    {
      const char *typeFields[] = { "base", "fixpt" };

      mxArray *mxType = mxCreateStructMatrix(1,1,2,typeFields);
      mxSetField(mxType,0,"base",mxCreateDoubleScalar(10));
      mxSetField(mxType,0,"fixpt",mxCreateDoubleMatrix(0,0,mxREAL));
      mxSetField(mxData,0,"type",mxType);
    }

    mxSetField(mxData,0,"complexity",mxCreateDoubleScalar(0));

    {
      mxArray *mxSize = mxCreateDoubleMatrix(1,2,mxREAL);
      double *pr = mxGetPr(mxSize);
      pr[0] = (double)(1);
      pr[1] = (double)(1);
      mxSetField(mxData,1,"size",mxSize);
    }

    {
      const char *typeFields[] = { "base", "fixpt" };

      mxArray *mxType = mxCreateStructMatrix(1,1,2,typeFields);
      mxSetField(mxType,0,"base",mxCreateDoubleScalar(10));
      mxSetField(mxType,0,"fixpt",mxCreateDoubleMatrix(0,0,mxREAL));
      mxSetField(mxData,1,"type",mxType);
    }

    mxSetField(mxData,1,"complexity",mxCreateDoubleScalar(0));
    mxSetField(mxAutoinheritanceInfo,0,"outputs",mxData);
  }

  return(mxAutoinheritanceInfo);
}

static mxArray *sf_get_sim_state_info_c2_BHclientT(void)
{
  const char *infoFields[] = { "chartChecksum", "varInfo" };

  mxArray *mxInfo = mxCreateStructMatrix(1, 1, 2, infoFields);
  char *infoEncStr[] = {
    "100 S1x3'type','srcId','name','auxInfo'{{M[1],M[9],T\"Fl\",},{M[1],M[5],T\"tau\",},{M[8],M[0],T\"is_active_c2_BHclientT\",}}"
  };

  mxArray *mxVarInfo = sf_mex_decode_encoded_mx_struct_array(infoEncStr, 3, 10);
  mxArray *mxChecksum = mxCreateDoubleMatrix(1, 4, mxREAL);
  sf_c2_BHclientT_get_check_sum(&mxChecksum);
  mxSetField(mxInfo, 0, infoFields[0], mxChecksum);
  mxSetField(mxInfo, 0, infoFields[1], mxVarInfo);
  return mxInfo;
}

static void chart_debug_initialization(SimStruct *S, unsigned int
  fullDebuggerInitialization)
{
  if (!sim_mode_is_rtw_gen(S)) {
    if (ssIsFirstInitCond(S) && fullDebuggerInitialization==1) {
      /* do this only if simulation is starting */
      {
        unsigned int chartAlreadyPresent;
        chartAlreadyPresent = sf_debug_initialize_chart(_BHclientTMachineNumber_,
          2,
          1,
          1,
          6,
          0,
          0,
          0,
          0,
          0,
          &(chartInstance.chartNumber),
          &(chartInstance.instanceNumber),
          ssGetPath(S),
          (void *)S);
        if (chartAlreadyPresent==0) {
          /* this is the first instance */
          init_script_number_translation(_BHclientTMachineNumber_,
            chartInstance.chartNumber);
          sf_debug_set_chart_disable_implicit_casting(_BHclientTMachineNumber_,
            chartInstance.chartNumber,1);
          sf_debug_set_chart_event_thresholds(_BHclientTMachineNumber_,
            chartInstance.chartNumber,
            0,
            0,
            0);
          _SFD_SET_DATA_PROPS(0,1,1,0,SF_DOUBLE,0,NULL,0,0,0,0.0,1.0,0,"alfa",0,
                              (MexFcnForType)c2_sf_marshall);
          _SFD_SET_DATA_PROPS(1,1,1,0,SF_DOUBLE,0,NULL,0,0,0,0.0,1.0,0,"q",0,
                              (MexFcnForType)c2_sf_marshall);
          _SFD_SET_DATA_PROPS(2,1,1,0,SF_DOUBLE,0,NULL,0,0,0,0.0,1.0,0,"dq",0,
                              (MexFcnForType)c2_sf_marshall);
          _SFD_SET_DATA_PROPS(3,1,1,0,SF_DOUBLE,0,NULL,0,0,0,0.0,1.0,0,"Fe",0,
                              (MexFcnForType)c2_sf_marshall);
          _SFD_SET_DATA_PROPS(4,2,0,1,SF_DOUBLE,0,NULL,0,0,0,0.0,1.0,0,"tau",0,
                              (MexFcnForType)c2_sf_marshall);
          _SFD_SET_DATA_PROPS(5,2,0,1,SF_DOUBLE,0,NULL,0,0,0,0.0,1.0,0,"Fl",0,
                              (MexFcnForType)c2_sf_marshall);
          _SFD_STATE_INFO(0,0,2);
          _SFD_CH_SUBSTATE_COUNT(0);
          _SFD_CH_SUBSTATE_DECOMP(0);
        }

        _SFD_CV_INIT_CHART(0,0,0,0);

        {
          _SFD_CV_INIT_STATE(0,0,0,0,0,0,NULL,NULL);
        }

        _SFD_CV_INIT_TRANS(0,0,NULL,NULL,0,NULL);

        /* Initialization of EML Model Coverage */
        _SFD_CV_INIT_EML(0,2,0,0,0,0,0,0);
        _SFD_CV_INIT_EML_FCN(0,0,"eML_blk_kernel",0,-1,346);
        _SFD_CV_INIT_EML_FCN(0,1,"StaticFrictionKoef",346,-1,474);
        _SFD_TRANS_COV_WTS(0,0,0,1,0);
        if (chartAlreadyPresent==0) {
          _SFD_TRANS_COV_MAPS(0,
                              0,NULL,NULL,
                              0,NULL,NULL,
                              1,NULL,NULL,
                              0,NULL,NULL);
        }

        {
          real_T *c2_alfa;
          real_T *c2_q;
          real_T *c2_dq;
          real_T *c2_Fe;
          real_T *c2_tau;
          real_T *c2_Fl;
          c2_alfa = (real_T *)ssGetInputPortSignal(chartInstance.S, 0);
          c2_Fe = (real_T *)ssGetInputPortSignal(chartInstance.S, 3);
          c2_dq = (real_T *)ssGetInputPortSignal(chartInstance.S, 2);
          c2_Fl = (real_T *)ssGetOutputPortSignal(chartInstance.S, 2);
          c2_q = (real_T *)ssGetInputPortSignal(chartInstance.S, 1);
          c2_tau = (real_T *)ssGetOutputPortSignal(chartInstance.S, 1);
          _SFD_SET_DATA_VALUE_PTR(0U, c2_alfa);
          _SFD_SET_DATA_VALUE_PTR(1U, c2_q);
          _SFD_SET_DATA_VALUE_PTR(2U, c2_dq);
          _SFD_SET_DATA_VALUE_PTR(3U, c2_Fe);
          _SFD_SET_DATA_VALUE_PTR(4U, c2_tau);
          _SFD_SET_DATA_VALUE_PTR(5U, c2_Fl);
        }
      }
    } else {
      sf_debug_reset_current_state_configuration(_BHclientTMachineNumber_,
        chartInstance.chartNumber,chartInstance.instanceNumber);
    }
  }
}

static void sf_opaque_initialize_c2_BHclientT(void *chartInstanceVar)
{
  chart_debug_initialization(chartInstance.S,0);
  initialize_params_c2_BHclientT();
  initialize_c2_BHclientT();
}

static void sf_opaque_enable_c2_BHclientT(void *chartInstanceVar)
{
  enable_c2_BHclientT();
}

static void sf_opaque_disable_c2_BHclientT(void *chartInstanceVar)
{
  disable_c2_BHclientT();
}

static void sf_opaque_gateway_c2_BHclientT(void *chartInstanceVar)
{
  sf_c2_BHclientT();
}

static mxArray* sf_opaque_get_sim_state_c2_BHclientT(void *chartInstanceVar)
{
  mxArray *st = (mxArray *) get_sim_state_c2_BHclientT();
  return st;
}

static void sf_opaque_set_sim_state_c2_BHclientT(void *chartInstanceVar, const
  mxArray *st)
{
  set_sim_state_c2_BHclientT(sf_mex_dup(st));
}

static void sf_opaque_terminate_c2_BHclientT(void *chartInstanceVar)
{
  if (sim_mode_is_rtw_gen(chartInstance.S) || sim_mode_is_external
      (chartInstance.S)) {
    sf_clear_rtw_identifier(chartInstance.S);
  }

  finalize_c2_BHclientT();
}

extern unsigned int sf_machine_global_initializer_called(void);
static void mdlProcessParameters_c2_BHclientT(SimStruct *S)
{
  int i;
  for (i=0;i<ssGetNumRunTimeParams(S);i++) {
    if (ssGetSFcnParamTunable(S,i)) {
      ssUpdateDlgParamAsRunTimeParam(S,i);
    }
  }

  if (sf_machine_global_initializer_called()) {
    initialize_params_c2_BHclientT();
  }
}

static void mdlSetWorkWidths_c2_BHclientT(SimStruct *S)
{
  if (sim_mode_is_rtw_gen(S) || sim_mode_is_external(S)) {
    int_T chartIsInlinable =
      (int_T)sf_is_chart_inlinable("BHclientT","BHclientT",2);
    ssSetStateflowIsInlinable(S,chartIsInlinable);
    ssSetRTWCG(S,sf_rtw_info_uint_prop("BHclientT","BHclientT",2,"RTWCG"));
    ssSetEnableFcnIsTrivial(S,1);
    ssSetDisableFcnIsTrivial(S,1);
    ssSetNotMultipleInlinable(S,sf_rtw_info_uint_prop("BHclientT","BHclientT",2,
      "gatewayCannotBeInlinedMultipleTimes"));
    if (chartIsInlinable) {
      ssSetInputPortOptimOpts(S, 0, SS_REUSABLE_AND_LOCAL);
      ssSetInputPortOptimOpts(S, 1, SS_REUSABLE_AND_LOCAL);
      ssSetInputPortOptimOpts(S, 2, SS_REUSABLE_AND_LOCAL);
      ssSetInputPortOptimOpts(S, 3, SS_REUSABLE_AND_LOCAL);
      sf_mark_chart_expressionable_inputs(S,"BHclientT","BHclientT",2,4);
      sf_mark_chart_reusable_outputs(S,"BHclientT","BHclientT",2,2);
    }

    sf_set_rtw_dwork_info(S,"BHclientT","BHclientT",2);
    ssSetHasSubFunctions(S,!(chartIsInlinable));
    ssSetOptions(S,ssGetOptions(S)|SS_OPTION_WORKS_WITH_CODE_REUSE);
  }

  ssSetChecksum0(S,(701499513U));
  ssSetChecksum1(S,(141719738U));
  ssSetChecksum2(S,(3193878490U));
  ssSetChecksum3(S,(3797385505U));
  ssSetmdlDerivatives(S, NULL);
  ssSetExplicitFCSSCtrl(S,1);
}

static void mdlRTW_c2_BHclientT(SimStruct *S)
{
  if (sim_mode_is_rtw_gen(S)) {
    sf_write_symbol_mapping(S, "BHclientT", "BHclientT",2);
    ssWriteRTWStrParam(S, "StateflowChartType", "Embedded MATLAB");
  }
}

static void mdlStart_c2_BHclientT(SimStruct *S)
{
  chartInstance.chartInfo.chartInstance = NULL;
  chartInstance.chartInfo.isEMLChart = 1;
  chartInstance.chartInfo.chartInitialized = 0;
  chartInstance.chartInfo.sFunctionGateway = sf_opaque_gateway_c2_BHclientT;
  chartInstance.chartInfo.initializeChart = sf_opaque_initialize_c2_BHclientT;
  chartInstance.chartInfo.terminateChart = sf_opaque_terminate_c2_BHclientT;
  chartInstance.chartInfo.enableChart = sf_opaque_enable_c2_BHclientT;
  chartInstance.chartInfo.disableChart = sf_opaque_disable_c2_BHclientT;
  chartInstance.chartInfo.getSimState = sf_opaque_get_sim_state_c2_BHclientT;
  chartInstance.chartInfo.setSimState = sf_opaque_set_sim_state_c2_BHclientT;
  chartInstance.chartInfo.getSimStateInfo = sf_get_sim_state_info_c2_BHclientT;
  chartInstance.chartInfo.zeroCrossings = NULL;
  chartInstance.chartInfo.outputs = NULL;
  chartInstance.chartInfo.derivatives = NULL;
  chartInstance.chartInfo.mdlRTW = mdlRTW_c2_BHclientT;
  chartInstance.chartInfo.mdlStart = mdlStart_c2_BHclientT;
  chartInstance.chartInfo.mdlSetWorkWidths = mdlSetWorkWidths_c2_BHclientT;
  chartInstance.chartInfo.extModeExec = NULL;
  chartInstance.chartInfo.restoreLastMajorStepConfiguration = NULL;
  chartInstance.chartInfo.restoreBeforeLastMajorStepConfiguration = NULL;
  chartInstance.chartInfo.storeCurrentConfiguration = NULL;
  chartInstance.S = S;
  ssSetUserData(S,(void *)(&(chartInstance.chartInfo)));/* register the chart instance with simstruct */
  if (!sim_mode_is_rtw_gen(S)) {
    init_dsm_address_info();
  }

  chart_debug_initialization(S,1);
}

void c2_BHclientT_method_dispatcher(SimStruct *S, int_T method, void *data)
{
  switch (method) {
   case SS_CALL_MDL_START:
    mdlStart_c2_BHclientT(S);
    break;

   case SS_CALL_MDL_SET_WORK_WIDTHS:
    mdlSetWorkWidths_c2_BHclientT(S);
    break;

   case SS_CALL_MDL_PROCESS_PARAMETERS:
    mdlProcessParameters_c2_BHclientT(S);
    break;

   default:
    /* Unhandled method */
    sf_mex_error_message("Stateflow Internal Error:\n"
                         "Error calling c2_BHclientT_method_dispatcher.\n"
                         "Can't handle method %d.\n", method);
    break;
  }
}
